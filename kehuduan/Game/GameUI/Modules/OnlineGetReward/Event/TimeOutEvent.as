package GameUI.Modules.OnlineGetReward.Event
{
	import flash.events.Event;

	public class TimeOutEvent extends Event
	{
		public static const TIME_IS_ZERO:String = "TIME_IS_ZERO";
		public function TimeOutEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
	}
}