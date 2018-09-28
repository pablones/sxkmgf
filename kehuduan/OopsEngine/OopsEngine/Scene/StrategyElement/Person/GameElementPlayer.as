package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.AI.PathFinder.AStar;
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.GameScene;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Game;
	
	import flash.geom.Point;
	
	/** 
	 * 玩家元件 （只管理渲染、移动）
	 * 功能：
	 * 1、人物皮肤加载
	 * 2、人物动画
	 * 3、人物移动
	 */
	public class GameElementPlayer extends GameElementAnimal
	{
		private var pathFinder:AStar;							// A*寻路对象
		private var pathDirArray:Array;							// 路径每步方向数组
		private var endPoint:Point;								// 终的的A*矩阵坐标
		private var isAStarMoving:Boolean = false;				// 是否已启用A*寻路
		public  var prepPoint:Point;                            // 主人要走的目标点 在宠物不能移动的时候 保存宠物瞬移的点
		public  var PathMap:Array;                              // 地图路径 
        	
        public function getTernalWeaponSkinName():String
		{
			return "Resources/Player/Ternal/w1.swf";
		}
		
		public function getTernalWeaponEffectName():String
		{
			if(this.Role.TernalMixLev >= 5)				
			{
				return "Resources/Player/Ternal/w1" + this.Role.TernalMutually +".swf";
			}
			else
			{
				return null;
			}
		}	
        	
		public function get PathDirection():Array
		{
			return this.pathDirArray;
		}
		
		public function GameElementPlayer(game:Game)
		{
			super(game, new GameElementPlayerSkin(this));
		}
		
		
		protected override function onChangeEquip(equipType:String):void
		{
			// 坐骑
			if(equipType == GameElementSkins.EQUIP_MOUNT)
			{
				if(this.Role.MountSkinName != null)									// 上坐骑
				{
					this.excursionY += this.MountHeight;			
					this.y 		    -= this.MountHeight;
					if(this.shadow != null)
					{
						try
						{
							this.removeChild(this.shadow);
						}
						catch(e:Error)
						{
							
						}
					}
					
				}
				else																// 下坐骑
				{
					this.excursionY -= this.MountHeight;
					this.y		    += this.MountHeight;
					if(this.shadow != null) this.addChildAt(this.shadow, 0); 
				}
				
				/** 处理选中后的人物上下坐骑选中影子效果  */
				if(this.shadow!=null && this.shadow.name == "Select")
				{
					this.IsSelect(true);
				}
				
				checkPoint();
			}
		}
		
		/** 设置父级场景  */
		public override function SetParentScene(gameScene:GameScene):void
		{
			super.SetParentScene(gameScene);
			this.pathFinder = new AStar(this.gameScene.Map);
		}
		
		/**移除装备*/
		public override function RemoveSkin(skinType:String):void
		{
			this.skins.RemovePersonSkin(skinType);
		}
	
		/** 人物换装备 */
		public override function SetSkin(skinType:String,skinName:String):void 
		{
			switch(skinType)
			{
				case GameElementSkins.EQUIP_PERSON:
					this.Role.PersonSkinName = skinName;
					GameElementPlayerSkin(this.skins).ChangePerson(true);
					break;
				case GameElementSkins.EQUIP_WEAOIB:
					this.Role.WeaponSkinName = skinName;
					GameElementPlayerSkin(this.skins).ChangeWeapon(true);
					break;
				case GameElementSkins.EQUIP_MOUNT:
					this.Role.MountSkinName = skinName;
					GameElementPlayerSkin(this.skins).ChangeMount(true);
					break;
			   case GameElementSkins.EQUIP_WEAOIBEFFECT:
			        this.Role.WeaponEffectName = skinName;
                    GameElementPlayerSkin(this.skins).ChangeWeaponEffect(true);
					break;
				default:
					break
			}
		}
		
		/** 人物移动 */
		public override function Move(targetPoint:Point,distance:int = 0):void
		{	
			this.MustMovePoint = targetPoint;
			this.Dis           = distance;
			
			if(this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
			{
				// 移动目标点和上一次相同，则不进行移动计算
				this.targetPoint = targetPoint;
				this.endPoint 	 = MapTileModel.GetTileStageToPoint(this.targetPoint.x, this.targetPoint.y);
				this.AStarMove(distance);
			}
		}
		
		public override function MoveTile(targetPoint:Point,distance:int = 0,IsStagePoint:Boolean = false):void
		{
			if(this.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				this.endPoint = targetPoint;
				this.AStarMove(distance,IsStagePoint);
			}
		}
		
		/** 停止移动并抛出移动完成事件 */
		public override function Stop():void
		{
			this.pathDirArray  		 = null;
			this.isAStarMoving		 = false;
			this.smoothMove.IsMoving = false;
			//当人物停止移动的时候修改宠物的去向点
			if(this.Role.UsingPetAnimal != null)
			{
				this.prepPoint = new Point(this.Role.TileX,this.Role.TileY);
			}
			super.Stop();
		}
		
		/** 使用A*寻路移动 */
		private function AStarMove(distance:int = 0,IsStagePoint:Boolean = false):void
		{
			this.isAStarMoving = true;		
			var startPoint:Point = null;
			// 当前人物脚下点 起点矩阵坐标
			if(IsStagePoint)
			{
				startPoint =  MapTileModel.GetTileStageToPoint(this.GameX, this.GameY);
			}
			else
			{
 		    	startPoint = new Point(this.Role.TileX, this.Role.TileY);
 		    }			
			
			// 开始A*寻路
			this.pathDirArray   = new Array();
			var pathStage:Array = new Array();
			PathMap = new Array();
			var path:Array      = this.pathFinder.Find(startPoint.x, startPoint.y, this.endPoint.x, this.endPoint.y);
				
			//宠物走的路
			var petpath:Array = new Array();

			if(this.Role.Type == GameRole.TYPE_OWNER &&  this.Role.UsingPetAnimal != null      // 判断是不是自己的宠物 宠物是否存在 
		    && this.Role.UsingPetAnimal.isWalk)                                                // 判断宠物是否可以走	 
			{
    			//判断宠物是否在正确的点 否则A×  
				var petEndPoint:Point =  GameElementPet.GetPetPoint(new Point(startPoint.x,startPoint.y) ,this.Role.Direction,new Point(this.Role.UsingPetAnimal.Role.TileX,this.Role.UsingPetAnimal.Role.TileY));
				var petStartPoint:Point = new Point(this.Role.UsingPetAnimal.Role.TileX, this.Role.UsingPetAnimal.Role.TileY);

				//判断是否需要走A*		
				if(petEndPoint != null && petEndPoint.x != 0 && petEndPoint.y != 0)
				{
					//宠物走到人身边不算A*阻挡
					this.Role.UsingPetAnimal.PathFinder.Isbalk = true;
					petpath  = this.Role.UsingPetAnimal.PathFinder.Find(petStartPoint.x, petStartPoint.y, petEndPoint.x,petEndPoint.y);
					this.Role.UsingPetAnimal.PathFinder.Isbalk = false;
				}
				else
				{
					//保存现在的点
					petpath.push([petStartPoint.x,petStartPoint.y]);
				}   				
			}
			
			//判断是否有要走的路
			if(path != null && path.length > 1)
			{
				//判断有效距离删除点
				for(var n:int = 1;n <= distance;n++)
				{
					if(path.length > 0)
					{
						path.pop();
					}
				}			
				//是否还需要多走路
				if(path.length > 1)
				{
					prepPoint = new Point(path[path.length - 1][0],path[path.length - 1][1]);
									
					for(var i:int = 1;i < path.length; i++)
					{	
						var dir:int = 	MapTileModel.Direction(path[i - 1][0],
																	  path[i - 1][1],
																	  path[i][0],
																	  path[i][1]);																		   
						this.pathDirArray.push(dir);
						
						var p:Point = MapTileModel.GetTilePointToStage(path[i][0], path[i][1]);
										
						if(this.Role.Type == GameRole.TYPE_OWNER &&  this.Role.UsingPetAnimal != null      // 判断是不是自己的宠物 宠物是否存在 
		   				 && this.Role.UsingPetAnimal.isWalk)                                               // 判断宠物是否可以走	 
						{
							GameElementPet.AddPetPath(petpath,new Point(path[i - 1][0],path[i - 1][1]) ,new Point(path[i][0],path[i][1]) ,dir);										  
						}						
						pathStage.push(p.add(new Point(-this.excursionX, -this.excursionY)));
						this.PathMap.push(p.clone());
					}	
					
					if(MapPathUpdate != null)
					{
						MapPathUpdate();
					}
					
					if(SMapPathUpdate !=null){
						SMapPathUpdate();
					}
					
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
					
					if(this.Role.Type == GameRole.TYPE_OWNER &&  this.Role.UsingPetAnimal != null      // 判断是不是自己的宠物 宠物是否存在 
		            && this.Role.UsingPetAnimal.isWalk)                                                // 判断宠物是否可以走	 
					{
						if(this.Role.UsingPetAnimal.isWalk)        //判断宠物是否可以行走
						{
							if(petpath !=null && petpath.length > 0)
							{
								this.Role.UsingPetAnimal.MoveAStar(petpath);  //宠物行走
							}
						}
					}				
				}	
			}
			else
			{
				this.Stop();
			}
		}
	
		/** Event Start */
		protected override function onMoveNode(direction:int):Boolean
		{			
			super.onMoveNode(direction);
			
			var IsMove:Boolean = true;		
			if(this.Role.ActionState !=  GameElementSkins.ACTION_NEAR_ATTACK
			&& this.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{	
				this.Role.Direction = direction;
				this.SetAction(GameElementSkins.ACTION_RUN, this.Role.Direction);			
			}
			if(MoveNode!=null) 
			{
				IsMove = MoveNode(direction);		
			}
			
			if(this.Role.gameElementTernal != null)
			{	
				this.Role.gameElementTernal.Move(GetTernalPoint(),this.smoothMove.MoveStepLength);
			    this.gameScene.MiddleLayer.DepthSort(this.Role.gameElementTernal);
			}
			
			
			return IsMove;
		}
		
		/** 人物移动完成 */
		protected override function onMoveComplete():void
		{
			super.onMoveComplete();
			
			if(this.Role.ActionState !=  GameElementSkins.ACTION_NEAR_ATTACK
			&& this.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{	
				this.Stop();
				this.SetAction(GameElementSkins.ACTION_STATIC);
				this.PathMap = null;
			}
			if(MoveComplete!=null) MoveComplete();
			
			if(this.Role.gameElementTernal != null)
			{
				this.Role.gameElementTernal.Move(GetTernalPoint(),this.smoothMove.MoveStepLength);
			    this.gameScene.MiddleLayer.DepthSort(this.Role.gameElementTernal);
			}
			
		}
		/** Event End */	
		public function AddTernal():void
		{
			RemoveTernal();
		    //死亡不显示武魂
			if(this.Role.HP >0 && this.Role.ActionState != GameElementSkins.ACTION_DEAD
			&& this.Role.isTernal )
			{
				var gameElementTernal:GameElementTernal = new GameElementTernal(this.Games);
	            gameElementTernal.Role = new GameRole();
	            gameElementTernal.Role.WeaponSkinName = getTernalWeaponSkinName();
	            gameElementTernal.Role.WeaponEffectName = getTernalWeaponEffectName();
	            this.Role.gameElementTernal = gameElementTernal;
	            gameElementTernal.x = GetTernalPoint().x;
	            gameElementTernal.y = GetTernalPoint().y;
	            gameElementTernal.SetParentScene(this.gameScene);
	            this.gameScene.MiddleLayer.Elements.Add(gameElementTernal);
			}
		}
		
		public function RemoveTernal():void
		{
            if(this.Role.gameElementTernal != null)
            {
            	try
            	{
            		this.gameScene.MiddleLayer.Elements.Remove(this.Role.gameElementTernal);
            	}
            	catch(e:Error)
            	{
            		
            	}
            	this.Role.gameElementTernal.Dispose();
           	 	this.Role.gameElementTernal = null;
           	}
		}
		
		public function checkPoint():void
		{
			if(this.Role.gameElementTernal != null )
			{
				this.Role.gameElementTernal.x = GetTernalPoint().x;
				this.Role.gameElementTernal.y = GetTernalPoint().y;
			}
		}
		
		public function GetTernalPoint():Point		
		{
			
			var p:Point = MapTileModel.GetTilePointToStage(this.Role.TileX, this.Role.TileY);
			
			if(this.Role.Direction == 1 ||
			   this.Role.Direction == 4 ||
			   this.Role.Direction == 7 ||
			   this.Role.Direction == 8 ) 
			{
				if(this.Role.MountSkinName != null)
				{			
					return p.add(new Point(48,-75-this.MountHeight));
				}
				return p.add(new Point(48,-75));
			}
			else
			{
				if(this.Role.MountSkinName != null)
				{
					return p.add(new Point(-48,-75-this.MountHeight));
				}
				return p.add(new Point(-48,-75));
			}
		}
		
		public override function Dispose():void
		{			
			super.Dispose();
		}
	}
}  