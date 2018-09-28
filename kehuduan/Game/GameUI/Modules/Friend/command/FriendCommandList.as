package GameUI.Modules.Friend.command
{
	
	/**
	 *  命令类型定义类
	 * @author felix
	 * 
	 */	
	public class FriendCommandList
	{
		/** 初始化好友管理组件  */
		public static const SHOWFRIEND:String="showFriend";
		/** 隐藏好友管理组件 */
		public static const HIDEFRIEND:String="hideFriend";
		/** 显示好友信息 */
		public static const SHOW_FRIENDINFO:String="ShowFriendInfo";
		/** 显示收信息 */
		public static const SHOW_RECEIVE_MSG:String="showReceiveMsg";
		/** 显示发信息  */
		public static const SHOW_SEND_MSG:String="showSendMsg";
		/** 更新好友列表 */
		public static const UPDATE_FRIEND_LIST:String="UpDate_Friend_List";
		/** 更改好友分组 */
		public static const CHANGE_GROUP:String="changeGroup";
		/** 获得表情名称*/
		public static const GET_FACE_NAME:String="getFaceName";
		/** 重新设置好友列表 */
		public static const RESET_FRIEND_LIST:String="resetFriendList";
		/** 受别人邀请加为好友 */
		public static const INVATE_TO_FRIEND:String="invate_to_Friend";
		/** 删除好友成功 */
		public static const DELETE_FRIEND_SUCCESS:String="deleteFriendSuccess";
		/** 显示颜色选择器  */
		public static const SHOW_FONT_COLOR:String="showFontColor";
		/**  隐藏颜色选择器 */
		public static const HIDE_FONT_COLOR:String="hideFontColor";
		/** 选择了某种颜色 */
		public static const SELECTED_FONT_COLOR:String="FriendselectedFontColor";
		/** 修改好友心情成功  */
		public static const EDIT_FEEL_SUCCESS:String="editFeelSuceeess";
		/**  更新好友心情 */
		public static const CHANGE_FRIEND_FEEL:String="changeFriendFeel";
		/** 好友上下线变化  */
		public static const CHANGE_FRIEND_ONLINE:String="changeFriendOnline";
		/** 有留言信息  */
		public static const LEAVE_WORD:String="Leave_Word";
		/** 好友消息 */
		public static const FRIEND_MESSAGE:String="Friend_Message";
		/** 已经读完消息 停止提示*/
		public static const READED_MESSAGE:String="Readed_Message";
		/** 收到消息*/
		public static const RECEIVE_MSG:String="receiveMsg";
		/** 添加临时好友 */
		public static const ADD_TEMP_FRIEND:String="addTempFriend"; 
		/** 可添加好友 */
		public static const ADD_TEMP_ENABLE:String="addTempEnable";
		/** 添加好友  */
		public static const ADD_TO_FRIEND:String="FriendCommandList_addToFriend";
		/** 实时更新好友的队伍情况*/
		public static const FRIEND_TEAM_UPDATE:String="Friend_Team_update";
		/** 清除所有的信息，进行重新申请好友列表 */
		public static const FRIEND_INFO_CLEAR:String="friend_info_clear";
		/** 显示添加好友弹出框*/
		public static const SHOW_FRIEND_ALTER:String="Show_Friend_Alter";
		/** 关闭添加好友弹出框 */
		public static const CLOSE_FRIEND_ALTER:String="Close_Friend_Alter";
		/** 当聊天信息打开的时候，添加一条聊天信息 */
		public static const ADD_MSG_CHAT:String="add_msg_chat";
		/** 聊天界面的信息栏显示 */
		public static const REVEIVE_CHAT_INFO:String="receive_chat_info";
		/** 好友修改头像*/
		public static const CHANGE_FRIEND_FACE:String="changeFriendFace";
		/** 最小化聊天窗 */
		public static const MINIMIZE_MSG_WINDOW:String = "minimizeMsgWindow";
		
		public function FriendCommandList()
		{
			
		}

	}
}