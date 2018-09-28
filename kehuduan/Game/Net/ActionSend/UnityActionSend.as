package Net.ActionSend
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	public class UnityActionSend
	{
		public static function SendSynAction(obj:Object):void 
		{			
//			if(UnityConstData.firstOnline) UnityConstData.dataSendState = true;		//如果是刚上线，就是数据传输中的状态
			var ll:int = 0;
			ll++;
			var m_GetLen:ByteArray;
			var s1:String = obj.data.shift();
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s1,GameCommonData.CODE);
			var l1:int = m_GetLen.length;
			ll+= l1 + 1;
			var s2:String = obj.data.shift();
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s2,GameCommonData.CODE);
			var l2:int = m_GetLen.length;
			ll+= l2 + 1;
			
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(12+ll);
			sendBytes.writeShort(obj.type);
			
			sendBytes.writeShort(obj.data.shift());
			sendBytes.writeShort(obj.data.shift());
			sendBytes.writeUnsignedInt(obj.data.shift());

			sendBytes.writeByte(2);

			sendBytes.writeByte(l1);
			if (l1>0) {
				var tempBytes:ByteArray = new ByteArray();
				tempBytes.writeMultiByte(s1,GameCommonData.CODE);
				sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			}
			sendBytes.writeByte(l2);
			if (l2>0) {
				tempBytes=new ByteArray  ;
				tempBytes.writeMultiByte(s2,GameCommonData.CODE);
				sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			}
			GameCommonData.GameNets.Send(sendBytes);
		}


	}
}