package GameUI.Modules.PetPlayRule.PetToBaby.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	public class PetToBabyConstData
	{
		public function PetToBabyConstData()
		{
			
		}
		
		/** 技能格子数据 */
		public static var GridUnitList:Array = new Array();
		
		/** 技能数据 */
		public static var skillDataList:Array = new Array();
		
		/** 要还童的宠物快照 */
		public static var toBabyPet:GamePetRole = null;
		
		/** 还童道具格子 */
		public static var GridItemUnit:GridUnit;
		
		/** 还童道具数据 */
		public static var itemData:Object;
		
		/** 还童需要花费的钱数 */
		public static var breedCost:Number = 0;

	}
}