package GameUI.Modules.Stall.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Chat.UI.FaceText;
	import GameUI.Modules.Chat.UI.ItemLinkEvent;
	import GameUI.Modules.Chat.UI.TextLinkEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.Stall.Proxy.StallNetAction;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.ItemInfo;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StallBBSMediator extends Mediator
	{
		public static const NAME:String = "StallBBSMediator";
		public static const TALK_MAX_CHAR:uint = 64;			/** 留言一句话的字数限制，32个中文，64个英文 */
		public static const INFO_MAX_CHAR:uint = 64;			/** 摊位信息的字数限制，32个中文，64个英文 */
		
		private var panelBase:PanelBase = null;
		
		private var msgArea:Sprite;
		private var iScrollPane:UIScrollPane;
		
		private var ownerId:int = 0;
		
		public function StallBBSMediator()
		{
			super(NAME);
		}
		
		private function get stallBBS():MovieClip
		{
			return this.viewComponent as MovieClip
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				StallEvents.SHOWSTALLBBS,	//显示BBS
				StallEvents.REMOVESTALLBBS,	//移除BBS
				StallEvents.UPDATESTALLINFO,//更新摊位说明信息
				StallEvents.UPDATESTALLMSG	//更新留言
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case StallEvents.SHOWSTALLBBS:
					ownerId = int(notification.getBody());
//					trace("ownerId:",ownerId);
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.STALLBBS});
					panelBase = new PanelBase(stallBBS, stallBBS.width+8, stallBBS.height+12);
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_stall_med_sbbsm_1" ]);   //"摊位信息"
					GameCommonData.GameInstance.GameUI.addChild(panelBase); 
					panelBase.x = StallConstData.BBS_DEFAULT_POS.x;
					panelBase.y = StallConstData.BBS_DEFAULT_POS.y;
					initView();
					StallConstData.StallBBSisOpen = true;
					break;
				case StallEvents.REMOVESTALLBBS:
					gc();
					break;
				case StallEvents.UPDATESTALLINFO:
					if(StallConstData.stallInfo) stallBBS.txtStallInfo.text = StallConstData.stallInfo.toString();
					break;
				case StallEvents.UPDATESTALLMSG:
					if ( notification.getBody() )
					{
						showChat( notification.getBody().toString() );
					}
					else
					{
						show();
					}
					break;
			}
		}
		
		private function initView():void
		{
			createMsgArea();
			stallBBS.txtInputMsg.text = "";
			stallBBS.txtModifyStallInfo.mouseEnabled = false;
			stallBBS.txtClearMsg.mouseEnabled = false;
			(ownerId!=0 && ownerId == StallConstData.stallSelfId) ? setModel(0) : setModel(1);
			//初始化 摊位信息数据、留言数据
			if(StallConstData.stallInfo) stallBBS.txtStallInfo.text = StallConstData.stallInfo.toString();
			show();
		}
		
		/** 关闭留言板 */
		private function panelCloseHandler(e:Event):void
		{
			gc();
		}
		
        /** 点击按钮  更改信息、清除留言、发送留言 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btnSendMsg":												//发送留言
					var msg:String = stallBBS.txtInputMsg.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(msg) {
						//发送 留言命令
						msg = UIUtils.filterChat(msg);
						StallNetAction.OperateMsg(2033, msg, ownerId);
						stallBBS.txtInputMsg.text = "";
					}else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sbbsm_2" ], color:0xffff00});    // "留言内容不能为空"
					}
					break;
				case "btnModifyStallInfo":										//更改摊位信息
					var msgInfo:String = stallBBS.txtStallInfo.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(msgInfo && msgInfo != StallConstData.stallInfo) {
						//发送 修改摊位信息命令
						if(!UIUtils.checkChat(msgInfo)) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sbbsm_3" ], color:0xffff00});   //"含有非法字符"
							stallBBS.txtStallInfo.text = StallConstData.stallInfo;
						} else {
							StallNetAction.OperateMsg(2032, msgInfo, ownerId);
							stallBBS.txtStallInfo.text = msgInfo;
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sbbsm_4" ], color:0xffff00});   //"摊位信息修改成功"
						}
					}
					break;
				case "btnClearMsg":												//清除留言
					if(StallConstData.stallMsg.length > 0) {
						//发送 清除留言命令
						StallNetAction.OperateItem(104,0,0,0,null);
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sbbsm_5" ], color:0xffff00});     //"所有留言已清除"
					}
					break;
			}
		}
		
		/** 留言输入框，限制字数 */
		private function txtChangeHandler(e:Event):void
		{
			switch(e.target.name)
			{
				case "txtInputMsg":												//留言输入
					var msg:String = stallBBS.txtInputMsg.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(msg) {
						stallBBS.txtInputMsg.text = UIUtils.getTextByCharLength(msg, TALK_MAX_CHAR);
					}
					break;
				case "txtStallInfo":											//摊位信息输入
					var msgInfo:String = stallBBS.txtStallInfo.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(msgInfo) {
						stallBBS.txtStallInfo.text = UIUtils.getTextByCharLength(msgInfo, INFO_MAX_CHAR);
					}
					break;
			}
		}
		
        /** 留言板模式，0=摊主，1=买家 */
        private function setModel(type:uint=0):void
		{
			if(type == 0) {
				stallBBS.txtClearMsg.visible = true;
				stallBBS.txtModifyStallInfo.visible = true;
				stallBBS.btnClearMsg.visible = true;
				stallBBS.btnModifyStallInfo.visible = true;
				stallBBS.txtStallInfo.mouseEnabled = true;
				stallBBS.btnClearMsg.addEventListener(MouseEvent.CLICK, btnClickHandler);
				stallBBS.btnModifyStallInfo.addEventListener(MouseEvent.CLICK, btnClickHandler);
				stallBBS.txtStallInfo.addEventListener(Event.CHANGE, txtChangeHandler);
			} else {
				stallBBS.txtStallInfo.mouseEnabled = false;
				stallBBS.btnClearMsg.visible = false;
				stallBBS.txtClearMsg.visible = false;
				stallBBS.btnModifyStallInfo.visible = false;
				stallBBS.txtModifyStallInfo.visible = false;
			}
			stallBBS.txtInputMsg.addEventListener(Event.CHANGE, txtChangeHandler);
//			stallBBS.txtInputMsg.addEventListener(KeyboardEvent.KEY_DOWN, keyHandler);
			stallBBS.btnSendMsg.addEventListener(MouseEvent.CLICK, btnClickHandler);
			
			stallBBS.txtInputMsg.addEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			stallBBS.txtStallInfo.addEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			stallBBS.txtInputMsg.addEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			stallBBS.txtStallInfo.addEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
		}
		
		/** 移除事件监听 */
		private function removeLis():void
		{
			if(stallBBS.btnClearMsg.hasEventListener(MouseEvent.CLICK)) stallBBS.btnClearMsg.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stallBBS.btnModifyStallInfo.hasEventListener(MouseEvent.CLICK)) stallBBS.btnModifyStallInfo.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stallBBS.btnSendMsg.hasEventListener(MouseEvent.CLICK)) stallBBS.btnSendMsg.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stallBBS.txtStallInfo.hasEventListener(Event.CHANGE)) stallBBS.txtStallInfo.removeEventListener(Event.CHANGE, txtChangeHandler);
			if(stallBBS.txtInputMsg.hasEventListener(Event.CHANGE)) stallBBS.txtInputMsg.removeEventListener(Event.CHANGE, txtChangeHandler);
//			if(stallBBS.txtInputMsg.hasEventListener(KeyboardEvent.KEY_DOWN)) stallBBS.txtInputMsg.removeEventListener(KeyboardEvent.KEY_DOWN, keyHandler);
			
			if(stallBBS.txtInputMsg.hasEventListener(FocusEvent.FOCUS_IN)) stallBBS.txtInputMsg.removeEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			if(stallBBS.txtStallInfo.hasEventListener(FocusEvent.FOCUS_IN)) stallBBS.txtStallInfo.removeEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			if(stallBBS.txtInputMsg.hasEventListener(FocusEvent.FOCUS_OUT)) stallBBS.txtInputMsg.removeEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			if(stallBBS.txtStallInfo.hasEventListener(FocusEvent.FOCUS_OUT)) stallBBS.txtStallInfo.removeEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
		}

		private function focusInHandler(e:FocusEvent):void
		{
			GameCommonData.isFocusIn = true;
		}
		private function focusOutHandler(e:FocusEvent):void
		{
			GameCommonData.isFocusIn = false;
		}
		
		/** 回车发送留言 */
		private function keyHandler(e:KeyboardEvent):void {
			var code:int = e.keyCode;
			if(code == 13) {
				var msg:String = stallBBS.txtInputMsg.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
				if(msg) {
					//发送 留言命令
					msg = UIUtils.filterChat(msg);
					StallNetAction.OperateMsg(2033, msg, ownerId);
					stallBBS.txtInputMsg.text = "";
				} else {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sbbsm_2" ], color:0xffff00});   //"留言内容不能为空"
				}
			}
		}
		
		/** GC */		
		private function gc():void
		{
			ownerId = 0;
			clearAllChild();
			msgArea = null;
			iScrollPane = null;
			removeLis();
			this.viewComponent = null;
			StallConstData.StallBBSisOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			panelBase = null;
			facade.removeMediator(NAME);
		}
		
		/** 创建留言区 */
		private function createMsgArea():void
		{
			msgArea = new Sprite();
			msgArea.graphics.beginFill(0xff00ff, 0);
			msgArea.graphics.drawRect(0, 0, 196, 150);
			msgArea.graphics.endFill();
			
			iScrollPane = new UIScrollPane(msgArea);
		 	iScrollPane.width = 211;
			iScrollPane.height = 150;
			iScrollPane.x = 9;
			iScrollPane.y = 111;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
//			iScrollPane.setScrollPos();		//滚动条在左边显示
			iScrollPane.refresh();
			stallBBS.addChild(iScrollPane);
		}
		
		private function show():void
		{
			clearAllChild();
			if(StallConstData.stallMsg.length == 0) {
//				var faceTextNull:FaceText = new FaceText();
//				faceTextNull.SetHtmlText("", "");
//				msgArea.addChild(faceTextNull);
				stallBBS.txtStaticInfo.visible = true;
			} else {
				stallBBS.txtStaticInfo.visible = false;
				if(StallConstData.stallMsg.length > 20) StallConstData.stallMsg.splice(0, 1);
				for(var i:int = 0; i < StallConstData.stallMsg.length; i++)
				{
					var faceText:FaceText = new FaceText();
					faceText.SetHtmlText(StallConstData.stallMsg[i], "",true, 200);
//					faceText.addEventListener(TextLinkEvent.EVENTNAME, getLinkName);
					faceText.addEventListener(ItemLinkEvent.EVENTNAME, getLinkName);
					msgArea.addChild(faceText);
				}
			}
			msgArea.width = 200;
			showElements();
			iScrollPane.scrollBottom();
			iScrollPane.refresh();
		}
		
		private function showChat( bossName:String ):void
		{
			if ( bossName != StallConstData.stallOwnerName ) return;
			clearAllChild();
			if(StallConstData.stallMsg.length == 0) {
				stallBBS.txtStaticInfo.visible = true;
			} else {
				stallBBS.txtStaticInfo.visible = false;
				if(StallConstData.stallMsg.length > 20) StallConstData.stallMsg.splice(0, 1);
				for(var i:int = 0; i < StallConstData.stallMsg.length; i++)
				{
					var faceText:FaceText = new FaceText();
					faceText.SetHtmlText(StallConstData.stallMsg[i], "",true, 200);
					faceText.addEventListener(ItemLinkEvent.EVENTNAME, getLinkName);
					msgArea.addChild(faceText);
				}
			}
			msgArea.width = 200;
			showElements();
			iScrollPane.scrollBottom();
			iScrollPane.refresh();
		}
		
		private function getLinkName(e:ItemLinkEvent):void
		{
//			trace("getLinkName");
			var type:String = e.name.split("_")[0];	
			switch(type)
			{
				case "name":
					if(ChatData.QuickChatIsOpen) return;
					var playerName:String  = e.name.split("_")[1];
					if(playerName != GameCommonData.Player.Role.Name) {
					 	var quickSelectMediator:QuickSelectMediator = new QuickSelectMediator();
					 	facade.registerMediator(quickSelectMediator);
						facade.sendNotification(ChatEvents.SHOWQUICKOPERATOR, playerName);
					}
					break;
				case "item":
					var itemOwerId:uint = uint(e.name.split("_")[1]);
					var itemId:uint = uint(e.name.split("_")[2]);
//					trace("============================================");
//					trace("itemId:",itemId,"ownerId:",itemOwerId);
					if(itemId >= 2000000000 && itemId <= 3999999999) {	//宠物
						if(GameCommonData.Player.Role.Id != itemOwerId ) {
							facade.sendNotification(PetEvent.PET_LOOK_OUTSIDE_INTERFACE, {petId:itemId, ownerId:itemOwerId});
						}
						return;
					}
					ItemInfo.isPanel = true;
					UiNetAction.GetItemInfo(itemId, itemOwerId);
					break;
			}
		}
		
		private function showElements():void {
			var ypos:Number=0;
			for (var i:int = 0; i < msgArea.numChildren; i++) 
			{
				var child:DisplayObject = msgArea.getChildAt(i);
				child.y = ypos;
				ypos += child.height + 2;
			}
		}
		
		private function clearAllChild():void
		{
			var index:int = msgArea.numChildren-1;		
			while(index>=0) {
				msgArea.getChildAt(index).removeEventListener(TextLinkEvent.EVENTNAME, getLinkName);
				msgArea.removeChildAt(index);
				index--;
			}
		}
		
		
		
	}
}




