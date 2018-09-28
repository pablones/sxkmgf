package Controller
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Skill.GameSkill;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	public class TargetController
	{	
		//已经选中的怪物列表
		public static var TargetList:Dictionary = new Dictionary();
		//挂机黑名单
		public static var DeadAnimal:Dictionary = new Dictionary();
		//挂机pk目标
		public static var TargetAutomatismID:int = 0;
		//是否选中了怪
		public static var IsTargetEnemy:Boolean = false;
		
		public static var SelectDis:int = 350;
		
		//挂机距离
		public static function Dis():int
		{		
			var dis:int = 5 * 35;
			
			if(GameCommonData.Player.IsAutomatism)
			{
				if(AutoPlayData.aSaveNum[7] != null)
				{
					var state :int = int(AutoPlayData.aSaveNum[7]);
					
					switch(state)
					{
						case 1 : 
						  dis =  5 * 35;
						  break;
						case 2 :
						  dis =  8 * 35;
						  break;  
						case 3 :
						  dis =  11 * 35;
						  break;
					}
				}
			}
			else
			{
				dis = SelectDis;
			}
			return dis;		
		}
		
		
		//是否在城里
		public static  function IsCity():Boolean
		{				
			  if(GameCommonData.GameInstance.GameScene.GetGameScene.MapId != "1001"     //江陵
	          && GameCommonData.GameInstance.GameScene.GetGameScene.MapId != "1034" 
	          && GameCommonData.GameInstance.GameScene.GetGameScene.MapId != "1036" 
	          && GameCommonData.GameInstance.GameScene.GetGameScene.MapId != "1038" 
	          && GameCommonData.GameInstance.GameScene.GetGameScene.MapId != "1040" 
	          && GameCommonData.GameInstance.GameScene.GetGameScene.MapId != "1006"     //开封
	          )
	          {
	           	 return false;
	          }
	          else
	          {
	          	 return true;
	          }
		}
		
		/**是否是PK场所**/
		public static function IsPKScene():Boolean
		{
			if(GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1042"
			|| GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1043"
			|| GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1044"
			|| GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1045"
			|| GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1046")
				return true;
			else
			    return false;
			
		}
		
		/** 是否是竞技场所 **/
		public static function IsPKTeam():Boolean
		{
			if(GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1049"
			|| GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1050"
			|| GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1051")
				return true;
			else
			    return false;
		}
		
    		
		/**设置目标 用于点击队伍人物的图标**/
		public static function SetTarget(target:GameElementAnimal):void
		{
			//清空当前目标
			if(GameCommonData.TargetAnimal != null)
			{
				GameCommonData.TargetAnimal.IsSelect(false);
			}			
			//切换目标			
			GameCommonData.TargetAnimal = target;
			GameCommonData.TargetAnimal.IsSelect(true);
		}	
		
		//获取宠物攻击对象
		public static function GetPetTarget():GameElementAnimal
		{
			//切换攻击目标的对象
			var targetAnimal:GameElementAnimal;
			
			//判断宠物的目标
			if(GameCommonData.PetTargetAnimal != null)
			{			
				if(PlayerController.IsUseSkill(GameCommonData.Player,GameCommonData.PetTargetAnimal))
				{
					targetAnimal = GameCommonData.PetTargetAnimal;
				}
			}
		
		    //切换人物的目标
			if(GameCommonData.AttackAnimal != null)
			{
				if(IsUseSkill(GameCommonData.Player,GameCommonData.AttackAnimal))
				{
					targetAnimal = GameCommonData.AttackAnimal;
				}
			}		
			return targetAnimal;
		}
		
		/**追踪任务怪**/
		public static function GetTaskTarget(EnemyTypeID:int,MovePoint:Point,IsAttack:Boolean = true):Boolean
		{
			var isattackTarget:Boolean = false;
			
			var targetList:Array = new Array();      	      	
	        for(var player:* in GameCommonData.SameSecnePlayerList)
			{
				var play:GameElementAnimal = GameCommonData.SameSecnePlayerList[player];
				if(( play.Role.HP > 0 && play.Role.Type == GameRole.TYPE_ENEMY && EnemyTypeID == 100000)   // 打全场景的怪
				||  (play.Role.idMonsterType == EnemyTypeID && play.Role.HP > 0)                           // 攻击指定的怪
				)
				{
					var dis:int = MapTileModel.Distance(GameCommonData.Player.GameX,GameCommonData.Player.GameY,play.GameX, play.GameY);												
					if(DistanceController.Distance(new Point(play.Role.TileX,play.Role.TileY),MapTileModel.GetTileStageToPoint(MovePoint.x,MovePoint.y),8))
					{							
						targetList.push({play: play, dis:dis});
					}
				}				
			}
						   		    							
			if(targetList.length > 0)
			{
				targetList.sortOn("dis",Array.NUMERIC);														
										
				var p:GameElementAnimal = targetList[0].play;
				
				if(GameCommonData.TargetAnimal != null)
				{
					GameCommonData.TargetAnimal.IsSelect(false);
				}						
				GameCommonData.TargetAnimal = targetList[0].play;
				GameCommonData.TargetAnimal.IsSelect(true);			
				UIFacade.UIFacadeInstance.selectPlayer();
				GameCommonData.Targettime	  = 0;	
				GameCommonData.AttackTargetID = GameCommonData.TargetAnimal.Role.Id;
				GameCommonData.Player.IsAttack  = true;
				if(IsAttack)
				{
					GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);		
				}				
				
				isattackTarget = true;
			}
			else
			{
				 isattackTarget = false;
			}
			
			return isattackTarget;
		}
		
		/**获取攻击目标**/
		public static function GetTarget(IsAdminister:Boolean = true):void
		{			  	 	
            IsTargetEnemy = false;       

           //优先选打玩家的怪
            var now:Date = new Date();       
            
            //挂机才选打自己的怪         
            if(GameCommonData.Player.IsAutomatism)
            {
	            for(var aggressorID:* in GameCommonData.Player.Role.Aggressor)
	            {
					//判断攻击者是否在5秒内对玩家发动攻击
					if(aggressorID != 0 && now.time - GameCommonData.Player.Role.Aggressor[aggressorID] <= 3000)
					{
						var aggressor:GameElementAnimal = GameCommonData.SameSecnePlayerList[aggressorID];
						
						//对象是否存在
						if(aggressor != null && aggressor.Role.HP > 0 && aggressor.Role.Type == GameRole.TYPE_ENEMY)
						{		
							//清空黑名单
							if(DeadAnimal[aggressorID] != null)
							{
								DeadAnimal[aggressorID] = null;
								delete DeadAnimal[aggressorID];
							}
							
							if(!GameCommonData.Scene.gameScenePlay.Map.IsPass(aggressor.Role.TileX,aggressor.Role.TileY))
							{
								if(AStarController.GetNearPoint(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),    //当前的点
							    new Point(aggressor.Role.TileX,aggressor.Role.TileY),true) != null )
								{
									//确定目标
							    	ConfirmTarget(aggressor,Dis() + 200);
								}
								else
								{
									//获取普通攻击距离
					            	var skill:GameSkill =  PlayerController.GetDefaultSkill(GameCommonData.Player);
					            	//判断是否可以打中
					            	if(DistanceController.PlayerTargetAnimalDistance(aggressor, skill.Distance))
					            	{
						            	//确定目标
									    ConfirmTarget(aggressor,Dis() + 200);
									    					    
									    //选中则撤销
									    if(IsTargetEnemy)
									    {
									    	return;
									    }
					            	}
				    			}
						 	}	
						 	else
						 	{					 		
						 		//确定目标
							    ConfirmTarget(aggressor,Dis() + 200);
							    					    
							    //选中则撤销
							    if(IsTargetEnemy)
							    {
							    	return;
							    }	
						 	}
						}
						else
						{
							GameCommonData.Player.Role.Aggressor[aggressorID] = null;
							delete GameCommonData.Player.Role.Aggressor[aggressorID];
						}
						
					}
					else
					{
						GameCommonData.Player.Role.Aggressor[aggressorID] = null;
						delete GameCommonData.Player.Role.Aggressor[aggressorID];
					}
	            }
            }
            
            if(GameCommonData.Player.IsAutomatism)
            {
	            //没有打玩家的怪则选别的怪
	            if(!IsTargetEnemy)
	            {
	            	var targetList:Array = new Array();      	      	
	            	for(var player:* in GameCommonData.SameSecnePlayerList)
					{
						var play:GameElementAnimal = GameCommonData.SameSecnePlayerList[player];
					    if(GameCommonData.Player.IsAutomatism && play.Role.Type == GameRole.TYPE_ENEMY
					    && play.Role.HP > 0)
						{
							//选怪的时候要判断是否在黑名单
							if(DeadAnimal[player] == null)
							{
								var dis:int = MapTileModel.Distance(GameCommonData.Player.GameX,GameCommonData.Player.GameY,play.GameX, play.GameY);									
								targetList.push({play: play, dis:dis});
							}
						}					
					}
					
					if(targetList.length > 0)
					{
						targetList.sortOn("dis",Array.NUMERIC);		
						for(var num:int=0;num < targetList.length;num++)
						{													
							var p:GameElementAnimal = targetList[num].play;
							
							//判断是否在选怪区域里面
							if(GameCommonData.AutomatismPoint != null && DistanceController.PlayerAutomatism(Dis(),new Point(p.GameX, p.GameY))) 
							{
								if(GameCommonData.TargetAnimal != null)
								{
									GameCommonData.TargetAnimal.IsSelect(false);
								}						
								GameCommonData.TargetAnimal = targetList[num].play;
								GameCommonData.TargetAnimal.IsSelect(true);			
								UIFacade.UIFacadeInstance.selectPlayer();								
								if(GameCommonData.Player.IsAutomatism)
								{
									TargetAutomatismID = targetList[num].play.Role.Id;
									//如果是挂机则对目标进行攻击
									if(GameCommonData.Player.IsAutomatism)
										GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);
											
									IsTargetEnemy = true;					
								}	
								return;
							}
						}
					}
					
	            }
	            
	            if(!IsTargetEnemy)
	            {
	            	DeadAnimal = new Dictionary();
	            	PlayerController.ClearAutomatism();
	            }
            }
            else
            {         
	            //没有打玩家的怪则选别的怪
	            if(!IsTargetEnemy)
	            {
		            //最近以2个开始 每次加2格
		            GetTargetDistance(67);
	            }
	            
	            //如果没有选中目标则重置数据
	            if(!IsTargetEnemy)
				{
					//清空怪物列表
					TargetList = new Dictionary();	
					//判断以免反复重置			
					if(IsAdminister)
					{
						GetTarget(false);
					}			
				}
            }
		}
		
		/**循环获取下个目标**/
		public static function GetTargetDistance(Distance:int):void
		{
			
			for(var player:* in GameCommonData.SameSecnePlayerList)
			{
				//判断这个怪是否被选中过
				if(TargetList[player] == null)
				{
					//获取同场景的玩家信息
					var play:GameElementAnimal = GameCommonData.SameSecnePlayerList[player];
					
					ConfirmTarget(play,Distance);
					
					if(IsTargetEnemy)
					{
						return;
					}
				}			
			}
			
			//增加2格距离
			Distance += 67; 
			
			//只选10格之内的怪
			if(Distance <= Dis())
			{
				GetTargetDistance(Distance);
			}			
		}
		
		/**确定目标**/                                                        
		public static function ConfirmTarget(play:GameElementAnimal,Distance:int):void
		{
		    //判断 是否可以攻击 
			if(play.Role.Type == GameRole.TYPE_ENEMY && play.Role.HP > 0)				
			{					 
				//如果不是挂机则在11格之内 如果是挂机则在所看范围内
				if((MapTileModel.Distance(GameCommonData.Player.GameX,GameCommonData.Player.GameY,play.GameX, play.GameY) <= Distance))				
				{
					if(GameCommonData.Player.IsAutomatism)
					{
						//如果是挂机状态则判断是否超过11格距离
						if(!(GameCommonData.AutomatismPoint != null  && DistanceController.PlayerAutomatism(Distance,new Point(play.GameX, play.GameY))))
						{
							return;
						}
					}
					
					if(DeadAnimal[play.Role.Id] == null
//					&& (GameCommonData.Scene.gameScenePlay.Map.IsPass(play.Role.TileX,play.Role.TileY)
//					&& GameCommonData.Player.Role.CurrentJobID == 1
//					&& GameCommonData.Player.Role.CurrentJobID == 8
//					&& GameCommonData.Player.Role.CurrentJobID == 16
//					&& GameCommonData.Player.Role.CurrentJobID == 4096
//					)
					) //不在黑名单里
					{
						if(GameCommonData.TargetAnimal != null)
						{
							GameCommonData.TargetAnimal.IsSelect(false);
						}						
						GameCommonData.TargetAnimal = play;
						GameCommonData.TargetAnimal.IsSelect(true);			
						UIFacade.UIFacadeInstance.selectPlayer();
						TargetList[play.Role.Id] = play.Role.Id;
												
						if(GameCommonData.Player.IsAutomatism)
						{
							TargetAutomatismID = play.Role.Id;
							//对目标进行攻击
							GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);	
						}								
						IsTargetEnemy = true;							
					}																	
				}
			}
		}

		public static function IsUseSkill(player:GameElementAnimal,targetAnimal:GameElementAnimal):Boolean
		{
			//是否可以使用技能
			var IsUseSkill:Boolean = false;
			var IsOwnerPet:Boolean = false;	
			
			//判断是否是自己的宠物	
			if(player.Role.UsingPetAnimal != null
			&& player.Role.UsingPetAnimal.Role.Id == targetAnimal.Role.Id)
			{
				IsOwnerPet = true;		
			}
			
			//不是自己的宠物
			if(!IsOwnerPet)
			{	
				if(player.Role.idTeam !=  targetAnimal.Role.idTeam               //不是一队的
			 	 || player.Role.idTeam==0)                                       //没组队
				 {
					 if(targetAnimal.Role.ActionState != GameElementSkins.ACTION_DEAD) //目标没有死亡
					 {
					 	  if(targetAnimal.Role.Type == GameRole.TYPE_PLAYER ||                        //判断是否是玩家 或者 怪物 
			 			  targetAnimal.Role.Type == GameRole.TYPE_ENEMY)
			 			  {
			 				  IsUseSkill = true;
			 			  }
					 }
		    	}					 
			}
			return IsUseSkill;
		}
	
	
	    /**是否可以攻击**/
	    public static function IsAttack(player:GameElementAnimal):Boolean
	    {
	    	var IsAttack:Boolean = false;
	    				    		
	    	if(GameCommonData.DuelAnimal == player.Role.Id)	
	    	{
	    		IsAttack = true;
	    	}
	    	else	    	   	
	    	{		
	    		if(IsPKTeam())
	    		{
	    			switch(player.Role.Type)
					{
						//怪物
						case GameRole.TYPE_ENEMY:		   
						     IsAttack = true;
						     break;
						//玩家
						case GameRole.TYPE_PLAYER:		              
					     	 if(player.Role.ActionState != GameElementSkins.ACTION_DEAD          					 //是否死亡
			   		          && player.Role.HP > 0)
			  		          {
				   			       if(GameCommonData.Player.Role.PKteam !=  player.Role.PKteam )
							       {
								      IsAttack = true;
								   }
							  }     
						     break;
						//宠物
						case GameRole.TYPE_PET:  
		     	      	   if(player.Role.ActionState != GameElementSkins.ACTION_DEAD           					 //是否死亡
		   		           && player.Role.HP > 0)
		  		           {
			   			        if(GameCommonData.Player.Role.PKteam != player.Role.MasterPlayer.Role.PKteam)
						        {
							       IsAttack = true;
							    }
						    }								     	      
						    break;   
						}
	    		}
	    		else
	    		{	  		
			    		 if(IsPKScene())
			    		{
			    			switch(player.Role.Type)
							{
								//怪物
								case GameRole.TYPE_ENEMY:		   
								     IsAttack = true;
								     break;
								//玩家
								case GameRole.TYPE_PLAYER:		              
							     	 if(player.Role.ActionState != GameElementSkins.ACTION_DEAD          					 //是否死亡
					   		          && player.Role.HP > 0)
					  		          {
						   			       if(GameCommonData.Player.Role.idTeam !=  player.Role.idTeam ||              		 //是否同1队              
										   GameCommonData.Player.Role.idTeam == 0)
									       {
										      IsAttack = true;
										   }
									  }     
								     break;
								//宠物
								case GameRole.TYPE_PET:  
				     	      	   if(player.Role.ActionState != GameElementSkins.ACTION_DEAD           					 //是否死亡
				   		           && player.Role.HP > 0)
				  		           {
					   			        if(GameCommonData.Player.Role.idTeam != player.Role.MasterPlayer.Role.idTeam ||      //是否同1队              
									    GameCommonData.Player.Role.idTeam==0)
								        {
									       IsAttack = true;
									    }
								    }								     	      
								    break;   
								}
			    		}   
			    		else
			    		{ 		
					    	//在城里则不能判断PK
				            if(!IsCity()) 
				            { 
								switch(player.Role.Type)
								{
									//怪物
									case GameRole.TYPE_ENEMY:		   
									     IsAttack = true;
									     break;
									//玩家
									case GameRole.TYPE_PLAYER:		              
									     switch(GameCommonData.Player.Role.PkState)   
									     {
									     	//和平模式
									     	case 0:break;
									     	//除恶模式
									     	case 1: 
									     	      if(player.Role.PkValue > 0 || player.Role.NameColor == "#fd70ff")
									     	      {
									     	      	  if(player.Role.ActionState != GameElementSkins.ACTION_DEAD           				 //是否死亡
									   		          && player.Role.HP > 0)
									  		          {
										   			       if(GameCommonData.Player.Role.idTeam !=  player.Role.idTeam ||                 //是否同1队              
														   GameCommonData.Player.Role.idTeam==0)
													       {
														      IsAttack = true;
														   }
													  }
									     	      }
									     	      break;   
									     	 //杀戮模式
									     	 case 2:
										     	 if(player.Role.ActionState != GameElementSkins.ACTION_DEAD          					 //是否死亡
								   		          && player.Role.HP > 0)
								  		          {
									   			       if(GameCommonData.Player.Role.idTeam !=  player.Role.idTeam ||              		 //是否同1队              
													   GameCommonData.Player.Role.idTeam == 0)
												       {
													      IsAttack = true;
													   }
												  }
												  break;
											  //帮派模式
										      case 3:
										          if(player.Role.unityId != GameCommonData.Player.Role.unityId 
										            || GameCommonData.Player.Role.unityId == 0)
										            {
									            	   if(GameCommonData.Player.Role.idTeam !=  player.Role.idTeam ||                 //是否同1队              
													   GameCommonData.Player.Role.idTeam==0)
												       {
													      IsAttack = true;
													   }
										            }	
										            break;			  
									     }        
									     break;
									//宠物
									case GameRole.TYPE_PET:  
									     //不是自己的宠物
									     if( !(GameCommonData.Player.Role.UsingPetAnimal != null 
									     && GameCommonData.Player.Role.UsingPetAnimal.Role.Id == player.Role.Id))
									     {  
									     	//宠物的主人必须存在
									     	if(player.Role.MasterPlayer != null)
									     	{
												 switch(GameCommonData.Player.Role.PkState)   
											     {
											     	//和平模式
											     	case 0:break;
											     	//除恶模式
											     	case 1: 
											     	      if(player.Role.MasterPlayer.Role.PkValue > 0 || player.Role.MasterPlayer.Role.NameColor == "#fd70ff")
											     	      {
											     	      	  if(player.Role.ActionState != GameElementSkins.ACTION_DEAD           								   //是否死亡
											   		          && player.Role.HP > 0)
											  		          {
												   			       if(GameCommonData.Player.Role.idTeam != player.Role.MasterPlayer.Role.idTeam ||                 //是否同1队              
																   GameCommonData.Player.Role.idTeam==0)
															       {
																      IsAttack = true;
																   }
															  }
											     	      }
											     	      break;   
											     	 //杀戮模式
											     	 case 2:
												     	 if(player.Role.ActionState != GameElementSkins.ACTION_DEAD           										//是否死亡
										   		          && player.Role.HP > 0)
										  		          {
											   			       if(GameCommonData.Player.Role.idTeam !=  player.Role.MasterPlayer.Role.idTeam ||                	 	//是否同1队              
															   GameCommonData.Player.Role.idTeam == 0)
														       {
															      IsAttack = true;
															   }
														  }
														  break;
												      case 3:
												          if(player.Role.MasterPlayer.Role.unityId != GameCommonData.Player.Role.unityId 
												            || GameCommonData.Player.Role.unityId == 0)
												            {
											            	   if(GameCommonData.Player.Role.idTeam !=  player.Role.MasterPlayer.Role.idTeam ||                 //是否同1队              
															   GameCommonData.Player.Role.idTeam==0)
														       {
															      IsAttack = true;
															   }
												            }	
												            break;			  
											     }  						     
										     }
									     }
									     break;	
								}
				            }
	     				}
	     		}
            }                  			
			return IsAttack;
	    }
	}
}