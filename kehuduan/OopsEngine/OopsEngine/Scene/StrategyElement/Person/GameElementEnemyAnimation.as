package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Graphics.Animation.AnimationClip;
	import OopsEngine.Graphics.Animation.AnimationFrame;
	import OopsEngine.Graphics.Animation.AnimationPlayer;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.geom.Matrix;
	import flash.geom.Rectangle;
	import flash.utils.Dictionary;

    // 怪物画动对象
	public class GameElementEnemyAnimation extends AnimationPlayer
	{
		public function GameElementEnemyAnimation(playMaxCount:int = LOOP)
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