package GameUI.Modules.HeroSkill.Command
{
	import Net.ActionSend.PlayerActionSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class LearnSkillCommand extends SimpleCommand
	{
		public function LearnSkillCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
//			trace("学一个技能  " +notification.getBody().SkillLevel," skillid: +"+notification.getBody().SkillId);
			var times:int = notification.getBody().times;
			var data:Array=[0,GameCommonData.Player.Role.Id,0,0,notification.getBody().SkillLevel,notification.getBody().SkillId,230,times,0];
			var obj:Object={type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
		}
			
	}
}