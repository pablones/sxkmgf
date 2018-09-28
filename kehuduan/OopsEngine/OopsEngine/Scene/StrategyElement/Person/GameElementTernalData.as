package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementData;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Collections.DictionaryCollection;
	
	public class GameElementTernalData extends GameElementData
	{
		public function GameElementTernalData()
		{
			this.action[GameElementSkins.ACTION_STATIC] = ["1-20"  ,"1-20"  ,"1-20" ,"1-20" ,"1-20"];
		}

		public override function Analyze(data:*,frames:DictionaryCollection = null):void
		{
			super.Analyze(data);
		 	
		 	this.CreateActionClips(GameElementSkins.ACTION_STATIC		 , this.action[GameElementSkins.ACTION_STATIC]);
	 	
//		 	//动画解析完成
//		 	if(AnalyzeComplete != null)
//		 	{
//		 		AnalyzeComplete(this);
//		 	}
		}
	}
}