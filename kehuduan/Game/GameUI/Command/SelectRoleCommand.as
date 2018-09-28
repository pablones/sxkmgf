package GameUI.Command
{
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.GameConnect;
	import Net.Protocol;
	
	import OopsFramework.Debug.Logger;
	
	import Vo.RoleVo;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SelectRoleCommand extends SimpleCommand
	{
		public function SelectRoleCommand()
		{
			super();
			this.initializeNotifier(UIFacade.FACADEKEY);
		}
		
		public override function execute(notification:INotification):void 
		{
			var index:int = notification.getBody() as int;
			if(GameCommonData.wordVersion == 2)
			{
				index = 0;
			}
		
			var role:RoleVo = GameCommonData.RoleList[index] as RoleVo;
			var obj:Object  = new Object();
			obj.type = Protocol.LOGIN_GSERVER;
			obj.data = new Array(); 
			obj.data.push(GameCommonData.GServerInfo.idAccount);
			obj.data.push(0);
			obj.data.push( GameCommonData.RoleList[index].SzName );
			GameCommonData.SelectedRole = role;
	
			GameConnect.GameServerConnect(obj);
			
			Logger.Info(this,"execute","SelectRoleCommand执行：GameConnect.GameServerConnect(obj)");
		}

		/**  测试重连   */
		private function reConnect(obj:Object):void
		{
			GameConnect.GameServerConnect(obj);
		}
	}
}