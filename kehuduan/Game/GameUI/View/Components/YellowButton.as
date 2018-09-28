package GameUI.View.Components
{
	import GameUI.Modules.Master.Data.MasterData;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	//黄字按钮
	public class YellowButton extends Sprite
	{
		public var main_mc:MovieClip;
		
		//初始化按钮
		public function init():void
		{
			main_mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "YellowButtonRes" );
			addChild( main_mc );
			
			( main_mc.txt as TextField ).mouseEnabled = false;
		}
		
		public function gc():void
		{
			if ( main_mc && this.contains( main_mc ) )
			{
				removeChild( main_mc );
				main_mc = null;
			}
		}
		
		//设置字
		public function set text( value:String ):void
		{
			( main_mc.txt as TextField ).htmlText = value;
		}
		
		public function set textColor( value:uint ):void
		{
			( main_mc.txt as TextField ).textColor = value;
		}
		
		//按钮是否变灰
		public function set isSee( value:Boolean ):void
		{
			if ( value )
			{
				( main_mc.btn as SimpleButton ).mouseEnabled = true;
				this.filters = null;
				this.mouseEnabled = true;
				this.mouseChildren = true;
//				( main_mc.btn as SimpleButton ).filters = null;
//				MasterData.addGlowFilter( main_mc.txt );
			}
			else
			{
				( main_mc.btn as SimpleButton ).mouseEnabled = false;
				this.mouseEnabled = false;
				this.mouseChildren = false;
//				main_mc.txt.filters = null;
				MasterData.setGrayFilter( this );
//				MasterData.setGrayFilter( main_mc.txt );
			}
		}
	
	}
}