package GameUI.Modules.PetPlayRule.PetSkillLearn.Data
{
	public class PetSkillLearnEvent
	{
		public function PetSkillLearnEvent()
		{
		}
		
		public static const SHOW_PETSKILLLEARN_VIEW:String = "show_petSkillLearn_view";
		
		public static const REMOVE_PETSKILLLEARN_VIEW:String = "remove_petSkillLearn_view";
		
		public static const BAG_TO_PETSKILLLEARN:String = "bag_to_petSkillLearn";
		
		/** 技能学习成功 */
		public static const LEARN_SKILL_SUCCESS_PET:String = "learn_skill_success";
		
		/** 遗忘了某技能 */
		public static const FORGET_SKILL_PET:String = "forget_skill_pet";
		
	}
}