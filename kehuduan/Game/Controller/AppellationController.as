package Controller
{
	import OopsEngine.Role.Appellation;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	public class AppellationController
	{
		public  static function ShowAppellation(playerID:int,appellation:Appellation,IsShow:Boolean):void		
		{
		    var player:GameElementAnimal =  PlayerController.GetPlayer(playerID);
		    if(player != null)
		    {
	    		player.IsShowAppellation = IsShow;
	    		if(IsShow)
			    	player.ShowAppellation(appellation);
			    else
			       player.ShowAppellation(null);
		    }
			
		}	
	}
}