package GameUI.Modules.PlayerInfo.Command
{
	public class PlayerInfoComList
	{
		public static const INIT_PLAYERINFO_UI:String="initPlayerInfoUI";
		/** 显示 对家 UI*/
		public static const SHOW_COUNTERWORKER_UI:String="showCounterWorkerUI";
		/** 隐藏 对家 UI */
		public static const HIDE_COUNTERWORKER_UI:String="hideCounterWorkerUI";
		/** 选中了玩家某个元素 */
		public static const SELECT_ELEMENT:String="selectElement";
		/** 显示队伍队*/
		public static const SHOW_TEAMLIST:String="showTeamList";
		/**  更新目标的信息 */
		public static const UPDATE_COUNTER_INFO:String="UPDATE_Counter_INFO";
		/** 更新自己的信息 */
		public static const UPDATE_SELF_INFO:String="UPDATE_SELF_INFO";
		/** 玩家攻击提示*/
		public static const UPDATE_ATTACK:String="updateAttack";
		/** 更新组队信息*/
		public static const UPDATE_TEAM:String="PlayerInfo_updateTeam";
		/** 更新队中人物属性*/
		public static const UPDATE_TEAM_UI:String="PlayerInfo_updateTeam_ui";
		/** 选中自己*/
		public static const SELECTEDSELF:String="selectedSelf";
		/** 更新选中自己时的数据*/
		public static const SELECTED_SELF_UPDATE:String="selected_self_update";
		/** 显示玩家物属性 */
		public static const SHOW_PLAYER_DETAILINFO:String="showPlayerDetailInfo";
		/** 显示玩家物属性 */
		public static const SHOW_PLAYER_DETAILINFO_UI:String="showPlayerDetailInfo_ui";
		/** 查看人物详细信息*/
		public static const QUERY_PLAYER_DETAILINFO:String="queryPlayerDetailInfo";
		/** 显示宠物 */
		public static const SHOW_PET_UI:String="Show_Pet_UI";
		/** 更新宠物信息 */
		public static const UPDATE_PET_UI:String="update_pet_UI";
		/** 收回宠物*/
		public static const REMOVE_PET_UI:String="remove_Pet_UI";
		/** 更新BUFF或DEBUFF 目标中 */
		public static const UPDATE_COUNTERWORKER_BUFF:String="UPDate_CounterWorker_BUFF";
		/** 更新队伍下的buff与DEBUFF状态 */
		public static const UPDATE_TEAM_BUFF:String="UPDATE_TEAM_BUFF";
		/** 显示展开或隐藏队伍图标 */
		public static const SHOW_EXPANDTEAM_ICON:String="show_expandTeam_icon";
		/** 隐藏展开或隐藏队伍图标 */
		public static const HIDE_EXPANDTEAM_ICON:String="hide_expandTeam_icon";
		/** 显示队伍 */
		public static const SHOW_TEAM_UI:String="show_team_ui";
		/** 隐藏队伍 */
		public static const HIDE_TEAM_UI:String="hide_team_ui";
		/** 更新队伍上下线 */
		public static const UPDATE_TEAMMEM_ONLINE:String="UPDATE_TEAMMEM_ONLINE";
		/** 处理队伍的位置信息  */
		public static const MSG_TEAMPOS_PROCESS:String="msg_teampos_process";
		/** 玩家离开队伍事件*/
		public static const MSG_LEAVE_TEAM:String="msg_leave_team";
		
		
		
		
		public function PlayerInfoComList()
		{
			
		}

	}
}