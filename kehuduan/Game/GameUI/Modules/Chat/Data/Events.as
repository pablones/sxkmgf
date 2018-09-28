package GameUI.Modules.Chat.Data
{
	public class Events
	{
		public function Events()
		{
		}
		
		/**
		 * Command
		 * */
		public static const SENDCOMMAND:String = "SendCommand";
		public static const RECEIVECOMMAND:String = "ReceviedCommand";
		
		/** 打开频道界面  */
		public static const OPENCHANNEL:String = "OpenChannel";
		
		/** 关闭频道界面  */
		public static const CLOSECHANNEL:String = "CloseChannel";
		
		/** 创建信息区界面  */
		public static const CREATORMSGAREA:String = "CreatorMsgArea";
		
		/** 显示创建小喇叭界面  */
		public static const CREATELRO:String = "CreatorLeo";
		
		/** 关闭创建小喇叭界面  */
		public static const CLOSELEO:String = "CLoseLeo";
		
		/** 显示创建频道界面  */
		public static const CREATORCHANNEL:String = "CreatorChannel";
		
		/** 关闭创建频道界面  */
		public static const CLOSECREATORCHANNEL:String = "CloseCreatorChannel";
		
		/** 显示颜色设置频道界面  */
		public static const SHOWCOLOR:String = "ShowColor";
		
		/** 关闭选择颜色界面界面  */
		public static const HIDECOLOR:String = "HideColor";
		
		/** 表情已经选择发到聊天栏上面 */
		public static const SELECTEDFACETOCHAT:String = "SelectedFaceToChat";
		
		/** 表情已经选择发到小喇叭上面 */
		public static const SELECTEDFACETOLEO:String = "SelectedFaceToLeo";
		
		/** 显示屏蔽列表界面  */
		public static const SHOWFILTERLIST:String = "ShowFilterList";
		
		/** 隐藏屏蔽列表界面  */
		public static const HIDEFILTERLIST:String = "HideFilterList";
		
		/** 选定字体颜色  */
		public static const SELECTEDFONTCOLOR:String = "selectedFontColor";
		
		/** 显示聊天内容  */
		public static const SHOWMSGINFO:String = "ShowMsgInfo";
		
		/** 改变聊天区高度  */
		public static const CHANGEHEIGHT:String = "ChangeHeight";
		
		/** 改变聊天区  */
		public static const CHANGEMSGAREA:String = "ChangeMsgArea";	
		
		/** 改变聊天区的鼠标事件  */
		public static const CHANGEMOUSE:String = "ChangeMouse";	
		
		/** 显示快速菜单  */
		public static const SHOWQUICKOPERATOR:String = "ShowQuickOperator";
		
		/** 快速私聊  */
		public static const QUICKCHAT:String = "QuickChat";
		
		/** 关闭频道选择  */
		public static const CLOSESELECTCHANNEL:String = "CloseSelectChannel";
		
		/** 收到小喇叭消息  */
		public static const RECEIVELEOMSG:String = "ReceiveLeoMsg";

	}
}