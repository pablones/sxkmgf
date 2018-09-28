package GameUI.Modules.SystemMessage.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.SystemMessage.Data.SysMessageData;
	import GameUI.Modules.SystemMessage.Data.SysMessageEvent;
	import GameUI.Modules.SystemMessage.Memento.MessageMemento;
	import GameUI.Modules.SystemMessage.UI.SystemMsgText;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SysCotentMediator extends Mediator
	{
		public static const NAME:String = "SysCotentMediator";
		
		private var megContentView:MovieClip;
		private var panelBase:PanelBase;
		private var message:MessageMemento;
		private var txtContent:SystemMsgText;
		public function SysCotentMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
						EventList.INITVIEW,
						SysMessageEvent.SHOWMESSAGECONTENTVIEW,
						SysMessageEvent.CLOSEMESSAGECONTENTVIEW
					];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					this.megContentView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MegContentView"); 
					panelBase = new PanelBase(megContentView, megContentView.width - 27, megContentView.height + 12);
					panelBase.name = "SysCotentMediator";
					panelBase.x = 5;
					panelBase.y = 58;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_sysmsg_med_scm_1" ]);	  //"系统消息"
					contentInit();
					if(megContentView != null)
					{
						megContentView.mouseEnabled = false;
					}
				break;
				case SysMessageEvent.SHOWMESSAGECONTENTVIEW:
					message = notification.getBody() as MessageMemento;
					if(message == null){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"没有该信息", color:0xffff00});
						gcAll();
					}else {
						message.state = GameCommonData.wordDic[ "mod_sysmsg_med_scm_2" ];    //"已读"
						if(SysMessageData.messageContentIsopen) updateView();		//面板开着就更新数据
						else
						{
							showView();
							addLis();
						}
					}
				break;
				case SysMessageEvent.CLOSEMESSAGECONTENTVIEW:
				break;
			}
		}
		private function contentInit():void
		{
			txtContent = new SystemMsgText(299.5 , 134);
			megContentView.addChild(txtContent);
			txtContent.x = 2;
			txtContent.y = 77;
			txtContent.mouseEnabled = false;
			
		}
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			megContentView.btnMegFont.addEventListener(MouseEvent.CLICK , fontHandler);
			megContentView.btnMegBack.addEventListener(MouseEvent.CLICK , backHandler);
			megContentView.btnMegClose.addEventListener(MouseEvent.CLICK , closeHandler);
		}
		private function gcAll():void
		{
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			SysMessageData.messageContentIsopen = false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			megContentView.btnMegFont.removeEventListener(MouseEvent.CLICK , fontHandler);
			megContentView.btnMegBack.removeEventListener(MouseEvent.CLICK , backHandler);
			megContentView.btnMegClose.removeEventListener(MouseEvent.CLICK , closeHandler);
			message = null;
		}
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		private function showView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			SysMessageData.messageContentIsopen = true;
			updateView();
		}
		private function updateView():void
		{
			megContentView.txtMegTitle.text = message.title;
			megContentView.txtTime.text = message.timeStr;
			txtContent.desText = "<font color = '#FFFFFF'>" + message.content + "</font>";
			
		}
		private function backHandler(e:MouseEvent):void
		{
			var index:int = int(message.index) - 1;
			var length:int = SysMessageData.messageList.length;
			if(index == (length - 1))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysmsg_med_scm_3" ], color:0xffff00});     //"当前是最后一封"
				return;
			}
			//facade.sendNotification(SysMessageEvent.SHOWMESSAGECONTENTVIEW , SysMessageData.messageList[length - index - 1 - 1]);
			facade.sendNotification(SysMessageEvent.SHOWMESSAGECONTENTVIEW , SysMessageData.messageList[index + 1]);
		}
		private function fontHandler(e:MouseEvent):void
		{
			var index:int = int(message.index) - 1;
			var length:int = SysMessageData.messageList.length;
			if(index == 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysmsg_med_scm_4" ], color:0xffff00});      //"当前是第一封"
				return;
			}
			//facade.sendNotification(SysMessageEvent.SHOWMESSAGECONTENTVIEW , SysMessageData.messageList[length - index ]);
			facade.sendNotification(SysMessageEvent.SHOWMESSAGECONTENTVIEW , SysMessageData.messageList[index - 1]);			
		}
		private function closeHandler(e:MouseEvent):void
		{
			gcAll();
		}
//		/** 上下页的选中处理 */
//		private function selectHandler():void
//		{
//			SysMessageData.selectItemIndex = message.index;
//			var item:MessageItem = listView.getChildByName("MessageItem_" + message.index) as MessageItem;
//			item.selected();
//		}
		
	}
}