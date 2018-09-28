package GameUI.ConstData
{
	import flash.sampler.StackFrame;
	
	public class EventList
	{
		public function EventList()
		{
		}
		/** 打开答题面板  */
		public static const DISPLAYANSWER:String = "DISPLAYANSWER";
		/** MVC注册完成  */
		public static const REGISTERCOMPLETE:String = "registerComplete";
		/** 获取资源  */
		public static const GETRESOURCE:String = "getResource";
		/** 收到所有角色信息  */
		public static const ALLROLESREADY:String = "allRolesReady";
		/** 玩家进入地图  */
		public static const ENTERMAPCOMPLETE:String = "enterMapComplete";
		/** 初始化界面  */
		public static const INITVIEW:String = "initView";
		/** 显示登录界面  */
		public static const SHOWLOGIN:String = "SHOWLOGIN";
		/** 移除登录界面  */
		public static const REMOVELOGIN:String = "removeLogin";
		/** 显示选择角色  */
		public static const SHOWSELECTROLE:String = "showSelectRole";
		/** 移除选择角色  */
		public static const REMOVEELECTROLE:String = "removeSelectRole";
	    /** 显示创建角色  */
		public static const SHOWCREATEROLE:String = "showCreateRole";
		/** 移除创建角色  */
		public static const REMOVECREATEROLE:String = "removeCreateRole";
		
		/** 显示组队  */
		public static const SHOWTEAM:String = "showTeam";
		/** 显示好友  */
		public static const SHOWFRIENDS:String = "showFriends";
		/** 显示交易  */
		public static const SHOWTRADE:String = "showtrade";
		/** 更新交易信息 */
		public static const UPDATETRADE:String = "updateTrade";
		/** 交易解锁 */
		public static const TRADEUNLOCK:String = "tradeUnlock";
		/** 对方解锁 */
		public static const TRADEUNLOCK_OP:String = "tradeUnlock_op";
		/** 移除交易 */
		public static const REMOVETRADE:String = "removeTrade";
		/** 申请交易 对外接口 */
		public static const APPLYTRADE:String = "applyTrade";
		/** 收到服务器命令，请求数据 */
		public static const ASKTEAMINFO:String = "askTeamInfo";
		/** 收到服务器数据，更新组队UI */
		public static const UPDATETEAM:String = "updateTeam";
		/** 移除组队 */
		public static const REMOVETEAM:String = "removeTeam";
		/** 申请入队 对外接口 */
		public static const APPLYTEAM:String = "applyTeam";
		/** 邀请组队 对外接口 */
		public static const INVITETEAM:String = "inviteTeam";
		/** 自己是否有队伍 */
		public static const HASTEAM:String = "HasTeam";
		/** 移交队长 对外接口 */
		public static const CHANGELEADERTEAMCOMMON:String = "changeLeaderTeamCommon";
		/** 踢出队伍 对外接口 */
		public static const KICKOUTTEAMCOMMON:String = "kickoutTeamCommon";
		/** 离开队伍 对外接口 */
		public static const LEAVETEAMCOMMON:String = "leaveTeamCommon";
		/** 建立队伍 对外接口 */
		public static const SETUPTEAMCOMMON:String = "setupTeamCommon";
		/** 显示背包 */
		public static const SHOWBAG:String = "showBag";
		/** 关闭背包 */
		public static const CLOSEBAG:String = "CloseBag";
		/** 得到物品 */
		public static const GETITEMS:String = "getItems";
//		/** 物品解锁 */
		//public static const ITEMUNLOCK:String = "itemUnLock";
		/** 更新背包物品 */
		public static const UPDATEBAG:String = "UpdateBag";
		/** 显示警告面板 */
		public static const SHOWALERT:String = "showAlert";
		/** 主界面上组队按钮开始闪烁 */
		public static const TEAMBTNRAY:String = "TeamBtnRay";
		/** 物品解锁 */
		public static const BAGITEMUNLOCK:String = "BagItemUnlock";
		/** 交易完成物品更新 */
		public static const TRADECOMPLETE:String = "TradeComplete";
		/** 交易失败物品解锁 */
		public static const TRADEFAULT:String = "TradeFault";
		/** 人物，宠物，任务，技能只显示一个 */
		public static const SHOWONLY:String = "ShowOnly";
		/** 背包，好友，帮派，组队，排行榜 只显示一个 */
		public static const SHOWONLY_CENTER_FIVE_PANEL:String = "showOnly_center_five_panel";
		/** 显示人物属性面板  */
		public static const SHOWHEROPROP:String = "ShowHeroProp";
		/** 关闭人物属性面板  */
		public static const CLOSEHEROPROP:String = "CloseHeroProp";
		/** 打开背包中摆摊 */
		public static const SHOWSTALL:String = "ShowStall";
		/** 开始摆摊（收到服务器成功返回）*/
		public static const BEGINSTALL:String = "BeginStall";
		/** 物品从背包放入摆摊栏 */
		public static const BAGTOSTALL:String = "BagToStall";
		/** 摆摊物品消息 */
		public static const STALLITEM:String = "StallItem";
		/** 查询某摊位物品等信息 */
		public static const GETSTALLITEMS:String = "getStallItems";
		/** 显示表情设置界面  */
		public static const SHOWFACEVIEW:String = "ShowFaceView";		
		/** 隐藏表情选择  */
		public static const HIDESELECTFACE:String = "hideSelectFace";
		/** 显示宠物  */
		public static const SHOWPETVIEW:String = "ShowPetView";
		/** 关闭宠物  */
		public static const CLOSEPETVIEW:String = "ClosePetView";
		/** 显示商城  */
		public static const SHOWMARKETVIEW:String = "ShowMarketView";
		/** 关闭商城  */
		public static const CLOSEMARKETVIEW:String = "CloseMarketView";
		/** 显示技能  */
		public static const SHOWSKILLVIEW:String = "ShowSkillView";
		/** 关闭技能  */
		public static const CLOSESKILLVIEW:String = "CloseSkillView";
		/** 关闭技能学习面板  */
		public static const CLOSE_LEARNSKILL_VIEW:String = "CLOSE_LEARNSKILL_VIEW";
		/** 显示任务  */
		public static const SHOWTASKVIEW:String = "ShowTaskView";
		/** 关闭任务  */
		public static const CLOSETASKVIEW:String = "CloseTaskView";
		/** 显示帮派  */
		public static const SHOWUNITYVIEW:String = "ShowUnityView";
		/** 关闭帮派  */
		public static const CLOSEUNITYVIEW:String = "CloseUnityView";
		/** 显示申请帮派  */
		public static const SHOWAPPLYUNITYVIEW:String = "ShowApplyUnityView";
		/** 关闭申请帮派  */
		public static const CLOSEAPPLYUNITYVIEW:String = "CloseApplyUnityView";
		/** 显示创建帮派  */
		public static const SHOWCREATEUNITYVIEW:String = "ShowCreateUnityView";
		/** 关闭创建帮派  */
		public static const CLOSECREATEUNITYVIEW:String = "CloseCreateUnityView";
		/** 打开帮派信息  */
		public static const SHOWUNITYINFOVIEW:String = "SHOWUNITYINFOVIEW";
		/** 关闭帮派信息  */
		public static const CLOSEUNITYINFOVIEW:String = "CLOSEUNITYINFOVIEW";
		/** 帮派频道 */
		public static const HASUINTY:String = "HASUINTY";
		/** 打开仓库 (外部打开命令，点NPC) */
		public static const SHOWDEPOTVIEW:String = "ShowDepotView";
		/** 关闭仓库 (外部关闭命令，比如点NPC对话中的关闭、人物跑远而自动关闭等等) */
		public static const CLOSEDEPOTVIEW:String = "CloseDepotView";
		/** 打开NPC商店(外部打开命令，点NPC) */
		public static const SHOWNPCSHOPVIEW:String = "ShowNPCShopView";
		/** 关闭NPC商店(外部关闭命令) */
		public static const CLOSENPCSHOPVIEW:String = "CloseNPCShopView";
		/** 打开跑商商店 */
		public static const SHOW_NPC_BUSINESS_SHOP_VIEW:String = "show_npc_business_shop_view";
		/** 关闭跑商商店 */
		public static const CLOSE_NPC_BUSINESS_SHOP_VIEW:String = "close_npc_business_shop_view";
		/** 更新界面数据  */
		public static const UPDATEUIINFO:String = "updateUIinfo";
		/** 显示提示面板 */
		public static const SHOWTOOLTIP:String="showToolTip";
		/** 隐藏提示面板 */
		public static const HIDETOOLTIP:String="hideToolTip";
		/** 显示物品捡取  */
		public static const SHOWPICK:String = "ShowPick";
		/** 关闭物品捡取面板（只关面板，不对里面物品进行操作） */
		public static const CLOSE_PICK_PANEL:String = "close_pick_panel"; 
		/** 移除物品  */
		public static const DELETEPACKAGE:String = "DeletePackage";
		/** 更新属性  */
		public static const UPDATEATTRIBUATT:String = "UpdateAttribuatt";
		/** 显示Tooltip  */
		public static const SHOWITEMTOOLTIP:String = "ShowItemToolTip";
		/** 显示NPC商店物品ToolTip */
		public static const SHOW_NPC_SHOP_TOOLTIP:String = "show_npc_shop_tooltip";
		/** 移动Tooltip  */
		public static const MOVEITEMTOOLTIP:String = "MoveItemToolTip";  
		/** 移除Tooltip  */
		public static const REMOVEITEMTOOLTIP:String = "RemoveItemToolTip"; 
		/** 更新背包钱  */
		public static const UPDATEMONEY:String = "UpdateMoney"; 
		/** 获取了物品详细信息  */
		public static const GETINFOCOMPLETE:String = "GetInfoComplete";
		/** 打开排行榜面板  */
		public static const SHOWRANKVIEW:String = "ShowRankView"; 
		/** 关闭排行榜面板  */
		public static const CLOSERANKVIEW:String = "CloseRankView"; 
		/** 显示物品悬浮面板  */
		public static const SHOWITEMTOOLPANEL:String = "ShowItemToolPanel";
		/** 更新小地图显示 */
		public static const UPDATE_SMALLMAP_DATA:String="UPDATE_SMALLMAP_DATA";
		/** 显示大地图 */
		public static const SHOWBIGMAP:String="ShowBigMap";
		/** 显示小地图 */
		public static const SHOW_SMALL_MAP:String = "showSmallMap";
		/** 关闭大地图*/
		public static const CLOSEBIGMAP:String="CloseBigMap";
		/** 显示场景地图  */
		public static const SHOWSENCEMAP:String = "ShowSenceMap"; 
		/** 关闭场景地图  */
		public static const CLOSESCENEMAP:String="closeSceneMap"; 
		/** 场景地图切换 */
		public static const CHANGE_SCENEMAP_EVENT:String="CHANGE_SCENEMAP_EVENT";
		 
		//_________________________________________________________________
		//拖动事件
		/** 将物品放入快速栏 */
		public static const DROPINQUICK:String = "dropinQuick";
		/** 将技能图标放入快速栏 */
		public static const DROPSKILLINQUICK:String = "dropSkillInQuick";
		/** 将物品放入交易栏 */
		public static const GOTRADEVIEW:String = "GoTradeView";
		/** 将交易栏物品放入背包 */
		public static const GOBAGVIEW:String = "GoBagView";	
		/** 将背包中的物品放入人物属性栏 */
		public static const GOHEROVIEW:String = "GoHeroView";		
		/** 将物品放入交易栏失败 */
		public static const GOTRADEVIEWFAULT:String = "GoTradeViewFault";
				
		/** 键盘事件 */
		public static const KEYBORADEVENT:String = "KeyBoradEvent";
		/** 有ToolTip的组件移出*/
		public static const ITEMREMOVED:String="ItemRemoved";
		
		//-------------------------------------------------------------------
		/** 角色信息数据更新*/
		public static const ALLROLEINFO_UPDATE:String="allRolesInfoUpdate";
		/** 更新主界面中的经验值*/
		public static const UPDATE_MAINSECEN_EXP:String="update_mainSecen_Exp";
		
		/** 显示宠物系列面板 */
		public static const SHOW_PET_PLAYRULE_VIEW:String = "show_pet_playRule_view";
		/** 关闭宠物系列面板 */
		public static const CLOSE_PET_PLAYRULE_VIEW:String = "close_pet_playRule_view";
		/** 改变小地图路径 */
		public static const UPDATE_SMALLMAP_PATH:String="update_SmallMap_Path";
		/** 关闭问答面板 */
		public static const CLOSE_ANSWER_VIEW:String = "close_answer_view";
		/** 收到快捷栏 */
		public static const RECEIVE_QUICKBAR_MSG:String="receive_quickBar_Msg";
		/** 发送快捷栏信息*/
		public static const SEND_QUICKBAR_MSG:String="Send_QuickBar_Msg";
		/** 同步快捷栏与背包数据*/
		public static const ONSYNC_BAG_QUICKBAR:String="onSync_Bag_QuickBar";
		/** 帮派发送消息 */
		public static const UNITY_SEND_MSG:String="UNity_send_Msg";
		/** 收到Cd消息*/ 
		public static const RECEIVE_COOLDOWN_MSG:String="receive_coolDown_Msg";
		/** 显示自动寻路*/
		public static const SHOW_AUTOPATH_UI:String="Show_AutoPath_UI";
		/** 关闭自动寻路*/
		public static const HIDE_AUTOPATH_UI:String="HIde_AUTOPath_UI";
		 
		/** 关闭由NPC打开的界面窗口 */
		public static const CLOSE_NPC_ALL_PANEL:String = "close_npc_all_panel";
		
		/** 让主界面按钮闪 */
		public static const SHOW_MAINSENCE_BTN_FLASH:String = "show_mainSence_btn_flash";
		
		/** 新手10级以下是，每升一级发送事件 */
		public static const PLAYER_TASK_LEVEL:String="PLAYER_TASK_LEVEL";
		/** 打开帮助面板 */
		public static const SHOWHELPVIEW:String = "SHOWHELPVIEW";
		/** 关闭帮助面板*/
		public static const CLOSEHELPVIEW:String = "CLOSEHELPVIEW";
		/** 宝石合成界面打开 */
		public static const STONE_COMPOSE_UI:String="STONE_COMPOSE_UI";
		/** 宝石合成界面关闭 */
		public static const CLOSE_STONE_COMPOSE_UI:String="CLOSE_STONE_COMPOSE_UI";
		/** 引擎发送选中了某个NPC*/
		public static const SELECTED_NPC_ELEMENT:String="selected_npc_element";
		/** 第一次进入游戏提示 */
		public static const DO_FIRST_TIP:String="do_first_tip";
		/** 第一次进入游戏提示玩家回应 */
		public static const DONE_FIRST_TIP:String="done_first_tip";
		/** 做第一个问题的回答  */
		public static const DO_FIRST_QUESTION:String="do_first_Question";
		/** 显示buff */
		public static const SHOWBUFF:String = "SHOWBUFF";
		/** 显示debuff */
		public static const SHOWDEBUFF:String = "SHOWDEBUFF";
		/** 打开PK */
		public static const SHOWPKVIEW:String = "SHOWPKVIEW";
		/** 关闭PK */
		public static const CLOSEPKVIEW:String = "CLOSEPKVIEW";
		
		/** 宠物体息或死亡消息*/	
		public static const PET_RESTORDEAD_MSG:String="pet_restOrDead_msg";
		/** 使用特殊技能*/
		public static const USE_EXTENDSKILL_MSG:String="use_extendSkill_msg";
		/** 显示未完成任务*/
		public static const SHOW_UNFINISH_TASK:String="SHOW_UNFINISH_TASK";
		/** 心情隐藏显示状态改变*/
		public static const UPDATE_FEEL_STATUS:String="UPDATE_FEEL_STATUS";
		/** 事件提醒 */
		public static const TIMEUP:String="TIMEUP";
		/** 全部捡取背包物品 */
		public static const ALL_PICK_BAG:String="ALL_PICK_BAG";
		/** 切换职业，改变快捷栏 */
		public static const CHANGE_QUICKBAR_UI:String="change_quickBar_ui";
		/** 打开面板，播放打开声音 */
		public static const PLAY_SOUND_OPEN_PANEL:String = "play_sound_open_panel";
		
		/** 显示服务器时间 */
		public static const SHOW_TIME:String = "SHOW_TIME";
		
		/** 通知主界面更新队员状态 */
		public static const MEMBER_ONLINE_STATUS_TEAM:String = "member_online_status_team";
		
		/** 获取场景地图队友信息*/
		public static const SHOW_SCENE_TEAMINFO:String="show_scene_teamInfo";
		
		/** 发送系统消息 */
		public static const SENDSYSTEMMESSAGE:String = "SENDSYSTEMMESSAGE";
		/** 清除副本倒计时 */
		public static const CLEARCOPYTIME:String = "CLEARCOPYTIME";
		/**给快捷键加滤镜*/
		public static const ADDQUICKFLOW:String = "add_quick_flow";
		/**移除快捷键滤镜*/
		public static const REMOVEQUICKFLOW:String = "remove_quick_flow";

		/** 组合打开面板 */
		public static const OPEN_PANEL_TOGETHER:String = "open_panel_together";
		
		/** 组合打开面板 */
		public static const SHOW_TICK_ALERT:String = "SHOW_TICK_ALERT";
		
		public static const SHOW_MERIDIANS_HELP:String = "SHOW_MERIDIANS_HELP";
		
		/** 游戏界面自适应  */
		public static const STAGECHANGE:String = "stageChange";
		/** 游戏部分界面自适应  */
		public static const CHANGE_UI:String = "changeUI";
		/** 魂魄润魂石（通灵玉）合成成功  */
		public static const SOUL_STONE_COMPOSE_SUCCED:String = "soulStoneComposeSucced";
		
		
//		/** 角色管理提示框 */ 
//		public static const SHOWALERTROLE:String="SHOWALERTROLE";
		
		
		
		
	}
}
