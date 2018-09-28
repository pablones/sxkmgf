package Net.ActionSend
{
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class WarGameSend
	{
		public static function sendWarGameAction(obj:Object):void
		{
			var sendbyte:ByteArray = new ByteArray;
			sendbyte.endian = Endian.LITTLE_ENDIAN;
			
			var msgLenth:int = 52;
			sendbyte.writeShort(msgLenth);
			sendbyte.writeShort(Protocol.ARENA_SCORE); // 2064
			sendbyte.writeUnsignedInt(GameCommonData.Player.Role.Id);
			sendbyte.writeShort( obj.action );//action 3
			
			sendbyte.writeByte( obj.pageIndex );//页码
			sendbyte.writeByte( 13 );//amount 每页显示数量
			
			sendbyte.writeUnsignedInt( obj.memID );//成员id
			sendbyte.writeShort(0);			//阵营
			sendbyte.writeShort(0);			//等级
			sendbyte.writeUnsignedInt(0);	//当前积分
			sendbyte.writeUnsignedInt(0);	//奖励积分
			sendbyte.writeShort(0);			//杀人数
			sendbyte.writeByte(0);			//阵营
			sendbyte.writeByte(0);			//viplev
			sendbyte.writeUnsignedInt(0);	//职业
			
			var name:String = GameCommonData.Player.Role.Name;
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(name,GameCommonData.CODE);
			sendbyte.writeBytes(tempBytes,0,tempBytes.length);
			while(sendbyte.length<msgLenth)
				sendbyte.writeByte(0);
				
			GameCommonData.GameNets.Send(sendbyte);
		}
	}
}