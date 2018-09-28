package GameUI.ConstData
{
	public class CommandList
	{
		public function CommandList()
		{
		} 
		/** 启动MVC  */
		public static const STARTUP:String = "startup";
		
		/** 启动角色管理 */
		public static const STARTUPROLE:String = "startuprole";
		
		/** 登录账号服务器  */
		public static const LOGINACCSEVERCOMMAND:String = "LoginAccSeverCommand";
		
		/** 登录账号服务器  */
		public static const LOGINGSEVERCOMMAND:String = "LoginGSeverCommand";
		
		/** 选择角色  */
		public static const SELECTROLECOMMAND:String = "SelectRoleCommand";
		
		/** 玩家操作  */
		public static const PLAYERACTIONCOMMAND:String = "playerActionCommand";
		
		/** 收到信息  */
		public static const RECEIVECOMMAND:String = "ReceviedCommand";
		
		/** 收到摆摊留言消息 */
		public static const STALLBBSRECEIVECOMMAND:String = "stallBBSReceiveCommand";
		
		/** 好友的聊天信息*/
		public static const FRIEND_CHAT_MESSAGE:String="FriendChatMessage";
		
		/** 收到角色创建消息*/
		public static const CREATEOVER:String = "CREATEOVER";
		
	}
}