package GameUI.Modules.Soul.Components
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class DownListCell extends Sprite
	{
		protected var content_mc:MovieClip;
		protected var contentStr:String;
		public var clickFun:Function;
		
		private var title_txt:TextField;
		
		public function DownListCell( sName:String,mWidth:Number=80,mHeight:Number=20 )
		{
			contentStr = sName;
//			var format:TextFormat = new TextFormat();
//			format.size = 12;
//			format.color = 0xE2CCA5;
//			format.font = "宋体";
			content_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MenuItemRenderer");
			content_mc.width = mWidth - 2;
			content_mc.height = mHeight - 2;
//			content_mc.txt_name.htmlText = sName;
//			( content_mc.txt_name as TextField ).setTextFormat( format );
//			content_mc.txt_name.x = 0;
//			content_mc.txt_name.x = 25;
//			content_mc.txt_name.autoSize = TextFieldAutoSize.CENTER;
//			content_mc.txt_name.y = 1;
//			content_mc.txt_name.width = mWidth-5;
			content_mc.txt_name.visible = false;
			content_mc.txt_name.mouseEnabled = false;
			
			this.addChild( content_mc );
			
			var format:TextFormat = new TextFormat();
			format.size = 12;
			format.font = "宋体";//"宋体";
//			format.color = 0xE2CCA5;
			format.color = 0xffffff;
			
			this.title_txt = new TextField();
			title_txt.mouseEnabled = false;
			title_txt.width = mWidth - 7;				//减去按钮的宽度
			title_txt.height = mHeight;
			title_txt.autoSize = TextFieldAutoSize.CENTER;
			title_txt.setTextFormat( format );
			title_txt.textColor = 0xE2CCA5;
			title_txt.text = sName;
//			title_txt.x = 30;
			title_txt.y = 0;
			this.addChild( title_txt );
//			title_txt.filters = Font.Stroke( 0x000000 );
			addChild( title_txt );
			
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		protected function addStageHandler(evt:Event):void
		{
			content_mc.gotoAndStop( 1 );
			this.addEventListener( MouseEvent.MOUSE_DOWN,clickCellHandler );
			this.addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		protected function clickCellHandler(evt:MouseEvent):void
		{
			if ( clickFun != null ) clickFun( this.contentStr );
		}
		
		protected function removeStageHandler(evt:Event):void
		{
			this.removeEventListener( MouseEvent.MOUSE_DOWN,clickCellHandler );
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		public function gc():void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}

	}
}