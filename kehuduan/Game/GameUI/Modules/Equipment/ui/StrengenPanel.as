package GameUI.Modules.Equipment.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;


	/**
	 *  
	 * @author felix 装备强化面板
	 * 
	 */	
	public class StrengenPanel extends Sprite
	{
		public var view:MovieClip;
		public function StrengenPanel()
		{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("StrengenPanel");
			this.addChild(this.view);
		}		
	}
}