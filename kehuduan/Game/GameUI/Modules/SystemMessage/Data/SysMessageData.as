package GameUI.Modules.SystemMessage.Data
{
	public class SysMessageData
	{
		/** 消息列表面板是否打开 */
		public static var messageListIsOpen:Boolean = false;
		/** 所有消息数组 */
		public static var messageList:Array = [];		
		/** 系统消息总数上线 */
		public static var messageTotalNum:int = 100;
		/** 选中的消息条目 */
		public static var selectItemIndex:int = 0;
		/** 消息内容面板是否打开 */
		public static var messageContentIsopen:Boolean = false;
	}
}