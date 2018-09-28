package OopsEngine.Skill
{
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	/** 游戏人物技能（管理技能效果、技能影响、影响的目标数量 */ 
	public class GameSkill
	{
		/** 目标HP变化  */
		public static const TARGET_HP 		     : String = "target_hp";
		/** 目标HP暴击变化  */
		public static const TARGET_ERUPTIVE_HP   : String = "target_eruptive_hp";
		/** 目标死亡变化  */
		public static const TARGET_DEAD          : String = "target_dead";
		/** 目标闪避  */
		public static const TARGET_EVASION       : String = "target_evasion";
		/** 目标吸收  */
		public static const TARGET_SUCK          : String = "target_suck";

		/* -------------------------------------------------- */
		
		/**获取技能所造成的伤害状态*/
		public static function GetSkillState(nstate:int):String
		{
			var state:String = "";
			switch(nstate)
			{
				case (0):state = TARGET_HP; break;
				case (1):state = TARGET_EVASION;break;
				case (2):state = TARGET_ERUPTIVE_HP;break;
				         
			}	
			return state;
		}
			
		/** 技能编号 */
		public var SkillID:int;
		/** 技能名 */
		public var SkillName:String;
	    /** 技能说明*/
		public var SkillReamark:String;			
		/**职业编号*/
		public var Job:int;  // -1 普通所有  然后按职业编写
	    /** 需要等级*/
        public var NeedLevel:int;
		/** 技能攻击距离 */
		public var Distance:Number;
		/** 技能飞行效果动画 */
		public var Effect:String;
		/**攻击效果动画**/
	    public var StartEffect:String;
		/** 技能击中后动画*/
		public var HitEffect:String;
		/** 经验等级*/
        public var Exp:int;
		/** 技能模式*/
	    public var SkillMode:int;    	    
	    /** 技能范围*/
	    public var SkillArea:int;
        /** 书的编号*/
        public var BookID:int;
        /** BUFF 编号**/
        public var Buff:int;
        /** 怒气*/
        public var SP:int;
		/** 技能冷却时间 */
		public var CoolTime:int;
		/** 技能冷却时间增值 */
		public var LevelCoolTime:Number;
	    /** 技能消耗基础蓝*/
        public var MP:int;
        /** 每级增加蓝的基数*/
        public var LevelMP:int;
		/** 攻击基础伤害*/
		public var Attack:int;
		/** 攻击增值伤害*/
		public var LevelAttack:Number;
//		/** 技能飞行效果动画MovieClip */
//		public var EffectMovieClip:Boolean = false;
//		/**攻击效果动画MovieClip**/
//	    public var StartEffectMovieClip:Boolean = false;
//		/** 技能击中后动画MovieClip*/
//		public var HitEffecMovieClipt:Boolean = false;
//		/** 技能飞行效果动画BulkLoaderResourceProvider */
//		public var EffectBR:BulkLoaderResourceProvider;
//		/**攻击效果动画BulkLoaderResourceProvider**/
//	    public var StartEffectBR:BulkLoaderResourceProvider;
//		/** 技能击中后动画BulkLoaderResourceProvider*/
//		public var HitEffecBR:BulkLoaderResourceProvider;
//        /**加载成功**/
//        public var OnLoadEffect:Function;
//        public var OnLoadStartEffect:Function;
//        public var OnLoadHitEffect:Function;
//        
//        /**飞行动画加载完成**/
//        public function onEffectComplete():void
//        {
//        	if(OnLoadEffect != null)
//        		OnLoadEffect(Effect);
//        }
//        
//        /**攻击动作动画加载完成**/
//        public function onStartEffectComplete():void
//        {
//        	if(OnLoadEffect != null)
//        		OnLoadStartEffect(StartEffect);
//        }
//        
//        /**击中爆炸效果动画加载完成**/
//        public function onHitEffectComplete():void
//        {
//        	if(OnLoadEffect != null)
//        		OnLoadHitEffect(HitEffect);
//        }	
	}
}