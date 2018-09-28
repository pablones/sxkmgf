package GameUI.View.UIKit.layout 
{
	import flash.display.DisplayObject;
	import flash.events.Event;
	import flash.geom.Point;
	import GameUI.View.UIKit.events.ResizeEvent;
	import GameUI.View.UIKit.core.UIComponent;
	/**
	 * ...
	 * @author statm
	 */
	public class LayoutBase 
	{
		
		public function LayoutBase(target:UIComponent) 
		{
			this.target = target;
		}
		
		protected var target:UIComponent;
		
		protected var layoutInvalidated:Boolean = false;
		
		public function invalidateLayout(validateNow:Boolean = false):void
		{
			if (!target) return;
			
			if (validateNow)
			{
				doLayout();
				return;
			}
			
			if (layoutInvalidated) return;
			
			layoutInvalidated = true;
			target.addEventListener(Event.ENTER_FRAME, _doLayout);
		}
		
		protected function _doLayout(event:Event):void
		{
			doLayout();
			
			//trace(">>> " + target.width + ", " + target.height);
			layoutInvalidated = false;
			target.removeEventListener(Event.ENTER_FRAME, _doLayout);
		}
		
		protected function doLayout():void
		{
		}
		
		protected function dispatchResizeEvent(oldWidth:Number = NaN, oldHeight:Number = NaN):void
		{
			target.dispatchEvent(new ResizeEvent(ResizeEvent.RESIZE, oldWidth, oldHeight));
		}
		
		
		
		// ----------------------------------------
		//
		//		测试代码：销毁
		//
		// ----------------------------------------
		
		public function finalize():void
		{
			this.target = null;
		}
	}

}