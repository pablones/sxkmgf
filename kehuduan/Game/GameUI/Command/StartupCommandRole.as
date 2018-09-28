package GameUI.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Login.StartMediator.CreateRoleMediatorYl;
	import GameUI.Modules.Login.StartMediator.LoginMediatorYL;
	
	import Net.GameNet;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class StartupCommandRole extends SimpleCommand
	{
		public function StartupCommandRole()
		{
			super();
		}
		public override function execute(notification:INotification):void
		{
			/** 注册警告 */
			facade.registerMediator(new CreateRoleMediatorYl());
			facade.sendNotification(EventList.SHOWCREATEROLE);
//			GameCommonData.flagTestYL = true;
			/**  登陆 选择角色 创建角色  (这个需要最后注册，其他的都加在这个上面)*/
//			if(GameCommonData.isLoginFromLoader) {
//				GameCommonData.GameNets = new GameNet(GameCommonData.ServerIp, 4808)
//			} else {
//				facade.registerMediator(new LoginMediatorYL());
//				facade.sendNotification(EventList.SHOWLOGIN);
//			}
		}
		
	}
}