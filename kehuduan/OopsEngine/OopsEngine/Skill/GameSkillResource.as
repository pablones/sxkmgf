package OopsEngine.Skill
{
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.display.MovieClip;
	
	public class GameSkillResource
	{
		/**动画路径**/
		public var EffectPath:String;
		/**动画名称**/
		public var EffectName:String;
		/** 技能飞行效果动画BulkLoaderResourceProvider */
		public var EffectBR:BulkLoaderResourceProvider = new BulkLoaderResourceProvider();
		/**加载成功**/
        public var OnLoadEffect:Function;
        /**技能编号**/
        public var SkillID:int = 0;
        /**人物编号**/
        public var playerID:int;
//        /**动画数据**/
//        public var animationskill:AnimationSkill;
        /**动画Data**/
        public var geed:GameSkillData;
           
        /**飞行动画加载完成**/
        public function onEffectComplete():void
        {
        	if(OnLoadEffect != null)
        		OnLoadEffect(this);
        }
		
		public function GetAnimation():SkillAnimation
		{
			var animationSkill:SkillAnimation = new SkillAnimation();
			if(EffectPath != null)
        	{
        		try
        		{
        			if(geed == null)
        			{
		        		var mc:MovieClip =  EffectBR.GetResource(EffectPath).GetMovieClip();
						// 游戏动画数据
						animationSkill       = new SkillAnimation();				
						geed                 = new GameSkillData(animationSkill);
						geed.Analyze(mc);	
						EffectBR             = null;
        			}
        			else
        			{
        				geed.SetAnimationData(animationSkill);
        			}
	        	}
	        	catch(e:Error)
	        	{
	        		throw new Error("EffectName  "+ EffectName+ "   EffectPath " + EffectPath);
	        	}		
        	}
        	
        	return animationSkill;
		}
	}
}