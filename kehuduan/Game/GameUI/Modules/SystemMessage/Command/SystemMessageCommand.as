package GameUI.Modules.SystemMessage.Command
{
	import GameUI.ConstData.CommandList;
	import GameUI.Modules.Friend.command.FriendActionList;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SystemMessageCommand extends SimpleCommand
	{
		public static const NAME:String = "systemMessageCommand";
		public function SystemMessageCommand()
		{
			super();
		}
		/** 发送系统消息 */
		public override function execute(notification:INotification):void
		{
			var message:Object = notification.getBody() as Object;
			var obj:Object={};
			obj.nSty = message.title;
			var arr:Array=[];
			arr.push("system");
			arr.push("fsddfsd");  //接收的玩家
			arr.push(UnityUtils.timeToNum());
			arr.push(message.content);
			arr.push("unknow");
			arr.push("unknow");
			obj.talkObj=arr;
			obj.nItemTypeID=1;
			obj.nItem=1;
			obj.nGm=0;
			obj.nColor=0;
			obj.nAtt=FriendActionList.SYSTEM_MSG;//2040
			facade.sendNotification(CommandList.FRIEND_CHAT_MESSAGE ,obj);
		}
	}
}