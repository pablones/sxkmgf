package Net.ActionProcessor
{
	import Controller.CombatController;
	import Controller.PlayerActionHandler;
	import Controller.PlayerController;
	import Controller.PlayerSkillHandler;
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.AutoPlay.command.AutoPlayItemsCommand;
	import GameUI.UICore.UIFacade;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.Handler;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillEffect;
	import OopsEngine.Skill.JobGameSkill;
	import OopsEngine.Skill.WeirdyGameSkill;
	
	import flash.geom.Point;
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;

	/** 攻击提示信息和相应动画（延时显示扣血和对应动做）  */
	public class PlayerCombat extends GameAction
	{
		private var delayTime:int    = 850;				// 延时时长
		private var dombatData:Array = new Array();		// 战斗数据
		private var combat:CombatController = new CombatController();
		
		public function PlayerCombat()
		{			 
			super(false);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position    		 = 4;
			var idSend:uint   		 = bytes.readUnsignedInt(); 	// 攻击者编号
			var idTarget:uint 		 = bytes.readUnsignedInt(); 	// 被攻击者编号
			var unType:uint   		 = bytes.readUnsignedShort(); 	// 消息类型
			bytes.readUnsignedShort();
			var dwData:uint 		 = bytes.readUnsignedInt(); 	// 伤害
			var unPosX:int  		 = bytes.readUnsignedShort(); 	// 位置
			var unPosY:int 			 = bytes.readUnsignedShort();
			var unMagicType:uint 	 = bytes.readUnsignedShort();	// 技能
			var unMagicLev:uint  	 = bytes.readUnsignedShort();
			var dwBattleAddInfo:uint = bytes.readUnsignedInt(); 	// 战斗状态			
			

            if(unType==14)							// 死亡(后要发一个打死目标的伤害值过来）
			{  
				var deadPlayer:GameElementAnimal = PlayerController.GetPlayer(idTarget);	
				
				if(deadPlayer != null)
				{
					deadPlayer.Role.HP = 0;
					if(deadPlayer.Role.Type == GameRole.TYPE_PLAYER || deadPlayer.Role.Type == GameRole.TYPE_OWNER)
					{
						setTimeout(ShowDead,2000,deadPlayer);
					}		
				}								
			}
			if(unType==2 || unType==25)
			{		
				if(dwBattleAddInfo == 0)			// 普通攻击
				{
					CreateHendler(idSend,idTarget,dwData,GameSkill.TARGET_HP);
				}
				else if(dwBattleAddInfo == 1)		// 闪避
				{
					CreateHendler(idSend,idTarget,dwData,GameSkill.TARGET_EVASION);
				}
				else if(dwBattleAddInfo == 2)		// 暴击
				{
				    CreateHendler(idSend,idTarget,dwData,GameSkill.TARGET_ERUPTIVE_HP); 
				} 
				else if(dwBattleAddInfo == 4)		// 吸收
				{
				    CreateHendler(idSend,idTarget,dwData,GameSkill.TARGET_SUCK); 
				} 
			}
		}
		
	    public  function ShowDead(deadPlayer:GameElementAnimal):void
		{		
			if(deadPlayer.Role.ActionState != GameElementSkins.ACTION_DEAD && deadPlayer.Role.HP == 0) 
			{  			  		                     
				UIFacade.UIFacadeInstance.upDateInfo(6);		
				
				PlayerSkinsController.PlayerDead(deadPlayer);		                              					
			
				//死亡清空职责链
				if(deadPlayer.handler != null)
					deadPlayer.handler.Clear();

	            //自己死亡
				if(deadPlayer.Role.Id == GameCommonData.Player.Role.Id)
				{
					//取消自动任务攻击
					GameCommonData.Scene.ClearTaskAttack();		
	            	//触发挂机死亡面板上勾选的条件
	            	UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(AutoPlayItemsCommand.NAME,"dead");		
				}
			}
		}
	
	
		public function CreateHendler(idSend:int,idTarget:int,dwData:int,nstate:String):void
		{
		     //攻击方
			var sendPlayer:GameElementAnimal = PlayerController.GetPlayer(idSend);
			//目标方
			var targerPlayer:GameElementAnimal = PlayerController.GetPlayer(idTarget);		
            var gameSkill:GameSkill;			
					
			//技能效果
			if(sendPlayer != null && targerPlayer != null)
			{
				//当前时间
				var now:Date;
				//攻击者是自己
				if(sendPlayer.Role.Id == GameCommonData.Player.Role.Id)
				{
					//设置挂机时间
					now = new Date();
					GameCommonData.AutomatismTime = now.time;
					//更新战斗时间
				    GameCommonData.Player.Role.UpdateAttackTime();	    
				}
				
				//如果是人
				if(targerPlayer.Role.Id == GameCommonData.Player.Role.Id)
				{
					//攻击者不能为自己
				 	if(sendPlayer.Role.Id  != GameCommonData.Player.Role.Id)
				 	{
						//设置攻击者的时间 用于选中攻击自己的怪
						now = new Date();			
						GameCommonData.Player.Role.Aggressor[sendPlayer.Role.Id] = now.time;
				    }				    
					
					//更新战斗时间
				    GameCommonData.Player.Role.UpdateAttackTime();
				}
				
				//如果是宠物
				if(GameCommonData.Player.Role.UsingPetAnimal != null
				&&  targerPlayer.Role.Id == GameCommonData.Player.Role.UsingPetAnimal.Role.Id)
				{
					//攻击者不能为自己 并且 攻击者不是自己的宠物
				 	if(sendPlayer.Role.Id  != GameCommonData.Player.Role.Id 
				 	&& sendPlayer.Role.Id  != GameCommonData.Player.Role.UsingPetAnimal.Role.Id)
				 	{
						//设置攻击者的时间 用于选中攻击自己的怪
						now = new Date();			
						GameCommonData.Player.Role.Aggressor[sendPlayer.Role.Id] = now.time;
				    }				    
					
					//更新战斗时间
				    GameCommonData.Player.Role.UpdateAttackTime();
				}
			
				var nowDate:Date = new Date();
            	var timeID:int   = nowDate.time;			
				var gameSkillEffectList:Array = new Array();
				
				if(sendPlayer.Role.Type == GameRole.TYPE_ENEMY)
		        {
		             var  weirdyGameSkill:WeirdyGameSkill =  GameCommonData.WeirdyGameSkillList[sendPlayer.Role.MonsterTypeID];
		             if(weirdyGameSkill != null)
                    	 gameSkill = GameCommonData.SkillList[weirdyGameSkill.SkillID];
		        }
		        else
		        {
		        	 var jobGameSkill:JobGameSkill = GameCommonData.JobGameSkillList[sendPlayer.Role.CurrentJobID];
		        	 if(jobGameSkill != null)
                    	 gameSkill = GameCommonData.SkillList[jobGameSkill.SkillID];
		        }
		        
		        if(gameSkill == null)
		        {
		        	gameSkill = GameCommonData.SkillList[9500];
		        }
		             			
		        var gameSkillEffect:GameSkillEffect = new GameSkillEffect();
		    	gameSkillEffect.TargerPlayer   = targerPlayer;
		    	gameSkillEffect.TargerState    = nstate;
			    gameSkillEffect.TargerHP       = dwData;
			    gameSkillEffect.TargerState    = nstate;	
                
//                //人物死亡
//			    if(targerPlayer.Role.HP <= 0)
                if(targerPlayer.Role.Id != GameCommonData.Player.Role.Id)
			   		gameSkillEffect.IsDead = true;  
			   		  
		 		gameSkillEffectList.push(gameSkillEffect);	 
			 		   	   			   	
		   	   var playerSkillHandler:PlayerSkillHandler = new PlayerSkillHandler(sendPlayer, null,gameSkillEffectList,
		  	                                                             gameSkill,null,timeID,new Point(targerPlayer.GameX,targerPlayer.GameY)); 
		 	   var playerActionHandler:PlayerActionHandler = new PlayerActionHandler(sendPlayer, null,gameSkillEffectList,
		  	                                                             gameSkill,null,timeID,new Point(targerPlayer.GameX,targerPlayer.GameY)); 
		  	   gameSkillEffect.InfoHandler = playerSkillHandler;                                                           	  	                                                             
		  	   playerActionHandler.InfoHandler =  playerSkillHandler;      
		  	                                                    
		 	   if(sendPlayer.handler == null)
		 	   {	
			  		sendPlayer.handler  = playerActionHandler;		  		
			   }
			   else
			   {
			   	     Handler.FindEndHendler(sendPlayer.handler,playerActionHandler);
			   }
				
				sendPlayer.ActionPlayFrame = PlayerController.onActionPlayFrame;						     		  
			}		
		}				
	}
}
