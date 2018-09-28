package GameUI.Modules.Stall.Command
{
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.UIUtils;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class StallBBSReceiveCommand extends SimpleCommand
	{
		public function StallBBSReceiveCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var chatObj:Object = notification.getBody(); 			
			if(chatObj == null) return;
			dealMsgInfo(chatObj);
		}
		
		private function dealMsgInfo(chatObj:Object):void {
			switch(chatObj.nAtt)
			{
				case 2031:	//修改摊位名
					StallConstData.stallName = chatObj.talkObj[3].toString();
					sendNotification(StallEvents.UPDATESTALLNAME);
					break;
				case 2032:	//修改摊位介绍信息
					StallConstData.stallOwnerName = chatObj.talkObj[0].toString();
					StallConstData.stallInfo = chatObj.talkObj[3].toString();
					sendNotification(StallEvents.UPDATESTALLOWNERNAME);
					sendNotification(StallEvents.UPDATESTALLINFO);
					break;
				case 2033: //摊位留言信息
					getStallBBSMsg(chatObj);
					sendNotification( StallEvents.UPDATESTALLMSG,chatObj.talkObj[0]  );   //把摊主的名字发出去
					break;
			}
		}
		
		/** 处理摆摊留言消息 */
		private function getStallBBSMsg(obj:Object):void
		{
			//系统:<dfg78979[PM]>购买10个<血药>， 总价：10
			//<dfg78979[PM]>:留言2
			var info:String = obj.talkObj[3].toString();
//			trace("======================================================");
//			trace(info);
			var htmlText:String = "";
			var regExpName:RegExp = /(<[\w\d\u005b\\u0391-\uFFE5]+>)/g;
			var regExpItem:RegExp = /(\{.*?\})/gs;
			var infoArr:Array=info.split(regExpName);
			
			var index:int=0;
			while (index < infoArr.length) {
				if (regExpName.test(infoArr[index])) {
					var name:String = infoArr[index].substring(1,infoArr[index].length-1);
					if(name != GameCommonData.Player.Role.Name) {
						infoArr[index] = '<font color="#0096cd"><a href="event:name_'+name+'">(*' + name + '*)</a></font>'
					} else {
						infoArr[index] = '<font color="#0096cd">(*' + GameCommonData.wordDic[ "mod_stall_com_sbbsrc_1" ] + '*)</font>'   //"你"
					}
				}
				index++;
			}
			index = 0;
			while (index < infoArr.length) {
				htmlText += infoArr[index];
				index++
			}
			index = 0;
			infoArr = htmlText.split(regExpItem);
			while (index < infoArr.length) {
				if(regExpItem.test(infoArr[index])) {
					var posBegin:int = (infoArr[index] as String).indexOf("{");
					var posEnd:int   = (infoArr[index] as String).indexOf("}");
					var item:String = infoArr[index].substring(posBegin+1, posEnd);
					var itemArr:Array = item.split("_");
					infoArr[index] = '<font color="#92d050"><a href="event:item_'+itemArr[0] + '_' + itemArr[1] +'">[' + itemArr[2] + ']</a></font>'
				}
				index++;
			}
			index = 0;
			htmlText = "";
			while (index < infoArr.length) {
				htmlText += infoArr[index];
				index++;
			}
			htmlText = htmlText.replace(GameCommonData.wordDic[ "mod_stall_com_sbbsrc_2" ], GameCommonData.wordDic[ "mod_stall_com_sbbsrc_3" ] )     //"系统："    "[系统]："
			if(htmlText.indexOf(GameCommonData.wordDic[ "mod_stall_com_sbbsrc_3" ] ) == 0) {         //"[系统]："
				var i:int = htmlText.lastIndexOf(GameCommonData.wordDic[ "mod_stall_com_sbbsrc_4" ]);   //"总价："
				var moneyStr:String = htmlText.substring(i, htmlText.length);	//总价：200
				var money:uint = int(htmlText.substring(i+3, htmlText.length));//200
				var moneyResult:String = UIUtils.getMoneyInfo(money);
				htmlText = htmlText.replace(moneyStr, GameCommonData.wordDic[ "mod_stall_com_sbbsrc_4" ]+moneyResult);   //"总价："
			}
			htmlText = htmlText.replace(GameCommonData.wordDic[ "mod_stall_com_sbbsrc_3" ] , GameCommonData.wordDic[ "mod_stall_com_sbbsrc_5" ])  //"[系统]："     "<font color='#dada00'>[系统]</font>："
			htmlText = htmlText.replace("(*", "[");
			htmlText = htmlText.replace("*)", "]");
			
			if(StallConstData.stallMsg.indexOf(htmlText) < 0) {
				StallConstData.stallMsg.push(htmlText);
			} 
//			else {
//				trace("repeat==",htmlText);
//			}
			
		}
		
	}
}