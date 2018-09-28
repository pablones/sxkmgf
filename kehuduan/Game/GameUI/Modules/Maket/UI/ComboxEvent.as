package GameUI.Modules.Maket.UI
{
	import flash.events.Event;

	public class ComboxEvent extends Event
	{
		public static const COMBOXEVENT_CLICKITEM:String = "comboxEvent_clickItem";
		public var data:Object;
		
		public function ComboxEvent(type:String, data:Object=null, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.data = data;
		}
		
	}
}