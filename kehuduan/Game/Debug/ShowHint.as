package Debug
{
	import flash.display.DisplayObjectContainer;
	import flash.text.TextField;
	
	public class ShowHint
	{
		
		public static var hint_txt:TextField = new TextField();
		
		public static function display( parent:DisplayObjectContainer,color:uint=0xff0000 ):void
		{
			if ( !parent.contains( hint_txt ) )
			{
				hint_txt.textColor = color;
//				hint_txt.autoSize = "left";
//				hint_txt.multiline = true;
				hint_txt.x = 0;
				hint_txt.border = true;
//				hint_txt.borderColor = 0xffffff;
				hint_txt.background = true;
				hint_txt.backgroundColor =0xffffff;
				hint_txt.width = 500;
				hint_txt.height = 400;
				hint_txt.wordWrap = true;
				hint_txt.mouseWheelEnabled = true;
				parent.addChild( hint_txt );
			}
		}
		
		public static function show( info:String ):void
		{
			hint_txt.appendText( info + "\n" );
		}

	}
}