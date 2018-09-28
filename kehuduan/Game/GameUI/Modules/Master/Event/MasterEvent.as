package GameUI.Modules.Master.Event
{
	import flash.events.Event;

	public class MasterEvent extends Event
	{
		public static const CLICK_INFO:String = "CLICK_MASTER_INFO";
		public var rate:uint;
		public var pLevel:uint;
		public function MasterEvent(type:String, rate:uint,pLevel:uint,bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.rate = rate;
			this.pLevel = pLevel;
		}
		
	}
}