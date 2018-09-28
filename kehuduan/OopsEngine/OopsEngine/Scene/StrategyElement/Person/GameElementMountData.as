package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementData;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Collections.DictionaryCollection;

	public class GameElementMountData extends GameElementData
	{
		public function GameElementMountData()
		{
			//											  上	        下      左        左上		左下			
			this.action[GameElementSkins.ACTION_STATIC]= ["5" ,"1" ,"3" ,"4" ,"2"];
			this.action[GameElementSkins.ACTION_RUN]   = ["5" ,"1" ,"3" ,"4" ,"2"];
		}
		
		public override function Analyze(data:*,frames:DictionaryCollection = null):void
		{
			this.CreateActionClips(GameElementSkins.ACTION_STATIC		 , this.action[GameElementSkins.ACTION_STATIC]);
		 	this.CreateActionClips(GameElementSkins.ACTION_RUN			 , this.action[GameElementSkins.ACTION_RUN]);		 	
			
			super.MountAnalyze(data);		 	
		}
	}
}