package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementData;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Collections.DictionaryCollection;

	public class GameElementNPCData extends GameElementData
	{
		public function GameElementNPCData(animationData:Object = null)
		{
			if (!animationData)
			{
//														   上	     下        左       左上	 左下
				this.action[GameElementSkins.ACTION_STATIC] = ["1-20"  ,"1-20"  ,"1-20" ,"1-20" ,"1-20"];
				this.action[GameElementSkins.ACTION_RUN] 	= ["21-40" ,"21-40" ,"21-40","21-40","21-40"];
			}
			else
			{
				for (var k:String in animationData)
				{
					this.action[k] = animationData[k];
				}
			}
		}
		
		public override function Analyze(data:*,frames:DictionaryCollection = null):void
		{
		 	this.CreateActionClips(GameElementSkins.ACTION_STATIC		 , this.action[GameElementSkins.ACTION_STATIC]);
		 	this.CreateActionClips(GameElementSkins.ACTION_RUN			 , this.action[GameElementSkins.ACTION_RUN]);
		 	
			super.Analyze(data);
		}
	}
}