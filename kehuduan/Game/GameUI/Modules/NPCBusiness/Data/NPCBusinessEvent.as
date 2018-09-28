package GameUI.Modules.NPCBusiness.Data
{
	public class NPCBusinessEvent
	{
		public function NPCBusinessEvent()
		{
		}
		
		/** 物品背包到跑商出售栏 */
		public static const BAG_TO_NPCBUSINESS:String = "bag_To_NPCBusiness";  
				
		/** 更新出售钱数显示 */
		public static const UPDATE_NPCBUSINESS_SALE_MONEY:String = "update_NPCBusiness_Sale_Money";
		
		/** 更新银票余额显示 */
		public static const UPDATE_MONEY_LAST_NPCBUSINESS:String = "update_money_last_npcBusiness";
		
	}
}