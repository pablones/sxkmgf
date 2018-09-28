package GameUI.Modules.Meridians.model
{
	import GameUI.Modules.Meridians.Components.MeridiansTimeOutComponent;
	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;
	
	public class MeridiansData extends Proxy
	{
				
		public static const NAME:String="MeridiansProxy";
				
		public static var meridiansVO:MeridiansVO = null;
		public static var instance:MeridiansData = null;
		
		public static var meridianEffectDic:Dictionary = null;				//经脉效果
		public static var allMeridiansStrengthDic:Dictionary = null;			//所有经脉强化等级及其效果
		public static var allMeridiansGradeDic:Dictionary = null;				//所有经脉等级及其效果
		public static var meridiansGradeDic:Dictionary = null;				//经脉强化及其信息	
		public static var meridiansUpgradeCondition:Array = null;
		
//		wordDic[ "Mod_Mer_mod_MeridiansData_name_1" ] = "阳跷";

		public static var meridiansNames:Array = ["",
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_1" ],//"阳跷"
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_2" ], //= "阴跷";
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_3" ], //= "阳维";
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_4" ], //= "阴维";
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_5" ], //= "带脉";
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_6" ], //= "冲脉";
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_7" ], //= "督脉";
					GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_name_8" ] //= "任脉";
					];							//经脉名称	

		public static function initDic():void
		{
			meridianEffectDic = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["meridianEffectDic"] as Dictionary;
			allMeridiansStrengthDic = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["allMeridiansStrengthDic"] as Dictionary;
			meridiansGradeDic = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["meridiansGradeDic"] as Dictionary;
			allMeridiansGradeDic = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["allMeridiansGradeDic"] as Dictionary;
			meridiansUpgradeCondition = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["meridiansUpgradeCondition"] as Array;
		}
		
		public static var numbers:Array = [
				GameCommonData.wordDic["CNNumber_0"],//="零";
				GameCommonData.wordDic["CNNumber_1"],//="一";
				GameCommonData.wordDic["CNNumber_2"],//="二";
				GameCommonData.wordDic["CNNumber_3"],//="三";
				GameCommonData.wordDic["CNNumber_4"],//="四";
				GameCommonData.wordDic["CNNumber_5"],//="五";
				GameCommonData.wordDic["CNNumber_6"],//="六";
				GameCommonData.wordDic["CNNumber_7"],//="七";
				GameCommonData.wordDic["CNNumber_8"],//="八";
				GameCommonData.wordDic["CNNumber_9"],//="九";
				GameCommonData.wordDic["CNNumber_10"],//="十";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_1"],//="一";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_2"],//="二";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_3"],//="三";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_4"],//="四";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_5"],//="五";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_6"],//="六";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_7"],//="七";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_8"],//="八";
				GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_9"],//="九";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"],/*"十"*/
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_1"],//="一";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_2"],//="二";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_3"],//="三";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_4"],//="四";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_5"],//="五";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_6"],//="六";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_7"],//="七";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_8"],//="八";
				GameCommonData.wordDic["CNNumber_2"] /*"二"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/+GameCommonData.wordDic["CNNumber_9"],//="九";
				GameCommonData.wordDic["CNNumber_3"] /*"三"*/+GameCommonData.wordDic["CNNumber_10"]/*"十"*/
//				"零","一","二","三","四","五","六","七","八","九","十",
//				"十一","十二","十三","十四","十五","十六","十七","十八","十九","二十",
//				"二十一","二十二","二十三","二十四","二十五","二十六","二十七","二十八","二十九","三十"
		];
		
		public static var MeridiansExplain:Array = ["",
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_1" ],// =		"提升血上限";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_2" ],// =		"提升气上限";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_3" ],// =		"提高闪避率";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_4" ],// =		"增加韧性";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_5" ],// =		"增加命中";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_6" ],// =		"增加暴击率。";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_7" ],// =		"增加内攻攻击";
			GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansData_explain_8" ]// =			"增加外攻攻击";
		];
		
		public function MeridiansData()
		{
			super(NAME);
		}
		
		public static function getInstance():MeridiansData
		{
			if(instance == null)
			{
				instance = new MeridiansData();
			}
			return instance;
		}
		
//		public function initMeridiansVO(meridiansVO:MeridiansVO):void
//		{
//			MeridiansProxy.meridiansVO = meridiansVO;
//			
//		}
		
//		public static function setMeridiansVO(newMeridiansVO:MeridiansVO):void
//		{
//			meridiansVO.roleID = newMeridiansVO.roleID;
//			meridiansVO.nAmount = newMeridiansVO.nAmount;
//			meridiansVO.nAllLevGrade = newMeridiansVO.nAllLevGrade;
//			meridiansVO.nAllStrengthLevAdd = newMeridiansVO.nAllStrengthLevAdd;
//			
//			for(var i:int = 0; i< 8 ; i++)
//			{
//				if(meridiansVO.meridiansArray[i] != null)
//				{
//					meridiansVO.meridiansArray[i].nType = newMeridiansVO.meridiansArray[i].nType;
//					meridiansVO.meridiansArray[i].nLev = newMeridiansVO.meridiansArray[i].nLev;
//					meridiansVO.meridiansArray[i].nLeaveTime = newMeridiansVO.meridiansArray[i].nLeaveTime;
//					
//					meridiansVO.meridiansArray[i].nStrengthLev = newMeridiansVO.meridiansArray[i].nStrengthLev;
//					meridiansVO.meridiansArray[i].nState = newMeridiansVO.meridiansArray[i].nState;
//					meridiansVO.meridiansArray[i].nOrderTimer = newMeridiansVO.meridiansArray[i].nOrderTimer;
//		
//				}
//			}
//		}
		
		public static function setTestData():void
		{
			meridiansVO = new MeridiansVO();									
			meridiansVO.roleID = GameCommonData.Player.Role.Id;					// 角色ID
			meridiansVO.nAmount = 8;											// 修炼的经脉数量
			meridiansVO.nAllLevGrade = 0;										//所有经脉修炼到达的等级（到达一定等级后会有4个效果参数，配置去写)
			meridiansVO.nAllStrengthLevAdd = 0;									//所有经脉强化到达的等级(到达一定等级会有1个加成效果参数，配置去写）
			
			for(var i:int = 0; i < 8; ++i)
			{
				var meridiansTypeVO:MeridiansTypeVO = new MeridiansTypeVO();
				meridiansTypeVO.nType = i + 1;									// 经脉类型
				meridiansTypeVO.nLev = 0;										// 经脉等级
				meridiansTypeVO.nLeaveTime = 0;								// 经脉当前等级剩余修炼时间
				meridiansTypeVO.nStrengthLev = 0;								// 经脉强化等级
				meridiansTypeVO.nState = 0;									// 经脉状态
				meridiansTypeVO.nOrderTimer = 0;   							//加入队列时间
				
				meridiansVO.meridiansArray.push(meridiansTypeVO);
			}
		}
		
		/** 获取当前经脉境界的最低等级
		 * @param n 所以经脉的最低等级
		 * @return 当前经脉境界的最低等级 */		
		public static function getLowest(n:int):int
		{
			if(n == 0)
			{
				return 0;
			}
			var a:int = (n - 1) / 30;
			var b:int = ((n - 1) % 30 +1) / 5;
			if(0 == b){
				return a * 30 + 1;
			}
			else
			{
				return a * 30 + b *5;
			}
		}
		
		//算下最低经脉等级
		public static function upDataNAllLevGrade():void
		{
			var nAllLevGrade:int = 100;
			for(var i:int = 0; i < MeridiansData.meridiansVO.meridiansArray.length ; ++i)
			{
				var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[i] as MeridiansTypeVO;
				if(meridiansTypeVO.nLev < nAllLevGrade)
				{
					nAllLevGrade = meridiansTypeVO.nLev;
				}
			}
			MeridiansData.meridiansVO.nAllLevGrade = nAllLevGrade;
		}
		
		//算下最低经脉强化等级
		public static function upDataNAllStrengthLevAdd():void
		{
			var nAllStrengthLevAdd:int = 100;
			for(var i:int = 0; i < MeridiansData.meridiansVO.meridiansArray.length ; ++i)
			{
				var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[i] as MeridiansTypeVO;
				if(meridiansTypeVO.nStrengthLev < nAllStrengthLevAdd)
				{
					nAllStrengthLevAdd = meridiansTypeVO.nStrengthLev;
				}
			}
			if(nAllStrengthLevAdd < 1)
			{
				MeridiansData.meridiansVO.nAllStrengthLevAdd = 0;
			}
			else if(nAllStrengthLevAdd < 4)
			{
				MeridiansData.meridiansVO.nAllStrengthLevAdd = 1;
			}
			else if(nAllStrengthLevAdd < 7)
			{
				MeridiansData.meridiansVO.nAllStrengthLevAdd = 2;
			}
			else if(nAllStrengthLevAdd < 9)
			{
				MeridiansData.meridiansVO.nAllStrengthLevAdd = 3;
			}
			else if(nAllStrengthLevAdd < 10)
			{
				MeridiansData.meridiansVO.nAllStrengthLevAdd = 4;
			}
			else
			{
				MeridiansData.meridiansVO.nAllStrengthLevAdd = 5;
			}
		}
		
		//更新剩余时间
		public static function upDataTime():void
		{
			var needStart:Boolean = false;
			for(var i:int = 0; i < meridiansVO.meridiansArray.length ; ++i)
			{
				var meridiansTypeVO:MeridiansTypeVO = meridiansVO.meridiansArray[i] as MeridiansTypeVO;
				if(meridiansTypeVO.nState == 2 && meridiansTypeVO.nLeaveTime > 0)
				{
					meridiansTypeVO.nLeaveTime -- ;
					needStart = true;
				}
			}
			if(!needStart)
			{
				MeridiansTimeOutComponent.getInstance().removeFun1("upDataTime");
			}
		}
	}
}