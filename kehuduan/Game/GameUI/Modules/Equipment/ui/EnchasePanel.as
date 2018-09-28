package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class EnchasePanel extends Sprite
	{
		public var view:MovieClip;
		
		public function EnchasePanel()
		{
			super();
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("enchasePanel");
			this.addChild(this.view);
		}
		
	}
}