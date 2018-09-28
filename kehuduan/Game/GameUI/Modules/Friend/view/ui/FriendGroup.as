package GameUI.Modules.Friend.view.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class FriendGroup extends Sprite
	{
		
		public var content:MovieClip;
		
		public function FriendGroup()
		{ 
			super();
			this.content=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("FrendGroup");
			content.txt_name.mouseEnabled=false;
			content.height=17;
			this.addChild(content);
		}
		
		public function setName(value:String):void{
			this.content.txt_name.text=value;
		}
		
		
	}
}