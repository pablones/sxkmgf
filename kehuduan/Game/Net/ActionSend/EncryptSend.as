package Net.ActionSend
{
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class EncryptSend
	{
		public function EncryptSend()
		{
		}
		
		public static function send(obj:Object):void
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(60);
			sendBytes.writeShort(Protocol.MSG_ENCRYPT);
						
			if( obj.type == 5 )
			{
				if( obj.index == 0 )
				{
					sendBytes.writeUnsignedInt(GameCommonData.Player.Role.Id);  //个人id
				}else{
					sendBytes.writeUnsignedInt(GameCommonData.Player.Role.unityId);  //帮派id
				}
				sendBytes.writeShort(obj.type);		//action
				sendBytes.writeShort(obj.index);
				sendBytes.writeMultiByte(obj.reName,GameCommonData.CODE);     //修改名字
			}else{
				sendBytes.writeUnsignedInt(GameCommonData.Player.Role.Id);//id
				sendBytes.writeShort(obj.type);		//action
				sendBytes.writeShort(1);			//data
				sendBytes.writeMultiByte(obj.oldPwd,GameCommonData.CODE);    //旧密码字段
				while(sendBytes.length < 36)
					sendBytes.writeByte(0);
				
				sendBytes.writeMultiByte(obj.newPwd,GameCommonData.CODE);//新密码字段
			}
			while(sendBytes.length < 60)
				sendBytes.writeByte(0);
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
	}
}