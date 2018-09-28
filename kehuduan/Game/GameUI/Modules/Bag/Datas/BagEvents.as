package GameUI.Modules.Bag.Datas
{
	public class BagEvents
	{
		public function BagEvents()
		{
		}
		
		/** 发频道消息 获得了某物品 */
		public static const SHOW_MSG_GETED_ITEM:String = "show_msg_geted_item";
		
		/** 显示拆分界面  */
		public static const SHOWSPLIT:String = "showSplit";
		
		/** 移除拆分界面 */
		public static const REMOVE_SPLIT:String = "remove_slipt";
		
		/** 显示扩充界面  */
		public static const SHOWEXTENDS:String = "ShowExtends"; 
		
		/** 移除扩充界面 */
		public static const REMOVE_BAG_EXTENDS:String = "Remove_bag_extends";
		
		/** 删除背包物品  */
		public static const DROPITEM:String = "DropItem"; 
		
		/** 添加背包物品  */
		public static const ADDITEM:String = "AddItem";
		
		/** 可以操作物品，显示按钮  */
		public static const SHOWBTN:String = "ShowBtn"; 
		
		/** 可以操作物品，显示按钮  */
		public static const UPDATEITEMNUM:String = "UpdateItemNum"; 
		
		/** 背包扩展  */
		public static const EXTENDBAG:String = "ExtendBag";
		
		/** 背包不可拖动 */
		public static const BAG_STOP_DROG:String = "bag_stop_drog";
		
		/** 背包位置还原 */
		public static const BAG_INIT_POS:String = "bag_init_pos";
		
		/** 背包跳页（跳到指定页） */
		public static const BAG_GOTO_SOME_INDEX:String = "bag_goto_some_index";
		
	}
}