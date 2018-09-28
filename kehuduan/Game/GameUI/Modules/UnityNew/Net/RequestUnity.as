package GameUI.Modules.UnityNew.Net
{
	import Net.ActionSend.UnityActionSend;
	
	public class RequestUnity
	{
		public static function send( action:int,page:int,targetId:int=0 ):void
		{
			var obj:Object = new Object();
			obj.type = 1107;
			obj.data = [ 0, 0, action, page, targetId ];
			UnityActionSend.SendSynAction( obj );	
		}

	}
}