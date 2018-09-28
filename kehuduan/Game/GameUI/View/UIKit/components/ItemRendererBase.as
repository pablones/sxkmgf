package GameUI.View.UIKit.components
{
	import flash.display.Graphics;
	import flash.display.Sprite;
	import flash.events.Event;
	import GameUI.View.UIKit.core.UIComponent;
	
	public class ItemRendererBase extends Sprite
	{
		public function ItemRendererBase():void
		{
			super();
		}
		
		protected var _data:Object;
		
		public function get data():Object
		{
			return _data;
		}
		
		public function set data(value:Object):void
		{
			if (value == _data) return;
			
			_data = value;
			
			requestRedraw();
		}
		
		protected var _index:int = 0;
		
		public function get index():int
		{
			return _index;
		}
		
		public function set index(value:int):void
		{
			if (value == _index) return;
			
			_index = value;
			
			requestRedraw();
		}
		
		protected function requestRedraw():void
		{
			this.addEventListener(Event.ENTER_FRAME, _redraw); 
		}
		
		protected function _redraw(event:Event):void
		{
			this.removeEventListener(Event.ENTER_FRAME, _redraw);
			redraw();
		}
		
		protected function redraw():void
		{
		}
	}
}