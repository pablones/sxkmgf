package GameUI.Modules.PetPlayRule.PetWinningUp.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	public class PetWinningConstData
	{
		public function PetWinningConstData()
		{
		}
		/** 当前选定后显示的宠物 */
		public static var petShow:GamePetRole = null;
		
		/** 提升灵性需要花费的钱数 */
		public static var breedCost:Number = 20000;
		
		/** 所需灵性丹type */
		public static var itemTypeNeed:String = "";
		
		/** 灵性丹道具格子 */
		public static var GridItemUnit:GridUnit;
		
		/** 灵性丹道具数据 */
		public static var itemData:Object;
		
		/** 宠物灵性提升 基础成功率 */
		public static var successList:Array = ["100%", "60%", "36%", "20%", "12%", "8%", "5%", "3%", "2%", "1%"];
		
		/** 宠物悟性提升 VIP加成成功率 */
		public static const VIP_SUCCESS_LEV:Array = [
														null,
														null,
														"1%",
														"2%"
														]
	}
}