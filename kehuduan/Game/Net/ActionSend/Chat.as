package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class Chat
	{
		/** 发送聊天内容  */
		public static function SendChat(obj:Object):void 
		{
			//trace("Socket Send SendChat");
			if(!GameCommonData.isSend) return;			
			var ll:int = 0;
			ll++;
			var m_GetLen:ByteArray;
			var s1:String = obj.data.shift();   //物品id       _XXXX
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s1,GameCommonData.CODE);
			var l1:int = m_GetLen.length;
			ll+= l1 + 1;
			var s2:String = obj.data.shift();			//type id     _XXXX
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s2,GameCommonData.CODE);
			var l2:int = m_GetLen.length;
			ll+= l2 + 1;										
			var s3:String = obj.data.shift();							//物品名称   _XXXXX
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s3,GameCommonData.CODE);					
			var l3:int = m_GetLen.length;
			ll+= l3 + 1;	
			var s4:String = obj.data.shift();							//玩家id			_133232356
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s4,GameCommonData.CODE);
			var l4:int = m_GetLen.length;
			ll+= l4 + 1;
			var s5:String = obj.data.shift();							//是否绑定  _1_1
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(s5,GameCommonData.CODE);
			var l5:int = m_GetLen.length;
			ll+= l5 + 1;

			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(28+ll); //2
			sendBytes.writeShort(obj.type);  //2

			sendBytes.writeUnsignedInt(obj.data.shift()); //4
			sendBytes.writeShort(obj.data.shift());  //2
			sendBytes.writeShort(obj.data.shift());  //2
			sendBytes.writeUnsignedInt(obj.data.shift());  //4
			sendBytes.writeUnsignedInt(obj.data.shift());
			sendBytes.writeUnsignedInt(obj.data.shift());
			sendBytes.writeUnsignedInt(obj.data.shift());

			sendBytes.writeByte(5);  //1

			sendBytes.writeByte(l1);  //
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
			sendBytes.writeByte(l3);
			if (l3>0) {
				tempBytes=new ByteArray  ;
				tempBytes.writeMultiByte(s3,GameCommonData.CODE);
				sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			}
			sendBytes.writeByte(l4);
			if (l4>0) {
				tempBytes=new ByteArray  ;
				tempBytes.writeMultiByte(s4,GameCommonData.CODE);
				sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			}
			sendBytes.writeByte(l5);
			if (l5>0) {
				tempBytes=new ByteArray  ;
				tempBytes.writeMultiByte(s5,GameCommonData.CODE);
				sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			}
			GameCommonData.GameNets.Send(sendBytes);
		}

	}
}