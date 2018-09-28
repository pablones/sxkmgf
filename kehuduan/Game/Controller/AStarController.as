package Controller
{
	import OopsEngine.AI.PathFinder.AStar;
	
	import flash.geom.Point;
	
	public class AStarController
	{
		
		
		/**寻找最近的点**/
		public static function GetNearPoint(startPoint:Point,movePoint:Point,isAttack:Boolean = false):Point
		{
			var endPoint:Point;
			//判断是否是可以走的点
			if(GameCommonData.Scene.gameScenePlay.Map.IsPass(movePoint.x,movePoint.y))
			{
				endPoint = movePoint;
			}
			else
			{
				//判断是否可以攻击
				if(isAttack)
				{
					 if((startPoint.x - movePoint.x) > 0)
					 {						 	
					 	if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,1)))
					 	{
					 		endPoint = AccountPoint(movePoint,1);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,4)))
					 	{
					 		endPoint = AccountPoint(movePoint,4);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,7)))
					 	{
					 		endPoint = AccountPoint(movePoint,7);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,8)))
					 	{
					 		endPoint = AccountPoint(movePoint,8);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,9)))
					 	{
					 		endPoint = AccountPoint(movePoint,9);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,6)))
					 	{
					 		endPoint = AccountPoint(movePoint,6);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,3)))
					 	{
					 		endPoint = AccountPoint(movePoint,3);
					 	}
					    else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,2)))
					 	{
					 		endPoint = AccountPoint(movePoint,2);
					 	}
					 	
					 }
					 else
					 {
					 	if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,3)))
					 	{
					 		endPoint = AccountPoint(movePoint,3);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,6)))
					 	{
					 		endPoint = AccountPoint(movePoint,6);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,9)))
					 	{
					 		endPoint = AccountPoint(movePoint,9);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,8)))
					 	{
					 		endPoint = AccountPoint(movePoint,8);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,7)))
					 	{
					 		endPoint = AccountPoint(movePoint,7);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,4)))
					 	{
					 		endPoint = AccountPoint(movePoint,4);
					 	}
					 	else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,1)))
					 	{
					 		endPoint = AccountPoint(movePoint,1);
					 	}
					    else if(GameCommonData.Scene.gameScenePlay.Map.IsPassPoint(AccountPoint(movePoint,2)))
					 	{
					 		endPoint = AccountPoint(movePoint,2);
					 	}
					 }
				}
				else
				{	     		    					
					 var pathFinder:AStar = new AStar(GameCommonData.Scene.gameScenePlay.Map);
					 pathFinder.Isbalk    = true;
					 var path:Array       = pathFinder.Find(startPoint.x, startPoint.y,movePoint.x,movePoint.y);
					 
					 if(path != null && path.length > 2)					 
					 {
						 for(var m:int = path.length - 1;m > 0;m--)
						 {					 	
						 	if(GameCommonData.Scene.gameScenePlay.Map.IsPass(path[m][0],path[m][1]))
						 	{
						 		endPoint = new Point(path[m][0],path[m][1]);
						 		break;
						 	}
						 }
					 }
				}
			}
			
			return endPoint;
		}		
		
		/**要移动到的目标的点**/
		public static function AccountPoint(movePoint:Point,dir:int):Point
		{
			//要移动到的点
			var endPoint:Point = movePoint.clone();
			
			switch(dir)
			{
				case 1:
				   endPoint.x -= 1;
				   endPoint.y += 1; 
				   break;
				case 2:
				   endPoint.y += 1; 
				   break;
				case 3:
				   endPoint.x += 1;
				   endPoint.y += 1; 
				   break;
				case 4:
				   endPoint.x -= 1;
				   break;
				case 6:
				   endPoint.x += 1;
				   break;
				case 7:
				   endPoint.x -= 1;
				   endPoint.y -= 1; 
				   break;
				case 8:
				   endPoint.y -= 1; 
				   break;
				case 9:
				   endPoint.x += 1;
				   endPoint.y -= 1; 
				   break;		
			}
			
			return endPoint;
		}
		
	}
}