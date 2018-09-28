package GameUI.Modules.PetPlayRule.PetSkillUp.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.View.items.UseItem;
	
	import OopsEngine.Role.GamePetRole;
	
	public class PetSkillUpConstData
	{
		public function PetSkillUpConstData()
		{
		}
		
		/** 技能格子数据 */
		public static var GridUnitList:Array = new Array();
		
		/** 技能数据 */
		public static var skillDataList:Array = new Array();
		
		/** 选择的格子 */
		public static var SelectedItem:GridUnit = null;
		
		/** XX丹格子 */
		public static var drugItem:UseItem = null;
		
		public static var TmpIndex:int = 0;
		
		/** 选中的宠物快照 */
		public static var petSelected:GamePetRole = null;
		
		/** 选中的技能格子 */
		public static var GridSkillUnit:GridUnit = null;
		
		/** 选中的技能数据 */
		public static var skillDataSelect:Object = null;
		
		/** 道具格子 */
		public static var GridItemUnit:GridUnit = null;
		
		/** 道具数据 */
		public static var itemData:Object = null;
		
		/** 需要花费的钱数 */
		public static var breedCost:Number = 10000; 
		
	}
}