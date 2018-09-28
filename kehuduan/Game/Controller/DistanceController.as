package Controller
{
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.geom.Point;
	
	public class DistanceController
	{
		/**判断玩家和目标的距离*/
		public static function PlayerTargetAnimalDistance(targetAnimal:GameElementAnimal,distance:int):Boolean
		{
			 if(targetAnimal != null)
			 {
				  var sendp:Point     = new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY);
	        	  var targetp:Point   = new Point(targetAnimal.Role.TileX,targetAnimal.Role.TileY);      	  
	        	  return Distance(sendp,targetp,distance);   
             }
             return false;
		}
		
		public static function PlayerDistance(target:Point,distance:int):Boolean
		{
				var sendp:Point     = new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY);
				 return Distance(sendp,target,distance);   
		}
		
        /**挂机距离**/
        public static function PlayerAutomatism(distance:int,targetp:Point):Boolean
        {  	 	
        	var p:Point = MapTileModel.GetTilePointToStage(GameCommonData.AutomatismPoint.x,GameCommonData.AutomatismPoint.y);
        	return Distance(p,targetp,distance);
        }

        /**宠物与人的距离**/
		public static function  PlayerPetDistance(distance:int):Boolean
        {
        	  var sendp:Point     = new Point(GameCommonData.Player.Role.UsingPetAnimal.Role.TileX,GameCommonData.Player.Role.UsingPetAnimal.Role.TileY);
        	  var targetp:Point   = new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY);      	  
        	  return Distance(sendp,targetp,distance);
        }
        
        
        /**宠物与自身攻击目标的距离**/
		public static function  PetTargetDistance(distance:int):Boolean
        {
        	  var sendp:Point     = new Point(GameCommonData.Player.Role.UsingPetAnimal.Role.TileX,GameCommonData.Player.Role.UsingPetAnimal.Role.TileY);
        	  var targetp:Point   = new Point(GameCommonData.PetTargetAnimal.Role.TileX,GameCommonData.PetTargetAnimal.Role.TileY);  	  
        	  return Distance(sendp,targetp,distance);
        }
        
        /**宠物与切换攻击目标的距离**/
		public static function  PetChangeTargetDistance(distance:int):Boolean
        {
        	if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.TargetAnimal != null)
        	{
        	  var sendp:Point     = new Point(GameCommonData.Player.Role.UsingPetAnimal.Role.TileX,GameCommonData.Player.Role.UsingPetAnimal.Role.TileY);
        	  var targetp:Point   = new Point(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY);       	  
        	  return Distance(sendp,targetp,distance);
        	}
        	return false;
        }
        
        /**判断距离
        * sendPoint 起始点
        * Target    目标点 
        * distance  距离 **/
        public static function Distance(sendPoint:Point,target:Point,distance:int):Boolean
        {
    		var targetDistance:int = MapTileModel.Distance(sendPoint.x, 
														   sendPoint.y,
									  					   target.x, 
									  					   target.y);     
           									  					   
			/** 判断类型动态改与目标判断的距离 */
			if(targetDistance <= distance)			// 动态参数据判断与目标的距离
			{
				return true;
			}
			return false;
        } 
        
	}
}