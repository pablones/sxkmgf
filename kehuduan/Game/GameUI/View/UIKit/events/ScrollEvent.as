package GameUI.View.UIKit.events 
{
	import flash.events.Event;
	import GameUI.View.UIKit.core.ViewportController;
	
	/**
	 * ...
	 * @author statm
	 */
	public class ScrollEvent extends Event 
	{
		
		public static const SCROLL:String = "scroll";
		
		public function ScrollEvent(type:String, viewportController:ViewportController) 
		{ 
			super(type);
			
			this.viewportController = viewportController;
		} 
		
		public var viewportController:ViewportController;
		
		public override function clone():Event 
		{ 
			return new ScrollEvent(type, bubbles, cancelable);
		} 
		
		public override function toString():String 
		{ 
			return formatToString("ScrollEvent", "type", "viewportController"); 
		}
		
	}
	
}