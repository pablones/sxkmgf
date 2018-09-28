package GameUI.Modules.Unity.LevelUpCondition.OtherLevelUpCondition
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.InterUnityFace.IOtherState;

	public class BuiltStateOther implements IOtherState
	{
		private var _obj:Object;
		public function BuiltStateOther(obj:Object)
		{
			_obj = obj;
		}

		public function writeCondition(work:OtherLevelWork):Boolean
		{
			var isOk:Boolean = true;
			var okVaule:int = 0;
			writeState(UnityConstData.LEVELCOLOR);
			if(UnityConstData.mainUnityDataObj.unityBuilt < int(UnityNumTopChange.UnityOtherChange(_obj.level , "bulit")))
			{ // 建设度
				UnityConstData.levelNeedList[4] = "<font color = '#e9ca9f'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ]+"    "+UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "bulit") + "</font>";
				isOk =  false;
				okVaule ++;
			}
			if(UnityConstData.mainUnityDataObj.unityMoney < int(UnityNumTopChange.UnityOtherChange(_obj.level , "otherLevelUpMoney")))
			{
				// 资  金   金
				UnityConstData.levelNeedList[5] = "<font color = '#e9ca9f'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_2" ]+"    "+int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "otherLevelUpMoney"))/10000 + GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ]+"</font>";
				isOk =  false;
				okVaule ++;
			}
			if(UnityConstData.mainUnityDataObj.unitybooming < int(UnityNumTopChange.UnityOtherChange(_obj.level , "bulit")))
			{ // 繁荣度
				UnityConstData.levelNeedList[6] = "<font color = '#e9ca9f'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ]+"    "+UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "bulit") + "</font>";
				isOk =  false;
				okVaule ++;
			}
			if(_obj.level == 5)
			{
				isOk =  false;
				okVaule ++;
			}
			if(_obj.level >= UnityConstData.mainUnityDataObj.level)		//不能高于主堂等级
			{// 主堂等级需到达  级
				UnityConstData.levelNeedList[1] =  "<font color = '#e9ca9f'>\n2"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_4" ] + (_obj.level + 1) +GameCommonData.wordDic[ "often_used_level" ]+"</font>";
				isOk =  false;
				okVaule ++;
			}
			else UnityConstData.levelNeedList[1] = "";
			work.setState(new SkillStateOther(_obj));
			isOk =  work.showCondition();
			if(okVaule > 0)  //有一项没满足要求
			{
				isOk = false;
			}
			return isOk;
		}
		/** 写下分堂升级消耗的状态*/
		private function writeState(color:String):void
		{
			// 本堂至少有一个技能研究至   级
			UnityConstData.levelNeedList[0] = "<font color = '" + color + "'>\n1" + GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_5" ] + UnityNumTopChange.UnityOtherChange(_obj.level , "skill")+GameCommonData.wordDic[ "often_used_level" ]+"</font>";
			UnityConstData.levelNeedList[1] = "";//空位
			UnityConstData.levelNeedList[2] = "";//空位
			UnityConstData.levelNeedList[3] = "";//空位
			UnityConstData.levelNeedList[4] = "<font color = '" + color + "'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ]+"    "+UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "bulit") + "</font>";   // 建设度
			UnityConstData.levelNeedList[5] = "<font color = '" + color + "'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_2" ]+"    "+int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "otherLevelUpMoney"))/10000 + GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ]+"</font>";  //资  金   金 
			UnityConstData.levelNeedList[6] = "<font color = '" + color + "'>\n"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ]+"    "+UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "bulit") + "</font>";  // 繁荣度
			UnityConstData.levelNeedList[7] = "";//空位
		}
		
	}
}