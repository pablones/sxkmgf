package Net.ActionSend
{
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class EquipSend
	{
		public static function createMsgCompound(parm:Array,isUseHelp:uint=0):void{
			var nGemCount:uint = parm.shift();//用户放入强化或升星宝石的个数
			var gemByteArray:ByteArray = new ByteArray;
			gemByteArray.endian = Endian.LITTLE_ENDIAN;
			for (var i:int=0; i<nGemCount; i++){
				gemByteArray.writeUnsignedInt(parm.shift());   //升星或强化用符的个数
				gemByteArray.writeUnsignedInt(isUseHelp);              //是否自动扣除元宝  1：是
			}
			var nSize:uint = gemByteArray.length;
			var m_byteArr:ByteArray=new ByteArray();
			m_byteArr.endian=Endian.LITTLE_ENDIAN;
			m_byteArr.writeShort(16+nSize);
			m_byteArr.writeShort(Protocol.MSG_EQUIP);
	
			m_byteArr.writeUnsignedInt(parm.shift());//action     68:申请加成数  23:打孔    62:强化 65：升星  82：铸灵   85铸灵转换
			m_byteArr.writeUnsignedInt(parm.shift());//装备ID     转移（取灵目标id）
			m_byteArr.writeUnsignedInt(parm.shift());//用户放入强化或升星宝石的个数   (申请加成数时（1：强化  2：升星);   铸灵（卷轴id或魔灵数） 转移（铸灵目标id）
	
			if (nSize>0)
			{
				m_byteArr.writeBytes(gemByteArray,0,nSize);
			}
			GameCommonData.GameNets.Send(m_byteArr);
		}

	}
}