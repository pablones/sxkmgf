package GameUI.Modules.Equipment.ui.event
{
	import flash.events.Event;

	public class ListCellEvent extends Event
	{
		public static var LISTCELL_CLICK:String="listCellClick";
		public var data:*;
		
		public function ListCellEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
	}
}