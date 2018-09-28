package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementData;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Collections.DictionaryCollection;
	
	public class GameElementEnemyData extends GameElementData
	{
		public function GameElementEnemyData()
		{
			//													 上		    下           左           左上		左下
			this.action[GameElementSkins.ACTION_STATIC] 	 = ["56-60"   ,"36-40"   ,"46-50"   ,"51-55"  ,"41-45"];
			this.action[GameElementSkins.ACTION_NEAR_ATTACK] = ["29-35"   ,"1-7"     ,"15-21"   ,"22-28"  ,"8-14"];
			this.action[GameElementSkins.ACTION_DEAD] 		 = ["104-107" ,"96-99"   ,"100-103" ,"100-103","100-103"];			// 没有左上、左下方向
			this.action[GameElementSkins.ACTION_RUN] 		 = ["89-95"   ,"61-67"   ,"75-81"   ,"82-88"  ,"68-74"];
		}
		
		public override function Analyze(data:*,frames:DictionaryCollection = null):void
		{
			super.Analyze(data);
		 	
		 	this.CreateActionClips(GameElementSkins.ACTION_STATIC		 , this.action[GameElementSkins.ACTION_STATIC]);
		 	this.CreateActionClips(GameElementSkins.ACTION_NEAR_ATTACK   , this.action[GameElementSkins.ACTION_NEAR_ATTACK]);
		 	this.CreateActionClips(GameElementSkins.ACTION_DEAD			 , this.action[GameElementSkins.ACTION_DEAD]);
		 	this.CreateActionClips(GameElementSkins.ACTION_RUN			 , this.action[GameElementSkins.ACTION_RUN]);
		 	
		}	
	}
}