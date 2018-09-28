package GameUI.Modules.CompensateStorage.view
{
	import flash.events.Event;
	
	public interface CSViewManager
	{
		function init():void;
		function show(object:Object = null):void;
		function close(event:Event = null):void;
	}
}