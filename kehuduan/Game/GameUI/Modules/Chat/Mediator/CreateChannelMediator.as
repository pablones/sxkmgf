package GameUI.Modules.Chat.Mediator
{
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CreateChannelMediator extends Mediator
	{
		public static const NAME:String = "CreateChannelMediator";
		
		private var panelBase:PanelBase;
		private var tmp:Array = null;
		public static const MAXCHANNEL:int = 9;
		
		public function CreateChannelMediator()
		{
			super(NAME);
		}
		
		private function get createChannel():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				ChatEvents.CREATORCHANNEL,
				ChatEvents.CLOSECREATORCHANNEL
			]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ChatEvents.CREATORCHANNEL:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.CREATORCHANNELVIEW});
					panelBase = new PanelBase(createChannel, createChannel.width + 10, createChannel.height + 16);
					panelBase.addEventListener(Event.CLOSE, closeHandler);
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_chat_med_cre_hand" ]);//"设定频道"
					panelBase.x = ChatData.tmpCreatePoint.x;
					panelBase.y = ChatData.tmpCreatePoint.y;
					panelBase.name = "CreateChannel";
					GameCommonData.GameInstance.GameUI.addChild(panelBase); 
					initView();
				break;
				case ChatEvents.CLOSECREATORCHANNEL:
					closeHandler(null);
				break;
			}
		}	
		
		private function closeHandler(event:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				ChatData.tmpCreatePoint.x = panelBase.x;
				ChatData.tmpCreatePoint.y = panelBase.y;
				for(var i:int = 0; i<MAXCHANNEL; i++)
				{
					createChannel["channel_"+i].removeEventListener(MouseEvent.CLICK, onChanelSelectHandler);
				}
				createChannel.btnModify.removeEventListener(MouseEvent.CLICK, modifyHandler);
				createChannel.btnCancel.removeEventListener(MouseEvent.CLICK, closeHandler);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				this.viewComponent = null;
				ChatData.CreateChannelIsOpen = false;
				facade.removeMediator(NAME);
			}
		}
		
		private function initView():void
		{
			if(ChatData.CurShowContent == 2) {			//个人
				tmp = UIUtils.DeeplyCopy(ChatData.Set1ChannelList) as Array;
			} else {									//自定
				tmp = UIUtils.DeeplyCopy(ChatData.Set2ChannelList) as Array;
			}
			for(var i:int = 0; i<MAXCHANNEL; i++) {
				if(tmp[i].value) {
					createChannel["channel_"+i].gotoAndStop(2);
				} else {
					createChannel["channel_"+i].gotoAndStop(1);
				}
				createChannel["channel_"+i].addEventListener(MouseEvent.CLICK, onChanelSelectHandler);
			}
			createChannel.btnModify.addEventListener(MouseEvent.CLICK, modifyHandler);
			createChannel.btnCancel.addEventListener(MouseEvent.CLICK, closeHandler);
		}
		
		private function onChanelSelectHandler(event:MouseEvent):void
		{
			var index:int = event.currentTarget.name.split("_")[1];			
			if(tmp[index].value)
			{
				createChannel["channel_"+index].gotoAndStop(1);
				tmp[index].value = false;
//				trace("tmp[index].value " +tmp[index].value);		 
			}
			else
			{
				createChannel["channel_"+index].gotoAndStop(2);
				tmp[index].value = true;
//				trace("tmp[index].value " +tmp[index].value);		
			}
		}
		
		private function modifyHandler(event:MouseEvent):void
		{
			var ret:int = 0;
			var tmpData:int = 0; 
			var len:int = tmp.length;
			if(ChatData.CurShowContent == 2) {
				ChatData.Set1ChannelList = tmp;
				for(var i:int = 0; i < len; i++) {
					if(ChatData.Set2ChannelList[i].value == true) {
						tmpData += Math.pow(2, len-1-i);
					}
					if(tmp[i].value == true) {
						ret += Math.pow(2, len+len-1-i);
					}
				}
			} else {
				ChatData.Set2ChannelList = tmp;
				for(var j:int = 0; j < len; j++) {
					if(ChatData.Set1ChannelList[j].value == true) {
						tmpData += Math.pow(2, len+len-1-j);
					}
					if(tmp[j].value == true) {
						ret += Math.pow(2, len-1-j); 
					}
				}
			}
			ret += tmpData;
			ChatData.channelSign = ret;
			closeHandler(null);
		}
		
		
	}
}