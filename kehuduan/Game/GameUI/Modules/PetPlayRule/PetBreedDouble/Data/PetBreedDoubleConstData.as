package GameUI.Modules.PetPlayRule.PetBreedDouble.Data
{
	import OopsEngine.Role.GamePetRole;
	
	public class PetBreedDoubleConstData
	{
		public function PetBreedDoubleConstData()
		{
		}
		
		/** 当前选中的宠物数据 */
		public static var petSelected:GamePetRole = null;
		
		/** 自己当前显示的宠物数据 */
		public static var petShowSelf:GamePetRole = null;
		
		/** 对方当前显示的宠物数据 */
		public static var petShowOp:GamePetRole = null;
		
		/** 繁殖需要花费的钱数 */
		public static const breedCost:Number = 20000; 
		
		/** 对方是否锁定 */
		public static var isLockOp:Boolean = false;
		
		/** 己方是否锁定 */
		public static var isLockSelf:Boolean = false;
		
	}
}