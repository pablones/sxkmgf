package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class PrepaidLevelSend
	{
		public static function Send(obj:Object):void
		{
			var prepaidByte:ByteArray = new ByteArray;
			prepaidByte.endian = Endian.LITTLE_ENDIAN;
			
			var msgLenth:int = 50;
			var arr:Array = obj.data as Array;
			prepaidByte.writeShort(msgLenth);
			prepaidByte.writeShort(1300);                           //协议号
			prepaidByte.writeShort(obj.type);			        //action
			prepaidByte.writeUnsignedInt(arr[0]);	                    //玩家ID	
			prepaidByte.writeUnsignedInt(arr[1]);		                //累计元宝总数
			prepaidByte.writeUnsignedInt(arr[2]);	                    //显示条元宝总数
			prepaidByte.writeUnsignedInt(arr[3]);			            //还需充值元宝数
			prepaidByte.writeUnsignedInt(arr[4]);		                //元宝VIP等级
			prepaidByte.writeUnsignedInt(arr[5]);                     //掩码数量
			
			prepaidByte.writeUnsignedInt(arr[6]);                     //游历次数
			prepaidByte.writeUnsignedInt(arr[7]);                     //每日最高游历次数
			prepaidByte.writeUnsignedInt(arr[8]);                     //神游千里所需元宝
			
			prepaidByte.writeUnsignedInt(arr[9]);		                //问题集id
			prepaidByte.writeUnsignedInt(arr[10]);                    //选择的答案   
			
			while(prepaidByte.length<50)
				prepaidByte.writeByte(0);
				
			GameCommonData.GameNets.Send(prepaidByte);
		}

	}
}