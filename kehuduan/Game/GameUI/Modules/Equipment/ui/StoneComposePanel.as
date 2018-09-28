package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class StoneComposePanel extends Sprite
	{
		public var view:MovieClip;
		
		public function StoneComposePanel()
		{
			super();
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("StoneComposePanel");
			this.addChild(this.view);
		}
		
	}
}