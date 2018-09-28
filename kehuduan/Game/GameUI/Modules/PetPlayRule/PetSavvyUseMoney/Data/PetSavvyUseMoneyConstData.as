package GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	public class PetSavvyUseMoneyConstData
	{
		public function PetSavvyUseMoneyConstData()
		{
		}
		
		/** 当前选中的宠物数据 */
		public static var petSelected:GamePetRole = null;
		
		/** 当前选定后显示的宠物 */
		public static var petShow:GamePetRole = null;
		
		/** 提升悟性需要花费的钱数 */
		public static var breedCost:Number = 0;
		
		/** 所需悟性丹type */
		public static var itemTypeNeed:String = "";
		
		/** 悟性丹道具格子 */
		public static var GridItemUnit:GridUnit;
		
		/** 悟性丹道具数据 */
		public static var itemData:Object;
		
		/** 宠物悟性提升 基础成功率 */
		public static var successList:Array = ["100%", "85%", "70%", "55%", "40%", "25%", "15%", "10%", "5%", "2%"];
		
		/** 宠物悟性提升 VIP加成成功率 */
		public static const VIP_SUCCESS_LEV:Array = [
														null,
														null,
														"1%",
														"2%"
														]
	}
}