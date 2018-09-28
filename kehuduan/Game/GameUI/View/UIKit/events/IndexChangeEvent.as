package GameUI.View.UIKit.events 
{
	import flash.events.Event;
	
	/**
	 * ...
	 * @author statm
	 */
	public class IndexChangeEvent extends Event 
	{
		public static const CHANGED:String = "changed";
		
		public function IndexChangeEvent(type:String, oldIndex:int, newIndex:int) 
		{ 
			super(type);
			
			this.oldIndex = oldIndex;
			this.newIndex = newIndex;
		} 
		
		public var oldIndex:int;
		
		public var newIndex:int;
		
		public override function clone():Event 
		{ 
			return new IndexChangeEvent(type, oldIndex, newIndex);
		} 
		
		public override function toString():String 
		{ 
			return formatToString("IndexChangeEvent", "type", "oldIndex", "newIndex"); 
		}
		
	}
	
}