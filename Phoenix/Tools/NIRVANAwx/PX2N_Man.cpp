// PX2N_Man.cpp

#include "PX2N_Man.hpp"
#include "PX2N_Frame.hpp"
#include "PX2Edit.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Selection.hpp"
#include "PX2Creater.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2NirvanaToLua.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2LuaPlusContext.hpp"
using namespace PX2;

BEGIN_EVENT_TABLE(NirMan, wxEvtHandler)
END_EVENT_TABLE();
//----------------------------------------------------------------------------
NirMan::NirMan() :
mCurMenu(0),
mCurToolBar(0)
{
}
//----------------------------------------------------------------------------
NirMan::~NirMan()
{
}
//----------------------------------------------------------------------------
bool NirMan::Initlize()
{
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;

	// Lua
	tolua_Nirvana_open((lua_State*)luaContext->GetLuaState());
	LuaPlus::LuaState *luaPlusState = luaContext->GetLuaPlusState();
	PX2_UNUSED(luaPlusState);

	luaContext->SetUserTypePointer("NirMan", "NirMan", this);

	return true;
}
//----------------------------------------------------------------------------
bool NirMan::Ternamate()
{
	return true;
}
//----------------------------------------------------------------------------
void NirMan::SetCurMenu(wxMenu *menu)
{
	mCurMenu = menu;
}
//----------------------------------------------------------------------------
wxMenu *NirMan::GetCurMenu()
{
	return mCurMenu;
}
//----------------------------------------------------------------------------
wxMenu *NirMan::AddSubMenu(wxMenu *menu, const std::string &title)
{
	wxMenu *subMenu = new wxMenu();
	menu->AppendSubMenu(subMenu, title);

	return subMenu;
}
//----------------------------------------------------------------------------
wxMenuItem *NirMan::AddMenuItem(wxMenu *menu, const std::string &title,
	const std::string &script, const std::string &scriptParam)
{
	return N_Frame::MainFrame->AddMenuItem(menu, title, script, scriptParam);
}
//----------------------------------------------------------------------------
void NirMan::AddSeparater(wxMenu *menu)
{
	menu->AppendSeparator();
}
//----------------------------------------------------------------------------
void NirMan::RefreshEffectableControllerModules(wxMenu *menu,
	EffectableController *eftCtrl)
{
	Effectable *ea = DynamicCast<Effectable>(eftCtrl->GetControlledable());

	const std::vector<std::string> &namesEA = ea->GetAllModuleNames_EA();
	const std::vector<std::string> &namesEO = ea->GetAllModuleNames_EO();

	for (int i = 0; i < (int)namesEA.size(); i++)
	{
		std::string createStr = "e_CreateEffectableControllerModule";
		AddMenuItem(menu, namesEA[i], createStr, namesEA[i]);
	}

	menu->AppendSeparator();

	for (int i = 0; i < (int)namesEO.size(); i++)
	{
		std::string createStr = "e_CreateEffectableControllerModule";		
		AddMenuItem(menu, namesEO[i], createStr, namesEO[i]);
	}
}
//----------------------------------------------------------------------------
void NirMan::CreateEffectableControllerModule(const std::string &typeStr)
{
	EffectableController *eftCtrl = DynamicCast<EffectableController>(
		PX2_SELECTM_E->GetFirstObject());
	if (eftCtrl && !eftCtrl->IsHasModuleByTypeName(typeStr))
	{
		EffectModule *module = EffectModule::CreateModule(typeStr);
		PX2_CREATER.AddObject(eftCtrl, module);
	}
}
//----------------------------------------------------------------------------
void NirMan::SetCurToolBar(PX2wxAuiToolBar *toolBar)
{
	mCurToolBar = toolBar;
}
//----------------------------------------------------------------------------
PX2wxAuiToolBar *NirMan::GetCurToolBar()
{
	return mCurToolBar;
}
//----------------------------------------------------------------------------
void NirMan::AddTool(const std::string &icon, std::string &script,
	const std::string &helpStr, int type)
{
	if (!mCurToolBar) return;

	N_Frame::MainFrame->AddTool(mCurToolBar, icon, script,
		helpStr, type);
}
//----------------------------------------------------------------------------
void NirMan::AddToolChoose(const std::string &script,
	const std::string &choose0,
	const std::string &choose1,
	const std::string &choose2,
	const std::string &choose3,
	const std::string &choose4)
{
	N_Frame::MainFrame->AddToolChoose(mCurToolBar, script, choose0,
		choose1, choose2, choose3, choose4);
}
//----------------------------------------------------------------------------
void NirMan::AddToolSeparater()
{
	if (!mCurToolBar) return;

	N_Frame::MainFrame->AddToolSeparater(mCurToolBar);
}
//----------------------------------------------------------------------------
void NirMan::AddToolStretch()
{
	if (!mCurToolBar) return;

	N_Frame::MainFrame->AddToolStretch(mCurToolBar);
}
//----------------------------------------------------------------------------
int NirMan::MessageBox(const std::string &caption, const std::string &content,
	int type)
{
	int theWxType = wxOK;
	if (0 == type)
		theWxType = wxOK;
	else if (1 == type)
		theWxType = wxYES_NO;

	return wxMessageBox(content, caption, theWxType);
}
//----------------------------------------------------------------------------