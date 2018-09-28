package GameUI.Modules.SystemMessage.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.Friend.view.ui.UINewSysMsgContainer;
	import GameUI.Modules.SystemMessage.Data.SysMessageData;
	import GameUI.Modules.SystemMessage.Data.SysMessageEvent;
	import GameUI.Modules.SystemMessage.Memento.MessageMemento;
	import GameUI.Modules.SystemMessage.UI.MessageItem;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SystemMediator extends Mediator
	{
		public static const NAME:String = "SystemMediator";
		
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase;
		private var iScrollPane:UIScrollPane	  = null;				//Scroll Bar
		private var listView:Sprite		  = null;				//帮助列表
		public var megView:MovieClip;			//消息面板
		private var placeY:int;
		private var fSysMsgContainer:UINewSysMsgContainer;
		public function SystemMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.INITVIEW,
					SysMessageEvent.SHOWMESSAGEVIEW,
					SysMessageEvent.CLOSEMESSAGEVIEW,
					SysMessageEvent.ADDSYSMESSAGE,
					SysMessageEvent.UPDATEMESSAGEVIEW
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
//					this.megView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SysMessageListView"); 
//					panelBase = new PanelBase(megView, megView.width + 9, megView.height + 12);
//					panelBase.name = "SystemMediator";
//					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_sysmsg_med_sm_1" ] );	   //"系统消息列表"
//					if(megView != null)
//					{
//						megView.mouseEnabled = false;
//					}
					this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
				break;
				case SysMessageEvent.SHOWMESSAGEVIEW:
					//this.panelBase = notification.getBody() as PanelBase;
					if(dataProxy.FriendsIsOpen == false)
					{
						sendNotification(FriendCommandList.SHOWFRIEND,SysMessageEvent.SHOWMESSAGEVIEW);
					}
				break;
				case SysMessageEvent.CLOSEMESSAGEVIEW:
					//gcAll();
				break;
				case SysMessageEvent.ADDSYSMESSAGE:			//添加系统消息
					var messageObject:Object = notification.getBody() as Object;
					addSysMessage(messageObject.title , messageObject.content);
					//if(dataProxy.FriendsIsOpen == true) updateList(); //如果打开了面板，就及时显示
				break;
				case SysMessageEvent.UPDATEMESSAGEVIEW:		//刷新消息面板
					//if(SysMessageData.messageListIsOpen && megView) updateList(); //如果打开了面板，就及时显示
				break;
			}
		}
		private function showView():void
		{
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = 302 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = 58 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;;
			}else{
				panelBase.x = 302;
				panelBase.y = 58;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			SysMessageData.messageListIsOpen = true;
		}
		private function addLis():void
		{
			megView.btnSysMegOpen.addEventListener(MouseEvent.CLICK , openHandler);
			//megView.btnSysMegClose.addEventListener(MouseEvent.CLICK , closeHandler);
			megView.btnSysMegClear.addEventListener(MouseEvent.CLICK , clearHandler);
			//megView.btnSysMegDelete.addEventListener(MouseEvent.CLICK , deleteHandler);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);		
		}
		private function gcAll():void
		{
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			SysMessageData.messageListIsOpen = false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			megView.btnSysMegOpen.removeEventListener(MouseEvent.CLICK , openHandler);
			//megView.btnSysMegClose.removeEventListener(MouseEvent.CLICK , closeHandler);
			megView.btnSysMegClear.removeEventListener(MouseEvent.CLICK , clearHandler);
			//megView.btnSysMegDelete.removeEventListener(MouseEvent.CLICK , deleteHandler);
		}
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 刷新列表 数据排列 */
		private function updateList():void
		{
			SysMessageData.selectItemIndex = 0;
			//scrollInit();
			//listView = new Sprite();
			
			var length:int = SysMessageData.messageList.length;
			for(var i:int = 0; i < SysMessageData.messageList.length ; i++)
			{
				if(megView.getChildByName("MessageItem_"+i) && megView.contains(megView.getChildByName("MessageItem_"+i)))  megView.removeChild(megView.getChildByName("MessageItem_"+i));
				SysMessageData.messageList[(length - i - 1)].index = (i + 1);
				if(i >= SysMessageData.messageTotalNum) continue;			//最多100条
				var item:MovieClip = this.fSysMsgContainer.(SysMessageData.messageList[(length - i - 1)]);
				//item.selectFunction = clickMegItem;
				//item.openFunction   = openMegItem;
				//item.name = "MessageItem_"+i;
				//this.megView.addChild(item);

			}
			//iScrollPane = new UIScrollPane(listView);
			//createScroll();
			//listViewInit();
			
		}
		/** 添加消息 */
		private function addSysMessage(_title:String , _content:String):void
		{
			var message:MessageMemento = new MessageMemento(_title , _content);
			SysMessageData.messageList.push(message);
			if(SysMessageData.selectItemIndex > 0 ) SysMessageData.selectItemIndex ++;		//添加时，消息选中也要加
			//this.fSysMsgContainer.fListDataPro = SysMessageData.messageList;
		} 
		/** 点击关闭按钮 */
		private function closeHandler(e:MouseEvent):void
		{
			gcAll();
		}
		/** 点击打开按钮 */
		private function openHandler(e:MouseEvent):void
		{
			if(SysMessageData.selectItemIndex == 0) return;
			openMegItem();
		}
		/** 点击清空按钮 */
		private function clearHandler(e:MouseEvent):void
		{
			SysMessageData.selectItemIndex = 0;
			SysMessageData.messageList = [];
			this.fSysMsgContainer.fListDataPro =  SysMessageData.messageList;
			//scrollInit();
		}
		/** 单击信息条目(委托) */
		public function clickMegItem(item:MessageMemento):void
		{
			SysMessageData.selectItemIndex = item.index;
		}
		/** 打开信息条目  (委托)*/
		public function openMegItem():void
		{
			var length:int = SysMessageData.messageList.length;
			//查看消息内容
			facade.sendNotification(SysMessageEvent.SHOWMESSAGECONTENTVIEW , SysMessageData.messageList[SysMessageData.selectItemIndex]);	//弹出消息面板
		}
		//////滚动条初始化
		private function scrollInit():void
		{
			if(iScrollPane && panelBase.contains(iScrollPane)) {
				panelBase.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
		}
		/** 创建滚动条 */
		private function createScroll():void
		{
			iScrollPane.x = 8;
			iScrollPane.y = 60;
			iScrollPane.width = megView.width - 7;
			iScrollPane.height = megView.height - 63;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;//SCROLLBAR_ALWAYS;//
			iScrollPane.refresh();
			panelBase.addChild(iScrollPane);
		}
		/** 列表初始化 */
		private function listViewInit():void
		{
		  	listView.graphics.clear();
			listView.graphics.beginFill(0x4d3c13, 0);
            listView.graphics.drawRect(0, 0, megView.width - 7, megView.height - 63);
            listView.graphics.endFill();
			listView.width = megView.width - 7;
		}
		/** 删除某一个消息 */
		private function deleteHandler(e:MouseEvent):void
		{
			var length:int = SysMessageData.messageList.length;
			if(SysMessageData.selectItemIndex == 0) return;
			SysMessageData.messageList.splice((length - SysMessageData.selectItemIndex) , 1);
			updateList();	
		}
	}
}