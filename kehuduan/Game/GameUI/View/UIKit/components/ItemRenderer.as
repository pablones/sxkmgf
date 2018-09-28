package GameUI.View.UIKit.components 
{
	import flash.events.MouseEvent;
	import GameUI.View.UIKit.components.ItemRendererBase;
	
	/**
	 * ...
	 * @author statm
	 */
	public class ItemRenderer extends ItemRendererBase 
	{
		public function ItemRenderer() 
		{
			super();
			
			addEventListener(MouseEvent.ROLL_OVER, mouseHandler);
			addEventListener(MouseEvent.ROLL_OUT, mouseHandler);
		}
		
		protected var _selected:Boolean = false;
		
		public function get selected():Boolean 
		{
			return _selected;
		}
		
		public function set selected(value:Boolean):void 
		{
			if (value == _selected) return;
			
			_selected = value;
			
			requestRedraw();
		}
		
		protected var _over:Boolean = false;
		
		protected function mouseHandler(e:MouseEvent):void 
		{
			if (e.type == MouseEvent.ROLL_OVER)
			{
				_over = true;
			}
			else if (e.type == MouseEvent.ROLL_OUT)
			{
				_over = false;
			}
			
			requestRedraw();
		}
	}

}