package GameUI.Modules.NPCShop.Data
{
	public class NPCShopEvent
	{
		public function NPCShopEvent()
		{
		}
		
		/** 物品背包到NPC商店出售栏 */
		public static const BAGTONPCSHOP:String = "bagToNPCShop";
				
		/** 更新出售钱数显示 */
		public static const UPDATENPCSALEMONEY:String = "updateNPCSaleMoney";
		
		/** NPC商店禁止拖动 */
		public static const NPC_SHOP_STOP_DRAG:String = "npc_shop_stop_drag";
	}
}