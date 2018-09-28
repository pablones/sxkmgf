package Net
{
	import flash.utils.ByteArray;
	
	public interface IGameNet
	{
		function endGameNet():void;
		function Send(byte:ByteArray):void;
		function get Gam():GameActionManager;
	}
}