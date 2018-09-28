package Net.ActionProcessor
{
	import Controller.PetController;
	
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Proxy.DataProxy;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.utils.ByteArray;

	public class PetInfoAllAction extends GameAction
	{
		private var dataProxy:DataProxy;
		
		public function PetInfoAllAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
		}
		
		/**  处理接收到的消息 */
		public override function Processor(bytes:ByteArray):void 
		{
		 	bytes.position = 4;
		 	var eudemoninfo:GamePetRole = new GamePetRole();	
		 	
			eudemoninfo.Id = bytes.readUnsignedInt();				//id
		 	eudemoninfo.OwnerId = bytes.readUnsignedInt();			//拥有者ID
		 	eudemoninfo.ClassId = bytes.readUnsignedInt();			//类型 兔子 
		 	eudemoninfo.TakeLevel = bytes.readUnsignedInt();		//携带等级（需人物达到该等级才可携带）
		 	eudemoninfo.HpNow = bytes.readUnsignedInt();			//HP
		 	eudemoninfo.ExpNow = bytes.readUnsignedInt();			//当前经验
		 	
		 	eudemoninfo.FaceType = bytes.readUnsignedShort();		//头像
		 	eudemoninfo.Potential = bytes.readUnsignedShort();		//潜力点
		 	eudemoninfo.Genius = bytes.readUnsignedShort();			//天赋
		 	eudemoninfo.Grade = bytes.readUnsignedShort();			//成长率
		 	eudemoninfo.Savvy = bytes.readUnsignedShort();			//悟性
		 	eudemoninfo.Level = bytes.readUnsignedShort();			//等级
		 	eudemoninfo.Type = bytes.readUnsignedShort();			//类型 0野生 1宝宝 2二代
		 	eudemoninfo.HappyNow = bytes.readUnsignedShort();		//当前快乐值
		 	eudemoninfo.Character = bytes.readUnsignedShort();		//性格
		 	if(PetPropConstData.isNewPetVersion)
		 	{
		 		eudemoninfo.BreedNow = bytes.readUnsignedByte()		//繁殖代数
		 		eudemoninfo.playNumber = bytes.readUnsignedByte();	//玩耍次数
		 	}
		 	else
		 	{
			 	eudemoninfo.BreedNow = bytes.readUnsignedShort();		//繁殖代数
		 	}
		 	eudemoninfo.State = bytes.readUnsignedShort();			//状态  休息 战斗中
		 	eudemoninfo.Sex = bytes.readUnsignedShort();		 	//性别
		 	if(PetPropConstData.isNewPetVersion)	//新版宠物
	 		{ 
	 			if(eudemoninfo.Sex == 2)	//已幻化
	 			{
	 				eudemoninfo.isFantasy = true;	
	 			}
	 			eudemoninfo.privity = bytes.readUnsignedShort();	//默契
	 			eudemoninfo.winning = bytes.readUnsignedShort();	//灵性
	 		 }    
		 	eudemoninfo.Appraise = bytes.readUnsignedInt();			//评价  评分
		 	eudemoninfo.LifeNow = bytes.readUnsignedInt();			//当前寿命
		 	eudemoninfo.LifeMax = bytes.readUnsignedInt();			//最大寿命
		 	
		 	var addPow:Number = Math.pow(1.06, eudemoninfo.Type); 
		 	
		 	eudemoninfo.ForceInitMax = bytes.readUnsignedInt() * addPow;		//初始最大力量资质 
		 	eudemoninfo.SpiritPowerInitMax = bytes.readUnsignedInt() * addPow;	//初始最大灵力资质		//bytes.readUnsignedInt() * (0.9 + eudemoninfo.Type * 0.1);
		 	eudemoninfo.PhysicalInitMax = bytes.readUnsignedInt() * addPow;		//初始最大体力资质
		 	eudemoninfo.ConstantInitMax = bytes.readUnsignedInt() * addPow;		//初始最大定力资质
		 	eudemoninfo.MagicInitMax = bytes.readUnsignedInt() * addPow;		//初始最大身法资质
		 	
		 	eudemoninfo.ForceInit = bytes.readUnsignedInt();		//初始力量资质
		 	eudemoninfo.SpiritPowerInit = bytes.readUnsignedInt();	//初始灵力资质
		 	eudemoninfo.PhysicalInit = bytes.readUnsignedInt();		//初始体力资质
		 	eudemoninfo.ConstantInit = bytes.readUnsignedInt();		//初始定力资质
		 	eudemoninfo.MagicInit = bytes.readUnsignedInt();		//初始身法资质
		 	
		 	eudemoninfo.ForceApt = bytes.readUnsignedInt();			//力量资质
		 	eudemoninfo.SpiritPowerApt = bytes.readUnsignedInt();	//灵力资质
		 	eudemoninfo.PhysicalApt = bytes.readUnsignedInt();		//体力资质
		 	eudemoninfo.ConstantApt = bytes.readUnsignedInt();		//定力资质
		 	eudemoninfo.MagicApt = bytes.readUnsignedInt();			//身法资质
		 	
		 	eudemoninfo.Force = bytes.readUnsignedInt();			//力量
		 	eudemoninfo.SpiritPower = bytes.readUnsignedInt();		//灵力
		 	eudemoninfo.Physical = bytes.readUnsignedInt();			//体力
		 	eudemoninfo.Constant = bytes.readUnsignedInt();			//定力
		 	eudemoninfo.Magic = bytes.readUnsignedInt();			//身法
		 	
		 	eudemoninfo.PhyAttack = bytes.readUnsignedInt();		//外攻
		 	eudemoninfo.MagicAttack = bytes.readUnsignedInt();		//内攻
		 	eudemoninfo.PhyDef = bytes.readUnsignedInt();			//外防
		 	eudemoninfo.MagicDef = bytes.readUnsignedInt();			//内防
		 	eudemoninfo.Hit = bytes.readUnsignedInt();				//命中
		 	eudemoninfo.Hide = bytes.readUnsignedInt();				//躲闪
		 	eudemoninfo.Crit = bytes.readUnsignedInt();				//暴击
		 	eudemoninfo.Toughness = bytes.readUnsignedInt();		//坚韧
		 	eudemoninfo.HpMax = bytes.readUnsignedInt();			//最大生命
		 	
		 	if(eudemoninfo.Type != 1) eudemoninfo.BreedMax = 0;
//		 	
			var skillNum:int;
			if(PetPropConstData.isNewPetVersion)
			{
				skillNum = 10;
			}
			else
			{
				skillNum = 7;
			}
		 	for(var j:int = 0; j < skillNum; j++) {
		 		 var skill:uint =  bytes.readUnsignedInt();
		 		 if(PetPropConstData.isNewPetVersion)
		 		 {
			 		 if(skill == 0)		//第一个技能格子，始终放主动技能（或回血，回蓝），没有就空着
			 		 {
			 		 	if(j == 0) 
			 		 	{
			 		 		eudemoninfo.SkillLevel.push(0);
			 		 	}
			 		 }
			 		 else
			 		 {  
			 		 	var idNew:int = skill/1000;
	//		 		 	var lev:int = skill % 100;
					 	var gameSkillNew:GameSkill = GameCommonData.SkillList[idNew] as GameSkill;
					 	var gameSkillLevelNew:GameSkillLevel = new GameSkillLevel(gameSkillNew);
					 	gameSkillLevelNew.Level = eudemoninfo.Level;//lev;
					 	eudemoninfo.SkillLevel.push(gameSkillLevelNew);
			 		 }
		 		 }
		 		 else
		 		 {
		 		 	if(skill != 0)
		 		 	{
			 		 	var id:int = skill/1000;
	//		 		 	var lev:int = skill % 100;
					 	var gameSkill:GameSkill = GameCommonData.SkillList[id] as GameSkill;
					 	var gameSkillLevel:GameSkillLevel = new GameSkillLevel(gameSkill);
					 	gameSkillLevel.Level = eudemoninfo.Level;//lev;
					 	eudemoninfo.SkillLevel.push(gameSkillLevel);
		 		 	}
		 		 }
		 	}
		 	
		 	var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int = 0;				
			for(var i:int = 0;i < nDataSeeNum; i++)
			{
				nDataSee = bytes.readByte();
				if(nDataSee != 0)
				{
					if(i == 0)
					{
						eudemoninfo.PetName = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); //名字
					}
				}
			}
			if(eudemoninfo.OwnerId == GameCommonData.Player.Role.Id) {
				if(GameCommonData.Player.Role.PetSnapList[eudemoninfo.Id]) GameCommonData.Player.Role.PetSnapList[eudemoninfo.Id] = eudemoninfo;
				GameCommonData.Player.Role.PetList[eudemoninfo.Id] = eudemoninfo;
			} 
			if(eudemoninfo.State == 1 && eudemoninfo.OwnerId == GameCommonData.Player.Role.Id) { //自己的宠物出战
				GameCommonData.Player.Role.UsingPet = eudemoninfo;
				sendNotification(PetEvent.PET_TO_FIGHT_AFTER_GETINFO);
				PetController.BuffUseSkill();
			}
			//普通查询详细信息
			sendNotification(PetEvent.RETURN_TO_SHOW_PET_INFO, eudemoninfo);
		}
		
	}
}