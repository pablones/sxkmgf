package Controller
{
	import Net.ActionSend.Zippo;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class CombatController
	{
		public static var attackTime:Number = 0;
	    public static var skillTime:Number = 0;
		public var INTERACT_ATTACK:int = 2;		// （普通攻击）
		public var INTERACT_SHOOT:int  = 21;	// （魔法攻击）		
		
		/** 人上坐骑  */
		public function ReserveMount(Mount:int):void
		{
			Zippo.SendAttack(GameCommonData.Player.Role.Id,
							 Mount,
							 0,
							 0,
							 300);
		}
		
		/** 人的普通攻击  */
		public function ReserveAttack(targetAnimal:GameElementAnimal):void
		{
			if(GameCommonData.Player.Role.HP > 0)
			{
				var date:Date = new Date();
				if(date.time - attackTime < 2000 
				&& GameCommonData.Player.IsAutomatism == false)
				{
					return;
				}
				attackTime = date.time;			
				Zippo.SendAttack(GameCommonData.Player.Role.Id,
								 targetAnimal.Role.Id,
								 0,
								 0,
								 INTERACT_ATTACK);						 
			}
		}

		/** 宠物普通攻击  */
		public function PetReserveAttack(targetAnimal:GameElementAnimal):void
		{
			//宠物不能走
			Zippo.SendAttack(GameCommonData.Player.Role.UsingPetAnimal.Role.Id,
							 targetAnimal.Role.Id,
							 0,
							 0,
							 INTERACT_ATTACK);
		}
		
		/**人自身为范围**/
		public function ReserveAffectSkillAttack(skill:int,level:int):void
		{	
			if(GameCommonData.Player.Role.HP > 0)
			{	
				Zippo.SendSkill(GameCommonData.Player.Role.Id,
			                    0,
			                    0,
								0,
								INTERACT_SHOOT,
								skill,level);
			}
		}
		
		/**以点为范围**/
		public function ReservePointAffectSkillAttack(skill:int,level:int,point:Point):void
		{
		    Zippo.SendSkill(GameCommonData.Player.Role.Id,
		                    0,
		                    point.x,
							point.y,
							INTERACT_SHOOT,
							skill,level);
		}
		
		/**人物选中目标的技能**/
		public function ReserveSkillAttack(skill:int,level:int,targetAnimal:GameElementAnimal):void
		{
			if(GameCommonData.Player.Role.HP > 0)
			{	
				var date:Date = new Date();
				if(date.time - CombatController.skillTime < 1000)
				{
					return;							
				}
				
				CombatController.skillTime = date.time;			
							
			    Zippo.SendSkill(GameCommonData.Player.Role.Id,
			                    targetAnimal.Role.Id,
			                    0,
								0,
								INTERACT_SHOOT,
								skill,level);					
			}
							
//			// 如果攻击者不是自己
//			if(GameCommonData.Player.Role.Id != targetAnimal.Role.Id)	
//			{			
//				ReserveAttack(targetAnimal);
//			}
		}
		
		/**宠物选中目标的技能**/
		public function ReservePetSkillAttack(skill:int,level:int,targetAnimal:GameElementAnimal):void
		{
		    Zippo.SendSkill(GameCommonData.Player.Role.UsingPetAnimal.Role.Id,
		                    targetAnimal.Role.Id,
		                    0,
							0,
							INTERACT_SHOOT,
							skill,level);
		}
		
	    /**宠物自身为范围**/
		public function ReservePetAffectSkillAttack(skill:int,level:int):void
		{
		    Zippo.SendSkill(GameCommonData.Player.Role.UsingPetAnimal.Role.Id,
		                    0,
		                    0,
							0,
							INTERACT_SHOOT,
							skill,level);
		}
		
	    /**宠物点地为范围**/
		public function ReservePetPointAffectSkillAttack(skill:int,level:int,point:Point):void
		{
		    Zippo.SendSkill(GameCommonData.Player.Role.UsingPetAnimal.Role.Id,
		                    0,
		                    point.x,
							point.y,
							INTERACT_SHOOT,
							skill,level);
		}
		
		/**人的BUFF**/
	    public function ReserveBuffSkillAttack(skill:int,level:int,targetAnimal:GameElementAnimal):void
		{
			if(GameCommonData.Player.Role.HP > 0)
			{	
				var date:Date = new Date();
				if(date.time - CombatController.skillTime < 1000)
				{
					return;							
				}
				
			    Zippo.SendBuffSkill(GameCommonData.Player.Role.Id,
			                    targetAnimal.Role.Id,
			                    0,
								0,
								INTERACT_SHOOT,
								skill,level);
			}
		}
		
		public function ReserveDirSkillAttack(skill:int,level:int,targetAnimal:GameElementAnimal):void
		{
		    Zippo.SendSkill(GameCommonData.Player.Role.Id,
		                    0,
		                    0,
							0,
							INTERACT_SHOOT,
							skill,level, GameCommonData.Player.Role.Direction);
		}
		
		/**宠物的buff**/
		 public function ReservePetBuffSkillAttack(skill:int,level:int,targetAnimal:GameElementAnimal):void
		{
			if(GameCommonData.Player.Role.UsingPetAnimal != null)
			{
			    Zippo.SendBuffSkill(GameCommonData.Player.Role.UsingPetAnimal.Role.Id,
			                    targetAnimal.Role.Id,
			                    0,
								0,
								INTERACT_SHOOT,
								skill,level);
			}
		}
		
		/** 回复HP提示  */
		public static function RestoreHPPrompt(target:DisplayObject, hp:int):void
		{
			Prompt(target, hp.toString(), "CombatPrompt_Right_Restore");
		}
		
		/** 回复MP提示  */
		public static function RestoreMPPrompt(target:DisplayObject, hp:int):void
		{
			Prompt(target, hp.toString(), "CombatPrompt_Right_Magic");
		}
		
		/** 攻击目标提示扣血提示 */
		public static function AttackPrompt(target:DisplayObject, hp:int,isCriticalHit:Boolean):void
		{
			if(isCriticalHit)
			{
				Prompt(target, hp.toString(), "CombatPrompt_UP_TargerDamage_B");
			}
			else
			{
				Prompt(target, hp.toString(), "CombatPrompt_UP_TargerDamage");
			}
		}
		
		/** 攻击目标提示扣血提示 */
		public static function PetAttackPrompt(target:DisplayObject, hp:int,isCriticalHit:Boolean):void
		{
			if(isCriticalHit)
			{
				Prompt(target, hp.toString(), "CombatPrompt_UP_C_TargerDamage_B");
			}
			else
			{
				Prompt(target, hp.toString(), "CombatPrompt_UP_C_TargerDamage");
			}
		}
		
		
		
		
		/** 被攻击提示  */
		public static function ByAttackPrompt(target:DisplayObject, hp:int,isCriticalHit:Boolean):void
		{
			if(isCriticalHit)
			{
				Prompt(target, hp.toString(), "CombatPrompt_Left_Damage_B");
			}
			else
			{
				Prompt(target, hp.toString(), "CombatPrompt_Left_Damage");
			}
		}
		
		/** 闪避 */
		public static function EvasionPrompt(target:DisplayObject):void
		{
			Prompt(target, "", "CombatPrompt_Left_Evasion");
		}
		
	    /** 吸收 */
		public static function SuckPrompt(target:DisplayObject):void
		{
			Prompt(target, GameCommonData.wordDic["con_com_suck_1"], "CombatPrompt_Left_Evasion"); // 吸收
		}
		
		/**技能提示*/
		public static function Skill(target:DisplayObject,text:String):void
		{
			Prompt(target, text, "CombatPrompt_UP_Skill");		
		}
		
	    /**加血提示*/
		public static function SkillAddHP(target:DisplayObject,text:String,px:int,py:int):void
		{
			var prompt:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("CombatPrompt_Right_Restore");
			if(text!="")
			{
				prompt.Container.txtPrompt.text = text;
			}
			prompt.x = px - 15;	
			prompt.y = py;
			GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(prompt);		
		}

		
		/** 提示显示 */
		public static function Prompt(target:DisplayObject, text:String,resourcesName:String):void
		{
			var prompt:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(resourcesName);
			if(text!="")
			{
				prompt.Container.txtPrompt.text = text;
			}
			prompt.x = target.x - 105;
			prompt.y = target.y;
			GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(prompt);
		}	
	}
}
