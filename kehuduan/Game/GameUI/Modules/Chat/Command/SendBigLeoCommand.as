package GameUI.Modules.Chat.Command
{
	import GameUI.UIUtils;
	
	import Net.ActionSend.Chat;
	import Net.Protocol;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SendBigLeoCommand extends SimpleCommand
	{
		public static const NAME:String = "SendBigLeoCommand";
		
		public function SendBigLeoCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var info:Object = notification.getBody();
			var msg:String = info.talkMsg;
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_CHAT;
			obj.data = getParams(info.name, info.color, msg, info.type);
			Chat.SendChat(obj);
		}
		
		private function getParams(target:String, color:int, text:String, type:int, item:String = " "):Array
		{
			var cleanText:String = UIUtils.filterChat(text);					//过滤聊天内容
			var data:Array = new Array();
			data.push(GameCommonData.Player.Role.Name);
			data.push(target);
			data.push("");
			data.push(cleanText); 			// cleanText
			data.push(item);				// 物品
			data.push(color);				// 颜色
			data.push(type);
			data.push(0);
			data.push(0);
			data.push(0);
			data.push(0);
			data.push(0);			
			return data;
		}
		
	}
}