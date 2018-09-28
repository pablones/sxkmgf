package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class PlayerActionSend
	{
		public static const GETROLEINFO:uint = 237;
		public static const LEVELUP:uint = 238;
		public static const ADDPotential:uint = 239;
		public static const PetMomentMove:uint = 19;
		
		
		/** 玩家操作  */
		public static function PlayerAction(obj:Object):void 
		{
			if(!GameCommonData.isSend) return;
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(48);
			sendBytes.writeShort(obj.type);     //1010    
			
			sendBytes.writeUnsignedInt(obj.data.shift());     	//   特殊任务捐装备的ID字段
			sendBytes.writeUnsignedInt(obj.data.shift());      //玩家ID
			sendBytes.writeShort(obj.data.shift());        //Titlex     //PK开关
			sendBytes.writeShort(obj.data.shift());        //titley
			sendBytes.writeShort(obj.data.shift());        //mapID

			sendBytes.writeShort(0);//对齐

			sendBytes.writeUnsignedInt(obj.data.shift());//用于一个     param ：答题号   元宝符数量  副本判断（脚本传过来的值）
 
			sendBytes.writeShort(obj.data.shift());   //按钮值     （action）
			sendBytes.writeShort(0);//对齐
			sendBytes.writeUnsignedInt(obj.data.shift());    //..........精力的档次
			
			if((obj.data as Array).length==0){
				obj.data.push(0);
			}
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(obj.data.shift(),GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);
			while(sendBytes.length!=48)
			{
				sendBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(sendBytes);
		}

	}
}