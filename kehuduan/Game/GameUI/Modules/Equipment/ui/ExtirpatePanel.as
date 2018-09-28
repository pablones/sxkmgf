package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class ExtirpatePanel extends Sprite
	{
		public var view:MovieClip;
		public function ExtirpatePanel()
		{
			super();
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ExtirpatePanel");
			this.addChild(this.view);
		
		}
		
	}
}