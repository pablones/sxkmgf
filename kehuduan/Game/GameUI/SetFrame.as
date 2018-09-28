package GameUI
{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	
	public class SetFrame
	{
		public function SetFrame()
		{
		}
		
		//添加外框
		public static function UseFrame(grid:DisplayObject, name:String = "YellowFrame"):void
		{
			var yellowFrame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(name);
			yellowFrame.name = name;
			yellowFrame.mouseEnabled = false;
			yellowFrame.mouseChildren = false;
			grid.parent.addChild(yellowFrame);
			grid.parent.setChildIndex(yellowFrame, grid.parent.numChildren-1);
			//grid.parent.parent.setChildIndex(grid.parent, grid.parent.parent.numChildren-1);
			yellowFrame.x = grid.x;
			yellowFrame.y = grid.y;	
		}		
		
		//移除黄框
		public static function RemoveFrame(parent:DisplayObjectContainer, name:String = "YellowFrame"):void
		{
			if(parent==null)return;
			var count:int = parent.numChildren-1;
			while(count)
			{
				if(parent.getChildByName(name)) 
    			{
    				parent.removeChild(parent.getChildByName(name));
    			}
    			count--;
   			}
		}
	}
}