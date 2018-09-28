package GameUI.Modules.PlayerInfo.Command
{
	import OopsEngine.Role.GameRole;
	
	import flash.events.Event;

	public class TeamEvent extends Event
	{
		public static const CELL_CLICK:String="cellClick";
		
		public var role:GameRole;
		public var flagStr:String;
		
		public function TeamEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
		
		
	}
}