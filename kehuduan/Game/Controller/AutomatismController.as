package Controller
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	
	
	public class AutomatismController
	{
		//自动释放技能冷却时间
		public static var AutomatismCoolTime:Number = 0;
		
		public static function AddHpSkill():void
		{
            if(AutoPlayData.aSaveNum[2] != null)
			{			
			    var limitHp:Number = uint(AutoPlayData.aSaveNum[2]) / 100;   
			
				//掉血丢自动释放技能
			    if(GameCommonData.Player.Role.HP/GameCommonData.Player.Role.MaxHp <= limitHp)
				{
					AutomatismSkill(true);
				}
			}
		}
		
		//给其他人释放技能
		public static function AddTeamSkill(teamID:int):void
		{	
			var now:Date = new Date();		
			//判断是否对队友使用技能
			if(GameCommonData.Player.IsAutomatism && AutoPlayData.aSaveTick[4] != null && AutoPlayData.aSaveTick[4] == 1)
			{
				if(AutoPlayData.aSaveNum[4] != null)
				{
					var targerPlayer:GameElementAnimal = PlayerController.GetPlayer(teamID);      
					var limitHp:Number = uint(AutoPlayData.aSaveNum[4]) / 100;   
					//掉血丢自动释放技能
			   		if(targerPlayer != null && targerPlayer.Role.HP/targerPlayer.Role.MaxHp <= limitHp)
			   		{
			   			//先查找单体治疗技能
	            		for(var skillName:String in GameCommonData.Player.Role.SkillList)
						{
							var skilllevel:GameSkillLevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
						    var state:int =	GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode);
							
							//改技能是否允许自动释放
							if(GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode) && GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode) && GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID )
							{
								if(DistanceController.PlayerTargetAnimalDistance(targerPlayer, skilllevel.gameSkill.Distance) && skilllevel.IsAutomatism && state == 3 && GameSkillMode.IsSingleDoctorSkill(skilllevel.gameSkill.SkillMode))						
								{
							 		 //自动释放技能蓝必须够 怒气必须够							     
								     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
								     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
								     && now.time - skilllevel.AutomatismUseTime > 1100)
								     {										 
                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
                                        GameCommonData.Scene.DistanceTargetUseSkill(skilllevel.gameSkill.Distance,targerPlayer);
                                        AutomatismCoolTime = now.time;
                                        return;			                                    	                                                       
            						 }                    										
								}
							}																				
						}
						
						//判断是否是DOT加血技能
					    for(skillName in GameCommonData.Player.Role.SkillList)
						{
						    skilllevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
						    state =	GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode);
							
							//改技能是否允许自动释放
							if(GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode) && GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode) && GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID )
							{
								if(skilllevel.IsAutomatism && DistanceController.PlayerTargetAnimalDistance(targerPlayer, skilllevel.gameSkill.Distance) && GameSkillMode.IsAddHp(skilllevel.gameSkill.SkillID)
								&& !targerPlayer.Role.IsBuff(skilllevel.gameSkill.Buff))						
								{
							 		 //自动释放技能蓝必须够 怒气必须够							     
								     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
								     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
								     && now.time - skilllevel.AutomatismUseTime > 1100)
								     {										 
                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
                                        GameCommonData.Scene.DistanceTargetUseSkill(skilllevel.gameSkill.Distance,targerPlayer);
                                        AutomatismCoolTime = now.time;
                                        return;			                                    	                                                       
            						 }                    										
								}
							}																				
						}
						
						//在查找群体释放技能
					    for(skillName in GameCommonData.Player.Role.SkillList)
						{
							skilllevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
							state =	GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode);
							//改技能是否允许自动释放
							if(GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode) && GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID )
							{
								if(skilllevel.IsAutomatism && state == 3 &&  !GameSkillMode.IsSingleDoctorSkill(skilllevel.gameSkill.SkillMode))							
								{									 										     
							        //自动释放技能蓝必须够 怒气必须够							     
								     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
								     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
								     && now.time - skilllevel.AutomatismUseTime > 1100)
								     {										 
                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
                                         GameCommonData.Scene.DistanceUseSkill(skilllevel.gameSkill.Distance);
                                        AutomatismCoolTime = now.time;
                                        return;			                                    	                                                       
            						 }                    										
								}
							}									
						}
			   		}
				}
			}
		}
		
		//获取可以攻击的技能编号
		public static function GetCanUseSkill():int
		{
			//要使用的技能编号
			var skillID:int = 0;	
		    var now:Date = new Date();
	    
		    if( now.time - CombatController.skillTime > 1000)
		    {
				for(var skillName:String in GameCommonData.Player.Role.SkillList)
				{
					var skilllevel:GameSkillLevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
									
					//改技能是否允许自动释放
					if(GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID)
					{
						if(GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode) == 1
						&& !GameSkillMode.IsAddHp(skilllevel.gameSkill.SkillID) && !GameSkillMode.IsAddMp(skilllevel.gameSkill.SkillID)
						 && ! GameSkillMode.IsPetBuffSkill(skilllevel.gameSkill.SkillMode))							
						{	
							//是否可以自动释放技能              技能冷却时间是否可用                              //追加1秒
								if(skilllevel.IsAutomatism && (now.time - (skilllevel.AutomatismUseTime)) > 0)
								{
									//攻击模式
									 skilllevel.AutomatismUseTime = now.time + 1000;			
								     //自动释放技能蓝必须够 怒气必须够				     
								     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP  
								     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
								     && GameCommonData.AttackAnimal != null 
								     && GameCommonData.AttackAnimal.Role.HP / GameCommonData.AttackAnimal.Role.MaxHp > 0.05)
								     {
										skillID = skilllevel.gameSkill.SkillID;
										return skillID;
								     }		
								}						
					    }
					}
				}
		    }
			return skillID;
		}
		
		
		
		//自动释放技能
		public static  function AutomatismSkill(IsDoctor:Boolean = false):Boolean
		{
			var IsUseSkill:Boolean = false;
			//当前时间
			var now:Date = new Date();
			
			//是否可以追加攻击                     治疗必追加攻击
			if(GameCommonData.Player.IsAddAttack || IsDoctor)
			{		
				//技能公共冷却时间CD 用于控制技能的连续释放时间 
				if(now.time - AutomatismCoolTime > 1000 || GameCommonData.Player.IsAutomatism)
	            {        	            		                	
	            	if(IsDoctor)
	            	{
						if(AutoPlayData.aSaveTick[2] != null && AutoPlayData.aSaveTick[2] == 1)
						{
		            		//先查找单体治疗技能
		            		for(var skillName:String in GameCommonData.Player.Role.SkillList)
							{
								var skilllevel:GameSkillLevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
							    var state:int =	GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode);
								
								//改技能是否允许自动释放
								if(GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID )
								{
									if(skilllevel.IsAutomatism && state == 3 && GameSkillMode.IsSingleDoctorSkill(skilllevel.gameSkill.SkillMode))						
									{
								 		 //自动释放技能蓝必须够 怒气必须够							     
									     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
									     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
									     && now.time - skilllevel.AutomatismUseTime > 1100)
									     {								 
	                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
	                                        GameCommonData.Scene.DistanceUseSkill(skilllevel.gameSkill.Distance);
	                                        AutomatismCoolTime = now.time;
	                                        return true;			                                    	                                                       
	            						 }                    										
									}
	
								}								
							}
							
							//在查找群体释放技能
						    for(skillName in GameCommonData.Player.Role.SkillList)
							{
								skilllevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
								state =	GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode);
								//改技能是否允许自动释放
								if(GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID )
								{
									if(skilllevel.IsAutomatism && state == 3 &&  !GameSkillMode.IsSingleDoctorSkill(skilllevel.gameSkill.SkillMode))							
									{									 										     
								        //自动释放技能蓝必须够 怒气必须够							     
									     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
									     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
									     && now.time - skilllevel.AutomatismUseTime > 1100)
									     {										 
	                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
	                                        GameCommonData.Scene.DistanceUseSkill(skilllevel.gameSkill.Distance);
	                                        AutomatismCoolTime = now.time;
	                                        return true;			                                    	                                                       
	            						 }                    										
									}
								}	
									
							}
						}
						
	            	}
	            	else
	            	{
						for(skillName in GameCommonData.Player.Role.SkillList)
						{
							skilllevel = GameCommonData.Player.Role.SkillList[skillName] as GameSkillLevel;
							
							//改技能是否允许自动释放
							if(GameSkillMode.IsPersonAutomatism(skilllevel.gameSkill.SkillMode) && skilllevel.gameSkill.Job == GameCommonData.Player.Role.CurrentJobID )
							{
								//是否可以自动释放技能              技能冷却时间是否可用                              //追加1秒
								if(skilllevel.IsAutomatism && (now.time - (skilllevel.AutomatismUseTime + 1100)) > 0)
								{
									//攻击模式 1 BUFF模式 2 治疗模式 3
								    state = GameSkillMode.GetAutomatismState(skilllevel.gameSkill.SkillMode);					

									switch(state)							
									{
										case 1:
										    //是否是加血的DOT 技能
										    if(GameSkillMode.IsAddHp(skilllevel.gameSkill.SkillID))
											{
												//判断是否勾选加血选项
												if(AutoPlayData.aSaveTick[2] != null && AutoPlayData.aSaveTick[2] == 1)
												{
													 if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
												     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP)
												     {														     	
											     	    if(AutoPlayData.aSaveNum[2] != null)
														{			
		   										 			var limitHp:Number = uint(AutoPlayData.aSaveNum[2]) / 100;   																											
														    if(GameCommonData.Player.Role.HP/GameCommonData.Player.Role.MaxHp <= limitHp)
															{
																GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;   
																PlayerController.UseSkill( skilllevel.gameSkill.SkillID);  
																AutomatismCoolTime = now.time;
															}
													   }
													 }
												}
											 }
											 else  if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP  
										     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
										     && GameCommonData.AttackAnimal != null 
										     && GameCommonData.AttackAnimal.Role.HP / GameCommonData.AttackAnimal.Role.MaxHp > 0.05)
										     {
										     	if(skilllevel.GetCoolTime < 5000)
										     	{
										     		if(now.time - skilllevel.AutomatismUseTime  > 1100)
										     		{
										     			skilllevel.AutomatismUseTime =  now.time;
										     			GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID; 								     	    
												     	GameCommonData.Scene.DistanceUseSkill(skilllevel.gameSkill.Distance);
												     	AutomatismCoolTime = now.time;  
												     	IsUseSkill = true;
										     		}
										     	} 
										     	else
										     	{
			    							     	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID; 								     	    
											     	GameCommonData.Scene.DistanceUseSkill(skilllevel.gameSkill.Distance);
											     	AutomatismCoolTime = now.time; 
											     	IsUseSkill = true;
										     	}
										     	return true;				
										     }								     								     				     								 
										     break;
										case 2:
	                                        //判断改buff 是否结束
											if(!GameCommonData.Player.Role.IsBuff(skilllevel.gameSkill.Buff))
											{
												if(GameSkillMode.IsAddMp(skilllevel.gameSkill.SkillID))
												{
													//判断是否勾选回蓝选项
													if(AutoPlayData.aSaveTick[3] != null && AutoPlayData.aSaveTick[3] == 1)
													{
														 if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
													     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP)
													     {		
													     	    if(AutoPlayData.aSaveNum[3] != null)
																{			
																	var limitMp:Number =  uint(AutoPlayData.aSaveNum[3]) / 100;  																													
																    if(GameCommonData.Player.Role.MP/GameCommonData.Player.Role.MaxMp <= limitMp)
																	{
																		GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;   
																		PlayerController.UseSkill( skilllevel.gameSkill.SkillID);  
																		AutomatismCoolTime = now.time;
																	}
																}
													     }
												   }
												}
												else if(GameSkillMode.IsAddHp(skilllevel.gameSkill.SkillID))
												{													
													//判断是否勾选加血选项
													if(AutoPlayData.aSaveTick[2] != null && AutoPlayData.aSaveTick[2] == 1)
													{												
														 if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
													     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP)
													     {		
													     	    if(AutoPlayData.aSaveNum[2] != null)
																{			
		   										 					limitHp = uint(AutoPlayData.aSaveNum[2]) / 100;   
																																				
																    if(GameCommonData.Player.Role.HP/GameCommonData.Player.Role.MaxHp <= limitHp)
																	{
																		GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;   
																		PlayerController.UseSkill( skilllevel.gameSkill.SkillID);  
																		AutomatismCoolTime = now.time;
																	}
																}														
													     }
												 	}
												}
												else
												{
													if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
												     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP)
												     {											     	
												        if(skilllevel.GetCoolTime < 5000)
											        	{
												     		if(now.time - skilllevel.AutomatismUseTime > 1100)
												     		{
												     			skilllevel.AutomatismUseTime =  now.time;
																GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;   
																PlayerController.UseSkill( skilllevel.gameSkill.SkillID);  
																AutomatismCoolTime = now.time; 
															    return true;
												     		}
											        	}
											        	else
											        	{
											        			GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;   
																PlayerController.UseSkill( skilllevel.gameSkill.SkillID);  
																AutomatismCoolTime = now.time; 
															    return true;
											        	}
													  }
												 }
											}
											
											if(AutoPlayData.aSaveTick[5] != null && AutoPlayData.aSaveTick[5] == 1
											&& GameCommonData.Player.IsAutomatism && GameCommonData.TeamPlayerList!=null)
											{
												if(!GameSkillMode.IsAddHp(skilllevel.gameSkill.SkillID) && !GameSkillMode.IsAddMp(skilllevel.gameSkill.SkillID) && GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode))
												{
									                //自己宠物
				                                	if(GameCommonData.Player.Role.UsingPetAnimal != null && !GameCommonData.Player.Role.UsingPetAnimal.Role.IsBuff(skilllevel.gameSkill.Buff))
				                                	{
				                                        if(DistanceController.PlayerTargetAnimalDistance(GameCommonData.Player.Role.UsingPetAnimal, skilllevel.gameSkill.Distance) && skilllevel.IsAutomatism)						
														{
													 		 //自动释放技能蓝必须够 怒气必须够							     
														     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
														     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
														     && now.time - skilllevel.AutomatismUseTime > 1000)
														     {										 
						                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
						                                        GameCommonData.Scene.DistanceTargetUseSkill(skilllevel.gameSkill.Distance,GameCommonData.Player.Role.UsingPetAnimal);
						                                        AutomatismCoolTime = now.time;
						                                        return true;			                                    	                                                       
						            						 }                    										
														}
				                                	}
			         							}
												
				                                for (var key:Object in GameCommonData.TeamPlayerList)
				                                { 
				                                	var targerPlayer:GameElementAnimal = PlayerController.GetPlayer(uint(key));  
				                                	
				                                	if(targerPlayer != null)    				
                                                    {                                                	                      	
				                                		if(!GameSkillMode.IsAddHp(skilllevel.gameSkill.SkillID) && !GameSkillMode.IsAddMp(skilllevel.gameSkill.SkillID) && GameSkillMode.UseTeamSkill(skilllevel.gameSkill.SkillMode))
					                                	{
					                                		//队友
					                                    	if(!targerPlayer.Role.IsBuff(skilllevel.gameSkill.Buff))
						                                	{
						                                		if(DistanceController.PlayerTargetAnimalDistance(targerPlayer, skilllevel.gameSkill.Distance) && skilllevel.IsAutomatism)						
																{
															 		 //自动释放技能蓝必须够 怒气必须够							     
																     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
																     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
																     && now.time - skilllevel.AutomatismUseTime > 1000)
																     {										 
								                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
								                                        GameCommonData.Scene.DistanceTargetUseSkill(skilllevel.gameSkill.Distance,targerPlayer);
								                                        AutomatismCoolTime = now.time;
								                                        return true;			                                    	                                                       								            						 
               	                                                     }
																}
						                                	}
						                                	
						                                	//宠物
						                                	if(targerPlayer.Role.UsingPetAnimal != null && !targerPlayer.Role.UsingPetAnimal.Role.IsBuff(skilllevel.gameSkill.Buff))
						                                	{
						                                		if(DistanceController.PlayerTargetAnimalDistance(targerPlayer.Role.UsingPetAnimal, skilllevel.gameSkill.Distance) && skilllevel.IsAutomatism)						
																{
															 		 //自动释放技能蓝必须够 怒气必须够							     
																     if(GameCommonData.Player.Role.MP >=  skilllevel.GetMP
																     && GameCommonData.Player.Role.SP >=  skilllevel.gameSkill.SP
																     && now.time - skilllevel.AutomatismUseTime > 1000)
																     {										 
								                                    	GameCommonData.Player.Role.DefaultSkill =  skilllevel.gameSkill.SkillID;                                  	 
								                                        GameCommonData.Scene.DistanceTargetUseSkill(skilllevel.gameSkill.Distance,targerPlayer.Role.UsingPetAnimal);
								                                        AutomatismCoolTime = now.time;
								                                        return true;			                                    	                                                       
								            						 }                    										
																}
						                                	}						                                	
					                                	}
                                                    }

				                                }
			        						}   										
											break;
									}
									
								}
							}				
						}
	            	}   	            	    	
	            }
			}
			
			return IsUseSkill;
		}
	}
}