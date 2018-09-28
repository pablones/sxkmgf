package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class TeamSend
	{
		public static function createMsgTeam(obj:Object):void
		{
			if(!GameCommonData.isSend) return;
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
//			sendBytes.writeShort(40);

			sendBytes.writeShort(56);
			sendBytes.writeShort(obj.type);
			
			sendBytes.writeShort(obj.data.shift());			// unAction	
			sendBytes.writeShort(obj.data.shift());			// usLev
			sendBytes.writeShort(obj.data.shift());			//usPro
			sendBytes.writeShort(obj.data.shift());			//usProLev
//			sendBytes.writeShort(0);
//			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(obj.data.shift());	// idPlayer
			sendBytes.writeUnsignedInt(obj.data.shift());	// idMap
			sendBytes.writeUnsignedInt(obj.data.shift());	// idTarget

			sendBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);//szPlayerName[16]	玩家姓名	   （用于游戏显示，和UI无关）
			while(sendBytes.position < 40)
			{
				sendBytes.writeByte(0);			
			}
			
			sendBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);//szTargetName[16]	玩家姓名	   （用于游戏显示，和UI无关）
			while(sendBytes.position < 56)
			{
				sendBytes.writeByte(0);			
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}

		
	}
}