package Net.ActionSend
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class PetEggSend
	{
		public static function SendPetEggAction(obj:Object):void
		{
			var petEggByte:ByteArray = new ByteArray;
			petEggByte.endian = Endian.LITTLE_ENDIAN;
			
			var msgLenth:int = 62;
			petEggByte.writeShort(msgLenth);
			petEggByte.writeShort(1206);
			petEggByte.writeUnsignedInt(GameCommonData.Player.Role.Id);	//玩家ID
			petEggByte.writeUnsignedInt(obj.itemid);					//宠物蛋道具ID
			petEggByte.writeUnsignedInt(0);
			petEggByte.writeShort(0);
			petEggByte.writeByte( obj.action );//action
			petEggByte.writeByte( 0 );//amount
			while(petEggByte.length<36)
				petEggByte.writeByte(0);
			petEggByte.writeUnsignedInt( obj.petid );//宠物id
			petEggByte.writeByte( obj.level );							//宠物档次
			petEggByte.writeByte( 0 );							//宠物成长率
			petEggByte.writeShort( obj.grow );
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			petEggByte.writeShort(0);
			
			
			GameCommonData.GameNets.Send(petEggByte);
		}

	}
}