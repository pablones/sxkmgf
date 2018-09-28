package GameUI.Modules.Chat.UI
{
	import flash.events.Event;

	public class TextLinkEvent extends Event
	{
		public static const EVENTNAME:String = "TextLinkEvent";
		
		public var name:String = "";
		public var namePerson:String = "";
		public var namePet:String = "";
		
		public function TextLinkEvent(name:String,namePerson:String = "",namePet:String ="")
		{
			super(EVENTNAME);
			this.name = name;
			this.namePerson = namePerson;
			this.namePet = namePet;
		}
		
	}
}