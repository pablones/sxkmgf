package GameUI.Modules.Chat.Command
{
	import GameUI.Modules.Arena.Data.ArenaScore;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PreventWallow.Data.PreventWallowEvent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Modules.Unity.Command.KeepOnCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ReceiveChatCommand extends SimpleCommand
	{
		public function ReceiveChatCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var chatObj:Object = notification.getBody(); 			

			if(chatObj == null) {
				clearMsg();
				return;
			}
			
			//收到心跳消息
			if(chatObj.nAtt == 2208)
			{
				if(ChgLineData.flyServerInfo == "")
				{
					if(chatObj.talkObj[3])
					{
						facade.sendNotification(ChgLineData.UPDATA_SERVER,{info:chatObj.talkObj[3]});
					}
				}else
				{
					if(chatObj.talkObj[3])
					{
						ChgLineData.isChooseLine = false;
						facade.sendNotification(ChgLineData.CHG_LINE_GO,{info:chatObj.talkObj[3]});
					}
				}
				return;
			}
			
			//个人信息
			if(chatObj.nAtt == 9999) {
				chatObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_exe_1" ]+']_7>' + chatObj.htmlText;//"个人"
				chatObj.dfColor = 0xFFFFFF;
				pushMsg(chatObj, 1007);
				return;
			}
			
			if(chatObj.nAtt == 2037) {
				if(!chatObj.talkObj || !chatObj.talkObj[3]) {
					return;
				}
				chatObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_exe_2" ]+']_7>' + chatObj.talkObj[3];//"帮助"
				chatObj.dfColor = 0xFFFFFF;
				pushMsg(chatObj, 2037);
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:chatObj.talkObj[3], color:0xffff00});		//头上飘黄字
				return;
			}
			
			//默认   这里是后来修改的，专门用来处理进入游戏的第一句话 
			if(chatObj.nAtt == 77777) {
				chatObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_exe_3" ]+']：_8>' + chatObj.info;		//chatObj.htmlText = '<3_[公告]_8">' + chatObj.htmlText;
				//"公告"
				chatObj.dfColor = 0xFF0000;
				pushMsg(chatObj, 2035); 
				return;
			}
			
			//用于显示每5分钟随机一次的小提示
			if(chatObj.nAtt == 4001) {
				chatObj.info = '<3_['+GameCommonData.wordDic[ "often_used_tip" ]+']：_2>' + chatObj.info;		//chatObj.htmlText = '<3_[公告]_8">' + chatObj.htmlText;
				//"提示"
				chatObj.dfColor = 0x00A651;
				pushMsg(chatObj, 4001); 
				return;
			}
			
			//防沉迷弹窗，走msgTalk协议
			if(chatObj.nAtt == 2038) {
				var infoStr:String = chatObj.talkObj[3];
				if(GameCommonData.wordVersion == 1)
				{
					/**新平台关闭原有防沉迷流程*/
					if(GameCommonData.isNew == 0)	//旧平台
					{
						sendNotification(PreventWallowEvent.SHOWPREVENTWALLOWBTN, infoStr);
					}
				}
				return;
			}
			
			//屏蔽列表
			for(var i:int = 0; i<ChatData.FilterList.length; i++) {
				if(ChatData.FilterList[i] == chatObj.talkObj[0]) {
					return;
				}
			}
			
			if(chatObj is Object) {
				dealChatInfo(chatObj);
				return;
			}
		}
		
		private function clearMsg():void
		{
			//清屏操作，对应不同的频道
			switch(ChatData.CurShowContent) {
				case 0:
					pushCleanMsg(ChatData.AllMsg);
					facade.sendNotification(ChatEvents.CLEAR_MSG_CUR_CHANNEL);
					break;
				case 1:
					pushCleanMsg(ChatData.SystemMsg);
					facade.sendNotification(ChatEvents.CLEAR_MSG_CUR_CHANNEL);
					break;
				case 2:
					pushCleanMsg(ChatData.Set1Msg);
					facade.sendNotification(ChatEvents.CLEAR_MSG_CUR_CHANNEL);
					break;
				case 3:
					pushCleanMsg(ChatData.Set2Msg);
					facade.sendNotification(ChatEvents.CLEAR_MSG_CUR_CHANNEL);
					break;
			}
		}
		
		//这些是后来修改的，主要用来清屏
		private function pushCleanMsg(arr:Array):Array
		{
			var len:uint = 0;
			switch(ChatData.CurAreaPos) {
				case 0:
					len = 6;
					break;
				case 1:
					len = 10;
					break;
				case 2:
					len = 12;
					break;
			}
			for(var i:int = 0; i < len; i++) {
				var obj:Object = new Object();
				obj.info = "												";
				obj.dfColor = 0xFFFFFF;
				arr.push(obj);
			}
			return arr;
		}
		
		private function dealChatInfo(chatObj:Object):void
		{
//			trace("default color from server : ", chatObj.nColor); 
			switch(chatObj.nAtt) {				
				case 2016:
					getWorldMsg(chatObj, 9);
				break;
				case 2004:		
					getUnityMsg(chatObj, 5);
				break;
				case 2030:
					facade.sendNotification(ChatEvents.RECEIVELEOMSG, getLeoMsg(chatObj, 7));
				break;
				case 2003:
					getTeamMsg(chatObj, 11);	
				break;
				case 2001:
					getPrivateMsg(chatObj, 12);
				break;
				case 2013:
					getMapMsg(chatObj, 7);	
				break;
				case 2041:
					getJobMsg(chatObj, 7);
				break;
				case 2031:					
				break;
				case 2032:					
				break;
				case 2019:					
				break;
				case 2110:					
				break;
				case 2040:
				break;
				case 2000:	
					chatObj.nAtt = 2014; 
					getDefaultMsg(chatObj);		
				break; 
				case 2033:					
				break;
				case 2100:
					chatObj.nAtt = 2014; 
					getDefaultMsg(chatObj);
				break
				case 2110:					
				break;
				case 2034:	
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:chatObj.talkObj[3], color:0xffff00});				
				break;
				case 2035:		//系统公告
					getSysNoticeMsg(chatObj, 8);
				break;
				case 2036:		//滚屏公告
					getScrollNoticeMsg(chatObj, 8);
				break;
				case 2039:		//大喇叭
					getBigLeoMsg(chatObj, 8)
				break;
				case 2118:			//新帮派
				case 2119:
				break;
				case 2046:
					getArenaMsg(chatObj);
				break;
				default:
					getDefaultMsg(chatObj);
				break;
			}
		}
		
		/** 竞技场聊天 */
		private function getArenaMsg(obj:Object):void
		{
			var msgObj:Object = new Object();
			var campStr:String = "<3_[";
			switch (ArenaScore.myCamp)
			{
				case 1:
					campStr += GameCommonData.wordDic[ "mod_are_med_are_upd_7" ] + "]"; // 贪狼
					if (obj.talkObj[0] == GameCommonData.Player.Role.Name)
					{
						campStr += GameCommonData.wordDic["mod_chat_com_rec_mak_4"] + "_16>："; // 你
					}
					else
					{
						campStr += "[" + obj.talkObj[0] + "]_16>：";
					}
				break;
				case 2:
					campStr += GameCommonData.wordDic[ "mod_are_med_are_upd_8" ] + "]"; // 破军
					if (obj.talkObj[0] == GameCommonData.Player.Role.Name)
					{
						campStr += GameCommonData.wordDic["mod_chat_com_rec_mak_4"] + "_17>："; // 你
					}
					else
					{
						campStr += "[" + obj.talkObj[0] + "]_17>：";
					}
				break;
				case 3:
					campStr += GameCommonData.wordDic[ "mod_are_med_are_upd_9" ] + "]"; // 七杀
					if (obj.talkObj[0] == GameCommonData.Player.Role.Name)
					{
						campStr += GameCommonData.wordDic["mod_chat_com_rec_mak_4"] + "_18>："; // 你
					}
					else
					{
						campStr += "[" + obj.talkObj[0] + "]_18>：";
					}
				break;
			}
			
			msgObj.info = campStr + obj.talkObj[3];
			msgObj.dfColor = 0xE2CCA5;
			pushMsg(msgObj, obj.nAtt);
		}
		
		/** 滚屏公告 */
		private function getScrollNoticeMsg(obj:Object, colorIndex:uint=0):void
		{
			var noticeObj:Object = new Object();
			noticeObj.info = obj.talkObj[3];
			noticeObj.nAtt = obj.nAtt;
			facade.sendNotification(ChatData.OPENSCROLLNOTICE, noticeObj);
			
			//同时显示在聊天里
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_exe_3" ]+']：_8>' + obj.talkObj[3];//"公告"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** GM命令 大喇叭 */
		private function getBigLeoMsg(obj:Object, colorIndex:uint=0):void
		{
			//滚屏
			var sendObj:Object = new Object();
			sendObj.nAtt = obj.nAtt;
			sendObj.info = makeChatInfo(obj, colorIndex, 1);//obj.talkObj[3];		//'<3_[大喇叭]：_8>' + obj.talkObj[3];
			facade.sendNotification(ChatData.USE_BIG_LEO, sendObj);
			
			//同时显示在聊天里
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getB" ]+']：_8>' + makeChatInfo(obj, colorIndex);//addSelfName(obj);//"大喇叭"
//			talkObj.dfColor = obj.nColor;
			talkObj.dfColor = 0x66ffff;
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** 公告 */
		private function getSysNoticeMsg(obj:Object, colorIndex:uint=0):void
		{
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_exe_3" ]+']：_8>' + obj.talkObj[3];	//makeChatInfo(obj, colorIndex);////"公告"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** 小喇叭 */
		private function getLeoMsg(obj:Object, colorIndex:uint=0):Object
		{
			var chatObj:Object = new Object();
			chatObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getL" ]+']_7>' + makeChatInfo(obj, colorIndex);//addSelfName(obj);//obj.talkObj[3];//"小喇叭"
			chatObj.dfColor = obj.nColor;
			pushMsg(chatObj, obj.nAtt);
			
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getL" ]+']_7>' + makeChatInfo(obj, colorIndex);//addSelfName(obj);//obj.talkObj[3];//"小喇叭"
			talkObj.dfColor = obj.nColor;
			return talkObj;
		}
		
		/** 世界频道 */
		private function getWorldMsg(obj:Object, colorIndex:uint=0):void
		{
			sendNotification( HeadTalkCommand.NAME,obj ); 
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getW" ]+']_9>' + makeChatInfo(obj, colorIndex);	//addSelfName(obj);//obj.talkObj[3];//"世界"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** 帮派频道 */
		private function getUnityMsg(obj:Object, colorIndex:uint=0):void
		{
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getU_1" ]+']_10>' + makeChatInfo(obj, colorIndex);//addSelfName(obj);//obj.talkObj[3];//"帮派"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
			
			if(obj.talkObj[0] == "SYSTEM") {
				for(var i:int = 0; i < UnityUtils.UnityImportStrArr.length; i++) {
					if(obj.talkObj[3].indexOf(UnityUtils.UnityImportStrArr[i]) >= 0) {				//如果存在数组里德字符，就发送消息
						if(obj.talkObj[3].indexOf(GameCommonData.wordDic[ "mod_chat_com_rec_getU_2" ]) >= 0) continue;	//"请求"						//如果存在“请求加入帮派”，就跳出当前循环
						UnityUtils.getPlayObj(obj.talkObj[3]);
						facade.sendNotification(UnityEvent.UNITYUPDATA ,UnityConstData.updataArr);// 216);	
					}
				}
				facade.sendNotification(KeepOnCommand.KEEPON , obj.talkObj[3]);			//发送截取命令
			}
		}
		
		/** 附近频道 */
		private function getMapMsg(obj:Object, colorIndex:uint=0):void
		{
			sendNotification( HeadTalkCommand.NAME,obj ); 
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getM" ]+']_0>' + makeChatInfo(obj, colorIndex); //addSelfName(obj);//obj.talkObj[3];//"附近"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** 组队频道 */
		private function getTeamMsg(obj:Object, colorIndex:uint=0):void
		{
			sendNotification( HeadTalkCommand.NAME,obj ); //聊天泡泡
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getT" ]+']_11>' + makeChatInfo(obj, colorIndex); //addSelfName(obj);//obj.talkObj[3];//"队伍"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** 其他频道 */
		private function getDefaultMsg(obj:Object, colorIndex:uint=0):void
		{
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getD" ]+']_0>' + makeChatInfo(obj, colorIndex);//obj.talkObj[3];//"系统"
			talkObj.dfColor = ChatData.CHAT_COLORS[0];
			pushMsg(talkObj, obj.nAtt);
		}
		
		/** 私聊频道 */
		private function getPrivateMsg(obj:Object, colorIndex:uint=0):void
		{
			var talkObj:Object = new Object();
			talkObj.info = '<3_['+GameCommonData.wordDic[ "mod_chat_com_rec_getP" ]+']_12>' + makeChatInfo(obj, colorIndex);//obj.talkObj[3];//"私聊"
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
			sendNotification(ChatEvents.SHOW_HIDE_CHAT_FLASH);
		}
		
		/** 门派频道 */
		private function getJobMsg(obj:Object, colorIndex:uint=0):void
		{
			var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var job:String = dataProxy.RolesListDic[obj.nItemTypeID];
			var talkObj:Object = new Object();
			talkObj.info = '<3_[' + job + ']_7>' + makeChatInfo(obj, colorIndex);//addSelfName(obj);
			talkObj.dfColor = obj.nColor;
			pushMsg(talkObj, obj.nAtt);
		}
		
		private function pushMsg(obj:Object, type:int):void
		{
			if(type != 2037 && type != 2046) {
				ChatData.AllMsg.push(obj);
				if(ChatData.AllMsg.length > ChatData.MAX_AMOUNT_MSG) {
					ChatData.AllMsg.shift();
				}
				if(ChatData.CurShowContent == 0) {
					facade.sendNotification(ChatEvents.SHOWMSGINFO, ChatData.AllMsg);
				}
			}
			switch(type) {
				case 2016:	
					getShowMsg(type, obj);			
				break;
				case 2004:
					getShowMsg(type, obj);	
				break;
				case 2003:
					getShowMsg(type, obj);
				break;
				case 2013:
					getShowMsg(type, obj);
				break;
				case 2001:
					getShowMsg(type, obj);
				break;
				case 2039:
					getShowMsg(type, obj);
				break;
				case 2040:
					getShowMsg(type, obj);
				break;
				case 2041:
					getShowMsg(type, obj);
				break;
				case 2046: // 竞技场阵营
					showArenaMsg(obj);
				break;
				default:
					showSystemMsg(obj);
					getShowMsg(type, obj);
				break;
			}
		}
		
		private function showSystemMsg(text:Object):void
		{
			ChatData.SystemMsg.push(text);
			if(ChatData.SystemMsg.length > ChatData.MAX_AMOUNT_MSG) {
				ChatData.SystemMsg.shift();
			}
			if(ChatData.CurShowContent == 1) {
				facade.sendNotification(ChatEvents.SHOWMSGINFO, ChatData.SystemMsg);	
			}
		}
		
		private function getShowMsg(type:int, text:Object):void
		{
			if(!testIsFilter(type, ChatData.Set1ChannelList)) {		//先将消息存下来，如果当前状态不在自定义类型里，则不发送消息
				ChatData.Set1Msg.push(text)
				if(ChatData.Set1Msg.length > ChatData.MAX_AMOUNT_MSG) {
					ChatData.Set1Msg.shift();
				}
				if(ChatData.CurShowContent == 2) {					//发送消息
					facade.sendNotification(ChatEvents.SHOWMSGINFO, ChatData.Set1Msg);	
//					return;
				}
			}
			if(!testIsFilter(type, ChatData.Set2ChannelList)) {
				ChatData.Set2Msg.push(text)
				if(ChatData.Set2Msg.length > ChatData.MAX_AMOUNT_MSG) {
					ChatData.Set2Msg.shift();
				}
				if(ChatData.CurShowContent == 3) {
					facade.sendNotification(ChatEvents.SHOWMSGINFO, ChatData.Set2Msg);
					return;
				}
			}
		}
		
		private function showArenaMsg(text:Object):void
		{
			facade.sendNotification(ChatEvents.ARENA_MESSAGE, text);
		}
		
		private function testIsFilter(type:int, filterList:Array):Boolean
		{
			for( var i:int = 0; i < filterList.length; i++) {
				if(filterList[i].channel == type && !filterList[i].value) {
					return true;
				}
			}
			return false;
		}
		
		/** 拼组件 */
		private function makeChatInfo(obj:Object, colorIndex:uint=0, levType:uint=0):String
		{
			var talkObj:Array   = obj.talkObj as Array;
			
			var sender:String   = talkObj[0];					//发送者
			var receiver:String = talkObj[1];					//接收者
			var userInfo:Array  = UIUtils.IntegerBitwiseAnd(uint(talkObj[2]));		//玩家信息		
//			var userInfo:Array  = String(talkObj[2]).split("_");//玩家信息
			var talkInfo:String = talkObj[3];					//说话内容
//			var channel:uint    = obj.nAtt;						//频道号
//			var hasItem:uint    = talkObj[4]; 					//是否有物品链接
			
			var line:String 	= "";							//玩家所在线
			var sex:String  	= "";							//玩家性别
			var identity:String = ""; 							//玩家身份  0-普通  1-新手指导员  3-GM
			var vip:String = "";
			
			var headSender:String   = "";
			var headRecevier:String = "";
			
			var res:String = "";
			
			if(userInfo.length > 1) {	//有玩家信息
//				line = "<3_" + ChatData.USER_LINE_NAME[uint(userInfo[0])] + "_" + colorIndex+">";
//				sex  = ChatData.USER_SEX[uint(userInfo[1])];
//				identity = ChatData.USER_INDENTITY[uint(userInfo[2])];

				var tmpLine:int = 0;
				if(userInfo[0] == 1) tmpLine+= Math.pow(2, 0); 
				if(userInfo[1] == 1) tmpLine+= Math.pow(2, 1); 
				if(userInfo[2] == 1) tmpLine+= Math.pow(2, 2); 
				
				// Math.pow(2, userInfo[0]) + Math.pow(2, userInfo[1]) + Math.pow(2, userInfo[2]); 
				line = "<3_" + ChatData.USER_LINE_NAME[tmpLine] + "_" + colorIndex+">";
								
				var tmpSex:int = 0; 
				if(userInfo[3] == 1) tmpSex+= Math.pow(2, 0); 
				
				//Math.pow(2, userInfo[3]);
				sex  = ChatData.USER_SEX[tmpSex];
				
				var tmpIdentity:int = 0;
				if(userInfo[4] == 1) tmpIdentity+= Math.pow(2, 0); 
				if(userInfo[5] == 1) tmpIdentity+= Math.pow(2, 1); 
				if(userInfo[6] == 1) tmpIdentity+= Math.pow(2, 2); 
				
				//Math.pow(2, userInfo[4]) + Math.pow(2, userInfo[5]) + Math.pow(2, userInfo[6]);
				identity = ChatData.USER_INDENTITY[tmpIdentity];
				
				var tmpVip:int = 0;
				if(userInfo[7] == 1) tmpVip+= Math.pow(2, 0); 
				if(userInfo[8] == 1) tmpVip+= Math.pow(2, 1); 
				if(userInfo[9] == 1) tmpVip+= Math.pow(2, 2); 
				
				//Math.pow(2, userInfo[7]) + Math.pow(2, userInfo[7]) + Math.pow(2, userInfo[9]);
				if(tmpVip > 0) {
					vip = "<3_[VIP]_" + IntroConst.VIP_COLORS[tmpVip] + ">";
				}
			}
			
			if(receiver != "ALLUSER") {		//私聊
				if(sender == GameCommonData.Player.Role.Name) {	//自己对别人说
					headSender   = "<3_"+GameCommonData.wordDic[ "mod_chat_com_rec_mak_1" ]+"_0>"//"你对"
					headRecevier = "<0_["+receiver+"]_"+colorIndex+">" + "<3_"+GameCommonData.wordDic[ "mod_chat_com_rec_mak_2" ]+"：_0>"; ////"说"
				} else {										//别人对自己说
					headSender   = sex + line + vip + identity + "<0_["+sender+"]_"+colorIndex+">";
					headRecevier = "<3_"+GameCommonData.wordDic[ "mod_chat_com_rec_mak_3" ]+"：_0>";//"对你说"
				}
			} else {
				if(sender == "SYSTEM") {
					headSender = "";
				} else if(sender == GameCommonData.Player.Role.Name && (obj.nAtt != 2039 || levType == 0)) {	//自己发言  大喇叭2039
					headSender = "<3_"+GameCommonData.wordDic[ "mod_chat_com_rec_mak_4" ]+"：_0>";//"你"
				} else {										//别人发言
					headSender = sex + line + vip + identity + "<0_[" + sender+"]_"+colorIndex+">" + "<3_：_0>";
				}
			} 
			
			res = headSender + headRecevier + talkInfo;
			return res;
		}
		
	}
}














