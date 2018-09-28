package GameUI.Modules.Master.Proxy
{
	import Net.ActionSend.PlayerActionSend;
	import Net.ActionSend.TutorSend;
	import Net.Protocol;
	
	public class RequestTutor
	{
		public static function requestTutorAction(action:int,page:uint=0):void							//78为请求列表  72发师傅  74发徒弟
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(page);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(action);							//请求师徒列表
			parm.push(0);
			if ( action == 72 || action == 225 )
			{
				parm.push(GameCommonData.Player.Role.Name);	
			}
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		//action 13发送拜师协议  发送师傅id
		public static function requestData( action:uint,mentorId:int ):void
		{
			var obj:Object = new Object();
			obj.action = action;
			obj.amount = 14;
			obj.pageIndex = 0;
			obj.mentorId = mentorId;
			TutorSend.sendTutorAction( obj );
		}
	}
}