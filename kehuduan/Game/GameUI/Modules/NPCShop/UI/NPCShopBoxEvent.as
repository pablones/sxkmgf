package GameUI.Modules.NPCShop.UI
{
	import flash.events.Event;

	public class NPCShopBoxEvent extends Event
	{
		public static const COMBOXEVENT_CLICKITEM:String = "comboxEvent_clickItem";
		
		public function NPCShopBoxEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
	}
}