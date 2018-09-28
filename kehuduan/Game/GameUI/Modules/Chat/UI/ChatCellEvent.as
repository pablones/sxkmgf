package GameUI.Modules.Chat.UI
{
	import flash.events.Event;

	public class ChatCellEvent extends Event
	{
		
		/** 链接点击事件*/
		public static const CHAT_CELLLINK_CLICK:String="chat_cellLink_click";
		/** 链接信息 */
		public var data:Object;
		public function ChatCellEvent(type:String,linkData:Object,bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.data=linkData;
		}
		
	}
}