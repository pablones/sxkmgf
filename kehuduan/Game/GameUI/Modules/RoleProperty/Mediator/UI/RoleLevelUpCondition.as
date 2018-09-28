package GameUI.Modules.RoleProperty.Mediator.UI
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.Hint.Events.HintEvents;
	
	public class RoleLevelUpCondition
	{
		/** 玩家升级*/
		public static function getIsRoleLevelUp(value:int):Boolean
		{
			var isLevelUp:Boolean = true;
			if(value >= 10 && value <= 18)
			{
				haveJop();
			}
			else if(value >= 19 && value <= 28)
			{
				isLevelUp = getIsMainLevelOk(10);
			}
			else if(value >= 29 && value <= 38)
			{
				isLevelUp = getIsMainLevelOk(20);
			}
			else if(value >= 39 && value <= 48)
			{
				isLevelUp = getIsMainLevelOk(30);
			}
			else if(value >= 49 && value <= 58)
			{
				isLevelUp = getIsMainLevelOk(40);
			}
			else if(value >= 59 && value <= 68)
			{
				isLevelUp = getIsMainLevelOk(50);
			}
			else if(value >= 69 && value <= 78)
			{
				isLevelUp = getIsMainLevelOk(60);
			}
			else if(value >= 79 && value <= 88)
			{
				isLevelUp = getIsMainLevelOk(70);
			}
			else if(value >= 89 && value <= 98)
			{
				isLevelUp = getIsMainLevelOk(80);
			}
			else if(value >= 99 && value <= 108)
			{
				isLevelUp = getIsMainLevelOk(90);
			}
			else if(value >= 109 && value <= 118)
			{
				isLevelUp = getIsMainLevelOk(100);
			}
			else if(value >= 119 && value <= 128)
			{
				isLevelUp = getIsMainLevelOk(110);
			}
			else if(value >= 129 && value <= 138)
			{
				isLevelUp = getIsMainLevelOk(120);
			}
			else if(value >= 139 && value <= 148)
			{
				isLevelUp = getIsMainLevelOk(130);
			}
			else if(value == 149)
			{
				isLevelUp = getIsMainLevelOk(140);
			}
			return isLevelUp;
			
		}
		/** 职业升级 */
		public static function getIsJopLevelUp(value:int):Boolean
		{
			var isLevelUp:Boolean = true;
			if(value >= 49 && value <= 78)
			{
				isLevelUp = SkillData.checkNumSkillLevel(7 , 40);
			} 
			else if(value >= 79)
			{
				isLevelUp = SkillData.checkNumSkillLevel(13 , 70);
			}
//			else if(value >= 49 && value <= 63)
//			{
//				isLevelUp = SkillData.checkNumSkillLevel(7 , 40);
//			}
//			else if(value >= 64 && value <= 78)
//			{
//				isLevelUp = SkillData.checkNumSkillLevel(9 , 55);
//			}
//			else if(value >= 79)
//			{
//				isLevelUp = SkillData.checkNumSkillLevel(13 , 70);
//			}
//			else if(value >= 79)
//			{
//				isLevelUp = SkillData.isCanPromote(80);
//				
//			} 
			return isLevelUp;
		}
		/** 是否加入主职业 */
		private static function haveJop():void
		{
			if(GameCommonData.Player.Role.MainJob.Job == 0) 
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_ui_rluc_1" ], color:0xffff00});   //"加入门派才能继续提升等级"
			}
		}
		/** 主职业等级是否达到了目标值 */
		private static function getIsMainLevelOk(value:int):Boolean
		{
			var isLevelOk:Boolean = true;
			if(GameCommonData.Player.Role.MainJob.Level >= value)
			{
				isLevelOk = true;
			}
			else
			{
				isLevelOk = false;
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_ui_rluc_2" ]+ value + GameCommonData.wordDic[ "mod_rp_med_ui_rluc_3" ], color:0xffff00});  //"主职业等级达到"  "级才可继续升级"
			} 
			return isLevelOk;
		}

	}
}