package GameUI.Modules.OnlineGetReward.Net
{
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	public class GainAwardAction
	{
		public function GainAwardAction()
		{
		}
		
		public static function send():void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);			
			parm.push(287);							   
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}

	}
}