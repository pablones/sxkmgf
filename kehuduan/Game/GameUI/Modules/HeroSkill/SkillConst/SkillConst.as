package GameUI.Modules.HeroSkill.SkillConst
{
	import GameUI.Modules.HeroSkill.View.SkillCell;
	
	public class SkillConst
	{
		public static const MOVESKILL:String = "moveSkill";
		public static const CELLCLICK:String = "cellClick";
		public static const LIFECELL_CLICK:String = "LIFECELL_CLICK";
		public static var heroLevel:int;
		public static const CHANGEF:String = "changeFrame";
		public static const LEARNSKILL:String = "learnSkill";
		public static const SKILLLEARNVIEW:String = "SkillLearnPanel";
		
		public static const LEARN_SKILL_SEND:String="Learn_Skill_Send";
		public static const SKILLUP_RECEIVE:String="up_Skill_Receive";
		public static const NEWSKILL_RECEIVE:String="new_Skill_Receive";
		public static const SKILLUP_SUC:String = "skill_Up_success";
		public static const DRAGSKILLITEM_QUICK:String = "DRAGSKILLITEM_QUICK";    					//拖动图标到快捷栏
		public static const SKILLITEM_DRAGED:String = "SKILLITEM_DRAGED";
		
		public static const LEARN_LIFE_SKILL_PAN:String = "LEARN_LIFE_SKILL_PAN";
		public static const LIFE_SKILL_UPDONE:String = "LIFE_SKILL_UPDONE";										//生活技能升级
		public static const LEARN_LIFESKILL_UIRES:String = "LearnLifeSkillUI";											//学习生活技能面板资源
		public static const LEARN_UNITY_SKILL_UI:String = "LearnUnitySkillUI";
		
		public static const LEARN_UNITY_SKILL_PAN:String = "LEARN_UNITY_SKILL_PAN";							//帮派技能学习打开
		public static const UNITY_SKILL_UPDONE:String = "UNITY_SKILL_UPDONE";										//帮派技能升级成功
		
		public static const REC_UNITY_SKILL_STUDLEV:String = "REC_UNITY_SKILL_STUDLEV";					//收到帮派技能消息
		
		public static const STOP_MOVE_SKILLLEARN_PANEL:String = "stop_move_skilllearn_panel";		//还原并锁定技能学习面板位置    by Ginoo  2010.11.7
		
		public static var aAutoPlay:Array = new Array();
		
		public static var selectedSkill:SkillCell;
		
		public function SkillConst()
		{
			
		}

	}
}