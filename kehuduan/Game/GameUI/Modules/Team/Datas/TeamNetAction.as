package GameUI.Modules.Team.Datas
{
	import Net.ActionSend.TeamSend;
	import Net.Protocol;
	
	public class TeamNetAction
	{
		public function TeamNetAction()
		{
		}
		
		/** 发送组队命令 */
		public static function sendTeamOrder(action:int, idPlayer:int=0, idTarget:int=0, playerName:String="", targetName:String=""):void
		{
			var o:Object = new Object();
			var data:Array = new Array();
			
			o.type = Protocol.TEAM_ACTION;
			
			data.push(action);
			data.push(0);
			data.push(0);
			data.push(0);
			
			data.push(idPlayer);
			data.push(0);
			data.push(idTarget);
			
			data.push(playerName);
			data.push(targetName);
			
			o.data = data;
			
			TeamSend.createMsgTeam(o);
		}
		
	}
}