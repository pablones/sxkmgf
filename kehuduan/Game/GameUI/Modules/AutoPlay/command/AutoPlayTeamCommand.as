package GameUI.Modules.AutoPlay.command
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.Team.Mediator.TeamMediator;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//挂机组队设置
	public class AutoPlayTeamCommand extends SimpleCommand
	{
		public static const NAME:String = "AutoPlayTeamCommand";
		
		public function AutoPlayTeamCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			if ( GameCommonData.Player.IsAutomatism )
			{
				start();
			}
			else
			{
				stop();
			}
		}
		
		private function start():void
		{
			autoJoin();
			autoKickOut();
		}
		
		private function stop():void
		{
			UIConstData.AUTO_ACCEPT_TEAM_INTITE_AND_APPLY = false;
			UIConstData.AUTO_KICKOUT_LEAVE_MEMBER = false;
		}
		
		//自动接受组队邀请
		private function autoJoin():void
		{
			var tick:uint = AutoPlayData.aSaveTick[10];
			if ( tick==1 )
			{
				UIConstData.AUTO_ACCEPT_TEAM_INTITE_AND_APPLY = true;
			}
		}
		
		//自动踢出离线队员
		private function autoKickOut():void
		{
			var tick:uint = AutoPlayData.aSaveTick[11];
			if ( tick==1 )
			{
				UIConstData.AUTO_KICKOUT_LEAVE_MEMBER = true;
				var teamMed:TeamMediator = facade.retrieveMediator(TeamMediator.NAME) as TeamMediator;
				teamMed.autoKickOutLevMem();	//将当前已离线的队员踢出
			}
		}
		
	}
}