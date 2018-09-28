package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import OopsFramework.Game;
	
	/** 人物摊位  */
	public class GameElementStall extends GameElementAnimal
	{
		public function GameElementStall(game:Game)
		{
			super(game, null);
		}
		
		public override function Initialize():void
		{
			super.Initialize();
			
			this.x = this.x - 60;
			this.y = this.y - 170;
			
			this.Enabled 	  = false;
			this.mouseEnabled = false;
		}
	}
}