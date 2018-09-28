package GameUI.Modules.StrengthenTransfer.data
{
	import Net.ActionSend.EquipSend;
	
	public class StrengthenTransferData
	{
		public static var StrengthenTransStr1:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_1" ]/** = "强化转移"*/;
		public static var StrengthenTransStr2:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_2" ]/** = "需要"*/;
		public static var StrengthenTransStr3:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_3" ]/** = "个"*/;
		public static var StrengthenTransStr4:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_4" ]/** = "当前拥有"*/;
		public static var StrengthenTransStr5:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_5" ]/** = "个"*/;
		public static var StrengthenTransStr6:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_6" ]/** = "该物品无法强化，请放入装备。"*/;
		public static var StrengthenTransStr7:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_7" ]/** = "该装备还没强化"*/;
		public static var StrengthenTransStr8:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_8" ]/** = "目标装备强化等级需低于原始装备强化等级"*/;
		public static var StrengthenTransStr9:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_9" ]/** = "同类型装备才能转移"*/;
		public static var StrengthenTransStr10:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_10" ]/** = "包裹中没有相应的转移符"*/;
		public static var StrengthenTransStr11:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_11" ]/** = "被转移目标已经强化，请确定是否转移？"*/;
		public static var StrengthenTransStr12:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_12" ]/**"目标装备已强化，转移后目标装备绑定，是否转移？"*/;
		public static var StrengthenTransStr13:String = GameCommonData.wordDic[ "Modules_StrengthenTransfer_data_StrengthenTransferData_13" ]/**"转移后目标装备绑定，是否转移？"*/;
		
		public static var isInit:Boolean = false;
		public static var isShowView:Boolean = false;
		
		/** 挪移护符相关信息 type x y */
		public static var helpItem:Array = [610060,317,0];
		/** 第一个物品与第二个物品的位置 */
		public static var useItemSite:Array = [6,6,6,6];
		/** 第一个格子与第二个格子的位置 */
		public static var gridSite:Array = [107,97,180,96];
		/** 界面位置偏移 */
		public static var panelBaseSite:Array = [160,20];
		/** 界面宽高调整 */
		public static var panelBaseWH:Array = [75,0];
		
		
		public static var resourcePath:String = "Resources/GameDLC/StrengthenTransfer.swf";
		
		
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
		public static function toStrengthenTransfer(id1:int,id2:int):void
		{
			var param:Array=[1, id2, 91, id1, 0];
			EquipSend.createMsgCompound(param);
		}
		
	}
}