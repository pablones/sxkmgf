package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.FacePanelMediator;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LeoPanelMediator extends Mediator
	{
		public static const NAME:String = "LeoPanelMediator";
		private var panelBase:PanelBase;
		private var selectColorMediator:SelectColorMediator = new SelectColorMediator();
		private var selectFaceMediator:FacePanelMediator = new FacePanelMediator();
		private var curColor:uint = 0xffffff;
		private var maxItem:int = 2;
		
		private var chacheInputStr:String = "";						/** 当要册除物品的缓冲文字 */
		private var reg:RegExp = /(.*)(<[0-9\u4E00-\u9FA5]*>)$/gm;	/** 删除物品的正则表达式  */
		
		public function LeoPanelMediator()
		{
			super(NAME);
		}
		
		private function get leoPanel():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				ChatEvents.CREATELRO,
				ChatEvents.CLOSELEO,
				ChatEvents.ADD_ITEM_LEO,
				ChatEvents.SELECTEDLEOFONTCOLOR,
				ChatEvents.SELECTEDFACETOLEO
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ChatEvents.CREATELRO:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SETLEOVIEW});
					panelBase = new PanelBase(leoPanel, leoPanel.width - 26, leoPanel.height+ 14);
					panelBase.addEventListener(Event.CLOSE, panelClosed);
					panelBase.x = ChatData.tmpLeoPoint.x;
					panelBase.y = ChatData.tmpLeoPoint.y;
					if( GameCommonData.fullScreen == 2 ) panelBase.y = ChatData.tmpLeoPoint.y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_chat_com_rec_getL" ]);//"小喇叭"
					GameCommonData.GameInstance.GameUI.addChild(panelBase);
					initView();
				break;
				case ChatEvents.SELECTEDLEOFONTCOLOR:
					curColor = notification.getBody() as uint;
					ChatData.SelectedLeoColor = curColor;
					leoPanel.txtInfo.defaultTextFormat = textFormat(curColor);
					leoPanel.txtInfo.setTextFormat(textFormat(curColor));
				break;
				case ChatEvents.CLOSELEO:
					panelClosed(null);
				break;
				case ChatEvents.ADD_ITEM_LEO:			//添加物品链接到小喇叭中
					addItemInTxt(notification.getBody() as String);
				break;
				case ChatEvents.SELECTEDFACETOLEO:
					leoPanel.stage.focus = leoPanel.txtInfo;
					leoPanel.txtInfo.text += "\\" + notification.getBody() as String; 
					leoPanel.txtInfo.setSelection(leoPanel.txtInfo.length, leoPanel.txtInfo.length);
				break;
			}
		}
		
		private function initView():void
		{
			leoPanel.txtInfo.maxChars = 32;
			leoPanel.txtInfo.textColor = curColor;
			addLis();
			leoPanel.stage.focus = leoPanel.txtInfo;
			UIConstData.FocusIsUsing = true;
//			leoPanel.txtInfo.autoSize = TextFormatAlign.LEFT;
//			leoPanel.txtInfo.setSelection(leoPanel.txtInfo.length, leoPanel.txtInfo.length);
		}
		
		private function addLis():void
		{
			UIUtils.addFocusLis(leoPanel.txtInfo);
			leoPanel.addEventListener(KeyboardEvent.KEY_UP, onKeyUp);
			leoPanel.addEventListener(KeyboardEvent.KEY_DOWN,onKeyDown);
			leoPanel.txtInfo.addEventListener(Event.CHANGE, txtChgHandler);
			leoPanel.btnFace.addEventListener(MouseEvent.CLICK, showSetFace);
			leoPanel.btnColor.addEventListener(MouseEvent.CLICK, showSetColor);
			leoPanel.btnSend.addEventListener(MouseEvent.CLICK, send);
			leoPanel.btnCancel.addEventListener(MouseEvent.CLICK, cancel);
		}
		
		private function removeLis():void
		{
			UIUtils.removeFocusLis(leoPanel.txtInfo);
			leoPanel.removeEventListener(KeyboardEvent.KEY_UP, onKeyUp);
			leoPanel.removeEventListener(KeyboardEvent.KEY_DOWN,onKeyDown);
			leoPanel.txtInfo.removeEventListener(Event.CHANGE, txtChgHandler);
			leoPanel.btnFace.removeEventListener(MouseEvent.CLICK, showSetFace);
			leoPanel.btnColor.removeEventListener(MouseEvent.CLICK, showSetColor);
			leoPanel.btnSend.removeEventListener(MouseEvent.CLICK, send);
			leoPanel.btnCancel.removeEventListener(MouseEvent.CLICK, cancel);
			panelBase.removeEventListener(Event.CLOSE, panelClosed);
		}
		
		/** 添加物品链接到文字中 */
		private function addItemInTxt(itemStr:String):void
		{
			if(itemStr.split("_")[1].length + leoPanel.txtInfo.text.length >= leoPanel.txtInfo.maxChars) {
				return;
			}
			var tmpItemArr:Array = [];
			//判断输入文本框是否为空
			if(leoPanel.txtInfo.text == "") {
				ChatData.tempItemStrLeo = "";
			} else {
				ChatData.tempItemStrLeo = ChatData.tempItemStrLeo.replace(/^\s*|\s*$/g,"").split(" ").join("");
				tmpItemArr = ChatData.tempItemStrLeo.split("&");
			}
			if(tmpItemArr.length >= maxItem) return;
			
			for(var i:int = 0; i < tmpItemArr.length; i++) {
				if(itemStr == tmpItemArr[i]) return;
			}
			if(ChatData.tempItemStrLeo == "") {
				ChatData.tempItemStrLeo = itemStr;
			} else {
				ChatData.tempItemStrLeo = ChatData.tempItemStrLeo + "&" + itemStr;
			}
			var itemName:String = itemStr.split("_")[1];
			leoPanel.txtInfo.text += "<"+itemName.substring(1, itemName.length-1)+">";
		}
		
		/** 监听输入，禁止输入<> */
		private function txtChgHandler(e:Event):void
		{
			var str:String = leoPanel.txtInfo.text.substring(leoPanel.txtInfo.text.length-1, leoPanel.txtInfo.text.length)
			if(!str) return;
			if(str == "<") {	//str == ">" || 
				leoPanel.txtInfo.text = leoPanel.txtInfo.text.substring(0, leoPanel.txtInfo.text.length-1);
			}
		}
		
		/**
		 *  键般按下事件处理
		 * @param e
		 */		
		private function onKeyDown(e:KeyboardEvent):void{
			if(leoPanel.stage.focus != leoPanel.txtInfo) return;
			if(e.keyCode == 8 ){
				this.chacheInputStr = leoPanel.txtInfo.text;
			}	
		}
		/**
		 * 键般弹起事件处理
		 * @param event
		 */		
		private function onKeyUp(event:KeyboardEvent):void
		{
			if(leoPanel.stage.focus != leoPanel.txtInfo) return;
			if(event.keyCode==8) {  //删除，回退处理
				var reg1:RegExp = /(<.*?>)/g;
				var arr:Array = this.chacheInputStr.split(reg1);
				var str:String = "";
				var pos:uint;
				var flag:Boolean = false;
				var index:uint;
				for each(var s:String in arr){
					if(reg1.test(s)){
						if(!flag && leoPanel.txtInfo.caretIndex >= str.length && leoPanel.txtInfo.caretIndex < str.length + s.length){
							pos = str.length;
							flag = true;
							continue;
						}
						if(!flag){
							index++;
						}
					}
					str += s;
				}
				if(flag){
					leoPanel.txtInfo.setSelection(pos,pos);
					leoPanel.txtInfo.text = str;
					var itemArr:Array = ChatData.tempItemStrLeo.split("&");
					if(itemArr.length == 2) {
						ChatData.tempItemStrLeo = itemArr[Math.abs(index - 1)];
					} else {
						ChatData.tempItemStrLeo = "";
					}
				}
			}
		}
		
		private function showSetFace(event:MouseEvent):void
		{
			if(!UIConstData.SelectFaceIsOpen)
			{
				facade.registerMediator(selectFaceMediator);
				facade.sendNotification(EventList.SHOWFACEVIEW, {x:panelBase.x, y:panelBase.y + panelBase.height, type:"leo"});	
			}
			else
			{
				facade.sendNotification(EventList.HIDESELECTFACE);	
			}
		}
		
		private function showSetColor(event:MouseEvent):void
		{
			if(!ChatData.ColorIsOpen)
			{
				facade.registerMediator(selectColorMediator);
				facade.sendNotification(ChatEvents.SHOWCOLOR, {x:panelBase.x, y:panelBase.y + panelBase.height, target:"Leo"});	
			}
			else
			{
				facade.sendNotification(ChatEvents.HIDECOLOR);	
			}
		}
		
		private function send(event:MouseEvent):void
		{
			var sendMsg:String = leoPanel.txtInfo.text;
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg == "") {
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_chat_med_fil_send" ], color:0xffff00});//"发言内容不能为空"
				return;
			}
			var obj:Object = new Object();
			obj.type = 2030;
			obj.color = this.curColor;
			obj.name = "ALLUSER";
			obj.talkMsg = getItemStr();
			obj.item = ChatData.tempItemStrLeo;
			facade.sendNotification(ChatEvents.SENDCOMMAND, obj);
			panelClosed(null);
		}
		
		/** 获取带物品的字符串 */
		private function getItemStr():String
		{
			var result:String = "";
			var msg:String = leoPanel.txtInfo.text.split("\r").join("");
			ChatData.tempItemStrLeo = ChatData.tempItemStrLeo.replace(/^\s*|\s*$/g,"").split(" ").join("");
			if(ChatData.tempItemStrLeo) {
				var reg:RegExp=/(<.*?>)/g;
				var arr:Array = ChatData.tempItemStrLeo.split("&");
				var targetArr:Array = msg.split(reg);
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
				result = msg;
			}
			return result;
		}
		
		private function cancel(event:MouseEvent):void
		{
			panelClosed(null);
		}
		
		private function textFormat(color:uint=0x000000):TextFormat
		{
			var tf:TextFormat = new TextFormat();
			tf.color = color;
			return tf;
		}
		
		private function panelClosed(e:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				removeLis();
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				ChatData.tmpLeoPoint = new Point(panelBase.x, panelBase.y);
				panelBase = null;
				this.viewComponent = null;
				ChatData.tempItemStrLeo = "";
				chacheInputStr = "";
				ChatData.SetLeoIsOpen = false;
				UIConstData.FocusIsUsing = false;
				facade.removeMediator(NAME);
			}
		}
	}
}