package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.AI.PathFinder.AStar;
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Scene.GameScene;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Utils.Vector2;
	
	import OopsFramework.Game;
	
	import flash.geom.Point;
	
	public class GameElementPet extends GameElementAnimal
	{
		private var pathFinder:AStar;							// A*寻路对象
		private var pathDirArray:Array = new Array();			// 路径每步方向数组                   
		private var endPoint:Point;								// 终的的A*矩阵坐标
		private var isAStarMoving:Boolean = false;				// 是否已启用A*寻路
		public  var MomentMove:Function;                        // 瞬间移动函数
		public  var isWalk:Boolean = true;                      // 是否可以走路

		public function get PathFinder():AStar
		{
			return pathFinder;
		}
		
		public function get PathDirection():Array
		{
			return this.pathDirArray;
		}
		
		public function GameElementPet(game:Game)
		{
			super(game, new GameElementEnemySkin(this));
		}
		
		/** 设置父级场景  */
		public override function SetParentScene(gameScene:GameScene):void
		{
			super.SetParentScene(gameScene);
			this.pathFinder = new AStar(this.gameScene.Map);
//			this.pathFinder.Isbalk = true;
		}	
		
		/** 宠物移动 
		 * 场景的点
		 * */
		public override function Move(targetPoint:Point,distance:int = 0):void
		{	
//			// 移动目标点和上一次相同，则不进行移动计算
//			this.targetPoint     = targetPoint;
//			this.endPoint 		 = MapTileModel.GetTileStageToPoint(this.targetPoint.x, this.targetPoint.y);		
//			this.AStarMove(distance);
	
	       	if( this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK
			&& this.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{	
				var p:Point = targetPoint.add(new Point(-this.excursionX, -this.excursionY));
				if(this.smoothMove.IsMoving)
				{
					this.smoothMove.AddPath(p);
				}
				else
				{
					this.smoothMove.Move([p]);
				}
				this.Role.Direction = Vector2.DirectionByTan(this.GameX, this.GameY, targetPoint.x, targetPoint.y);
				this.SetAction(GameElementSkins.ACTION_RUN, this.Role.Direction);
			}
		}
		
		/**追赶主人**/
		public function MoveSeek():void
		{
			if(this.Role.MasterPlayer.prepPoint != null &&                                            //判断是否有预存的点
			!(this.Role.MasterPlayer.prepPoint.x == 0 && this.Role.MasterPlayer.prepPoint.y == 0))    
			{
				//设置结束点
				this.endPoint =  this.Role.MasterPlayer.prepPoint; 
				//移动到目标点
				AStarMove();
			}
			//可以行走
			this.isWalk = true;		
		}
		
		
		/** 宠物移动  在宠物不在人的右手边的时候 移动到右手边 然后根据人的轨迹算出宠物的行走轨迹
		 * */
		public function MoveAStar(path:Array):void
		{			
//			var a:int = getTimer();
			
			this.isAStarMoving = true;
			// 开始A*寻路
			this.pathDirArray   = new Array();			
			var pathStage:Array = new Array();
			
			if(path != null && path.length > 1)
			{				
				for(var i:int = 1;i < path.length; i++)
				{					   
					this.pathDirArray.push(MapTileModel.Direction(path[i - 1][0],
																  path[i - 1][1],
																  path[i][0],
																  path[i][1]));
																  
					var p:Point = MapTileModel.GetTilePointToStage(path[i][0], path[i][1]);
					pathStage.push(p.add(new Point(-this.excursionX, -this.excursionY)));
				}	
	
				if(pathStage.length > 0)
				{	
					// 开始移动
					if(this.smoothMove.IsMoving)
					{
						for(var j:int = 0;j < pathStage.length; j++)
						{
							this.smoothMove.AddPath(pathStage[j] as Point);
						}
					}
					else
					{
						this.smoothMove.Move(pathStage);
					}
				}
			}
			else
			{
				this.Stop();																// A*没找到路时人物调用默认动画
			}			
		}
		
        /**添加宠物路径**/
        public static function AddPetPath(petpath:Array,before:Point,next:Point,Dir:int):Array
		{
            if(petpath != null  && petpath.length > 0)
            {
				var pet:Point = new Point(petpath[petpath.length - 1][0], petpath[petpath.length - 1][1]);	
				var personR:Number=Math.atan2((before.y-next.y),(before.x-next.x))*180/Math.PI;
				var petR:Number=Math.atan2((next.y-pet.y),(next.x-pet.x))*180/Math.PI;			
				var nextDis:Number=Math.sqrt((pet.x-next.x)*(pet.x-next.x)+(pet.y-next.y)*(pet.y-next.y));
				var beforeDis:Number=Math.sqrt((pet.x-before.x)*(pet.x-before.x)+(pet.y-before.y)*(pet.y-before.y));
				
				if(nextDis < beforeDis)
				{
					if(Math.abs(personR-petR) == 0) //正对着宠物前进
					{						
						switch(Dir)
						{
							case 1:
									pet.x-=1,pet.y+=1;
									petpath.push([pet.x,pet.y]);
									pet.x-=1;
									petpath.push([pet.x,pet.y]); 
									break;
							case 3:
									pet.x-=1,pet.y-=1;
									petpath.push([pet.x,pet.y]);
									pet.y-=1;
									petpath.push([pet.x,pet.y]);
									break;
							case 7:
									pet.x+=1,pet.y+=1;
									petpath.push([pet.x,pet.y]);
									pet.y+=1;
									petpath.push([pet.x,pet.y]);
									break;
							case 9:
									pet.x+=1,pet.y-=1;
									petpath.push([pet.x,pet.y]);
									
									pet.x+=1;
									petpath.push([pet.x,pet.y]);
									break;
						}
					}
					else if(Math.abs(personR-petR) != 0) //斜向走向宠物时    
					{				
						switch(Dir)
						{			
							case 2:
									pet.x-=1,pet.y+=1;
									petpath.push([pet.x,pet.y]);
									break;
							case 4:
									pet.x+=1,pet.y+=1;
									petpath.push([pet.x,pet.y]);
									break;
							case 6:
									pet.x-=1,pet.y-=1;
									petpath.push([pet.x,pet.y]);
									break;
							case 8:
									pet.x+=1,pet.y-=1;
									petpath.push([pet.x,pet.y]);
									break;
						}
					}
			    }
			   else if((nextDis > beforeDis)&& (Math.abs(personR-petR) != 180)) 
			   {	
					switch(Dir)
						{
						case 1:
								pet.y-=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 3:
								pet.x+=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 7:
								pet.x-=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 9:
								pet.y+=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 2:
								pet.x+=1,pet.y-=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 4:
								pet.x-=1,pet.y-=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 6:
								pet.x+=1,pet.y+=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 8:
								pet.x-=1,pet.y+=1;
								petpath.push([pet.x,pet.y]);
								break;
				
					}
			}
			else if((nextDis > beforeDis)   &&   (Math.abs(personR-petR)  == 180) ){//径向远离宠物时					
				switch(Dir){				
						case 1:
								pet.x-=1,pet.y-=1;
								petpath.push([pet.x,pet.y]);
								pet.x-=1,pet.y-=1;
								petpath.push([pet.x,pet.y]);
								pet.y-=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 3:
								pet.x+=1,pet.y-=1;
								petpath.push([pet.x,pet.y]);
								pet.x+=1,pet.y-=1;
								petpath.push([pet.x,pet.y]);
								pet.x+=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 7:	
								pet.x-=1,pet.y+=1;
								petpath.push([pet.x,pet.y]);
								pet.x-=1,pet.y+=1;
								petpath.push([pet.x,pet.y]);
								pet.x-=1;
								petpath.push([pet.x,pet.y]);
								break;
						case 9:
								pet.x+=1,pet.y+=1;
								petpath.push([pet.x,pet.y]);
								pet.x+=1,pet.y+=1;
								petpath.push([pet.x,pet.y]);
								pet.y+=1;
								petpath.push([pet.x,pet.y]);
								break;
					    }
			      }
             }
             
             return petpath;
        }

		/**返回宠物的预计点**/
    	public static function GetPetPoint(person:Point,Dir:int,nowPet:Point):Point 
		{
			var pet:Point = new Point(0,0);
			var degree:int = 0;
			switch(Dir)
			{
				case 1:
					degree=-135;
					pet.x=person.x-2;
					pet.y=person.y;
					break;
				case 2:
					degree=180;
					pet.x=person.x-2;
					pet.y=person.y;
					break;
				case 3:
					degree=135;
					pet.x=person.x;
					pet.y=person.y-2;
					break;
				case 4:
					degree=-90;
					pet.x=person.x;
					pet.y=person.y+2;
					break;
				case 6:
					degree=90;
					pet.x=person.x;
					pet.y=person.y-2;
					break;
				case 7:
					degree=-45;
					pet.x=person.x;
					pet.y=person.y+2;
					break;
				case 8:
					degree=0;
					pet.x=person.x+2;
					pet.y=person.y;
					break;
				case 9:
					degree=45;
					pet.x=person.x+2;
					pet.y=person.y;
					break;
				default:
					break;
			}	
			
			if(nowPet.x == pet.x && nowPet.y == pet.y)		
			{			
				return  null;
			}
			else
			{
				return pet;
			}
		}
		
	    /**宠物根据场景点移动**/	
		public override function MoveTile(targetPoint:Point,distance:int = 0,IsStagePoint:Boolean = false):void
		{
//			this.endPoint = targetPoint;
//			this.AStarMove(distance);
		    this.Move(MapTileModel.GetTilePointToStage(targetPoint.x,targetPoint.y));
		}
		
		/** 停止移动并抛出移动完成事件 */
		public override function Stop():void
		{
			this.pathDirArray  		 = null;
			this.isAStarMoving		 = false;
			this.smoothMove.IsMoving = false;		
			super.Stop();
		}
		
		/** 使用A*寻路移动 */
		private function AStarMove(distance:int = 0):void
		{
			this.isAStarMoving = true;
			
			// 当前人物脚下点 起点矩阵坐标
			var startPoint:Point = new Point(this.Role.TileX, this.Role.TileY);		
			
			// 开始A*寻路
			this.pathDirArray   = new Array();
			var pathStage:Array = new Array();
			var path:Array      = this.pathFinder.Find(startPoint.x, startPoint.y, this.endPoint.x, this.endPoint.y);
					
	        if(path != null && path.length > 1)
			{
				for(var i:int = 1;i < path.length; i++)
				{					   
					this.pathDirArray.push(MapTileModel.Direction(path[i - 1][0],
																  path[i - 1][1],
																  path[i][0],
																  path[i][1]));
																  
					var p:Point = MapTileModel.GetTilePointToStage(path[i][0], path[i][1]);
					pathStage.push(p.add(new Point(-this.excursionX, -this.excursionY)));
				}	
				
				//判断有效距离删除点
				for(var n:int = 1;n <= distance;n++)
				{
					if(pathStage != null && pathStage.length > 0)
					{
						pathStage.pop();
					}
				}
				
				
				if(pathStage.length > 0)
				{	
					// 开始移动
					if(this.smoothMove.IsMoving)
					{
						for(var j:int = 0;j < pathStage.length; j++)
						{
							this.smoothMove.AddPath(pathStage[j] as Point);
						}
					}
					else
					{
						this.smoothMove.Move(pathStage);
					}
				}
			}
			else
			{
				this.Stop();																// A*没找到路时人物调用默认动画
			}
		}
		
		/**与宠物距离的判断**/
		public function DistanceMaster(master:GameElementAnimal):void
		{
//			var targetDistance:int = MapTileModel.Distance(master.Role.TileX,
//														   master.Role.TileY,
//									  					   this.Role.TileX , 
//									  					   this.Role.TileY);
//		    if(targetDistance >= 10)
//		    {
		    	//宠物飞行
		    	if(MomentMove != null)
		    	{
		    		//MomentMove(new Point(master.Role.TileX,master.Role.TileY),master.Role.Direction);
		    		MomentMove();
		    	}
//		    }		    
		}
		
		/** Event Start */
		/** 宠物每走一步所触发的事件 */
		protected override function onMoveNode(direction:int):Boolean
		{	
			super.onMoveNode(direction);
			
			var IsMove:Boolean = true;
			if(this.Role.ActionState != GameElementSkins.ACTION_DEAD &&
			this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
			{	
				this.Role.Direction = direction;
				this.SetAction(GameElementSkins.ACTION_RUN, this.Role.Direction);
			}
			if(MoveNode!=null)
			{ 
			   IsMove = MoveNode(direction);		
			}
			return IsMove;
		}
		
		/** 宠物移动完成 */
		protected override function onMoveComplete():void
		{
			super.onMoveComplete();
			
			this.Stop();
			this.SetAction(GameElementSkins.ACTION_STATIC);
			if(MoveComplete!=null) MoveComplete();
		}
		/** Event End */
	}
}