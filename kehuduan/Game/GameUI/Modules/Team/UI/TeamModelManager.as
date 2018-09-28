package GameUI.Modules.Team.UI
{
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	
	import flash.display.MovieClip;
	
	/** 组队UI模型管理 */
	public class TeamModelManager
	{
		private var team:MovieClip = null;
		private var teamDataProxy:TeamDataProxy;
		
		public function TeamModelManager(team:MovieClip, teamDataProxy:TeamDataProxy)
		{
			this.team = team;
			this.teamDataProxy = teamDataProxy;
			init();
		}
		
		private function init():void
		{
			team.txtAddFriend.mouseEnabled = false;		//添加好友 文字   不可用
			team.txtRemoveMember.mouseEnabled = false;	//移除队员 文字
			team.txtGiveCaptain.mouseEnabled = false;	//转交队长 文字
			team.txtCreateTeam.mouseEnabled = false;	//建立队伍 文字
			team.txtLeaveTeam.mouseEnabled = false;		//退出队伍 文字
			team.txtAcceptJoin.mouseEnabled = false;	//同意加入 文字
			team.txtRefuseJoin.mouseEnabled = false;	//拒绝加入 文字
			team.txtOperaInfo.mouseEnabled = false;		//操作提示 文字
			team.txtAcceptJoin.visible = false;			//同意加入 文字
			team.txtRefuseJoin.visible = false;			//拒绝加入 文字
		}
		/** 设置模式 */
		public function setModel(model:int):void
		{
			if(!this.team) return;
			switch(model) 
			{
				case 0:
					modelDefault();		//默认模式
					break;
				case 1:
					modelIsLeader();	//自己是队长
					break;
				case 2:
					modelIsMember();	//自己是队员
					break;
				case 3:
					modelHaveInvite();	//自己受到邀请
					break;
				case 4:
					modelNoPerson();	//队中无人(尚无队伍)，可以建立队伍
					break;
				case 5:
					
					break;
				default:
					break;
			}
			
		}
		
		/** 默认模式 所有都开 */
		private function modelDefault():void
		{
			lockBtns(false);
			
			team.txtAddFriend.visible = true;			//添加好友 文字
			team.txtCreateTeam.visible = false;			//建立队伍 文字
			team.txtRemoveMember.visible = false;		//移除队员 文字
			team.txtGiveCaptain.visible = false;		//转交队长 文字
			team.txtLeaveTeam.visible = false;			//退出队伍 文字
			team.txtAcceptJoin.visible = false;			//同意加入 文字
			team.txtRefuseJoin.visible = false;			//拒绝加入 文字
			team.txtOperaInfo.text = "";				//操作提示 文字
//			
			team.btnAddFriend.visible = true;			//添加好友 按钮
			team.btnCreateTeam.visible = false;			//建立队伍 按钮
			team.btnRemoveMember.visible = false;		//移除队员 按钮
			team.btnGiveCaptain.visible = false;		//转交队长 按钮
			team.btnLeaveTeam.visible = false;			//退出队伍 按钮
			team.btnAcceptJoin.visible = false;			//同意加入 按钮
			team.btnRefuseJoin.visible = false;			//拒绝加入 按钮
		}
		
		/** 受到邀请 */
		private function modelHaveInvite():void
		{
			team.txtAddFriend.visible = true;			//添加好友 文字
			team.txtRemoveMember.visible = false;		//移除队员 文字
			team.txtGiveCaptain.visible = false;		//转交队长 文字
			team.txtCreateTeam.visible = false;			//建立队伍 文字
			team.txtLeaveTeam.visible = false;			//退出队伍 文字
			team.txtAcceptJoin.visible = false;			//同意加入 文字
			team.txtRefuseJoin.visible = false;			//拒绝加入 文字
			team.txtOperaInfo.text = "";				//操作提示 文字
			
			team.btnAddFriend.visible = true;			//添加好友 按钮
			team.btnRemoveMember.visible = false;		//移除队员 按钮
			team.btnGiveCaptain.visible = false;		//转交队长 按钮
			team.btnCreateTeam.visible = false;			//建立队伍 按钮
			team.btnLeaveTeam.visible = false;			//退出队伍 按钮
			team.btnAcceptJoin.visible = false;			//同意加入 按钮
			team.btnRefuseJoin.visible = false;			//拒绝加入 按钮
			team.txtOperaInfo.text = "";				//操作提示 文字
		}
		
		/** 自己是队长 */
		private function modelIsLeader(): void
		{
			lockBtns(true);
			var canMove:Boolean = (teamDataProxy.teamMemberList.length > 1) ? true : false;
			team.txtAddFriend.visible = true;			//添加好友 文字
			team.txtRemoveMember.visible = canMove;		//移除队员 文字
			team.txtGiveCaptain.visible = canMove;		//转交队长 文字
			team.txtCreateTeam.visible = false;			//建立队伍 文字
			team.txtLeaveTeam.visible = true;			//退出队伍 文字
			var canJoin:Boolean = (teamDataProxy.teamReqList.length > 0) ? true : false;
			team.txtAcceptJoin.visible = canJoin;		//同意加入 文字
			team.txtRefuseJoin.visible = canJoin;		//拒绝加入 文字
			team.txtOperaInfo.text = "";				//操作提示 文字
			
			team.btnAddFriend.visible = true;			//添加好友 按钮
			team.btnRemoveMember.visible = canMove;		//移除队员 按钮
			team.btnGiveCaptain.visible = canMove;		//转交队长 按钮
			team.btnCreateTeam.visible = false;			//建立队伍 按钮
			team.btnLeaveTeam.visible = true;			//退出队伍 按钮
			team.btnAcceptJoin.visible = canJoin;		//同意加入 按钮
			team.btnRefuseJoin.visible = canJoin;		//拒绝加入 按钮
		}
		
		/** 自己是队员 */
		private function modelIsMember():void
		{
			lockBtns(true);
			team.txtAddFriend.visible = true;			//添加好友 文字
			team.txtRemoveMember.visible = false;		//移除队员 文字
			team.txtGiveCaptain.visible = false;		//转交队长 文字
			team.txtCreateTeam.visible = false;			//建立队伍 文字
			team.txtLeaveTeam.visible = true;			//退出队伍 文字
			team.txtAcceptJoin.visible = false;			//同意加入 文字
			team.txtRefuseJoin.visible = false;			//拒绝加入 文字
			team.txtOperaInfo.text = "";				//操作提示 文字
			
			team.btnAddFriend.visible = true;			//添加好友 按钮
			team.btnRemoveMember.visible = false;		//移除队员 按钮
			team.btnGiveCaptain.visible = false;		//转交队长 按钮
			team.btnCreateTeam.visible = false;			//建立队伍 按钮
			team.btnLeaveTeam.visible = true;			//退出队伍 按钮
			team.btnAcceptJoin.visible = false;			//同意加入 按钮
			team.btnRefuseJoin.visible = false;			//拒绝加入 按钮
		}
		
		/** 队中无人(尚无队伍)，可以建立队伍 */
		private function modelNoPerson():void
		{
			lockBtns(true);
			team.txtAddFriend.visible = true;			//添加好友 文字
			team.txtRemoveMember.visible = false;		//移除队员 文字
			team.txtGiveCaptain.visible = false;		//转交队长 文字
			team.txtCreateTeam.visible = true;			//建立队伍 文字
			team.txtLeaveTeam.visible = false;			//退出队伍 文字
			team.txtAcceptJoin.visible = false;			//同意加入 文字
			team.txtRefuseJoin.visible = false;			//拒绝加入 文字
			team.txtOperaInfo.text = "";				//操作提示 文字
			
			team.btnAddFriend.visible = true;			//添加好友 按钮
			team.btnRemoveMember.visible = false;		//移除队员 按钮
			team.btnGiveCaptain.visible = false;		//转交队长 按钮
			team.btnCreateTeam.visible = true;			//建立队伍 按钮
			team.btnLeaveTeam.visible = false;			//退出队伍 按钮
			team.btnAcceptJoin.visible = false;			//同意加入 按钮
			team.btnRefuseJoin.visible = false;			//拒绝加入 按钮
		}
		
		/** 加锁按钮 */
		private function lockBtns(mouseEabled:Boolean):void
		{
			team.btnAddFriend.mouseEnabled = mouseEabled;			//添加好友 按钮
			team.btnRemoveMember.mouseEnabled = mouseEabled;		//移除队员 按钮
			team.btnGiveCaptain.mouseEnabled = mouseEabled;			//转交队长 按钮
			team.btnCreateTeam.mouseEnabled = mouseEabled;			//建立队伍 按钮
			team.btnLeaveTeam.mouseEnabled = mouseEabled;			//退出队伍 按钮
			team.btnAcceptJoin.mouseEnabled = mouseEabled;			//同意加入 按钮
			team.btnRefuseJoin.mouseEnabled = mouseEabled;			//拒绝加入 按钮
		}
		
	}
}
