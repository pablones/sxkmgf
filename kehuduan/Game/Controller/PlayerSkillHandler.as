package Controller
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.command.AutoPlayItemsCommand;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.Handler;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillBuff;
	import OopsEngine.Skill.GameSkillEffect;
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	import OopsEngine.Skill.GameSkillResource;
	import OopsEngine.Skill.SkillAnimation;
	import OopsEngine.Utils.SmoothMove;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.setTimeout;
	
	public class PlayerSkillHandler extends  Handler
	{	
		/**清空职责链*/
		public override function Clear():void
		{
	  		if(SkillInfo != null)
	  		{
		  		switch(SkillInfo.SkillMode)
				{		
					//普通单体攻击	 （处理好了）
					case (1):
					//近战魔法单体攻击
					case (3):
					//背刺
					case (28):    
					    //播放爆炸效果
						AddPlayerHitEffect(SkillEffectList[0].TargerPlayer);
					    ShowAttackPrompt(this.SkillEffectList[0]);			    			
						break;
					
				    //击退
				    case (27):
					    //播放爆炸效果
						AddHitEffect(this.TargerPoint);		
					    ShowAttackPrompt(this.SkillEffectList[0]);		    
						break;
						
					//单体治疗
					case (32):
					case (18):
					case (5):
						for each(var gameSkillEffect:GameSkillEffect in this.SkillEffectList)
						{
							AddPlayerHitEffect(gameSkillEffect.TargerPlayer);
					    	ShowAttackPrompt(gameSkillEffect);
					    }				     
						break;
					
					//孙子
					case (33):
						AddHitEffect(new Point(this.Animal.GameX,this.Animal.GameY - this.Animal.height));
					    break;
					
					//指显示中文字
					case (19):	
					//宠物主动单攻
					case (14):
					//宠物自动释放技能
					case (11):
						for each(gameSkillEffect in this.SkillEffectList)
						{
					    	ShowAttackPrompt(gameSkillEffect);
					    }				     
						break;
				   
					//DOT 起始技能
					case (7):
					case (9):
					    //播放击中效果
					    AddPlayerHitEffect(SkillEffectList[0].TargerPlayer);
					    if(SkillEffectList.length > 0)
					    	ShowAttackPrompt(SkillEffectList[0]);		    
						break;			
					
					//远程普通单打
					case (2):		
					//逃避射击 （处理好了）
					case (31):	
					//远程普通单打
					case (4):
					//连环箭
					case (30):	  	
						var Effettime:int = 0
					  	for each(gameSkillEffect in this.SkillEffectList)
						{  		
	                      	setTimeout(AddEffect,Effettime, gameSkillEffect);	                          	
	                      	Effettime += 100;			       	    				         			    
						}          	    								
						break;	
						
					//刷CD （以处理）
					case (23):	
					//BUFF单体技能只能是自己
					case (20):
					//BUFF单体技能对所有玩家
					case (6):
					    for each(gameSkillEffect in this.SkillEffectList)
						{
							AddPlayerHitEffect(gameSkillEffect.TargerPlayer);
					    }         
				        break;	
	                
					//群自身 （以处理）
					case (26):
					case (16):
					case (13):
					    AddPlayerHitEffect(this.Animal);
					   	for each(gameSkillEffect in this.SkillEffectList)
					    {
					    	ShowAttackPrompt(gameSkillEffect);
					    }	
					    break;
				   			   		   	
				   	//群选地
				    case (17):
					case (15):
					   AddHitEffect(this.TargerPoint);
					   for each(gameSkillEffect in this.SkillEffectList)
					   {
					    	ShowAttackPrompt(gameSkillEffect);
					   }				   
					   break;	
					
					//群选人
					case (21): 
					   AddHitEffect(this.TargerPoint);
					   for each(gameSkillEffect in this.SkillEffectList)
					   {
					    	ShowAttackPrompt(gameSkillEffect);
					   }	
					   break;	   		
					 
					//瞬移
					case (22): 
					   AddHitEffect(this.TargerPoint);
					   break;	
				    
				    //无敌斩 
					case (29):
					   //执行播放动画	 
					   for each(gameSkillEffect in this.SkillEffectList)
					   {
					   		LightningSkill(gameSkillEffect);
					   }
					   break; 
					 
					// 全屏技能
					case (40):  
					   var pointList:Array = new Array();
				       var check:Point = new Point(0,-50);
					   pointList.push(new Point(this.Animal.GameX + 225 + check.x,this.Animal.GameY + check.y));
					   pointList.push(new Point(this.Animal.GameX + check.x,this.Animal.GameY +check.y + 160));
					   pointList.push(new Point(this.Animal.GameX + check.x,this.Animal.GameY +check.y - 160));
					   pointList.push(new Point(this.Animal.GameX - 225 + check.x,this.Animal.GameY +check.y ));
					   pointList.push(new Point(this.Animal.GameX - 145 + check.x,this.Animal.GameY +check.y -137));
					   pointList.push(new Point(this.Animal.GameX + 145 + check.x,this.Animal.GameY +check.y + 137));
					   pointList.push(new Point(this.Animal.GameX + 145 + check.x,this.Animal.GameY +check.y - 137));
					   pointList.push(new Point(this.Animal.GameX - 145 + check.x,this.Animal.GameY +check.y + 137));
					   pointList.push(new Point(this.Animal.GameX + 80 + check.x,this.Animal.GameY +check.y + 260));
					   pointList.push(new Point(this.Animal.GameX + 80 + check.x,this.Animal.GameY +check.y - 260));
					   pointList.push(new Point(this.Animal.GameX - 80 + check.x,this.Animal.GameY +check.y + 260));
					   pointList.push(new Point(this.Animal.GameX - 80 + check.x,this.Animal.GameY +check.y - 260));
					   pointList.push(new Point(this.Animal.GameX + 265 + check.x,this.Animal.GameY +check.y + 225));
					   pointList.push(new Point(this.Animal.GameX + 265 + check.x,this.Animal.GameY +check.y - 225));
					   pointList.push(new Point(this.Animal.GameX - 265 + check.x,this.Animal.GameY +check.y + 225));
					   pointList.push(new Point(this.Animal.GameX - 265 + check.x,this.Animal.GameY +check.y - 225));
					   pointList.push(new Point(this.Animal.GameX + 370 + check.x,this.Animal.GameY +check.y + 95));
					   pointList.push(new Point(this.Animal.GameX + 370 + check.x,this.Animal.GameY +check.y - 95));
					   pointList.push(new Point(this.Animal.GameX - 370 + check.x,this.Animal.GameY +check.y + 95));
					   pointList.push(new Point(this.Animal.GameX - 370 + check.x,this.Animal.GameY +check.y - 95));
					   var length:int = pointList.length;
					   for(var n:int = 0;n < length; n++)
					   {
					   		AddHitEffect(pointList[n]);
					   }
					   
					   for each(gameSkillEffect in this.SkillEffectList)
					   {
					    	ShowAttackPrompt(gameSkillEffect);
					   }		
					   break;
					   
				    case (41):
				       for each(gameSkillEffect in this.SkillEffectList)
					   {
					    	ShowAttackPrompt(gameSkillEffect);
					   }	
				       break;
				}
				
                DitherSkill();		
                SkillTalk();	
		    }
		    else
		    {
		    	for each(gameSkillEffect in this.SkillEffectList)
				{
					if(GameCommonData.TargetAnimal != null && gameSkillEffect.TargerPlayer.Role.Id == GameCommonData.TargetAnimal.Role.Id)
		  			{				
			    		ShowAttackPrompt(gameSkillEffect);
			    	}
				}
		    }
			super.Clear();
		}
		
		public function SkillTalk():void
		{
			if(GameCommonData.BossTalk[this.Animal.Role.PersonSkinID] != null)
			{
				if(GameSkillMode.IsCommon(this.SkillInfo.SkillMode))
				{
					if(this.Animal.Role.HP == this.Animal.Role.MaxHp)
					{
						TalkController.BossTalk(1,this.Animal);
					}
				}
				else
				{
					if(this.Animal.Role.HP == this.Animal.Role.MaxHp
					&& GameCommonData.Scene.begin == true)
					{
						TalkController.BossTalk(1,this.Animal);
					}
					else
					{
						TalkController.BossTalk(2,this.Animal,this.SkillInfo.SkillID);
					}
				}
			}	
		}
		
		/**技能抖动效果**/
		public function DitherSkill():void
		{
			 //判断是否是宠物的技能
			if(GameSkillMode.IsPetSkill(this.SkillInfo.SkillMode))
			{
				//判断是否可以抖动
				if(GameSkillMode.IsDither(this.SkillInfo.SkillID))
				{
					//是否是自己的宠物释放的技能
					if(GameCommonData.Player.Role.UsingPetAnimal != null
					&& GameCommonData.Player.Role.UsingPetAnimal.Role.Id == this.Animal.Role.Id)
					{
						//抖动频率是否可以抖动
						if(GameCommonData.Scene.gameScenePlay.Dither == 12)
						{
							GameCommonData.Scene.gameScenePlay.Dither = 0;
						}
					}
				}
			} 
			else
			{
				//判断是否可以抖动
				if(GameSkillMode.IsDither(this.SkillInfo.SkillID))
				{
					//抖动频率是否可以抖动
					if(GameCommonData.Scene.gameScenePlay.Dither == 12)
					{
						GameCommonData.Scene.gameScenePlay.Dither = 0;
					}
				}
			}
		}
		
		/**执行职责链*/	
		public override function Run():void
		{	
			switch(this.Process)
			{				
				//添加技能
				case (1):
				    AddSkill();	    
				    break;
				//DOT技能
				case (3):
				    DotSkill();
				    break;
			}				
		}
		
		public  function DeadPlayer():void
		{		
			for each(var gameSkillEffect:GameSkillEffect in this.SkillEffectList)
			{			
				//目标死亡
				if(gameSkillEffect.TargerPlayer.Role.HP <= 0 && gameSkillEffect.TargerPlayer.Role.ActionState != GameElementSkins.ACTION_DEAD)
				{
					ShowDead(gameSkillEffect);
				}		
			}	
		}
		
		//删除地面效果
		public static function MustDeleteHeffect(animationSkill:SkillAnimation):void
		{
			try
        	{
	            //移除飞行物体		
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.removeChild(animationSkill);	
				animationSkill.IsPlayComplete = true;		
			    animationSkill                = null;
        	}
        	catch(e:Error)
        	{
        		
        	}
		}
		
		public static function LoadEffect(gameSkillResource:GameSkillResource):void
		{
			GameCommonData.SkillOnLoadEffectList[gameSkillResource.EffectName] = gameSkillResource;
		}
		
		/**缓存攻击
		 * 攻击对象**/
		public function CacheAttack(targerPlayer:GameElementAnimal):void
		{
			if(GameCommonData.TargetAnimal == null)
			{
			     GameCommonData.TargetAnimal = targerPlayer;	
				 GameCommonData.TargetAnimal.IsSelect(true);					
				 UIFacade.UIFacadeInstance.selectPlayer();
				 GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);
			}
		}
		
		
		public function AddAttack():void
		{
			//攻击者是否是自己    并且必须选中目标   当时不为普通攻击者接普通攻击
	        if(this.Animal.Role.Id == GameCommonData.Player.Role.Id && GameSkillMode.TargetState(SkillInfo.SkillMode) == 1
	        && ! GameSkillMode.IsCommon(SkillInfo.SkillMode))                                       
		    {
		    	if(GameSkillMode.IsAddTarget(SkillInfo.SkillMode)) 
		    	{
		    		//击退目标延迟处理
	            	var t:Date = new Date();
	            	GameCommonData.Targettime = t.time;
		    		
		    		setTimeout(CacheAttack,600,this.SkillEffectList[0].TargerPlayer);	
		    	}
		    	else
		    	{
		    		if(GameCommonData.Player.IsAutomatism && GameCommonData.IsAttack == false)
		    		{
		    			if(GameCommonData.TargetAnimal != null)
							GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);
		    		}
		    		else
		    		{		    		
			    		if(GameCommonData.AttackAnimal != null)
							GameCommonData.Scene.Attack(GameCommonData.AttackAnimal);
		    		}
		    	}
		    }
			    
			//普功释放自动使用技能    并且普攻是否被移动取消
            if(GameCommonData.Player.IsAddAttack && this.Animal.Role.Id == GameCommonData.Player.Role.Id)
			{    
			   //攻击者是自己并且使用的是普通攻击   
			   if(this.Animal.Role.Id == GameCommonData.Player.Role.Id &&  GameSkillMode.IsCommon(SkillInfo.SkillMode))
			   {
	                //追加自动释放技能
	                AutomatismController.AutomatismSkill();
			   }
			   
			   //挂机页面需要选择宠物自动释放技能
			   if(AutoPlayData.aSaveTick[9] != null && AutoPlayData.aSaveTick[9] == 1 && GameCommonData.Player.IsAutomatism && this.Animal.Role.Id == GameCommonData.Player.Role.Id &&
			    this.Animal.Role.UsingPetAnimal != null  && GameCommonData.Player.Role.UsingPet != null 
			     &&  GameCommonData.Player.Role.UsingPet.SkillLevel != null)
			    {
			    	var date:Date = new Date();
			    	
			    	//查找是否宠物拥有自动释放的技能
			      	for(var n:int=0;n<GameCommonData.Player.Role.UsingPet.SkillLevel.length;n++)
					{					
						if(!GameCommonData.Player.Role.UsingPet.SkillLevel[n]) continue;
						var skill:GameSkillLevel = GameCommonData.Player.Role.UsingPet.SkillLevel[n];
						
						if(skill.AutomatismUseTime < date.time)
						{
							var state:int =  GameSkillMode.Affect(skill.gameSkill.SkillMode);       	
						    var combat:CombatController = new CombatController();							
			           		switch(state)
							{
							    case 2:
								     combat.ReservePetAffectSkillAttack(skill.gameSkill.SkillID,skill.Level);
							         break;
							   	case 3:
							   	     if(GameCommonData.TargetAnimal != null)
								     	combat.ReservePetPointAffectSkillAttack(skill.gameSkill.SkillID,skill.Level,new Point(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY));
							         break;   						      
							}	
						}
					}
			    }
		   }
		}
		
		/**添加技能*/
		public function  AddSkill():void
		{ 		
			switch(SkillInfo.SkillMode)
			{		
				//普通单体攻击	 （处理好了）
				case (1):
				//近战魔法单体攻击
				case (3):
				//背刺
				case (28):    
				    //播放爆炸效果
					AddPlayerHitEffect(SkillEffectList[0].TargerPlayer);
				    ShowAttackPrompt(this.SkillEffectList[0]);			    			
					break;
				
			    //击退
			    case (27):
				    //播放爆炸效果
					AddHitEffect(this.TargerPoint);		
				    ShowAttackPrompt(this.SkillEffectList[0]);		    
					break;
					
				//单体治疗
				case (32):
				case (18):
				case (5):
					for each(var gameSkillEffect:GameSkillEffect in this.SkillEffectList)
					{
						AddPlayerHitEffect(gameSkillEffect.TargerPlayer);
				    	ShowAttackPrompt(gameSkillEffect);
				    }				     
					break;
					
				//孙子
				case (33):
					AddHitEffect(new Point(this.Animal.GameX,this.Animal.GameY - this.Animal.height));
				    break;	
				
				//指显示中文字
				case (19):	
				//宠物主动单攻
				case (14):
				//宠物自动释放技能
				case (11):
					for each(gameSkillEffect in this.SkillEffectList)
					{
				    	ShowAttackPrompt(gameSkillEffect);
				    }				     
					break;
			   
				//DOT 起始技能
				case (7):
				case (9):
				    //播放击中效果
				    AddPlayerHitEffect(SkillEffectList[0].TargerPlayer);
				    if(this.SkillEffectList.length > 0)	
				   		 ShowAttackPrompt(SkillEffectList[0]);	    
					break;			
						
				//逃避射击 （处理好了）
				case (31):	
				//远程普通单打
				case (2):
				//远程普通单打
				case (4):
				//连环箭
				case (30):	  	
				    var Effettime:int = 0
				  	for each(gameSkillEffect in this.SkillEffectList)
					{  		
                      	setTimeout(AddEffect,Effettime, gameSkillEffect);	                          	
                      	Effettime += 100;			       	    				         			    
					}      	    								
				break;							    
					
				//刷CD （以处理）
				case (23):	
				//BUFF单体技能只能是自己
				case (20):
				//BUFF单体技能对所有玩家
				case (6):
				    for each(gameSkillEffect in this.SkillEffectList)
					{
						AddPlayerHitEffect(gameSkillEffect.TargerPlayer);
				    }         
			        break;	

				//群自身 （以处理）
				case (26):
				case (16):
				case (13):
				    AddPlayerHitEffect(this.Animal);
				   	for each(gameSkillEffect in this.SkillEffectList)
				    {
				    	ShowAttackPrompt(gameSkillEffect);
				    }	
				    break;
			   			   		   	
			   	//群选地
			    case (17):
				case (15):
				   AddHitEffect(this.TargerPoint);
				   for each(gameSkillEffect in this.SkillEffectList)
				   {
				    	ShowAttackPrompt(gameSkillEffect);
				   }				   
				   break;	
				
				//群选人
				case (21): 
				   AddHitEffect(this.TargerPoint);
				   for each(gameSkillEffect in this.SkillEffectList)
				   {
				    	ShowAttackPrompt(gameSkillEffect);
				   }	
				   break;	   		
				 
				//瞬移
				case (22): 
				   AddHitEffect(this.TargerPoint);
				   break;	
			    
			    //无敌斩 
				case (29):
				   //执行播放动画	 
				   for each(gameSkillEffect in this.SkillEffectList)
				   {
				   		LightningSkill(gameSkillEffect);
				   }
				   break;   
				
			    case (40):  
				   var pointList:Array = new Array();
				   var check:Point = new Point(0,-50);
				   pointList.push(new Point(this.Animal.GameX + 225 + check.x,this.Animal.GameY + check.y));
				   pointList.push(new Point(this.Animal.GameX + check.x,this.Animal.GameY +check.y + 160));
				   pointList.push(new Point(this.Animal.GameX + check.x,this.Animal.GameY +check.y - 160));
				   pointList.push(new Point(this.Animal.GameX - 225 + check.x,this.Animal.GameY +check.y ));
				   pointList.push(new Point(this.Animal.GameX - 145 + check.x,this.Animal.GameY +check.y -137));
				   pointList.push(new Point(this.Animal.GameX + 145 + check.x,this.Animal.GameY +check.y + 137));
				   pointList.push(new Point(this.Animal.GameX + 145 + check.x,this.Animal.GameY +check.y - 137));
				   pointList.push(new Point(this.Animal.GameX - 145 + check.x,this.Animal.GameY +check.y + 137));
				   pointList.push(new Point(this.Animal.GameX + 80 + check.x,this.Animal.GameY +check.y + 260));
				   pointList.push(new Point(this.Animal.GameX + 80 + check.x,this.Animal.GameY +check.y - 260));
				   pointList.push(new Point(this.Animal.GameX - 80 + check.x,this.Animal.GameY +check.y + 260));
				   pointList.push(new Point(this.Animal.GameX - 80 + check.x,this.Animal.GameY +check.y - 260));
				   pointList.push(new Point(this.Animal.GameX + 265 + check.x,this.Animal.GameY +check.y + 225));
				   pointList.push(new Point(this.Animal.GameX + 265 + check.x,this.Animal.GameY +check.y - 225));
				   pointList.push(new Point(this.Animal.GameX - 265 + check.x,this.Animal.GameY +check.y + 225));
				   pointList.push(new Point(this.Animal.GameX - 265 + check.x,this.Animal.GameY +check.y - 225));
				   pointList.push(new Point(this.Animal.GameX + 370 + check.x,this.Animal.GameY +check.y + 95));
				   pointList.push(new Point(this.Animal.GameX + 370 + check.x,this.Animal.GameY +check.y - 95));
				   pointList.push(new Point(this.Animal.GameX - 370 + check.x,this.Animal.GameY +check.y + 95));
				   pointList.push(new Point(this.Animal.GameX - 370 + check.x,this.Animal.GameY +check.y - 95));
				   var length:int = pointList.length;
				   for(var n:int = 0;n < length; n++)
				   {
				   		AddHitEffect(pointList[n]);
				   }				   
				   for each(gameSkillEffect in this.SkillEffectList)
				   {
				    	ShowAttackPrompt(gameSkillEffect);
				   }		
				   break;	
				   
			   	case (41):
		           for each(gameSkillEffect in this.SkillEffectList)
			       {
			    		ShowAttackPrompt(gameSkillEffect);
			  	   }	
		           break;	   
				   
				case 42:
				   if(this.Animal.Role.Type == GameRole.TYPE_ENEMY)
				   {
				   	    for each(gameSkillEffect in this.SkillEffectList)
						{  		
	                      	setTimeout(AddEffect,Effettime, gameSkillEffect);	                          	
	                      	Effettime += 100;			       	    				         			    
						}  
				   }
				   else
				   {
				   		AddPlayerHitEffect(this.Animal);
					   	for each(gameSkillEffect in this.SkillEffectList)
					    {
					    	ShowAttackPrompt(gameSkillEffect);
					    }	
				   }

				   break;		  		
			}				
			DitherSkill();
			SkillTalk();	
			
//			//屏幕抖动
//			if(this.Animal.Role.Id == GameCommonData.Player.Role.Id && GameSkillMode.IsDither(this.SkillInfo.SkillID) && GameCommonData.Scene.gameScenePlay.Dither == 13)
//			{
//				GameCommonData.Scene.gameScenePlay.Dither = 0;
//			}
			
//			if(this.Animal.Role.Id == GameCommonData.Player.Role.Id  && GameSkillMode.IsCommon(this.SkillInfo.SkillMode))
//			{
//				if(SkillEffectList[0].TargerPlayer.Role.Type == GameRole.TYPE_ENEMY)
//				{	
////					if(SkillEffectList[0].gameSkillEffect.TargerState == )
////					
//				    var enemy:GameElementEnemy =  SkillEffectList[0].TargerPlayer as GameElementEnemy;
//				    if(enemy.Dither >= 2)
//				   	   enemy.Dither = 0;
//				}
//			}
			
			//追加攻击
			AddAttack(); 			
		}

        public function MustDelete(gameSkillEffect:GameSkillEffect,effect:Sprite):void
        {
        	try
        	{
	            //移除飞行物体		
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.removeChild(effect);			
			    gameSkillEffect.smoothMove  =  null;
			    gameSkillEffect.Effect      =  null;	 
			    gameSkillEffect.IsDelete    =  true;
			    DeadPlayer();
        	}
        	catch(e:Error)
        	{
        		
        	}
        }
		

	    /**添加飞行动画**/
		public function AddEffect(gameSkillEffect:GameSkillEffect):void
		{
  		    //初始化远程技能飞行图片
	       if(GameCommonData.SkillLoadEffectList[SkillInfo.Effect] == null)
	       {
	       	    var gameSkillResource:GameSkillResource = new GameSkillResource();
	       	    gameSkillResource.SkillID      = SkillInfo.SkillID;
		        gameSkillResource.EffectName   = SkillInfo.Effect;
		        gameSkillResource.OnLoadEffect = PlayerSkillHandler.LoadEffect;
		        gameSkillResource.EffectPath   = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF + SkillInfo.Effect+".swf";
		        gameSkillResource.EffectBR.LoadComplete = gameSkillResource.onEffectComplete;
		        gameSkillResource.EffectBR.Download.Add(gameSkillResource.EffectPath);
		        gameSkillResource.EffectBR.Load();
		        GameCommonData.SkillLoadEffectList[SkillInfo.Effect] = SkillInfo.Effect;
		     	ShowAttackPrompt(gameSkillEffect);				       				        			        		        					        
	       } 		  
	       else if (GameCommonData.SkillOnLoadEffectList[SkillInfo.Effect] != null)
	       {
	       	  	//设置平滑移动		
	       	   var ongameSkillResource:GameSkillResource = GameCommonData.SkillOnLoadEffectList[SkillInfo.Effect] as GameSkillResource;					       	   		
		  	   var animationSkill:SkillAnimation = ongameSkillResource.GetAnimation();
		  	   var effect:Sprite = new Sprite(); 		  	    
		  	   animationSkill.StartClip("jn");	  
               effect.addChild(animationSkill);
               effect.x = this.Animal.HitX;
		  	   effect.y = this.Animal.HitY;	
		  	   effect.mouseEnabled = false;
		  	   
		  	   if(GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer != null)
		  	   {
			  	   GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(effect);
			  	   Orientation(new Point(gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY),effect);
			  	   var smoothMove:SmoothMove;	    		  	   
			  	   //设置平滑移动				  	    	
			  	   if((this.Animal.handler != null && this.Animal.handler.Floor > 5) || this.Animal.QuickPlay)
			  	   {
				   	   smoothMove  = new SmoothMove(effect,90);
			  	   }
			  	   else
			  	   {
			  	   	   smoothMove  = new SmoothMove(effect,30);
			  	   }
				   gameSkillEffect.smoothMove = smoothMove;	
			       gameSkillEffect.Effect     = effect;   	    	   
			       smoothMove.Move([new Point(gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY)]);					       	    
			       gameSkillEffect.onSkillFly           = SkillFly;
			       this.Animal.skillEffectList.push(gameSkillEffect);
			       setTimeout(MustDelete,4000,gameSkillEffect,effect);
		      }
		      else
		      {
		      		ShowAttackPrompt(gameSkillEffect);	
		      }
	       }
	       else
	       {
	       	   ShowAttackPrompt(gameSkillEffect);	
	       }				
		}
		
		/**闪电链**/
	    public function LightningSkill(gameSkillEffect:GameSkillEffect):void
	    {
	    	ShowAttackPrompt(gameSkillEffect);
	    }
	
		/**技能飞行图片坐标点移动改变*/
		public function SkillFly(gameSkillEffect:GameSkillEffect):Boolean
		{
			//判断是否击中处理过
			if(gameSkillEffect.smoothMove != null)
			{					
				//修改移动轨迹
				gameSkillEffect.smoothMove.Move([new Point(gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY)]);
	    		//修改飞行物体方向
	    		Orientation(new Point(gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY),gameSkillEffect.Effect);
	    		
	    		//设定区域范围
	    		var rect:Rectangle = new Rectangle(gameSkillEffect.TargerPlayer.HitX - 25,gameSkillEffect.TargerPlayer.HitY - 25,50,50 );
	    		//判断是否击中
	    		if(rect.containsPoint(new Point(gameSkillEffect.Effect.x,gameSkillEffect.Effect.y)))
				{ 
					// 击中未必需要爆炸
					if(this.SkillInfo.SkillMode != 42)
					{
						//爆炸效果
						SkillArrive(gameSkillEffect);
					}
					else
					{
						try
						{
							//移除飞行物体		
							GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.removeChild(gameSkillEffect.Effect);			
						    gameSkillEffect.smoothMove  =  null;
						    gameSkillEffect.Effect      =  null;	    
					 	}	
					 	catch(e:Error)
					 	{
					 		
					 	}
					}
					return true;
				}
			}
			return false;
		}
		
	    /**Dot技能处理*/
	    public function	DotSkill():void
	    {
	    	//也许没有击中对象
	    	if(SkillEffectList != null)
	    	{
	    		//对所有被击者掉血 
		    	for each(var gameSkillEffect:GameSkillEffect in this.SkillEffectList)
				{
					ShowAttackPrompt(gameSkillEffect);
				}
	    	}
	    	
			super.NextHendler();
			if(this.Animal.handler != null)
			   this.Animal.handler.Run();
	    }
		
		/** 击中处理
		 *  gameSkillEffect          目标对象信息
		 * */
		public function SkillArrive(gameSkillEffect:GameSkillEffect):void
		{		
			try
			{
				//移除飞行物体		
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.removeChild(gameSkillEffect.Effect);			
			    gameSkillEffect.smoothMove  =  null;
			    gameSkillEffect.Effect      =  null;	    
		 	}	
		 	catch(e:Error)
		 	{
		 		
		 	}	
		    		    
		    //播放爆炸效果
		    AddPlayerHitEffect(this.SkillEffectList[0].TargerPlayer);  
		    		    
		    //掉血
			ShowAttackPrompt(gameSkillEffect);	
		    
		    gameSkillEffect.EffectState = 2;
		    			
			//判断是否都击中（多重箭判断）
			for each(var gameSkillEffect:GameSkillEffect in this.SkillEffectList)
			{		
				if(gameSkillEffect.EffectState != 2)
			    {
					return;
				}
			}		
		}
		
		/**移除人身上的技能效果**/
		public function PlayerPlayComplete(animationSkill:SkillAnimation):void
		{
		    if(animationSkill != null && animationSkill.player.contains(animationSkill))
		    {
		    	animationSkill.player.removeChild(animationSkill);
				animationSkill.IsPlayComplete = true;
				animationSkill = null;
		    }
		}
		
		/**移除人场景上的技能效果**/
		public function ScenePlayComplete(animationSkill:SkillAnimation):void
		{
			if(animationSkill != null && animationSkill.gameScene.contains(animationSkill))
		    {				
				animationSkill.gameScene.removeChild(animationSkill);
				animationSkill = null;
		    }
		}
		
		/**添加自身击中爆炸效果*/
		public  function AddPlayerHitEffect(player:GameElementAnimal):void
		{
			//被击打者 皮肤是否加载完成
			if(player.IsLoadSkins && player.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				 if(SkillInfo.HitEffect.length > 0)
			     {     	
			     	if(GameCommonData.SkillLoadEffectList[SkillInfo.HitEffect] == null)
			        {
	        			var gameSkillResource:GameSkillResource = new GameSkillResource();
	        			gameSkillResource.SkillID      = SkillInfo.SkillID;
				        gameSkillResource.EffectName   = SkillInfo.HitEffect;
				        gameSkillResource.EffectPath   = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF + SkillInfo.HitEffect+".swf";
				        gameSkillResource.OnLoadEffect = PlayerSkillHandler.LoadEffect;
				        gameSkillResource.EffectBR.LoadComplete = gameSkillResource.onEffectComplete;
				        gameSkillResource.EffectBR.Download.Add(gameSkillResource.EffectPath);
				        gameSkillResource.EffectBR.Load();
				        GameCommonData.SkillLoadEffectList[SkillInfo.HitEffect] = SkillInfo.HitEffect;				        					        
			       } 		  
			       else if(GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] != null)
			       {	
	    		  	   var ongameSkillResource:GameSkillResource = GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] as GameSkillResource;				
	    		  	   var animationSkill:SkillAnimation = ongameSkillResource.GetAnimation();
	    		  	   animationSkill.skillID = SkillInfo.SkillID;
	    		  	   animationSkill.offsetX = player.PlayerHitX;
	    		  	   animationSkill.offsetY = player.PlayerHitY;  	   
	    		  	   animationSkill.StartClip("jn");
	    		  	   
	    		  	   if(animationSkill.eventArgs.CurrentClipTotalFrameCount > 0)
	    		  	   {
		    		  	   player.addChild(animationSkill);
		    		  	   animationSkill.player = player; 		  	   
		    		  	   animationSkill.PlayComplete = PlayerPlayComplete;	  	    	   
		    		  	   player.skillAnimation.push(animationSkill);
			       	   }
			       	   else
			       	   {
			       	   	   GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] = null; 
		    		  	   GameCommonData.SkillLoadEffectList[SkillInfo.HitEffect] = null;	
			       	   }     
			       }	     	
			    }
		    }
	   }
		
		
		/**添加击中爆炸效果*/
		public  function AddHitEffect(point:Point):void
		{
			 if(SkillInfo.HitEffect.length > 0)
		     {
		     	if(GameCommonData.SkillLoadEffectList[SkillInfo.HitEffect] == null)
		        {
		        	var gameSkillResource:GameSkillResource = new GameSkillResource();
		        	gameSkillResource.SkillID      = SkillInfo.SkillID;
			        gameSkillResource.EffectName   = SkillInfo.HitEffect;
			        gameSkillResource.EffectPath   = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF + SkillInfo.HitEffect+".swf";
			        gameSkillResource.OnLoadEffect = PlayerSkillHandler.LoadEffect;
			        gameSkillResource.EffectBR.LoadComplete = gameSkillResource.onEffectComplete;
			        gameSkillResource.EffectBR.Download.Add(gameSkillResource.EffectPath);
			        gameSkillResource.EffectBR.Load();
			        GameCommonData.SkillLoadEffectList[SkillInfo.HitEffect] = SkillInfo.HitEffect;				        					        
		       } 		  
		       else if(GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] != null)
		       {	
		       	   var ongameSkillResource:GameSkillResource = GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] as GameSkillResource;				
    		  	   var animationSkill:SkillAnimation = ongameSkillResource.GetAnimation();
    		  	   animationSkill.offsetX = point.x;
    		  	   animationSkill.offsetY = point.y;  	
    		  	   animationSkill.skillID = SkillInfo.SkillID;    
    		  	   animationSkill.StartClip("jn");	   
    		  	   if(animationSkill.eventArgs.CurrentClipTotalFrameCount > 0)
    		  	   {
    		  	   	   GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(animationSkill);
	    		  	   animationSkill.gameScene = GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer;
	    		  	   animationSkill.PlayComplete = ScenePlayComplete;	  	    	
	    		  	   //修改   
	    		  	   setTimeout(MustDeleteHeffect,4000,animationSkill);
	    		  	   this.Animal.skillAnimation.push(animationSkill); 
		       	   }
		       	   else
		       	   {
		       	   	   GameCommonData.SkillOnLoadEffectList[SkillInfo.HitEffect] = null; 
	    		  	   GameCommonData.SkillLoadEffectList[SkillInfo.HitEffect] = null;	
		       	   }      
		       }	     	
		    }
	   }
	
	    /**判断是否可以看见攻击提示**/
		public function IsShowAttackPrompt(gameSkillEffect:GameSkillEffect):Boolean
		{
			//是否可以看见
			var IsShow:Boolean = false;	
			
			if(gameSkillEffect.TargerPlayer.Role.Id == GameCommonData.Player.Role.Id ||                        //如果被打方是自己
			  (GameCommonData.Player.Role.UsingPetAnimal != null                                               //宠物是否存在
			 && GameCommonData.Player.Role.UsingPetAnimal.Role.Id == gameSkillEffect.TargerPlayer.Role.Id))                       //被打的是自己的宠物		
			{
				IsShow = true;
			}
			
			if(this.Animal.Role.Id == GameCommonData.Player.Role.Id)  				                        // 攻击者是否为自己    
			{
				IsShow = true;
			}
			
			if(GameCommonData.Player.Role.UsingPetAnimal != null   &&                                            // 宠物是否存在
			   this.Animal.Role.Id ==  GameCommonData.Player.Role.UsingPetAnimal.Role.Id)  				         // 攻击者是否为自己宠物    
			{
				IsShow = true;
			}
			return IsShow;
		}
		
		/**修改掉血*/			
		public  function ShowAttackPrompt(gameSkillEffect:GameSkillEffect):void
		{		     	     		          
			//攻击动作提示文字信息
			if(IsShowAttackPrompt(gameSkillEffect)) 				            // 攻击者是否为自己     
			{
				switch(gameSkillEffect.TargerState)
				{			
					case GameSkill.TARGET_ERUPTIVE_HP:
							//如果是有血量才显示
							if(gameSkillEffect.TargerHP > 0)
							{
							    if(SkillInfo != null)
							     {
								    //判断是否是治疗				    
								    if(GameSkillMode.IsDoctorSkill(SkillInfo.SkillMode))
								   	    CombatController.SkillAddHP(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP.toString(), gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY);
								    else
								    {
	                        	       //攻击者是否是宠物
							           if(this.Animal.Role.Type == GameRole.TYPE_PET)
								          CombatController.PetAttackPrompt(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP, true);
								       else
										  CombatController.AttackPrompt(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP, true);
								    }
							     }
							     else
							     {
							     	switch(GameSkillBuff.IsShowState(this.gameSkillBuff.BuffType))
							     	{
							     		case 1: 
							     		     CombatController.SkillAddHP(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP.toString(), gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY);
							     		     break;
							     		case 2:
											 CombatController.AttackPrompt(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP, true);
							     		     break;
							     	}
							     }		
							}     
						break;
					case GameSkill.TARGET_HP:
					    if(gameSkillEffect.TargerHP > 0)
						{
							if(SkillInfo != null)
						    {
							    if(GameSkillMode.IsDoctorSkill(SkillInfo.SkillMode))
		                            CombatController.SkillAddHP(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP.toString(), gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY);
		                        else
		                        {
	                        	   //攻击者是否是宠物
							       if(this.Animal.Role.Type == GameRole.TYPE_PET)
							          CombatController.PetAttackPrompt(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP, false);
							       else
									  CombatController.AttackPrompt(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP, false);
		                        }		
						    }
						    else
						    {
						    	switch(GameSkillBuff.IsShowState(this.gameSkillBuff.BuffType))
						     	{
						     		case 1: 
						     		     CombatController.SkillAddHP(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP.toString(), gameSkillEffect.TargerPlayer.HitX,gameSkillEffect.TargerPlayer.HitY);
						     		     break;
						     		case 2:
										  CombatController.AttackPrompt(gameSkillEffect.TargerPlayer, gameSkillEffect.TargerHP, false);
						     		     break;
						     	}
						    }
					    }
					    break;
					case GameSkill.TARGET_EVASION:
						CombatController.EvasionPrompt(gameSkillEffect.TargerPlayer);
					    break;
					case GameSkill.TARGET_SUCK:
						CombatController.SuckPrompt(gameSkillEffect.TargerPlayer);
					    break;
				}
			}			
			
			//目标死亡
			if(gameSkillEffect.TargerPlayer.Role.HP <= 0 && gameSkillEffect.TargerPlayer.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				ShowDead(gameSkillEffect);
			}			
		}

        public  function ShowDead(gameSkillEffect:GameSkillEffect):void
		{		   			  		                     
			UIFacade.UIFacadeInstance.upDateInfo(6);	
			
			if(gameSkillEffect.TargerPlayer.Role.Type == GameRole.TYPE_ENEMY)
			{
				TalkController.BossTalk(4,gameSkillEffect.TargerPlayer);
			}
			
			//人物死亡
			PlayerSkinsController.PlayerDead(gameSkillEffect.TargerPlayer);					                              					
			
			//死亡清空职责链
			if(gameSkillEffect.TargerPlayer.handler != null)
				gameSkillEffect.TargerPlayer.handler.Clear();

            //自己死亡
			if(gameSkillEffect.TargerPlayer.Role.Id == GameCommonData.Player.Role.Id)
			{		
				//取消自动任务攻击
				GameCommonData.Scene.ClearTaskAttack();					
	            //触发挂机死亡面板上勾选的条件
	            UIFacade.GetInstance(UIFacade.FACADEKEY ).sendNotification(AutoPlayItemsCommand.NAME,"dead" );	            
			}
			
			//挂机目标死亡
            if(TargetController.TargetAutomatismID == gameSkillEffect.TargerPlayer.Role.Id)
            {
            	GameCommonData.IsAttack = false;
            	//清除攻击玩家的数据集合
            	if(GameCommonData.Player.Role.Aggressor[TargetController.TargetAutomatismID] != null)
            	{
                	GameCommonData.Player.Role.Aggressor[TargetController.TargetAutomatismID] = null;
					delete GameCommonData.Player.Role.Aggressor[TargetController.TargetAutomatismID];
            	}
            	setTimeout(PlayerController.Automatism,500);
            }
            
            //任务攻击目标死亡	
            if(GameCommonData.AttackTargetID == gameSkillEffect.TargerPlayer.Role.Id)
            {
            	GameCommonData.AttackTargetID = 0;
            }
            
            if(this.Animal.Role.Id == GameCommonData.Player.Role.Id)
            {
            	//取消自动任务攻击
			    GameCommonData.autoPlayAnimalType = 0;
            	CombatController.attackTime = 0;
            }
				
			//判断宠物是否杀死敌方 回来
			if(GameCommonData.PetTargetAnimal != null &&
			GameCommonData.PetTargetAnimal.Role.Id == gameSkillEffect.TargerPlayer.Role.Id)
			{
				GameCommonData.Player.Role.UsingPetAnimal.isWalk = true;
				//宠物在杀死怪后 3格以外回到主人身边	
                if(!DistanceController.PlayerPetDistance(3))
                {			
					GameCommonData.Player.Role.UsingPetAnimal.MoveSeek();
                }
			}
		}

        /**修改飞行物体转向*/
	    public  function Orientation(point:Point,displayObject:DisplayObject):void
		{		
			var dx:Number=displayObject.x - point.x;
			var dy:Number=displayObject.y - point.y;
			var r:Number= Math.atan2(dy,dx)*180/Math.PI;
			displayObject.rotation=r;
		}
		
		public function PlayerSkillHandler(animal:GameElementAnimal,next:Handler,skillEffectList:Array,skill:GameSkill,buff:GameSkillBuff,timeID:int,point:Point = null,process:int = 1)
		{
			super(animal,next,skillEffectList,skill,buff,timeID,point,process);
			this.Type = 2;
		}

	}
}