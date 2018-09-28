package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class TradeSend
	{
		public static function createMsgTeam(obj:Object):void
		{
//			trace("Socket Send createMsgTeam ");
			if(!GameCommonData.isSend) return;
			var m_byteArr:ByteArray = new ByteArray( );
			m_byteArr.endian = Endian.LITTLE_ENDIAN;
			//////////////////////////////////////////////////////////
			m_byteArr.writeShort(24);
			
			m_byteArr.writeShort(obj.type);
			
			m_byteArr.writeUnsignedInt(obj.data.shift());
			m_byteArr.writeUnsignedInt(obj.data.shift());
			m_byteArr.writeUnsignedInt(obj.data.shift());
			m_byteArr.writeUnsignedInt(obj.data.shift());
			m_byteArr.writeShort(obj.data.shift());
			
			while(m_byteArr.length!=24){
				m_byteArr.writeByte(0);
			}
			//////////////////////////////////////////////////////////
			GameCommonData.GameNets.Send(m_byteArr);
//			trace("发送交易消息...........");
		}
		
	}
}