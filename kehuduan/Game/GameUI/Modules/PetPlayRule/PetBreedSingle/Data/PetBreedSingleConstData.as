package GameUI.Modules.PetPlayRule.PetBreedSingle.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	public class PetBreedSingleConstData
	{
		public function PetBreedSingleConstData()
		{
		}
		
		/** 当前选中的宠物数据 */
		public static var petSelected:GamePetRole = null;
		
		/** 当前显示的雄性宠物数据 */
		public static var petMaleShow:GamePetRole = null;
		
		/** 当前显示的雌性宠物数据 */
		public static var petFemaleShow:GamePetRole = null;
		
		/** 繁殖道具格子 */
		public static var GridItemUnit:GridUnit;
		
		/** 繁殖道具数据 */
		public static var itemData:Object = null;
		
		/** 繁殖需要花费的钱数 */
		public static const breedCost:Number = 25000; 
	}
}