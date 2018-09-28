package OopsEngine.Role
{
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	import OopsEngine.Scene.StrategyElement.Person.GameElementTernal;
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	/** 游戏人物角色－玩家在游戏中的职业 */
	public class GameRole
	{	
		/** 交易状态 */
		public static const STATE_TRADE:String = "state_trade";
		/** 摆摊状态 */
		public static const STATE_STALL:String = "state_stall";
		/** 打开仓库状态 */
		public static const STATE_DEPOT:String = "state_depot";
		/** 正在查看摆摊 */
		public static const STATE_LOOKINGSTALL:String   = "state_lookingStall";
		/** 正在查看NPC商店 */
		public static const STATE_LOOKINGNPCSHOP:String = "state_lookingNPCShop";
		/** 无状态 */
		public static const STATE_NULL:String  = null;
		/** 附加状态  1 中毒**/
		public var AdditionalState:int = 0;
		
		public var isAddEffect:Boolean = false;
		
		/** 人物类型 开始  */
		public static const TYPE_OWNER:String  = "主角";	
		public static const TYPE_PLAYER:String = "玩家";
		public static const TYPE_ENEMY:String  = "敌人";
		public static const TYPE_NPC:String	   = "NPC";
		public static const TYPE_STALL:String  = "摊位";
		public static const TYPE_PET:String    = "宠物";
		public static const TYPE_BANNER:String = "旗帜";
		
		/** 人物类型 结束  */
		
		//---------------公共信息---------------
		/** 是否为VIP 0 1 2 3级0是没有VIP  */
		public var VIP:int = 0;
		/** VIP字样颜色  */
		public var VIPColor:String = null;
		
		/** 角色唯一编号 */
		public var Id:int;	
		/** 人物名 */
		public var Name:String;
		/** 人物名颜色 */
		public var NameColor:String = "#ffffff";
		/** 人物名字体边线颜色 */
		public var NameBorderColor:uint = 0x000000;
		/** 人物称号 */
		public var Title:String;
		/** 人物称号颜色 */
		public var TitleColor:uint;
		/** 人物称号体边线颜色 */
		public var TitleBorderColor:uint = 0x000000;
		/** 人物性别 */
		public var Sex:uint;
		/** 体力值 */
		public var HP:int;
		/** 最大体力值 */
		public var MaxHp:int;
		/** 魔力值 */
		public var MP:int;
		/** 最大魔力值 */
		public var MaxMp:int;
		/** 怒气值 */
		public var SP:int;
		/** 最大怒气值 */
		public var MaxSp:int;
		/** 人物等级 */
		public var Level:uint;
		/** 经验 */
		public var Exp:uint;
		/** 头像 */
		public var Face:uint;
		/** 角色类型（玩家，任务NPC，怪物NPC）*/
		public var Type:String;
		/** PKteam 阵营**/		
		public var PKteam:int = 0;	
		/** 是否在队伍中  */
		public var IsInTeam:Boolean = false;
		/** 默认技能 */
		public var DefaultSkill:int = 0;
		/** NPC任务当前任务状态 */
		public var MissionState:int = 0; // 0 没任务，3完成，2未接，1接了未完成 优先显示大的状态图标
		
		/**是否是试衣间*/
		public var isSkinTest:Boolean = false;
		
		public var idMonsterType:uint=0;

//		/** 当前使用技能  */
//		public var UseSkill:int = 0;
//		/** 下一招使用技能  */
//		public var NextSkill:int = 0;
        /** 人物皮肤（人物类型）**/
        public var MonsterTypeID:int = 0;

		/** 战力评分  */
		public var Score:uint = 0;
		/** 背包等级  */
		public var BagLevel:uint = 0;
		/** 仓库等级 */
		public var DepotLevel:uint = 0; 
		
//		/** 宠物背包等级  */
//		public var PetBagLevel:uint = 10; 
//		/** 宠物仓库等级 */
//		public var PetDepotLevel:uint = 10;
  
		/** 玩家当前状态  */
		public var State:String;
		/** 是否正在攻击  */
//		public var IsAttacking:Boolean = false;
		/** 技能列表 */
		public var SkillList:Dictionary = new Dictionary();
		/** 生活技能列表 */
		public var LifeSkillList:Dictionary = new Dictionary();
		/** 宠物快照列表 */
		public var PetSnapList:Dictionary = new Dictionary();
		/** 宠物数据列表 */
		public var PetList:Dictionary = new Dictionary();
		/** 当前使用中的宠物数据信息 */
		public var UsingPet:GamePetRole = null;
		/** 当前宠物的对象的信息*/
		public var UsingPetAnimal:GameElementPet;
		/** 当前主人对象**/
		public var MasterPlayer:GameElementPlayer; 
		/** 是否为简单 NPC **/
		public var IsSimpleNPC:Boolean = false;

		
		//---------------玩家信息---------------
		/** 是否显示心情0为显示 */
		public var IsShowFeel:uint=0;
		/** 心情 */
		public var Feel:String;
		/** 角色经验值  */
		public var EXT:uint
		/** 主职业  */
		public var MainJob:RoleJob = new RoleJob();
		/** 副职业  */
		public var ViceJob:RoleJob = new RoleJob();
		/** 职业列表 */
		public var RoleList:Array = [MainJob, ViceJob];
		/** 附加属性  */
		public var AdditionAtt:AdditionalAtt = new AdditionalAtt();
		/** 冰火玄毒八属性数组*/
		public var AttendPro:Array = new Array(8);
		/** 当前选职业 1主 2副 服务器发的参数据为1,2，在到RoleList在获取当前职业 */
		public var CurrentJob:uint
		/** 精力（生活技能用）*/
		public var Vit:uint;
		/** 最大精力（生活技能用）*/
		public var MaxVit:uint;
		/** 活力（生活技能用）*/
		public var Ene:uint;
		/** 最大活力（生活技能用）*/
		public var MaxEne:uint;
		/** PK值 */
		public var PkValue:uint;
		/** PK保护状态 */
		public var PkState:uint = 0;
		/** 绑定货币 */
		public var BindMoney:uint;
		/** 非绑定货币 */
		public var UnBindMoney:uint;
		/** 赠卷 (点券)*/
		public var GiveAway:uint
		/** 绑定人民币 (元宝)*/
		public var BindRMB:uint;
		/** 非绑定人民币 (珠宝)*/
		public var UnBindRMB:uint;
		/** 存款  */
		public var SaveMoney:uint;
		/** 是否为队长 */
		public var IsCaptain:Boolean = false;
		/** 道具列表 */
//		public var PropertyList:DictionaryCollection = new DictionaryCollection();
		public var Savvy:int
		/**透明度**/
		public var WeaponDiaphaneity:Number = 1; 
		
		/**  人物皮肤 */ 
	    private var _PersonSkinName	:String;
		/** 人物皮肤名  */
		public function set PersonSkinName(value:String):void
		{  
			if(Type == GameRole.TYPE_OWNER)
			{
				CommonData.owner[value] = true;
			}
			_PersonSkinName = value;
		}
		public function get  PersonSkinName():String
		{  
			return this._PersonSkinName;
		}
		
		/** 人物光影 */
		public var _PersonSkinEffectName:String;
		public function set PersonSkinEffectName(value:String):void
		{  
			if(Type == GameRole.TYPE_OWNER)
			{
				CommonData.owner[value] = true;
			}
			_PersonSkinEffectName = value;
		}
		public function get PersonSkinEffectName():String
		{  
			return this._PersonSkinEffectName;
		}
					
		/** 武器皮肤名 */
		public var _WeaponSkinName:String;
		public function set WeaponSkinName(value:String):void
		{  
			if(Type == GameRole.TYPE_OWNER)
			{
				CommonData.owner[value] = true;
			}
			_WeaponSkinName = value;
		}
		public function get WeaponSkinName():String
		{  
			return this._WeaponSkinName;
		}
		
		/** 武器皮肤名 */
		public var _WeaponEffectName:String;
		public function set WeaponEffectName(value:String):void
		{  
			if(Type == GameRole.TYPE_OWNER)
			{
				CommonData.owner[value] = true;
			}
			_WeaponEffectName = value;
		}
		public function get WeaponEffectName():String
		{  
			return this._WeaponEffectName;
		}
		
		/** 坐骑皮肤名 */
		public var _MountSkinName:String;
		public function set MountSkinName(value:String):void
		{  
			if(Type == GameRole.TYPE_OWNER)
			{
				CommonData.owner[value] = true;
			}
			_MountSkinName = value;
		}
		public function get MountSkinName():String
		{  
			return this._MountSkinName;
		}
		

		/**武器编号**/
		public var WeaponSkinID:int;
		/**皮肤光影编号**/
		public var PersonSkinEffectID:int;
		/**皮肤编号**/
		public var PersonSkinID:int;
		/**坐骑编号**/
		public var MountSkinID:int;	
		/** 武器光影编号 **/
		public var WeaponEffectModel:int;
		/** 武器光影样式 **/
		public var WeaponEffectModelName:String;
		/**当前选中职业编号**/
	    public var CurrentJobID:int;
		/**是否隐身**/
		public var isHidden:Boolean = false;
		/**是否显示时装**/
	    public var IsShowDress:Boolean = true;
	    
	    public var playeffectID:int = 0;
	    
		/** 人物方向 */
		public var Direction:int      = GameElementSkins.DIRECTION_DOWN;
		/** 人物当前动做状态 */
		public var ActionState:String = GameElementSkins.ACTION_STATIC;
		/** 人物A*格子X轴坐标 */
		public var TileX:int;
		/** 人物A*格子Y轴坐标 */
		public var TileY:int;
		
		/** 摊位编号  */
		public var StallId:int;
		/** 人物的队伍ID号*/
		public var idTeam:uint = 0;
		/** 队长ID*/
		public var idTeamLeader:uint = 0;
		/** 帮会ID*/
		public var unityId:uint;
		/** 帮会职业 */
		public var unityJob:uint;
		/** 帮会贡献度 */
		public var unityContribution:uint;
		/**攻击时间**/
		public var AttackTime:Number = 0;
		/**切磋时间**/
		public var DuelTime:Number = 0;
		/**领取在线奖励次数**/
		public var OnLineAwardTime:uint = 0;
		/**人物称号*/
		public var DesignationCall:uint = 0;
		/** 攻击者**/
		public var Aggressor:Dictionary = new Dictionary();
		/**下个皮肤切换的属性**/
		public var skinNameController:SkinNameController;
		/** 武魂 */
		public var gameElementTernal:GameElementTernal;
		/** 真元 */
		public var archaeus:int;
		/** 减少真气百分比 */
		public var MPPercentage:int = 100;
		
		public var showSkinPoint:Point = null;
		/** 是否变异 **/
		public var variation:int = 0 ;
//		/** 攻击时间**/
//		public var AggressorTime:Number;
		
		/**是否是攻击状态**/
		public function get IsAttack():Boolean
		{
			var time:Date = new Date();
			//判断是否超过10秒
			if(time.time - AttackTime > 5000)
			{
				return false;
			}	
			else
			{
				return true;
			}
		}
		
		/**是否是攻击状态**/
		public function get IsDuel():Boolean
		{
			var time:Date = new Date();
			//判断是否超过5秒
			if(time.time - DuelTime > 5000)
			{
				return false;
			}	
			else
			{
				return true;
			}
		}
		
		/**更新切磋状态**/
		public function UpdateDuelTime():void
		{
			var time:Date = new Date();
			DuelTime      = time.time;
		}
		
		/**更新交战时间**/
		public function UpdateAttackTime():void
		{
			var time:Date = new Date();
			AttackTime    = time.time;
		}
		
		//增益BUFF列表
		public var PlusBuff:Array = new Array();
		//Dot Buff列表
		public var DotBuff:Array = new Array();
						
		//是否包含改BUFF
		public function IsBuff(BuffID:int):Boolean
		{
			 for(var n:int = 0;n <PlusBuff.length;n++)
			 {
			 	var buff:GameSkillBuff =  PlusBuff[n] as GameSkillBuff;
			 	if(buff.BuffID == BuffID)
			 	{
			 		return true;
			 	}
			 }			 
			 return false;
		}	
		
		public function IsDot(DotID:int):Boolean
		{
			 for(var n:int = 0;n <DotBuff.length;n++)
			 {
			 	var buff:GameSkillBuff =  DotBuff[n] as GameSkillBuff;
			 	if(buff.BuffID == DotID)
			 	{
			 		return true;
			 	}
			 }			 
			 return false;
		}
		
		public function DelteDot(dot:GameSkillBuff):Boolean
		{
			 for(var n:int = 0;n <DotBuff.length;n++)
			 {
			 	if(DotBuff[n].BuffID == dot.BuffID)
			 	{
			 		 DotBuff.splice(n,1);
			 		 return true;
			 	}
			 }	
			 return false;		 
		}
		
		public function DelteBuff(buff:GameSkillBuff):Boolean
		{
			 for(var n:int = 0;n <PlusBuff.length;n++)
			 {
			 	if(PlusBuff[n].BuffID == buff.BuffID)
			 	{
			 		 PlusBuff.splice(n,1);
			 		 return true;
			 	}
			 }	
			 return false;		 		 
		}
		
		public function UpdateDot(dot:GameSkillBuff):void
		{
			 for(var n:int = 0;n <DotBuff.length;n++)
			 {
			 	if(DotBuff[n].BuffID == dot.BuffID)
			 	{
			 		DotBuff[n] = dot;
			 		return;
			 	}
			 }		

			 DotBuff.push(dot);
		}
		
		public function UpdateBuff(buff:GameSkillBuff):void
		{
			 for(var n:int = 0;n <PlusBuff.length;n++)
			 {
			 	if(PlusBuff[n].BuffID == buff.BuffID)
			 	{
			 		 PlusBuff[n] = buff;
			 		 return;
			 	}
			 }		

			 PlusBuff.push(buff);	 
		}
		
		public function IsChangeDirection():Boolean
		{
			if(this.ActionState != GameElementSkins.ACTION_DEAD)
			   return true;
			else 
			   return false;
		}
		
		public function IsChangeActionState():Boolean
		{
			if(this.ActionState != GameElementSkins.ACTION_DEAD && this.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
			   return true;
			else 
			   return false;
		}
		
		//当前技能编号
		public var UseSkill:int = 0;
		//下个技能编号
		public var NextSkill:int = 0;
		//属性
		public var TernalType:int = 0;
		//属相
		public var TernalMutually:int = 0;
		//武魂级别
		public var TernalLevel:int = 0;
		//成长率
		public var TernalGrow:int = 0;
		//合成等级
		public var TernalMixLev:int = 0;
		// 是否携带武魂
		public  var isTernal:Boolean;       
//		//下个技能的对象
//		public var target:GameElementAnimal;
		
//		/**设置技能*/
//		public function SetGameSkill(skillID:int):void
//		{
//			if(this.UseSkill == 0)
//			{
//				this.UseSkill = skillID;
//			}
//			else
//			{
//				this.NextSkill = skillID;
//			}
//		}
//		
//		/**使用技能*/
//		public function UseGameSkill():void
//		{
//			if(this.UseSkill != 0)
//			{	
//				this.UseSkill = 0;	
//				this.UseSkill = this.NextSkill ;
//			}
//		}

		
//		/** 设置当前使用技能  */
//		public function SetUseSkill(SkillId:int):void
//		{
////			this.UseSkill = this.SkillList[SkillId];
//			//this.UseSkill = new GameSkill();
//		}
//		
//		/** 设置下一招使用技能  */
//		public function SetNextSkill(SkillId:int):void
//		{
////			this.NextSkill = this.SkillList[SkillId];
//			//this.NextSkill = new GameSkill();
//		}

		// 竞技场荣誉值
		public var arenaScore: int;
	}
}