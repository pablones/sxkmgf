package GameUI.Modules.Unity.LevelUpCondition.OtherLevelUpCondition
{
	/** 分堂升级需要 分堂下辖3个技能至少有一个研究至当前满级(暂时为最后一个条件) */
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.InterUnityFace.IOtherState;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;

	public class SkillStateOther implements IOtherState
	{
		private var _obj:Object;
		public function SkillStateOther(obj:Object)
		{
			_obj = obj;
		}

		public function writeCondition(work:OtherLevelWork):Boolean
		{
			var isOk:Boolean = true;
			var okVaule:int = 0; 
			if(getTopSkill(_obj) == false)
			{
				if(_obj.level == 0)
				{
					if(UnityUtils.getOtherNum() >= int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level,"otherTopNum")))		//分堂数量已满
					{
						UnityConstData.levelNeedList[0] = "\n<font color = '#e9ca9f'>1"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_4" ] + int(UnityUtils.getOtherNum()+1) + GameCommonData.wordDic[ "often_used_level" ] + "</font>";  // 主堂等级需要达到  级
					}
					else
					{
						UnityConstData.levelNeedList[0] = "\n<font color = '"+UnityConstData.LEVELCOLOR+"'>1"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_4" ] + int(UnityUtils.getOtherNum()+1) + GameCommonData.wordDic[ "often_used_level" ] + "</font>";  //  主堂等级需要达到  级
					}
				}
				else UnityConstData.levelNeedList[0] = "\n<font color = '#e9ca9f'>1"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_5" ] + UnityNumTopChange.UnityOtherChange(_obj.level , "skill") + GameCommonData.wordDic[ "often_used_level" ] + "</font>";  // 本堂至少有一个技能研究至  级
				isOk =  false;
				okVaule ++;
			}
			if(okVaule == 0)
			{
				isOk = true;
			}
			return isOk;
		}
		/** 分堂最高等级是否达到要求 */
		private function getTopSkill(obj:Object):Boolean
		{
			if(obj.level == 0) return false;
			var arr:Array = [];
			for(var i:int = 0;i < 3;i++)
			{
				arr[i] = obj["skillStudyCurr" + int(i+1)];
			}
			var df:Array = arr.sort(Array.NUMERIC | Array.DESCENDING);
			if(df[0] >= UnityNumTopChange.UnityOtherChange(obj.level , "skill"))
			{
				return true;
			}
			return false;
		}
		/** 写下分堂升级要求的状态*/
//		private function writeState(color1:String, color2:String = "" , color3:String = "" , color4:String = ""):void
//		{
//			UnityConstData.levelNeedList[0] = "<font color = '" + color1 + "'>\n分堂下辖3个技能至少有一个研究至当前满级</font>";
//			UnityConstData.levelNeedList[1] = "";//空位
//			UnityConstData.levelNeedList[2] = "";//空位
//			UnityConstData.levelNeedList[3] = "";//空位
//			UnityConstData.levelNeedList[4] = "<font color = '#e9ca9f'>\n建设度    "+UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "bulit") + "</font>";
//			UnityConstData.levelNeedList[5] = "<font color = '#e9ca9f '>\n资  金    "+int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "otherLevelUpMoney"))/10000 + "金</font>";
//			UnityConstData.levelNeedList[6] = "<font color = '#e9ca9f'>\n繁荣度    "+UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[UnityConstData.unityCurSelect].level , "bulit") + "</font>";
//			UnityConstData.levelNeedList[7] = "";//空位
//		}
		
	}
}