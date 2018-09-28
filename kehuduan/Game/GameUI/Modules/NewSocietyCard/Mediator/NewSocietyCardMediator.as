package GameUI.Modules.NewSocietyCard.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Command.SendBigLeoCommand;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewSocietyCard.Data.NewSocietyCardData;
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
	

	
	
	
	import org.puremvc.as3.multicore.interfaces.IMediator;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	import org.puremvc.as3.multicore.interfaces.INotification;
	public class NewSocietyCardMediator extends Mediator  implements IMediator
	{
		public static const NAME:String = "NewSocietyCardMeidator";
		private var panelBase:PanelBase;
		
		private var upState_cancel:DisplayObject;
		private var overState_cancel:DisplayObject;
		private var upState_send:DisplayObject;
		private var overState_send:DisplayObject;
		private var newCard_mc:MovieClip;
		public function NewSocietyCardMediator(mediatorname:String= null, viewComponent:Object= null)
		{
			super(NAME, viewComponent);
		}
				
		public override function listNotificationInterests():Array
		{
			return [
					NewSocietyCardData.SHOW_SOCIETY_CARD_PAN			
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{	
				case NewSocietyCardData.SHOW_SOCIETY_CARD_PAN:
					initView();
					break;
			}
		}
		
		private function initView():void
		{
			newCard_mc=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("societyCard"); 
			newCard_mc.send_btn.addEventListener(MouseEvent.CLICK, send);
			newCard_mc.cancel_btn.addEventListener(MouseEvent.CLICK, cancel);
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"societyCard"});
			facade.registerCommand(SendBigLeoCommand.NAME,SendBigLeoCommand);
			panelBase = new PanelBase(newCard_mc, newCard_mc.width - 26, newCard_mc.height+14);

			panelBase.addEventListener(Event.CLOSE, panelClosed);
			panelBase.x = ChatData.tmpLeoPoint.x;
			panelBase.y = ChatData.tmpLeoPoint.y;
			panelBase.SetTitleTxt("公会卡");//"公会卡"
					//防止按钮变态
			this.overState_cancel = ( newCard_mc.cancel_btn as SimpleButton ).overState;
			this.upState_cancel = ( newCard_mc.cancel_btn as SimpleButton ).upState;
			this.overState_send = ( newCard_mc.send_btn as SimpleButton ).overState;
			this.upState_send = ( newCard_mc.send_btn as SimpleButton ).upState;

			
			
			///////////////////////////////防止按钮变态
			newCard_mc.send_btn.addEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
			newCard_mc.cancel_btn.addEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			
			newCard_mc.txtInfo.text = "";
			newCard_mc.txtInfo.maxChars = 18;
			newCard_mc.txtInfo.multiline = false;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			

			
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
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_1" ], color:0xffff00});//"请输入媒体卡卡号"
				return;
			}
			
			if ( sendMsg.length<18 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			var aStr:Array = sendMsg.split("-");
			if ( aStr[0].length != 4  )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			


			
			//发送卡号至服务器
			Zippo.SendPickSocietyCard( sendMsg );
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
			if ( facade.hasMediator( NAME ) )
			{
				facade.removeMediator( NAME );
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


