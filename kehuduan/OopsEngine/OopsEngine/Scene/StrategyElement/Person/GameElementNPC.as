package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Game;

	public class GameElementNPC extends GameElementAnimal
	{
		public function GameElementNPC(game:Game)
		{
			super(game, new GameElementNPCSkin(this));
		}
		
		public override function Dispose():void
		{
			this.SetMissionPrompt(0);
			super.Dispose();
		}
	}
}