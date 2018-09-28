package GameUI.View.UIKit.events 
{
	import flash.events.Event;
	
	/**
	 * ...
	 * @author statm
	 */
	public class ResizeEvent extends Event 
	{
		
		public static const RESIZE:String = "resize";
		
		public function ResizeEvent(type:String, oldWidth:Number = NaN, oldHeight:Number = NaN):void
		{
			super(type);
			
			this.oldWidth = oldWidth;
			this.oldHeight = oldHeight;
		}
		
		
		public var oldWidth:Number;
		
		public var oldHeight:Number;
		
		public override function clone():Event 
		{ 
			return new ResizeEvent(type, oldWidth, oldHeight);
		} 
		
		public override function toString():String 
		{ 
			return formatToString("ResizeEvent", "type", "oldWidth", "oldHeight"); 
		}
	}

}