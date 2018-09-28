package GameUI.Modules.Trade.Data
{
	public class TradeEvent
	{
		public function TradeEvent()
		{
		}
		
		/** 某人要与我交易 */
		public static const SOMEONETRADEME:String = "someOneTradeMe";
			
		/** 某人拒绝了与我交易 */
		public static const SOMEONEREFUSEME:String = "someOneRefuseMe";
		
		/** 显示交易相关的提示信息 */
		public static const SHOWTRADEINFORMATION:String = "showTradeInformation";
		
		/** 自己添加宠物的返回（s-c） */
		public static const PET_ADD_SELF_TRADE:String = "pet_add_self_trade";
		
		/** 自己删除宠物的返回（s-c） */
		public static const PET_DEL_SELF_TRADE:String = "pet_del_self_trade";
		
		/** 对方减少宠物（s-c） */
		public static const PET_DEL_OP_TRADE:String = "pet_del_op_trade";
		
		/** 对方加了宠物 */
		public static const PET_ADD_OP_TRADE:String = "pet_add_op_trade";
	}
}