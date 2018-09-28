package GameUI.Modules.PetPlayRule.PetSkillLearn.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.View.items.UseItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	
	public class PetSkillLearnConstData
	{
		public function PetSkillLearnConstData()
		{
		}
		/**新宠物（选择的要要覆盖的技能格子）*/
		public static var NewSelectGrid:GridUnit; 
		/** 技能格子数据 */
		public static var GridUnitList:Array = new Array();
		
		/** 技能数据 */
		public static var skillDataList:Array = new Array();
		
		/** 遗忘的技能ID */
		public static var skillForgeted:uint = 0;
		 
		/** 选择的格子 */
		public static var SelectedItem:GridUnit = null;
		
		/** 选择的技能书格子 */
		public static var itemShow:UseItem = null;
		
		public static var TmpIndex:int = 0;
		
		
		/** 选中的宠物快照 */
		public static var petSelected:GamePetRole = null;
		
		/** 道具格子 */
		public static var GridItemUnit:MovieClip;
		
		
		
		/** 道具数据 */
		public static var itemData:Object = null;
		
		/** 需要花费的钱数 */
		public static const breedCost:Number = 10000; 
		
		/** 宠物技能数据 */
		public static var SKILL_DATA_PET:Array = [];
		
		/** 获取技能数据 */
		public static function getSkillData(skillId:int):Object
		{
			var res:Object = null;
			var len:int = SKILL_DATA_PET.length;
			for(var i:int = 0; i < len; i++) {
				if(SKILL_DATA_PET[i].id == skillId) {
					res = SKILL_DATA_PET[i];
					break;
				}
			}
			return res;	
		}
		
		/** 获取技能数据 */
		public static function getSkillDataByType(type:uint):Object
		{
			var res:Object = null;
			var len:int = SKILL_DATA_PET.length;
			for(var i:int = 0; i < len; i++) {
				if(SKILL_DATA_PET[i].itemType == type) {
					res = SKILL_DATA_PET[i];
					break;
				}
			}
			return res;	
		}
		
	}
}