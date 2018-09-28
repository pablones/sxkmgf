package Net.ActionSend
{
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class OperatorItemSend
	{
		public function OperatorItemSend()
		{
			
		}
		
		/** 玩家操作物品  */
		public static function PlayerAction(obj:Object):void 
		{
			//trace("Socket Send OperatorItemSend");
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			//
			var action:int = obj.data.shift();    
			var aMount:int = obj.data.shift();
			
			if(aMount < 0)
				return;
			if(aMount > 20)	
				return;
			sendBytes.writeShort(36 + aMount * 28); 
			sendBytes.writeShort(Protocol.OPERATE_ITEMS);
					
			sendBytes.writeShort(action);
			sendBytes.writeShort(aMount);
			
			sendBytes.writeUnsignedInt(obj.data.shift());
			sendBytes.writeUnsignedInt(obj.data.shift());
			sendBytes.writeShort(obj.data.shift());
			sendBytes.writeShort(obj.data.shift());
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);			
			 
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			while(sendBytes.length!=36)
			{
				sendBytes.writeByte(0);
			}
//			trace("Socket Send OperatorItemSend");
//			var sendBytes:ByteArray = new ByteArray();
//			sendBytes.endian = Endian.LITTLE_ENDIAN;
//			//
//			var action:int = obj.data.shift();
//			var aMount:int = obj.data.shift();
//			
//			if(aMount < 0)
//				return;
//			if(aMount > 20)	
//				return;
//			sendBytes.writeShort(20 + aMount * 20);
//			sendBytes.writeShort(Protocol.OPERATE_ITEMS);
//					
//			sendBytes.writeShort(action);
//			sendBytes.writeShort(aMount);
//				
//			sendBytes.writeUnsignedInt(obj.data.shift());
//			sendBytes.writeUnsignedInt(obj.data.shift());
//			sendBytes.writeShort(obj.data.shift());
//			sendBytes.writeShort(obj.data.shift());

			for(var i:int = 0; i < aMount ; i ++)
			{
				sendBytes.writeUnsignedInt(obj.data.shift());
				sendBytes.writeUnsignedInt(obj.data.shift());
				sendBytes.writeUnsignedInt(obj.data.shift());	//sendBytes.writeShort(obj.data.shift());
				sendBytes.writeUnsignedInt(obj.data.shift());	//sendBytes.writeByte(obj.data.shift());
				sendBytes.writeByte(obj.data.shift());
				sendBytes.writeByte(obj.data.shift());
				sendBytes.writeShort(obj.data.shift());
				sendBytes.writeUnsignedInt(obj.data.shift());	//单价价格
				sendBytes.writeUnsignedInt(0);					//颜色
			}
			if(GameCommonData.Player.Role.HP == 0)
			{
				return;
			}
			GameCommonData.GameNets.Send(sendBytes);
		}
	}
}