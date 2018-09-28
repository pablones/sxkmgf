package GameUI.Modules.Hint.Mediator.UI
{
	import OopsEngine.Graphics.Font;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	public class HintView extends Sprite
	{
		private var delayID:uint 	= 0;
		private var fadeID:uint 	= 0;
		private var delayNum:uint 	= 0;
		private var delayCount:uint = 4000;
		private var fadeCount:uint = 100;
		private var content:String = "";
		private var textFeild:TextField	= null;
		private var color:uint = 0xffff00;
		
		public var Pos:int = 0;
		
		public function HintView(info:String, color:uint)
		{
			this.content = info;	
			this.color = color;
			this.mouseEnabled = false;
		}
		
		public function StartShow():void
		{
			delayID = setInterval(clearShow, delayCount);
			textFeild = new TextField();
			textFeild.defaultTextFormat = new TextFormat(GameCommonData.wordDic["mod_cam_med_ui_UIC1_getI"], 12);//"宋体" 
			textFeild.width = 469;
			textFeild.mouseEnabled = false;
			textFeild.selectable = false;
			textFeild.textColor = this.color;
			textFeild.htmlText = content;
			textFeild.autoSize = TextFieldAutoSize.CENTER; 
			textFeild.filters = Font.Stroke(0);
			this.addChild(textFeild);
		}
		
		private function clearShow():void
		{
			clearInterval(delayID);
			fadeID = setInterval(fadeShow, fadeCount);			
		}
		
		private function fadeShow():void
		{
			if(textFeild.alpha <= 0)
			{
				clearInterval(fadeID);
				textFeild.text = "";
				this.removeChild(textFeild);
				textFeild = null;
				this.dispatchEvent(new Event(Event.COMPLETE));
				return;
			}
			textFeild.alpha -= 0.1;
		}
	}
}