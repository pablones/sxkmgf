package GameUI.View.Components
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;

	public class YellowReadingBar extends Sprite
	{	
		private var curNum:int;
		private var maxNum:int;
		private var barWidth:int;
		private var main_mc:MovieClip;
		
		public function YellowReadingBar( _curNum:int,_maxNum:int,_barWidth:int )
		{
			curNum = _curNum;
			maxNum = _maxNum;
			barWidth = _barWidth;
			if ( curNum > maxNum )
			{
				curNum = maxNum;
			}
		}
		
		public function init():void
		{
			if ( !main_mc )
			{
				main_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip( "GoingRect" );
			}
			main_mc.width = barWidth;
			main_mc.mcPower.width = curNum/maxNum * barWidth;
			addChild( main_mc );
			
			var format:TextFormat = new TextFormat();
			format.size = 12;
			format.font = "宋体";
			format.color = 0xff0000;
			format.align = TextFormatAlign.CENTER;
			
			var txt:TextField = new TextField();
//			txt.backgroundColor = 0x666666;
//			txt.background = true;
			txt.text = curNum.toString() + "/" + maxNum.toString();
			txt.width = barWidth;
			txt.height = 16;
			txt.multiline = false;
//			txt.alpha = .3
			txt.y = -1;
			txt.setTextFormat( format );
			main_mc.addChild( txt );
		}
		
		public function gc():void
		{
			var des:Object;
			while ( this.numChildren>0 )
			{
				des = this.removeChildAt( 0 );
				des = null;
			}
		}
	}
}