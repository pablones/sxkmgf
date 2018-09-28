package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.*;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PerfectNoticeMediator extends Mediator
	{
		public static const NAME:String = "PerfectNoticeMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var perfectView:MovieClip;
		public function PerfectNoticeMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			 		EventList.INITVIEW,
			       	UnityEvent.SHOWPERFECTVIEW,
			       	UnityEvent.CLOSEPERFECTVIEW,
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					perfectView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PerfectView");
					panelBase = new PanelBase(perfectView, perfectView.width-25,perfectView.height+12);
					panelBase.name = "PerfectView";
					panelBase.x = 16;//UIConstData.DefaultPos2.x - 600;
					panelBase.y = 100;//UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_per_han_1" ] );  // 修改通告
					if(perfectView != null)
					{
						perfectView.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWPERFECTVIEW:
					showPerfectView();
					perfectView.txtNotice.maxChars = 70;
					perfectView.txtNotice.wordWrap = true;
					perfectView.txtNotice.text = UnityConstData.mainUnityDataObj.unityNotice;								//默认为当前通告
			    	addLis();
		    	break;
		    	
		    	case UnityEvent.CLOSEPERFECTVIEW:
		    		gcAll();
		    	break;
			}
		}
		
		private function showPerfectView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			panelBase.x = 16;
			panelBase.y = 100;
			UnityConstData.perfectNoticeIsOpen = true;
		}
		
		private function addLis():void
		{
			UIUtils.addFocusLis(perfectView.txtNotice);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
		 	perfectView.btnComfrim.addEventListener(MouseEvent.CLICK,comfrimHandler);
			perfectView.btnCancel.addEventListener(MouseEvent.CLICK,cancelHandler);
		}
		
		private function gcAll():void
		{
			UIUtils.removeFocusLis(perfectView.txtNotice);
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			UnityConstData.perfectNoticeIsOpen = false;
			cleartxt();
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 点击确定按钮 */
		private function comfrimHandler(e:MouseEvent):void
		{
			perfectView.txtNotice.text = perfectView.txtNotice.text.replace(/^\s*|\s*$/g,"").split(" ").join("");					//去掉空格
			if(UIUtils.checkChat(perfectView.txtNotice.text) == false)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_7" ], color:0xffff00});  // 文本内容不合法
				return;
			}
			sendPerfect(perfectView.txtNotice.text);
			facade.sendNotification(UnityEvent.CLOSEPERFECTVIEW);
		}
		/** 点击取消按钮 */
		private function cancelHandler(e:MouseEvent):void
		{
			facade.sendNotification(UnityEvent.CLOSEPERFECTVIEW);
		}
		/** 发送修改通知请求 */
		private function sendPerfect(notice:String ):void
		{
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , notice , 218 , 0 , 0];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送修改公告请求
		}
		/** 公告txt清空*/
		private function cleartxt():void
		{
			perfectView.txtNotice.text = ""; 
		}
		/** 发送请求帮派成员信息方法 */
		private function sendAction(type:int , currentPage:int , id:int):void
		{
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , type, currentPage , id];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求
		}
		private function applyTrade():void{};
	}
}