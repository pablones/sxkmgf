package GameUI.Modules.RoleProperty.Datas
{
	import GameUI.Modules.RoleProperty.Prxoy.ItemUnit;
	
	public class RolePropDatas
	{
		public function RolePropDatas()
		{
		}
		
		//装备栏列表
		public static var ItemUnitList:Array = new Array();
		
		//当前界面
		public static var CurView:int 	= 0;
		
		//装备列表
		public static var ItemList:Array = new Array(16);
		
		//选择当前的装备
		public static var SelectedOutfit:ItemUnit = null;
		
		//选择当前的装备
		public static var ItemPos:Array = [11,12,13,14,15,16,17,18,19,20,21,21,22,22,23,25];
		/** 选择的属性面板代号 */
		public static var selectedPageIndex:uint=1;
		/** 请求上次其他人物信息的时间 */
		public static var lastRequestOtherTime:Number = 0;
		
		public static function getDoubleTimeStr(time:uint):String
		{
			if (time<=0)
			{
				return "0";
			}
			var hourStr:String = ( int(time/3600) ).toString();
			var miniteStr:String = ( int( (time%3600)/60 ) ).toString();
			if ( hourStr=="0" )
			{
				return miniteStr + GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ];   //"分"
			}
			else
			{
				return hourStr + GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_2" ] + miniteStr + GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ];		//"时"   "分"	
			}
		}
		
		public static function getVipTime(time:int):String
		{
			if (time<=0)
			{
				return GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ];    //"无"
			}
			var dayStr:String = ( int(time/86400) ).toString();
			var hourStr:String = ( int((time%86400)/3600)).toString();
			var miniteStr:String = ( int( (time%3600)/60 ) ).toString();
			if ( dayStr == "0" )
			{
				if ( hourStr == "0" )
				{
					return miniteStr+GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ];    //"分"
				}
				else
				{
					return hourStr+GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_2" ]+miniteStr+GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ];  //"时"  "分"
				}
			}
			else
			{
				return dayStr+GameCommonData.wordDic[ "mod_rp_dat_rpd_gvt" ]+hourStr+GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_2" ]+miniteStr+GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ];	  //"天"   "时"   "分"
			}
		}
		
	}
}