package GameUI.Modules.ChangeLine.View
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class ChgListCell extends Sprite
	{
		private var content_mc:MovieClip;
		private var content_txt:TextField;
		private var format:TextFormat;
		
		public var sName:String;
		private var isFull:String;
		public var gsName:String = "";
		
		public function ChgListCell(_sName:String,_isFull:String)
		{
			sName = _sName;
			isFull = _isFull;
			initUI();
		}
		
		private function initUI():void
		{
			this.content_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MenuItemRenderer");
			
			content_mc.width = 89;
			this.addChild(content_mc);
			content_mc.txt_name.mouseEnabled = false;
			content_mc.txt_name.autoSize = TextFieldAutoSize.LEFT;
			content_mc.txt_name.htmlText = sName+isFull;
			content_mc.txt_name.x = 0;
			content_mc.txt_name.y = 1;
		}
	}
}