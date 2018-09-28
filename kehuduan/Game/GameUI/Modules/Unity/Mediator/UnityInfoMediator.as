package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class UnityInfoMediator extends Mediator
	{
		public static const NAME:String = "UnityInfo";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		public function UnityInfoMediator()
		{
			super(NAME); 
		}
		
		private function get unityInfo():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			        EventList.INITVIEW,
					UnityEvent.SHOWUNITYINFOVIEW,
					UnityEvent.CLOSEUNITYINFOVIEW,
					UnityEvent.UPTATAINFODATE
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.UNITYINFOVIEW});
					panelBase = new PanelBase(unityInfo, unityInfo.width+8, unityInfo.height+12);
					panelBase.name = "UnityInfoView";
					panelBase.x = UIConstData.DefaultPos2.x - 500;
					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_uim_han_1" ] );  // 帮派信息
					if(unityInfo != null)
					{
						unityInfo.mouseEnabled = false;
						unityInfo.txtUnityName.mouseEnabled = false;
						unityInfo.txtUnityMan.mouseEnabled = false;
						unityInfo.txtUnityTime.mouseEnabled = false;
						unityInfo.txtUnityLevel.mouseEnabled = false;
						unityInfo.txtNumOnline.mouseEnabled = false;
						unityInfo.txtUnityNum.mouseEnabled = false;
						unityInfo.txtNumTop.mouseEnabled = false;
						unityInfo.txtPlace.mouseEnabled = false;
						unityInfo.txtUnityNotice.mouseEnabled = false;
					}
			    break;
			    
			    case UnityEvent.SHOWUNITYINFOVIEW:
		        	var id:int = notification.getBody() as int;
			    	showUnityInfoView();
			    	sendUnityID(id);
			    	addLis();
			    break;
			    
			    case UnityEvent.CLOSEUNITYINFOVIEW:
			    	gcAll();
		    	break;
		    	
	    		case UnityEvent.UPTATAINFODATE:												//更新帮派信息列表
	    			var arr:Array = new Array();
	    			arr = notification.getBody() as Array;
	    			showData(arr);
		    	break;
			}
		}
		
		private function showUnityInfoView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			dataProxy.UnitInfoIsOpen = true;
		}
		
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			unityInfo.btncomfrim.addEventListener(MouseEvent.CLICK,confrimHandler);
			unityInfo.btnfriend.addEventListener(MouseEvent.CLICK,jionFriendHandler);
		}
		
		private function gcAll():void
		{
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			dataProxy.UnitInfoIsOpen = false;
		}
		
		/** 点击红叉关闭面板 */
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 点击确定按钮 */
		private function confrimHandler(e:MouseEvent):void
		{
			gcAll();
		}
		/** 点击加为好友按钮 */
		private function jionFriendHandler(e:MouseEvent):void
		{
			sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:0  , name:unityInfo.txtUnityMan.text});
		}
		/** 发送请求可以申请查看到的帮派信息方法 */
		private function sendUnityID(id:int):void
		{
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , 208 , 0 , id];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求
		}
		/** 显示数据 */
		private function showData(arr:Array):void
		{
			var time:String = String(arr[5]); 
			unityInfo.txtUnityName.text = arr[0];
			unityInfo.txtUnityMan.text = arr[7];
			unityInfo.txtUnityTime.text = time.slice(0,4) + "/" + time.slice(4,6) + "/" + time.slice(6,8) + "/" + time.slice(8,11);
			unityInfo.txtUnityLevel.text = arr[1];
			unityInfo.txtNumOnline.text = arr[11];
			unityInfo.txtUnityNum.text = arr[6];
			unityInfo.txtNumTop.text   = UnityNumTopChange.change(unityInfo.txtUnityLevel.text);
			unityInfo.txtPlace.text ;
			unityInfo.txtUnityNotice.text = arr[10];
		}
	}
}