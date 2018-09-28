package GameUI.Modules.Chat.Command
{
	import GameUI.ConstData.CommandList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIUtils;
	
	import Net.ActionSend.Chat;
	import Net.Protocol;
	
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SendCommand extends SimpleCommand
	{
		private var worldDelayID:uint = 0;
		private var otherDelayID:uint = 0; 
			
		public function SendCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			//如果人物等级小于10级，则不能使用世界聊天
			if(ChatData.curSelectModel == 2) 
			{
				if(GameCommonData.Player.Role.Level < 8) 
				{
					facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_chat_com_sendC_exe_1" ], name:"", nAtt:9999});//"只有等级达到8级或以上才能使用世界频道聊天！"
					ChatData.tempItemStr = " ";
					return; 
				}
			}
			
			var info:Object = notification.getBody();
			var msg:String = info.talkMsg;
			if(msg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "") {
				return;
			}
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_CHAT;
			
			obj.data = getParams(info.name, info.color, msg, info.type, info.item, info.jobId);
//			if(info.item != undefined)
//			{
//				obj.data = getParams(info.name, info.color, msg, info.type, info.item, info.jobId);
//			}
//			else
//			{
//				obj.data = getParams(info.name, info.color, msg, info.type, " ", info.jobId);
//			}
			/**
			 * 世界消息：30秒，其他消息：3秒
			 * 如果发言中带!，则为GM命令，不予处理
			 * 正式上线的时候 屏蔽 （msg.charAt(0) != "!"） 此句代码
			 *  */
			if(info.type == 2016 && !ChatData.worldBool && msg.charAt(0) != "!")
			{
				facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_chat_com_sendC_exe_2" ], name:"", nAtt:9999});//"你说话太快了"
				ChatData.tempItemStr = " ";
				return;
			}
			if(!ChatData.otherBool && msg.charAt(0) != "!" && info.type != 2016)
			{
				facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_chat_com_sendC_exe_2" ], name:"", nAtt:9999});//"你说话太快了"
				ChatData.tempItemStr = " ";
				return;
			} 
			//
			if(info.type == 2001)
			{
				testIsDFilter(info.name);
			}
			if(UIUtils.checkCheatStr(msg)) {
				Chat.SendChat(obj);
			} else {
				sendLocalMsg(obj);
			}
			ChatData.tempItemStr = " ";
			if(info.type == 2016 && ChatData.worldBool && msg.charAt(0) != "!")
			{
				ChatData.worldBool = false;
				worldDelayID = setInterval(setWorldChatDelay, 30000);
				return;
			}
			ChatData.otherBool = false;
			otherDelayID = setInterval(setOtherChatDelay, 2000);
		}
			
		private function setWorldChatDelay():void
		{
			clearInterval(worldDelayID);
			ChatData.worldBool = true;
		}
		
		private function setOtherChatDelay():void
		{
			clearInterval(otherDelayID);
			ChatData.otherBool = true;
		}	
		
		private function testIsDFilter(name:String):void
		{
			for(var i:int = 0; i<ChatData.FilterList.length; i++)
			{
				if(name == ChatData.FilterList[i])
				{
					facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "mod_chat_com_sendC_tes_1" ]+"<font color='#ff0000'>["+name+"]</font>"+GameCommonData.wordDic[ "mod_chat_com_sendC_tes_2" ]+"，<font color='#ff0000'>["+name+"]</font>"+GameCommonData.wordDic[ "mod_chat_com_sendC_tes_3" ], color:0xffff00});
					//"你在"  "的黑名单中"	"收不到你的消息"
					return;
				}
			}
		}
				
		private function getParams(target:String, color:int, text:String, type:int, item:String = " ", jobId:uint = 0):Array
		{
			var res:String = "";
			if(GameCommonData.Player.Role.Name.indexOf("[PM]") >= 0) {	//GM 不过滤
				res = text;
			} else {	//过滤聊天内容 
				res = UIUtils.filterChat(text)
			}
//			var cleanText:String = res;					
			var data:Array = [];
			data.push(GameCommonData.Player.Role.Name);
			data.push(target);				//目标
			data.push("0"); 	//0_0_0
			data.push(res); 			// cleanText
			data.push(item);				// 物品
			data.push(color);				// 颜色
			data.push(type);
			data.push(0);
			data.push(0);
			data.push(0);
			data.push(0);
			data.push(jobId);				//门派聊天  时携带职业ID
			return data;
		} 
		
		/** 发送本地消息 */
		private function sendLocalMsg(obj:Object):void
		{
			var chatObj:Object = {};
			var talkObj:Array = [];
			
			talkObj.push(GameCommonData.Player.Role.Name);
			talkObj.push(obj.data[1]);	//接收方
			talkObj.push(0);
			talkObj.push(obj.data[3]);
//			if(obj.data[1] == "ALLUSER") { 
//				talkObj.push("<3_你_0><3_：_1>" + obj.data[3]);
//			} else {
//				talkObj.push("<3_你对_0><3_["+obj.data[1]+"]_12><3_说：_1>" + obj.data[3]);
//			}
			talkObj.push(0);
			
			chatObj.nAtt   = obj.data[6];	//频道
			chatObj.nColor = obj.data[5];	//颜色
			if(chatObj.nAtt == 2041) {
				var jobId:uint = (ChatData.curSelectModel == 4) ? GameCommonData.Player.Role.MainJob.Job : GameCommonData.Player.Role.ViceJob.Job;
				chatObj.nItemTypeID = jobId;
			}
			chatObj.talkObj = talkObj;
			
			facade.sendNotification(CommandList.RECEIVECOMMAND, chatObj);
		}
	}
}