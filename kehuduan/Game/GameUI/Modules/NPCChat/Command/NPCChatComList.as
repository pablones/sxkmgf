package GameUI.Modules.NPCChat.Command
{
	public class NPCChatComList
	{
		
		/** 显示NPC对话框 */
		public static const SHOW_NPC_CHAT:String="show_npc_chat";
		/** 隐藏NPC对话框 */
		public static const HIDE_NPC_CHAT:String="hide_npc_chat";
		/** 收到NPC对话消息 */
		public static const RECEIVE_NPC_MSG:String="receiveNpcMsg";
		/** 发送NPC对话消息*/
		public static const SEND_NPC_MSG:String="sendNpcMSg";
		/** 传入到某个门派消息*/
		public static const TRANSLATE_TO_UNITY:String="TRANSLATE_TO_UNITY";
		/** 设置NPC对话是否可拖动( 0:可拖动 1：不可拖动 ) */
		public static const SET_NPCTALK_ISMOVE:String="SET_NPCTALK_ISMOVE";
		/** 打开模拟NPC对话     */
		public static const SHOW_SIMULATE_NPC_CHAT:String="SHOW_SIMULATE_NPC_CHAT";		
		
		public static const MSGDIALOG_TEXT:uint 	= 101; 		//对话框描述
		public static const MSGDIALOG_LINK:uint 	= 102; 		//对话框选项
		public static const MSGDIALOG_SHOW:uint		= 103; 		//对话框显示
		public static const MSGDIALOG_ANSWER:uint 	= 201 ;		//对话框回答
		
		public function NPCChatComList()
		{
			
		}

	}
}