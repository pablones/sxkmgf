package GameUI.Modules.Login.SetFrame
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
		public static function UseFrame(grid:DisplayObject , Frame:String = "YellowFrame" , x:int = 0 , y:int = 0 , width:int = 53 ,height:int = 53 ):void
		{
			var yellowFrame:MovieClip;
			if(GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary) == null)
				yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip(Frame);
			else 
				yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(Frame);
			yellowFrame.name = Frame;
			grid.parent.addChild(yellowFrame);
			grid.parent.setChildIndex(yellowFrame, grid.parent.numChildren-1);
			//grid.parent.parent.setChildIndex(grid.parent, grid.parent.parent.numChildren-1);
			yellowFrame.x = grid.x+x;
			yellowFrame.y = grid.y+y;	
			yellowFrame.width = width;
			yellowFrame.height = height;
		}		
		
		public static function RemoveFrame(parent:DisplayObjectContainer, name:String = "YellowFrame"):void
		{
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