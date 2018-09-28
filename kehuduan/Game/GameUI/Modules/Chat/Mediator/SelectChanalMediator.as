package GameUI.Modules.Chat.Mediator
{
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.View.BaseUI.ListComponent;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SelectChanalMediator extends Mediator
	{
		public static const SELECTCHANNEL:String = "SelectChanalMediator";	
		
		public static const STARTPOSX:Number = 10;
		public static const STARTPOSY:Number = 556.5;
			
		private var container:ListComponent = null;
		
		public function SelectChanalMediator()
		{
			super(SELECTCHANNEL);
			this.setViewComponent(container);
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				ChatEvents.OPENCHANNEL,
				ChatEvents.CLOSESELECTCHANNEL
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ChatEvents.OPENCHANNEL:
					ChatData.SelectChannelOpen = true;
					setView();
				break;
				case ChatEvents.CLOSESELECTCHANNEL:
					if(GameCommonData.GameInstance.GameUI.contains(container))
					{
						GameCommonData.GameInstance.GameUI.removeChild(container);
						container = null;
						ChatData.SelectChannelOpen = false;
					}
				break;
			}
		}
		
		private function setView():void
		{
			container = new ListComponent();
			for( var i:int = 0; i<ChatData.channelModel.length; i++ )
			{
				if(ChatData.channelModel[i] == undefined) continue;
				var channel:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Channel");
				channel.txtInfo.htmlText = ChatData.channelModel[i].label;
				channel.txtInfo.filters = Font.Stroke(0x000000);
				channel.txtInfo.mouseEnabled = false;
				channel.name = i.toString();
				channel.addEventListener(MouseEvent.CLICK, onChannelClick);
				container.SetChild(channel);
			}
			container.upDataPos();	
			GameCommonData.GameInstance.GameUI.addChild(container);
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_DOWN, removeList);
			container.x = STARTPOSX;
			container.y = STARTPOSY - container.height + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
		}		
		
		private function removeList(event:MouseEvent):void
		{
//			trace(event.currentTarget.name);
//			trace(event.target.name);
			if(event.target.name == "btnSelectCh" || event.target.name == "selectChanelBtn") return;
			GameCommonData.GameInstance.TooltipLayer.removeEventListener(MouseEvent.MOUSE_DOWN, removeList);
			if(container)
			{
				if(GameCommonData.GameInstance.GameUI.contains(container))
				{
					GameCommonData.GameInstance.GameUI.removeChild(container);
					container = null;
					ChatData.SelectChannelOpen = false;
				}
			}
		}
					
		private function onChannelClick(event:MouseEvent):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(container))
			{
				GameCommonData.GameInstance.GameUI.removeChild(container);
				container = null;
				var index:int = int(event.currentTarget.name);
				if(ChatData.curSelectModel != index) {
					ChatData.curSelectModel = index;
					facade.sendNotification(ChatEvents.CLOSECHANNEL);
				}
				ChatData.SelectChannelOpen = false;
			}
		}
				
	}
}