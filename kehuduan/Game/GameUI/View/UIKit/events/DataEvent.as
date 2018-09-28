package GameUI.View.UIKit.events
{
	import flash.events.Event;
	
	public class DataEvent extends Event
	{
		public static const ADD: String = "add";
		
		public static const REMOVE: String = "remove";
		
		public static const UPDATE: String = "update";
		
		public static const CLEAR: String = "clear";
		
		public static const REFRESH: String = "refresh";
		
		
		public var index: int;
		
		public function DataEvent(type:String, index:int = -1)
		{
			super(type);
			
			this.index = index;
		}
		
		public override function clone():Event 
		{ 
			return new DataEvent(type, index);
		} 
		
		public override function toString():String 
		{ 
			return formatToString("DataEvent", "type", "index"); 
		}
	}
}