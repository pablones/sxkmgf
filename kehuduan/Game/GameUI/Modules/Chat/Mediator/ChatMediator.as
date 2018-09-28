package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.FacePanelMediator;
	import GameUI.Modules.Chat.Command.HeadTalkCommand;
	import GameUI.Modules.Chat.Command.ReceiveChatCommand;
	import GameUI.Modules.Chat.Command.SendCommand;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.UI.TextFieldColor;
	import GameUI.Modules.PreventWallow.Data.PreventWallowData;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ChatMediator extends Mediator
	{
		public static const NAME:String = "ChatMediator";
		
		private var selectChannelMedator:SelectChanalMediator = null;
		private var dataProxy:DataProxy = null;
		private var msgArea:Sprite = null;
		private var iScrollPane:UIScrollPane;
		private var createChannelMediator:CreateChannelMediator = new CreateChannelMediator();
		private var filterListMediator:FilterListMediator = new FilterListMediator();
		private var selectColorMediator:SelectColorMediator = new SelectColorMediator();
		private var leoPanelMediator:LeoPanelMediator = new LeoPanelMediator();
		private var facePanelMediator:FacePanelMediator = new FacePanelMediator();
		private var msgMediator:MsgMedaitor;
		private var curColor:uint = 0;
		private var isHideMouse:Boolean = false;
		private var curIndex:int = 0;
		private var maxItem:int = 2;
		/** 当要册除物品的缓冲文字 */
		private var chacheInputStr:String="";
		/** 删除物品的正则表达式  */
		private var reg:RegExp=/(.*)(<[0-9\u4E00-\u9FA5]*>)$/gm;
		
		/** 欢迎公告当前播放下标 */
		private var curWelcomeIndex:int = 0;
		
		/** 聊天框文本颜色调整 */
		private var textFieldColor:TextFieldColor;	
		
		public function ChatMediator()
		{
			super(NAME);
		}
		
		public function get chatView():MovieClip
		{
			return this.viewComponent as MovieClip
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.ENTERMAPCOMPLETE,
				ChatEvents.CLOSECHANNEL,
				ChatEvents.SELECTEDFONTCOLOR,
				EventList.KEYBORADEVENT,
				ChatEvents.QUICKCHAT,
				ChatEvents.SELECTEDFACETOCHAT,
				EventList.HASTEAM,
				EventList.HASUINTY,
				ChatEvents.HAS_MAINJOB_CHANNEL,
				ChatEvents.ADDITEMINCHAT,
				ChatEvents.NO_FATIGUE_WINDOW,
				ChatEvents.SHOW_HIDE_CHAT_FLASH,
				ChatEvents.INIT_CHAT_CHANNEL_VIEW
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.CHAT});
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					selectChannelMedator = new SelectChanalMediator();
					facade.registerMediator(selectChannelMedator);
					facade.registerCommand(ChatEvents.SENDCOMMAND, SendCommand);
					facade.registerCommand(CommandList.RECEIVECOMMAND, ReceiveChatCommand);
					facade.registerCommand( HeadTalkCommand.NAME,HeadTalkCommand );
					chatView.txtInput.addEventListener(Event.CHANGE, txtChgHandler);
					textFieldColor = new TextFieldColor(chatView.txtInput as TextField, 0xFFFFFF, 0x3399FF, 0xFFFFFF);
					initShowHide();
					chatView.addEventListener(Event.ADDED_TO_STAGE, initResize);
				break;
				case EventList.KEYBORADEVENT:
					var code:int = notification.getBody() as int;
					if (dataProxy.arenaMsgPanelIsTyping) // 如果在用竞技场面板输入文字，就由 ArenaMsgMediator 接管通知
					{
						return;
					}
					if (code == 27) // esc
					{
						chatView.txtInput.text = "";
						chatView.stage.focus = null;
						UIConstData.FocusIsUsing = false;
						dataProxy.lastUsedMsgPanel = "chat";
						return;
					}
					if(chatView.stage.focus != chatView.txtInput && !UIConstData.FocusIsUsing && dataProxy.lastUsedMsgPanel == "chat")
					{
						chatView.stage.focus = chatView.txtInput;
						chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
						return;
					}
					if(code == 13 && chatView.stage.focus == chatView.txtInput)
					{
						var msgNow:String = chatView.txtInput.text;
						if(msgNow == "") {
							chatView.stage.focus = null;
							UIConstData.FocusIsUsing = false;
							dataProxy.lastUsedMsgPanel = "chat";
						} else {
							sendMsg(null);
						}
					}
				break;
				case EventList.ENTERMAPCOMPLETE:
					initChatSence();
					sendNotification(EventList.HASUINTY);
				break;
				break;
				case ChatEvents.INIT_CHAT_CHANNEL_VIEW:		//初始化自定义频道位
					initChatChannel();
				break;
				case ChatEvents.CLOSECHANNEL:
					getChannel();
				break;
				case ChatEvents.SELECTEDFONTCOLOR:
					curColor = notification.getBody() as uint;
					ChatData.SelectedMsgColor = curColor;
//					chatView.txtInput.defaultTextFormat = textFormat(curColor);
//					chatView.txtInput.setTextFormat(textFormat(curColor));
					textFieldColor.textColor = curColor;
					chatView.stage.focus = chatView.txtInput;
				break; 
				case ChatEvents.QUICKCHAT:
					var name:String = notification.getBody() as String;
					hasPrivate(name);
					chatView.stage.focus = chatView.txtInput;
					chatView.txtInput.text = "/"+name+" ";
					var obj:Array =  ChatData.channelModel;
					chatView.txtCurChanel.text = ChatData.channelModel[ChatData.curSelectModel].name;
					chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
				break;
				case ChatEvents.SELECTEDFACETOCHAT:
					chatView.stage.focus = chatView.txtInput;
					chatView.txtInput.text += "\\" + notification.getBody() as String; 
					chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
				break;
				case EventList.HASTEAM:
					var bool:Boolean = notification.getBody() as Boolean;
					if(bool)
					{
						ChatData.channelModel[1] = {label:"<font color='#FCB68B'>["+GameCommonData.wordDic[ "mod_chat_med_cha_hand_1" ]+"]</font>:"+GameCommonData.wordDic[ "mod_chat_med_cha_hand_2" ], name:GameCommonData.wordDic[ "mod_chat_med_cha_hand_1" ], channel:2003, rece:"ALLUSER", color:"#FCB68B"};
						// "队"		"组队信息"	 "队"		
					}
					else
					{
						ChatData.channelModel[1] = undefined;
						if(ChatData.curSelectModel == 1) {
							ChatData.curSelectModel = 0;//ChatData.channelModel[0];
							chatView.txtCurChanel.text = ChatData.channelModel[ChatData.curSelectModel].name;
						}
					}
				break;
				case EventList.HASUINTY:
					UnityConstData.iscreating = int(GameCommonData.Player.Role.unityJob-1) / 100;
					if(GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0)		//如果职业除以100得到0，则创建成功，为1，则正在创建中
					{
					  	//if(ChatData.channelModel[3] != undefined) 
					  	 ChatData.channelModel[3] = {label:"<font color='#FF6532'>["+GameCommonData.wordDic[ "mod_chat_med_cha_hand_3" ]+"]</font>:"+GameCommonData.wordDic[ "mod_chat_med_cha_hand_4" ], name:GameCommonData.wordDic[ "mod_chat_med_cha_hand_3" ], channel:2004, rece:"ALLUSER", color:"0xFFF54F"};
					  	 //"帮"		"帮派信息"		"帮"
					}
					else
					{
						ChatData.channelModel[3] = undefined;
						if(ChatData.curSelectModel == 3) {
							ChatData.curSelectModel = 0;//ChatData.channelModel[0];
							chatView.txtCurChanel.text = ChatData.channelModel[ChatData.curSelectModel].name;
						}
					}
				break;
				case ChatEvents.ADDITEMINCHAT:
					//添加物品浏览链接，如果达到最大物品(maxItem)则不再添加，每个物品已&隔开
					//同样的物品则不再添加
					//显示在txtInput是以"<xxxx>"的格式
					//发言内容提交后会清空ChatData.tempItemStr 
					
					var itemStr:String = notification.getBody() as String;
					if(itemStr.split("_")[1].length + chatView.txtInput.text.length >= chatView.txtInput.maxChars) {
						return;
					}
					var tmpItemArr:Array = [];
					//判断输入文本框是否为空
					if(chatView.txtInput.text == "") {
						ChatData.tempItemStr = "";
					} else {
						ChatData.tempItemStr = ChatData.tempItemStr.replace(/^\s*|\s*$/g,"").split(" ").join("");
						tmpItemArr = ChatData.tempItemStr.split("&");
					}
					if(tmpItemArr.length >= maxItem) return;
					
					for(var i:int = 0; i < tmpItemArr.length; i++) {
						if(itemStr == tmpItemArr[i]) return;
					}
					if(ChatData.tempItemStr == "") {
						ChatData.tempItemStr = itemStr;
					} else {
						ChatData.tempItemStr = ChatData.tempItemStr + "&" + itemStr;
					}
					var itemName:String = itemStr.split("_")[1];
					chatView.txtInput.text += "<"+itemName.substring(1, itemName.length-1)+">";
				break;
				case ChatEvents.NO_FATIGUE_WINDOW:		//防沉迷
					var noFatInfo:String = notification.getBody().toString();
					noFatNotice(noFatInfo);
				break;
				case ChatEvents.HAS_MAINJOB_CHANNEL:	//加入职业  创建职业聊天频道
					var jobType:uint = notification.getBody().type as uint;
					createJobChannel(jobType);
				break;
				case ChatEvents.SHOW_HIDE_CHAT_FLASH:
					if(!ChatData.chat_view_is_show) {
						showChatFlash();
					}
				break;
			}
		}
		
		private function initShowHide():void
		{
			chatView["mc_FlashChat"].stop();
			chatView["mc_FlashChat"].mouseEnabled = false;
			chatView["mc_FlashChat"].visible = false;
			chatView.btnShowChat.visible = false;
			chatView.btnHideChat.visible = true;
			chatView.btnShowChat.addEventListener(MouseEvent.CLICK, chatOpHandler);
			chatView.btnHideChat.addEventListener(MouseEvent.CLICK, chatOpHandler);
		}
		
		private function chatOpHandler(e:MouseEvent):void
		{
			var name:String = e.target.name;
			switch(name) {
				case "btnShowChat":
					chatView.btnShowChat.visible = false;
					chatView["mc_FlashChat"].stop();
					chatView["mc_FlashChat"].visible = false;
					ChatData.chat_view_is_show = true;
					sendNotification(ChatEvents.SHOW_HIDE_CHAT_VIEW);
					chatView.btnHideChat.visible = true;
					break;
				case "btnHideChat":
					chatView.btnHideChat.visible = false;
					ChatData.chat_view_is_show = false;
					sendNotification(ChatEvents.SHOW_HIDE_CHAT_VIEW);
					chatView.btnShowChat.visible = true;
					break;
			}
		}
		
		private function showChatFlash():void
		{
			chatView["mc_FlashChat"].visible = true;
			(chatView["mc_FlashChat"] as MovieClip).play();
		}
		
		private function noFatNotice(infoStr:String):void
		{
			var infoResult:String = "<font color='#00ff00'>    " + infoStr + "</font><br>" + ChatData.FATIGUE_STR;
			sendNotification(EventList.DO_FIRST_TIP, {comfrim:toFatPage, cancel:cancelFat, info:infoResult, title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_chat_med_cha_noF" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ] });
			//"小提示"	"完善信息"		"取 消"
		}
		
		private function toFatPage():void
		{
			navigateToURL(new URLRequest(ChatData.FAT_URL), "_blank");
		}
		
		private function cancelFat():void
		{
			
		}
		
		private function textFormat(color:uint=0x000000):TextFormat
		{
			var tf:TextFormat = new TextFormat();
			tf.color = color;
			return tf;
		}
		
		/** 初始化频道位 */
		private function initChatChannel():void
		{
			var channelData:int = ChatData.channelSign;//	2200201;
			var len:int = ChatData.Set1ChannelList.length;
			for(var i:int = 0; i < len; i++) {
				var data1:int = Math.pow(2, i+len);	//个人   高12位
				var data2:int = Math.pow(2, i);		//自定   低12位
				var ret1:int = channelData & data1;
				var ret2:int = channelData & data2;
				ChatData.Set1ChannelList[len-1-i].value = (ret1 > 0) ? true : false;
				ChatData.Set2ChannelList[len-1-i].value = (ret2 > 0) ? true : false;
			}
//			var a:Object = ChatData.Set1ChannelList;
//			var b:Object = ChatData.Set2ChannelList;
//			var c:int = 0;
		}
		
		private function initChatSence():void
		{
			GameCommonData.GameInstance.GameUI.addChild(chatView);
			ChatData.HtmlStyle.parseCSS("a:hover{color:#00FFFF;background-color:black;cursor:none;}");
			ChatData.NameStyle.parseCSS("a:hover{color:#00FF00;background-color:black;cursor:none;}");
			for(var i:int = 0; i<4; i++)
			{
				chatView["mcCh_"+i].addEventListener(MouseEvent.CLICK, selectModel);
				if(i == ChatData.CurShowContent)
				{
					chatView["mcCh_"+i].btnChang.visible = false;
				}
			}
			chatView.txtInput.maxChars = 50;
			chatView.txtCurChanel.text = ChatData.channelModel[ChatData.curSelectModel].name;
			chatView.txtCurChanel.mouseEnabled = false;
			chatView.btnSelectCh.addEventListener(MouseEvent.CLICK, selectChannel);				//选择频道
			chatView.btnSetHeight.addEventListener(MouseEvent.CLICK, setMsgAreaHandler);		//聊天显示区
			chatView.btnCreateChannel.addEventListener(MouseEvent.CLICK, createChannel);		//创建频道
			chatView.btnFilter.addEventListener(MouseEvent.CLICK, FilterMsg);					//屏蔽聊天
			chatView.btnSelectColor.addEventListener(MouseEvent.CLICK, showSelectColor);		//显示颜色选择器
			chatView.btnSetLeo.addEventListener(MouseEvent.CLICK, setLeo);						//设置小喇叭
			chatView.btnSend.addEventListener(MouseEvent.CLICK, sendMsg);						//发送
			chatView.bthSetMouse.addEventListener(MouseEvent.CLICK, setMsgArea);
			chatView.btnClear.addEventListener(MouseEvent.CLICK, clearMsg);						//清屏操作
			chatView.btnFace.addEventListener(MouseEvent.CLICK, selectFace);	
			chatView.addEventListener(KeyboardEvent.KEY_UP, onKeyUp);
			chatView.addEventListener(KeyboardEvent.KEY_DOWN,onKeyDown);
			chatView.txtInput.addEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			chatView.txtInput.addEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
			creatorMsgArea();
			//-----创建职业聊天频道---------------------------------------------
			var jobId:uint = GameCommonData.Player.Role.MainJob.Job;
			var job:String = "";
			if(jobId != 0 && jobId != 4096) {	//有正当主职业
				job = dataProxy.RolesListDic[jobId];
				ChatData.channelModel[4] = {label:"<font color='#FFFF00'>[" + ((jobId == 32) ? job.substr(1, 1) : job.substr(0, 1)) + "]</font>:" + job + GameCommonData.wordDic[ "mod_chat_med_cha_ini" ], name:(jobId == 32) ? job.substr(1, 1) : job.substr(0, 1), channel:2041, rece:"ALLUSER", color:"0xFFFF00"};
				//"信息"
			}
			jobId = GameCommonData.Player.Role.ViceJob.Job;
			if(jobId != 0 && jobId != 4096) {	//有正当副职业
				job = dataProxy.RolesListDic[jobId];
				ChatData.channelModel[5] = {label:"<font color='#FFFF00'>[" + ((jobId == 32) ? job.substr(1, 1) : job.substr(0, 1)) + "]</font>:" + job + GameCommonData.wordDic[ "mod_chat_med_cha_ini" ], name:(jobId == 32) ? job.substr(1, 1) : job.substr(0, 1), channel:2041, rece:"ALLUSER", color:"0xFFFF00"};
				//"信息"
			}
			//---------------------------------------------------------------------
			for(var index:int = 0; index < ChatData.WELCOME_ARR.length; index++) {
				if(GameCommonData.wordVersion == 1)
				{
					if(!PreventWallowData.PREVENT_CHAT_DATA[3])
				    {
				    	PreventWallowData.PREVENT_CHAT_DATA[3] = true;
				    	facade.sendNotification(CommandList.RECEIVECOMMAND,{info:PreventWallowData.PREVENT_CHAT_DATA[0], nAtt:77777});
				    }
				}
				facade.sendNotification(CommandList.RECEIVECOMMAND,{info:ChatData.WELCOME_ARR[index], nAtt:77777});
			}
			setInterval(showWelcome, ChatData.WELCOME_INTERVAL * 60 * 1000);	 		//每隔10分钟发一次欢迎  
			setTimeout(startNotice, 20000);												//1分钟后启动 随机5分钟的小提示 (这里目的是与上面的欢迎错开)
		}
		
		private function createJobChannel(jobType:uint):void
		{
			var jobId:uint = 0;
			var job:String = "";
			if(jobType == 1) {				//主
				if(ChatData.channelModel[4] != undefined) return;
				jobId = GameCommonData.Player.Role.MainJob.Job;
				if(jobId != 0 && jobId != 4096) {
					job = dataProxy.RolesListDic[jobId];
					ChatData.channelModel[4] = {label:"<font color='#FFFF00'>[" + ((jobId == 32) ? job.substr(1, 1) : job.substr(0, 1)) + "]</font>:" + job + GameCommonData.wordDic[ "mod_chat_med_cha_ini" ], name:(jobId == 32) ? job.substr(1, 1) : job.substr(0, 1), channel:2041, rece:"ALLUSER", color:"0xFFFF00"};
					//"信息"
				}				
			} else if(jobType == 2) {		//副	
				if(ChatData.channelModel[5] != undefined) return;
				jobId = GameCommonData.Player.Role.ViceJob.Job;
				if(jobId != 0 && jobId != 4096) {
					job = dataProxy.RolesListDic[jobId];
					ChatData.channelModel[5] = {label:"<font color='#FFFF00'>[" + ((jobId == 32) ? job.substr(1, 1) : job.substr(0, 1)) + "]</font>:" + job + GameCommonData.wordDic[ "mod_chat_med_cha_ini" ], name:(jobId == 32) ? job.substr(1, 1) : job.substr(0, 1), channel:2041, rece:"ALLUSER", color:"0xFFFF00"};
					//"信息"
				}
			}
		}
		
		private function showWelcome():void
		{
			if(curWelcomeIndex == ChatData.WELCOME_ARR.length) curWelcomeIndex = 0;
			facade.sendNotification(CommandList.RECEIVECOMMAND,{info:ChatData.WELCOME_ARR[curWelcomeIndex], nAtt:77777});
			curWelcomeIndex++;
		}

		private function startNotice():void
		{
			setInterval(showNotice, ChatData.NOTICE_HELP_INTERVAL * 60 * 1000);			//每隔5分钟随机发一次提示  
			showNotice(); 
		}
		
		private function showNotice():void
		{
			var ran:uint = Math.random() * ChatData.NOTICE_ARR.length; 
			while(ran == ChatData.lastPlayIndex) {
				ran = Math.random() * ChatData.NOTICE_ARR.length;
			}
			ChatData.lastPlayIndex = ran;
			facade.sendNotification(CommandList.RECEIVECOMMAND,{info:ChatData.NOTICE_ARR[ran], nAtt:4001});
		}
		
		private function onFoucsIn(event:FocusEvent):void { GameCommonData.isFocusIn = true; ChatData.txtIsFoucs = true;}
		
		private function onFoucsOut(event:FocusEvent):void { GameCommonData.isFocusIn = false; ChatData.txtIsFoucs = false;}
		
		private function selectModel(event:MouseEvent):void
		{
			var index:int = event.currentTarget.name.split("_")[1];
			if(ChatData.CurShowContent == index) return;
			for(var i:int = 0; i<4; i++)
			{
				chatView["mcCh_"+i].btnChang.visible = true;
			}
			ChatData.CurShowContent = index;
			chatView["mcCh_"+index].btnChang.visible = false;
			facade.sendNotification(ChatEvents.CHANGEMSGAREA);
			if(ChatData.CreateChannelIsOpen) {	//关闭创建频道
				facade.sendNotification(ChatEvents.CLOSECREATORCHANNEL);
			}
		}
		
		private function getChannel():void
		{
			if(ChatData.channelModel[ChatData.curSelectModel]) {
				chatView.txtCurChanel.text = ChatData.channelModel[ChatData.curSelectModel].name;
				chatView.stage.focus = chatView.txtInput;
				var msg:String = chatView.txtInput.text;
				if(msg.charAt(0) == "/") {
					 var tmpArr:Array = msg.split(" ");
					 tmpArr.shift();
					 msg = tmpArr.join("");
				}
				if(ChatData.channelModel[ChatData.curSelectModel].rece != "ALLUSER")
				{
					chatView.txtInput.text = "/"+ChatData.channelModel[ChatData.curSelectModel].label.split(":")[1] + msg;
					chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
				} else {
					chatView.txtInput.text = msg;
				}
			}
		}
		
		private function hasPrivate(name:String):void
		{
			for(var i:int = 0; i<ChatData.channelModel.length; i++)
			{
				if(ChatData.channelModel[i] == undefined) continue;
				if(ChatData.channelModel[i].rece == name)
				{
					ChatData.curSelectModel = i;
					return;
				}
			}
			if(ChatData.channelModel.length < 9)
			{
				ChatData.channelModel.push({label:"<font color='#FF4A79'>["+GameCommonData.wordDic[ "mod_chat_med_cha_has" ]+"]</font>:"+name+" ",name:GameCommonData.wordDic[ "mod_chat_med_cha_has" ], channel:2001, rece:name,color:"#FF4A79"});//"密"
				ChatData.curSelectModel = ChatData.channelModel.length-1;
				return;
			}
			else
			{
				ChatData.channelModel.splice(6, 1);
				ChatData.channelModel.push({label:"<font color='#FF4A79'>["+GameCommonData.wordDic[ "mod_chat_med_cha_has" ]+"]</font>:"+name+" ",name:GameCommonData.wordDic[ "mod_chat_med_cha_has" ], channel:2001, rece:name,color:"#FF4A79"});//"密"
				ChatData.curSelectModel = ChatData.channelModel.length-1;
				return;
			}
		}
		
		private function selectChannel(event:MouseEvent):void
		{
			if(!ChatData.SelectChannelOpen)
			{
				facade.sendNotification(ChatEvents.OPENCHANNEL);
				ChatData.SelectChannelOpen = true;
			}
			else
			{
				facade.sendNotification(ChatEvents.CLOSESELECTCHANNEL);
			}
		}
		
		//创建信息区
		private function creatorMsgArea():void
		{
			msgMediator = new MsgMedaitor(msgArea)			
			facade.registerMediator(msgMediator);
			facade.sendNotification(ChatEvents.CREATORMSGAREA);
		}
		
		//改变信息区大小
		private function setMsgAreaHandler(event:MouseEvent):void
		{
			facade.sendNotification(ChatEvents.CHANGEHEIGHT);
		}
		
		//显示创建频道
		private function createChannel(event:MouseEvent):void
		{
			if(ChatData.CurShowContent<2) 
			{
				facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_chat_med_cha_cre" ], name:"", nAtt:9999});//"只有[个人]和[自定]频道可以设置"
				return;
			}
			if(!ChatData.CreateChannelIsOpen)
			{
				ChatData.CreateChannelIsOpen = true;
				facade.registerMediator(createChannelMediator);
				facade.sendNotification(ChatEvents.CREATORCHANNEL);
			}
			else
			{
				facade.sendNotification(ChatEvents.CLOSECREATORCHANNEL);
			}
		}
		
		//显示屏蔽列表
		private function FilterMsg(event:MouseEvent):void
		{
			if(!ChatData.FilterListIsOpen)
			{
				facade.registerMediator(filterListMediator);
				facade.sendNotification(ChatEvents.SHOWFILTERLIST);
				ChatData.FilterListIsOpen = true; 
			}
			else
			{
				facade.sendNotification(ChatEvents.HIDEFILTERLIST);
			}
		}
		
		//显示选择器
		private function showSelectColor(event:MouseEvent):void
		{
			if(!ChatData.ColorIsOpen)
			{
				facade.registerMediator(selectColorMediator);
				facade.sendNotification(ChatEvents.SHOWCOLOR);	
			}
			else
			{
				facade.sendNotification(ChatEvents.HIDECOLOR);	
			}
		}
		
		//设置小喇叭
		private function setLeo(event:MouseEvent):void
		{
			if(!ChatData.SetLeoIsOpen)
			{
				if(ChatData.SetBigLeoIsOpen) sendNotification(ChatEvents.CLOSE_BIG_LEO);	//先关闭大喇叭
				facade.registerMediator(leoPanelMediator);
				facade.sendNotification(ChatEvents.CREATELRO);
				ChatData.SetLeoIsOpen = true;
			}
			else
			{
				facade.sendNotification(ChatEvents.CLOSELEO);	
			}
		}
		
		private function sendMsg(event:MouseEvent):void
		{
			dataProxy.lastUsedMsgPanel = "chat";
			
			var msgToSend:String = chatView.txtInput.text;
			msgToSend = msgToSend.replace(/^\s*/g, "");    //去掉前面的空格
			msgToSend = msgToSend.replace(/\s*$/g, "");    //去掉后面的空格
			if(msgToSend == "") 
			{
				facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_chat_med_cha_send" ], name:"", nAtt:9999});//"发言内容不能为空"
//				ChatData.curSelectModel = 2; 
//				getChannel();
//				chatView.stage.focus = null;
//				UIConstData.FocusIsUsing = false;
//				chatView.stage.focus = null; 
//				UIConstData.FocusIsUsing = false;
				return;
			}
			var obj:Object = {};
			if(ChatData.channelModel[ChatData.curSelectModel].rece != "ALLUSER" || chatView.txtInput.text.charAt(0) == "/")
			{
				var msg:String = getItemStr();			//chatView.txtInput.text;
				
				var msgArr:Array = msg.split(" ");
				var name:String = msgArr.shift();
				obj.name = name.slice(1,name.length);
				var tmpPriStr:String = msgArr.join(" ");
				
				tmpPriStr = tmpPriStr.replace(/^\s*/g, "");		//去掉前面的空格
				tmpPriStr = tmpPriStr.replace(/\s*$/g, "");		//去掉后面的空格

				if(tmpPriStr == "") 
				{
					facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_chat_med_cha_send" ], name:"", nAtt:9999});
//					ChatData.tmpChatInfo.pop();
//					ChatData.curSelectModel = 2;
//					getChannel();
//					chatView.stage.focus = null;;
//					chatView.stage.focus = null; 
//					UIConstData.FocusIsUsing = false;
					return;
				}
				
				if(filterChatInfo(chatView.txtInput.text))
				{
					ChatData.tmpChatInfo.push({txt:chatView.txtInput.text, tmpItemStr:ChatData.tempItemStr});
				}
				
				obj.talkMsg = tmpPriStr;
				hasPrivate(obj.name);
				chatView.txtCurChanel.text = ChatData.channelModel[ChatData.curSelectModel].name;
				chatView.txtInput.text = "/"+obj.name + " ";
				obj.type = ChatData.channelModel[ChatData.curSelectModel].channel;
				obj.color = ChatData.SelectedMsgColor;
				obj.jobId = 0;
				chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
			}
			else
			{
				obj.jobId = 0;
				if(ChatData.curSelectModel == 4) {
					obj.jobId = GameCommonData.Player.Role.MainJob.Job;
				} else if(ChatData.curSelectModel == 5){
					obj.jobId = GameCommonData.Player.Role.ViceJob.Job;
				}
				obj.name = ChatData.channelModel[ChatData.curSelectModel].rece
				obj.type = ChatData.channelModel[ChatData.curSelectModel].channel;
				obj.talkMsg = getItemStr();		//chatView.txtInput.text;
				obj.color = ChatData.SelectedMsgColor;
				if(filterChatInfo(chatView.txtInput.text))
				{
					ChatData.tmpChatInfo.push({txt:chatView.txtInput.text, tmpItemStr:ChatData.tempItemStr});
//					ChatData.tmpChatInfo.push(chatView.txtInput.text);
				}
				chatView.txtInput.text = "";
			}
			chatView.stage.focus = chatView.txtInput;
			if(ChatData.tmpChatInfo.length > 10)
			{
				ChatData.tmpChatInfo.shift();
			}
			obj.item = ChatData.tempItemStr;
			curIndex = 0;	//清0
			ChatData.tempItemStr = "";
			facade.sendNotification(ChatEvents.SENDCOMMAND, obj);
			//--------
//			chatView.stage.focus = null; 
//			UIConstData.FocusIsUsing = false; 
		}
		
		/** 监听输入，禁止输入<> */
		private function txtChgHandler(e:Event):void
		{
			var str:String = chatView.txtInput.text.substring(chatView.txtInput.text.length-1, chatView.txtInput.text.length)
			if(!str) return;
			if(str == "<") {	//str == ">" || 
				chatView.txtInput.text = chatView.txtInput.text.substring(0, chatView.txtInput.text.length-1);
			}
		}
		
		/** 获取带物品的字符串 */
		private function getItemStr():String
		{
			var result:String = "";
			ChatData.tempItemStr = ChatData.tempItemStr.replace(/^\s*|\s*$/g,"").split(" ").join("");
			if(ChatData.tempItemStr) {
				var reg:RegExp = /(<.*?>)/g;
				var arr:Array  = ChatData.tempItemStr.split("&");
				var targetArr:Array = chatView.txtInput.text.split(reg);
				var pos:uint = 0;
				for each(var s:String in targetArr) {
					if(reg.test(s)) {
						result += arr[pos];
						pos++;
					} else {
						result += s;
					}
				}
			} else {
				result = chatView.txtInput.text;
			}
			while(result.indexOf("undefined") >= 0) {
				result = result.replace("undefined", "");
			}
			
			var regL:RegExp = /(<.*?>)/g;
			var textArr:Array = result.split(regL);
			
			for each(var sT:String in textArr) {
				if(sT != "") {
					if(regL.test(sT)) {
						if(ChatData.tempItemStr.indexOf(sT) < 0)
							result = result.replace(sT, "");
					}
				}
			}
			
			return result;
		}
		
		/**
		 *  键般按下事件处理
		 * @param e
		 * 
		 */		
		private function onKeyDown(e:KeyboardEvent):void{
			if(e.keyCode==8 ){
				this.chacheInputStr=chatView.txtInput.text;
			}	
		}
		/**
		 * 
		 * 
		 * @param event
		 * 
		 */		
		private function onKeyUp(event:KeyboardEvent):void
		{
			if(	chatView.stage.focus != chatView.txtInput )return ;
			if(event.keyCode == 40)  {
			 	curIndex++;
			 	if(curIndex>ChatData.tmpChatInfo.length-1) curIndex=0;
			 	if(ChatData.tmpChatInfo[curIndex]) {
//					chatView.txtInput.text = ChatData.tmpChatInfo[curIndex];
					chatView.txtInput.text = ChatData.tmpChatInfo[curIndex].txt;
					ChatData.tempItemStr = ChatData.tmpChatInfo[curIndex].tmpItemStr;
				}
			 	chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
			} else if(event.keyCode == 38) {
				curIndex--;
				if(curIndex < 0) curIndex = ChatData.tmpChatInfo.length-1;
				if(ChatData.tmpChatInfo[curIndex]) {
//					chatView.txtInput.text = ChatData.tmpChatInfo[curIndex]; 
					chatView.txtInput.text = ChatData.tmpChatInfo[curIndex].txt;
					ChatData.tempItemStr = ChatData.tmpChatInfo[curIndex].tmpItemStr; 
				}
				chatView.txtInput.setSelection(chatView.txtInput.length, chatView.txtInput.length);
			} else if(event.keyCode==8){  //删除，回退处理
				var reg1:RegExp=/(<.*?>)/g;
				var arr:Array=this.chacheInputStr.split(reg1);
				var str:String="";
				var pos:uint;
				var flag:Boolean=false;
				var index:uint;
				for each(var s:String in arr){
					if(reg1.test(s)){
						if(!flag && chatView.txtInput.caretIndex>=str.length && chatView.txtInput.caretIndex<str.length+s.length){
							pos=str.length;
							flag=true;
							continue;
						}
						if(!flag){
							index++;
						}
					}
					str+=s;
				}
				if(flag){
					chatView.txtInput.setSelection(pos,pos);
					chatView.txtInput.text=str;
					var itemArr:Array = ChatData.tempItemStr.split("&");
					if(itemArr.length == 2) {
						ChatData.tempItemStr = itemArr[Math.abs(index - 1)];
					} else {
						ChatData.tempItemStr = "";
					}
				}
			}
		}
		
		private function filterChatInfo(str:String):Boolean
		{
			for(var i:int = 0; i<ChatData.tmpChatInfo.length; i++)
			{
				if(str == ChatData.tmpChatInfo[i].txt)
				{
					return false;
				}
			}
			return true;
		}
		
		//选择表情
		private function selectFace(event:MouseEvent):void
		{
			if(!UIConstData.SelectFaceIsOpen)
			{
				facade.registerMediator(facePanelMediator);
				facade.sendNotification(EventList.SHOWFACEVIEW );	
			}
			else
			{
				facade.sendNotification(EventList.HIDESELECTFACE);	
			}
		}
		
		private function setMsgArea(event:MouseEvent):void
		{
			facade.sendNotification(ChatEvents.CHANGEMOUSE);
		}
	
		private function clearMsg(event:MouseEvent):void
		{
			facade.sendNotification(CommandList.RECEIVECOMMAND);
		}
		
		protected function initResize(event:Event):void
		{
			sendNotification(EventList.STAGECHANGE);
		}
	}
}