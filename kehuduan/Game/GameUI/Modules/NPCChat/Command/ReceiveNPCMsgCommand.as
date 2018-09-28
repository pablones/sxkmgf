package GameUI.Modules.NPCChat.Command
{
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.Modules.NPCChat.Proxy.PipeDataProxy;
	import GameUI.MouseCursor.DelayOperation;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ReceiveNPCMsgCommand extends SimpleCommand
	{
		public function ReceiveNPCMsgCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var pipeDataProxy:PipeDataProxy=facade.retrieveProxy(PipeDataProxy.NAME) as PipeDataProxy;
			var obj:Object=notification.getBody();
			switch (notification.getBody()["action"]){
				case NPCChatComList.MSGDIALOG_TEXT:
					var desText:String=notification.getBody()["szText"].toString();
					pipeDataProxy.desText+=desText;
					break;
				case NPCChatComList.MSGDIALOG_LINK:
						var szText:String=obj["szText"];
						var startIndex:int=szText.indexOf("[");
						var endIndex:int=szText.indexOf("]");
						if(startIndex!=-1 && endIndex!=-1){
							var index:String=szText.substring(startIndex+1,endIndex);
							var colorIndex:uint=(uint(index)/10);
							var str:String=DialogConstData.getInstance().getSymbolName((uint(index)%10));
							pipeDataProxy.linkArr.push({iconUrl:str,showText:szText.substring(endIndex+1),linkText:obj["nData"],linkColor:colorIndex});
						}else{
							pipeDataProxy.linkArr.push({iconUrl:"symbol",showText:obj["szText"],linkText:obj["nData"],linkColor:0});
						}
					break;
				case NPCChatComList.MSGDIALOG_SHOW:
						DelayOperation.getInstance().unLockNpcTalk();
						pipeDataProxy.desText='<font face="宋体"  size="12" color="#ffffff">'+ changeStr(pipeDataProxy.desText)+'</font>';       //"宋体"
						sendNotification(NPCChatComList.SHOW_NPC_CHAT);
						GameCommonData.NPCDialogIsOpen = true;
					break;	
			}
		}
		
		protected function changeStr(str:String):String{
			var tempStr:String=str;
			while(tempStr.indexOf("|")>=0){
				tempStr=tempStr.replace("|","\n");
			}
			return tempStr;
			
		}
		
		
		
	}
}