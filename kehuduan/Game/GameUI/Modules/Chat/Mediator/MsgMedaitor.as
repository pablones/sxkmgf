package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.AutoResize.Data.AutoSizeData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.UI.ChatCellEvent;
	import GameUI.Modules.Chat.UI.ChatCellText;
	import GameUI.Modules.IdentifyTreasure.Command.ShowTreasureCommand;
	import GameUI.Modules.IdentifyTreasure.Mediator.IdentifyTreaMediator;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Modules.StoneMoney.Mediator.StoneMoneyMediator;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.ItemInfo;
	import Net.ActionProcessor.SoulDetailInfoAction;
	import Net.ActionSend.SoulDetailInfoSend;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.utils.clearInterval;
	import flash.utils.getTimer;
	import flash.utils.setInterval;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MsgMedaitor extends Mediator
	{
		public static const NAME:String = "MsgMedaitor";
		private var tmpMsgList:Array;
		private var iScrollPane:UIScrollPane; 
		private var msgArea:Sprite;
		private var back:Sprite = new Sprite();
		private var backAlpha:Array = [.8, .6, .4, .2, 0];
		private var curpos:int = 0;
		private var leoView:ChatCellText;
		private var leoMsg:Array = [];
		private var leoDelay:Number = 0;
		private var isShow:Boolean = false;
		private var delayNum:int = 12000;
		
		private var isBottom:Boolean = true;
		
		
		public function MsgMedaitor(view:Sprite)
		{
			super(NAME);
			this.viewComponent = view; 
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				ChatEvents.CREATORMSGAREA,
				ChatEvents.SHOWMSGINFO,
				ChatEvents.CHANGEHEIGHT,
				ChatEvents.CHANGEMSGAREA,
				ChatEvents.CHANGEMOUSE,
				ChatEvents.RECEIVELEOMSG,
				ChatEvents.SHOW_HIDE_CHAT_VIEW,
				ChatEvents.BOSS_MESSAGE,
				ChatEvents.CLEAR_MSG_CUR_CHANNEL	//清屏，将当前频道的消息顶上去
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ChatEvents.CREATORMSGAREA:
					createMsgArea();
					sendNotification( AutoSizeData.MSG_MED_CREATE_OVER );
				break;	
				case ChatEvents.SHOWMSGINFO:
					tmpMsgList = notification.getBody() as Array;
					show();
				break;
				case ChatEvents.CHANGEHEIGHT:
					changeArea();
				break;
				case ChatEvents.CHANGEMSGAREA:
					changeMsgArea();
				break;
				case ChatEvents.CHANGEMOUSE:
					curpos++;
					if(curpos==backAlpha.length)
					{
						curpos = 0;
					}
					var alpha:Number = backAlpha[curpos];
					back.alpha = alpha;
				break;
				case ChatEvents.RECEIVELEOMSG:
					receviceLeoMsg(notification.getBody());
				break;
				case ChatEvents.CLEAR_MSG_CUR_CHANNEL:	//清屏，将当前频道的消息顶上去
					changeCancel();
				break;
				case ChatEvents.SHOW_HIDE_CHAT_VIEW:
					showHideChatView();	
				break;
				case ChatEvents.BOSS_MESSAGE:
					tmpMsgList = notification.getBody() as Array;
					if (tmpMsgList[0])
					{
						tmpMsgList[0]["info"] = "<3_["
															+ GameCommonData.wordDic["mod_chat_com_rec_getD"] // 系统
															+"]_8><3_["
															+ tmpMsgList[0].boss
															+ "]_8>："
															+ tmpMsgList[0].msg;
					}
					show();
				break;
			}
		}
		
		private function createMsgArea():void
		{
			msgArea = new Sprite();
			msgArea.name = "msgArea";
			msgArea.mouseEnabled = false;
			back = new Sprite();
			back.name = "msgBack";
			back.mouseEnabled = false;
			back.alpha = backAlpha[curpos];
			msgArea.graphics.beginFill(0xff00ff, 0);
			msgArea.graphics.drawRect(0,0,ChatData.MsgPosArea[ChatData.CurAreaPos].width, ChatData.MsgPosArea[ChatData.CurAreaPos].height);
			
			msgArea.graphics.endFill();
			//
			creatorBack();
			back.width = ChatData.MsgPosArea[ChatData.CurAreaPos].width;
			back.height = ChatData.MsgPosArea[ChatData.CurAreaPos].height
			back.x = ChatData.MsgPosArea[ChatData.CurAreaPos].x+16;
			back.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y;
			GameCommonData.GameInstance.GameUI.addChild(back);
		 	//
		 	iScrollPane = new UIScrollPane(msgArea);
		 	iScrollPane.width = ChatData.MsgPosArea[ChatData.CurAreaPos].width;
			iScrollPane.height = ChatData.MsgPosArea[ChatData.CurAreaPos].height;
			iScrollPane.x = ChatData.MsgPosArea[ChatData.CurAreaPos].x;
			iScrollPane.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.setScrollPos();
			iScrollPane.refresh();
			iScrollPane.scrollBottom();
			iScrollPane.refresh();
			msgArea.width = 328;
			GameCommonData.GameInstance.GameUI.addChild(iScrollPane);
		}
		
		private function showHideChatView():void
		{
			if(ChatData.chat_view_is_show) {	//显示聊天
				back.visible = true;
				iScrollPane.visible = true;
			} else {							//隐藏聊天
				back.visible = false;
				iScrollPane.visible = false;
			}
		}
		
		private function receviceLeoMsg(obj:Object):void
		{
			leoMsg.push(obj);	
			if(!isShow) {
				showLeo(obj);	
			}
		}
		
		private function showLeo(obj:Object):void
		{
			isShow = true;
			leoView = new ChatCellText(obj.info, obj.dfColor);
			leoView.name = "leoView";
			leoView.x = ChatData.MsgPosArea[ChatData.CurAreaPos].x + 16;
			leoView.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y - 34;
			if( GameCommonData.fullScreen == 2 )
			{
				leoView.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y - 34 + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
			}
			GameCommonData.GameInstance.GameUI.addChild(leoView);
			leoView.addEventListener(ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler);
			GameCommonData.GameInstance.GameUI.setChildIndex(leoView, GameCommonData.GameInstance.GameUI.numChildren-1);
			leoDelay = setInterval(delayShow, delayNum);
		}
		
		private function delayShow():void
		{
			clearInterval(leoDelay);
			facade.sendNotification(ChatEvents.HIDEQUICKOPERATOR);
			
			if(leoView) {
				leoView.removeEventListener(ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler);
				leoView.dispose();
				GameCommonData.GameInstance.GameUI.removeChild(leoView);
				leoView = null;
			}
			leoMsg.shift();
			isShow = false;
			if(leoMsg.length != 0) {
				showLeo(leoMsg[0]);
			}
		}
		
		private function creatorBack():void
		{
			back.graphics.beginFill(0x000000, 0.6);
			back.graphics.drawRect(0,0,100,100);
			back.graphics.endFill();
		}
			
		private function show():void
		{
//			var str:String="<3_[公告]:_1><0_[老范]_fsadfsf_98989>潜心钻研，终于将<1_[厅里要]_fksd_fsdjf_2>的升寺地到<3_5_3>级，<1_[一颗宝石]_sfdsf_fsdfd_3>，宠物的资质得到了，提升到<3_7_6>级大幅的ss升\\008！";
			isBottom = iScrollPane.isBottom();
			clearFirstChild();
			var index:int = tmpMsgList.length - 1;
			var faceText:ChatCellText = new ChatCellText(tmpMsgList[index].info, tmpMsgList[index].dfColor);
			faceText.addEventListener(ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler);
			msgArea.addChild(faceText);
			faceText.x = 2;
//			msgArea.width = 328;
			showElements();
			iScrollPane.refresh();
			if(isBottom) {	//如果滑动条已经滑到最下则继续滑到最下，否则停留
				iScrollPane.scrollBottom();
				iScrollPane.refresh();
			}
			if(GameCommonData.wordVersion == 2) //台服
			{
				if(SelfInfoMediator.isDuelState) 
				{
					var msg:String = tmpMsgList[index].info;
					if(msg.indexOf("在切磋中") > 0)
					{
						if(GameCommonData.Player.Role.HP == 1)
						{
							GameCommonData.UIFacadeIntance.sendNotification(TaskCommandList.SEND_FB_AWARD,14);	//切磋失败
						}
						else if(GameCommonData.Player.Role.HP > 1)
						{
							GameCommonData.UIFacadeIntance.sendNotification(TaskCommandList.SEND_FB_AWARD,13);	//切磋成功
						}
					}
				}	
			}
		}
		
		/** 点击链接 */
		private function getLinkHandler(e:ChatCellEvent):void
		{
			var data:String = e.data as String;
			var dataArr:Array = data.split("_");
			
			if(dataArr[0] == 0) {				//玩家名
				if(dataArr[1] == "["+GameCommonData.Player.Role.Name+"]" || dataArr[1] == GameCommonData.Player.Role.Name || ChatData.QuickChatIsOpen) return;
				var quickSelectMediator:QuickSelectMediator = new QuickSelectMediator();
			 	facade.registerMediator(quickSelectMediator);
				facade.sendNotification(ChatEvents.SHOWQUICKOPERATOR, dataArr[1].substring(1, dataArr[1].length-1));
			} else if(dataArr[0] == 1) {		//物品
				var tmpName:String = dataArr[1] ;
				if(tmpName == "["+GameCommonData.wordDic[ "mod_chat_med_msg_getL_1" ]+"]") {//剑徒
					IdentifyTreaMediator.curBtn = 0;
					sendNotification( ShowTreasureCommand.NAME );
					return;
				} else if(tmpName == "["+GameCommonData.wordDic[ "mod_chat_med_msg_getL_2" ]+"]") {//剑客
					IdentifyTreaMediator.curBtn = 1;
					sendNotification( ShowTreasureCommand.NAME );
					return;
				} else if(tmpName == "["+GameCommonData.wordDic[ "mod_chat_med_msg_getL_3" ]+"]") {//剑豪
					IdentifyTreaMediator.curBtn = 2;
					sendNotification( ShowTreasureCommand.NAME );
					return;
				}
				var type:int = int(dataArr[3]);
				var dataItem:Object = null;
				if(type >= 250000 && type < 300000)
				{
					ItemInfo.isPanel = true;
					SoulDetailInfoAction.isPanel = true;
//					SoulProxy.getSoulDetailInfoFromBag( dataArr[2] );
					
					//请求魂魄详细信息
					var reqData:Array = [ 101,dataArr[2],dataArr[4],0 ];
					SoulDetailInfoSend.createSoulDetailInfoMsg( reqData );
					
					UiNetAction.GetItemInfo(dataArr[2], dataArr[4]);
				}
				else if(type > 300000 && type < 700000) {
					if ( type>=506000 && type<=506005 )
					{
						ItemInfo.isPanel = true;
						UiNetAction.GetItemInfo(dataArr[2], dataArr[4]);
					}
					else
					{
						if(type == 351001)	//元宝票
						{
							if(IntroConst.ItemInfo[dataArr[2]])
							{
								dataItem = IntroConst.ItemInfo[dataArr[2]];		
							}
							else
							{
								StoneMoneyMediator.isSendAction = true;
								UiNetAction.GetItemInfo(dataArr[2], dataArr[4]);
								return;
							}
						}
						else
						{
							dataItem = UIConstData.getItem(type);
							dataItem.id = undefined; 
							dataItem.isBind = int(dataArr[5]);
						}
						facade.sendNotification(EventList.SHOWITEMTOOLPANEL, {type:dataItem.type, data:dataItem});
					}
				} else if(type < 250000) {	
					if( ChatEvents.CLICK_AND_SEND )
					{
//						trace( ( getTimer() - ChatEvents.CLICK_TIME_STORAGE ) );
						if( ( getTimer() - ChatEvents.CLICK_TIME_STORAGE ) > 5000 )
						{
							ChatEvents.CLICK_AND_SEND = false;
							ChatEvents.CLICK_TIME_STORAGE = 0;
//							trace( "reset" );
						}	
					}
					else
					{
						ItemInfo.isPanel = true;
						ChatEvents.CLICK_TIME_STORAGE = getTimer();
//						trace( ChatEvents.CLICK_TIME_STORAGE );
						UiNetAction.GetItemInfo(dataArr[2], dataArr[4]);
						ChatEvents.CLICK_AND_SEND = true;
//						trace("send");						
					}
//					if(IntroConst.ItemInfo[dataArr[2]]) {
//						facade.sendNotification(EventList.SHOWITEMTOOLPANEL, {type:dataArr[3], isEquip:true, data:IntroConst.ItemInfo[dataArr[2]]});
//					} else {
//						UiNetAction.GetItemInfo(dataArr[2], dataArr[4]);
//					}
				}
			} else if(dataArr[0] == 2) {		//宠物
				if(dataArr[2]  >= 2000000000 && dataArr[2] <= 3999999999) {
					if(GameCommonData.Player.Role.Id == int(dataArr[4]) ) return;
					facade.sendNotification(PetEvent.PET_LOOK_OUTSIDE_INTERFACE, {petId:int(dataArr[2]),ownerId:int(dataArr[4])});
				}
			}
		}
		
		private function changeArea():void
		{
			ChatData.CurAreaPos++; 
			if(ChatData.CurAreaPos == ChatData.MsgPosArea.length)
			{
				ChatData.CurAreaPos = 0;
			}
//			back.width = ChatData.MsgPosArea[ChatData.CurAreaPos].width;
			back.height = ChatData.MsgPosArea[ChatData.CurAreaPos].height
			back.x = ChatData.MsgPosArea[ChatData.CurAreaPos].x+16;
			back.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y;
			if(leoView) {
				leoView.x = ChatData.MsgPosArea[ChatData.CurAreaPos].x + 16;
				leoView.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y - 34;
			}
//			iScrollPane.width = ChatData.MsgPosArea[ChatData.CurAreaPos].width;
			iScrollPane.height = ChatData.MsgPosArea[ChatData.CurAreaPos].height;
			iScrollPane.x = ChatData.MsgPosArea[ChatData.CurAreaPos].x;
			iScrollPane.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y;
			iScrollPane.setScrollPos();
			iScrollPane.refresh();
			iScrollPane.scrollBottom();
			iScrollPane.refresh();
			
			this.sendNotification( EventList.CHANGE_UI, 2 );
//			msgArea.width = 328;
		}
				
		private function showElements():void {
			var ypos:Number=1;
			for (var i:int = 0; i < msgArea.numChildren; i++) 
			{
				var child:DisplayObject = msgArea.getChildAt(i);
				child.y = ypos;
				ypos += child.height;
			}
		}
		
		private function clearAllChild():void
		{
			var index:int = msgArea.numChildren-1;		
			while(index>=0) {
				msgArea.getChildAt(index).removeEventListener(ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler);
				(msgArea.getChildAt(index) as ChatCellText).dispose();
				msgArea.removeChildAt(index);
				index--;
			}
		}
		
		private function clearFirstChild():void
		{
			if(msgArea.numChildren > 30) {
				msgArea.getChildAt(0).removeEventListener(ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler);
				(msgArea.getChildAt(0) as ChatCellText).dispose();
				msgArea.removeChildAt(0);
			}
		}
		
		private function changeMsgArea():void
		{
			switch(ChatData.CurShowContent)
			{
				case 0:
					tmpMsgList = ChatData.AllMsg;
				break;
				case 1:
					tmpMsgList = ChatData.SystemMsg;
				break;
				case 2:
					tmpMsgList = ChatData.Set1Msg;
				break;
				case 3:
					tmpMsgList = ChatData.Set2Msg;
				break;
			}
			changeCancel();
		}
		
		private function changeCancel():void
		{
			clearAllChild();
			for(var i:int = 0; i < tmpMsgList.length; i++)
			{
				var faceText:ChatCellText = new ChatCellText(tmpMsgList[i].info, tmpMsgList[i].dfColor);
				faceText.addEventListener(ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler);
				msgArea.addChild(faceText);
				faceText.x = 2;
			}
//			msgArea.width = 328;
			showElements();
			iScrollPane.refresh();
			iScrollPane.scrollBottom();
			iScrollPane.refresh();
		}
		
		public function get MsgArea():Sprite
		{
			return this.msgArea;
		}
		
		public function get ScrollPane():UIScrollPane
		{
			return this.iScrollPane;
		}		
	}
}