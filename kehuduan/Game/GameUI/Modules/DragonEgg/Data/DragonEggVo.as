package GameUI.Modules.DragonEgg.Data
{
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	public class DragonEggVo
	{
		public var name:String;
		public var id:int;
		public var lookface:int;				//头像
		public var type:int;						//宠物类型
		public var level:int;					//档次
		public var grow:int;					//成长率
		
//		public var eggId:int;				//宠物蛋id
		
		//资质
		public var streng:int;		//力量
		public var maxStreng:int;
		
		public var force:int;			//灵力
		public var maxForce:int;
		
		public var physical:int;		//体力
		public var maxPhysical:int;
		
		public var concentration:int;		//定力
		public var maxConcentration:int;
		
		public var waza:int;						//身法
		public var maxWaza:int;
		
		public function get dangci():String
		{
			var str:String;
			switch ( level-1 )
			{
				case 1:		//一档
					str = "<font color='"+IntroConst.itemColors[ 1 ]+"'>"+GameCommonData.wordDic["CNNumber_1"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 2:		//二档
					str = "<font color='"+IntroConst.itemColors[ 2 ]+"'>"+GameCommonData.wordDic["CNNumber_2"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 3:   //三档
					str = "<font color='"+IntroConst.itemColors[ 3 ]+"'>"+GameCommonData.wordDic["CNNumber_3"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 4:   //四档
					str = "<font color='"+IntroConst.itemColors[ 3 ]+"'>"+GameCommonData.wordDic["CNNumber_4"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 5:   //五档
					str = "<font color='"+IntroConst.itemColors[ 4 ]+"'>"+GameCommonData.wordDic["CNNumber_5"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 6:   //六档
					str = "<font color='"+IntroConst.itemColors[ 4 ]+"'>"+GameCommonData.wordDic["CNNumber_6"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 7:   //七档
					str = "<font color='"+IntroConst.itemColors[ 5 ]+"'>"+GameCommonData.wordDic["CNNumber_7"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				case 8:    //八档
					str = "<font color='"+IntroConst.itemColors[ 5 ]+"'>"+GameCommonData.wordDic["CNNumber_8"] + GameCommonData.wordDic[ "mod_dra_data_dan_1" ] + "</font>";
				break;
				default:
				break;
			}
			return str;
		}
		
		public function get growStr():String
		{
			var str:String;
			if(grow >= 81) 
			{
				str = "<font color='"+IntroConst.itemColors[ 5 ]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ] + "</font>";   // 完美
			} else if (grow >= 61)
			{
				str = "<font color='"+IntroConst.itemColors[ 4 ]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ] + "</font>";    //  卓越
			} else if(grow >= 41) 
			{
				str = "<font color='"+IntroConst.itemColors[ 3 ] + "'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_14" ] + "</font>";    // 杰出
			} else if(grow >= 21) 
			{
				str = "<font color='"+IntroConst.itemColors[ 2 ]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_15" ] + "</font>";    // 优秀
			} else 
			{
				str = "<font color='"+IntroConst.itemColors[ 1 ]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_16" ] + "</font>";  // 普通
			}
			return str;
		}
	}
}