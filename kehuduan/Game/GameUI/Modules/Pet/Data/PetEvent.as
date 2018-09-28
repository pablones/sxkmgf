package GameUI.Modules.Pet.Data
{
	public class PetEvent
	{
		/** 查看宠物属性 通过ID去查询，等待返回再显示 */
		public static const LOOKPETINFO_BYID:String = "lookPetInfo_byId";
		
		/** 查看宠物属性，直接传宠物GamePetRole对象，直接显示 */
		public static const LOOKPETINFO_OBJ:String = "lookPetInfo_obj";
		
		/** 服务器返回详细宠物数据，显示在界面 */
		public static const RETURN_TO_SHOW_PET_INFO:String = "return_to_show_pet_info";
		
//		/** 改名成功 */
//		public static const PET_RENAME_SUCCESS:String = "pet_rename_success";

		/** 更新宠物信息，新增、改名 */
		public static const PET_UPDATE_SHOW_INFO:String = "pet_update_show_info";
		
		/** 改名失败 */
		public static const PET_RENAME_FAIL:String = "pet_rename_fail";
		
		/** 成功删除了宠物 */
		public static const PET_DELETE_SUCCESS:String = "pet_delete_success";
		
		/** 外部接口，查询某人的宠物 */
		public static const GET_PETINFO_OF_PLAYER:String = "get_petInfo_of_player";
		
		/** 外部接口，喂食 */
		public static const PET_FEED_OUTSIDE_INTERFACE:String = "pet_feed_outside_interface";
		
		/** 外部接口，驯养 */
		public static const PET_TRAIN_OUTSIDE_INTERFACE:String = "pet_train_outside_interface";
		
		/** 外部接口，休息 */
		public static const PET_REST_OUTSIDE_INTERFACE:String = "pet_rest_outside_interface";
		
		/** 外部接口，出战 */
		public static const PET_COMEOUT_FIGHT_OUTSIDE_INTERFACE:String = "pet_comeout_fight_outside";
		
		/** 外部接口，休息（不受时间限制） */
		public static const PET_AUTO_REST_OUTSIDE_INTERFACE:String = "pet_auto_rest_outside_interface";
		
		/** 外部接口，查看资料 */
		public static const PET_LOOK_OUTSIDE_INTERFACE:String = "pet_look_outside_interface";
		
		/** 收到其他玩家的宠物列表后，显示列表 */
		public static const SHOW_PET_LIST_OF_PLAYER:String = "show_pet_list_of_player";
		
		/** 收到PetAtt， 更新指定属性*/
		public static const  PET_ATT_UPDATE_UI:String = "pet_att_update_ui";
		
		/** 宠物面板禁用拖动 */
		public static const PETPROP_PANEL_STOP_DROG:String = "petprop_panel_stop_drop";
		
		/** 宠物面板位置还原 */
		public static const PETPROP_INIT_POS:String = "petprop_init_pos";
		
		/** 打开宠物选择面板，用于（上交宠物的任务=） */
		public static const OPEN_PET_CHOICE_PANEL_SINGLE:String = "open_pet_choice_panel_single";
		
		/** 关闭宠物选择面板，用于（上交宠物的任务=） */
		public static const CLOSE_PET_CHOICE_PANEL_SINGLE:String = "close_pet_choice_panel_single";
		
		
		/** 宠物出战（获得出战宠物的详细信息后） */
		public static const PET_TO_FIGHT_AFTER_GETINFO:String = "pet_to_fight_after_getInfo";
		/** 查看宠物详细信息（改版后） */
		public static const LOOK_FOR_PET_DETAIL_INFO:String = "look_for_pet_detail_info";
		/** 查看其他玩家宠物详细信息（改版后） */
		public static const LOOK_FOR_OTHER_PET_DETAIL_INFO:String = "look_for_other_pet_detail_info";
		/** 关闭宠物详细信息面板（改版后） */
		public static const CLOSE_PET_DETAIL_INFO:String = "close_pet_detail_info";
		/**显示宠物幻化*/
		public static const SHOW_PET_FANTASY_VIEW:String = "SHOW_PET_FANTASY_VIEW";
		/**显示宠物默契提升*/
		public static const SHOW_PET_PRIVITY_VIEW:String = "SHOW_PET_PRIVITY_VIEW";
		/**显示宠物变性*/
		public static const SHOW_PET_CHANGE_SEX_VIEW:String = "SHOW_PET_CHANGE_SEX_VIEW";
		
		/**宠物幻化反馈*/
		public static const PET_FANTASY_FEEDBACK:String = "PET_FANTASY_FEEDBACK";
		/**宠物默契提升反馈*/
		public static const PET_PRIVITY_FEEDBACK:String = "PET_PRIVITY_FEEDBACK";
		/**宠物附体反馈*/
		public static const PET_DEPENDECNE_FEEDBACK:String = "PET_DEPENDECNE_FEEDBACK";
		/**宠物分离反馈*/
		public static const PET_CANCEL_DEPENDECNE_FEEDBACK:String = "PET_CANCEL_DEPENDECNE_FEEDBACK";
		/**显示宠物变性反馈*/
		public static const PET_CHANGE_SEX_FEEDBACK:String = "PET_CHANGE_SEX_FEEDBACK";
		
		/**打开宠物玩耍界面*/
		public static const SHOW_PET_PLAY_VIEW:String = "SHOW_PET_PLAY_VIEW";	
		/**宠物玩耍，显示选择的宠物*/
		public static const SHOW_PLAY_PET_MODULE_FROM_SELECT:String = "SHOW_PLAY_PET_MODULE_FROM_SELECT";
		/**宠物玩耍，显示对方快照*/
		public static const SHOW_PLAY_PET_OTHER_PET_INFO:String = "SHOW_PLAY_PET_OTHER_PET_INFO";
		/**宠物玩耍，对方玩家锁定宠物*/
		public static const SHOW_PLAY_PET_OTHER_LOCK_PET:String = "SHOW_PLAY_PET_OTHER_LOCK_PET";
	}
}