package GameUI.Modules.Artifice.data
{
	import Net.ActionSend.EquipSend;
	
	public class ArtificeData
	{
		public static var StrengthenTransStr1:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_1" ]/* = "分解"*/;
		public static var StrengthenTransStr2:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_2" ] /*= "该物品不能分解，请放入装备"*/;
		public static var StrengthenTransStr3:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_3" ]/* = "紫色品质以下装备无法被分解"*/;
		public static var StrengthenTransStr4:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_4" ]/* = "50级以下装备无法被分解"*/;
		public static var StrengthenTransStr5:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_5" ]/* = "打了魂印宝石的装备无法被分解"*/;
		public static var StrengthenTransStr6:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_6" ]/* = ""*/;
		public static var StrengthenTransStr7:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_7" ]/* = ""*/;
		public static var StrengthenTransStr8:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_8" ]/* = ""*/;
		public static var StrengthenTransStr9:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_9" ]/* = ""*/;
		public static var StrengthenTransStr10:String = GameCommonData.wordDic[ "Modules_Artifice_data_ArtificeData_10" ]/* = "该装备已经提升过属性请确认是否分解？"*/;		
		
		
		public static var isInit:Boolean = false;
		public static var isShowView:Boolean = false;
		
		
		/** 第一个物品的位置 */
		public static var useItemSite:Array = [6,6];
		/** 格子位置 */
		public static var gridSite:Array = [107,97];
		/** 界面位置偏移 */
		public static var panelBaseSite:Array = [160,20];
		/** 分解需求  颜色品质、等级、不能是打了魂印宝石的装备 */
		public static var artificeNeed:Array = [4,50,2];
		/** 弹框提示条件 宝石个数、星数、强化等级、品质 */ 	/**(白、白、绿、蓝、紫、橙)*/
		public static var promptNeed:Array = [1,4,1,6];
		
		public static var resourcePath:String = "Resources/GameDLC/Artifice.swf";
		
		/** 是否是装备 */
		public static function isEquip(type:*):Boolean
		{
			var t:int = int(type/10000);
			if(11 <= t && t <= 22 && t != 20)
			{
				return true;
			}
			return false;
		}
		
		/** 类型是否相同 */
		public static function typeCompare(type1:*,type2:*):Boolean
		{
			return int(type1/10000) == int(type2/10000);
		}
		
		/** 炼化装备 */
		public static function artificeById(id:int):void
		{
			var param:Array=[0,88,id,0];
			EquipSend.createMsgCompound(param);
		}
		
	}
}