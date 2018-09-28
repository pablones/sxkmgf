package GameUI.View.Components
{
	import flash.display.Sprite;
	import flash.display.DisplayObject;
	import flash.display.Shape;

	public class UISprite extends Sprite
	{
		protected var iBackground:DisplayObject;
		
		public function UISprite()
		{
			this.name = "UISprite";
			//this.mouseChildren = false;
			this.mouseEnabled = false;
			var shape:Shape = new Shape();
            shape.graphics.lineStyle(0, 0, 0);
            shape.graphics.beginFill(0, 0);
            shape.graphics.drawRect(0, 0, 10, 10);
            shape.graphics.endFill();
            iBackground = shape;
            addChildAt(iBackground,0);
		}
		
		override public function get width() : Number
        {
            return iBackground.width;
        }
		
		override public function set width(v:Number) : void
        {
            iBackground.width = v;
        }
	
		override public function set height(v:Number) : void
        {
            iBackground.height = v;
        }
		
		override public function get height() : Number
        {
            return iBackground.height;
        }
	}
}