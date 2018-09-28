package GameUI.Modules.Soul.Data
{
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	
	import OopsEngine.Skill.GameSkill;
	
	public class SoulSkillVO
	{
		public function SoulSkillVO()
		{
		}
		/**技能编号*/
		public var sId:int;
		/**魂魄技能编号*/
		public var number:int;
		/**当前技能状态 0 可升级 1 可学习 */
		public var state:int;
		/**技能名称*/
		public var name:String = "";
		/**技能等级*/
		public var level:int;
		
		public function get num():int
		{
			return SoulData.computeSkillAttack(sId,level,SoulMediator.soulVO.composeLevel,SoulMediator.soulVO.level).nTempData;
		}	
		
		public function get gameskill():GameSkill
		{
			return GameCommonData.SkillList[sId] as GameSkill;
		}
	}
}