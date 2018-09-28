package GameUI.Modules.Unity.Command
{
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class UnityStartUpCommand extends SimpleCommand
	{
		public static const NAME:String = "UnityStartUpCommand";
		public function UnityStartUpCommand()
		{
			super();
		}
		public override function execute(notification:INotification):void
		{
			facade.registerCommand(SendActionCommand.SENDACTION , SendActionCommand);		/** 注册发送请求 */
//			facade.registerCommand(KeepOnCommand.KEEPON , KeepOnCommand);					/** 注册截取聊天中的数据操作 */ 
		}
	}
}