package  GameUI.Modules.BigDrug.Data
{
	public class BigDrugData
	{
		public function BigDrugData()
		{
		}
		
		/** 大药数据数组 */
		public static var drugList:Array = [];
		/** 药品名 */
		public static var drugName:Array = ["Blood" , "Blue", "Pet"];
		/** 图片Id */
		public static var typeId:Array = ["30100" , "31100", "32100"];
		/** 悬浮框数据 */
		//  金丹瓶  仙丹瓶  血量10000   血量5000    玉液瓶   仙露瓶   气4000   气2000     宠物滋补丹   宠物血量2500   
		public static var drugDataList:Array = [{type:"Boold",big:GameCommonData.wordDic[ "mod_bag_dat_big_dru_1" ],small:GameCommonData.wordDic[ "mod_bag_dat_big_dru_2" ],numbig:GameCommonData.wordDic[ "mod_bag_dat_big_dru_3" ],numsmall:GameCommonData.wordDic[ "mod_bag_dat_big_dru_4" ]},{type:"Bule",big:GameCommonData.wordDic[ "mod_bag_dat_big_dru_5" ],small:GameCommonData.wordDic[ "mod_bag_dat_big_dru_6" ],numbig:GameCommonData.wordDic[ "mod_bag_dat_big_dru_7" ],
																	numsmall:GameCommonData.wordDic[ "mod_bag_dat_big_dru_8" ]},{type:"Pet",big:GameCommonData.wordDic[ "mod_bag_dat_big_dru_9" ],small:GameCommonData.wordDic[ "mod_bag_dat_big_dru_9" ],numbig:GameCommonData.wordDic[ "mod_bag_dat_big_dru_10" ],numsmall:GameCommonData.wordDic[ "mod_bag_dat_big_dru_10" ]}];
	}
}