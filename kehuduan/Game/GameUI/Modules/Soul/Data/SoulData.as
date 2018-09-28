package GameUI.Modules.Soul.Data
{
	
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	
	import flash.utils.Dictionary;
	
	public class SoulData
	{
		/**魂魄type*/
		public static const soulType:int = 250000;
		/**所有魂魄的详细信息*/
		public static var SoulDetailInfos:Dictionary = new Dictionary();
		/** 其他属性 */
		public static var other:Object;
		/** 扩展属性管理 */
		public static var attributesManagement:Array = [];
		/** 合成率 */
		public static var growth:Array = [];
		/** 魂魄合成 */
		public static var compound:Array = [];
		/** 合玉 */
		public static var jade:Array = [];  
		/** 技能花费信息 */
		public static var skill:Array = [];
		/** 技能值 */
		public static var skillCompute:Array = [12,21,39,69,110,165,234,319,420,538];
		/** 技能增值*/
		public static var skillAttack:Dictionary = new Dictionary();
		/**魂魄小悬浮框信息*/
		public static var soulToolTipInfo:Array = [						
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_1" ], 								//0  魂魄属相
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_2" ],  											//1  装备预留点
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_3" ],									//2  当前经验
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_4" ],									//3  成长率
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_5" ],	//4  合成等级
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_6" ],									//5  身，定，体，灵，力
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_7" ],	//6  扩展属性技能
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_8" ],								//7  属性槽（未学习）
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_9" ],								//8  属性槽（未开槽）
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_10" ],								//9  属性槽（不可开启）
			"1",			//10  扩展属性重洗刷新（可以刷新）
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_11" ],					//11  技能（已经学习的）
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_12" ],		//12  可学习的技能槽
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_13" ],							//13 技能说明
			"2",//14  技能重置刷新
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_14" ],		  //15  寿命	
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_15" ],		  //16 等级	
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_16" ],		  //17 力量型	
			GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_17" ]		  //18 灵力型	
		];
		/* public static var soulToolTipInfo:Array = [						
			"当合成等级达到5时，魂魄随机获得一种属相，在战斗中会直接放大伤害。属相有相克关系：地克水、水克火、火克风、风克地，怪物及没有属相的魂魄，会被所有属相克制。", 								//0  魂魄属相
			"点击查看魂魄属性",  											//1  装备预留点
			"使用魂魄珠可以使魂魄获得经验，经验满自动升级。魂魄珠可通过击杀副本小怪获得。",									//2  当前经验
			"成长率越高，魂魄的原始属性值越高。分为普通、优秀、杰出、卓越、完美五个档次。",									//3  成长率
			"合成等级越高，扩展属性和魂魄技能的效果也越明显，能开辟的扩展属性槽也越多。合成等级达到5级可获得属相和对应的新外观。",	//4  合成等级
			"原始属性直接附加在人物属性上",									//5  身，定，体，灵，力
			"点击升级",	//6  扩展属性技能
			"点击学习",								//7  属性槽（未学习）
			"点击开槽",								//8  属性槽（未开槽）
			"不可开槽",								//9  属性槽（不可开启）
			"1",			//10  扩展属性重洗刷新（可以刷新）
			"此技能在战斗中自动释放，点击此技能可以进行升级",					//11  技能（已经学习的）
			"魂魄级别达到40，70，90级时，可以领悟新的技能。",		//12  可学习的技能槽
			"点击技能图标升级",							//13 技能说明
			"2",//14  技能重置刷新
			"玩家每死亡一次，寿命减少10点。当寿命为0时魂魄不再生效，可使用魂魄延寿丹将魂魄寿命回满。",		  //15  寿命	
			"随着等级提高，魂魄的原始属性值会越大。当前魂魄的等级不能超过人物等级。",		  //16 等级	
			"力量型魂魄适合门派：少林、丐帮、点苍",		  //17 力量型	
			"灵力型魂魄适合门派：全真、峨眉、唐门"		  //18 灵力型	
		]; */
		
		/**
		 * 
		 * @param skillID     技能编号
		 * @param skillLevel  技能等级
		 * @param mixLev      合成等级
		 * @param soulLevel   武魂等级
		 * @return 
		 * 
		 */		
		public static function computeSkillAttack(skillID:int,skillLevel:int,mixLev:int,soulLevel:int):Object
		{
			var objectInfo:Object = skillAttack[skillID];
			var levparam:Number = objectInfo.levparam;
			var mixparam:Number = objectInfo.mixparam;
			var nTempData:int = Math.round(levparam * skillCompute[skillLevel - 1] + mixparam * mixLev * mixLev);
			if(objectInfo.isSoul == 1)
			{
				nTempData = nTempData * soulLevel/100 + 1;
			}
			var object:Object = new Object();
			object.skillName = objectInfo.skillName;
			object.nTempData = nTempData; //属性加成
			return object;
		}
		
		public static function getAttributesInfo(lev:int):Object
		{
			return attributesManagement[lev];
		}
		
		public static function getGrowth(value:int):Object
		{
			var growthInfo:Object = {};
			for(var n:int = 0;n < growth.length;n++)
			{
				growthInfo.gold = growth[n].gold;
				growthInfo.compound = int(50 - (value - 500)/10); 
				if(value >= growth[n].min && value <= growth[n].max)
				{
					break;
				}								
			}
			return growthInfo;
		}

		public static function getSoulDetailById(id:int):SoulVO
		{
			var soulVo:SoulVO;
			if(SoulDetailInfos[id])
			{
				soulVo = SoulDetailInfos[id];
			}
			else
			{
				SoulProxy.getSoulDetailInfoFromBag(id);
			}
			return soulVo;
		}
		
		
	}
}