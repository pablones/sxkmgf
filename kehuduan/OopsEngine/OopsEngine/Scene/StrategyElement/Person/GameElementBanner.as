package OopsEngine.Scene.StrategyElement.Person
{
    import OopsEngine.Scene.StrategyElement.GameElementAnimal;
    
    import OopsFramework.Game;
	
	/*决斗插旗*/
	public class GameElementBanner extends GameElementAnimal
	{
		public function GameElementBanner(game:Game)
		{
			super(game,new GameElementEnemySkin(this));
		}

		public override function Initialize():void
		{
            // 显示旗帜
			super.Initialize(); 
				
			this.x = this.x + 45;
			this.y = this.y - 35;
			
			this.Enabled 	  = false;
			this.mouseEnabled = false;
		}
	}
}