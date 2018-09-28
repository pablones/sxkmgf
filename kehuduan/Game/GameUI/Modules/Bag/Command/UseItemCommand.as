package GameUI.Modules.Bag.Command
{
	import Controller.CooldownController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Artifice.data.ArtificeConst;
	import GameUI.Modules.Artifice.data.ArtificeData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.BigDrug.Data.BigDrugData;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.CastSpirit.Mediator.CastSpiritTransferMediator;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessEvent;
	import GameUI.Modules.NPCShop.Data.NPCShopEvent;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.PlayerInfo.Mediator.CounterWorkerInfoMediator;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Mediator.ComposeSoulMediator;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.StoneMoney.Mediator.StoneMoneyMediator;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferConst;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferData;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.SetFrame;
	import GameUI.UICore.UIFacade;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Role.RoleJob;
	
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class UseItemCommand extends SimpleCommand
	{
		public static const NAME:String = "UseItemCommand";
		
		public function UseItemCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
		//	trace("UseItemCommand execute");
			if(GameCommonData.Player.Role.HP == 0)
			{				
				return;
			}
			
		
			var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var counterMediator:CounterWorkerInfoMediator=facade.retrieveMediator(CounterWorkerInfoMediator.NAME) as CounterWorkerInfoMediator;
			var friendMediator:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
			var role:GameRole=counterMediator.role;
			var item:UseItem=BagData.SelectedItem.Item as UseItem;
			var itemData:Object=UIConstData.getItem(item.Type); 
			var obj:Object = new Object();
			var mask:uint;
			if(itemData){
				mask=itemData.Monopoly & 0x20;
				if(itemData.type == 351001)
				{
					mask = 0;
				}
			}
			
			if(dataProxy.taskEquipReturnIsOpen){	//装备回收
				sendNotification(TaskCommandList.RETURN_EQUIP,BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index])
				return;
			}
			
			
			if(ComposeSoulMediator.isComposeSoulMediatorOpen){	//合成魂魄
				 if(int(BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index].type/10) != int(SoulData.soulType/10))
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_onMouseUp_1" ], color:0xffff00});//"您选择的物品不是魂魄"
					return;  
				} 
				sendNotification(ComposeSoulMediator.ADD_COMPOSE_ITEM_BY_CLICK,BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index])
				return;
			}
			
			/** 炼化 */
			if (ArtificeData.isShowView)
			{ 
				sendNotification(ArtificeConst.ADD_ITEM_TO_ARTIFICE_VIEW, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index])
				return;
			}
			
			/** 装备强化转移 */
			if (StrengthenTransferData.isShowView)
			{ 
				sendNotification(StrengthenTransferConst.ADD_ITEM_TO_STRENGTHENTRANSFER_VIEW, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index])
				return;
			}
			
			if( CastSpiritData.CastSpiritIsOpen )
			{
				if(UIConstData.useItemTimer.running) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
					return;
				}
				if( CastSpiritData.isEquip(item.Type) == true )
				{
					SetFrame.RemoveFrame(item.ItemParent);
					item.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					obj.source = item;
					obj.lockIndex = BagData.SelectedItem.Index;
					obj.index = 0;
					facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, obj);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				}
				else if( CastSpiritData.isReel( item.Type ) == true )
				{
					obj.source = item;
					obj.index = 1;
					facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, obj);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_2"], 0xffff00);//该物品无法铸灵，请放入装备
				}
				return;
			}
			
			if(CastSpiritData.CastSpiritUpIsOpen)
			{
				if(UIConstData.useItemTimer.running) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
					return;
				}
				var object:Object = IntroConst.ItemInfo[item.Id];
				if( CastSpiritData.isEquip(item.Type) == true )
				{
					if( object.castSpiritLevel > 0 )
					{
						if( object.castSpiritLevel == 10 )
						{
							UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_1"], 0xffff00);//该装备铸灵已经满级
						}
						else
						{
							SetFrame.RemoveFrame(item.ItemParent);
							item.IsLock = true;
							BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
							obj.source = item;
							obj.lockIndex = BagData.SelectedItem.Index;
							facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, obj);
							UIConstData.useItemTimer.reset();
							UIConstData.useItemTimer.start();
						}
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic[ "mod_castSpirit_med_cstm_8" ], 0xffff00);//该装备还没铸灵
					}
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_2"], 0xffff00);//该物品无法铸灵，请放入装备
				}
				return;
			}
			
			if( CastSpiritData.CastSpiritTransferIsOpen )
			{
				if(UIConstData.useItemTimer.running) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
					return;
				}
				var ob:Object = IntroConst.ItemInfo[item.Id];
				var transferView:CastSpiritTransferMediator = facade.retrieveMediator( CastSpiritTransferMediator.NAME ) as CastSpiritTransferMediator;
				if( CastSpiritData.isEquip(item.Type) == true )
				{
					if( transferView.checkLevel(ob) == true )
					{
						SetFrame.RemoveFrame(item.ItemParent);
						item.IsLock = true;
						BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
						obj.source = item;
						obj.lockIndex = BagData.SelectedItem.Index;
						facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, obj);
						UIConstData.useItemTimer.reset();
						UIConstData.useItemTimer.start();
					}
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_2"], 0xffff00);//该物品无法铸灵，请放入装备
				}
				transferView = null
				return;
			}
				
			if(dataProxy.NPCShopIsOpen)	//NPC商店
			{
				var saleMask:int = itemData.Monopoly & 0x20;
				var o:Object = IntroConst.ItemInfo[item.Id];
				if( ( saleMask >0 ) || o.isBind == 2 )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_1" ], color:0xffff00});  //此物品不能出售
					return ;
				}
//				if(BagData.isDealGoods(BagData.SelectedItem.Item.Id))
//				{
//					sendNotification(EventList.SHOWALERT, {comfrim:saleGoods, cancel:cancelUse, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作？'	"提 示"	"确 定"	"取 消" 
//				}
//				else
//				{
					saleGoods();
//				}
				/* BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
				facade.sendNotification(NPCShopEvent.BAGTONPCSHOP, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				 */
				return;
			}
			if(dataProxy.NPCBusinessIsOpen) {	//NPC跑商商店
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
				facade.sendNotification(NPCBusinessEvent.BAG_TO_NPCBUSINESS, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				return;
			}
			if(UIConstData.IsTrading)	//交易
			{
				
				if(mask>0 || BagData.SelectedItem.Item.IsBind == 1 || BagData.SelectedItem.Item.IsBind == 2){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_2" ], color:0xffff00});  //此物品不能交易
					return ;
				}
				if(BagData.isDealGoods(BagData.SelectedItem.Item.Id))
				{
					sendNotification(EventList.SHOWALERT, {comfrim:TradeGoods, cancel:cancelUse, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作？'	"提 示"	"确 定"	"取 消"
				}
				else
				{
					TradeGoods();
				}
				/* BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
				facade.sendNotification(EventList.GOTRADEVIEW, BagData.SelectedItem.Item.Id); */
				return;
				
			}
			else if(dataProxy.StallIsOpen && StallConstData.stallSelfId > 0 && StallConstData.stallOwnerName == GameCommonData.Player.Role.Name)	//摆摊
			{
				if(mask>0 || BagData.SelectedItem.Item.IsBind == 1 || BagData.SelectedItem.Item.IsBind == 2){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_3" ], color:0xffff00});//此物品不能摆摊
					return ;
				}
				if(StallConstData.stallOwnerName != GameCommonData.Player.Role.Name)
				{
					return; 
				}
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
				BagData.lockBagGridUnit(false);
				facade.sendNotification(EventList.BAGTOSTALL, BagData.SelectedItem.Item.Id);
				return;
			}
			else if(dataProxy.DepotIsOpen)	//仓库
			{
//				if(BagData.SelectedItem.Item.IsBind == 1)
//				{
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该物品已绑定，不能存放", color:0xffff00});
//					return;
//				}
//				if(String(BagData.SelectedItem.Item.Type).indexOf("62") == 0) //任务物品
//				{
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"此物品不能存放", color:0xffff00});
//					return;
//				} 
				if((UIConstData.getItem(BagData.SelectedItem.Item.Type).Monopoly & 0x2) > 0) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_4" ], color:0xffff00}); //此物品不能存放
					return;
				}
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
				facade.sendNotification(DepotEvent.BAGTODEPOT, {id:BagData.SelectedItem.Item.Id, index:-1});	
				return;
			}
			
			//如果是玫瑰花，可以赠送给好友
			if(item!=null && isRoseItem(item.Type)){
				if(role!=null && role.Type==GameRole.TYPE_PLAYER){
					if(friendMediator.isHasTheFriend(role.Name,role.Id)){
						//todo给好友赠送东西
						NetAction.presentRoseToFriend(item.Id,role.Id,item.Pos);
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_5" ], color:0xffff00}); //不是你的好友，不能赠送玫瑰
					}	
				}		
			} 
			
		
			//使用江湖指南
			if(BagData.SelectedItem.Item.Type == 501002)
			{
				facade.sendNotification(EventList.SHOWHELPVIEW);
				return;
			}
			
			//捐献物品
			if ( UnityConstData.contributeIsOpen )
			{
				if ( !CooldownController.getInstance().cooldownReady(item.Type) )
				{
					return;
				}
				if ( UIConstData.useItemTimer.running )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00} ); //请稍候
				}
				else
				{	
//					if(BagData.isDealGoods(BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index].id))//贵重物品
//					{
//						sendNotification(EventList.SHOWALERT, {comfrim:contributeDealGoods, cancel:cancelContributeGoods, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作？'	"提 示"	"确 定"	"取 消"		
//					}
//					else
//					{
						contributeDealGoods();
//					}
				}
				return;
			}
			
			//使用师徒坐标物品
			if( BagData.SelectedItem.Item.Type == 506003 )
			{
				var compassData:Object = IntroConst.ItemInfo[ BagData.SelectedItem.Item.Id ];
				if ( !facade.hasCommand( UseCompassItemCommand.NAME ) )
				{
					facade.registerCommand( UseCompassItemCommand.NAME,UseCompassItemCommand );
					sendNotification( UseCompassItemCommand.NAME,compassData );
				}
				return; 
			}
			
			//使用人皮面具坐标物品
			if( BagData.SelectedItem.Item.Type == 501033 )
			{
				var personSkinData:Object = IntroConst.ItemInfo[ BagData.SelectedItem.Item.Id ];
				if ( !facade.hasCommand( UsePersonSkinItemCommand.NAME ) )
				{
					facade.registerCommand( UsePersonSkinItemCommand.NAME,UsePersonSkinItemCommand );
					sendNotification( UsePersonSkinItemCommand.NAME,personSkinData );
				}
				return; 
			}
			
			//给玩家角色上装备
			if(BagData.SelectedItem.Item.Type < 300000)
			{
				if(UIConstData.useItemTimer.running) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
				} else { 
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();	
					if(this.isEnableUseTheEquip(BagData.SelectedItem.Item as UseItem)){
						SetFrame.RemoveFrame(BagData.SelectedItem.Item.ItemParent);
						BagData.SelectedItem.Item.IsLock = true;
						BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 	
						facade.sendNotification(RoleEvents.GETOUTFITBYCLICK, BagData.SelectedItem.Item);
					}
				}
				return;
			}
			//物品（血药等消耗品）
			if(BagData.SelectedItem.Item.Type > 300000)
			{	
				var typeMul:uint = BagData.SelectedItem.Item.Type / 1000;
				var type:int = BagData.SelectedItem.Item.Type;
				if(typeMul==351){
					if(type == 351001)	//元宝票
					{
						var dt:int = StoneMoneyMediator.distanceTime;
						if((getTimer() - StoneMoneyMediator.distanceTime) < 5000)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ext_ballAndTic" ], color:0xffff00});   //每次使用间隔时间为5秒
							return;
						}
						else
						{
							StoneMoneyMediator.distanceTime = getTimer();
						}
					}
					NetAction.UseItem(OperateItem.USE, 1, BagData.SelectedItem.Index+1, BagData.SelectedItem.Item.Id);
					return;
				}
				if(BagData.SelectedItem.Item.Type<310000&&(GameCommonData.Player.Role.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP) <= GameCommonData.Player.Role.HP)
				{   
					if(typeMul == 301 || typeMul == 302) { 		//人大小血
						if(BagData.SelectedItem.Item.Type == 301002 || BagData.SelectedItem.Item.Type == 302002) {	//小血
							if(BigDrugData.drugList[0] && BigDrugData.drugList[0].type == 1) {	//当前有大血  可以覆盖大血，需提示
								// 你还有金丹瓶没有用完，是否使用仙丹瓶覆盖？
								facade.sendNotification(EventList.SHOWALERT, {comfrim:applyUse, cancel:cancelUse, isShowClose:false, info: GameCommonData.wordDic[ "mod_bag_com_use_exe_6" ], title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});
								return;
							}
						}
					} else if(typeMul == 321 || typeMul == 322) {	//宠物红
						
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_7" ], color:0xffff00});   //生命值已满，无需恢复
						return;
					}
				}
				if(BagData.SelectedItem.Item.Type<320000&&BagData.SelectedItem.Item.Type>310000&&(GameCommonData.Player.Role.MaxMp+GameCommonData.Player.Role.AdditionAtt.MaxMP) <= GameCommonData.Player.Role.MP)
				{
					if(typeMul == 311 || typeMul == 312) {		//人大小蓝	(客户端添加剩余血量)
						if(BagData.SelectedItem.Item.Type == 311002 || BagData.SelectedItem.Item.Type == 312002) {	//小蓝
							if(BigDrugData.drugList[1] && BigDrugData.drugList[1].type == 1) {	//当前有大蓝   可以覆盖大蓝，需提示
								//你还有玉液瓶没有用完，是否使用仙露瓶覆盖？
								facade.sendNotification(EventList.SHOWALERT, {comfrim:applyUse, cancel:cancelUse, isShowClose:false, info: GameCommonData.wordDic[ "mod_bag_com_use_exe_8" ], title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});
								return;
							}
						}
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_9" ], color:0xffff00}); //气已满，无需恢复
						return; 
					}
				}
				if(BagData.SelectedItem.Item.Type >= 320000 && BagData.SelectedItem.Item.Type < 350000) {
					if(String(BagData.SelectedItem.Item.Type).indexOf("321") == 0 || String(BagData.SelectedItem.Item.Type).indexOf("322") == 0) {		//宠物滋补丹  可以不出战直接吃
						NetAction.presentRoseToFriend(BagData.SelectedItem.Item.Id, 0, BagData.SelectedItem.Index+1);
					} else if(GameCommonData.Player.Role.UsingPet) {
						NetAction.presentRoseToFriend(BagData.SelectedItem.Item.Id, GameCommonData.Player.Role.UsingPet.Id, BagData.SelectedItem.Index+1);
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_10" ], color:0xffff00}); //当前没有出战宠物
					}
					return;
				}
				if(BagData.SelectedItem.Item.Type==502038 || BagData.SelectedItem.Item.Type==502039){
					if(StallConstData.stallSelfId>0 || UIFacade.UIFacadeInstance.isLookStall() ){
						UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_bag_com_use_exe_11" ],0xffff00);  // 摆摊或查看摊位时不能使用回城符
						return ;
					}
			
					if(UIFacade.UIFacadeInstance.isTrading()){		
						UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_bag_com_use_exe_12" ],0xffff00); //  交易中不能使用回城符
						return ;
					}
				}
				if( BagData.SelectedItem.Item.Type == 501037 )
				{
					if( GameCommonData.Player.Role.unityId > 0 )
					{
						facade.sendNotification(EventList.SHOWALERT, {comfrim:applyUse, cancel:cancelUse, isShowClose:false, info:"<font color='#E2CCA5'>"+GameCommonData.wordDic["mod_bag_command_use_1"]+"<font color='#00ff00'>20000</font>\\ce"+GameCommonData.wordDic["mod_bag_command_use_2"]+"<font color='#00ff00'>50000</font>"+GameCommonData.wordDic["mod_bag_command_use_3"]+"</font>", title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});
					}                                                                                                                                   //使用道具一统江湖令将增加帮派资金                                                                      和建设度                                                                                           ，有可能达到帮派资金和建设度的上限，请确认是否使用？
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["mod_bag_command_use_4"], color:0xffff00});//你还没加入帮派
					}
					return;
				}
				//使用大喇叭
				if ( BagData.SelectedItem.Item.Type == 610040 )
				{
					facade.sendNotification( ChatEvents.SHOW_BIG_LEO );
					return;
				}			
				if(BagData.SelectedItem.Item.Type == 502003) {	//使用4级背包扩充， 通知新手引导系统
					if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 13);
				}  
				SetFrame.RemoveFrame(BagData.SelectedItem.Item.ItemParent);
				NetAction.UseItem(OperateItem.USE, 1, BagData.SelectedItem.Index+1, BagData.SelectedItem.Item.Id);
				if(BagData.SelectedItem.Item.Type == 501026) {
					sendNotification(NewerHelpEvent.USE_ITEM_NEWER_HELP, 501026); //酒
				}
				return;
			}
		}
		
		/** 确定使用小血/蓝进行覆盖 */
		private function applyUse():void
		{
			if(BagData.SelectedItem) {
				SetFrame.RemoveFrame(BagData.SelectedItem.Item.ItemParent);
				NetAction.UseItem(OperateItem.USE, 1, BagData.SelectedItem.Index+1, BagData.SelectedItem.Item.Id);
			}
		}
		
		/** 取消覆盖 */
		private function cancelUse():void
		{
		}
		
		/**
		 * 物品出售
		 * 
		 */		
		private function saleGoods():void
		{
			BagData.SelectedItem.Item.IsLock = true;
			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
			facade.sendNotification(NPCShopEvent.BAGTONPCSHOP, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
		}
		/**
		 * 物品交易
		 * 
		 */		
		private function TradeGoods():void
		{
			BagData.SelectedItem.Item.IsLock = true;
			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
			facade.sendNotification(EventList.GOTRADEVIEW, BagData.SelectedItem.Item.Id);
		}
		/**
		 * 物品捐献
		 * 
		 */		
		private function contributeDealGoods():void
		{
			BagData.SelectedItem.Item.IsLock = true;
			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
			facade.sendNotification(UnityEvent.BAGTOCONTRIBUTE, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
			UIConstData.useItemTimer.reset();
			UIConstData.useItemTimer.start();
		}
		/**取消捐献*/
		private function cancelContributeGoods():void
		{
//			sendNotification(EventList.BAGITEMUNLOCK, BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index].id);
		}
		
		private function isRoseItem(type:uint):Boolean{
			if(type>=650000 && type<=650010){
				return true;
			}
			return false;
		}
		
		/**
		 *  判断该装备是否能够放置到人物身上
		 * @param useItem ：放入物品
		 * @return  ：true :可以装备  false:不能装备上
		 * 
		 */		
		private function isEnableUseTheEquip(useItem:UseItem):Boolean{
			var currentJob:uint=(GameCommonData.Player.Role.CurrentJob==1) ? GameCommonData.Player.Role.MainJob.Job :GameCommonData.Player.Role.ViceJob.Job;
			var obj:Object=UIConstData.getItem(useItem.Type);
			var objInfo:Object=IntroConst.ItemInfo[useItem.Id]
			if(obj==null || objInfo==null)return false;
			//职业
			if(obj.Job!=0 && obj.Job!=currentJob){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_1" ], color:0xffff00});   // 职业不符，不能使用
				return false;
			}
			//使用等级
			if(GameCommonData.Player.Role.Level<obj.Level){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_2" ], color:0xffff00});  // 你的等级不够，不能使用
				return false;
			}
			//姓别
			if(obj.Sex!=0 && GameCommonData.Player.Role.Sex!=(obj.Sex-1)){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_3" ], color:0xffff00}); // 性别不符，不能使用
				return false;
			}
			//耐久
			if(objInfo.type != 250000)
			{
				if(objInfo.amount==0){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_4" ], color:0xffff00});   // 耐久不足，不能使用
					return false;
				}
			}
			//职业等级
			if ( obj.PlayerLevel>0 )
			{
				var curJobLevel:uint = (GameCommonData.Player.Role.RoleList [GameCommonData.Player.Role.CurrentJob-1] as RoleJob).Level;
				if ( curJobLevel < obj.PlayerLevel )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_5" ], color:0xffff00});   // 职业等级不够，不能使用
					return false;
				}
			}
			return true;
		} 
		
	}
}