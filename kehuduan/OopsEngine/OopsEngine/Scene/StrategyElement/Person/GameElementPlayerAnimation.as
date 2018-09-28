package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Graphics.Animation.AnimationPlayer;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;

	public class GameElementPlayerAnimation extends AnimationPlayer
	{
		public function GameElementPlayerAnimation(playMaxCount:int = LOOP)
		{
        	super(playMaxCount);
		}
		
		public override function StartClip(clipName:String, frameIndex:int = 0):void
		{
			if(clipName.indexOf(GameElementSkins.ACTION_DEAD) > -1)				// 死亡动做不重复播放
			{
				this.playMaxCount = SINGLE;
			}
			else
			{
				this.playMaxCount = LOOP;
			}
			super.StartClip(clipName, frameIndex);
		}
	}
}