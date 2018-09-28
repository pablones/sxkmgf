package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class HunYunPanel extends Sprite
	{
		public var view:MovieClip;
		
		public function HunYunPanel()
		{
			super();
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("HunYunPanel");
			this.addChild(this.view);
		}
		
	}
}