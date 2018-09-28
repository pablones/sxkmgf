package GameUI.Modules.Team.Mediator
{
	import Controller.PlayerController;
	import Controller.TargetController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Icon.Data.IconData;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	import GameUI.Modules.Team.Datas.TeamEvent;
	import GameUI.Modules.Team.Datas.TeamNetAction;
	import GameUI.Modules.Team.UI.TeamItem;
	import GameUI.Modules.Team.UI.TeamModelManager;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionProcessor.TeamAction;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	/**
	 * 组队UI管理器
	 * @author:Ginoo
	 * @date:5/24/2010
	 */
	public class TeamMediator extends Mediator
	{	
		public static const NAME:String = "TeamMediator";
		public static const TEAMDEFAULTPOS:Point = new Point(450,58);			/** UI默认坐标 */
		public static const TEAMITEM_STARTPOS:Point = new Point(9.3, 28);			/** 队员组件开始坐标 */
		public static const TEAMREQLITEM_STARTPOS:Point = new Point(179.3, 28);
		//申请列表的 5个元素  43 * 5 = 215
		private var dataProxy:DataProxy;
		private var teamDataProxy:TeamDataProxy;
		
		private var teamModelManager:TeamModelManager = null;
//		private var loadingMC:MovieClip = null;							/** 正在加载数据MC*/
		private var teamPanel:PanelBase = null;							/** 组队UI基面板 */
		
		private var notiData:Object = new Object();						/** 消息数据 */
		
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		
		private var inviteDataArr:Array = new Array();					/** 某队员邀请某玩家的数据 队列 */
		
		private var inviteData:Object = new Object();					/** 某队员邀请某玩家的数据 */
		
		private var joinPlayerName:String = "";							/** 新加入的玩家名字 */
		
		public function TeamMediator()
		{
			super(NAME);
		}
		
		private function get team():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,					//加载资源
				EventList.SHOWTEAM,					//显示组队界面，在主界面点击“组队”时立刻显示组队面板，然后在此处请求服务器，同时显示lodgingMC，禁用UI上的按钮等，等服务器返回数据给我后，我这边移除loadingMC，更新画面数据，放开UI上的按钮等。
				EventList.UPDATETEAM,				//收到服务器返回的数据，更新界面数据
				EventList.ASKTEAMINFO,				//收到服务器命令，向服务器发送请求获取数据
				EventList.REMOVETEAM,				//删除组队界面
				TeamEvent.HAVAINVITE,				//受到邀请
				EventList.APPLYTEAM,				//申请入队，对外的接口
				EventList.INVITETEAM,				//邀请入队，对外的接口
				EventList.LEAVETEAMCOMMON,			//离开队伍，对外的接口
				EventList.KICKOUTTEAMCOMMON,		//踢出队伍，对外的接口
				EventList.CHANGELEADERTEAMCOMMON,	//移交队长，对外的接口
				EventList.SETUPTEAMCOMMON,			//建立队伍，对外的接口
				TeamEvent.MEMBERINVITESOMEONE,		//某个队员邀请了某人，此时自己是队长
				TeamEvent.SHOWTEAMINFORMATION,		//显示组队相关的提示信息
				TeamEvent.INVITETEAMBYNAME,			//邀请组队，对外接口，用名字查询
				TeamEvent.SUPER_MAKE_TEAM,			//超级组队模式，根据对方ID
				TeamEvent.SUPER_MAKE_TEAM_BY_NAME,	//超级组队模式，根据对方名字 
				TeamEvent.LEAVE_TEAM_AFTER_CHANGE_LINE,	//换线成功，客户端自己清除组队数据
				EventList.MEMBER_ONLINE_STATUS_TEAM		//更新队员在线状态 
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:													//初始化
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.TEAM});
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					teamDataProxy = new TeamDataProxy();
					facade.registerProxy(teamDataProxy);
					facade.registerMediator(new TeamInviteMediator());
					sendNotification(TeamEvent.INIT_INVITE_PANEL);	//初始化邀请面板
					teamModelManager = new TeamModelManager(team, teamDataProxy);
					
					initView();
					break;
				case EventList.REMOVETEAM:													//删除组队面板
					gc();
					break;
				case EventList.SHOWTEAM:													//显示组队界面
					showTeam();
					break;
				case EventList.UPDATETEAM:													//收到服务器返回，更新画面
					var teamInfo:Object = notification.getBody();
					if(teamDataProxy.isInviting) {
						if(teamDataProxy.inviteIsOpen) {
							teamDataProxy.teamMemberList = [];
							teamDataProxy.teamReqList = [];
							teamDataProxy.teamMemSelected = new Object();
							teamDataProxy.teamReqSelected = new Object();
							sendNotification(TeamEvent.SHOWINVITETEAMINFO, teamInfo);
						}
						return;
					}
					if(this.haveChange(teamInfo.teamMemList)) {		//队员列表数据有变化 			//teamInfo.teamMemList
						teamDataProxy.teamMemberList = [];
						teamDataProxy.teamMemSelected = new Object();
						
						teamDataProxy.teamMemberList = teamInfo.teamMemList;
						var isMember:Boolean = false;
						var isLeader:Boolean = false;
						for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {		//通知聊天栏，增加或删除组队栏目
							if(teamDataProxy.teamMemberList[i] && teamDataProxy.teamMemberList[i].id && teamDataProxy.teamMemberList[i].id == GameCommonData.Player.Role.Id) {
								isMember = true;
								if(i == 0) isLeader = true;
								break;
							}
						}
						if(isMember) {				//在队伍中
							if(StallConstData.stallSelfId == 0) {	//未在摆摊中
								if(isLeader) {			//是队长	
									GameCommonData.Player.SetTeamLeader(true);
									dataProxy.isTeamLeader = true;
									dataProxy.isTeamMember = true;
								} else {				//是队员	
									GameCommonData.Player.SetTeam(true);
									dataProxy.isTeamLeader = false;
									dataProxy.isTeamMember = true;
								}
							}
							sendNotification(EventList.HASTEAM, true);
						} else {
							sendNotification(EventList.HASTEAM, false);
							dataProxy.isTeamLeader = false;
							dataProxy.isTeamMember = false;
						}
						clearData(0);
						updateData(0);
					}
					
					teamDataProxy.teamReqList = [];
					teamDataProxy.teamReqSelected = new Object();
					teamDataProxy.teamReqList = teamInfo.teamReqList;
					
					updateView();
					
					clearData(1);						//更新组件数据
					updateData(1);
					
					if(joinPlayerName) {	//有新人加入
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:2});
					}
					break;
				case EventList.ASKTEAMINFO:					//向服务器查询队伍详细信息
					var oB:Object = notification.getBody();
					if(oB.unAction == TeamAction._MSGTEAM_TARGET_INTEAM && teamDataProxy.isInviting) {
						return;
					}
					teamDataProxy.isInviting = false;
					if(oB.unAction == TeamAction._MSGTEAM_JOINTEAM) {
						joinPlayerName = String(oB.szPlayerName);
					} else if(oB.unAction == TeamAction._MSGTEAM_AGREEJOIN) {
						joinPlayerName = GameCommonData.Player.Role.Name;
					}
					
					if(dataProxy.TeamIsOpen) {
						teamModelManager.setModel(0);
					} else if(oB.unAction == TeamAction._MSGTEAM_APPLYJOIN) {//有人申请
						if(teamDataProxy.teamMemberList[0] && teamDataProxy.teamMemberList[0].id == GameCommonData.Player.Role.Id) {//自己是队长
							if(UIConstData.AUTO_ACCEPT_TEAM_INTITE_AND_APPLY) {	//挂机中 自动接受申请和邀请
								TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_AGREEJOIN, oB.idPlayer);
							} else {
								sendNotification(EventList.TEAMBTNRAY);	//组队按钮闪烁
								sendNotification(IconData.SHOW_ICON, {index:2, message:"zhudui"});
								sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:16});
							}
						}
					} else if(oB.unAction == TeamAction._MSGTEAM_INVITE) {	//有人邀请自己
						teamDataProxy.isInviting = true;
						sendNotification(EventList.TEAMBTNRAY);//组队按钮闪烁
						sendNotification(IconData.SHOW_ICON, {index:2, message:"zhudui"});
					}
					sendData(TeamAction._MSGTEAM_ASKINFO); 
					break;
				case TeamEvent.HAVAINVITE:
					var inviteObj:Object = notification.getBody();
					if(UIConstData.AUTO_ACCEPT_TEAM_INTITE_AND_APPLY) {		//挂机 自动接受邀请
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ACCEPTINVITE, inviteObj.idPlayer, 0, "");
					} else {
						teamDataProxy.isInviting = true;
						if(teamDataProxy.inviteIsOpen) {
	//						sendNotification(TeamEvent.REMOVEINVITE);
							sendNotification(TeamEvent.INVITEINIT);
						} else {
							sendNotification(EventList.TEAMBTNRAY);//组队按钮闪烁
							sendNotification(IconData.SHOW_ICON, {index:2, message:"zhudui"});
						}
						gc();
					}
					break;
				case EventList.APPLYTEAM:						//申请入队，对外接口					
					notiData = notification.getBody();
					if(TargetController.IsPKTeam())
					{
						var player:GameElementAnimal = PlayerController.GetPlayer(int(notiData));
						if(player != null)
						{
							if(GameCommonData.Player.Role.PKteam == player.Role.PKteam)
							{
								sendData(TeamAction._MSGTEAM_APPLYJOIN);
							}
							else
							{
								UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_team_med_teamm_jjc_1"],0xffff00);  // "必须同一阵营"
							}
						}
						else
						{
							sendData(TeamAction._MSGTEAM_APPLYJOIN);
						}	
					}
					else
					{
						sendData(TeamAction._MSGTEAM_APPLYJOIN);
					}
					break;
				case EventList.INVITETEAM:						//邀请入队，对外接口
					notiData = notification.getBody();
					if(TargetController.IsPKTeam())
					{
						var playerteam:GameElementAnimal = PlayerController.GetPlayer(int(notiData));
						if(playerteam != null)
						{
							if(GameCommonData.Player.Role.PKteam == playerteam.Role.PKteam)
							{
								sendData(TeamAction._MSGTEAM_INVITE);
							}
							else
							{
								UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_team_med_teamm_jjc_1"],0xffff00); // "必须同一阵营" 
							}
						}
						else
						{
							sendData(TeamAction._MSGTEAM_INVITE);
						}	
					}
					else
					{
						sendData(TeamAction._MSGTEAM_INVITE);
					}
					break;
				case TeamEvent.INVITETEAMBYNAME:				//邀请入队，给聊天的接口，用名字查询				
					notiData = notification.getBody();
					sendInviteFromChat();
					break;
				case EventList.LEAVETEAMCOMMON:					//离开队伍，对外的接口
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:3});
					sendData(TeamAction._MSGTEAM_LEAVE);
					teamModelManager.setModel(0);
					//2011. 1.4  客户端主动清除队伍信息
					teamDataProxy.teamMemberList = [];
					teamDataProxy.teamReqList = [];
					//
					sendNotification(PlayerInfoComList.MSG_LEAVE_TEAM);	//通知主界面 清除队伍坐标列表
					break;
				case EventList.KICKOUTTEAMCOMMON:				//踢出队伍，对外的接口
					notiData = notification.getBody();
					teamDataProxy.teamMemSelected.id = notiData.id;
					sendData(TeamAction._MSGTEAM_KICKOUT);
					teamModelManager.setModel(0);
					break;
				case EventList.CHANGELEADERTEAMCOMMON:			//移交队长，对外的接口
					notiData = notification.getBody();
					teamDataProxy.teamMemSelected.id = notiData.id;
					sendData(TeamAction._MSGTEAM_CHG_LEADER_APPLY);
					teamModelManager.setModel(0);
					break;
				case EventList.SETUPTEAMCOMMON:					//建立队伍，对外的接口
					sendData(TeamAction._MSGTEAM_CREATE);
					teamModelManager.setModel(0);
					break;
				case TeamEvent.MEMBERINVITESOMEONE:				//某个队员邀请了某人，此时自己是队长
					var inviteD:Object = notification.getBody();
					if(UIConstData.AUTO_ACCEPT_TEAM_INTITE_AND_APPLY) {	//挂机自动接受申请和邀请
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVITE, inviteD.idPlayer, inviteD.idTarget, inviteD.szPlayerName, inviteD.szTargetName);
					} else {
						for(var count:int = 0; count < inviteDataArr.length; count++) {
							if(inviteD.szPlayerName == inviteDataArr[count].szPlayerName) {
								return;
							}
						}
						inviteDataArr.push(inviteD);
						if(inviteDataArr.length == 1) memberInviteSomeone();
					}
					break;
				case TeamEvent.SHOWTEAMINFORMATION:				//显示组队相关的提示信息
					var infoObj:Object = notification.getBody();
					showTeamInformation(infoObj);
					break;
				case TeamEvent.SUPER_MAKE_TEAM:					//超级组队模式 (只发对方的ID给服务器，服务器判断，对方没队伍则邀请对方组队；对方有队伍且自己无队伍则申请加入对方队伍)
					var superId:uint = uint(notification.getBody());
					if(superId > 0) {
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_SUPER_MAKE_TEAM, superId);
					}
					break;
				case TeamEvent.SUPER_MAKE_TEAM_BY_NAME:
					var superName:String = String(notification.getBody());
					if(superName) {
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_SUPER_MAKE_TEAM, 0,0, superName);
					}
					break;
				case TeamEvent.LEAVE_TEAM_AFTER_CHANGE_LINE:		//换线成功，客户端自己清除组队数据
					changeLineClearData();
					break; 
				case EventList.MEMBER_ONLINE_STATUS_TEAM:			//更新队员在线状态     新功能
					if(UIConstData.AUTO_KICKOUT_LEAVE_MEMBER) {	//挂机 自动踢出离线队员
						var stateData:Object = notification.getBody();
						var state:uint = stateData.state;
						if(state == 0) {
							if(GameCommonData.Player.Role.idTeamLeader > 0 && GameCommonData.Player.Role.idTeamLeader == GameCommonData.Player.Role.Id) {
								TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_KICKOUT, stateData.id);
								return;
							}
						}
					}
					updateMemOnlineState(notification.getBody());
					break;
			}
		}
		
		/** 自动踢出离线玩家 */
		public function autoKickOutLevMem():void
		{
			if(GameCommonData.Player.Role.idTeamLeader > 0 && GameCommonData.Player.Role.idTeamLeader == GameCommonData.Player.Role.Id) {
				for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {
					var mem:Object = teamDataProxy.teamMemberList[i];
					if(mem.onlineStatus == 0) {
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_KICKOUT, mem.id);
					}
				}
			}
		}
		
		/** 更新队员 */
		private function updateMemOnlineState(info:Object):void
		{
			var idPlayer:int = info.id; 
			var state:int    = info.state;			//1-上线，0-离线
			for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {
				if(idPlayer == teamDataProxy.teamMemberList[i].id) {
					teamDataProxy.teamMemberList[i].onlineStatus = state;
					GameCommonData.TeamPlayerList[idPlayer]["state"] = state;
					var teamItem:TeamItem = teamDataProxy.teamMemItemList[i] as TeamItem;
					teamItem.setOnlineState(state);
					break;
				}
			}
//			//--------通知主界面
//			var teamDic:Dictionary = new Dictionary();
//			for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {
//				var oM:Object = teamDataProxy.teamMemberList[i];
//				var dicObj:Object = {};
//				dicObj.isCaption = (i == 0) ? true : false;
//				dicObj.id 		 = oM.id;
//				dicObj.state	 = (oM.id == idPlayer) ? state : oM.onlineStatus;
//				teamDic[oM.id] =  dicObj;
//			}
//			GameCommonData.TeamPlayerList = teamDic;
//			notiFan();
			//----------------------------------
		}
		
		private function sendInviteFromChat():void
		{
			var nameToInvite:String = notiData.name;
			if( teamDataProxy.teamMemberList[0] && teamDataProxy.teamMemberList[0].id && teamDataProxy.teamMemberList[0].id == GameCommonData.Player.Role.Id) {
//				trace("自己是队长");
				if(isMember(nameToInvite)) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_1" ], color:0xffff00});         //你们已经是队友
				} else if(isFullTeam()){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_2" ], color:0xffff00});              //队伍已满
				} else {
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVITE, 0, 0, nameToInvite ,"");
				}
			} else if(teamDataProxy.teamMemberList.length == 0) {
//				trace("自己没有队伍，发出邀请");
				TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVITE, 0, 0, nameToInvite ,"");
			} else {
//				trace("自己不是队长，发出邀请");
				if(isMember(nameToInvite)) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_1" ], color:0xffff00});             //你们已经是队友
				} else if(isFullTeam()){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_2" ], color:0xffff00});                //队伍已满
				} else {
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:9});
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_MEMMBER_APPLY, 0, 0, nameToInvite, "");
				}
			}
		}
		
		private function isMember(nameToInvite:String):Boolean
		{
			var isMember:Boolean = false;
			for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {
				if(teamDataProxy.teamMemberList[i].szName == nameToInvite) {
					isMember = true;
					break;
				}
			}
			return isMember;
		}
		
		private function isFullTeam():Boolean
		{
			if(teamDataProxy.teamMemberList && teamDataProxy.teamMemberList.length == 6) return true;
			return false;
		}
			
		
		/** 初始化显示组队界面 */
		private function initView():void
		{	
			teamPanel = new PanelBase(team, team.width+8, team.height+14);		//面板外壳
			teamPanel.name = "teamPanelBase";
			teamPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_team_med_teamm_ini_1" ]);         //组 队
			teamPanel.addEventListener(Event.CLOSE, onPanelCloseHandler);
			teamPanel.x = TEAMDEFAULTPOS.x;
			teamPanel.y = TEAMDEFAULTPOS.y
//			loadingMC = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("LoadingUIData");	//实例化加载MC
//			loadingMC.x = teamPanel.width/2;
//			loadingMC.y = teamPanel.height/2;
			initTeamItems();		//初始化队员列表组件
			initReqItems();			//初始化请求列表组件
		}
		
		/** 初始化队伍成员组件 */
		private function initTeamItems():void
		{
			for(var i:int = 0; i < 6; i++) {
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TeamItem");	//实例化队员列表组件
				var teamItem:TeamItem = new TeamItem(item,i);
				teamItem.name = "teamMemItem_"+i;
				teamItem.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
				teamItem.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
				teamItem.addEventListener(MouseEvent.CLICK, itemClickHanler);
//				item.height = 43;
				item.width = 163;
				var hei:int = item.height;
				team.addChild(teamItem);
				teamItem.x = TEAMITEM_STARTPOS.x;
				teamItem.y = TEAMITEM_STARTPOS.y + (i*hei) - (i*1);
				teamDataProxy.teamMemItemList.push(teamItem);
			}
		}
		
		/** 初始化请求列表组件 */
		private function initReqItems():void
		{	
			listView = new ListComponent(false);
			
			listView.Offset = 0;
			
			for(var i:int = 0; i < 5; i++) {
				addReqItem(i);
			}
			
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = TEAMREQLITEM_STARTPOS.x;
			iScrollPane.y = TEAMREQLITEM_STARTPOS.y;
			iScrollPane.width = 180;
			iScrollPane.height = 212;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			team.addChild(iScrollPane);
		}
		
		/** 添加请求列表组件 */
		private function addReqItem(index:int):void
		{
			var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TeamItem");	//实例化队员列表组件
			var teamItem:TeamItem = new TeamItem(item, index);
			teamItem.name = "teamReqItem_"+index;
			teamItem.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			teamItem.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			teamItem.addEventListener(MouseEvent.CLICK, itemClickHanler);
//			item.height = 43;
			item.width = 163.8;
			var hei:int = item.height;
			listView.SetChild(teamItem);
//			listView.upDataPos();
			teamItem.x = 0;
			teamItem.y = (index*hei) - (index*1);
			teamDataProxy.teamReqItemList.push(teamItem);
		}
		
		/** 移除申请列表组件的最后一个 */
		private function removeReqItem():void
		{
			teamDataProxy.teamReqItemList[teamDataProxy.teamReqItemList.length-1].removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			teamDataProxy.teamReqItemList[teamDataProxy.teamReqItemList.length-1].removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			teamDataProxy.teamReqItemList[teamDataProxy.teamReqItemList.length-1].removeEventListener(MouseEvent.CLICK, itemClickHanler);
			listView.removeChild(teamDataProxy.teamReqItemList[teamDataProxy.teamReqItemList.length-1]);
			listView.upDataPos();
			teamDataProxy.teamReqItemList.pop();
		}
		
		private function mouseOverHandler(e:MouseEvent):void
		{
			e.currentTarget.item.mcMouseOver.visible = true;
		}
		private function mouseOutHandler(e:MouseEvent):void
		{
			e.currentTarget.item.mcMouseOver.visible = false;
		}
		private function itemClickHanler(e:MouseEvent):void
		{
			var type:String = e.currentTarget.name.split("_")[0];
			var index:int   = int(e.currentTarget.name.split("_")[1]);
			if(type == "teamMemItem") { 		//队员列表
				removeAllYellowFrame(0);
				e.currentTarget.item.mcMouseDown.visible = true;
				teamDataProxy.teamMemSelected = teamDataProxy.teamMemberList[index];
			} else if(type == "teamReqItem") {	//请求列表
				removeAllYellowFrame(1);
				e.currentTarget.item.mcMouseDown.visible = true;
				teamDataProxy.teamReqSelected = teamDataProxy.teamReqList[index];
			}
		}
		
		/** 移除所有黄色点击框 0=队员列表，1=请求列表 */
		private function removeAllYellowFrame(type:int):void
		{
			if(type == 0) {
				for(var i:int = 0; i < teamDataProxy.teamMemItemList.length; i++) {
					var itemMem:TeamItem = teamDataProxy.teamMemItemList[i]
					itemMem.item.mcMouseDown.visible = false;
				}
			} else {
				for(var j:int = 0; j < teamDataProxy.teamReqItemList.length; j++) {
					var itemReq:TeamItem = teamDataProxy.teamReqItemList[j];
					itemReq.item.mcMouseDown.visible = false;
				}
			}
		}
		
		/** 显示组队界面 */
		private function showTeam():void
		{
			if(teamDataProxy.isInviting) {
				if(teamDataProxy.inviteIsOpen) {
					sendNotification(TeamEvent.REMOVEINVITE);
				}
//				var inviteMediator:TeamInviteMediator = new TeamInviteMediator();
				sendNotification(TeamEvent.INVITEINIT);
				return;
			}
			
//			if(!teamPanel.contains(loadingMC)) teamPanel.addChild(loadingMC);		//添加正在加载的MC
			teamModelManager.setModel(0);
//			clearData(0);
			clearData(1);
			if( GameCommonData.fullScreen == 2 )
			{
				teamPanel.x = 450 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				teamPanel.y = 58 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				teamPanel.x = 450;
				teamPanel.y = 58;
			}
			GameCommonData.GameInstance.GameUI.addChild(teamPanel);
//			teamPanel.x = TEAMDEFAULTPOS.x;
//			teamPanel.y = TEAMDEFAULTPOS.y
			addLis();																//监听事件
			dataProxy.TeamIsOpen = true;
			sendData(TeamAction._MSGTEAM_ASKINFO);
		}
		
		/** 收到服务器数据，更新界面 */
		private function updateView():void
		{
//			if(loadingMC && teamPanel.contains(loadingMC)) {
//				teamPanel.removeChild(loadingMC);
//			}
			var teamMemArr:Array = teamDataProxy.teamMemberList;
			var teamReqArr:Array = teamDataProxy.teamReqList;
			if(teamMemArr && teamMemArr.length > 0) 			//队中有人
			{
				if(teamMemArr[0].id == GameCommonData.Player.Role.Id)
				{
					teamModelManager.setModel(1);				//队长模式
				} else {										
					teamModelManager.setModel(2);				//队员模式
				}
			} else {
				teamModelManager.setModel(4);					//空队模式				
			}
		}
		
		/** 移除组件UI并垃圾回收 */
		private function gc():void
		{
			clearData(1);
			removeLis();
//			if(teamPanel.contains(loadingMC)) teamPanel.removeChild(loadingMC);
			if(GameCommonData.GameInstance.GameUI.contains(teamPanel)) GameCommonData.GameInstance.GameUI.removeChild(teamPanel);
//			teamDataProxy.teamMemberList = [];
//			teamDataProxy.teamReqList = [];
			teamDataProxy.teamMemSelected = new Object();
			teamDataProxy.teamReqSelected = new Object();
			removeAllYellowFrame(0);
			removeAllYellowFrame(1);
			joinPlayerName = "";
			dataProxy.TeamIsOpen = false;
		}
		
		/** 开始事件监听 */
		private function addLis():void
		{
			team.btnAddFriend.addEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnCreateTeam.addEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnRemoveMember.addEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnGiveCaptain.addEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnLeaveTeam.addEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnAcceptJoin.addEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnRefuseJoin.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		/** 移除事件监听 */
		private function removeLis():void 
		{
			team.btnAddFriend.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnRemoveMember.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnGiveCaptain.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnLeaveTeam.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnAcceptJoin.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			team.btnRefuseJoin.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		/** 关闭父级面板(整个UI关闭) */
		private function onPanelCloseHandler(e:Event):void
		{
			gc();
		}
		
		/** 点击按钮 加为好友、移除队员、转交队长、建立队伍、退出队伍、同意加入、拒绝加入、同意邀请、拒绝邀请 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btnAddFriend":																	//添加好友
					if(isCorrectChose(0)) {
						var userName:String = teamDataProxy.teamMemSelected.szName;
						sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:-1, name:userName});
					}
					break;
				case "btnRemoveMember":																	//移除队员
					if(isCorrectChose(1)) {
						sendData(TeamAction._MSGTEAM_KICKOUT);
						teamModelManager.setModel(0);
					}
					break;
				case "btnGiveCaptain":																	//转交队长
					if(isCorrectChose(2)) {
						sendData(TeamAction._MSGTEAM_CHG_LEADER_APPLY);
						teamModelManager.setModel(0);
					}
					break;
				case "btnCreateTeam":																	//建立队伍
					sendData(TeamAction._MSGTEAM_CREATE);
					teamModelManager.setModel(0);
					break;
				case "btnLeaveTeam":																	//退出队伍
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:3});
					sendData(TeamAction._MSGTEAM_LEAVE);
					teamDataProxy.teamMemberList = [];
					teamDataProxy.teamReqList = [];
					sendNotification(EventList.HASTEAM, false);
					sendNotification(PlayerInfoComList.MSG_LEAVE_TEAM);	//通知主界面 清除队伍坐标列表
					gc();
					break;
				case "btnAcceptJoin":																	//同意申请
					if(isCorrectChose(3)) {
						sendData(TeamAction._MSGTEAM_AGREEJOIN);
						teamModelManager.setModel(0);
						sendData(TeamAction._MSGTEAM_ASKINFO);
					}
					break;
				case "btnRefuseJoin":																	//拒绝申请
					if( isCorrectRefuse() ) { 
						sendData(TeamAction._MSGTEAM_DENY_APPLY);
						teamModelManager.setModel(0);
						sendData(TeamAction._MSGTEAM_ASKINFO);
					}
					break;
				case "btnAcceptInvite":																	//同意邀请
					sendData(TeamAction._MSGTEAM_ACCEPTINVITE);
					teamDataProxy.isInviting = false;
					teamModelManager.setModel(0);
					break;
				case "btnRefuseInvite":																	//拒绝邀请
					sendData(TeamAction._MSGTEAM_DENY_INVITE);
					teamDataProxy.isInviting = false;
					break;
			}
		}
		
		/** 发命令到Sever */
		private function sendData(action:int):void
		{
			switch(action) {
				case TeamAction._MSGTEAM_CREATE:																//建立队伍
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_CREATE, GameCommonData.Player.Role.Id);
					break;
				case TeamAction._MSGTEAM_ASKINFO:															//查询队伍详细信息
					if(GameCommonData.Player && GameCommonData.Player.Role && GameCommonData.Player.Role.Id) TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, GameCommonData.Player.Role.Id);
					break;
				case TeamAction._MSGTEAM_KICKOUT:																//移除队员
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_KICKOUT, teamDataProxy.teamMemSelected.id);
					break;
				case TeamAction._MSGTEAM_CHG_LEADER_APPLY:														//转交队长
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_CHG_LEADER_APPLY, teamDataProxy.teamMemSelected.id);
					break;
				case TeamAction._MSGTEAM_LEAVE:																	//退出队伍
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_LEAVE, GameCommonData.Player.Role.Id);
					if(teamDataProxy.teamMemberList[0] && teamDataProxy.teamMemberList[0].id == GameCommonData.Player.Role.Id) {
						GameCommonData.Player.SetTeamLeader(false);
					} else {
						GameCommonData.Player.SetTeam(false);
					}
					if(GameCommonData.Player && GameCommonData.Player.Role && GameCommonData.Player.Role.Id) TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, GameCommonData.Player.Role.Id);
					dataProxy.isTeamLeader = false;
					dataProxy.isTeamMember = false;
					//----------- 11.13
					GameCommonData.TeamPlayerList = new Dictionary();
					notiFan();
					break;
				case TeamAction._MSGTEAM_AGREEJOIN:																//同意申请
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_AGREEJOIN, teamDataProxy.teamReqSelected.id);
					break;
				case TeamAction._MSGTEAM_DENY_APPLY:															//拒绝申请
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:17});
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_DENY_APPLY, teamDataProxy.teamReqSelected.id);
					break;
				case TeamAction._MSGTEAM_ACCEPTINVITE:															//同意邀请
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ACCEPTINVITE, teamDataProxy.invitePerson.idPlayer, 0, GameCommonData.Player.Role.Name);
					break;
				case TeamAction._MSGTEAM_DENY_INVITE:															//拒绝邀请
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_DENY_INVITE, teamDataProxy.invitePerson.idPlayer, 0, teamDataProxy.invitePerson.szPlayerName);
					break;
				case TeamAction._MSGTEAM_APPLYJOIN:																//申请加入
					sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:15});
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_APPLYJOIN, notiData.id);
					break;
				case TeamAction._MSGTEAM_INVITE:																//邀请加入
					var idToInvite:int = notiData.id;
					if( (teamDataProxy.teamMemberList[0] && teamDataProxy.teamMemberList[0].id && teamDataProxy.teamMemberList[0].id == GameCommonData.Player.Role.Id)) {
//						trace("自己是队长");
						if(isMemberById(idToInvite)) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_1" ], color:0xffff00});            //你们已经是队友
						} else if(isFullTeam()) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_2" ], color:0xffff00});              //队伍已满
						} else {
							TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVITE, idToInvite, 0, "", "");
						}
					} else if(teamDataProxy.teamMemberList.length == 0){
//						trace("没有队伍，发出邀请");
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVITE, idToInvite, 0, "", "");
					} else {
//						trace("自己不是队长，发出邀请");
						var idToInvite1:int = notiData.id;
						if(isMemberById(idToInvite1)) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_1" ], color:0xffff00});               //你们已经是队友
						} else if(isFullTeam()) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_2" ], color:0xffff00});               //队伍已满
						} else {
							sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:9});
							TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_MEMMBER_APPLY, idToInvite1, 0, "", "");
						}
					}
					break;
				default:
					
					break;
			}
		}
		
		private function isMemberById(id:int):Boolean
		{
			var result:Boolean = false;
			for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {
				if(id == teamDataProxy.teamMemberList[i].id) {
					result = true;
					break;
				}
			}
			return result;
		}
		
		/** 选择状态是否正确 */
		public function isCorrectChose(type:int):Boolean
		{
			var result:Boolean = false;
			var teamMemSelected:Object = teamDataProxy.teamMemSelected;
			var teamReqSelected:Object = teamDataProxy.teamReqSelected;
			switch(type) {
				case 0:		//添加好友
					if(!teamMemSelected || !teamMemSelected.id) {
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:19});
					} else if(teamMemSelected.id == GameCommonData.Player.Role.Id){
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:20});
					} else {
						result = true;
					}
					break;
				case 1:		//移除队员
					if(!teamMemSelected || !teamMemSelected.id) {
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:19});
					} else if(teamMemSelected.id == GameCommonData.Player.Role.Id){
						facade.sendNotification(EventList.SHOWALERT, {comfrim:applyLeave, cancel:cancelClose, info: GameCommonData.wordDic[ "mod_team_med_teamm_isc_1" ], title:GameCommonData.wordDic[ "mod_team_med_teamm_isc_2" ]});  //你是否要退出队伍？     提 示
					} else {
						result = true;
					}
					
					break;
				case 2:		//转交队长
					if(!teamMemSelected || !teamMemSelected.id) {
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:19});
					} else if(teamMemSelected.id == GameCommonData.Player.Role.Id){
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:6});
					} else {
						result = true;
					}
					break;
				case 3:		//同意，拒绝 申请
					if(!teamReqSelected || !teamReqSelected.id) {
						sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:19});
					} else if(teamDataProxy.teamMemberList.length == 6) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sen_2" ], color:0xffff00});       //队伍已满
					} else {
						result =  true;
					}
					break;
			}
			return result;
		} 
		
		//点击拒绝组队时的判断
		private function isCorrectRefuse():Boolean
		{
			var result:Boolean = false;
			var teamReqSelected:Object = teamDataProxy.teamReqSelected;
			if(!teamReqSelected || !teamReqSelected.id)
			{
				sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:19});
			} 
			else 
			{
				result =  true;
			}
			return result;
		}
		
		/** 初始化面板时删除组件上的原有内容 0=成员列表，1=申请列表 */
		public function clearData(type:int):void
		{
			if(type == 0) {
				for(var i:int = 0; i < teamDataProxy.teamMemItemList.length; i++) {
					var teamItem:TeamItem = teamDataProxy.teamMemItemList[i] as TeamItem;
	//				teamItem.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
	//				teamItem.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
					teamItem.removeEventListener(MouseEvent.CLICK, itemClickHanler);
					teamItem.setPhoto(null);
					teamItem.setTxtByName("txtMemberName");
					teamItem.setTxtByName("txtRoleLevel");
					teamItem.setTxtByName("txtBusiness");
					teamItem.setTxtByName("txtBusinessLevel");
				}
				removeAllYellowFrame(0);
			} else {
				var reqDataLen:int = teamDataProxy.teamReqList.length;
				//增删请求列表组件
				if(teamDataProxy.teamReqItemList.length < reqDataLen) {
					while(teamDataProxy.teamReqItemList.length < reqDataLen) {
						addReqItem(teamDataProxy.teamReqItemList.length);
					}
				} else {
					while(teamDataProxy.teamReqItemList.length > reqDataLen) {
						if(teamDataProxy.teamReqItemList.length == 5) break;
						removeReqItem();
					}
				}
				
				for(var j:int = 0; j < teamDataProxy.teamReqItemList.length; j++) {
					var teamItemR:TeamItem = teamDataProxy.teamReqItemList[j] as TeamItem;
	//				teamItemR.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
	//				teamItemR.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
					teamItemR.removeEventListener(MouseEvent.CLICK, itemClickHanler);
					teamItemR.setPhoto(null);
					teamItemR.setTxtByName("txtMemberName");
					teamItemR.setTxtByName("txtRoleLevel");
					teamItemR.setTxtByName("txtBusiness");
					teamItemR.setTxtByName("txtBusinessLevel");
				}
				removeAllYellowFrame(1);
				iScrollPane.refresh();
			}
		}
		
		/** 更新数据 0=成员列表，1=申请列表 */
		public function updateData(type:int):void
		{
			if(type == 0) {
				
				var teamDic:Dictionary = new Dictionary(false);
				
				for(var i:int = 0; i < teamDataProxy.teamMemberList.length; i++) {
					var teamItem:TeamItem = teamDataProxy.teamMemItemList[i] as TeamItem;
					var oM:Object = teamDataProxy.teamMemberList[i];
					i == 0 ? teamDic[oM.id] ={isCaption:true, id:oM.id, state:oM.onlineStatus} : teamDic[oM.id] ={isCaption:false, id:oM.id, state:oM.onlineStatus};  //   新功能
					var faceMem:FaceItem = new FaceItem(oM.dwLookFace.toString(), teamItem.item.mcMemberPhoto, "face", (32/50));
					faceMem.offsetPoint = new Point(0, 0);
					teamItem.setPhoto(faceMem);
					teamItem.setOnlineState(oM.onlineStatus);	//新功能
					teamItem.setTxtByName("txtMemberName", oM.szName);
					teamItem.setTxtByName("txtRoleLevel", oM.usLev+GameCommonData.wordDic[ "often_used_level" ]);          //级
					teamItem.setTxtByName("txtBusiness", dataProxy.RolesListDic[oM.usPro]);
					teamItem.setTxtByName("txtBusinessLevel", oM.usProLev+GameCommonData.wordDic[ "often_used_level" ]);         //级
					teamItem.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
					teamItem.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
					teamItem.addEventListener(MouseEvent.CLICK, itemClickHanler);
//					if(i == 0) {
//						teamItem.item.mcMouseDown.visible = true;
//						teamDataProxy.teamMemSelected = teamDataProxy.teamMemberList[i];
//					}
				}
				GameCommonData.TeamPlayerList = teamDic;
				notiFan();
			} else {
				for(var j:int = 0; j < teamDataProxy.teamReqList.length; j++) {
					var teamItemR:TeamItem = teamDataProxy.teamReqItemList[j] as TeamItem;
					var oR:Object = teamDataProxy.teamReqList[j];
					var faceReq:FaceItem = new FaceItem(oR.dwLookFace.toString(), teamItemR.item.mcMemberPhoto, "face", (32/50));
					teamItemR.setPhoto(faceReq);
					teamItemR.setTxtByName("txtMemberName", oR.szName);
					teamItemR.setTxtByName("txtRoleLevel", oR.usLev+GameCommonData.wordDic[ "often_used_level" ]);          //级
					teamItemR.setTxtByName("txtBusiness", dataProxy.RolesListDic[oR.usPro]);
					teamItemR.setTxtByName("txtBusinessLevel", oR.usProLev+GameCommonData.wordDic[ "often_used_level" ]);         //级
					teamItemR.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
					teamItemR.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
					teamItemR.addEventListener(MouseEvent.CLICK, itemClickHanler);
//					if(j == 0) {
//						teamItemR.item.mcMouseDown.visible = true;
//						teamDataProxy.teamReqSelected = teamDataProxy.teamReqList[j];
//					}
				}
				if( teamDataProxy.teamReqList && teamDataProxy.teamReqList.length > 0)    selectFirst();         //默认选中申请列表的第一个
			}
		}
		
		private function selectFirst():void
		{
			var itemReqFirst:TeamItem = teamDataProxy.teamReqItemList[0];
			itemReqFirst.item.mcMouseDown.visible = true;
			teamDataProxy.teamReqSelected = teamDataProxy.teamReqList[0];
		}
		
		private function notiFan():void
		{
			UIFacade.GetInstance(UIFacade.FACADEKEY).upDateInfo(1);
		}
		
		/** 某队员邀请某人加入队伍 */
		private function memberInviteSomeone():void
		{
			var nameFrom:String = inviteDataArr[0].szTargetName;
			var nameTo:String = inviteDataArr[0].szPlayerName;
			
			//邀请       加入队伍，是否接受？        提 示        接 受        拒 绝
			facade.sendNotification(EventList.SHOWALERT, {comfrim:applyInvite, cancel:refuseInvite, isShowClose:false, info: "【"+nameFrom+"】"+GameCommonData.wordDic[ "mod_team_med_teamm_mem_1" ]+"【"+nameTo+"】"+GameCommonData.wordDic[ "mod_team_med_teamm_mem_2" ], title:GameCommonData.wordDic[ "mod_team_med_teamm_isc_2" ], comfirmTxt:GameCommonData.wordDic[ "mod_team_med_teamm_mem_3" ], cancelTxt:GameCommonData.wordDic[ "mod_team_med_teamm_mem_4" ]});
		}
		/** 同意 某队员邀请某人加入队伍 */
		private function applyInvite():void
		{
			var inviteD:Object = inviteDataArr.shift();
			TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVITE, inviteD.idPlayer, inviteD.idTarget, inviteD.szPlayerName, inviteD.szTargetName);
			if(inviteDataArr.length > 0) memberInviteSomeone();
		}
		/** 拒绝 某队员邀请某人加入队伍 */
		private function refuseInvite():void
		{
			var inviteD:Object = inviteDataArr.shift();
			TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_REFUSE_INVITE_SOMEONT, inviteD.idPlayer, inviteD.idTarget);
			if(inviteDataArr.length > 0) memberInviteSomeone();
		}
		/** 队长确定移除自己（离开队伍） */
		private function applyLeave():void
		{
			sendNotification(TeamEvent.SHOWTEAMINFORMATION, {type:3});
			teamDataProxy.teamMemberList = [];
			teamDataProxy.teamReqList = [];
			gc();
			sendNotification(EventList.HASTEAM, false);
			sendNotification(PlayerInfoComList.MSG_LEAVE_TEAM);	//通知主界面 清除队伍坐标列表
			sendData(TeamAction._MSGTEAM_LEAVE);
		}
		/** 询问框 取消操作 */
		private function cancelClose():void
		{
			
		}
		
		/** 显示组队相关的信息提示文字 */
		private function showTeamInformation(infoObj:Object):void
		{
			switch(infoObj.type) {
				case 1:																											//建立队伍
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_1" ], color:0xffff00});          //你建立了一个队伍
					break;
				case 2:																											//某人加入了队伍
//					trace(joinPlayerName);
					if(joinPlayerName == GameCommonData.Player.Role.Name) {
						var leaderName:String = teamDataProxy.teamMemberList[0].szName;
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_2" ]+"<font color='#ff0000'>["+leaderName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_3" ], color:0xffff00});  //你加入了     的队伍
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+joinPlayerName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_4" ], color:0xffff00});  //加入了队伍
					}
					joinPlayerName = "";
					break;
				case 3:																											//自己退出队伍
					if(teamDataProxy.teamMemberList[0]) {
						var leaderName1:String = teamDataProxy.teamMemberList[0].szName;
						if(leaderName1 == GameCommonData.Player.Role.Name) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_5" ], color:0xffff00});          //你退出了队伍
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_6" ]+"<font color='#ff0000'>["+leaderName1+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_3" ], color:0xffff00});     //你退出了         的队伍 
						}
					}
					break;
				case 4:																											//某人退出了队伍
					var leavePlayerName:String = infoObj.data.szPlayerName;
					if(leavePlayerName == GameCommonData.Player.Role.Name) return;
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+leavePlayerName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_7" ], color:0xffff00});       //退出了队伍
					break;
				case 5:																											//队长变更了
					if(teamDataProxy.teamMemberList.length == 0) return;
					var newLeaderName:String = infoObj.data.szPlayerName;
					if(newLeaderName == GameCommonData.Player.Role.Name) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_8" ], color:0xffff00});          //你被提升为队长
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+newLeaderName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_9" ], color:0xffff00});          //被提升为队长
					}
					break;
				case 6:																											//自己已经是队长了
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_10" ], color:0xffff00});            //你已经是队长
					break;
				case 7:																											//某玩家被踢出队伍
					var beiJuName:String = infoObj.data.szPlayerName;
					var bossName:String  = teamDataProxy.teamMemberList[0].szName;
					if(bossName == GameCommonData.Player.Role.Name) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_11" ]+"<font color='#ff0000'>["+beiJuName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_12" ], color:0xffff00});    //你将        请离了队伍
						return;
					}
					if(beiJuName == GameCommonData.Player.Role.Name) {
						GameCommonData.Player.SetTeam(false);
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_13" ]+"<font color='#ff0000'>["+teamDataProxy.teamMemberList[0].szName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_3" ], color:0xffff00});        //你被请离了         的队伍
						clearTeamLocal();
					}
					break;
				case 8:
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你的组队邀请已发出", color:0xffff00});
					break;
				case 9:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_14" ], color:0xffff00});        //你的组队邀请已发出，需要队长确认
					break;
				case 10:
					var inviteLeaderName:String = infoObj.data.szPlayerName;
					var inviterName:String = infoObj.data.szTargetName;
					if(inviterName) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+inviterName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_15" ]+"<font color='#ff0000'>["+inviteLeaderName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_3" ], color:0xffff00});    //邀请你加入           的队伍
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+inviteLeaderName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_16" ], color:0xffff00});        //邀请你加入队伍
					}
					break;
				case 11:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_17" ], color:0xffff00});         //加入队伍失败
					break;
				case 12:
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该队伍已满，加入队伍失败", color:0xffff00});
					break;
				case 13:
					var denyInviter:String = infoObj.data.szPlayerName;
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+denyInviter+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_18" ], color:0xffff00});          //拒绝了你的组队邀请
					break;
				case 14:
					var inTeamLeader:String = teamDataProxy.teamMemberList[0].szName;
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_19" ]+"<font color='#ff0000'>["+inTeamLeader+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_20" ], color:0xffff00});     //你已经在            的队伍中，无法加入其他队伍
					break;
				case 15:
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你的组队申请已发出", color:0xffff00});
					break;
				case 16:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_21" ], color:0xffff00});        //你收到一个组队申请
					break;
				case 17:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_22" ]+"<font color='#ff0000'>["+teamDataProxy.teamReqSelected.szName+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_23" ], color:0xffff00});        //你拒绝了          的组队申请
					break;
				case 18:
					var denyApplyLeader:String = infoObj.data.szPlayerName;
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+denyApplyLeader+"]</font>"+GameCommonData.wordDic[ "mod_team_med_teamm_sho_24" ], color:0xffff00});          //拒绝了你的组队申请
					break;
				case 19:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_25" ], color:0xffff00});             //请先选择玩家
					break;
				case 20:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_26" ], color:0xffff00});            //不能添加自己为好友
					break;
				case 21:
					
					break;
			}
		}
		
		/** 本地清除组队，用于自己被踢出队伍 */
		private function clearTeamLocal():void
		{
			var obj:Object = {};
			obj.teamMemList = [];
			obj.teamReqList = [];
			sendNotification(EventList.UPDATETEAM, obj);	//更新组队界面
			sendNotification(PlayerInfoComList.MSG_LEAVE_TEAM);	//通知主界面 清除队伍坐标列表
			notiFan();
		}
		
		/** 新的队员列表和原有的相比是否有变化 true-有变化，false-没变化 */
		private function haveChange(newArr:Array):Boolean
		{
			var result:Boolean = false;
			var temlList:Array = teamDataProxy.teamMemberList;
			if(temlList.length != newArr.length || newArr.length == 0) {
				result = true;
			} else {
				for(var i:int = 0; i < newArr.length; i++) {
					var newObj:Object = newArr[i];
					var oldObj:Object = temlList[i];
					if( (newObj.id != oldObj.id) || (newObj.usLev != oldObj.usLev) || (newObj.onlineStatus != oldObj.onlineStatus) ) {   //   新功能 
						result = true;
						break;
					}
				}
			}
			return result;
		}
		
		/** 换线后清除组队数据 */
		private function changeLineClearData():void
		{
			GameCommonData.Player.SetTeam(false);
			GameCommonData.Player.SetTeamLeader(false); 
			GameCommonData.Player.Role.idTeam = 0;
			GameCommonData.Player.Role.idTeamLeader = 0;
			sendNotification(PlayerInfoComList.MSG_LEAVE_TEAM);	//通知主界面 清除队伍坐标列表
			sendData(TeamAction._MSGTEAM_ASKINFO); 
		}
		
	}
}
