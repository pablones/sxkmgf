package GameUI.Modules.Chat.Data
{
	public class ChatEvents
	{
		public function ChatEvents()
		{
		}
		
		/**
		 * Command
		 * */
		public static const SENDCOMMAND:String = "SendCommand";
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
		
		/** 小喇叭选定字体颜色  */
		public static const SELECTEDLEOFONTCOLOR:String = "selectedLeoFontColor";
		
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
		
		/** 隐藏快速菜单  */
		public static const HIDEQUICKOPERATOR:String = "HideQuickOperator";
		
		/** 快速私聊  */
		public static const QUICKCHAT:String = "QuickChat";
		
		/** 关闭频道选择  */
		public static const CLOSESELECTCHANNEL:String = "CloseSelectChannel";
		
		/** 收到小喇叭消息  */
		public static const RECEIVELEOMSG:String = "ReceiveLeoMsg";
		
		/** 更新屏蔽列表  */
		public static const UPDATEFILTER:String = "UpdateFilter";
		
		/** 添加物品到聊天中 */
		public static const ADDITEMINCHAT:String = "addItemInChat";
		
		/** 添加物品到小喇叭聊天中 */
		public static const ADD_ITEM_LEO:String = "add_item_leo"; 
		
		/** 清屏 */
		public static const CLEAR_MSG_CUR_CHANNEL:String = "clear_msg_cur_cannel";
		
		/** 防沉迷弹窗 */
		public static const NO_FATIGUE_WINDOW:String = "no_fatigue_window";
		
		/** GM大喇叭 */
		public static const SHOW_BIG_LEO:String = "SHOW_BIG_LEO";
		/** 关闭GM大喇叭 */
		public static const CLOSE_BIG_LEO:String = "CLOSE_BIG_LEO";
		
		/** 加入职业（创建职业频道） */
		public static const HAS_MAINJOB_CHANNEL:String = "has_mainJob_channel";
		
		/** 收到快捷键中聊天频道位，开始初始化频道设置 */
		public static const INIT_CHAT_CHANNEL_VIEW:String = "init_chat_channel_view";
		
		/** 显示/隐藏聊天面板 */
		public static const SHOW_HIDE_CHAT_VIEW:String = "show_hide_chat_view";
		
		/** 主界面 显示/隐藏聊天 按钮闪 */
		public static const SHOW_HIDE_CHAT_FLASH:String = "show_hide_chat_flash";
		/** 是否已发送请求 */
		public static var CLICK_AND_SEND:Boolean = false;
		/** 计时器 */
		public static var CLICK_TIME_STORAGE:int = 0;
		/** 开启/关闭竞技场阵营面板 */
		public static var ARENA_MSG_PANEL_OPEN:String = "arenaMsgPanelOpen";
		public static var ARENA_MSG_PANEL_CLOSE:String = "arenaMsgPanelClose";
		/** 竞技场阵营消息 */
		public static var ARENA_MESSAGE:String = "arenaMessage";
		/** 竞技场阵营聊天面板中的表情/物品 */
		public static var SELECTEDFACETOARENACHAT:String = "selectedFaceToArenaChat";
		public static var ADDITEMINARENACHAT:String = "addItemInArenaChat";
		
		/** BOSS 喊话 */
		public static var BOSS_MESSAGE:String = "bossMessage";
	}
}