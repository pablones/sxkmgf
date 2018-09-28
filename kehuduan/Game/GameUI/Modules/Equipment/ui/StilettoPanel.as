package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class StilettoPanel extends Sprite
	{
		public var view:MovieClip;
		
		public function StilettoPanel()
		{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("stilettoPanel");
			this.addChild(this.view);
		}
		
	}
}