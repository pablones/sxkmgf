package GameUI.Modules.Unity.LevelUpCondition
{
	/** 帮派升级 建设度若干,金钱若干繁荣度若干 */
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.InterUnityFace.IState;

	public class BuiltState implements IState
	{

		public function writeCondition(work:LevelWork):Boolean
		{
			var isOk:Boolean = true;		//是否满足条件
			var okVaule:int = 0;
			writeState(UnityConstData.LEVELCOLOR);
			var obj:Object = UnityConstData.mainUnityDataObj;
			if(int(UnityConstData.mainUnityDataObj.unityBuilt) < int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit")))
			{// 建设度
				UnityConstData.levelNeedList[4] = "<font color = '#e9ca9f'>\n" + GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ] + "    "+UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit") + "</font>";
				isOk =  false;
				okVaule ++;
			}
			if(int(UnityConstData.mainUnityDataObj.unityMoney) < int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpMoney")))
			{//资  金 金
				UnityConstData.levelNeedList[5] = "<font color = '#e9ca9f'>\n" + GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_2" ] + "    "+int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpMoney"))/10000 + GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ] + "</font>";
				isOk =  false;
				okVaule ++;
			}
			if(int(UnityConstData.mainUnityDataObj.unitybooming) < int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit")))
			{// 繁荣度 
				UnityConstData.levelNeedList[6] = "<font color = '#e9ca9f'>\n" + GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ] + "    "+UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit") + "</font>";
				isOk =  false;
				okVaule ++;
			}
			work.setState(new SkillState());
			isOk =  work.showCondition();
			if(okVaule > 0)  //有一项没满足要求
			{
				isOk = false
			}
			return isOk;
		}
		/** 写下主堂升级消耗的状态*/
		private function writeState(color:String):void
		{
			
			UnityConstData.levelNeedList[0] = "<font color = '" + color + "'>\n1"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_1" ]+UnityConstData.mainUnityDataObj.level+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_2" ]+"</font>";  // 至少有  个分堂 
			UnityConstData.levelNeedList[1] = "<font color = '" + color + "'>\n2"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_3" ]+ UnityConstData.mainUnityDataObj.level +GameCommonData.wordDic[ "often_used_level" ]+"</font>";  // 至少有一个分堂达到  级
			UnityConstData.levelNeedList[2] = "<font color = '" + color + "'>\n3"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_4" ] + UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "skill")+GameCommonData.wordDic[ "often_used_level" ]+"</font>";  // 至少有一个技能研究至 级
			UnityConstData.levelNeedList[3] = "";//空位
			UnityConstData.levelNeedList[4] = "<font color = '" + color + "'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ]+"    "+UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit") + "</font>";  // 建设度
			UnityConstData.levelNeedList[5] = "<font color = '" + color + "'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_2" ]+"    "+int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpMoney"))/10000 + GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ]+"</font>";  // 资  金 金
			UnityConstData.levelNeedList[6] = "<font color = '" + color + "'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ]+"    "+UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit") + "</font>";  // 繁荣度
			UnityConstData.levelNeedList[7] = "";//空位" + color1 + "
		}
	}
}