package GameUI.View.UIKit.core 
{
	import flash.display.DisplayObject;
	import flash.events.Event;
	import flash.geom.Rectangle;
	import GameUI.View.UIKit.events.ResizeEvent;
	/**
	 * ...
	 * @author statm
	 */
	public class ViewportController 
	{
		
		public function ViewportController(host:UIComponent):void
		{
			this.target = host;
		}
		
		protected var target:UIComponent;
		
		protected var scrollRect:Rectangle = new Rectangle();
		
		
		// ----------------------------------------
		//
		//		视口开关
		//
		// ----------------------------------------
		
		protected var activated:Boolean = false;
		
		public function activate():void
		{
			if (activated) return;
			
			activated = true;
			invalidateScrollRect();
		}
		
		public function deactivate():void
		{
			if (!activated) return;
			
			activated = false;
			invalidateScrollRect();
		}
		
		
		
		// ----------------------------------------
		//
		//		视口尺寸
		//
		// ----------------------------------------
		
		protected var _width:Number = 0;
		
		public function get width():Number
		{
			return scrollRect.width;
		}
		
		public function set width(value:Number):void
		{
			if (value == _width) return;
			
			_width = value;
			
			scrollRect.width = _width;
			
			invalidateScrollRect();
		}
		
		protected var _height:Number = 0;
		
		public function get height():Number
		{
			return scrollRect.height;
		}
		
		public function set height(value:Number):void
		{
			if (value == _height) return;
			
			_height = value;
			
			scrollRect.height = _height;
			
			invalidateScrollRect();
		}
		
		
		
		// ----------------------------------------
		//
		//		视口位置
		//
		// ----------------------------------------
		
		public function get scrollPosX():Number
		{
			return scrollRect.x;
		}
		
		public function set scrollPosX(value:Number):void
		{
			if (value == scrollRect.x) return;
			
			var oldX:Number = scrollRect.x;
			
			scrollRect.x = Math.min(target.width, value);
			scrollRect.x = Math.max(0, scrollRect.x);
			
			if (scrollRect.x != oldX)
				invalidateScrollRect();
		}
		
		public function get scrollPosY():Number
		{
			return scrollRect.y;
		}
		
		public function set scrollPosY(value:Number):void
		{
			if (value == scrollRect.y) return;
			
			var oldY:Number = scrollRect.y;
			
			scrollRect.y = Math.min(target.height - scrollRect.height, value);
			scrollRect.y = Math.max(0, scrollRect.y);
			
			if (scrollRect.y != oldY)
				invalidateScrollRect();
		}
		
		
		
		// ----------------------------------------
		//
		//		视口更新
		//
		// ----------------------------------------
		
		protected var scrollRectChanged:Boolean = false;
		
		protected function invalidateScrollRect():void
		{
			if (scrollRectChanged) return;
			
			scrollRectChanged = true;
			target.addEventListener(Event.ENTER_FRAME, _updateScrollRect);
		}
		
		protected function _updateScrollRect(event:Event):void
		{
			if (activated)
			{
				target.scrollRect = scrollRect;
			}
			else
			{
				target.scrollRect = null;
			}
//			trace(">>> actual update");
			scrollRectChanged = false;
			target.removeEventListener(Event.ENTER_FRAME, _updateScrollRect);
		}
		
		
		
		// ----------------------------------------
		//
		//		测试代码：销毁
		//
		// ----------------------------------------
		
		public function finalize():void
		{
			if (!target) return;
			target.scrollRect = null;
			target = null;
		}
	}

}