package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	/**
	 * 宝石雕琢界面
	 * @author lh
	 * 
	 */
	public class StoneDecoratePanel extends Sprite
	{
		public var view:MovieClip;
		public function StoneDecoratePanel()
		{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("StoneDecoratePanel");
			this.addChild(this.view);
		}
		
	}
}