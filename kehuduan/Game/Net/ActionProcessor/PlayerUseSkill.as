package Net.ActionProcessor
{
	import Controller.CombatController;
	import Controller.PlayerActionHandler;
	import Controller.PlayerController;
	import Controller.PlayerSkillHandler;
	
	import GameUI.ConstData.EventList;
	
	import Net.GameAction;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.Handler;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillBuff;
	import OopsEngine.Skill.GameSkillEffect;
	import OopsEngine.Skill.GameSkillMode;
	
	import flash.geom.Point;
	import flash.utils.ByteArray;
	
	public class PlayerUseSkill extends GameAction
	{ 
		public function PlayerUseSkill()
		{
			super(true);
		}
		
		/**监控职责链**/
		public function TimeHandler(player:GameElementAnimal,timeID:int):void
		{
			if(player.handler != null && player.handler.TimeID == timeID)
			 	player.handler.Clear();
		}
		
		public override function Processor(bytes:ByteArray):void 
		{		
		    bytes.position = 4;
       
            var idUser:uint = bytes.readUnsignedInt();//使用者
            var idTarget:uint = bytes.readUnsignedInt();//攻击目标

            var posx:int = bytes.readShort();//X坐标
            var posy:int = bytes.readShort();//Y坐标
            var ucCollideDir:int = bytes.readByte();//方向
            bytes.readByte();//格式对齐
                     

            var type:int = bytes.readUnsignedShort();//技能ID
            var level:int = bytes.readUnsignedShort();//技能等级
            var dir:int = bytes.readByte();//方向
            var effectCount:int = bytes.readByte();//影响目标的数量 
            
            if ( type == 2501 ) return;
                             
            var skill:GameSkill;
            var buff:GameSkillBuff;
                          
            //判断是否为BUff       
            if(type > 9999)
            {
            	buff = GameCommonData.BuffList[type] as GameSkillBuff;
            }        
            else
            {
            	skill = GameCommonData.SkillList[type] as GameSkill;
            	//击退延长选怪时间
            	if(skill.SkillMode == 27)
            	{
            		var t:Date = new Date();
            		GameCommonData.Targettime = t.time;
            	}
            }
                  
            //读取攻击者信息
		    var sendPlayer:GameElementAnimal = PlayerController.GetPlayer(idUser);
		    
		        
		   //被击打者数组
           var gameSkillEffectList:Array = new Array();
          
           //判断攻击者是否在该场景内
           if(sendPlayer != null)
           { 
           	     //攻击状态
			    if(sendPlayer.Role.Id == GameCommonData.Player.Role.Id && skill != null)
				{	
					PlayerController.SendAttack = 0;
//					if(GameSkillMode.TargetState(skill.SkillMode) == 1 && GameCommonData.AttackAnimal != null)
//		 			{
//		 				GameCommonData.Scene.Attack(GameCommonData.AttackAnimal);
//		 			}			 
					GameCommonData.Player.Role.UpdateAttackTime();
				}
           	
	           	//判断是否是刷CD的技能 是的话要调用方法     
			    if(sendPlayer.Role.Id == GameCommonData.Player.Role.Id && skill != null && skill.SkillMode == 23)
			    {	      	           
					sendNotification(EventList.USE_EXTENDSKILL_MSG);
			    }        	

           	
           	
           	    //被击打者信息
           	    for (var i:int=0; i<effectCount; i++)
            	{
            		 var gameSkillEffect:GameSkillEffect = new GameSkillEffect();
              	 	 var idRole:uint = bytes.readUnsignedInt();//被影响的目标ID
                     var nData:int = bytes.readInt();//影响的数值
                     var addinfo:uint = bytes.readUnsignedInt();//附加信息              
                     var targerPlayer:GameElementAnimal = PlayerController.GetPlayer(idRole);                                             

                     //技能失效 服务器的协议
                     if(addinfo == 256)
                     {
                     	return;
                     }
                     
                     //攻击状态
                     if(targerPlayer != null && targerPlayer.Role.Id == GameCommonData.Player.Role.Id)
					 {	
					 	//施法者不能为自己
					 	if(sendPlayer.Role.Id  != GameCommonData.Player.Role.Id)
					 	{
						 	//设置攻击者的时间 用于选中攻击自己的怪
							var now:Date = new Date();				
							GameCommonData.Player.Role.Aggressor[sendPlayer.Role.Id] = now.time;	
					  	}
					    	 
						GameCommonData.Player.Role.UpdateAttackTime();
					 }
					 	 
//					 if(targerPlayer != null && targerPlayer.Role.Type == GameRole.TYPE_PLAYER && sendPlayer.Role.Id == GameCommonData.Player.Role.Id)
//					 {
					 	 //杨小龙
//					 	  GameCommonData.UIFacadeIntance.sendNotification(PkEvent.INITTIMER);
//					 }	 
                     
                     //判断被击打者是否在该场景内
                     if(targerPlayer != null)
                     {           	
                     	switch(addinfo)
                     	{
                     		case 0: //普
                     			 gameSkillEffect.TargerState = GameSkill.TARGET_HP;
                     		     break;
                     		case 1: //闪避
                     		     gameSkillEffect.TargerState = GameSkill.TARGET_EVASION;
                     		     break; 
                     		case 2://暴击
                     		     gameSkillEffect.TargerState = GameSkill.TARGET_ERUPTIVE_HP;
                     		     break; 
                     	    case 4://吸收
                     		     gameSkillEffect.TargerState = GameSkill.TARGET_SUCK;
                     		     break; 
                     	}
                          gameSkillEffect.TargerPlayer = targerPlayer;
                          gameSkillEffect.TargerHP = nData;

		                  //人物死亡
					      if(targerPlayer.Role.HP - nData <= 0)
					   		 gameSkillEffect.IsDead = true;  

                 		  gameSkillEffectList.push(gameSkillEffect);
                      }
                 }
            }       
        
            //攻击者必须存在 并且有受到伤害的目标                                   攻击者必须存在 并且是技能攻击  可以没受伤害有目标 比如群伤      
            if((sendPlayer != null && gameSkillEffectList.length > 0) ||(sendPlayer != null && skill != null && GameSkillMode.IsNoTarget(skill.SkillMode)))
            {  
            	var playerSkillHandler:PlayerSkillHandler;
            	var playerActionHandler:PlayerActionHandler
            	sendPlayer.ActionPlayFrame = PlayerController.onActionPlayFrame;
            	
            	//设置时间 用于控制职责链的释放
            	var nowDate:Date = new Date();
            	var timeID:int   = nowDate.time;
            	
            	//判断是技能还是BUFF 
            	if(type > 9999)
            	{
            		// 只有治疗 和 伤害技能才有文字动画
            		if(buff.BuffType == 2 || buff.BuffType == 1)
            		{          			
	            	    playerSkillHandler = new PlayerSkillHandler(sendPlayer,null,gameSkillEffectList,null,buff,timeID,null,3);
				 		 	
				 		if(sendPlayer.handler == null)
				 		{				    
					    	 sendPlayer.handler  = playerSkillHandler;
						}
						else
						{ 		
					   	     Handler.FindEndHendler(sendPlayer.handler,playerSkillHandler);
						}
            		}
	            }
	            else
	            {
	            	//判断是否需要瞬移背刺的技能
	            	if(skill.SkillMode == 28 || skill.SkillMode == 31 || skill.SkillMode == 22)
					{
						if(sendPlayer.Role.Id == GameCommonData.Player.Role.Id)
						{
							GameCommonData.Player.Role.TileX     = posx;
							GameCommonData.Player.Role.TileY     = posy;
							GameCommonData.Scene.StopPlayerMove(GameCommonData.Player);						     
						}	
						else
						{
							var gamePoint:Point = MapTileModel.GetTilePointToStage(posx,posy);
                    		sendPlayer.Role.TileX = posx;
							sendPlayer.Role.TileY = posy;
							sendPlayer.X  = gamePoint.x;
							sendPlayer.Y  = gamePoint.y;
							if(sendPlayer.Role.Type == GameRole.TYPE_OWNER
							|| sendPlayer.Role.Type == GameRole.TYPE_PLAYER)
							{
								(sendPlayer as GameElementPlayer).AddTernal();
							}
						}
					}
					
					//击退技能
					if(skill.SkillMode == 27)
					{
					    gamePoint = MapTileModel.GetTilePointToStage(posx,posy);
                    	gameSkillEffectList[0].TargerPlayer.Role.TileX = posx;
						gameSkillEffectList[0].TargerPlayer.Role.TileY = posy;
						gameSkillEffectList[0].TargerPlayer.X  = gamePoint.x;
						gameSkillEffectList[0].TargerPlayer.Y  = gamePoint.y;
						if((gameSkillEffectList[0].TargerPlayer as GameElementAnimal).Role.Type == GameRole.TYPE_OWNER
						|| (gameSkillEffectList[0].TargerPlayer as GameElementAnimal).Role.Type == GameRole.TYPE_PLAYER)
						{
					   		(gameSkillEffectList[0].TargerPlayer as GameElementPlayer).AddTernal();
					   }
					}
	            	            	
	            	var point:Point;      
	            	if(skill.SkillMode == 3 || skill.SkillMode == 4 || skill.SkillMode == 5 || skill.SkillMode == 6 || skill.SkillMode == 7 
	            	|| skill.SkillMode == 9 || skill.SkillMode == 11  ||  skill.SkillMode == 12 ||  skill.SkillMode == 14 
	            	|| skill.SkillMode == 20 || skill.SkillMode == 28 || skill.SkillMode == 29 || skill.SkillMode == 30 || skill.SkillMode == 31)
	            	{ 
	            		point = new Point(gameSkillEffectList[0].TargerPlayer.GameX,gameSkillEffectList[0].TargerPlayer.GameY);
	            	}  
	            	else
	            	{
	            		if(skill.SkillMode == 27)
	            		{
	            			point = new Point(gameSkillEffectList[0].TargerPlayer.HitX,gameSkillEffectList[0].TargerPlayer.HitY);
	            		}
	            		else
	            		{
	            		 	point = MapTileModel.GetTilePointToStage(posx,posy);
	            		}
	            	}	    
	            	  
	            	if(skill.SkillMode == 12)
	            	{
	            		if(GameCommonData.Player.Role.UsingPetAnimal != null && sendPlayer.Role.Id == GameCommonData.Player.Role.UsingPetAnimal.Role.Id)
	            		{
	            			CombatController.Skill(GameCommonData.Player.Role.UsingPetAnimal,skill.SkillName);         		
	            		}
				 			 				    
	            	}  
	            	else if(GameSkillMode.IsPetSkill(skill.SkillMode))
	            	{
	            	    if(GameCommonData.Player.Role.UsingPetAnimal != null && sendPlayer.Role.Id == GameCommonData.Player.Role.UsingPetAnimal.Role.Id
	            	    && skill.SkillMode != 33)
	            		{
	            			CombatController.Skill(GameCommonData.Player.Role.UsingPetAnimal,skill.SkillName);         		
	            		}
	            		
                    	playerSkillHandler  = new PlayerSkillHandler(sendPlayer,null,gameSkillEffectList,skill,null,timeID,point);
				 		playerActionHandler =  new PlayerActionHandler(sendPlayer,null,gameSkillEffectList,skill,null,timeID,point); 		 	
				 		playerActionHandler.InfoHandler = playerSkillHandler;
				 	
				 		if(sendPlayer.handler == null)
				 		{
					    	 sendPlayer.handler  = playerActionHandler;    	 
						}
						else
						{ 		
					   	     Handler.FindEndHendler(sendPlayer.handler,playerActionHandler);
						}      
	            	}
	            	else if(skill.SkillMode == 22)   
	            	{
	            	    playerActionHandler =  new PlayerActionHandler(sendPlayer,null,gameSkillEffectList,skill,null,timeID,point); 
				 		 	
				 		if(sendPlayer.handler == null)
				 		{	
					    	 sendPlayer.handler  = playerActionHandler;
						}
						else
						{ 		
					   	     Handler.FindEndHendler(sendPlayer.handler,playerActionHandler);
						}
						
						if(sendPlayer.golem != null && sendPlayer.handler != null)
						{
							sendPlayer.handler.Clear();
							sendPlayer.Skins.ChangeAction(GameElementSkins.ACTION_STATIC,true);
						}
	            	}	
	            	else
	            	{    	           	
				   		playerSkillHandler  = new PlayerSkillHandler(sendPlayer,null,gameSkillEffectList,skill,null,timeID,point);
				 		playerActionHandler =  new PlayerActionHandler(sendPlayer,null,gameSkillEffectList,skill,null,timeID,point); 		 	
				 		playerActionHandler.InfoHandler = playerSkillHandler;
				 	
				 	    // 这个是第2次释放技能的处理
				 	    if(sendPlayer.Role.Type != GameRole.TYPE_ENEMY
				 	    && skill.SkillMode == 42)
				 	    {
				 	    	playerSkillHandler.Run();
				 	    }
				 	    else
				 	    {
					 		if(sendPlayer.handler == null)
					 		{
						    	 sendPlayer.handler  = playerActionHandler;   	 
							}
							else
							{ 		
						   	     Handler.FindEndHendler(sendPlayer.handler,playerActionHandler);
							}
				 	    }						
	            	}
	            }
            }
		}
	}
}