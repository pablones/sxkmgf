package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.FacePanelMediator;
	import GameUI.Modules.Arena.Command.ArenaPanelCommandList;
	import GameUI.Modules.Arena.Data.ArenaScore;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.UI.ChatCellText;
	import GameUI.Modules.Chat.UI.TextFieldColor;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ArenaMsgMediator extends Mediator
	{
		public static const NAME:String = "ArenaMsgMediator"
		
		public var DEFAULT_POS:Point = new Point(747, 329);  /* 747 */
		
		public function ArenaMsgMediator()
		{
			super(NAME);
		}
		
		override public function listNotificationInterests():Array
		{
			return [ChatEvents.ARENA_MSG_PANEL_OPEN,
						ChatEvents.ARENA_MSG_PANEL_CLOSE,
						ChatEvents.ARENA_MESSAGE,
						ChatEvents.SELECTEDFACETOARENACHAT,
						ChatEvents.ADDITEMINARENACHAT,
						EventList.KEYBORADEVENT];
		}
		
		public var panel:MovieClip;
		protected var initialized:Boolean = false;
		protected var dataProxy:DataProxy;
		protected var msgScrollPane:UIScrollPane;
		protected var msgSprite:Sprite;
		protected var textFieldColor:TextFieldColor;
		protected var facePanel:FacePanelMediator = new FacePanelMediator();
		
		protected const MAX_ITEM_COUNT:int = 2;
		
		override public function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case ChatEvents.ARENA_MSG_PANEL_OPEN:
					if (!initialized)
					{
						ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ArenaMsgPanel.swf", onPanelLoadComplete);
					}
					else
					{
						show();
					}
				break;
				case ChatEvents.ARENA_MSG_PANEL_CLOSE:
					if (initialized)
					{
						hide();
					}
				break;
				case ChatEvents.ARENA_MESSAGE:
					if (initialized)
					{
						var msgBody:Object = notification.getBody(); 
						var msgText:ChatCellText;
						if (msgBody.nAtt == 9999)
						{
							msgText = new ChatCellText('<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_exe_1" ]+']_7>' + msgBody.htmlText, 0xE2CCA5, 232);
						}
						else
						{
							msgText = new ChatCellText(msgBody.info, msgBody.dfColor, 232);
						}
						msgSprite.addChild(msgText);
						msgText.x = 2;
						arrangeText();
						msgScrollPane.refresh();
						msgScrollPane.scrollBottom();
						msgScrollPane.refresh();
					}
				break; 
				case ChatEvents.SELECTEDFACETOARENACHAT:
					panel.stage.focus = panel.panelBig.txtInput;
					(panel.panelBig.txtInput as TextField).appendText("\\" + notification.getBody() as String); 
					panel.panelBig.txtInput.setSelection(panel.panelBig.txtInput.length, panel.panelBig.txtInput.length);
				break;
				case EventList.KEYBORADEVENT:
					if (notification.getBody() == 13 && !UIConstData.FocusIsUsing && dataProxy.lastUsedMsgPanel == "arena")
					{
						panel.stage.focus = panel.panelBig.txtInput;
						panel.panelBig.txtInput.setSelection(panel.panelBig.txtInput.length, panel.panelBig.txtInput.length);
						return;
					}
					if ((notification.getBody() == 13) && dataProxy && dataProxy.arenaMsgPanelIsTyping)
					{
						sendMsg();
						return;						
					}
					if ((notification.getBody() == 27) && dataProxy.arenaMsgPanelIsTyping)
					{
						panel.panelBig.txtInput.text = "";
						panel.stage.focus = null;
						UIConstData.FocusIsUsing = false;
						dataProxy.lastUsedMsgPanel = "arena";
						return;
					}
				break;
				case ChatEvents.ADDITEMINARENACHAT:
					var itemStr:String = notification.getBody() as String;
					if(itemStr.split("_")[1].length + panel.panelBig.txtInput.text.length >= panel.panelBig.txtInput.maxChars) {
						return;
					}
					var tmpItemArr:Array = [];
					//判断输入文本框是否为空
					if(panel.panelBig.txtInput.text == "") {
						ChatData.tempItemStr = "";
					} else {
						ChatData.tempItemStr = ChatData.tempItemStr.replace(/^\s*|\s*$/g,"").split(" ").join("");
						tmpItemArr = ChatData.tempItemStr.split("&");
					}
					if(tmpItemArr.length >= MAX_ITEM_COUNT) return;
					
					for(var i:int = 0; i < tmpItemArr.length; i++) {
						if(itemStr == tmpItemArr[i]) return;
					}
					if(ChatData.tempItemStr == "") {
						ChatData.tempItemStr = itemStr;
					} else {
						ChatData.tempItemStr = ChatData.tempItemStr + "&" + itemStr;
					}
					var itemName:String = itemStr.split("_")[1];
					panel.panelBig.txtInput.text += "<"+itemName.substring(1, itemName.length-1)+">";
				break;
			}
		}
		
		private function arrangeText():void
		{
			var delCount:int = Math.max(0, msgSprite.numChildren - 30);
			for (var i:int = 0; i < delCount; i ++)
			{
				msgSprite.removeChildAt(0);
			}
			
			var ypos:Number=0;
			for (var j:int = 0; j < msgSprite.numChildren; j++) 
			{
				var child:DisplayObject = msgSprite.getChildAt(j);
				child.y = ypos;
				ypos += child.height;
			}
		}
		
		protected function sendMsg():void
		{
			dataProxy.lastUsedMsgPanel = "arena";
			if (!ArenaScore.initialized) return;
			
			var msgToSend:String = panel.panelBig.txtInput.text.replace(/^\s*/g, "").replace(/\s*$/g, "");
			if(msgToSend == "") 
			{
				facade.sendNotification(ChatEvents.ARENA_MESSAGE,{htmlText:GameCommonData.wordDic[ "mod_chat_med_cha_send" ], name:"", nAtt:9999});//"发言内容不能为空"
				return;
			}
			
			var obj:Object = {};
			
			obj.jobId = 0;
			obj.name = "ALLUSER";
			obj.type = 2046;
			obj.talkMsg = getItemStr();
			obj.color = 0xFFFFFF;
			obj.item = "";
			facade.sendNotification(ChatEvents.SENDCOMMAND, obj);
			
			panel.panelBig.txtInput.text = "";
		}
		
		protected function getItemStr():String
		{
			var result:String = "";
			ChatData.tempItemStr = ChatData.tempItemStr.replace(/^\s*|\s*$/g,"").split(" ").join("");
			if(ChatData.tempItemStr) {
				var reg:RegExp = /(<.*?>)/g;
				var arr:Array  = ChatData.tempItemStr.split("&");
				var targetArr:Array = panel.panelBig.txtInput.text.split(reg);
				var pos:uint = 0;
				for each(var s:String in targetArr) {
					if(reg.test(s)) {
						result += arr[pos];
						pos++;
					} else {
						result += s;
					}
				}
			} else {
				result = panel.panelBig.txtInput.text;
			}
			while(result.indexOf("undefined") >= 0) {
				result = result.replace("undefined", "");
			}
			
			var regL:RegExp = /(<.*?>)/g;
			var textArr:Array = result.split(regL);
			
			for each(var sT:String in textArr) {
				if(sT != "") {
					if(regL.test(sT)) {
						if(ChatData.tempItemStr.indexOf(sT) < 0)
							result = result.replace(sT, "");
					}
				}
			}
			
			return result;
		}
		
		protected function onPanelLoadComplete():void
		{
			var panelSwf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ArenaMsgPanel.swf");
			panel = new (panelSwf.loaderInfo.applicationDomain.getDefinition("Panel"));
			this.setViewComponent(panel);
			
			initPanel();
			resetPanel();
			
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			initialized = true;
			show();
		}
		
		protected function initPanel():void
		{
			msgSprite = new Sprite();
			msgSprite.graphics.beginFill(0xFFFFFF, 0);
			msgSprite.graphics.drawRect(0, 0, 232, 105);
			msgSprite.graphics.endFill();
			msgSprite.y = 23;
			
			msgScrollPane = new UIScrollPane(msgSprite);
			msgScrollPane.width = 250;
			msgScrollPane.height = 105;
			msgScrollPane.x = 0;
			msgScrollPane.y = 23;
			panel.panelBig.addChild(msgScrollPane);
			
			panel.panelSmall.btnUnfold.addEventListener(MouseEvent.CLICK, fold);
			panel.panelBig.btnFold.addEventListener(MouseEvent.CLICK, fold);
			panel.panelBig.mcMove.addEventListener(MouseEvent.MOUSE_DOWN, onDragMcMouseDown);
			
			panel.panelBig.txtInput.maxChars = 50;
			panel.panelBig.txtInput.addEventListener(FocusEvent.FOCUS_IN, txtInput_focusInHandler);
			panel.panelBig.txtInput.addEventListener(FocusEvent.FOCUS_OUT, txtInput_focusOutHandler);
			panel.panelBig.txtInput.addEventListener(Event.CHANGE, txtInput_changeHandler);
			
			panel.panelBig.btnSendMsg.addEventListener(MouseEvent.CLICK, sendMsg_clickHandler);
			
			panel.panelBig.mcMove.stop();
			
			panel.panelBig.mcEmoticon.addEventListener(MouseEvent.CLICK, mcEmoticon_clickHandler); 
			
			textFieldColor = new TextFieldColor(panel.panelBig.txtInput as TextField, 0xFFFFFF, 0x3399FF, 0xFFFFFF);
			
			GameCommonData.UIFacadeIntance.sendNotification(ArenaPanelCommandList.ARENASMALLPANEL_CALL_UPDATE);
		}
		
		protected function mcEmoticon_clickHandler(event:MouseEvent):void
		{
			if (!UIConstData.SelectFaceForArenaIsOpen)
			{
				var pos:Point = panel.panelBig.mcEmoticon.localToGlobal(new Point(panel.panelBig.mcEmoticon.width, 0));
				facade.registerMediator(facePanel);
				facade.sendNotification(EventList.SHOWFACEVIEW, {type: "arena", x: pos.x - 200, y: pos.y - 106});
			}
			else
			{
				facade.sendNotification(EventList.HIDESELECTFACE);
			}
		}
		
		protected function sendMsg_clickHandler(event:MouseEvent):void
		{
			sendMsg();						
		}
		
		protected function resetPanel():void
		{
			panel.panelSmall.visible = false;
			panel.x = DEFAULT_POS.x;
			panel.y = DEFAULT_POS.y;
			sendNotification(EventList.STAGECHANGE);
		}
		
		protected function txtInput_focusInHandler(event:FocusEvent):void
		{
			GameCommonData.isFocusIn = UIConstData.FocusIsUsing = ChatData.txtIsFoucs = dataProxy.arenaMsgPanelIsTyping = true;
		}
		
		protected function txtInput_focusOutHandler(event:FocusEvent):void
		{
			GameCommonData.isFocusIn = UIConstData.FocusIsUsing = ChatData.txtIsFoucs = dataProxy.arenaMsgPanelIsTyping = false;;
		}
		
		protected function txtInput_changeHandler(event:Event):void
		{
			var str:String = panel.panelBig.txtInput.text.substring(panel.panelBig.txtInput.text.length-1, panel.panelBig.txtInput.text.length)
			if(!str) return;
			if(str == "<") {	//str == ">" || 
				panel.panelBig.txtInput.text = panel.panelBig.txtInput.text.substring(0, panel.panelBig.txtInput.text.length-1);
			}
		}
		
		protected var anchorPoint:Point;
		protected var dragBoundMinX:Number;
		protected var dragBoundMaxX:Number;
		protected var dragBoundMinY:Number;
		protected var dragBoundMaxY:Number;
		
		protected function onDragMcMouseDown(event:MouseEvent):void
		{
			dragBoundMinX = 0;
			dragBoundMaxX = GameCommonData.GameInstance.GameUI.stage.stageWidth - this.panel.panelBig.width; //GameCommonData.GameInstance.GameUI.width;
			dragBoundMinY = 0;
			dragBoundMaxY = GameCommonData.GameInstance.GameUI.stage.stageHeight - this.panel.panelBig.height; //GameCommonData.GameInstance.GameUI.height;
					
			this.anchorPoint = new Point(this.panel.mouseX, this.panel.mouseY);
			this.panel.stage.addEventListener(MouseEvent.MOUSE_MOVE, dragUIHandler);
			this.panel.stage.addEventListener(MouseEvent.MOUSE_UP, stopDragUIHandler);
			this.panel.panelBig.mcMove.gotoAndStop(2);
		}
		
		protected function dragUIHandler(event:MouseEvent):void
		{
			var actualX:Number = event.stageX - anchorPoint.x;
			var actualY:Number = event.stageY - anchorPoint.y;
			
			actualX = Math.max(dragBoundMinX, actualX);
			actualX = Math.min(dragBoundMaxX, actualX);
			
			actualY = Math.max(dragBoundMinY, actualY);
			actualY = Math.min(dragBoundMaxY, actualY); 
			
			this.panel.x = actualX;
			this.panel.y = actualY;
		}
		
		protected function stopDragUIHandler(event:MouseEvent):void
		{
			this.panel.stage.removeEventListener(MouseEvent.MOUSE_MOVE, dragUIHandler);
			this.panel.stage.removeEventListener(MouseEvent.MOUSE_UP, stopDragUIHandler);
			this.panel.panelBig.mcMove.gotoAndStop(1);
		}
		
		protected function show():void
		{
			if (!GameCommonData.GameInstance.GameUI.contains(this.panel))
				GameCommonData.GameInstance.GameUI.addChild(this.panel);
				
			resetPanel();
			
			dataProxy.arenaMsgPanelIsOpen = true;
		}
		
		protected function hide():void
		{
			dataProxy.arenaMsgPanelIsOpen = false;
			if (GameCommonData.GameInstance.GameUI.contains(this.panel))
				GameCommonData.GameInstance.GameUI.removeChild(this.panel);
		}
		
		protected function fold(event:Event):void // 实际作用是双状态切换
		{
			panel.panelSmall.visible = !panel.panelSmall.visible;
			panel.panelBig.visible = !panel.panelBig.visible;
			panel.x = DEFAULT_POS.x;
			panel.y = DEFAULT_POS.y;
			if (!panel.panelSmall.visible) dataProxy.lastUsedMsgPanel = "chat";
			sendNotification(EventList.STAGECHANGE);
		}
	}
}