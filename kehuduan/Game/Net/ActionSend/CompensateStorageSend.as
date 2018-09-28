package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class CompensateStorageSend
	{
		/** 请求补偿物品列表 */
		public static function listSend(obj:Object):void
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(20);
			sendBytes.writeShort(2095);
			
			sendBytes.writeShort(2);			//action
			sendBytes.writeShort(1);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		/** 请求补偿详情（日志） */
		public static function logSend(obj:Object):void
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(20);
			sendBytes.writeShort(2095);
			
			sendBytes.writeShort(3);			//action
			sendBytes.writeShort(1);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		/** 根据id提取补偿物品 */
		public static function getCompensateById(id:uint):void
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(20);
			sendBytes.writeShort(2095);
			
			sendBytes.writeShort(4);			//action
			sendBytes.writeShort(1);
			sendBytes.writeUnsignedInt(id);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			GameCommonData.GameNets.Send(sendBytes);
		}
	}
}