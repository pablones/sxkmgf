package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class DepotSend
	{
		public static function createMsgDepot(obj:Object):void
		{
			//trace("Socket Send createMsgDepot");
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(28);
			sendBytes.writeShort(obj.type);
			
			sendBytes.writeUnsignedInt(obj.data.shift());//id
			sendBytes.writeUnsignedInt(obj.data.shift());//type
			sendBytes.writeUnsignedInt(obj.data.shift());		//amount
			sendBytes.writeUnsignedInt(obj.data.shift());		//maxamount
			sendBytes.writeByte(obj.data.shift());		//position
			sendBytes.writeByte(obj.data.shift());		//isbind
			sendBytes.writeShort(obj.data.shift());		//index
			
			sendBytes.writeByte(obj.data.shift());		//action
			while(sendBytes.length < 28)
				sendBytes.writeByte(0);
			
			GameCommonData.GameNets.Send(sendBytes);
//			trace("发送仓库消息");
		}
	}
}