package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	public class CreateRoleSend
	{
		public static function createMsgReg(parm:Array):void
		{
			var m_bWritet:Boolean;
			var m_byteArr:ByteArray = new ByteArray();
			m_byteArr.endian = Endian.LITTLE_ENDIAN;
			var _MSG_REG:uint = 1001;
			m_bWritet = true;
//		USHORT	unMsgSize;
//		USHORT	unMsgType;
//		char	szName[_MAX_NAMESIZE];
//		char	szPassword[_MAX_NAMESIZE];
//		OBJID	idAccount;
//		USHORT	unLook;
//		USHORT	unData;	//职业 + 性别
			m_byteArr.writeShort(44);
		    m_byteArr.writeShort(_MSG_REG);	
			
			var m_TempByteArr:ByteArray;
			
			m_TempByteArr = new ByteArray( );
			m_TempByteArr.writeMultiByte(parm.shift(),GameCommonData.CODE);	                                  //名字				
			m_byteArr.writeBytes(m_TempByteArr,0,m_TempByteArr.length);
			
			while(m_byteArr.position != 20)
				m_byteArr.writeByte(0);
				
			m_TempByteArr = new ByteArray( );
			m_TempByteArr.writeMultiByte(parm.shift(),GameCommonData.CODE);					                  //保护密码
			m_byteArr.writeBytes(m_TempByteArr,0,m_TempByteArr.length);
			
			while(m_byteArr.position != 36)
				m_byteArr.writeByte(0);

			m_byteArr.writeUnsignedInt(parm.shift());                                             //ID
			m_byteArr.writeShort(parm.shift());                                                   //人物图像
			m_byteArr.writeShort(parm.shift());		                                              //性别 * 100 加帮派
			GameCommonData.GameNets.Send(m_byteArr);
		}
	}
}