package GameUI.Modules.Friend.command
{
	import GameUI.MouseCursor.SysCursor;
	import GameUI.Proxy.DataProxy;
	
	import OopsEngine.Role.GameRole;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SelectElementCommand extends SimpleCommand
	{
		public function SelectElementCommand()
		{
			super();
		}
		
		override public function execute(notification:INotification):void{
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			if(GameCommonData.TargetAnimal==null)return;
			var role:GameRole=GameCommonData.TargetAnimal.Role;
			
			if(role.Type==GameCommonData.wordDic[ "mod_fri_com_sel" ] && SysCursor.GetInstance().isLock){//"玩家"
				SysCursor.GetInstance().isLock=false;
				sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:role.Id,name:role.Name});
			}else{
				SysCursor.GetInstance().isLock=false;
			}
		}
		
	}
}