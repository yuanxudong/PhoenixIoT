// PX2EngineUI.hpp

#ifndef PX2ENGINECANVAS_HPP
#define PX2ENGINECANVAS_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2EngineSceneCanvas.hpp"
#include "PX2EngineUICanvas.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIFText.hpp"
#include "PX2UIButton.hpp"
#include "PX2UICollapsePanel.hpp"
#include "PX2UIList.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EngineCanvas : public Canvas,
		public Singleton<EngineCanvas>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EngineCanvas);

	public:
		EngineCanvas();
		virtual ~EngineCanvas();
		
		virtual void SetScreenRect(const Rectf &rect);
		virtual void OnEvent(Event *event);
		virtual void OnSizeChanged();

		void SetDebugText(const std::string &debugText);
		UIFrame *GetEngineFrame();
		UIList *GetEngineUDPNeighborList();
		UIList *GetEngineBluetoothList();
		UIList *GetEngineInfoList();

	protected:
		virtual void UpdateLayout(Movable *parent);
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _CalSize(const Sizef &projSize);
		void _CreateEngineFrame();

		EngineSceneCanvasPtr mEngineSceneCanvas;
		EngineUICanvasPtr mEngineUICanvas;

		UIFramePtr mInfoFrame;
		UIFTextPtr mInfoText;
		UIFTextPtr mDebugText;
		UIButtonPtr mEngineBut;

		UIFramePtr mEngineFrame;
		UICollapsePanelPtr mEngineCollapsePanel;
		// UDP
		UIListPtr mEngineUDPNeighbors;
		// Bluetooth
		UIButtonPtr mEngineBluetoothButConnect;
		UIListPtr mEngineBluetoothList;
		// info
		UIListPtr mEngineInfoList;

		int mFPS;
		int mFPSUpdate;
		float mFPSUpdateTime;
	};
	PX2_REGISTER_STREAM(EngineCanvas);
	typedef Pointer0<EngineCanvas> EngineCanvasPtr;

}

#endif