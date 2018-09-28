package GameUI.Modules.ToolTip.Mediator.UI
{
	public interface IToolTip
	{
		function Show():void;
		
		function Update(obj:Object):void;
		
		function Remove():void;
		
		function GetType():String;
		
		function BackWidth():Number
		
		/* function set Type(v:String):void;
		
		function get Type():String; */
	}
}