package GameUI.Modules.Task.Commamd
{
	import flash.events.Event;

	public class TreeEvent extends Event
	{
		
		public static const CHANGE_SELECTED:String="changeSelected";
	
	
		public var id:uint;
		public function TreeEvent(type:String, id:uint,bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.id=id;
		}
		
	}
}