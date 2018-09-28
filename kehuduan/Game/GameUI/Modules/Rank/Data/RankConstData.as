package GameUI.Modules.Rank.Data
{
	public class RankConstData
	{
	public static var totalPage  :int = 10;                  	//总页数 ;
	public static var currentPage:int;							//当前页数;
		public static var total_h:int;                    	 	//总行数 ;
		public function RankConstData()
		{
		}
		
		public static const LAB_POS:Array = [
											[332, 478, 240, 392, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_2" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_3" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_4" ]],		//"玩家名称"	"帮派"	"人物等级"	"等级排行榜"
											[332, 478, 240, 386, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_5" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_6" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_7" ]],	//"玩家名称"	"主职业"	"主职业等级"	"主职业排行榜"
											[332, 478, 240, 386, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_8" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_9" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_10" ]],	//"玩家名称"  "副职业"  "副职业等级"  "副职业排行榜"
											[332, 478, 240, 392, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_2" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_11" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_12" ]],		//"玩家名称"  "帮派"	"银两数量"	"富豪排行榜"
											[332, 478, 240, 392, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_13" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_14" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_15" ]], 	//"玩家名称"  "职业"  "外攻"  "外攻排行榜"
											[332, 478, 240, 392, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_13" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_16" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_17" ]],		//"玩家名称"  "职业"  "内攻"  "内攻排行榜"
											[332, 478, 240, 392, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_13" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_18" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_19" ]],		//"玩家名称"  "职业"  "生命力"  "生命力排行榜"
											[332, 478, 240, 392, 498, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_13" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_20" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_21" ]],		//"玩家名称"  "职业"  "暴击"  "暴击排行榜"
											[332, 478, 240, 392, 498, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_13" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_22" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_23" ]],		//"玩家名称"  "职业"   "命中"  "命中排行榜" 
											[332, 478, 240, 392, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_1" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_13" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_24" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_25" ]],		//"玩家名称"  "职业"  "战斗评价"  "战斗力排行榜"
											[332, 478, 240, 382, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_26" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_27" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_28" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_29" ]],		//"帮派名"  "帮派等级"  "帮派人数"  "帮派人数排行榜"
											[332, 478, 240, 382, 494, GameCommonData.wordDic[ "mod_rank_dat_rcd_26" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_28" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_30" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_31" ]],	 //"帮派名"  "帮派人数"  "帮派资产"  "帮派资产排行榜"
											[332, 478, 240, 382, 495, GameCommonData.wordDic[ "mod_rank_dat_rcd_32" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_33" ], GameCommonData.wordDic[ "mod_rank_dat_rcd_34" ],GameCommonData.wordDic[ "mod_rank_dat_rcd_35" ]]  //"主人名称"   "宠物名称"  "战力评价"  "宠物评价排行榜"
										];
		public static var rankAllList:Array = new Array(13);			/** 缓存数组 */
		public static function rankAllListInit():void
		{
			for(var i:int = 0;i < 13;i++)
			{
				var obj:Object = new Object();
				obj.i = null;
				rankAllList[i] = obj;
			}
		} 

	}
}