package GameUI.Modules.Friend.command
{
	import GameUI.Mediator.UiNetAction;
	
	public class FriendActionList
	{
		
		/** 初始化好友列表 */
		public static const INIT_FRIEND_LIST:uint=15;
		/** 改变好友的分组*/
		public static const CHANGE_GROUP:uint=25;
		/** 添加好友  */
		public static const ADD_FRIEND:uint=1;
		/** 添加好友成功  */
		public static const ADD_FRIEND_SUCCESS:uint=24;
		/** 添加好友失败 */
		public static const ADD_FRIEND_FAIL:uint=23;
		/** 删除好友 || 删除好友成功*/
		public static const DELETE_FRIEND:uint=27;
		/** 删除好友失败 */
		public static const DELETE_FRIEND_FAIL:uint=28;
		/** 修改心情 */
		public static const EDIT_FEEL:uint=29;
		/** 好友心情更新*/
		public static const FRIEND_FEEL_CHANGE:uint=31;
		/** 好友上线 */
		public static const FRIEND_ONLINE:uint=12;
		/** 好友下线 */
		public static const FRIEND_DOWNLINE:uint=13;
		/** 聊取好友详细信息 */
		public static const GET_FRIEND_INFO:uint=32;
		/** 聊天功能号*/
		public static const CHAT_FLAG:uint=2019;
		/** 留言 */
		public static const LEAVE_WORD:uint=2110;
		/** 系统消息  */
		public static const SYSTEM_MSG:uint=2040;
		/** 队伍*/
		public static const HAVA_TEAM:uint=34;
		/** 修改好友头像 */
		public static const CHANGE_FRIEND_FACE:uint=38;
		
		
		public function FriendActionList()
		{
		}

	}
}