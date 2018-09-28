package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//师徒发送消息协议
	public class TutorSend
	{
		//需要发送的数据为成员id和关系
//		public static function sendTutorAction( obj:Object ):void
//		{
//			var tutorbyte:ByteArray = new ByteArray;
//			tutorbyte.endian = Endian.LITTLE_ENDIAN;
//			var msgLenth:int = 56;
//			tutorbyte.writeShort( msgLenth );
//			tutorbyte.writeShort(2038);
//			tutorbyte.writeUnsignedInt( GameCommonData.Player.Role.Id );
//			tutorbyte.writeByte( obj.action );//action 10为请求列表
//				
//			tutorbyte.writeByte( obj.amount );//amount 每页显示数量
//				
//			tutorbyte.writeShort( obj.pageIndex );//页码
//			tutorbyte.writeUnsignedInt( 0 );//成员id
//			tutorbyte.writeUnsignedInt( 0 );//impart
//			tutorbyte.writeShort( 0 );//level
//			tutorbyte.writeShort( 0 );//viplevel
//			tutorbyte.writeUnsignedInt( 0 );//tutorlev
//			tutorbyte.writeUnsignedInt( 0 );//giftmask
//			tutorbyte.writeUnsignedInt( 0 );//battle
//			tutorbyte.writeByte( 0 );//relation
//
//			var name:String = " ";
//			var tempBytes:ByteArray = new ByteArray();
//			tempBytes.writeMultiByte(name,"ANSI");
//			tutorbyte.writeBytes(tempBytes,0,tempBytes.length);
//			while(tutorbyte.length<msgLenth)
//				tutorbyte.writeByte(0);
//			GameCommonData.GameNets.Send(tutorbyte);
//		}
		
		public static function sendTutorAction(obj:Object):void
		{
			var tutorbyte:ByteArray = new ByteArray;
			tutorbyte.endian = Endian.LITTLE_ENDIAN;
			var msgLenth:int = 56;
			tutorbyte.writeShort(msgLenth);
			tutorbyte.writeShort(2038);
			tutorbyte.writeUnsignedInt(GameCommonData.Player.Role.Id);
			tutorbyte.writeShort( obj.action );//action  10为请求师傅等级列表
				
			tutorbyte.writeByte( obj.amount );//amount 每页显示数量
			tutorbyte.writeByte( obj.pageIndex );//页码
			
			tutorbyte.writeUnsignedInt( obj.mentorId );//成员id
			tutorbyte.writeUnsignedInt(555);//主职业/战斗力
			tutorbyte.writeUnsignedInt(666);//副职业/离线时间
			tutorbyte.writeShort(111);//level
			tutorbyte.writeShort(111);//主职业等级
			tutorbyte.writeShort(111);//副职业等级
			tutorbyte.writeShort(111);//头像
			tutorbyte.writeUnsignedInt(222);//传授度/师父奖励掩码
			
			tutorbyte.writeByte(1);//vip等级
			tutorbyte.writeByte(0);//是否有队伍/性别
			tutorbyte.writeByte(1);//线路
			tutorbyte.writeByte(0);//relation

			var name:String = "test";
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(name,GameCommonData.CODE);
			tutorbyte.writeBytes(tempBytes,0,tempBytes.length);
			while(tutorbyte.length<msgLenth)
				tutorbyte.writeByte(0);
				
			GameCommonData.GameNets.Send(tutorbyte);
		}
		
	}
}