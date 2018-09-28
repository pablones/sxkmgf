package GameUI.Modules.RoleProperty.Mediator.RoleUtils
{
	import Controller.PlayerController;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.MovieClip;
	import flash.utils.getTimer;
	
	public class RoleLevUp
	{	
		public static var lastRoleUpTime:uint = 0;
		public static var lastPetUpTime:uint = 0;
		/** 播放升级特效 */
		public static function PlayLevUp(id:int , isRole:Boolean):void
		{
			//两次播放特效不能过于频繁
			var lastTime:uint;
			var newTime:uint;
			isRole ? ( lastTime = lastRoleUpTime ) : ( lastTime = lastPetUpTime );
			newTime = getTimer();
			if ( lastTime != 0 )
			{
				if ( ( newTime - lastTime ) < 700 )
				{
					isRole ? ( lastRoleUpTime = newTime ) : ( lastPetUpTime = newTime );
					return;
				}
			}
			isRole ? ( lastRoleUpTime = newTime ) : ( lastPetUpTime = newTime );
			
			var animal:GameElementAnimal = PlayerController.GetPlayer(id);
			if(animal == null) return;
			 	
			var mcLevUp:MovieClip;  
			mcLevUp = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("McLevUp") as MovieClip;
			mcLevUp.alpha = 0.85;
					
			//  皮肤加载完成               不是小蓝人
			if(animal.IsLoadSkins && animal.golem == null)
			{
				if(isRole == true)			// 是人
				{
					mcLevUp.x = 58;
					mcLevUp.y = 105;
				}
				else						// 是宠物
				{
					mcLevUp.x = animal.PlayerHitX;
					mcLevUp.y = animal.PlayerHitY;
				}
				animal.addChild(mcLevUp);
			}
			else
			{
				mcLevUp.x = animal.GameX;
				mcLevUp.y = animal.GameY - 40;
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(mcLevUp);
			}
		}
	}
}