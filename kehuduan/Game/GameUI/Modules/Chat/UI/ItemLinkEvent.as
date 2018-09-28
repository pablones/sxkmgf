package GameUI.Modules.Chat.UI
{
	import flash.events.Event;

	public class ItemLinkEvent extends Event
	{
		public static const EVENTNAME:String = "ItemLinkEvent";
		
		public var name:String = "";
		
		public function ItemLinkEvent(name:String)
		{
			super(EVENTNAME);
			this.name = name;
		}
		
	}
}