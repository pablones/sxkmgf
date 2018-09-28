package Controller
{
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	
	import flash.geom.Point;
	import flash.utils.setTimeout;
	
	public class PetController
	{
		public static var PetTimer:Number = 0;
			
		/**核对宠物标点**/
		public static function CheckPetPoint():void
		{	
	        if(GameCommonData.Player.Role.UsingPetAnimal != null)
	        {
				var gamePoint:Point = MapTileModel.GetTilePointToStage(GameCommonData.Player.Role.UsingPetAnimal.Role.TileX,GameCommonData.Player.Role.UsingPetAnimal.Role.TileY);		
				GameCommonData.Player.Role.UsingPetAnimal.X  = gamePoint.x;
				GameCommonData.Player.Role.UsingPetAnimal.Y  = gamePoint.y;
				GameCommonData.Scene.PetResetMoveState();
				GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);		
	        }
		} 
			
		public static function PetReserveAttack(targetAnimal:GameElementAnimal = null):void
		{		
			if(targetAnimal == null)
				targetAnimal = GameCommonData.TargetAnimal;
					
			var combat:CombatController = new CombatController();
				
		    //判断宠物是否可以攻击
     		if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.Role.HP > 0
     		&&  targetAnimal.Role.HP > 0 && GameCommonData.Player.Role.UsingPetAnimal.Role.Id != targetAnimal.Role.Id)
     		{
     			//是否可以使用普通攻击
				if(PetIsUseSkill(GameCommonData.Player.Role.UsingPetAnimal,targetAnimal))
				{
     				GameCommonData.PetTargetAnimal = targetAnimal;
   				}
     		}    
		}
		
		/**监控兔子所攻击的目标是否逃跑攻击目标7格**/
		public static function PetDistance():void
		{
			if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.Role.HP > 0
			&& GameCommonData.PetTargetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.isWalk == false)
			{			
				if(!DistanceController.PetTargetDistance(9))
				{
					GameCommonData.Player.Role.UsingPetAnimal.MoveSeek(); 
				}
			}
		}
		
//	    /** 计算玩家与目标之间的距离  */
//		private static function CalculateDistance(tileX:int,tileY:int,distance:int):Boolean
//		{
//			var p:Point = MapTileModel.GetTileStageToPoint(GameCommonData.Player.Role.UsingPetAnimal.GameX,GameCommonData.Player.Role.UsingPetAnimal.GameY);
//			var targetDistance:int = MapTileModel.Distance(tileX, 
//														   tileY,
//									  					   p.x, 
//									  					   p.y);
//			/** 判断类型动态改与目标判断的距离 */
//			if(targetDistance <= distance)			// 动态参数据判断与目标的距离
//			{
//				return true;
//			}
//			return false;
//		}
		
		/**设置宠物速度
		 * 主人对象**/
		public static function SetPetSpeed(person:GameElementAnimal,moveStepLength:int):void
	    {
	    	if(person.Role.UsingPetAnimal != null)
	    	{
	    		person.Role.UsingPetAnimal.SetMoveSpend(moveStepLength);
	    	}
	    }
		
		
		/**宠物使用技能*/
		public static function PetUseSkill(UseskillID:int,targetAnimal:GameElementAnimal = null,point:Point = null):void
		{	
           var combat:CombatController = new CombatController();	
           
           var skill:GameSkillLevel;
            
           //查找宠物的技能
           if(GameCommonData.Player.Role.UsingPetAnimal != null)
           {
				for(var n:int=0;n<GameCommonData.Player.Role.UsingPet.SkillLevel.length;n++)
				{
					if(!GameCommonData.Player.Role.UsingPet.SkillLevel[n]) continue;
					skill = GameCommonData.Player.Role.UsingPet.SkillLevel[n];
					
					if(skill.gameSkill.SkillID == UseskillID)
					{								
						var state:int =  GameSkillMode.Affect(skill.gameSkill.SkillMode);
           	
		           		switch(state)
						{
							case 1:
								 if(targetAnimal == null)
								 {
								 	 targetAnimal = TargetController.GetPetTarget();
								 }
								 if(PetIsUseSkill(GameCommonData.Player.Role.UsingPetAnimal,targetAnimal,skill.gameSkill))
								 {
									 combat.ReservePetSkillAttack(skill.gameSkill.SkillID,skill.Level,targetAnimal);						    
								     GameCommonData.PetTargetAnimal = targetAnimal;
								     //追加普通攻击
								     PetReserveAttack(GameCommonData.PetTargetAnimal);
								  }
							     break;
						    case 2:
						         if(PetIsUseSkill(GameCommonData.Player.Role.UsingPetAnimal,targetAnimal,skill.gameSkill))
								 {
						         	combat.ReservePetAffectSkillAttack(skill.gameSkill.SkillID,skill.Level);
						         } 
						         break;
						   	case 3:
						         if(PetIsUseSkill(GameCommonData.Player.Role.UsingPetAnimal,targetAnimal,skill.gameSkill))
								 {
						         	combat.ReservePetPointAffectSkillAttack(skill.gameSkill.SkillID,skill.Level,point);
						         } 
						         break;   
						      
						}	
					}
				}
           }  
		}
		
		/**自动释放技能**/
		public static function AutomatismUseSkill():void
		{
		    var nowDate:Date = new Date();
			if(nowDate.time - PetController.PetTimer > 2000)
			{
				//判断是否有攻击对象
				if(GameCommonData.Player.Role.UsingPetAnimal != null
				&& GameCommonData.PetTargetAnimal != null 
				&& GameCommonData.PetTargetAnimal.Role.ActionState != GameElementSkins.ACTION_DEAD
				&&  GameCommonData.PetTargetAnimal.Role.HP > 0)
				{
					if(GameCommonData.Player.Role.UsingPet != null && GameCommonData.Player.Role.UsingPet.SkillLevel != null)
					{
						//查找是否宠物拥有自动释放的技能
						for(var n:int=0;n<GameCommonData.Player.Role.UsingPet.SkillLevel.length;n++)
						{
							if(!GameCommonData.Player.Role.UsingPet.SkillLevel[n]) continue;
							var skill:GameSkillLevel = GameCommonData.Player.Role.UsingPet.SkillLevel[n];
							//判断是否是自动释放的技能
							if(GameSkillMode.IsPetAutomatism(skill.gameSkill.SkillMode))
							{				
		    					//判断时间是否到
								if(nowDate.time - PetController.PetTimer > 2000 && nowDate.time - skill.GetCoolTime * 1000 > skill.UseTime)
								{
									PetUseSkill(skill.gameSkill.SkillID,GameCommonData.PetTargetAnimal);	
									skill.UseTime = nowDate.time;
									PetController.PetTimer = nowDate.time;
								}		
							}
						}
					}
				}
			}
		}
		
		/**自动释放技能**/
		public static function BuffUseSkill():void
		{
			//判断是否存在宠物
			if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPet != null 
			&&  GameCommonData.Player.Role.UsingPet.SkillLevel != null)
			{	
				var time:int = 0;	
				var startTime:int = 2000;			
				//查找是否宠物拥有自动释放的技能
		      	for(var n:int=0;n<GameCommonData.Player.Role.UsingPet.SkillLevel.length;n++)
				{
					if(!GameCommonData.Player.Role.UsingPet.SkillLevel[n]) continue;
					var skill:GameSkillLevel = GameCommonData.Player.Role.UsingPet.SkillLevel[n];
					//判断是否是BUff的技能
					if(GameSkillMode.IsPetBuffSkill(skill.gameSkill.SkillMode))
					{		
						//判断该Buff是否存在
						if(!GameCommonData.Player.Role.IsBuff(skill.gameSkill.Buff))
						{	
							time ++;						
							setTimeout(AddBuff,startTime + time * 1000,skill);

						}					
					}
				}
			}
		}		
		
		public static function AddBuff(skill:GameSkillLevel):void
		{
			if(!GameCommonData.Player.Role.IsBuff(skill.gameSkill.Buff))
			{	
				var combat:CombatController = new CombatController();
				combat.ReservePetBuffSkillAttack(skill.gameSkill.SkillID,skill.Level,GameCommonData.Player);
			}
		}
		
		public static function PetIsUseSkill(player:GameElementAnimal,targetAnimal:GameElementAnimal,gameSkill:GameSkill=null):Boolean
		{		
			var IsUseSkill:Boolean = false;
			var skillState:int = 1;           // 1 指定玩家伤害  2 只能对自己释放 3 宠物只能对主人释放  4 群放魔法不需要判断  5 只能对玩家释放的友善技能
			
			if(gameSkill != null)  //为空则为普通攻击
			{
				skillState = GameSkillMode.TargetState(gameSkill.SkillMode);
			}

			switch(skillState)
			{
				case 1:
				    if(targetAnimal != null)
					{
						if(player.Role.Id != targetAnimal.Role.Id &&                            //不是自己
						targetAnimal.Role.ActionState != GameElementSkins.ACTION_DEAD           //是否死亡
				   		&& targetAnimal.Role.HP > 0)
				  		 {
				   			if(targetAnimal.Role.Type == GameRole.TYPE_ENEMY)                      //是敌人
				   			{
				   			     IsUseSkill = true;
				   		    }
				   		    else if (targetAnimal.Role.Type == GameRole.TYPE_PLAYER)               //如果是玩家   
				   		    {
					   			if(player.Role.MasterPlayer.Role.idTeam !=  targetAnimal.Role.idTeam ||              //是否同1队              
									player.Role.MasterPlayer.Role.idTeam==0)
								{
									IsUseSkill = true;
								}
				   		    }
					   		else if (targetAnimal.Role.Type == GameRole.TYPE_PET)                   //如果是宝宝
					   		{
				   				if(player.Role.MasterPlayer.Role.idTeam !=  targetAnimal.Role.MasterPlayer.Role.idTeam ||              //是否同1队              
								player.Role.MasterPlayer.Role.idTeam==0)
								{
				   					IsUseSkill = true;
				   				}
					   		}
					   	}
				     }
				     break;
				case 2:
				     IsUseSkill = true;	
				     break;
				case 3:
					if(targetAnimal != null)
					{
					     if(player.Role.MasterPlayer.Role.Id == targetAnimal.Role.Id)
					     {
					     	IsUseSkill = true;	
					     }
					}
				     break;
				case 4:
				     IsUseSkill = true;	
				      break;
				case 5:
					if(targetAnimal != null)
					{	
						if (targetAnimal.Role.Type == GameRole.TYPE_PLAYER || targetAnimal.Role.Type == GameRole.TYPE_PET)  
						{
							IsUseSkill = true;	
						}
					}
					break;
  
			}
			return IsUseSkill;	
		}

	}
}