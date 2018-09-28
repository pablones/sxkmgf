package GameUI.Modules.Team.Mediator
{
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	import GameUI.Modules.Team.Datas.TeamEvent;
	import GameUI.Modules.Team.Datas.TeamNetAction;
	import GameUI.Modules.Team.UI.TeamItem;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionProcessor.TeamAction;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TeamInviteMediator extends Mediator
	{
		public static const NAME:String = "TeamInviteMediator";
		public static const TEAMINVITE_POS:Point = new Point(600, 95);
		public static const ITEMSTART_POS:Point  = new Point(8.5, 30);
		
		private var teamDataProxy:TeamDataProxy = null;
		private var invitePanel:PanelBase = null;
		private var dataProxy:DataProxy = null;
		
		public function TeamInviteMediator()
		{
			super(NAME);
		}
		
		private function get teamInvite():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				TeamEvent.SHOWINVITE,			//显示邀请界面
				TeamEvent.SHOWINVITETEAMINFO,	//显示邀请的队伍信息
				TeamEvent.REMOVEINVITE,			//移除邀请界面
				TeamEvent.INVITEINIT,			//打开面板
				TeamEvent.INIT_INVITE_PANEL		//初始化邀请面板 (8.26)
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case TeamEvent.INIT_INVITE_PANEL:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					teamDataProxy = facade.retrieveProxy(TeamDataProxy.NAME) as TeamDataProxy;
					initPanel();
					break;
				case TeamEvent.INVITEINIT:
//					if(invitePanel && GameCommonData.GameInstance.GameUI.contains(invitePanel)) {
//						GameCommonData.GameInstance.GameUI.removeChild(invitePanel);
//					}
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_INVEITE_INFO, GameCommonData.Player.Role.Id, 0, "");
//					} else {
//						teamDataProxy = facade.retrieveProxy(TeamDataProxy.NAME) as TeamDataProxy;
//						dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
//					}
//					if(GameCommonData.Player.Role && GameCommonData.Player.Role.Id) {
//					}
					break;
				case TeamEvent.SHOWINVITE:
					var teamIds:Object = notification.getBody();
					teamDataProxy.inviteIdList = [];					//邀请ID列表
					for(var i:int = 0; i < teamIds.teamMemList.length; i++) {
						teamDataProxy.inviteIdList.push(teamIds.teamMemList[i].id);
					}
					teamDataProxy.invitePageCount = teamDataProxy.inviteIdList.length;
					if(teamDataProxy.invitePageCount == 0) {	//空列表
						if(teamDataProxy.isInviting) {
							teamDataProxy.isInviting = false;
						}
						gc();
						return;
					}
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
					initView();
					addLis();
					teamDataProxy.inviteIsOpen = true;
					break;
				case TeamEvent.REMOVEINVITE:
					gc();
					break;
				case TeamEvent.SHOWINVITETEAMINFO:
					var teamInfo:Object = notification.getBody();
					teamDataProxy.inviteMemList = teamInfo.teamMemList;
//					trace("显示邀请队伍信息");
					clearData();
					if(teamDataProxy.inviteMemList.length == 0) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teami_han_1" ], color:0xffff00});        //该邀请信息不存在
						if(teamDataProxy.invitePageCount == 1) {//邀请数据不存在，直接gc
							teamDataProxy.isInviting = false;
							gc();
						} else {								//还有其他邀请信息，继续查询其他邀请信息
							teamDataProxy.inviteIdList.splice(teamDataProxy.curInvitePage, 1);
							teamDataProxy.invitePageCount --;
							teamDataProxy.curInvitePage = 0;
							updatePage();
							lockBtns(false);
							TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
						}
						return;
					}
					updateData();
					lockBtns(true);
					break;
			}
		}
		
		/** 初始化面板 */
		private function initPanel():void
		{
			var teamInvite:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TeamInvite");	//实例化队员列表组件
			viewComponent = teamInvite;
			invitePanel = new PanelBase(teamInvite, teamInvite.width+8, teamInvite.height+14);
			invitePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_team_med_teami_ini_1" ]);         //邀请入队
			invitePanel.addEventListener(Event.CLOSE, onPanelCloseHandler);
			invitePanel.x = TEAMINVITE_POS.x;
			invitePanel.y = TEAMINVITE_POS.y
			
			addItems();
			clearData();
		}
		
		private function addItems():void
		{
			for(var i:int = 0; i < 6; i++) {
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TeamItem");	//实例化队员列表组件
				var teamItem:TeamItem = new TeamItem(item,i);
				teamItem.name = "teamMemInvite_"+i;
				teamItem.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
				teamItem.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
				item.width = 163.5;
				var hei:int = item.height;
				teamInvite.addChild(teamItem);
				teamItem.x = ITEMSTART_POS.x;
				teamItem.y = ITEMSTART_POS.y + (i*hei) - (i*1);
				teamDataProxy.inviteMemItemList.push(teamItem);
			}
		}
		
		private function initView():void
		{
//			var teamInvite:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TeamInvite");	//实例化队员列表组件
//			viewComponent = teamInvite;
//			
//			invitePanel = new PanelBase(teamInvite, teamInvite.width+8, teamInvite.height+14);
//			invitePanel.SetTitleTxt("邀请入队");
//			invitePanel.addEventListener(Event.CLOSE, onPanelCloseHandler);
//			invitePanel.x = TEAMINVITE_POS.x;
//			invitePanel.y = TEAMINVITE_POS.y
//			
//			addItems();
//			clearData();
			
			GameCommonData.GameInstance.GameUI.addChild(invitePanel);
			
			updatePage();
		}
		
		private function updatePage():void
		{
			if(teamDataProxy.curInvitePage == 0) {
				teamInvite.btnFrontPage.visible = false;
			} else {
				teamInvite.btnFrontPage.visible = true;
			}
			if(teamDataProxy.invitePageCount == teamDataProxy.curInvitePage + 1) {
				teamInvite.btnNextPage.visible = false;
			} else {
				teamInvite.btnNextPage.visible = true;
			}
		}
		
		
		private function mouseOverHandler(e:MouseEvent):void
		{
			e.currentTarget.item.mcMouseOver.visible = true;
		}
		private function mouseOutHandler(e:MouseEvent):void
		{
			e.currentTarget.item.mcMouseOver.visible = false;
		}
		
		/** 点击按钮 上一页、下一页、同意、拒绝 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btnFrontPage":													//前一页
					teamDataProxy.curInvitePage --;
					updatePage();
					lockBtns(false);
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
					break;
				case "btnNextPage":														//下一页
					teamDataProxy.curInvitePage	++;
					updatePage();
					lockBtns(false);
					TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
					break;
				case "btnAcceptInvite":													//接受邀请
					if(teamDataProxy.isInviting && teamDataProxy.inviteIdList.length > 0) {
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ACCEPTINVITE, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
						gc();
						sendNotification(TeamEvent.INVITEINIT);
					}
					break;
				case "btnRefuseInvite":													//拒绝邀请
					if(teamDataProxy.isInviting && teamDataProxy.inviteIdList.length > 0) {
						TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_DENY_INVITE, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
						teamDataProxy.inviteIdList.splice(teamDataProxy.curInvitePage, 1);
						teamDataProxy.invitePageCount --;
						teamDataProxy.curInvitePage = 0;
						if(teamDataProxy.invitePageCount == 0) {
							teamDataProxy.isInviting = false;
							gc();
						} else {
							updatePage();
							lockBtns(false);
							TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, teamDataProxy.inviteIdList[teamDataProxy.curInvitePage], 0, "");
						}
					}
					break;
			}
		}
		
		private function gc():void
		{
			removeLis();
			clearData();
			teamDataProxy.inviteMemList 	= [];
			teamDataProxy.inviteIdList 		= [];
			teamDataProxy.curInvitePage 	= 0;
			teamDataProxy.invitePageCount 	= 0;
			if(GameCommonData.GameInstance.GameUI.contains(invitePanel)) GameCommonData.GameInstance.GameUI.removeChild(invitePanel);
			teamDataProxy.inviteIsOpen = false;
//			teamDataProxy.inviteMemItemList = [];
//			teamDataProxy = null;
//			dataProxy 	  = null;
//			invitePanel   = null;
//			facade.removeMediator(NAME);
		}
		
		private function addLis():void
		{
			teamInvite.btnFrontPage.addEventListener(MouseEvent.CLICK, btnClickHandler);
			teamInvite.btnNextPage.addEventListener(MouseEvent.CLICK, btnClickHandler);
			teamInvite.btnAcceptInvite.addEventListener(MouseEvent.CLICK, btnClickHandler);
			teamInvite.btnRefuseInvite.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		private function removeLis():void
		{
			teamInvite.btnFrontPage.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			teamInvite.btnNextPage.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			teamInvite.btnAcceptInvite.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			teamInvite.btnRefuseInvite.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function clearData():void
		{
			for(var i:int = 0; i < teamDataProxy.inviteMemItemList.length; i++) {
				var teamItem:TeamItem = teamDataProxy.inviteMemItemList[i] as TeamItem;
				teamItem.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
				teamItem.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
				teamItem.setPhoto(null);
				teamItem.setTxtByName("txtMemberName");
				teamItem.setTxtByName("txtRoleLevel");
				teamItem.setTxtByName("txtBusiness");
				teamItem.setTxtByName("txtBusinessLevel");
			}
		}
		
		private function updateData():void
		{
			for(var i:int = 0; i < teamDataProxy.inviteMemList.length; i++) {
				var teamItem:TeamItem = teamDataProxy.inviteMemItemList[i] as TeamItem;
				var oM:Object = teamDataProxy.inviteMemList[i];
				var faceMem:FaceItem = new FaceItem(oM.dwLookFace.toString(), teamItem.item.mcMemberPhoto, "face", (32/50));
				teamItem.setPhoto(faceMem);
				teamItem.setTxtByName("txtMemberName", oM.szName);						//"#e9ca9b"
				teamItem.setTxtByName("txtRoleLevel", oM.usLev+GameCommonData.wordDic[ "often_used_level" ]);	//级				//"#9baae9"
				teamItem.setTxtByName("txtBusiness", dataProxy.RolesListDic[oM.usPro]);	//"#ffb400"
				teamItem.setTxtByName("txtBusinessLevel", oM.usProLev+GameCommonData.wordDic[ "often_used_level" ]);		//级		//"#9baae9"
				teamItem.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
				teamItem.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			}
		}
		
		private function lockBtns(val:Boolean):void
		{
			teamInvite.btnFrontPage.mouseEnabled 	  = val;
			teamInvite.btnNextPage.mouseEnabled 	  = val;
			teamInvite.btnAcceptInvite.mouseEnabled   = val;
			teamInvite.btnRefuseInvite.mouseEnabled   = val;
		}
		
		private function onPanelCloseHandler(e:Event):void
		{
			gc();
		}
		
	}
}
