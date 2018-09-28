package GameUI.View.BaseUI
{
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class ListComponent extends Sprite
	{
		private var isUseBack:Boolean = true;
		private var backMc:MovieClip = null;
		private var itemWidth:Number = 0;
		private var itemHeigth:Number = 0;
		private var itemCount:int = 0;
		
		public var Offset:int = 2; 
		
		public static const OFFSET_WIDTH:int =  6;
		public static const OFFSET_HEIGHT:int =  6;
		
		public function ListComponent(isUseBack:Boolean = true)
		{
			this.isUseBack = isUseBack;
			if(this.isUseBack)
			{
				backMc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ListBack");
				this.addChild(backMc);
			}
		}
		
		public function SetChild(child:DisplayObject):void
		{
			this.addChild(child);
			itemCount++;
			itemWidth = child.width;
			itemHeigth = child.height;
		}
		
		public function upDataPos():void
		{
			var ypos:int = 0;
			var i:int = 0;
			if(this.isUseBack)
			{
				i = 1;
			}
			for(; i < numChildren; i++)
			{
				var child:DisplayObject = getChildAt(i);
				child.y = ypos+Offset;
				child.x = Offset;
				ypos +=  child.height;
			}
			if(isUseBack)
			{
				backMc.x = 0;
				backMc.y = 0;
				backMc.height = itemCount * itemHeigth + OFFSET_HEIGHT;
				backMc.width = itemWidth + OFFSET_WIDTH;
			}
		}
	}
}