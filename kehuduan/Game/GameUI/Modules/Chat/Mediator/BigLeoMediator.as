package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Command.SendBigLeoCommand;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	// 大喇叭 发GM滚屏
	public class BigLeoMediator extends Mediator
	{
		public static const NAME:String = "BigLeoMediator";
		private var panelBase:PanelBase;
		
		public function BigLeoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		private function get bigLeo_mc():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [ 
						EventList.INITVIEW,
						ChatEvents.SHOW_BIG_LEO,
						ChatEvents.CLOSELEO
			
						]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"BigLeoPanel"});
					facade.registerCommand(SendBigLeoCommand.NAME,SendBigLeoCommand);
					panelBase = new PanelBase(bigLeo_mc, bigLeo_mc.width - 26, bigLeo_mc.height+ 14);
					panelBase.addEventListener(Event.CLOSE, panelClosed);
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_chat_com_rec_getB" ]);//"大喇叭"
					break;
				
				case ChatEvents.SHOW_BIG_LEO:
					initView();
					break;
					
				case ChatEvents.CLOSE_BIG_LEO:
					panelClosed(null);
					break;
			}
		}
		
		private function initView():void
		{
			if ( ChatData.SetLeoIsOpen )
			{
				facade.sendNotification(ChatEvents.CLOSELEO);
			}
			ChatData.SetBigLeoIsOpen = true;
			bigLeo_mc.send_btn.addEventListener(MouseEvent.CLICK, send);
			bigLeo_mc.cancel_btn.addEventListener(MouseEvent.CLICK, cancel);
			bigLeo_mc.txtInfo.text = "";
			bigLeo_mc.txtInfo.maxChars = 32;
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;
			}else{
				panelBase.x = ChatData.tmpLeoPoint.x;
				panelBase.y = ChatData.tmpLeoPoint.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			
			UIUtils.addFocusLis(bigLeo_mc.txtInfo);
			bigLeo_mc.stage.focus = bigLeo_mc.txtInfo;
			UIConstData.FocusIsUsing = true;
		}
		
		private function send(evt:MouseEvent):void
		{
			var sendMsg:String = bigLeo_mc.txtInfo.text;
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "")
			{
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_chat_med_big_send" ], color:0xffff00});//"发言内容不能为空"
				return;
			}
			var obj:Object = new Object();
			obj.type = 2039;
			obj.color = 0xff0000;
			obj.name = "ALLUSER";
			obj.talkMsg = bigLeo_mc.txtInfo.text.split("\r").join("");
			facade.sendNotification(SendBigLeoCommand.NAME, obj);
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
				bigLeo_mc.send_btn.removeEventListener(MouseEvent.CLICK, send);
				bigLeo_mc.cancel_btn.removeEventListener(MouseEvent.CLICK, cancel);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			ChatData.SetLeoIsOpen = false;
			UIConstData.FocusIsUsing = false;
			ChatData.SetBigLeoIsOpen = false;
		}
	}
}