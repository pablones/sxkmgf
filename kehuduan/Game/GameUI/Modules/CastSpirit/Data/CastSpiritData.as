package GameUI.Modules.CastSpirit.Data
{
	public class CastSpiritData
	{
		public static const SHOW_CASTSPIRIT_VIEW:String = "show_castSpirit_view";
		
		public static const DROP_EQUIP_FROM_BAG:String = "drop_equip_from_bag";
		
		public static const SUCCESS_CASTSPIRIT:String = "success_castSpirit";
		
		public static const FAILD_CASTSPIRIT:String = "faild_castSpirit";
		
		public static const SHOW_CASTSPIRIT_UP_VIEW:String = "show_castSpirit_up_view";
		
		public static const CLOSE_CASTSPIRIT_VIEW:String = "close_castspirit_view";
		
		public static const SHOW_CASTSPIRIT_TRANSFER_VIEW:String = "show_castSpirit_transfer_view";
		
		public static const SUCCESS_CASTSPIRIT_TRANSFER:String = "success_castspirit_transfer";
		
		public static const FAILD_CASTSPIRIT_TRANSFER:String = "faild_castspirit_transfer";
		
		public static const UPDATE_CASTSPIRIT_NUMBER:String = "update_castspirit_number";
		
		public static var CastSpiritIsOpen:Boolean = false;         //铸灵界面
		
		public static var CastSpiritUpIsOpen:Boolean = false;       //升级铸灵界面
		
		public static var CastSpiritTransferIsOpen:Boolean = false; //铸灵转移界面
		
		public static function isEquip( type:uint ):Boolean
		{
			if( ( type > 110000 && type < 200000) || (type > 210000  && type < 230000) )
			{
				return true;
			}
			return false;
		}
		
		public static function isReel( type:uint ):Boolean
		{
			if ( type == 431301 || type == 431201 || type == 431101 || type == 431001 || type == 430901 || type == 432001 || type == 432101 || type == 432201 || type == 432301 )
			{
				return true;
			}
			return false;
		}
		
		// 1为攻击类的  2为防御类的
		public static function checkEquip( type:uint ):uint
		{
			if ( (110000 < type && type < 140000) || (170000 <= type && type < 200000) )
			{
				return 2;
			}
			if ( (140000 <= type && type < 170000) || (210000 < type && type < 230000) )
			{
				return 1;
			}
			return 0;
		}
		
		// 1为攻击类的  2为防御类的
		public static function checkReel( type:uint ):uint
		{
			if ( type == 431301 || type == 431201 || type == 431101 || type == 431001 || type == 430901 )
			{
				return 2;
			}
			if ( type == 432001 || type == 432101 || type == 432201 || type == 432301 )
			{
				return 1;
			}
			return 0;
		}
		
		public static function isSameEquip( typeOne:uint, typeTwo:uint ):Boolean
		{
			var leftType:uint = checkType(typeOne);
			var rightType:uint = checkType(typeTwo);
			if( leftType == rightType && leftType != 0 && rightType != 0  )
			{
				return true;
			}
			return false;
		}
		
		private static function checkType( type:uint ):uint
		{
			if( 110000 < type && type < 120000 )
			{
				return 1;
			}
			else if( 120000 <= type && type < 130000 )
			{
				return 2;
			}
			else if( 130000 <= type && type < 140000 )
			{
				return 3;
			}
			else if( 140000 <= type && type < 150000 )
			{
				return 4;
			}
			else if( 150000 <= type && type < 160000 )
			{
				return 5;
			}
			else if( 160000 <= type && type < 170000 )
			{
				return 6;
			}
			else if( 170000 <= type && type < 180000 )
			{
				return 7;
			}
			else if( 180000 <= type && type < 190000 )
			{
				return 8;
			}
			else if( 190000 <= type && type < 200000 )
			{
				return 9;
			}
			else if( 210000 <= type && type < 220000 )
			{
				return 10;
			}
			else if( 220000 <= type && type < 230000 )
			{
				return 11;
			}
			return 0;
		}
	}
}