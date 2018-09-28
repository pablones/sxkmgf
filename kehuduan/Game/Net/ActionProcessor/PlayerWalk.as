package Net.ActionProcessor
{
	import Net.GameAction;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import Vo.WalkDataVo;
	
	import flash.geom.Point;
	import flash.utils.ByteArray;
	
	/** 人物移动信息 */
	public class PlayerWalk extends GameAction
	{
		public static const WALKMODE_RAND:int = 1;		//  闲逛
		public static const MODE_MOVE:int     = 2;		//  追击
		public static const MODE_RUN:int      = 3;		//  回位
		
		public function PlayerWalk(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{					
			bytes.position = 4;
			var obj:WalkDataVo = new WalkDataVo();
			obj.RoleID 	= bytes.readUnsignedInt(); 
			obj.UsPoxX 	= bytes.readUnsignedShort(); 
			obj.UsPoxY 	= bytes.readUnsignedShort(); 
			obj.UcDir 	= bytes.readUnsignedByte();
			obj.UcMode  = bytes.readUnsignedByte();
			bytes.readUnsignedByte();
			bytes.readUnsignedByte();			
			obj.WalkInt = bytes.readUnsignedShort();
			obj.WalkEnd = bytes.readUnsignedShort();	
			
			if(GameCommonData.Scene != null && 
			   GameCommonData.Scene.IsSceneLoaded == true && 
			   GameCommonData.SameSecnePlayerList!=null && 
			   GameCommonData.SameSecnePlayerList[obj.RoleID]!=null)
			{				
				var player:GameElementAnimal = GameCommonData.SameSecnePlayerList[obj.RoleID];
				
				// 如果是自己宠物就是用通信协议移动（不是自己的宠物）
				if(GameCommonData.Player.Role.UsingPetAnimal != null && 
				   GameCommonData.Player.Role.UsingPetAnimal.Role.Id  == obj.RoleID )
				{		
					return;				
				}
				
				// 跟随
				if(player == GameCommonData.TargetAnimal && GameCommonData.IsFollow)
				{
					var targetDistance:int = MapTileModel.Distance(GameCommonData.Player.Role.TileX, 
																   GameCommonData.Player.Role.TileY,
											  					   player.Role.TileX, 
											  					   player.Role.TileY);
					//跟随距离小于 7则 开始跟随 否则取消跟随
					if(targetDistance > 1 && targetDistance < 7 )			// 动态参数据判断与目标的距离
					{
						GameCommonData.Scene.MapPlayerTitleMove(new Point(player.Role.TileX,player.Role.TileY));
					}
					else if(targetDistance > 7)
					{
						GameCommonData.IsFollow = false;
					}
				}
				
				//误差距离
				var ErrorDistance:int = MapTileModel.Distance(obj.UsPoxX, obj.UsPoxY,
											  				  player.Role.TileX, player.Role.TileY);
                 
				if(ErrorDistance < 7) //误差距离 当服务器的距离和自己的
				{					
					if(
					   player.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK &&		// 攻击不移动
					   player.Role.ActionState != GameElementSkins.ACTION_DEAD 					// 死了不移动
					   && obj.RoleID != GameCommonData.Player.Role.Id) //&&						// 收到自己的移动信息不移动
	//				   player.IsLoadComplete == true)
					{
							
						// 服务器发的是两步后的终点过来，这样处理玩家走路的时候，碰到二步中有转弯时，玩家会是直线过的。而发信息人是转过去的
						// 如果不细看，其实影响效果不大.(在墙角时转弯，这样过从理论上是错的）
						switch(obj.UcMode)
						{
							case WALKMODE_RAND:					// 散步
								player.SetMoveSpend(2);		
								break;
							case MODE_MOVE:						// 追击
								player.SetMoveSpend(4);
								break;
							case MODE_RUN:						// 回位
								player.SetMoveSpend(8);
								break;
						}
						
						//是否隐形
	                    if(player.Visible)
	                    {
						    player.MoveTile(new Point(obj.UsPoxX,obj.UsPoxY));
							player.Role.TileX = obj.UsPoxX;
							player.Role.TileY = obj.UsPoxY;
	                    }
	                    else
	                    {
	                    	var gamePoint:Point = MapTileModel.GetTilePointToStage(obj.UsPoxX,obj.UsPoxY);
	                    	player.Role.TileX = obj.UsPoxX;
							player.Role.TileY = obj.UsPoxY;
							player.X = gamePoint.x;
							player.Y = gamePoint.y;
	                    }
					}
					else
					{
						player.Role.TileX = obj.UsPoxX;
						player.Role.TileY = obj.UsPoxY;
					}	
				}
				else
				{
                	var errorPoint:Point = MapTileModel.GetTilePointToStage(obj.UsPoxX,obj.UsPoxY);
                	player.Role.TileX = obj.UsPoxX;
					player.Role.TileY = obj.UsPoxY;
					player.X = errorPoint.x;
					player.Y = errorPoint.y;
				}
			}
		}
	} 
}