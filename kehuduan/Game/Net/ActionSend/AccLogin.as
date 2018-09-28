package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class AccLogin
	{
		/** 登录账号服务器  */
		/**
		 * Object obj
		 * obj.type 消息类型
		 * obj.data 参数(Array)
		 *   */
		public static function AccLoginCreate(obj:Object):void 
		{
			if(!GameCommonData.isSend) return;
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(76);
			sendBytes.writeShort(obj.type);

			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=28) {
				sendBytes.writeByte(0);
			}

			tempBytes=new ByteArray();
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);


			while (sendBytes.position!=44) {
				sendBytes.writeByte(0);
			}

			tempBytes=new ByteArray  ;
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=60) {
				sendBytes.writeByte(0);
			}

			tempBytes=new ByteArray  ;
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=76) {
				sendBytes.writeByte(0);
			}
			//trace("isSend");
			GameCommonData.AccNets.Send(sendBytes);
		}
	}
}