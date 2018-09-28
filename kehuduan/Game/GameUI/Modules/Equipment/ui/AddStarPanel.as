package GameUI.Modules.Equipment.ui
{
	import GameUI.View.Components.UISprite;
	
	import flash.display.MovieClip;

	public class AddStarPanel extends UISprite
	{
		public var view:MovieClip;
		
		public function AddStarPanel()
		{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AddStarPanel");
			this.addChild(this.view);
		}
		
	}
}