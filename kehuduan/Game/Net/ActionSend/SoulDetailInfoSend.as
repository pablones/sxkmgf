package Net.ActionSend
{
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	/**
	 * 请求魂魄详细信息
	 * @author lh
	 * 
	 */	
	public class SoulDetailInfoSend
	{
		public function SoulDetailInfoSend()
		{
		}
		
		public static function createSoulDetailInfoMsg(parm:Array):void{
			var m_byteArr:ByteArray=new ByteArray();
			m_byteArr.endian=Endian.LITTLE_ENDIAN;
			m_byteArr.writeShort(16);
			m_byteArr.writeShort(Protocol.MSG_SOUL_DETAIL);
	
			m_byteArr.writeShort(parm.shift());//action
			m_byteArr.writeShort(0);  //对齐
			m_byteArr.writeUnsignedInt(parm.shift());//idUser   0是自己
			m_byteArr.writeUnsignedInt(parm.shift());//
	
			GameCommonData.GameNets.Send(m_byteArr);
		}

	}
}