package GameUI.Modules.HeroSkill.Command
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ReceiveSkillCommand extends SimpleCommand
	{
		public function ReceiveSkillCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var obj:Object = notification.getBody();
			updateSkillList(obj.id,obj.level,obj.familiar);
			if ( obj.id>6000 && obj.id<7000 )
			{
				sendNotification(SkillConst.LIFE_SKILL_UPDONE,{id:obj.id,level:obj.level,familiar:obj.familiar});
			}
			else if ( obj.id>2000 && obj.id<2500 )
			{
				sendNotification( SkillConst.UNITY_SKILL_UPDONE,{ skillID:obj.id,skillLevel:obj.level } );
				sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);	
			}
			else
			{
				sendNotification(SkillConst.SKILLUP_SUC,{skillID:obj.id,skillLevel:obj.level});
				//技能学习通知新手引导
				sendNotification(NewerHelpEvent.SKILL_LEVUP_NEWER_HELP, {id:obj.id, lev:obj.level});
			}
		}
		
		private function updateSkillList(newID:int,newLevel:int,familiar:uint):void
		{
			//新学技能
			if(newLevel >= 1)
			{
				if ( newID>6000 && newID<7000 )
				{
					var lifeSkillLevel:GameSkillLevel = new GameSkillLevel(GameCommonData.LifeSkillList[newID] as GameSkill);
					lifeSkillLevel.Level = newLevel;
					lifeSkillLevel.Familiar = familiar;
					GameCommonData.Player.Role.LifeSkillList[newID] = lifeSkillLevel;
				}
				else
				{
					var gameSkillLevel:GameSkillLevel = new GameSkillLevel(GameCommonData.SkillList[newID] as GameSkill);
					gameSkillLevel.Level = newLevel;
					GameCommonData.Player.Role.SkillList[newID] = gameSkillLevel;
				}
			}else
			{
				if ( newID>6000 && newID<7000 )
				{
					( GameCommonData.Player.Role.LifeSkillList[newID] as GameSkillLevel ).Level = newLevel;
				}
				else
				{
					( GameCommonData.Player.Role.SkillList[newID] as GameSkillLevel ).Level = newLevel;
				}
//				for(var skill:* in GameCommonData.Player.Role.SkillList)
//				{
//					var gameSkill:GameSkill = (GameCommonData.Player.Role.SkillList[skill] as GameSkillLevel ).gameSkill;
//					if(gameSkill.SkillID == newID)
//					{
//						(GameCommonData.Player.Role.SkillList[skill] as GameSkillLevel).Level = newLevel;
//					}
//				}
			}
		}
		
	}
}