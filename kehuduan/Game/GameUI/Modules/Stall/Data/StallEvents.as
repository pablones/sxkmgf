package GameUI.Modules.Stall.Data
{
	public class StallEvents
	{
		public function StallEvents()
		{
		}
		
		/** 显示摆摊留言板界面  */
		public static const SHOWSTALLBBS:String = "showStallBBS"; 
		
		/** 收摊  */
		public static const REMOVESTALL:String = "removeStall"; 
		
		/** 强行收摊  */
		public static const REMOVESTALLNOW:String = "removeStallNow";
		
		/** 显示宠物列表界面  */
		public static const SHOWPETLIST:String = "showPetList"; 
		
		/** 移除宠物列表界面  */
		public static const REMOVEPETLIST:String = "removePetList"; 
		
		/** 移除摆摊留言板界面 */
		public static const REMOVESTALLBBS:String = "removeStallBBS";
		
		/** 商品拖回背包 */
		public static const STALLTOBAG:String = "stallToBag";
		
		/** 更新摆摊界面 */
		public static const DELSTALLITEM:String = "delStallItem";
		
		/** 收到某摊位物品等信息 */
		public static const SHOWSOMESTALL:String = "showSomeStall";
		
		/** 刷新显示钱数 */
		public static const REFRESHMONEY:String = "refreshMoney";
		
		/** 刷新摆摊中显示的自己的携带金钱 */
		public static const REFRESHMONEYSEFLSTALL:String = "refreshMoneySelfStall";
		
		/** 更新摊位名 */
		public static const UPDATESTALLNAME:String = "updateStallName";
		
		/** 更新摊主名 */
		public static const UPDATESTALLOWNERNAME:String = "updateStallOwnerName";
		
		/** 更新摊位信息 */
		public static const UPDATESTALLINFO:String = "updateStallInfo";
		
		/** 更新摊位留言 */
		public static const UPDATESTALLMSG:String = "updateStallMsg";
		
		/** 更新出售中的宠物栏 */
		public static const UPDATE_SALE_PET_STALL:String = "update_sale_pet_stall";
		
		/** 更新宠物选择列表 */
		public static const UPDATE_PET_LIST_STALL:String = "update_pet_list_stall";
		
	}
}