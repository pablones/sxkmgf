package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class GameConnect
	{	
		/** 连接游戏服务器  */	
		public static function GameServerConnect(obj:Object):void 
		{
			if(!GameCommonData.isSend) return;			
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(336);														//原来为76  332
			sendBytes.writeShort(obj.type);

			sendBytes.writeUnsignedInt(obj.data.shift());
			sendBytes.writeUnsignedInt(obj.data.shift());

			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=336) {											//原来为76   332
				sendBytes.writeByte(0);
			}
//			trace("GameCommonData.GameNets.Send(sendBytes);");
			GameCommonData.GameNets.Send(sendBytes);	
		}
	}
}