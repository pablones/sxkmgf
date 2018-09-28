package GameUI.View.UIKit.events 
{
	import flash.events.Event;
	
	/**
	 * ...
	 * @author statm
	 */
	public class UIEvent extends Event 
	{
		
		public static const VALUE_COMMIT:String = "valueCommit";
		
		public function UIEvent(type:String) 
		{ 
			super(type);
		} 
		
		public override function clone():Event 
		{ 
			return new UIEvent(type);
		} 
		
		public override function toString():String 
		{ 
			return formatToString("UIEvent", "type"); 
		}
		
	}
	
}