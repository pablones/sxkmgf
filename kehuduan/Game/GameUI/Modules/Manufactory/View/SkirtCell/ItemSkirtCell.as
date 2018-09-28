package GameUI.Modules.Manufactory.View.SkirtCell
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
	public class ItemSkirtCell extends Sprite
	{
		protected var content_mc:MovieClip;
		protected var contentStr:String;
		
		public function ItemSkirtCell(sName:String)
		{
			contentStr = sName;
			var format:TextFormat = new TextFormat();
			format.size = 11;
			format.color = 0xffcc00;
			content_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MenuItemRenderer");
			content_mc.width = 74;
//			content_mc.width = 70;
			content_mc.height = 18;
			content_mc.txt_name.htmlText = sName;
			( content_mc.txt_name as TextField ).setTextFormat( format );
			content_mc.txt_name.mouseEnabled = false;
			content_mc.txt_name.x = 0;
			content_mc.txt_name.x = 5;
			content_mc.txt_name.y = 1;
			content_mc.txt_name.width = 120;
			this.addChild( content_mc );
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
			
		}
		
		protected function removeStageHandler(evt:Event):void
		{
			this.removeEventListener( MouseEvent.CLICK,clickCellHandler );
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}

	}
}