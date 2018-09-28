package GameUI.View.UIKit.layout 
{
	import flash.display.DisplayObject;
	import GameUI.View.UIKit.core.UIComponent;
	
	/**
	 * ...
	 * @author statm
	 */
	public class VerticalLayout extends LayoutBase 
	{
		
		public function VerticalLayout(target:UIComponent) 
		{
			super(target);
		}
		
		protected var _gap:Number = 0;
		
		public function get gap():Number
		{
			return _gap;
		}
		
		public function set gap(value:Number):void
		{
			if (value == _gap) return;
			
			_gap = value;
			
			invalidateLayout();
		}
		
		override protected function doLayout():void 
		{
			if (!target) return;
			
			var oldWidth:Number = target.width;
			var oldHeight:Number = target.height;
			
			var n:int = target.numChildren;
			
			var currWidth:Number = 0;
			var currYPos:Number = 0;
			
			for (var i:int = 0; i < n; i ++)
			{
				var elem:DisplayObject = target.getChildAt(i);
				elem.x = 0;
				elem.y = currYPos;
				
				currWidth = Math.max(elem.width, currWidth);
				currYPos += elem.height + _gap;
			}
			
			if (n > 0) currYPos -= _gap;
			
			target.setActualSize(isNaN(target.explicitWidth) ? currWidth : target.explicitWidth,
								isNaN(target.explicitHeight) ? currYPos : target.explicitHeight);
			
			if (target.width != oldWidth || target.height != oldHeight)
			{
				dispatchResizeEvent(oldWidth, oldHeight);
			}
		}
	}

}