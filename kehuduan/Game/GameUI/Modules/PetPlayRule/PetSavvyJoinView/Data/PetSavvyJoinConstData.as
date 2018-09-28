package GameUI.Modules.PetPlayRule.PetSavvyJoinView.Data
{
	import OopsEngine.Role.GamePetRole;
	
	public class PetSavvyJoinConstData
	{
		public function PetSavvyJoinConstData()
		{
		}
		
		/** 选中的第一个宠物 */
		public static var petSelected_0:GamePetRole = null; 
		
		/** 选中的第二个宠物 */
		public static var petSelected_1:GamePetRole = null;
		
		/** 显示的第一个宠物 */
		public static var petShow_0:GamePetRole = null;
		
		/** 显示的第二个宠物 */
		public static var petShow_1:GamePetRole = null;
		
		/** 合成花费 */
		public static var breedCost:Number = 0;
		
		/** 宠物合成 基础成功率 */
		public static var successList:Array = ["100%", "85%", "70%", "55%", "40%", "25%", "15%", "10%", "5%", "2%"];
		
		/** 宠物合成花费金钱 */
		public static var breedCostList:Array = [Number(10000), Number(10000), Number(10000), Number(10000), Number(10000), Number(10000), Number(10000), Number(10000), Number(10000), Number(10000)];

//		/** 宠物合成花费金钱 */
//		public static var breedCostList:Array = [Number(85), Number(170), Number(340), Number(680), Number(1360), Number(2720), Number(5440), Number(10880), Number(21760), Number(43520)];

	}
}