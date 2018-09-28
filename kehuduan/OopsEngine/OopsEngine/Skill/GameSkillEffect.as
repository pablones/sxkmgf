package OopsEngine.Skill
{
	import OopsEngine.Scene.Handler;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Utils.SmoothMove;
	
	import flash.display.DisplayObject;
	
	public class GameSkillEffect
	{
		/**被击目标*/
		public var TargerPlayer:GameElementAnimal;
		/**被击目标状态*/
		public var TargerState:String;
		/**被击目标HP*/
		public var TargerHP:int;
		/**平滑移动对象*/
		public var smoothMove:SmoothMove ;
		/**技能施放*/
		public var Effect:DisplayObject;
		/**技能击中*/
		public var HitEffect:DisplayObject;
		/**技能状态**/ // 0 未处理  1 加入飞行动画  2 击中
		public var EffectState:int = 0;
		/**当前被击中目标是否死亡**/
		public var IsDead:Boolean = false;	
		/**打中动画播放事件**/
		public var onAddPlayerHitEffect:Function;
		/**飞行事件**/
		public var onSkillFly:Function;
		/**职责链信息**/
		public var InfoHandler:Handler;
		/**是否被干掉了**/
		public var IsDelete:Boolean = false;
		
	    public function AddPlayerHitEffect():void
	    {
	    	onAddPlayerHitEffect(this);
	    }
	    
	    public function SkillFly():Boolean
	    {
	    	return onSkillFly(this);
	    }
	    
	}
}