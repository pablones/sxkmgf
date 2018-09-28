package GameUI.Modules.Unity.LevelUpCondition
{
	/** 帮派升级需要 分堂数量达到帮派等级数  至少有一个分堂达到主堂的等级  最高级的分堂的技能是否达到了满级*/
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.InterUnityFace.IState;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;

	public class SkillState implements IState
	{
		public function SkillState()
		{
			super();
		}
		public function writeCondition(work:LevelWork):Boolean
		{
			var isOk:Boolean = true;
			var okVaule:int = 0;
			if(UnityUtils.getOtherNum() < int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level,"otherTopNum")))		//分堂数量没达到标准
			{// 至少有  个分堂
				UnityConstData.levelNeedList[0] = "<font color = '#e9ca9f'>\n1"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_1" ]+UnityConstData.mainUnityDataObj.level+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_2" ];
				isOk =  false;
				okVaule ++;
			}
			if(getOtherLevelReach() == false)			//至少有一个分堂达到主堂的等级 
			{ // 至少有一个分堂达到  级
				UnityConstData.levelNeedList[1] = "<font color = '#e9ca9f'>\n2"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_3" ]+ UnityConstData.mainUnityDataObj.level +GameCommonData.wordDic[ "often_used_level" ]+"</font>";
				isOk =  false;
				okVaule ++;
			}
			if(getTopOtherIntex() == false)
			{ // 至少有一个技能研究至  级
				UnityConstData.levelNeedList[2] = "<font color = '#e9ca9f'>\n3"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_4" ] + UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "skill")+GameCommonData.wordDic[ "often_used_level" ]+"</font>";
				isOk =  false;
				okVaule ++;
			}
			if(okVaule == 0)
			{
				writeState(UnityConstData.LEVELCOLOR , UnityConstData.LEVELCOLOR , UnityConstData.LEVELCOLOR );
				isOk =  true;
			}
			return isOk;
		}
		/** 至少有一个技能研究到最高级分堂的满级 , 是的话就返回true ,没达到满级就返回false*/
		private function getTopOtherIntex():Boolean
		{
			var isReach:Boolean = false;
			for(var i:int = 1;i < 5;i++)
			{
				for(var n:int = 1; n < 4; n++)
				{
					if(UnityConstData.otherUnityArray[i]["skillStudyCurr" + n] >= int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "skill")))
					{
						var rty:Array = UnityConstData.otherUnityArray;
						isReach = true;
					}
				}
			}

			return isReach;
		}
		/** 得到分堂最高等级的技能 */
		private function getTopSkill(obj:Object):Boolean
		{
			for(var k:int = 0 ; k < 3;k++)
			{
				if(obj["skillStudyCurr" + k] >= UnityNumTopChange.UnityOtherChange(obj.level , "skill"))
					return true;
			}
			return false;
		}
		/** 至少有一个分堂达到主堂的等级 */
		private function getOtherLevelReach():Boolean
		{
			var isReach:Boolean = false;
			for(var i:int = 0 ; i < UnityConstData.otherUnityArray.length ; i++)
			{
				if(i == 0) continue;
				if(UnityConstData.otherUnityArray[i].level >= UnityConstData.mainUnityDataObj.level)
				{
					isReach = true;
				}
			}
			return isReach;
		}
		/** 写下主堂升级要求的状态*/
		private function writeState(color1:String, color2:String = "" , color3:String = "" , color4:String = ""):void
		{
			UnityConstData.levelNeedList[0] = "<font color = '" + color2 + "'>\n1"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_1" ]+UnityConstData.mainUnityDataObj.level+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_2" ]+"</font>";  // 至少有  个分堂
			UnityConstData.levelNeedList[1] = "<font color = '" + color1 + "'>\n2"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_3" ]+ UnityConstData.mainUnityDataObj.level +GameCommonData.wordDic[ "often_used_level" ]+"</font>";  // 至少有一个分堂达到  级
			UnityConstData.levelNeedList[2] = "<font color = '" + color3 + "'>\n3"+GameCommonData.wordDic[ "mod_uni_lev_oth_bui_write_4" ] + UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "skill")+GameCommonData.wordDic[ "often_used_level" ]+"</font>";  // 至少有一个技能研究至 级 
			UnityConstData.levelNeedList[3] = "";//空位
		}
		
	}
}