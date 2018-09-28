package GameUI.Modules.NewPlayerCard.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Command.SendBigLeoCommand;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewPlayerCard.Data.NewerCardData;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	// 新手卡
	public class NewPlayerCardMediator extends Mediator
	{
		public static const NAME:String = "NewPlayerCardMeidator";
		private var panelBase:PanelBase;
		
		private var upState_cancel:DisplayObject;
		private var overState_cancel:DisplayObject;
		private var upState_send:DisplayObject;
		private var overState_send:DisplayObject;
		
		public function NewPlayerCardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		private function get newCard_mc():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [ 
						EventList.INITVIEW,
						NewerCardData.SHOW_NEW_CARD_PAN
						]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"NewHandCard"});
					facade.registerCommand(SendBigLeoCommand.NAME,SendBigLeoCommand);
					panelBase = new PanelBase(newCard_mc, newCard_mc.width - 26, newCard_mc.height+14);
//					panelBase = new PanelBase(newCard_mc, newCard_mc.width - 20, newCard_mc.height+14);
					panelBase.addEventListener(Event.CLOSE, panelClosed);
					panelBase.x = ChatData.tmpLeoPoint.x;
					panelBase.y = ChatData.tmpLeoPoint.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_newerP_med_new_han" ]);//"新手卡"
					//防止按钮变态
					this.overState_cancel = ( newCard_mc.cancel_btn as SimpleButton ).overState;
					this.upState_cancel = ( newCard_mc.cancel_btn as SimpleButton ).upState;
					this.overState_send = ( newCard_mc.send_btn as SimpleButton ).overState;
					this.upState_send = ( newCard_mc.send_btn as SimpleButton ).upState;
					break;
				
				case NewerCardData.SHOW_NEW_CARD_PAN:
					initView();
					break;
			}
		}
		
		private function initView():void
		{
			newCard_mc.send_btn.addEventListener(MouseEvent.CLICK, send);
			newCard_mc.cancel_btn.addEventListener(MouseEvent.CLICK, cancel);
			
			///////////////////////////////防止按钮变态
			newCard_mc.send_btn.addEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
			newCard_mc.cancel_btn.addEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			
			newCard_mc.txtInfo.text = "";
			newCard_mc.txtInfo.maxChars = 36; 
			newCard_mc.txtInfo.multiline = false;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			
			newCard_mc.gainCard_txt.htmlText = '<a href="event:reload_'+GameCommonData.wordDic[ "mod_med_med_onL_1" ]+'">'+GameCommonData.wordDic[ "mod_newerP_med_new_ini" ]+'</a>';//"刷新"		"点此领取新手卡"
			newCard_mc.gainCard_txt.addEventListener(TextEvent.LINK,openPage);
			
//			newCard_mc.gainCard_txt.htmlText = "点此领取新手卡";
			
			UIUtils.addFocusLis(newCard_mc.txtInfo);
			newCard_mc.stage.focus = newCard_mc.txtInfo;
			UIConstData.FocusIsUsing = true;
		}
		
		private function overSendBtnHandler( evt:MouseEvent ):void
		{
			newCard_mc.send_btn.removeEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
			( newCard_mc.send_btn as SimpleButton ).overState = this.overState_send;	
		}
		
		private function overCancelHandler( evt:MouseEvent ):void
		{
			newCard_mc.cancel_btn.removeEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			( newCard_mc.cancel_btn as SimpleButton ).overState = this.overState_cancel;
		}
		
		private function openPage(evt:TextEvent):void
		{
			navigateToURL( new URLRequest ( ChatData.NEWER_CARD_INTERFACE_ADDR ), "_blank" );
		}
		
		private function send(evt:MouseEvent):void
		{
			var sendMsg:String = newCard_mc.txtInfo.text;
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "")
			{
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_newerP_med_new_send" ], color:0xffff00});//"请输入新手卡卡号"
				return;
			}
			//如果是360，就直接发消息出去了  发的是7
			if ( is360() || GameConfigData.AccSocketIP == "192.168.6.85" )
			{
				Zippo.SendPickNewCard( sendMsg,7 );
				panelClosed(null);
				return;
			}
			var aStr:Array = sendMsg.split("-");
			if ( aStr.length != 3 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			else
			{
				if ( (aStr[0] != "YJJH") || (aStr[1].length != 8 ) || (aStr[2].length != 4) )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
					return;
				}
			}
			
//			if ( ( BagUtils.TestBagIsFull(0)+1 )>BagData.BagNum[0] )
//		 	{
//		 		facade.sendNotification(HintEvents.RECEIVEINFO, {info:"领取奖励需要1个道具栏空位，请先清理背包", color:0xffff00});
//				return;
//		 	}
			
			//发送卡号至服务器 其他平台发1
			Zippo.SendPickNewCard( sendMsg,1 );
			panelClosed(null);
		}
		
		private function cancel(evt:MouseEvent):void
		{
			panelClosed(null);
		}
		
		private function panelClosed(evt:Event):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				newCard_mc.send_btn.removeEventListener(MouseEvent.CLICK, send);
				newCard_mc.cancel_btn.removeEventListener(MouseEvent.CLICK, cancel);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				( newCard_mc.send_btn as SimpleButton ).overState = this.upState_send;
				( newCard_mc.cancel_btn as SimpleButton ).overState = this.upState_cancel;
				newCard_mc.send_btn.removeEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
				newCard_mc.cancel_btn.removeEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			}
			UIConstData.FocusIsUsing = false;
		}
		
		//判断是否是360 
		private function is360():Boolean
		{
			var arr:Array = GameConfigData.AccSocketIP.split( "." );
			if ( arr[ arr.length-1 ] == "cn" && arr[ arr.length-2 ] == "360" )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
//		private function remove():void
//		{
//			clearTimeout( timeId );
//			if ( panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase) )
//			{
//				newCard_mc.send_btn.removeEventListener(MouseEvent.CLICK, send);
//				newCard_mc.cancel_btn.removeEventListener(MouseEvent.CLICK, cancel);
//				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
//			}
//			UIConstData.FocusIsUsing = false;
//		}
	}
}