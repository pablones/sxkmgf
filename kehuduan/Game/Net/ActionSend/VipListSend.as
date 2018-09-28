package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class VipListSend
	{
		public static function sendVipListAction(obj:Object):void
		{
			var tutorbyte:ByteArray = new ByteArray;
			tutorbyte.endian = Endian.LITTLE_ENDIAN;
			
			var msgLenth:int = 40;
			tutorbyte.writeShort(msgLenth);
			tutorbyte.writeShort(1069);
			tutorbyte.writeUnsignedInt(GameCommonData.Player.Role.Id);
			tutorbyte.writeShort( obj.action );//action 
			
			tutorbyte.writeByte( obj.pageIndex );//页码
			tutorbyte.writeByte( obj.amount );//amount 每页显示数量
			
			tutorbyte.writeUnsignedInt( obj.memID );//成员id
			tutorbyte.writeUnsignedInt(0);			 //地图ID
			tutorbyte.writeShort(0);				//level
			tutorbyte.writeByte(0);					//线路
			tutorbyte.writeByte(0);					//vip等级

			var name:String = "test";
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(name,"ANSI");
			tutorbyte.writeBytes(tempBytes,0,tempBytes.length);
			while(tutorbyte.length<msgLenth)
				tutorbyte.writeByte(0);
				
			GameCommonData.GameNets.Send(tutorbyte);
		}
	}
}