package GameUI.Modules.CastSpirit.Net
{
	import Net.ActionSend.EquipSend;
	
	public class CastSpiritNet
	{
		//铸灵（装备id， 卷轴id）   升级铸灵（装备id， 魔灵数）   升级铸灵（取灵装备id， 铸灵装备id） 
		public static function UpCastSpiritSend( equipId:uint, reelId:uint ):void
		{
			var arr:Array = [0, 82, equipId, reelId ];
			EquipSend.createMsgCompound( arr );
		}
		
		//升级铸灵（取灵装备id， 铸灵装备id） 
		public static function TransferCastSpiritSend( leftEquipId:uint, rightEquipId:uint ):void
		{
			var arr:Array = [0, 85, leftEquipId, rightEquipId ];
			EquipSend.createMsgCompound( arr );
		}
	}
}