package GameUI.View.UIKit.layout 
{
	import flash.geom.Point;
	import GameUI.View.UIKit.core.UIComponent;
	
	/**
	 * ...
	 * @author statm
	 */
	public class BasicLayout extends LayoutBase 
	{
		
		public function BasicLayout(target:UIComponent) 
		{
			super(target);
		}
		
		override protected function doLayout():void 
		{
			if (!target) return;
			if (!isNaN(target.explicitWidth) && !isNaN(target.explicitHeight)) return;
			
			var oldWidth:Number = target.width;
			var oldHeight:Number = target.height;
			
			var br:Point = new Point();
			var n:int = target.numChildren;
			
			for (var i:int = 0; i < n; i ++)
			{
				var elemBR:Point = target.getChildAt(i).getBounds(target).bottomRight;
				br.x = Math.max(elemBR.x, br.x);
				br.y = Math.max(elemBR.y, br.y);
			}
			
			target.setActualSize(isNaN(target.explicitWidth) ? br.x : target.explicitWidth,
								isNaN(target.explicitHeight) ? br.y : target.explicitHeight);
			
			//trace(">>> target size=" + target.width + ", " + target.height);
			
			if (n > 0 && 
				(target.width != oldWidth || target.height != oldHeight))
			{
				dispatchResizeEvent(oldWidth, oldHeight);
			}
		}
	}

}