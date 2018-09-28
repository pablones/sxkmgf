/**---------  消息条目对象  ---------------**/
package GameUI.Modules.SystemMessage.UI
{
	import GameUI.Modules.SystemMessage.Data.SysMessageData;
	import GameUI.Modules.SystemMessage.Mediator.SystemMediator;
	import GameUI.Modules.SystemMessage.Memento.MessageMemento;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;

	public class MessageItem extends Sprite
	{
		public var sysMessage:MessageMemento;
		public var countClick:int = 0;
		public static var clorWidth:int  = 478;
		public static var clorHeight:int = 20;
		private var systemMediator:SystemMediator;
		private var currentThis:MessageItem;
		private var titleField:TextField;
		private var timeField:TextField;
		private var stateField:TextField;
		private var numField:TextField;
		
		public var selectFunction:Function; 
		public var openFunction:Function 
		public function MessageItem(message:MessageMemento)
		{
			super();
			message.setSateFun = setSateFun;
			sysMessage = message;
			showTxt();
			addLis();
			init();
		}
		public function init():void
		{
			this.mouseChildren = false;
			this.name = "MessageItem_" + sysMessage.index;
			systemMediator = GameCommonData.UIFacadeIntance.retrieveMediator(SystemMediator.NAME) as SystemMediator;
			currentThis = systemMediator.megView.getChildByName("MessageItem_" + sysMessage.index) as MessageItem;
			if(currentThis == null) currentThis = this;
			if(SysMessageData.selectItemIndex == sysMessage.index)	//如果有选中状态就默认颜色	
			{
				selected();
			}
			this.graphics.clear();
            this.graphics.beginFill(0x4d3c13, 0);
            this.graphics.drawRect(0, 0, clorWidth, clorHeight);
            this.graphics.endFill();
		}
		public function addLis():void
		{
			this.addEventListener(MouseEvent.CLICK		, clickHandler);
			this.addEventListener(MouseEvent.MOUSE_OVER , overHandler);
			this.addEventListener(MouseEvent.MOUSE_OUT  , outHandler);
		}
		/** 文本初始化*/
		private function txtInit(textField:TextField , x:int , y:int):void
		{
			textField.filters 	= Font.Stroke(0);
			textField.type		= TextFieldType.DYNAMIC;
			textField.autoSize	= TextFieldAutoSize.LEFT;
			textField.textColor = 0xFFFFFF;
			textField.defaultTextFormat = new TextFormat("宋体" , 12);
			textField.x = x;
			textField.y = y;
			textField.mouseEnabled = false;
		}
		public function showTxt():void
		{
			titleField 	= new TextField();
			timeField 	= new TextField();
			stateField 	= new TextField();
			numField 	= new TextField();
			this.addChild(titleField);
			this.addChild(timeField);
			this.addChild(stateField);
			this.addChild(numField);
			txtInit(titleField , 35 , 1);
			txtInit(timeField , 155 , 1);
			txtInit(stateField , 350, 1);
			txtInit(numField , 425, 1);
			titleField.htmlText = sysMessage.title;
			timeField.htmlText 	= sysMessage.timeStr;
			stateField.htmlText = sysMessage.state;
			var index:int = (SysMessageData.messageList.length > SysMessageData.messageTotalNum ? SysMessageData.messageTotalNum:SysMessageData.messageList.length);
			numField.text   = (index - sysMessage.index + 1) + "/" + SysMessageData.messageTotalNum;
		}
		private function overHandler(e:MouseEvent):void
		{
			this.graphics.clear();
            this.graphics.beginFill(0x4d3c13, 7);
            this.graphics.drawRect(0, 0, clorWidth, clorHeight);
            this.graphics.endFill();
		}
		private function outHandler(e:MouseEvent):void
		{
			this.graphics.clear();
            this.graphics.beginFill(0x4d3c13, 0);
            this.graphics.drawRect(0, 0, clorWidth, clorHeight);
            this.graphics.endFill();
		}
		private function clickHandler(e:MouseEvent):void
		{
			countClick++;
			var id:int;
			if (countClick == 1) {
				id = setTimeout(selected , 200);
				e.stopPropagation();
			} 
			else if (countClick == 2)
			{
				SysMessageData.selectItemIndex = sysMessage.index;		//选中的消息序列
				countClick = 0;
				clearTimeout(id);
				e.stopPropagation();
				if(openFunction != null) openFunction();
//				openMessage();			//打开消息内容面板
			}
			
		}
		/** 选中 */
		private function selected():void
		{
			clear();
			this.removeEventListener(MouseEvent.MOUSE_OVER , overHandler);
			this.removeEventListener(MouseEvent.MOUSE_OUT  , outHandler);
			this.countClick = 0;
			this.graphics.clear();
			this.graphics.beginFill(0xfffe65,.7);
			this.graphics.drawRect(0,0,clorWidth,clorHeight);
			this.graphics.endFill();
			//选中的消息序列
//			SysMessageData.selectItemIndex = sysMessage.index;
			
			if(selectFunction != null) selectFunction(this);
		}
		/** 清除选中 */
		public  function clear():void
		{
		}
		/** 移除事件监听 */
		public function removeEvent():void
		{
			this.removeEventListener(MouseEvent.MOUSE_OUT  , outHandler);
			this.removeEventListener(MouseEvent.MOUSE_OVER , overHandler);
		}
		/** 状态改变时文本颜色也要变 */
		private function setSateFun():void
		{
			stateField.htmlText = sysMessage.state;
		}
	}
}