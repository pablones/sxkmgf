package GameUI.Modules.Pk.Data
{
	import GameUI.Modules.Task.View.AcceptTaskInfoPanel;
	public class PkData
	{
//		public static var isPkOpen:Boolean = false;
		public static const dataArr:Array = [
												{cellText:GameCommonData.wordDic[ "mod_pk_dat_pkd_1" ],data:{type:GameCommonData.wordDic[ "mod_pk_dat_pkd_1" ]}},  //  和平模式   和平模式
												{cellText:GameCommonData.wordDic[ "mod_pk_dat_pkd_2" ],data:{type:GameCommonData.wordDic[ "mod_pk_dat_pkd_2" ]}},   // 除恶模式    除恶模式
												{cellText:GameCommonData.wordDic[ "mod_pk_dat_pkd_3" ],data:{type:GameCommonData.wordDic[ "mod_pk_dat_pkd_3" ]}},		//自由模式	自由模式
												{cellText:GameCommonData.wordDic[ "mod_pk_dat_pkd_4" ],data:{type:GameCommonData.wordDic[ "mod_pk_dat_pkd_4" ]}},		//帮派模式	帮派模式
												{cellText:GameCommonData.wordDic[ "mod_pk_dat_pkd_5" ],data:{type:GameCommonData.wordDic[ "mod_pk_dat_pkd_5" ]}}			//关于PK		关于PK
											]
		/** 无冷却时间的匹配,无和平模式的判断*/
		public static const pkNameList:Array = [
		GameCommonData.wordDic[ "Modules_Pk_Data_PkData_1" ]/** = "和平";*/,
		GameCommonData.wordDic[ "Modules_Pk_Data_PkData_2" ]/** = "除恶";*/,
		GameCommonData.wordDic[ "Modules_Pk_Data_PkData_3" ]/** = "自由";*/,
		GameCommonData.wordDic[ "Modules_Pk_Data_PkData_4" ]/** = "帮派";*/
			];
		public static const pkStateList:Array = [0 , 1 , 2 ,3];		//主角的PK状态，对应切换PK状态的输入表
		public static const pkChangeStateList:Array = [2 , 2, 2, 2];	//

	}
}