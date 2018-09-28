package GameUI.View.Components.countDown
{
	import flash.events.Event;

	public class CountDownEvent extends Event
	{
		/** 倒计时结束 */
		public static const TIME_OVER:String = "timer_over";
		
		public var data:Object;
		
		public function CountDownEvent(type:String, data:Object, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.data = data;
		}
		
	}
}