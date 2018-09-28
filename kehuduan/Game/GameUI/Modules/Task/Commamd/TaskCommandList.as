package GameUI.Modules.Task.Commamd
{
	public class TaskCommandList
	{
		public static const SHOW_TASKINFO_UI:String="showTaskInfoUI";
		public static const HIDE_TASKINFO_UI:String="hideTaskInfoUI";
		
		public static const SHOW_TASKFOLLOW_UI:String="showTaskFollowUI";
		public static const HIDE_TASKFOLLOW_UI:String="hideTaskFollowUI";
		
		public static const ADD_TASK_FOLLOW:String="addTaskFollow";
		public static const REMOVE_TASK_FOLLOW:String="removeTaskFollow";
		public static const REFRESH_TASKFOLLOW:String="REFRESH_TASKFOLLOW";
		
		public static const UPDATE_MASK:String="upDate_Mask";
		public static const UPDATETASKTREE:String="updateTaskTree";
		public static const RECEIVE_TASK:String="receiveTask";
		/** 更新任务处理状态 */
		public static const UPDATE_TASK_PROCESS:String="update_task_Process";
		/** 状态更新至UI */
		public static const UPDATE_TASK_PROCESS_VIEW:String="UPDATE_TASK_PROCESS_VIEW";
		/** 更新任务总数 */
		public static const UPDATE_TASK_TOTAL:String="updateTaskTotal";
		public static const RECALL_TASK:String="recallTasKCommand";
		 /** 添加可接任务 */
		public static const ADD_ACCEPT_TASK:String="add_accept_task";
		 /** 删除可接任务 */
		public static const REMOVE_ACCEPT_TASK:String="REMOVE_ACCEPT_TASK";
		/** 显示可接的任务 */		
		public static const SELECT_TASKACC_PAGE:String = "select_taskacc_page";
		/** 显示追踪的任务 */
		public static const SELECT_TASKFOLLOW_PAGE:String = "select_taskfollow_page";
		/** 显示选中的任务 */
		public static const SHOW_SELECTED_TASK:String="show_Selected_Task";
		/** 让跟踪显示/隐藏 与任务中的开关按钮同步 */
		public static const SET_SHOW_FOLLOW:String="Set_SHOW_FOLLOW";
		/** 重新刷新一下任务可接列表  */
		public static const UPDATE_ACCTASK_UITREE:String="UPDATE_ACCTASK_UITREE";
		/** 设置是否可以拖动*/
		public static const SET_TASKFOLLOW_DRAG:String="set_TaskFollow_Drag";
		public static const SET_TASKINFO_DRAG:String="set_TaskInfo_Drag";
		/** 更新人物等级任务系统的处理 */
		public static const UPDATE_LEVEL_TASK:String="Update_level_task";
		/** 接收与完成任务特效显示命令*/
		public static const SHOW_TASKSPECIFIC_COMMAND:String="show_taskspecific_command";
		/** 选择精力值面板  */
		public static const SHOW_VIT_PANEL:String="show_vit_ui";
		/** 显示任务中提交装备界面 */
		public static const SHOW_TASKEXPAND_PANEL:String="show_taskexpand_panel";
		/**点击背包物品，触发装备回收*/
		public static const RETURN_EQUIP:String="return_equip";
		/**关闭任务面板*/
		public static const CLOSE_TASK_PANEL:String="close_task_panel";	
		/**播放装备回收特效*/
		public static const SHOW_COLLECT_SPECIAL:String="show_collect_special";
		/** 同步可接任务面板：添加 */
		public static const ACCTASK_SYNC_ADD:String = "acctask_sync_add"; 
		/** 同步可接任务面板：删除 */
		public static const ACCTASK_SYNC_REMOVE:String = "acctask_sync_remove";
		/**可接任务发出fb账号绑定后，返回信息*/
		public static const DEAL_AFTER_SEND_FB_FORM_TASKPANEL:String = "DEAL_AFTER_SEND_FB_FORM_TASKPANEL";
		/**发送fb成就*/
		public static const SEND_FB_AWARD:String = "SEND_FB_AWARD";
		
		public function TaskCommandList()
		{
			
		}

	}
}