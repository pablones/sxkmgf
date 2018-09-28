package GameUI.Modules.Depot.Data
{
	/**
	 * 仓库事件
	 * @
	 * @
	 */ 
	public class DepotEvent
	{
		public function DepotEvent()
		{
		}
		
		
		public static const INITMONEY:String = "initMoney";				/** 初始化仓库的金钱总数 只用于初始化，收到这个命令表示前面物品列表已经接收完毕，可以开始显示了 */
		
		public static const UPDATEMONEY:String = "updateMoney";			/** 更新金钱，只用于平时的存钱、取钱操作 */
		
		public static const BAGTODEPOT:String = "bagToDepot";			/** 背包物品拖向仓库 */
		
		public static const DEPOTTOBAG:String = "depotToBag";			/** 仓库物品拖向背包 */
		
		public static const ADDITEM:String = "addItem";					/** 增加物品 */
		
		public static const DELITEM:String = "delItem";					/** 删除物品 */
		
		public static const SHOWITEMEXT:String = "showItemExt";			/** 显示物品扩充面板 */
		
		public static const SHOWPETEXT:String = "showPetExt";			/** 显示宠物扩充面板 */
		
		public static const REMOVEITEMEXT:String = "removeItemExt";		/** 移除物品扩充面板 */
		
		public static const REMOVEPETEXT:String = "removePetExt";		/** 移除宠物扩充面板 */
		
		public static const EXTITEMDEPOT:String = "extItemDepot";		/** 物品仓库扩容 */		
		
		public static const IN_OUT_PET_UPDATE_DEPOT:String = "in_out_pet_update_depot";		/** 存入/取出宠物，更新仓库画面 */		
					
//		public static const SHOWITEMS:String = "showItems";				/**  */
//		
//		public static const SHOWPETS:String = "showPets";				/** 显示宠物 */
//		
//		public static const ITEMOPERATE:String = "itemOperate";			/** 物品操作 */
		
		
	}
}