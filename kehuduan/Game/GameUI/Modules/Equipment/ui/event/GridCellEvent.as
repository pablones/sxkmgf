package GameUI.Modules.Equipment.ui.event
{
	import flash.events.Event;

	public class GridCellEvent extends Event
	{
		
		public static const GRIDCELL_CLICK:String="GariCell_click";
		
		public var data:*;
		public function GridCellEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
	}
}