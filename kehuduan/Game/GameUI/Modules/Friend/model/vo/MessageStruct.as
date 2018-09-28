package GameUI.Modules.Friend.model.vo
{
	
	
	/**
	 * 消息格式 
	 * @author felix
	 * 
	 */	
	public class MessageStruct
	{
		/** 发信人ID*/
		public var sendId:uint;
		/** 功能号*/
		public var action:uint;
		public var sendPersonName:String;
		public var receivePersonName:String;
		public var sendTime:Number;
		public var msg:String;
		public var style:String;
		/** 是否是好友 0：是 1：不是*/
		public var isFriend:uint;
		public var face:uint;
		public var isOnline:uint;
		public var feel:String;
		
		public var isleave:Boolean=false;
		public function MessageStruct()
		{
			
		}
		
		public function toString():void{
			for (var s:* in  this){
				trace(s+":"+this[s]);
			}
				
		}

	}
}