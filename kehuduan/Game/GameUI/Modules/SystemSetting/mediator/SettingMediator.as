package GameUI.Modules.SystemSetting.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SettingMediator extends Mediator
	{
		
		public static const NAME:String = "settingMediator";
		private var dataProxy:DataProxy;		
		private var panelBase:PanelBase;
		private var btnSystem:MovieClip;
		private var btnKeyboard:MovieClip;
		private var mcFrame:MovieClip;
		private var mcList:MovieClip;
		private var tempBtn:MovieClip;
		private var tempMC:MovieClip;
		private var mcNum:int = 1;
		private var isFirstOpenUI:Boolean = true;
		public function SettingMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		public function get settingUI():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				SystemSettingData.OPEN_SETTING_UI
			];
		}
		public override function handleNotification(note:INotification):void
		{
			switch(note.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"settingcompose"});
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					panelBase = new PanelBase(settingUI, settingUI.width + 8,settingUI.height+12);
					panelBase.addEventListener(Event.CLOSE,closeHandler);
					panelBase.name = "settingPanel";
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_too_con_int_int_71" ]);    //"系统设置"
					firstInitPanel();
					facade.registerMediator(new SoundSetMediator(SoundSetMediator.NAME,mcFrame));
					sendNotification(SystemSettingData.INIT_SOUND_VIEW);				
					break;
				case SystemSettingData.OPEN_SETTING_UI:
					openUI();
					break;
			}
		} 
		private function firstInitPanel():void
		{
			btnSystem = settingUI.getChildByName("btn_system") as MovieClip;
			btnKeyboard = settingUI.getChildByName("bnt_keyboard") as MovieClip;
			mcFrame = settingUI.getChildByName("mc_frame") as MovieClip;
			mcList = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcList");
			mcList.mouseEnabled = false;
			mcList.mouseChildren = false;
			btnSystem.buttonMode = true;
			btnKeyboard.buttonMode = true;
			tempBtn = btnSystem;
			tempMC = mcFrame;
			btnKeyboard.gotoAndStop(2);
			mcNum = settingUI.getChildIndex(mcFrame);
			addEventListeners();
		}
		private function closeHandler(e:Event):void
		{
			gc();
		}
		private function openUI():void
		{			
			if(isFirstOpenUI)
			{
				isFirstOpenUI = false;
//				facade.registerMediator(new SoundSetMediator(SoundSetMediator.NAME,mcFrame));
			}
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos1.x+160 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos1.y+30 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x+160;
				panelBase.y = UIConstData.DefaultPos1.y+30;
			}
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				gc();
			}
			else
			{
				dataProxy.settingPanIsOpen = true;
				sendNotification(SystemSettingData.INIT_SOUND_VIEW);				
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
				addEventListeners();
			}
		}
		private function addEventListeners():void
		{
			btnSystem.addEventListener(MouseEvent.CLICK,clickHandler);
			btnKeyboard.addEventListener(MouseEvent.CLICK,clickHandler);
		}
		private function clickHandler(me:MouseEvent):void
		{
			if((me.target as MovieClip).currentFrame == 1)
			 	return;
			 	
			 if(me.target == btnSystem)
			 {
			 	btnSystem.gotoAndStop(1);
			 	tempBtn.gotoAndStop(2);
			 	tempBtn = btnSystem;
			 	mcList.parent.removeChild(mcList);
			 	settingUI.addChild(mcFrame);
			 	settingUI.setChildIndex(mcFrame,mcNum);
			 	tempMC = mcFrame;
			 	mcFrame.x = 0;
			 	mcFrame.y = 17;
			 	
			 }
			 if(me.target == btnKeyboard)
			 {
			 	btnKeyboard.gotoAndStop(1);
			 	tempBtn.gotoAndStop(2);  
			 	tempBtn = btnKeyboard;
			 	mcFrame.parent.removeChild(mcFrame);
			 	settingUI.addChild(mcList);
			 	settingUI.setChildIndex(mcList,mcNum);
			 	tempMC = mcList;
			 	mcList.x = 0;
			 	mcList.y = 17;
			 	
			 }
		}
		private function gc():void
		{
			dataProxy.settingPanIsOpen = false;
			sendNotification(SystemSettingData.CLOSE_SETTING_VIEW);
			btnSystem.removeEventListener(MouseEvent.CLICK,clickHandler);
			btnKeyboard.removeEventListener(MouseEvent.CLICK,clickHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);		
			
		}
		
	}
}