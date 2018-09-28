package GameUI.Modules.MusicPlayer.View
{
	import GameUI.View.UIKit.components.VScrollBar;

	public class PlaylistVScrollBar extends VScrollBar
	{
		public function PlaylistVScrollBar()
		{
			super();
		}
		override public function loadSkinParts():void
		{
			upButton = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("UpArrow");
			downButton = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("DownArrow");
			track = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByFlashComponent("BarBackground");
			thumb = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByFlashComponent("Thumb");
			grip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByFlashComponent("Grip");
			
			this.addChild(track);
			this.addChild(upButton);
			this.addChild(downButton);
			this.addChild(thumb);
			this.addChild(grip);
			
			grip.mouseEnabled = false;
			
			registerEventListeners();
		}
	}
}