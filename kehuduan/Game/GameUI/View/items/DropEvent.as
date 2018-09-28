package GameUI.View.items
{
	import flash.events.Event;

	public class DropEvent extends Event
	{
		public static const DRAG_THREW:String = "dragThrew";
        public static const DRAG_DROPPED:String = "dragDropped";
        public static const SKILLITEMDRAGED:String = "skillItemDarged";
		
		public var Data:Object = null;
		
		public function DropEvent(type:String, data:Object = null)
		{
			super(type);
			Data = data;
		}
		
		public override function clone():Event
		{
			return new DropEvent(type, Data);
		}
	}
}