// PX2EU_ResTree.cpp

#include "PX2EU_ResTree.hpp"
#include "PX2Dir.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2Edit.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2LanguageManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, EU_ResTree);
PX2_IMPLEMENT_STREAM(EU_ResTree);
PX2_IMPLEMENT_FACTORY(EU_ResTree);

//----------------------------------------------------------------------------
EU_ResTree::EU_ResTree(ResTreeType rtt):
mResTreeType(rtt)
{
	//mOpenData = AddItem(mRootItem, PX2_LM_EDITOR.GetValue("OpenedData"), "OpenedData");
	//mOpenData->SetName("OpenedData");
	//mOpenData->GetFText()->GetText()->SetDrawStyle(FD_SHADOW);

	if (RTT_DIR == mResTreeType)
	{
		ShowRootItem(false);
		mDataItem = AddItem(mRootItem, "Data", "Data");
		mDataItem->SetName("Data");
		mDataItem->SetUserData("path", std::string("Data/"));
		mDataItem->GetFText()->GetText()->SetDrawStyle(FD_SHADOW);
		RefreshItems(mDataItem, RT_DIR, true);
	}

	mCloudData = AddItem(mRootItem, "Cloud", "Cloud");
	mCloudData->SetName("Cloud");
	mCloudData->GetFText()->GetText()->SetDrawStyle(FD_SHADOW);

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
EU_ResTree::~EU_ResTree()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_ResTreeCallback(UIFrame *frame, UICallType type)
{
	UICheckButton *checkBut = DynamicCast<UICheckButton>(frame);
	if (checkBut)
	{
		UIItem *item = checkBut->GetUserData<UIItem*>("Item");

		if (UICT_CHECKED == type)
		{
			item->Expand(false);
		}
		else if (UICT_DISCHECKED == type)
		{
			item->Expand(true);
		}
	}
}
//----------------------------------------------------------------------------
UIItem *EU_ResTree::AddItem(UIItem *parentItem, const std::string &label,
	const std::string &name, Object *obj)
{
	UIItem *item = UITree::AddItem(parentItem, label, name, obj);
	
	UICheckButton *cb = parentItem->GetButArrow();
	if (!cb)
	{
		cb = parentItem->CreateButArrow();

		cb->AddUICallback(EU_ResTreeCallback);
		cb->SetUserData("Item", parentItem);

		cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
			"DataNIRVANA2/images/icons/tree/tree_expanded.png");
		cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
			"DataNIRVANA2/images/icons/tree/tree_collapsed.png");
	}

	mMaskFrame->SetNeedAdjustChildrenMask(true);

	return item;
}
//----------------------------------------------------------------------------
void EU_ResTree::OnSelected(UIItem *item, bool isDouble)
{
	UITree::OnSelected(item, isDouble);

	if (item)
	{
		const std::string &path = item->GetUserData<std::string>("path");
		if (path.empty()) return;

		if (RTT_DIR == mResTreeType)
		{
			PX2_EDIT.ChangeSelectResDir(path, Edit::CDT_TREE_REFRESH);

			SelectResData srd;
			srd.ResPathname = path;
			PX2_EDIT.SetSelectedResource(srd);
		}
	}
}
//----------------------------------------------------------------------------
void EU_ResTree::OnEvent(Event *event)
{
	if (EditorEventSpace::IsEqual(event, EditorEventSpace::RefreshRes))
	{
		if (RTT_DIR == mResTreeType)
		{
			ClearItems();
			RefreshItems(mDataItem, RT_DIR, true);
		}
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::UpRes))
	{
		UIItem *item = GetSelectedItem();
		if (item)
		{
			UIItem *parentItem = DynamicCast<UIItem>(item->GetParent());
			if (parentItem && !parentItem->IsRootItem())
			{
				ClearAllSelectItems(true);
				AddSelectItem(parentItem, false, true);
			}
		}
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::DownRes))
	{

	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::ChangeResDir))
	{
		Edit::ChangeDirType type = event->GetData<Edit::ChangeDirType>();
		if (Edit::CDT_GRID_REFRESH == type)
		{
			const std::string &selectedResDir = PX2_EDIT.GetSelectedResDir();
			if (!selectedResDir.empty())
			{
				UIItem *itemParent = GetSelectedItem();
				UIItem *item = _GetChildItemByPath(itemParent, selectedResDir);
				if (item)
				{
					ClearAllSelectItems(true);
					AddSelectItem(item, false, true);
				}
			}
		}
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::OpenContentRes))
	{
		//std::string path = event->GetData<std::string>();

		//std::string outPath;
		//std::string outBaseName;
		//std::string outExtention;
		//StringHelp::SplitFullFilename(path, outPath, outBaseName, outExtention);
		//std::string filename = outBaseName + "." + outExtention;

		//UIItem *itemChild = AddItem(mOpenData, filename, path);
		//itemChild->SetName(path);
		//itemChild->SetUserData("path", path);
		//itemChild->SetUserData("filename", filename);
	}
	else if (EditorEventSpace::IsEqual(event, EditorEventSpace::RemoveContentRes))
	{
		//std::string path = event->GetData<std::string>();
		//mOpenData->RemoveItem(path);
	}
}
//----------------------------------------------------------------------------
void EU_ResTree::ClearItems()
{
	mDataItem->RemoveAllChildItems();
}
//----------------------------------------------------------------------------
void EU_ResTree::RefreshItems(UIItem *parent, RefreshType type, bool isExpand)
{
	std::string path = parent->GetUserData<std::string>("path");
	if (path.empty())
		return;

	std::string name;
	std::string compareStr = "Data/";
	if (path == compareStr)
	{
		name = "Data";
	}
	else
	{
		name = path.substr(compareStr.length(),
			path.length() - compareStr.length() - 1);
	}

	Dir d;
	std::string eachFilename;
	if (d.Open(path))
	{
		if (!d.HasFiles() && !d.HasSubDirs())
			return;

		int flags = 0;
		if (type == RT_DIR)
			flags = Dir::DIR_DIRS;
		else if (type == RT_DIR_FILES)
			flags = Dir::DIR_FILES;
		else if (type == RT_DIR_ALL || RT_DIR_ALL_NOCHILDREN)
			flags = Dir::DIR_DIRS | Dir::DIR_FILES;

		if (d.GetFirst(&eachFilename, "", flags))
		{
			do 
			{
				if ((eachFilename != ".") && (eachFilename != ".."))
				{
					std::string fileName = eachFilename;
					std::string nextPath = path + fileName;
					std::string nextPath1 = nextPath + "/";

					UIItem *itemChild = AddItem(parent, fileName, fileName);
					itemChild->SetName(fileName);
					itemChild->SetUserData("path", nextPath1);
					itemChild->SetUserData("filename", nextPath);
				}

			} while (d.GetNext(&eachFilename));
		}
	}

	if (type != RT_DIR_FILES && type != RT_DIR_ALL_NOCHILDREN)
	{
		for (int i = 0; i < parent->GetNumChildren(); i++)
		{
			UIItem *childItem = DynamicCast<UIItem>(parent->GetChild(i));
			if (childItem)
			{
				RefreshItems(childItem, type, false);
			}
		}
	}

	if (parent)
	{
		parent->Expand(isExpand);
	}
}
//----------------------------------------------------------------------------
bool EU_ResTree::_IsAFile(const std::string &filename)
{
	if (filename.find(".") != std::string::npos)
		return true;

	return false;
}
//----------------------------------------------------------------------------
UIItem *EU_ResTree::_GetChildItemByPath(UIItem *itemParent,
	const std::string &path)
{
	int numChildItem = itemParent->GetNumChildItem();
	for (int i = 0; i < numChildItem; i++)
	{
		UIItem *childItem = itemParent->GetChildItemByIndex(i);
		if (childItem && path == childItem->GetUserData<std::string>("path"))
		{
			return childItem;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void EU_ResTree::OnSizeNodePicked(const CanvasInputData &inputData)
{
	UITree::OnSizeNodePicked(inputData);

	if (UIPT_RELEASED == inputData.PickType)
	{
		if (CanvasInputData::MT_RIGHT == inputData.TheMouseTag)
		{

		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// �־û�֧��
//----------------------------------------------------------------------------
EU_ResTree::EU_ResTree(LoadConstructor value) :
UITree(value)
{
}
//----------------------------------------------------------------------------
void EU_ResTree::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UITree::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ResTree, source);
}
//----------------------------------------------------------------------------
void EU_ResTree::Link(InStream& source)
{
	UITree::Link(source);
}
//----------------------------------------------------------------------------
void EU_ResTree::PostLink()
{
	UITree::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ResTree::Register(OutStream& target) const
{
	if (UITree::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ResTree::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UITree::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ResTree, target);
}
//----------------------------------------------------------------------------
int EU_ResTree::GetStreamingSize(Stream &stream) const
{
	int size = UITree::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------