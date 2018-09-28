package GameUI.Modules.Team.Datas
{
	public class TeamEvent
	{
		public function TeamEvent()
		{
		}
		
		public static const LEAVE_TEAM_AFTER_CHANGE_LINE:String = "leave_team_after_change_line";
		
		public static const INIT_INVITE_PANEL:String = "init_invite_panel";		/** 初始化邀请面板 */
		
		public static const SHOWINVITE:String = "showInvite";					/** 显示被邀请界面 */
		
		public static const REMOVEINVITE:String = "removeInvite";				/** 移除被邀请界面 */
		
		public static const SHOWINVITETEAMINFO:String = "showInviteTeamInfo";	/** 显示邀请的队伍信息 */
		
		public static const HAVAINVITE:String = "havaInvite";					/** 有邀请 */
		
		public static const INVITEINIT:String = "initInvite";					/** 初始化邀请 */
		
		public static const MEMBERINVITESOMEONE:String = "memberInviteSomone";	/** 队员邀请了某人 */
		
		public static const SHOWTEAMINFORMATION:String = "showTeamInformation";	/** 显示组队相关的提示信息 */
		
		public static const INVITETEAMBYNAME:String = "inviteTeamByName";		/** 邀请组队，用名字查询,给聊天的接口 */
		
		public static const SUPER_MAKE_TEAM:String = "super_make_team";			/** 超级组队模式 (只发对方的ID给服务器，服务器判断，对方没队伍则邀请对方组队；对方有队伍且自己无队伍则申请加入对方队伍)*/
		
		public static const SUPER_MAKE_TEAM_BY_NAME:String = "super_make_team_by_name";	/** 超级组队模式 (只发对方的名字给服务器，服务器判断，对方没队伍则邀请对方组队；对方有队伍且自己无队伍则申请加入对方队伍) */
	}
}