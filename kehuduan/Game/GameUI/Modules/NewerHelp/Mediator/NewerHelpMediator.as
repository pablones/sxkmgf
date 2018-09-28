package GameUI.Modules.NewerHelp.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Proxy.DataProxy;
	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 * 新手指导UI管理器
	 * @author:Ginoo
	 * @date:8/31/2010
	 */
	public class NewerHelpMediator extends Mediator
	{
		private static const NAME:String = "NewerHelpMediator";
		private var _dataProxy:DataProxy = null;
		private var _uiManager:NewerHelpUIMediaror = null;
		
		public function NewerHelpMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				NewerHelpEvent.SHOW_NEWER_HELP,							//开始新任务
				NewerHelpEvent.OPEN_BAG_NOTICE_NEWER_HELP,				//打开背包
				NewerHelpEvent.CLOSE_BAG_NOTICE_NEWER_HELP,				//关闭背包
				NewerHelpEvent.CLOSE_DEPOT_NOTICE_NEWER_HELP,			//关闭仓库
				NewerHelpEvent.OPEN_PETPROP_NOTICE_NEWER_HELP,			//打开宠物面板	
				NewerHelpEvent.CLOSE_PETPROP_NOTICE_NEWER_HELP,			//关闭宠物面板	
				NewerHelpEvent.OPEN_HEROPROP_PANEL_NOTICE_NEWER_HELP,	//打开人物属性面板
				NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP,	//关闭人物属性面板
				NewerHelpEvent.PET_FIGHT_NOTICE_NEWER_HELP,				//宠物出战		
				TaskCommandList.HIDE_TASKFOLLOW_UI,						//关闭任务追踪面板
				NewerHelpEvent.USE_QUICKBAR_NOTICE_NEWER_HELP,			//将物品拖到快捷键
				NewerHelpEvent.DRESSON_NOTICE_NEWER_HELP,				//穿上装备，通知指导，进行下一步
				NewerHelpEvent.SELECT_NPC_NOTICE_NEWER_HELP,			//点击选择画面中NPC
				NewerHelpEvent.TASK_UPDATE_NOCICE_NEWER_HELP,			//任务变为完成状态时		开启提示
				NewerHelpEvent.TASK_FIND_PATH_NOTICE_NEWER_HELP,		//点击自动寻路			更新指导
				NewerHelpEvent.TASK_COMPLETE_NOTICE_NEWER_HELP,			//点“完成”按钮完成任务	更新指导
				NewerHelpEvent.LOOP_DELETE_GRID_NOTICE_NEWER_HELP,		//循环删除格子
				NewerHelpEvent.NPC_SHOP_INGOOD_NOTICE_NEWER_HELP,		//NPC商店拖物品进去
				NewerHelpEvent.NPC_SHOP_CLICK_SALE_NOTICE_NEWER_HELP,	//NPC商店点出售按钮
				NewerHelpEvent.NPC_SHOP_CLOSE_NOTICE_NEWER_HELP,		//关闭NPC商店
				NewerHelpEvent.CLOSE_NPCCHAT_NOTICE_NEWER_HELP,			//关闭NPC对话
				NewerHelpEvent.OPEN_TASK_ACCEPT_NOTICE_NEWER_HELP,		//打开任务接受面板
				NewerHelpEvent.CLOSE_TASK_SHOW_NOTICE_NEWER_HELP,		//关闭任务说明面板
				NewerHelpEvent.ROLE_LEVUP_NOTICE_NEWER_HELP,			//人物升级
				//-----
				NewerHelpEvent.OPEN_MARKET_NOTICE_NEWER_HELP,			//打开商城
				NewerHelpEvent.CLOSE_MARKET_NOTICE_NEWER_HELP,			//关闭商城
				NewerHelpEvent.CLICK_DQ_MARKET_NOTICE_NEWER_HELP,		//点击点券页签
				NewerHelpEvent.CLICK_WDQK_MARKET_NOTICE_NEWER_HELP,		//点击武道乾坤道具
				NewerHelpEvent.CLICK_BUY_MARKET_NOTICE_NEWER_HELP,		//点击购买按钮
				//-----
				NewerHelpEvent.OPEN_NEWER_AWARD_NEWER_HELP,				//打开新手成就
				NewerHelpEvent.CLOSE_NEWER_AWARD_NEWER_HELP,			//关闭新手成就
				NewerHelpEvent.CLICK_GET_NEWER_AWARD_NEWER_HELP,		//点击领取按钮 新手成就
				NewerHelpEvent.CLICK_ITEM_NEWER_AWARD_NEWER_HELP,		//点击某一条新手成就
				//-----
				NewerHelpEvent.OPEN_AUTO_PLAY_NEWER_HELP,				//打开挂机界面
				NewerHelpEvent.CLOSE_AUTO_PLAY_NEWER_HELP,				//关闭挂机界面
				NewerHelpEvent.CLICK_AUTO_SET_NEWER_HELP,				//点击自动设置按钮
				NewerHelpEvent.CLICK_SAVE_SET_NEWER_HELP,				//点击保存设置按钮
				NewerHelpEvent.CLICK_START_AUTO_PLAY_NEWER_HELP,		//点击开始挂机按钮
				//-----
				NewerHelpEvent.OPEN_SENCE_MAP_NOTICE_NEWER_HELP,		//打开场景地图
				NewerHelpEvent.CLOSE_SENCE_MAP_NOTICE_NEWER_HELP,		//关闭场景地图
				//-----
				NewerHelpEvent.CLICK_SKILLCELL_NOTICE_NEW_HELP,			//点击技能学习组件
				NewerHelpEvent.CLICK_SKILLLEARN_BUTTON_NOTICE_NEW_HELP,	//点击技能学习按钮
				NewerHelpEvent.CLOSE_SKILLLEARN_NOTICE_NEW_HELP,		//关闭技能学习面板
				NewerHelpEvent.SKILL_LEVUP_NEWER_HELP,					//技能等级提升
				//-----
				EventList.ENTERMAPCOMPLETE,								//登陆游戏 显示经验泡泡
				NewerHelpEvent.UPDATE_EXP_NOTICE_NEWER_HELP,			//更新人物经验 显示经验泡泡
				//-----
				EventList.CHANGE_SCENEMAP_EVENT,						//转场成功，删除白云飞泡泡
				//-----
				NewerHelpEvent.USE_ITEM_NEWER_HELP,						//使用某物品
				//-----------
				NewerHelpEvent.SHOW_I_KNOW_NEWER_HELP,					//显示单个我知道了
				//----------
				NewerHelpEvent.CLICK_FLY_SHOE,							//弹出使用小飞鞋提示
				//---------
				NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE,				//根据任务号	关闭任务
				//---------
				NewerHelpEvent.RECALL_TASK_HELP                              //回调任务
			];
		}
		private function test(type:uint):void
		{
			NewerHelpData.curType = type;
			NewerHelpData.curStep = 1;
			doTask(); 
		} 
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:									//初始化
//					NewerHelpData.dealNewerMask(0);	//处理新手掩码
					_dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					_uiManager = new NewerHelpUIMediaror();
					facade.registerMediator(_uiManager);
//					setTimeout(test, 5000, 46);    
//					setTimeout(test, 8000, 24);
//					setTimeout(test, 4000, 26); 
					break;
				case NewerHelpEvent.SHOW_NEWER_HELP:						//显示新手帮助指示  任务链开始
					var helpType:uint = uint(notification.getBody());
					if(!NewerHelpData.newerHelpIsOpen && helpType != 23 && helpType != 24 && helpType != 25) return;	//不在新手阶段则不显示
					if(helpType > 0) {
						if(helpType == 33 || helpType == 39) {
							doIknow(helpType);
						} else if(helpType == 42 || helpType == 43) {
							checkUseNewerAward(helpType);
						} else {
							NewerHelpData.curType = helpType;
							NewerHelpData.curStep = 1;
							doTask();
						}
					}
					break;
				case NewerHelpEvent.OPEN_BAG_NOTICE_NEWER_HELP:				//打开背包
					showBag();
					break;
				case NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE:				//根据任务号清除任务
					var closeType:uint = notification.getBody() as uint;
					closeHelp(closeType);
					break;
				case NewerHelpEvent.CLOSE_BAG_NOTICE_NEWER_HELP:			//关闭背包
					closeBag();
					break;
				case NewerHelpEvent.OPEN_PETPROP_NOTICE_NEWER_HELP:			//打开宠物面板
					openPet();
					break;
				case NewerHelpEvent.CLOSE_PETPROP_NOTICE_NEWER_HELP:		//关闭宠物面板 
					closePet();
					break;
				case NewerHelpEvent.OPEN_HEROPROP_PANEL_NOTICE_NEWER_HELP:	//打开人物属性面板
					openHeroProp();
					break;
				case NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP:	//关闭人物属性面板
					closeHeroProp();
					break;
				case NewerHelpEvent.NPC_SHOP_CLOSE_NOTICE_NEWER_HELP:		//关闭NPC商店
					closeNPCShop();
					break;
				case NewerHelpEvent.LOOP_DELETE_GRID_NOTICE_NEWER_HELP:		//循环删除格子
					delGrid(notification.getBody());
					break;
				case NewerHelpEvent.USE_QUICKBAR_NOTICE_NEWER_HELP:			//使用快捷键
					useQuickKey(notification.getBody());
					break;
				case NewerHelpEvent.PET_FIGHT_NOTICE_NEWER_HELP:			//宠物出战
					fightPet();
					break
				case NewerHelpEvent.NPC_SHOP_INGOOD_NOTICE_NEWER_HELP:		//NPC商店拖物品进去
					npcShopInGood();
					break;
				case NewerHelpEvent.NPC_SHOP_CLICK_SALE_NOTICE_NEWER_HELP:	//NPC商店点击出售按钮
					npcShopClickSale();
					break;
				case NewerHelpEvent.SELECT_NPC_NOTICE_NEWER_HELP:			//点击选中NPC
					clickNPC(notification.getBody());
					break
				case NewerHelpEvent.TASK_UPDATE_NOCICE_NEWER_HELP:			//任务变成完成状态时，触发提示
					var taskUpd:Object = notification.getBody()
					taskUpdate(taskUpd);
					break;
				case NewerHelpEvent.TASK_FIND_PATH_NOTICE_NEWER_HELP:		//点击自动寻路，更新指导
					var taskPath:Object = notification.getBody();
					taskPathNew(taskPath);
					break;
				case NewerHelpEvent.TASK_COMPLETE_NOTICE_NEWER_HELP:		//点“完成”按钮完成任务，更新指导
					var taskComp:Object = notification.getBody();
					taskCompNew(taskComp);
					break;
				case TaskCommandList.HIDE_TASKFOLLOW_UI:					//关闭任务追踪面板，更新指导
					closeTaskFollow();
					break;
				case NewerHelpEvent.ROLE_LEVUP_NOTICE_NEWER_HELP:			//人物升级
					var levType:uint = notification.getBody() as uint;
					levUp(levType);
					break;
				case NewerHelpEvent.DRESSON_NOTICE_NEWER_HELP:				//穿上装备，通知指导系统，进行下一步
					dressOn(notification.getBody() as uint);
					break;
				case NewerHelpEvent.CLOSE_DEPOT_NOTICE_NEWER_HELP:			//关闭仓库
					closeDepot();
					break;
				case NewerHelpEvent.CLOSE_NPCCHAT_NOTICE_NEWER_HELP:		//关闭NPC对话
					closeNPCChat();
					break;
				case NewerHelpEvent.OPEN_TASK_ACCEPT_NOTICE_NEWER_HELP:		//打开任务接受面板
					openTaskAccept();
					break;
				case NewerHelpEvent.CLOSE_TASK_SHOW_NOTICE_NEWER_HELP:		//关闭任务说明面板
					closeTaskInfoShow();
					break;
				case NewerHelpEvent.OPEN_MARKET_NOTICE_NEWER_HELP:			//打开商城
					openMarket();
					break;
				case NewerHelpEvent.CLOSE_MARKET_NOTICE_NEWER_HELP:			//关闭商城
					closeMarket();
					break;
				case NewerHelpEvent.CLICK_DQ_MARKET_NOTICE_NEWER_HELP:		//点击点券页签
					clickDQBtnMarket();
					break;
				case NewerHelpEvent.CLICK_WDQK_MARKET_NOTICE_NEWER_HELP:	//点击武道乾坤道具
					clickWDQKMarket();
					break;
				case NewerHelpEvent.CLICK_BUY_MARKET_NOTICE_NEWER_HELP:		//点击购买按钮
					clickBuyBtnMarket();
					break;
				case NewerHelpEvent.OPEN_NEWER_AWARD_NEWER_HELP:			//打开新手成就
					openNewerAward();
					break;
				case NewerHelpEvent.CLOSE_NEWER_AWARD_NEWER_HELP:			//关闭新手成就
					closeNewerAward();
					break;
				case NewerHelpEvent.CLICK_GET_NEWER_AWARD_NEWER_HELP:		//点击领取按钮 新手成就
					clickGetNewerAward();
					break;
				case NewerHelpEvent.CLICK_ITEM_NEWER_AWARD_NEWER_HELP:		//点击某一条新手成就
					clickItemNewerAward(notification.getBody() as uint);
					break;
				case NewerHelpEvent.OPEN_AUTO_PLAY_NEWER_HELP:				//打开挂机界面
					openAutoPlay()
					break;
				case NewerHelpEvent.CLOSE_AUTO_PLAY_NEWER_HELP:				//关闭挂机界面
					closeAutoPlay();
					break;
				case NewerHelpEvent.CLICK_AUTO_SET_NEWER_HELP:				//点击自动设置按钮
					clickAutoSet();
					break;
				case NewerHelpEvent.CLICK_SAVE_SET_NEWER_HELP:				//点击保存设置按钮
					clickSaveSet();
					break;
				case NewerHelpEvent.CLICK_START_AUTO_PLAY_NEWER_HELP:		//点击开始挂机按钮
					clickStartAutoPlay();
					break;
				case NewerHelpEvent.OPEN_SENCE_MAP_NOTICE_NEWER_HELP:		//打开场景地图
					openSenceMap();
					break;
				case NewerHelpEvent.CLOSE_SENCE_MAP_NOTICE_NEWER_HELP:		//关闭场景地图
					closeSenceMap();
					break;
				case NewerHelpEvent.CLICK_SKILLCELL_NOTICE_NEW_HELP:		//点击技能学习组件
					clickSkillLearnCell(notification.getBody());
					break;
				case NewerHelpEvent.CLICK_SKILLLEARN_BUTTON_NOTICE_NEW_HELP://点击技能学习按钮
					clickSkillLearnButton();
					break;
				case NewerHelpEvent.CLOSE_SKILLLEARN_NOTICE_NEW_HELP:		//关闭技能学习面板
					closeSkillLearnPanel();
					break;
				case NewerHelpEvent.SKILL_LEVUP_NEWER_HELP:					//技能等级提升
					skillLevUp(notification.getBody());
					break;
				case NewerHelpEvent.UPDATE_EXP_NOTICE_NEWER_HELP:			//更新人物经验
					updateRoleExp();
					break;
				case EventList.ENTERMAPCOMPLETE:							//登陆游戏 显示经验泡泡
					if(GameCommonData.Player.Role.Level >= 12) {
						initRoleExp();
					}
					break;
				case EventList.CHANGE_SCENEMAP_EVENT:						//转场成功，删除白云飞泡泡
					changeMapComp();
					break;
				case NewerHelpEvent.USE_ITEM_NEWER_HELP:					//使用某物品
					useSomeItem(notification.getBody() as uint);
					break;
				case NewerHelpEvent.CLICK_FLY_SHOE:							//弹出使用小飞鞋提示
					useFlyShoe(uint(notification.getBody()));
					break;
				case NewerHelpEvent.SHOW_I_KNOW_NEWER_HELP:					//显示单个我知道了	
					doIknow(notification.getBody() as uint);
					break;
				case NewerHelpEvent.RECALL_TASK_HELP:
			        recallTask(notification.getBody() as uint);
			        break;
			}
		}
		
		private function recallTask(type:uint):void
		{
			switch(NewerHelpData.curType) {
				case 8:
				    if(NewerHelpData.curStep == 3)
				    {
				    	_uiManager.doTask_8();
				    }
					break;
				case 12:
					_uiManager.doTask_12(); 
					break;
				case 13:
				    if( NewerHelpData.curStep == 2 )
				    {
						_uiManager.doTask_13();	
				    }
					break;
			}
		}
		
		/** 显示我知道了 */
		private function doIknow(type:uint):void
		{
			switch(type) {
				case 33:
					_uiManager.doTask_33();
					break;
				case 36:
					if(AutoPlayData.offLineTime <= 12) {	//第一个成就已过期 则 不提示
						_uiManager.doTask_36();
					}
					break;
				case 39:
					_uiManager.doTask_39();
					break;
					
			}
		}
		
		/** 开始任务 */
		private function doTask():void
		{
			switch(NewerHelpData.curType) {
				case 1:
					if(_dataProxy.BagIsOpen) {
						sendNotification(BagEvents.BAG_INIT_POS);
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_1();
					break;
				case 2:
					if(_dataProxy.TaskFollowIsOpen)
						_uiManager.doTask_2();
					break;
				case 3:
					if(_dataProxy.BagIsOpen) {
						sendNotification(BagEvents.BAG_INIT_POS);
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_3();
					break;
				case 4:
					if(_dataProxy.PetIsOpen) {
						NewerHelpData.curStep++;
					}
					sendNotification(PetEvent.PETPROP_INIT_POS);
					_uiManager.doTask_4();
					break;
				case 5:
					if(_dataProxy.BagIsOpen) {
						sendNotification(BagEvents.BAG_INIT_POS);
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_5();
					break;
				case 6:
					if(_dataProxy.PetIsOpen) {
						NewerHelpData.curStep++;
					}
					sendNotification(PetEvent.PETPROP_INIT_POS);
					_uiManager.doTask_6();
					break;
				case 7:
					if(_dataProxy.BagIsOpen) {
						sendNotification(BagEvents.BAG_INIT_POS);
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_7();
					break;
				case 8:
					_uiManager.doTask_8();
					break;
				case 9:
					if(_dataProxy.HeroPropIsOpen) {
						NewerHelpData.curStep++;
					}
					sendNotification(RoleEvents.HEROPROP_PANEL_INIT_POS);
					_uiManager.doTask_9();
					break;
//				case 10:
//					if(_dataProxy.HeroPropIsOpen) {
//						NewerHelpData.curStep++;
//					}
//					sendNotification(RoleEvents.HEROPROP_PANEL_INIT_POS); 
//					_uiManager.doTask_10();
//					break;
				case 11:
					if(_dataProxy.BagIsOpen) {
						sendNotification(BagEvents.BAG_INIT_POS);
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_11();
					break;
				case 12:
					sendNotification(BagEvents.BAG_INIT_POS);
					_uiManager.doTask_12();
					break;
				case 13:
//					if(_dataProxy.BagIsOpen) {
					sendNotification(BagEvents.BAG_INIT_POS);
//						NewerHelpData.curStep++;
//					}
					_uiManager.doTask_13();
					break;
				case 14:	//皇陵  获得湛卢剑
					NewerHelpData.curType = 8;
					NewerHelpData.curStep = 3;
					if(_dataProxy.BagIsOpen) {
						NewerHelpData.curStep++;
						sendNotification(BagEvents.BAG_INIT_POS);
					}
					_uiManager.doTask_8();
					break;
				case 15:
					_uiManager.doTask_15();
					break;
				case 16:
					_uiManager.doTask_16();
					break;
				case 17:
					_uiManager.doTask_17();
					break;
				case 18:
					_uiManager.doTask_18();
					break;
				case 19:
					_uiManager.doTask_19();
					break;
				case 20:	//去找屠大勇
					_uiManager.doTask_20();
					break;
				case 21:		//进宠物林
					_uiManager.doTask_21();
					break;
				case 22:	//商城购物
					if(_dataProxy.MarketIsOpen) {
						NewerHelpData.curStep++;
						if(MarketConstData.curPageIndex == 7) NewerHelpData.curStep++;
						if(NewerHelpData.havaMarketItemByType(610046)) NewerHelpData.curStep++;
					}
					_uiManager.doTask_22();
					break;
				case 23:
					_uiManager.doTask_23();
					break;
				case 24:
					_uiManager.doTask_24();
					break;
				case 25:
					_uiManager.doTask_25();
					break;
				case 26:
					_uiManager.doTask_26();
					break;
				case 27:	//指导 绒毛兔  点击这里自动寻路
					_uiManager.doTask_27();
					break;
				case 28:	//完成大侠之路加入门派
					_uiManager.doTask_28();
					break;
				case 29:	//第一次加入门派
					_uiManager.doTask_29();
					break;
				case 30:	//拜入师门
					_uiManager.doTask_30();
					break;
				case 31:	//第一次打开技能面板学技能
					_uiManager.doTask_31();
					break;
				case 32:	//去找白云飞
					_uiManager.doTask_32();
					break;
				case 33:
					_uiManager.doTask_33();
					break;
					break;
				case 34:
					if(_dataProxy.PetIsOpen) {
						NewerHelpData.curStep++;
					}
					sendNotification(PetEvent.PETPROP_INIT_POS);
					_uiManager.doTask_34();
					break;
					break;
				case 35:
					if(_dataProxy.PetIsOpen) {
						NewerHelpData.curStep++;
					}
					sendNotification(PetEvent.PETPROP_INIT_POS);
					_uiManager.doTask_35();
					break;
				case 36:
					_uiManager.doTask_36();
					break;
				case 37:
					if(NewAwardData.newPlayAwardIsOpen) {
						NewerHelpData.curStep++;
					}
					sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_4" ] , content:"    "+DialogConstData.getInstance().getTipDesByType(9)});//"系统消息"
					_uiManager.doTask_37();
					break;
				case 38:
					var jobStr:String = '<font color="#00ff00">'+_dataProxy.RolesListDic[GameCommonData.Player.Role.CurrentJobID]+GameCommonData.wordDic[ "mod_new_med_new_doT_1" ]+'</font>';//掌门认为你天资聪慧，进步神速，颇具大侠之慧根。现召你回去学习更高级的技能，助你江湖之路。
					sendNotification(EventList.DO_FIRST_TIP, {comfrim:null, info:jobStr, title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ], width:300});//"小提示"	"我知道了"
					sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_4" ] , content:"    "+jobStr});//"系统消息"
					break;
				case 39:
					_uiManager.doTask_39();
					break;
				case 40:
					if(GameCommonData.Player.IsAutomatism) {
						NewerHelpData.curStep = 5;
					} else if(_dataProxy.autoPlayIsOpen){
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_40();
					break;
				case 41:
					if(_dataProxy.BagIsOpen) {
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_41();
					break;
				case 44:
					if(_dataProxy.BagIsOpen) {
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_44();
					break;
				case 45:
					sendNotification(SkillConst.STOP_MOVE_SKILLLEARN_PANEL);	//锁定技能学习面板位置
					NewerHelpData.curStep = judgeCurStep();
					_uiManager.doTask_45();
					break;
				case 46:
					_uiManager.doTask_46();
					break;
			}
		}
		
		/**
		 * 弹出使用小飞鞋提示
		 * @param type 0-点击弹出  1-关闭提示面板
		 */ 
		private function useFlyShoe(type:uint):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(type == 0) {
				if(NewerHelpData.curType == 46 && NewerHelpData.curStep == 1) {
					NewerHelpData.curStep++;
					_uiManager.doTask_46();
					return;
				}
			} else if(type == 1) {
				if(NewerHelpData.curType == 46 && NewerHelpData.curStep == 2) {
					NewerHelpData.curStep++;
					_uiManager.doTask_46();
					return;
				}
			}
		}
		
		private function useSomeItem(type:uint):void
		{
			if(type == 500100 && NewerHelpData.curType == 41 && NewerHelpData.curStep == 2) {	//第一个新手大礼包		
				NewerHelpData.curStep++;
				_uiManager.doTask_41();
				return;
			}
			if(type == 501026 && NewerHelpData.curType == 44 && NewerHelpData.curStep == 3) {	//第二个新手大礼包	使用酒	
				NewerHelpData.curStep++;
				_uiManager.doTask_44();
				return;
			}
		}
		
		/** 使用大礼包 */
		private function checkUseNewerAward(helpType:uint):void
		{
			switch(helpType) {
				case 42:	//使用了第一个新手大礼包 获得装备
					if(NewerHelpData.curType == 41 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep = 4;
						_uiManager.doTask_41();
					} else if(NewerHelpData.curType == 44 && NewerHelpData.curStep == 2) {
						_uiManager.doTask_44();
					}
					break;
				case 43:	//使用了第二个新手大礼包 获得装备
					if(NewerHelpData.curType == 44 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep++;
						_uiManager.doTask_44();
					}
					break;
			}
		}
		
		/** 打开自动挂机界面 */
		private function openAutoPlay():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 1) {			
				NewerHelpData.curStep++;
				_uiManager.doTask_40();
			}
		}
			
		/** 关闭自动挂机界面 */
		private function closeAutoPlay():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 40 && NewerHelpData.curStep != 5) {			
				NewerHelpData.curStep = 5;
				_uiManager.doTask_40();
			}
		}
		
		/** 点击自动设置 */
		private function clickAutoSet():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 2) {			
				NewerHelpData.curStep++;
				_uiManager.doTask_40();
			}
		}
		
		/** 点击保存设置 */
		private function clickSaveSet():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 3) {			
				NewerHelpData.curStep++;
				_uiManager.doTask_40();
			}
		} 
			
		/** 点击开始挂机 */
		private function clickStartAutoPlay():void
		{
//			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
//			if(NewerHelpData.curType == 40) {			
//				NewerHelpData.curStep = 5;
//				_uiManager.doTask_40();
//			}
		}
		
		/** 打开新手成就 */
		private function openNewerAward():void
		{
			NewerHelpData.deleteIKnow(NewerHelpData.LEV13_BAO_IKNOW);	//删除13级提示  到15级即可获得第一个新手成就大礼包 
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 37 && NewerHelpData.curStep == 1) {			//任务37
				NewerHelpData.curStep++;
				_uiManager.doTask_37();
			}
		}
		
		/** 关闭新手成就 */
		private function closeNewerAward():void
		{
			NewerHelpData.deleteIKnow(NewerHelpData.LEV13_BAO_IKNOW);	//删除13级提示  到15级即可获得第一个新手成就大礼包 
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 37) {			//任务37
				NewerHelpData.curStep = 4;
				_uiManager.doTask_37();
			}
		}
		
		/** 点击领取奖励 新手成就 */
		private function clickGetNewerAward():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 37 && NewerHelpData.curStep == 2) {			//任务37
				NewerHelpData.curStep++;
				_uiManager.doTask_37();
			}
		}
		
		/** 点击某一条新手成就 */
		private function clickItemNewerAward(itemIndex:uint):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 37 && NewerHelpData.curStep == 3 && itemIndex == 1) {			//任务37
				NewerHelpData.curStep++;
				_uiManager.doTask_37();
			}
		}
		
		/** 打开场景地图 */
		private function openSenceMap():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 26 && NewerHelpData.curStep == 1) {			//任务26
				NewerHelpData.curStep++;
				_uiManager.doTask_26();
			}
		}
		
		/** 关闭场景地图 */
		private function closeSenceMap():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 26 && NewerHelpData.curStep == 2) {			//任务26
				NewerHelpData.curStep++;
				_uiManager.doTask_26();
			}
		}
		
		/** 打开商城 */
		private function openMarket():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 22 && NewerHelpData.curStep == 1) {			//任务22  购买武道乾坤
				NewerHelpData.curStep++;
				if(_dataProxy.MarketIsOpen && MarketConstData.curPageIndex == 7) NewerHelpData.curStep++;
				if(NewerHelpData.havaMarketItemByType(610046)) NewerHelpData.curStep = 4;
				_uiManager.doTask_22();
				return;
			}
		}
		
		/** 关闭商城 */
		private function closeMarket():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 22) {
				NewerHelpData.curStep = 5;
				_uiManager.doTask_22();
			}
		}
		
		/** 点击点券页签 */
		private function clickDQBtnMarket():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 22 && NewerHelpData.curStep == 2) {			//任务22  购买武道乾坤
				NewerHelpData.curStep++;
				if(NewerHelpData.havaMarketItemByType(610046)) NewerHelpData.curStep++;
				_uiManager.doTask_22();
				return;
			}
		}
		
		/** 点击武道乾坤道具 */
		private function clickWDQKMarket():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 22 && NewerHelpData.curStep == 3) {			//任务22  购买武道乾坤
				NewerHelpData.curStep++;
				_uiManager.doTask_22();
				return;
			}
		}
		
		/** 点击购买按钮 */
		private function clickBuyBtnMarket():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 22 && NewerHelpData.curStep == 4) {			//任务22  购买武道乾坤
				NewerHelpData.curStep++;
				_uiManager.doTask_22();
				return;
			}
		}
		
		/** 关闭仓库 */
		private function closeDepot():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 13 && NewerHelpData.curStep == 1) {			//任务13  关闭仓库
				NewerHelpData.curStep++;
				if(_dataProxy.BagIsOpen) NewerHelpData.curStep++;
				_uiManager.doTask_13();
				return;
			}
		}
		
		/** 人物穿上装备，继续下一步 */
		private function dressOn(taskId:uint):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 1 && NewerHelpData.curStep == 2) {			//任务1   上了刀
				NewerHelpData.curStep++;
				_uiManager.doTask_1();
				return;
			}
			if(NewerHelpData.curType == 3 && NewerHelpData.curStep == 2) {			//任务3   上了袍子
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(NewerHelpData.curType == 5 && NewerHelpData.curStep == 2) {			//任务5   上了帽子
				NewerHelpData.curStep++;
				_uiManager.doTask_5();
				return;
			}
		}
		
		/** 人物升级 */
		private function levUp(type:uint):void
		{
			if(type == 0) {
				if(NewerHelpData.curType == 9 && NewerHelpData.curStep == 2) {		//任务9   手动升级
					NewerHelpData.curStep++;
					_uiManager.doTask_9();
				}
			} else if(type == 1) {
				if(NewerHelpData.curType == 10 && NewerHelpData.curStep == 2) {		//任务10  提升职业等级到10级
					NewerHelpData.curStep++;
					if(GameCommonData.Player.Role.Level > 11) {
						NewerHelpData.curStep++;
					}
					_uiManager.doTask_10();
				}
			} else if(type == 2) {
				if(NewerHelpData.curType == 10 && NewerHelpData.curStep == 3) {		//任务10  人物等级升到12级
					NewerHelpData.curStep++;
					_uiManager.doTask_10();
				}
			}
		}
		
		/** NPC商店放物品进去 */
		private function npcShopInGood():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 12 && NewerHelpData.curStep == 1) {		//任务12
				NewerHelpData.curStep++;
				_uiManager.doTask_12();
				return;
			}
		}
		
		/** NPC商店点击出售按钮 */
		private function npcShopClickSale():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 12 && NewerHelpData.curStep == 2) {		//任务12
				NewerHelpData.curStep++;
				_uiManager.doTask_12();
				return;
			}
		}
		
		/** 关闭NPC商店 */
		private function closeNPCShop():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 12) {						//任务12
				NewerHelpData.curStep = 3;
				_uiManager.doTask_12();
			}
		}
		
		/** 删除格子 */
		private function delGrid(gridInfo:Object):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			var type:uint = gridInfo.type;
			var pos:uint  = gridInfo.pos;
			if(NewerHelpData.curType == 11 && (NewerHelpData.curStep == 2 || NewerHelpData.curStep == 3) && type == NewerHelpData.curType) {			//任务11
				NewerHelpData.curStep = 3;
				_uiManager.doTask_11(pos);
				return;
			}
			if(NewerHelpData.curType == 41 && (NewerHelpData.curStep == 4 || NewerHelpData.curStep == 5) && type == NewerHelpData.curType) {			//任务11
				NewerHelpData.curStep = 5;
				_uiManager.doTask_41(pos);
				return;
			}
			if(NewerHelpData.curType == 44 && (NewerHelpData.curStep == 4 || NewerHelpData.curStep == 5) && type == NewerHelpData.curType) {			//任务11
				NewerHelpData.curStep = 5;
				_uiManager.doTask_44(pos);
				return;
			}
		}
		
		/** 打开人物属性面板 */
		private function openHeroProp():void
		{
			_uiManager.clearItemExp();
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 9 && NewerHelpData.curStep == 1) {			//任务9
				NewerHelpData.curStep++;
				_uiManager.doTask_9();
				return;
			}
			if(NewerHelpData.curType == 10 && NewerHelpData.curStep == 1) {			//任务10
				NewerHelpData.curStep++;
				_uiManager.doTask_10();
				return;
			}
		}
		
		/** 关闭人物属性面板 */
		private function closeHeroProp():void
		{
//			_uiManager.clearItemExp();
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 9 && NewerHelpData.curStep == 2) {			//任务9
				NewerHelpData.curStep++;
				_uiManager.doTask_9();
				return;
			}
			if(NewerHelpData.curType == 10) {			//任务10  && NewerHelpData.curStep == 2
//				NewerHelpData.curStep++;
				NewerHelpData.curStep = 5;
				_uiManager.doTask_10();
				return;
			} 
		}
		
		/** 向快捷键上面放格子 */
		private function useQuickKey(typeObj:Object):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			var type:uint = typeObj.type;
			if(type == 320001 && NewerHelpData.curType == 7 && NewerHelpData.curStep == 2) {	//任务7	 拖放宠物药到快捷栏
				NewerHelpData.curStep++;
				_uiManager.doTask_7();
				return;
			}
		}
		
		/** 打开背包 */
		private function showBag():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 1 && NewerHelpData.curStep == 1) {		//任务1  得到侠义剑 	打开背包
				NewerHelpData.curStep++;
				_uiManager.doTask_1();
				return;
			}
			if(NewerHelpData.curType == 3 && NewerHelpData.curStep == 1) {		//任务3  得到侠义袍 	打开背包
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(NewerHelpData.curType == 5 && NewerHelpData.curStep == 1) {		//任务3  得到侠义冠 	打开背包
				NewerHelpData.curStep++;
				_uiManager.doTask_5();
				return;
			}
			if(NewerHelpData.curType == 7 && NewerHelpData.curStep == 1) {		//任务7  得到宠物药 	打开背包
				NewerHelpData.curStep++;
				_uiManager.doTask_7();
				return;
			}
			if(NewerHelpData.curType == 8 && NewerHelpData.curStep == 3) {		//任务8  得到湛卢剑 	打开背包
				NewerHelpData.curStep++;
				_uiManager.doTask_8();
				return;
			}
			if(NewerHelpData.curType == 11 && NewerHelpData.curStep == 1) {		//任务11  得到门派时装 	打开背包
				NewerHelpData.curStep++;
				_uiManager.doTask_11();
				return;
			}
			if(NewerHelpData.curType == 13 && NewerHelpData.curStep == 1) {		//任务13  扩充仓库
				_uiManager.doTask_13();
				return;
			}
			if(NewerHelpData.curType == 13 && NewerHelpData.curStep == 2) {		//任务13  扩充仓库
				NewerHelpData.curStep++;
				_uiManager.doTask_13();
				return;
			}
			if(NewerHelpData.curType == 41 && NewerHelpData.curStep == 1) {		//任务41  
				NewerHelpData.curStep++;
				_uiManager.doTask_41();
				return;
			}
			if(NewerHelpData.curType == 44 && NewerHelpData.curStep == 1) {		//任务44  
				NewerHelpData.curStep++;
				_uiManager.doTask_44();
				return;
			}
		}
		
		/** 关闭背包通知新手任务系统（做相应关闭类处理） */
		private function closeBag():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 1 && (NewerHelpData.curStep == 2 || NewerHelpData.curStep == 3)) {	//任务1
				NewerHelpData.curStep = 4;
				_uiManager.doTask_1();
				return;
			}
			if(NewerHelpData.curType == 3 && NewerHelpData.curStep == 2) {	//任务3
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(NewerHelpData.curType == 5 && NewerHelpData.curStep == 2) {	//任务5
				NewerHelpData.curStep++;
				_uiManager.doTask_5();
				return;
			}
			if(NewerHelpData.curType == 7 && NewerHelpData.curStep == 2) {	//任务7
				NewerHelpData.curStep++;
				_uiManager.doTask_7();
				return;
			}
			if(NewerHelpData.curType == 8 && NewerHelpData.curStep == 4) {	//任务8
				NewerHelpData.curStep++;
				_uiManager.doTask_8();
				return;
			}
			if( (NewerHelpData.curType == 11 && NewerHelpData.curStep == 2) || (NewerHelpData.curType == 11 && NewerHelpData.curStep == 3) ) {	//任务11
				NewerHelpData.curStep = 4;
				_uiManager.doTask_11();
				return;
			}
			if(NewerHelpData.curType == 12) {								//任务12
				NewerHelpData.curStep = 3;
				_uiManager.doTask_12();
				return;
			}
			if(NewerHelpData.curType == 13 && NewerHelpData.curStep == 1) {	//任务13
				_uiManager.clearView();
				return;
			}
			if(NewerHelpData.curType == 13 && NewerHelpData.curStep == 3) {	//任务13
				NewerHelpData.curStep++;
				_uiManager.doTask_13();
				return;
			}
			if(NewerHelpData.curType == 41) {
				NewerHelpData.curStep = 7;
				_uiManager.doTask_41();
				return;
			}
			if(NewerHelpData.curType == 44) {
				NewerHelpData.curType = 0;
				NewerHelpData.curStep = 0;
				_uiManager.clearView();
				return;
			}
		}
		
		/** 关闭任务 */
		private function closeHelp(type:uint):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			switch(type) {
//				case 1:		//关闭  任务1 （装备上侠义剑）
//					if(NewerHelpData.curType == 1 && NewerHelpData.curStep == 2) {
//						NewerHelpData.curStep++;
//						_uiManager.doTask_1();
//					}
//					break;
				case 3:		//关闭  任务3 （装备上侠义袍）
					if(NewerHelpData.curType == 3 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep++;
						_uiManager.doTask_3();
					}
					break;
				case 5:		//关闭  任务5 （装备上侠义冠）
					if(NewerHelpData.curType == 5 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep++;
						_uiManager.doTask_5();
					}
					break;
				case 6:		//关闭  任务6  （宠物加点）
					if(NewerHelpData.curType == 6 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep++;
						_uiManager.doTask_6();
					}
					break;
				case 8:		//关闭	任务8 （皇陵拿刀）
					if(NewerHelpData.curType == 8 && NewerHelpData.curStep == 4) {
						NewerHelpData.curStep++;
						_uiManager.doTask_8();
					}
					break;
				case 13:	//关闭 	任务13  （4级背包扩充）
					if(NewerHelpData.curType == 13 && NewerHelpData.curStep == 3) {
						NewerHelpData.curStep++;
						_uiManager.doTask_13();
					}
					break;
				case 18:
					if(NewerHelpData.curType == 18 && NewerHelpData.curStep == 1) {
						NewerHelpData.curStep++;
						_uiManager.doTask_18();
					}
					break;
				case 34:
					if(NewerHelpData.curType == 34 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep++;
						_uiManager.doTask_34();
					}
					break;
				case 35:
					if(NewerHelpData.curType == 35 && NewerHelpData.curStep == 2) {
						NewerHelpData.curStep++;
						_uiManager.doTask_35();
					}
					break;
				case 46:
					if(NewerHelpData.curType == 46 && NewerHelpData.curStep == 1) {
						NewerHelpData.curStep = 3;
						_uiManager.doTask_46();
					}
					break;
			}
		}
		
		/** 打开宠物面板 */
		private function openPet():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 4 && NewerHelpData.curStep == 1) {  //任务4-获得宠物后，点击宠物按钮
				NewerHelpData.curStep++;
				_uiManager.doTask_4();
				return;
			}
			if(NewerHelpData.curType == 6 && NewerHelpData.curStep == 1) {	//任务6 - 宠物获得潜力点
				NewerHelpData.curStep++;
				_uiManager.doTask_6();
				return;
			}
			if(NewerHelpData.curType == 34 && NewerHelpData.curStep == 1) {	//任务34 - 宠物获得潜力点
				NewerHelpData.curStep++;
				_uiManager.doTask_34();
				return;
			}
			if(NewerHelpData.curType == 35 && NewerHelpData.curStep == 1) {	//任务35 - 宠物获得潜力点
				NewerHelpData.curStep++;
				_uiManager.doTask_35();
				return;
			}
			
		}
				
		/** 关闭宠物面板 */
		private function closePet():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 4 && NewerHelpData.curStep == 2) {	//任务4 - 宠物出战
				NewerHelpData.curStep++;
				_uiManager.doTask_4();
				return;
			}
			if(NewerHelpData.curType == 6 && NewerHelpData.curStep == 2) {	//任务6 - 宠物获得潜力点
				NewerHelpData.curStep++;
				_uiManager.doTask_6();
				return;
			}
			if(NewerHelpData.curType == 34 && NewerHelpData.curStep == 2) {	//任务34 - 宠物获得潜力点
				NewerHelpData.curStep++;
				_uiManager.doTask_34();
				return;
			}
			if(NewerHelpData.curType == 35 && NewerHelpData.curStep == 2) {	//任务35 - 宠物获得潜力点
				NewerHelpData.curStep++;
				_uiManager.doTask_35();
				return;
			}
		}
		
		/** 宠物出战 */
		private function fightPet():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 4 && NewerHelpData.curStep == 2) {	//任务4 - 宠物出战
				NewerHelpData.curStep++;
				_uiManager.doTask_4();
				return;
			}
		}
		
		/** 打开任务接受面板 */
		private function openTaskAccept():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 17 && NewerHelpData.curStep == 2) {			//任务17 - 点击“第一把武器”任务
				NewerHelpData.curStep++;
				_uiManager.doTask_17();
				return;
			}
		}
		
		/** 关闭任务说明面板 */
		private function closeTaskInfoShow():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 17 && NewerHelpData.curStep == 3) {			//任务17 - 点击“第一把武器”任务
				NewerHelpData.curStep++;
				_uiManager.doTask_17();
				return;
			}
		}
		
		/** 任务变成完成状态时触发提示 */
		private function taskUpdate(taskUpd:Object):void
		{
			var taskId:uint = taskUpd.id;
			if(taskId == 0) return;
			if(taskId == 303) {
				sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 2);
				return;
			}
			if(taskId == 202) {	//打完8个山猪肉
				var a:int = NewerHelpData.curType;
				var b:int = NewerHelpData.curStep;
				if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 5) {
					NewerHelpData.curStep++;
					_uiManager.doTask_40();
				}
//				 else {
//					NewerHelpData.curType = 0;
//					NewerHelpData.curStep = 0;
//				}
				return;
			}
			if(taskId == 311 && NewerHelpData.curType == 8 && NewerHelpData.curStep == 5) {		//点击刺客自动寻路
				NewerHelpData.curStep++;
				_uiManager.doTask_8();
				return;
			}
			if(taskId == 306 && NewerHelpData.curType == 46) {
				NewerHelpData.curStep = 3;
				_uiManager.doTask_46();
			}
			if( NewerHelpData.curType == 19 && NewerHelpData.curStep == 1) {
				NewerHelpData.curStep++;
				_uiManager.doTask_19();
			}
		}
		
		/** 点击NPC */
		private function clickNPC(npcObj:Object):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			var id:uint = npcObj.id;
			var newerData:Object = npcObj.newerData;
			var npc:Object = GameCommonData.SameSecnePlayerList[id];
			if(!npc) return;
			var monsterType:uint = npc.Role.MonsterTypeID;
			if(monsterType == 9001 && NewerHelpData.curType == 8 && NewerHelpData.curStep == 1) {	//任务8  点击皇陵张择端
				NewerHelpData.curStep++;
				_uiManager.doTask_8();
				return;
			}
			if(newerData != null) {
				var curType:uint = newerData.curType;
				var curStep:uint = newerData.curStep;
				var obj:Object = NewerHelpData.changeId( 0, 120 );
				if(monsterType == obj._npcId && curType == 20 && curStep == 1 && curType == NewerHelpData.curType && curStep == NewerHelpData.curStep) {//屠大勇
					NewerHelpData.curStep++;
					_uiManager.doTask_20();
					return;
				}
				obj = NewerHelpData.changeId( 0, 116 );
				if(monsterType == obj._npcId && curType == 1 && curStep == 4 && curType == NewerHelpData.curType && curStep == NewerHelpData.curStep) {	//张老头
					NewerHelpData.curStep++;
					_uiManager.doTask_1();
					return;
				}
			}
//			if( (monsterType == 116 || monsterType == 3416 || monsterType == 3616 || monsterType == 3816 || monsterType == 4016) && (NewerHelpData.curType == 17 && NewerHelpData.curStep == 3) ) {	//任务17 	不同新手城的张老头
//				_uiManager.doTask_17();
//				return;
//			}
		}
		
		/** 关闭NPC对话 */
		private function closeNPCChat():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 17 && NewerHelpData.curStep == 2) {		//任务17  	关闭2级提示 
				_uiManager.clearView();
				return;
			}
			if(NewerHelpData.curType == 20 && NewerHelpData.curStep == 2) {	//任务20 去找屠大勇
				NewerHelpData.curStep++;
				_uiManager.doTask_20();
				return;
			}
		}
		
		/** 点击自动寻路 */
		private function taskPathNew(taskPath:Object):void
		{
			var taskId:uint = taskPath.id;
			if(taskId == 0) return;
			if(taskId == 303 && NewerHelpData.curType == 2 && NewerHelpData.curStep == 1) {		//任务2  完成杀四只绒毛兔， 点击寻路，删除框
				NewerHelpData.curStep++;
				_uiManager.doTask_2();
				return;
			}
			if(taskId == 301 && NewerHelpData.curType == 20 && NewerHelpData.curStep == 1) {	//任务20 去找屠大勇
//				NewerHelpData.curStep++;
//				_uiManager.doTask_20();
				return;
			}
			if(taskId == 302 && NewerHelpData.curType == 1 && NewerHelpData.curStep == 4) {		//任务1  拿了武器回去找张老头
//				NewerHelpData.curStep++;
//				_uiManager.doTask_1();
				return;
			}
			if(taskId == 304 && NewerHelpData.curType == 3 && NewerHelpData.curStep == 3) {		//任务3  拿了袍子回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(taskId == 305 && NewerHelpData.curType == 4 && NewerHelpData.curStep == 3) {		//任务4  拿了宠物回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_4();
				return;
			}
			if(taskId == 306 && NewerHelpData.curType == 4 && NewerHelpData.curStep == 3) {		//任务5  拿了头回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_5();
				return;
			}
			if(taskId == 303 && NewerHelpData.curType == 27 && NewerHelpData.curStep == 1) {	//任务27  点击寻路绒毛兔  
				NewerHelpData.curStep++;
				_uiManager.doTask_27();
				return;
			}
			if(taskId == 101 && NewerHelpData.curType == 28 && NewerHelpData.curStep == 1) {	//任务28  升到10级提示找张择端
				NewerHelpData.curStep++;
				_uiManager.doTask_28();
				return;
			}
			if(taskId == 102 && NewerHelpData.curType == 29 && NewerHelpData.curStep == 1) {	//任务29  第一次加入门派
				NewerHelpData.curStep++;
				_uiManager.doTask_29();
				return;
			}
			if(NewerHelpData.curType == 30 && NewerHelpData.curStep == 1 && (taskId == 103 || taskId == 107 || taskId == 111 || taskId == 115 || taskId == 119 || taskId == 123) ) {	//任务30  拜入师门
				NewerHelpData.curStep++;
				_uiManager.doTask_30();
				return;
			}
			if((taskId == 104 || taskId == 108 || taskId == 112 || taskId == 116 || taskId == 120 || taskId == 124) && NewerHelpData.curType == 0) {					//任务9  第一次加入门派
				_uiManager.clearView();
				return;
			}
			if(taskId == 128 && NewerHelpData.curType == 32 && NewerHelpData.curStep == 1) {	//任务32  去找白云飞
				NewerHelpData.curStep++;
				_uiManager.doTask_32();
				return;
			}
			var a:int = NewerHelpData.curType;
			var b:int = NewerHelpData.curStep;
			if(taskId == 202 && NewerHelpData.curType == 40 && NewerHelpData.curStep == 6) {	//打完8个山猪肉
				NewerHelpData.curStep++;
				_uiManager.doTask_40();
				return;
			}
			if(taskId == 309 && NewerHelpData.curType == 41 && NewerHelpData.curStep == 7) {	//打完8个山猪肉
				NewerHelpData.curStep++;
				_uiManager.doTask_41();
				return;
			}
			if(taskId == 304 && NewerHelpData.curType == 3 && NewerHelpData.curStep == 3) {		//点击苏小小另宠物
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(taskId == 311 && NewerHelpData.curType == 8 && NewerHelpData.curStep == 5) {		//点击刺客自动寻路
				NewerHelpData.curStep++;
				_uiManager.doTask_8();
				return;
			}
//			if(taskId == 202) {	//打完8个山猪肉
//				if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 5) {
//					NewerHelpData.curStep++;
//					_uiManager.doTask_40();
//				} else {
//					NewerHelpData.curType = 0;
//					NewerHelpData.curStep = 0;
//				}
//				return;
//			}
			
		}
		
		/** 关闭任务追踪 */
		private function closeTaskFollow():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 2) {		//任务2
				NewerHelpData.curStep = 2;
				_uiManager.doTask_2();
				return;
			}
			if(NewerHelpData.curType == 20) {		//任务20
				NewerHelpData.curStep = 3;
				_uiManager.doTask_20();
				return;
			}
			if(NewerHelpData.curType == 1 && NewerHelpData.curStep == 3) {	//任务1  拿了武器回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_1();
				return;
			}
			if(NewerHelpData.curType == 3 && NewerHelpData.curStep == 3) {	//任务3  拿了袍子回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(NewerHelpData.curType == 4 && NewerHelpData.curStep == 3) {	//任务4  拿了宠物回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_4();
				return;
			}
			if(NewerHelpData.curType == 5 && NewerHelpData.curStep == 3) {	//任务5  拿了头回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_5();
				return;
			}
			if(NewerHelpData.curType == 28 && NewerHelpData.curStep == 1) {	//任务28  升到10级提示找张择端
				NewerHelpData.curStep++;
				_uiManager.doTask_28();
				return;
			}
			if(NewerHelpData.curType == 29 && NewerHelpData.curStep == 1) {	//任务29  第一次加入门派
				NewerHelpData.curStep++;
				_uiManager.doTask_29();
				return;
			}
			if(NewerHelpData.curType == 30 && NewerHelpData.curStep == 1) {	//任务30  拜入师门
				NewerHelpData.curStep++;
				_uiManager.doTask_30();
				return;
			}
			if(NewerHelpData.curType == 32 && NewerHelpData.curStep == 1) {	//任务32  去找白云飞
				NewerHelpData.curStep++;
				_uiManager.doTask_32();
				return;
			}
			if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 6) {	//打完8个山猪肉
				NewerHelpData.curStep++;
				_uiManager.doTask_40();
				return;
			}
			if(NewerHelpData.curType == 3 && NewerHelpData.curStep == 3) {		//点击苏小小另宠物
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(NewerHelpData.curType == 8 && NewerHelpData.curStep == 5) {		//点击刺客自动寻路
				NewerHelpData.curStep++;
				_uiManager.doTask_8();
				return;
			}
			if(NewerHelpData.curType == 46) {		//鸭翅膀
				NewerHelpData.curStep = 3;
				_uiManager.doTask_46();
			}
		}
		
		/** 点击“完成”任务按钮 */
		private function taskCompNew(taskComp:Object):void
		{
			var taskId:uint = taskComp.id;
			if(taskId == 0) return;
			if(taskId == 303 && NewerHelpData.curType == 2 && NewerHelpData.curStep == 1) {		//任务2  完成杀四只绒毛兔，点击完成，删除框
				NewerHelpData.curStep++;
				_uiManager.doTask_2();
				return;
			}
			if(taskId == 301 && NewerHelpData.curType == 20 && NewerHelpData.curStep == 2) {	//任务20 去找屠大勇
				NewerHelpData.curStep++;
				_uiManager.doTask_20();
				return;
			}
			if(taskId == 302 && NewerHelpData.curType == 1 && NewerHelpData.curStep == 3) {		//任务1  拿了武器回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_1();
				return;
			}
			if(taskId == 302 && NewerHelpData.curType == 1 && NewerHelpData.curStep == 4) {		//任务1  拿了武器回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_1();
				return;
			}
			if(taskId == 304 && NewerHelpData.curType == 3 && NewerHelpData.curStep == 3) {		//任务3  拿了袍子回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_3();
				return;
			}
			if(taskId == 305 && NewerHelpData.curType == 4 && NewerHelpData.curStep == 3) {		//任务4  拿了宠物回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_4();
				return;
			}
			if(taskId == 306 && NewerHelpData.curType == 5 && NewerHelpData.curStep == 3) {		//任务5  拿了头回去找张老头
				NewerHelpData.curStep++;
				_uiManager.doTask_5();
				return;
			}
			if(taskId == 101 && NewerHelpData.curType == 28 && NewerHelpData.curStep == 1) {	//任务28  升到10级提示找张择端
				NewerHelpData.curStep++;
				_uiManager.doTask_28();
				return;
			}
			if(taskId == 102 && NewerHelpData.curType == 29 && NewerHelpData.curStep == 1) {	//任务29  第一次加入门派
				NewerHelpData.curStep++;
				_uiManager.doTask_29();
				return;
			}
			if(NewerHelpData.curType == 30 && NewerHelpData.curStep == 1 && (taskId == 103 || taskId == 107 || taskId == 111 || taskId == 115 || taskId == 119 || taskId == 123) ) {	//任务30  拜入师门
				NewerHelpData.curStep++;
				_uiManager.doTask_30();
				return;
			}
			if(taskId == 104 || taskId == 108 || taskId == 112 || taskId == 116 || taskId == 120 || taskId == 124) {			//任务9  加入门派学习武功
				_uiManager.clearView();
				return;
			}
			if(taskId == 202) {
				NewerHelpData.curType = 0;
				NewerHelpData.curStep = 0;
				_uiManager.clearView();
			}
		}
		
		/** 选中技能学习组件 */
		private function clickSkillLearnCell(info:Object):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 31 && NewerHelpData.curStep == 1) {		//任务31  第一次学习技能
				NewerHelpData.curStep++;
				_uiManager.doTask_31();
				return;
			}
			var skillId:int = info.id;
			if(NewerHelpData.curType == 45 && NewerHelpData.curStep == 1) {		//任务45  
				if(skillId == SkillData.aMainSkillId[0]) {
					NewerHelpData.curStep++;
					_uiManager.doTask_45();
					return;
				}
			}
			if(NewerHelpData.curType == 45 && NewerHelpData.curStep == 3) {		//任务45
				if(skillId == SkillData.aMainSkillId[1]) {
					NewerHelpData.curStep++;
					_uiManager.doTask_45();
					return;
				}
			}
			if(NewerHelpData.curType == 45 && NewerHelpData.curStep == 5) {		//任务45
				var a:Array = SkillData.aMainSkillId;
				if(skillId == SkillData.aMainSkillId[2]) {
					NewerHelpData.curStep++;
					_uiManager.doTask_45();
					return;
				}
			}
		}
		
		/** 点击技能学习按钮 */
		private function clickSkillLearnButton():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 31 && NewerHelpData.curStep == 2) {		//任务31  第一次学习技能
				NewerHelpData.curStep++;
				_uiManager.doTask_31();
				return;
			}
		}
		
		/** 关闭技能学习面板 */
		private function closeSkillLearnPanel():void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			if(NewerHelpData.curType == 31) {		//任务31  第一次学习技能
				NewerHelpData.curStep = 4;
				_uiManager.doTask_31();
				return;
			}
			if(NewerHelpData.curType == 45) {		//任务45  学习技能到10级
				NewerHelpData.curType = 0;
				NewerHelpData.curStep = 0;
				_uiManager.clearView();
				return;
			}
		}
		
		/** 技能升级 */
		private function skillLevUp(info:Object):void
		{
			if(NewerHelpData.curType == 0 || NewerHelpData.curStep == 0) return;
			var id:int  = info.id;
			var lev:int = info.lev;
			if(NewerHelpData.curType == 45 && NewerHelpData.curStep == 2) {		//任务45  
				if(id == SkillData.aMainSkillId[0] && SkillData.countSkillLevel(id) >= 10) {
					NewerHelpData.curStep = judgeCurStep();
					_uiManager.doTask_45();
					return;
				}
			}
			if(NewerHelpData.curType == 45 && NewerHelpData.curStep == 4) {		//任务45
				if(id == SkillData.aMainSkillId[1] && SkillData.countSkillLevel(id) >= 10) {
					NewerHelpData.curStep = judgeCurStep();
					_uiManager.doTask_45();
					return;
				}
			}
			if(NewerHelpData.curType == 45 && NewerHelpData.curStep == 6) {		//任务45
				if(id == SkillData.aMainSkillId[2] && SkillData.countSkillLevel(id) >= 10) {
					NewerHelpData.curStep = judgeCurStep();
					_uiManager.doTask_45();
					return;
				}
			}
		}
		
		/** 判断 任务45 需要的技能等级情况 */
		private function judgeCurStep():int
		{
			var res:int = 7;
			if(SkillData.countSkillLevel(SkillData.aMainSkillId[0]) < 10) {
				res = 1;
			} else if(SkillData.countSkillLevel(SkillData.aMainSkillId[1]) < 10) {
				res = 3;
			} else if(SkillData.countSkillLevel(SkillData.aMainSkillId[2]) < 10) {
				res = 5;
			}
			return res;
		}
		
		/** 更新人物经验 */
		private function updateRoleExp():void
		{
			var maxExp:uint = UIConstData.ExpDic[GameCommonData.Player.Role.Level];
			var exp:uint = GameCommonData.Player.Role.Exp;
			var lev:int = 0;
			if(exp < maxExp) return;
			for(var key:* in UIConstData.ExpDic) {
				if(int(key) > 150) continue;
				var tmpExp:int = UIConstData.ExpDic[key];
				if(exp <= tmpExp) {
					lev = int(key);
					break;
				}
			}
			if(_uiManager.curExpLevel != lev) { // && !_dataProxy.HeroPropIsOpen
				_uiManager.curExpLevel = lev;
				_uiManager.showItemExp();
			}
		}
		
		/** 初始化人物经验 */
		private function initRoleExp():void
		{
			var expDic:Dictionary = UIConstData.ExpDic;
			var maxExp:uint = UIConstData.ExpDic[GameCommonData.Player.Role.Level];
			var exp:uint = GameCommonData.Player.Role.Exp;
			var lev:int = 0;
			if(exp < maxExp) return;
			for(var key:* in UIConstData.ExpDic) {
				if(int(key) > 150) continue;
				var tmpExp:int = UIConstData.ExpDic[key];
				if(exp <= tmpExp) {
					lev = int(key);
					break;
				}
			}
			_uiManager.curExpLevel = lev;
			_uiManager.showItemExp();
		}
		
		/** 转场景成功（删除白云飞） */
		private function changeMapComp():void
		{
			if(NewerHelpData.curType == 32 && NewerHelpData.curStep == 1) {		//任务32  去找白云飞
				NewerHelpData.curStep++;
				_uiManager.doTask_32();
				return;
			}
		}
		
	}
}