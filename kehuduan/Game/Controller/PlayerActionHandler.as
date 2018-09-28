package Controller
{
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.Handler;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillBuff;
	import OopsEngine.Skill.GameSkillMode;
	import OopsEngine.Utils.Vector2;
	
	import flash.geom.Point;
	
	public class PlayerActionHandler extends Handler
	{
				
		public function PlayerActionHandler(animal:GameElementAnimal,next:Handler,skillEffectList:Array,skill:GameSkill,buff:GameSkillBuff,timeID:int,point:Point = null,process:int = 1)
		{			
			super(animal,next,skillEffectList,skill,buff,timeID,point,process);
		    this.Type = 1;
		}
		
		/**清空职责链*/
		public override function Clear():void
		{	
			if(this.InfoHandler != null)
			{
				if(this.Animal.Role.Type != GameRole.TYPE_PET)	
				{	
			        //判断是否显示技能名称			
					if(GameSkillMode.IsShowSkillName(SkillInfo.SkillMode))
					{
						CombatController.Skill(this.Animal,SkillInfo.SkillName);				
					}	
				}	
				this.InfoHandler.Clear(); 
			}
						
		    super.Clear();
		}
		
		/**运行职责链*/
		public override function Run():void
		{
			switch(this.Process)
			{ 
				case(1):
				    PlayerAction();
				    break;
			}
			super.Run();
		}
			
//	    /**添加击中爆炸效果*/
//		public  function AddStartEffect(point:Point):void
//		{
//			 if(SkillInfo.StartEffect.length > 0)
//		     {
//			     if(GameCommonData.SkillLoadEffectList[SkillInfo.StartEffect] == null)
//			     {
//			     	var gameSkillResource:GameSkillResource = new GameSkillResource();
//			     	gameSkillResource.SkillID      = SkillInfo.SkillID;
//			        gameSkillResource.EffectName   = SkillInfo.StartEffect;
//			        gameSkillResource.OnLoadEffect = PlayerSkillHandler.LoadEffect;
//			        gameSkillResource.EffectBR.LoadComplete = gameSkillResource.onEffectComplete;
//			        gameSkillResource.EffectBR.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF + SkillInfo.StartEffect+".swf");
//			        gameSkillResource.EffectBR.Load();
//			        GameCommonData.SkillLoadEffectList[SkillInfo.StartEffect] = SkillInfo.StartEffect;			     			     					        					        
//			     } 		  
//			     else if(GameCommonData.SkillOnLoadEffectList[SkillInfo.StartEffect] != null)
//			     {	  	   		
//				  	 var ongameSkillResource:GameSkillResource = GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] as GameSkillResource;				  	   	    	
//					 var startEffect:MovieClip = ongameSkillResource.EffectBR.GetResource(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF + SkillInfo.StartEffect+".swf").GetClassByMovieClip(SkillInfo.StartEffect);
//					 startEffect.x = point.x;
//					 startEffect.y = point.y;
//				     GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(startEffect);
//			     }	
//		      }
//		}
//	
		/**攻击者动作*/
		public  function  PlayerAction():void
		{ 			
            if(this.SkillInfo.SkillMode == 42)
            {
				if(this.Animal.Role.Type != GameRole.TYPE_ENEMY  && this.SkillInfo.SkillMode == 42)
				{
					this.Animal.handler.NextHendler();
					return;
				}
            }
										
			if(this.SkillEffectList != null && this.SkillEffectList.length == 1  &&  !GameSkillMode.IsPlay(this.SkillInfo.SkillMode))
			{
				var playerSkillHandler:PlayerSkillHandler;
				
				//判断对象是否存在
				if(GameCommonData.SameSecnePlayerList!=null && GameCommonData.Player != null)
				{
					if(GameCommonData.Player.Role.Id == this.SkillEffectList[0].TargerPlayer.Role.Id)
					{
						if(GameCommonData.Player.Role.ActionState ==  GameElementSkins.ACTION_DEAD)
						{				
				    		if(this.Animal.handler.InfoHandler != null)
							{
								if(this.Animal.handler.InfoHandler is PlayerSkillHandler)
								{
								    playerSkillHandler = this.Animal.handler.InfoHandler  as PlayerSkillHandler;
									playerSkillHandler.DeadPlayer();
								}
							}
							
							this.Animal.handler.NextHendler();
							return;
						}
					}
				    else
				    {
				    	if(GameCommonData.SameSecnePlayerList[this.SkillEffectList[0].TargerPlayer.Role.Id] == null)
				    	{
				    		if(this.Animal.handler.InfoHandler != null)
							{
								if(this.Animal.handler.InfoHandler is PlayerSkillHandler)
								{
								    playerSkillHandler = this.Animal.handler.InfoHandler  as PlayerSkillHandler;
									playerSkillHandler.DeadPlayer();
								}
							}
							
							this.Animal.handler.NextHendler();
							return;
				    	}
				    	else
				    	{
				    		if(GameCommonData.SameSecnePlayerList[this.SkillEffectList[0].TargerPlayer.Role.Id] != null && GameCommonData.SameSecnePlayerList[this.SkillEffectList[0].TargerPlayer.Role.Id].Role.ActionState == GameElementSkins.ACTION_DEAD)
				    		{
					    		if(this.Animal.handler.InfoHandler != null)
								{
									if(this.Animal.handler.InfoHandler is PlayerSkillHandler)
									{
										playerSkillHandler = this.Animal.handler.InfoHandler  as PlayerSkillHandler;
										playerSkillHandler.DeadPlayer();
									}
								}
								
								this.Animal.handler.NextHendler();
								return;
				    		}
				    	}
				    	
				    }
				}
			}
				
			//是否需要修改朝向 
			if(GameSkillMode.IsNoChangeDir(this.SkillInfo.SkillMode))
			{
				//同一点则 不处理了
				if(!(this.Animal.GameX == this.TargerPoint.x && this.Animal.GameY == this.TargerPoint.y))
				{
					// 获取设置发起人的人物朝向
				    var dir:int = Vector2.DirectionByTan(this.Animal.GameX,this.Animal.GameY,this.TargerPoint.x,this.TargerPoint.y);		
				    this.Animal.SetDirection(dir);		
			 	}
		 	}
			
           
            //判断是否是宠物
            if(this.Animal.Role.Type == GameRole.TYPE_PET)
            {      	
                if(this.SkillInfo.SkillMode != 12)
            	{       		
             		// 禁止人物移动
            		this.Animal.Stop();
            		// 将人物状态改为休闲模式   
           			 this.Animal.SetAction(GameElementSkins.ACTION_STATIC);  
					// 动作发起人做动作
					this.Animal.SetAction(GameElementSkins.ACTION_NEAR_ATTACK);
					
					//宠物普通攻击不走
					if(this.SkillInfo.SkillMode  == 19)	
					{
						var pet:GameElementPet = this.Animal as GameElementPet;
					 	pet.isWalk  = false;
					}
            			
            	}
//            	else
//            	{
//            		if(this.Animal.handler.InfoHandler != null)
//						this.Animal.handler.InfoHandler.Run();
//					
//            	}
            }
            else
            {
            	 // 禁止人物移动
            	if(this.Animal.Role.ActionState == GameElementSkins.ACTION_RUN && this.Animal.Role.HP > 0) 
            	{
	            	this.Animal.Stop();
	            	// 将人物状态改为休闲模式   
	           		this.Animal.SetAction(GameElementSkins.ACTION_STATIC);  
           		}

				this.Animal.SetAction(GameElementSkins.ACTION_NEAR_ATTACK);
            }
            
//            this.Animal.MustMovePoint = null;
			

//			AddStartEffect(new Point(this.Animal.HitX,this.Animal.HitY));
			
			if(this.Animal.Role.Type != GameRole.TYPE_PET)	
			{	
		        //判断是否显示技能名称			
				if(GameSkillMode.IsShowSkillName(SkillInfo.SkillMode))
				{
					CombatController.Skill(this.Animal,SkillInfo.SkillName);				
				}	
			}				
		}	
	}
}