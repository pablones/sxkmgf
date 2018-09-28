package GameUI.Modules.Bag.Proxy
{	
	import Controller.CooldownController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Artifice.data.ArtificeConst;
	import GameUI.Modules.Artifice.data.ArtificeData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Bag.Command.UseItemCommand;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Mediator.BagMediator;
	import GameUI.Modules.Bag.Mediator.ExtendsMediator;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.CastSpirit.Mediator.CastSpiritTransferMediator;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessEvent;
	import GameUI.Modules.NPCShop.Data.NPCShopEvent;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleEvent;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyEvent;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnEvent;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpEvent;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyEvent;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferConst;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.SetFrame;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	
	import OopsEngine.Role.RoleJob;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;
	
	public class GridManager extends Proxy
	{
		public static const NAME:String = "GridManager";
//		private var redFrame:MovieClip = null;
//		private var yellowFrame:MovieClip = null;
		private var dataProxy:DataProxy = null;
		private var tempEventData:Object;
		
		public function GridManager()
		{
			super(NAME);
		}
		
		/**
		 * 格子管理器初始化
		 *   */
		public function Initialize():void
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
//			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
//			redFrame.name = "redFrame";
//			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
//			yellowFrame.name = "yellowFrame";
			for( var i:int = 0; i<BagData.GridUnitList.length; i++ )
			{
				var gridUint:GridUnit = BagData.GridUnitList[i] as GridUnit;
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}	
		}
		
		public function Gc():void
		{
			for( var i:int = 0; i<BagData.GridUnitList.length; i++ )
			{
				var gridUint:GridUnit = BagData.GridUnitList[i] as GridUnit;
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.removeEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			if(BagData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == BagData.SelectedItem.Index) return;
			}
			SetFrame.UseFrame(event.currentTarget as DisplayObject, "RedFrame");			
		}
		 
		private function onMouseOut(event:MouseEvent):void
		{
    		SetFrame.RemoveFrame(event.currentTarget.parent, "RedFrame");
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
   			SetFrame.RemoveFrame(event.currentTarget.parent);
   			SetFrame.RemoveFrame(event.currentTarget.parent, "RedFrame");
			var index:int = int(event.target.name.split("_")[1]);
			if(BagData.GridUnitList[index].HasBag == false)
			{
				BagData.SelectedItem = null;
				facade.sendNotification(BagEvents.SHOWBTN, false);
				if(BagData.SelectIndex == 2) 
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_gri_onm_1" ], color:0xffff00}); // 任务背包不可扩展
					return;
				}
				if(!BagData.ExtendIsOpen)
				{
					var extendsMediator:ExtendsMediator = new ExtendsMediator();
					facade.registerMediator(extendsMediator);
					facade.sendNotification(BagEvents.SHOWEXTENDS);
					BagData.ExtendIsOpen = true;	
				}
				return;
			}
					
			if(BagData.GridUnitList[index].Item)
			{	
				SetFrame.UseFrame(BagData.GridUnitList[index].Grid);
//				if(UIConstData.ControlIsDown)												//原版本侦听ctrl键被按下
				if(event.ctrlKey)
				{
					if(UIConstData.getItem(BagData.GridUnitList[index].Item.Type) == null) return;  
					var id:int = BagData.GridUnitList[index].Item.Id;
					var type:int = BagData.GridUnitList[index].Item.Type;
					var name:String = UIConstData.getItem(BagData.GridUnitList[index].Item.Type).Name;
					var isBind:int = BagData.AllItems[BagData.SelectIndex][index].isBind;
					//当按下Ctrl键单击物品的时候，创建快速链接物品，目前只做装备的快速链接，以后需要再加上type区间
					//格式:id_type_name_玩家id_pos
					//id:物品ID，type:物品类型，name:物品名称, isBind:物品是否绑定
					
					if ( type>=250000 && type<300000 )					//魂魄的名字特殊处理
					{
						var soulVo:SoulVO = SoulData.SoulDetailInfos[ id ];
						if ( soulVo )
						{
							if ( soulVo.belong==1 )
							{
								name = GameCommonData.wordDic[ "mod_bag_pro_gridMa_onMouseDow_1" ];//"九阳之魄";
							}
							else
							{
								name = GameCommonData.wordDic[ "mod_bag_pro_gridMa_onMouseDow_2" ];//"九阴之魂";
							}
						}
					}
					
					var color:uint = 0;
					var obj:Object = BagData.getItemById(id);
					if(obj) color = obj.color;
					if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
						facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<1_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_"+isBind+"_"+color+">");
					} else {
						facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<1_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_"+isBind+"_"+color+">");
					}
				}
				if(BagData.GridUnitList[index].Item.IsLock)
				{
					BagData.SelectedItem = BagData.GridUnitList[index];
					facade.sendNotification(BagEvents.SHOWBTN, true);	
					return;
				}
				BagData.TmpIndex = BagData.GridUnitList[index].Index;
				BagData.SelectedItem = BagData.GridUnitList[index];
				facade.sendNotification(BagEvents.SHOWBTN, true);
				var displayObj:DisplayObject=event.target as DisplayObject;
				if(displayObj.mouseX<=2 || displayObj.mouseX>=displayObj.width-2){
					return;
				}
				if(displayObj.mouseY<=2 || displayObj.mouseY>=displayObj.height-2){
					return;
				}
				BagData.GridUnitList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
				BagData.GridUnitList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				BagData.GridUnitList[index].Item.onMouseDown();
				return;			
			}
			BagData.SelectedItem = null;	
			facade.sendNotification(BagEvents.SHOWBTN, false);			
		}
		
		private function dragThrewHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
			var bagMediator:BagMediator = facade.retrieveMediator(BagMediator.NAME) as BagMediator;
			if(!BagData.SelectedItem || !BagData.SelectedItem.Item) {
				return;
			}
			var obj:Object=UIConstData.getItem(e.Data.Type);
			if(obj!=null){
				var mask:uint=obj.Monopoly & 0x40;
				if(mask>0){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_2" ], color:0xffff00});  // 此物品不能丢弃
					return;
				}	
				if(int(obj.type/10) == 25000)  //是魂魄
				{
					if(SoulData.SoulDetailInfos[BagData.SelectedItem.Item.Id])
					{
						if((SoulData.SoulDetailInfos[BagData.SelectedItem.Item.Id] as SoulVO).composeLevel >= 3)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_2" ], color:0xffff00});  // 此物品不能丢弃
							return;
						}
					}
				}
			}
			// 物品丢弃将消失，确定要丢弃么？
			facade.sendNotification(EventList.SHOWALERT, { comfrim:bagMediator.comfrim, cancel:bagMediator.cancel, info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_3" ] });
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			var obj:Object=UIConstData.getItem(e.Data.source.Type);
			var mask:uint;
			if(obj!=null){
				mask=obj.Monopoly & 0x20;	
				if(obj.type == 351001)
				{
					mask = 0;
				}
			}
			switch(e.Data.type)
			{
				case "bag":
					
					if(BagData.GridUnitList[e.Data.index].HasBag == false )
					{
						returnItem(e.Data.source);
						return;
					}
					/* if(BagData.GridUnitList[e.Data.index].Item!=null && (BagData.GridUnitList[e.Data.index].Item as UseItem).IsCdTimer){
							e.Data.source.IsLock=false;
							return;
					} */
					DroppedIsBag(e.Data.index, e.Data.target, e.Data.source);
				break;
				 
				case "key":
				case "keyf": 
				case "quickf":
				case "quick":
					returnItem(e.Data.source);
//					BagData.GridUnitList[e.Data.source.Pos].Grid;
					facade.sendNotification(EventList.DROPINQUICK, {target:e.Data.target, source:e.Data.source,index:e.Data.index,type:e.Data.type});
				break;
				case "autoPlayHp":
				case "autoPlayMp":
				case "autoPlayPetHp":
				case "autoPlayPetToy":
					facade.sendNotification(AutoPlayEventList.ADD_ITEM_AUTOPLAYUI,{target:e.Data.target, source:e.Data.source,index:e.Data.index,type:e.Data.type});
					break;	
				//交易		
				case "mcPhoto":
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					if(e.Data.source.IsBind == 1 || e.Data.source.IsBind == 2 || mask>0)	//绑定  魂印  掩码
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_2" ], color:0xffff00});  // 此物品不能交易 
						return;
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					tempEventData = e.Data;
					if(BagData.isDealGoods(e.Data.source.Id))
					{
//						sendNotification(EventList.SHOWALERT, {comfrim:TradeGoods, cancel:cancelUse, info:'此物品是贵重物品，确定要进行此操作？' ,title:"提 示",comfirmTxt:"确 定",cancelTxt:"取 消"});
						sendNotification(EventList.SHOWALERT, {comfrim:TradeGoods, cancel:cancelUse, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作？'	"提 示"	"确 定"	"取 消"
					}
					else
					{
						TradeGoods();
					}
					////
					/* e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 	
					BagData.SelectedItem = BagData.GridUnitList[e.Data.index];
					facade.sendNotification(EventList.GOTRADEVIEW, e.Data.source.Id);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start(); */
					////
//					e.Data.source.IsLock = true;
//					BagData.SelectedItem = BagData.GridUnitList[e.Data.index];
//					facade.sendNotification(EventList.GOTRADEVIEW, e.Data.source.Id);
//					facade.sendNotification(BagEvents.SHOWBTN, true);			
				break;
				//装备 
				case "hero":
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
						return;
					}
					if(this.isEnableUseTheEquip(e.Data.source)){
						SetFrame.RemoveFrame(e.Data.source.ItemParent);
						e.Data.source.IsLock = true;	
						facade.sendNotification(EventList.GOHEROVIEW, e.Data);
					}
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				break;
				//摆摊
				case "stall":
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					if(e.Data.source.IsBind == 1 || e.Data.source.IsBind == 2 || mask>0){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_3" ], color:0xffff00});   // 此物品不能摆摊
						return;
					}
//					if(e.Data.source.IsBind == 1)
//					{
//						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"此物品已绑定，不能摆摊", color:0xffff00});
//						return;
//					}
					if(StallConstData.stallOwnerName != GameCommonData.Player.Role.Name || StallConstData.stallSelfId == 0)
					{
						return; 
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 	
					for(var i:int = 0; i<BagData.GridUnitList.length; i++)
					{
						BagData.GridUnitList[i].Grid.mouseEnabled = false;
					}
					facade.sendNotification(EventList.BAGTOSTALL, e.Data.source.Id);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				break;
				case "depot":
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					if(DepotConstData.GridUnitList[e.Data.index].HasBag == false) return;
//					if(e.Data.source.IsBind == 1)
//					{
//						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该物品已绑定，不能存放", color:0xffff00});
//						return;
//					}
//					if(String(e.Data.source.Type).indexOf("62") == 0) //任务物品 
//					{
//						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"此物品不能存放", color:0xffff00});
//						return;
//					}
					if((UIConstData.getItem(e.Data.source.Type).Monopoly & 0x02) > 0) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_4" ], color:0xffff00});  // 此物品不能存放
						return;
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					e.Data.source.IsLock = true; 
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(DepotEvent.BAGTODEPOT, {id:e.Data.source.Id, index:e.Data.index});
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				break;
				//NPC商店
				case "NPCShopSale":
					var saleMask:int = obj.Monopoly & 0x20;
					var obt:Object = IntroConst.ItemInfo[e.Data.source.Id];
					if(saleMask>0 || obt.isBind == 2){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_exe_1" ], color:0xffff00});  // 此物品不能出售
						return;
					}

					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
						return;
					}
					tempEventData = e.Data;
//					if(BagData.isDealGoods(e.Data.source.Id))
//					{
////						sendNotification(EventList.SHOWALERT, {comfrim:saleGoods, cancel:cancelUse, info:'此物品是贵重物品，确定要进行此操作？' ,title:"提 示",comfirmTxt:"确 定",cancelTxt:"取 消"});
//						sendNotification(EventList.SHOWALERT, {comfrim:saleGoods, cancel:cancelUse, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作，确定要进行此操作？'	"提 示"	"确 定"	"取 消"
//					}
//					else
//					{
						saleGoods();
//					}
					/* e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(NPCShopEvent.BAGTONPCSHOP, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start(); */
				break;
				case "NPCBusinessSale":
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)) {
						return;	
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(NPCBusinessEvent.BAG_TO_NPCBUSINESS, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				break;
				case "CreateUnityGrid":		//创建帮派需物品格子
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(UnityEvent.CREATEUNITYGRID, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				break;
				case "Contribute":
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					tempEventData = e.Data;
//					if(BagData.isDealGoods(e.Data.source.Id))//贵重物品
//					{
////						sendNotification(EventList.SHOWALERT, {comfrim:contributeDealGoods, cancel:cancelUse, info:'此物品是贵贵重物品，确定要进行此操作？' ,title:"提 示",comfirmTxt:"确 定",cancelTxt:"取 消"});		
//						sendNotification(EventList.SHOWALERT, {comfrim:contributeDealGoods, cancel:cancelUse, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作？'	"提 示"	"确 定"	"取 消"		
//					}
//					else
//					{
						contributeDealGoods();
//					}
					/* e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(UnityEvent.BAGTOCONTRIBUTE, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start(); */
				break;
				case "petBreedSingleItem"://宠物单人繁殖  准生证
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(PetBreedSingleEvent.BAG_TO_PETBREEDSINGLE, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				break;
				case "petToBabyItem":	//宠物还童  还童丹
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(PetToBabyEvent.BAG_TO_PETTOBABY, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				break;
				
				case "petSavvyUseMoneyItem"://宠物悟性提升  悟性丹
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(PetSavvyUseMoneyEvent.BAG_TO_SAVVY_USEMONEY, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				break;
				case "petSkillLearnItem"://宠物学习技能  技能书
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(PetSkillLearnEvent.BAG_TO_PETSKILLLEARN, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				break;
				case "petSkillUpItem":	//宠物提升技能  灵兽丹
					if(!CooldownController.getInstance().cooldownReady(e.Data.source.Type)){
						return;						
					}
					e.Data.source.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					facade.sendNotification(PetSkillUpEvent.BAG_TO_PETSKILLUP, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
				break;
				case "castSpiritUp":
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
						return;
					}
					var object:Object = IntroConst.ItemInfo[e.Data.source.Id];
					if( CastSpiritData.isEquip(e.Data.source.Type) == true )
					{
						if( object.castSpiritLevel > 0 )
						{
							if( object.castSpiritLevel == 10 )
							{
								UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_1"], 0xffff00);//该装备铸灵已经满级
							}
							else
							{
								SetFrame.RemoveFrame(e.Data.source.ItemParent);
								e.Data.source.IsLock = true;
								BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
								e.Data.lockIndex = BagData.SelectedItem.Index;
								facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, e.Data);
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
				break;
				case "castSpirit":
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
						return;
					}
					if( CastSpiritData.isEquip(e.Data.source.Type) == true )
					{
						if( e.Data.index == 0 )
						{
							SetFrame.RemoveFrame(e.Data.source.ItemParent);
							e.Data.source.IsLock = true;
							BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
							e.Data.lockIndex = BagData.SelectedItem.Index;
							facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, e.Data);
							UIConstData.useItemTimer.reset();
							UIConstData.useItemTimer.start();
						}
						else
						{
							UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_3"], 0xffff00);//不能放在该位置，请放在装备栏上
						}
					}
					else if( CastSpiritData.isReel( e.Data.source.Type ) == true )
					{
						facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, e.Data);
						UIConstData.useItemTimer.reset();
						UIConstData.useItemTimer.start();
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_2"], 0xffff00);//该物品无法铸灵，请放入装备
					}
				break;
				case "castSpiritTransfer":
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //请稍候
						return;
					}
					
					var o:Object = IntroConst.ItemInfo[e.Data.source.Id];
					if( CastSpiritData.isEquip(e.Data.source.Type) == true )
					{
						if( e.Data.index == 0 )
						{
							if( o.castSpiritLevel > 0 )
							{
								SetFrame.RemoveFrame(e.Data.source.ItemParent);
								e.Data.source.IsLock = true;
								BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
								e.Data.lockIndex = BagData.SelectedItem.Index;
								facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, e.Data);
								UIConstData.useItemTimer.reset();
								UIConstData.useItemTimer.start();
							}
							else
							{
								UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic[ "mod_castSpirit_med_cstm_8" ], 0xffff00);//该装备还没铸灵
							}
						}
						else if( e.Data.index == 1 )
						{
							var transfer:CastSpiritTransferMediator  = facade.retrieveMediator( CastSpiritTransferMediator.NAME ) as CastSpiritTransferMediator;
							if( transfer.checkPos(o))
							{
								SetFrame.RemoveFrame(e.Data.source.ItemParent);
								e.Data.source.IsLock = true;
								BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
								e.Data.lockIndex = BagData.SelectedItem.Index;
								facade.sendNotification( CastSpiritData.DROP_EQUIP_FROM_BAG, e.Data);
								UIConstData.useItemTimer.reset();
								UIConstData.useItemTimer.start();
							}
							transfer = null;
						}
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_2"], 0xffff00);//该物品无法铸灵，请放入装备
					}
				break;
				case "castSpiritItem":
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt(GameCommonData.wordDic["mod_Bag_pro_gridManager_4"], 0xffff00);//请先取下物品
					break;
				default:
					returnItem(e.Data.source);
				break;
			}
			if(ArtificeData.isShowView || StrengthenTransferData.isShowView)
			{
				switch(e.Data.target.type)
				{
					/** 分解装备 */
					case "ArtificeGrid":
						facade.sendNotification(ArtificeConst.ARTIFIC_MOUSEUP);
						break;
					/** 装备强化转移 */
					case "StrengthenTransferGrid":
						facade.sendNotification(StrengthenTransferConst.STRENGTHENTRANSFER_MOUSEUP);
						break;
				}
			}
		}
				
		private function DroppedIsBag(index:int, target:MovieClip, source:UseItem):void
		{
			if(BagData.GridUnitList[index].IsUsed)
			{
				if(BagData.GridUnitList[index].Item.IsLock == true) {
					BagData.SelectedItem = null;
					facade.sendNotification(BagEvents.SHOWBTN, false);			
					return;
				}
//				if(!CooldownController.getInstance().cooldownReady(BagData.GridUnitList[index].Item.Type) || !CooldownController.getInstance().cooldownReady(source.Type)){
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_gri_dro_1" ], color:0xffff00});  // 有物品正在冷却中不能进行交换
//					return;
//				}	
				//两个物品的类型不同，交换位置
				if(BagData.GridUnitList[index].Item.Type != source.Type) {
//					trace("TRADEPACKIDX = ", index+1);
//					trace("TRADEPACKIDX BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex] = ", BagData.AllItems[BagData.SelectIndex][index].id);
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					NetAction.OperateItem(OperateItem.TRADEPACKIDX, 1, BagData.AllItems[BagData.SelectIndex][index].id, BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex]);
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
					BagData.SelectedItem = BagData.GridUnitList[index];
					facade.sendNotification(BagEvents.SHOWBTN, true);			
					source.IsLock = true;
					BagData.AllItems[BagData.SelectIndex][index] = undefined;
					SetFrame.UseFrame(BagData.GridUnitList[index].Grid);
				} else  {	
					//未拖动到指定位置，不做任何处理
					if(index == source.Pos) {
						SetFrame.UseFrame(BagData.GridUnitList[index].Grid);
						return;
					}
					if(UIConstData.useItemTimer.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
						return;
					}
					//两个物品的类型一样，但是绑定类型相同，合并两个物品
					if(BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex].isBind == BagData.AllItems[BagData.SelectIndex][index].isBind) {
//						var targetID:int = BagData.AllItems[BagData.SelectIndex][index].id;	
//						source.IsLock = true;					
//						NetAction.OperateItem(OperateItem.REQUEST_COMBINEITEM, 1, targetID, BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex]);
						var targetID:int = BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex].id;	
						source.IsLock = true;					
						NetAction.OperateItem(OperateItem.REQUEST_COMBINEITEM, 1, targetID, BagData.AllItems[BagData.SelectIndex][index]);
						BagData.SelectedItem = BagData.GridUnitList[index];
						facade.sendNotification(BagEvents.SHOWBTN, true);			
					} else {//绑定类型不同交换物品
						NetAction.OperateItem(OperateItem.TRADEPACKIDX, 1, BagData.AllItems[BagData.SelectIndex][index].id, BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex]);
						BagData.SelectedItem = BagData.GridUnitList[index];	
						facade.sendNotification(BagEvents.SHOWBTN, true);			
						source.IsLock = true;
						BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex] = undefined;		
						SetFrame.UseFrame(BagData.GridUnitList[index].Grid);
					}
					UIConstData.useItemTimer.reset();
					UIConstData.useItemTimer.start();
				}
				
			}
			else
			{
//				trace("CHGPACKIDX = ", index+1);
//				trace("CHGPACKIDX BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex] = ", BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex]);
				//向没有存放物品的位置拖动，改变位置
				if(UIConstData.useItemTimer.running) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});   //请稍候
					return;
				}
				NetAction.OperateItem(OperateItem.CHGPACKIDX, 1, index+1, BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex]);
				UIConstData.useItemTimer.reset();
				UIConstData.useItemTimer.start();
				BagData.SelectedItem = BagData.GridUnitList[index];	
				facade.sendNotification(BagEvents.SHOWBTN, true);			
				source.IsLock = true;
				BagData.AllItems[BagData.SelectIndex][BagData.TmpIndex] = undefined;		
				SetFrame.UseFrame(BagData.GridUnitList[index].Grid);				
			}
		}

		/**
		 * 物品交易
		 * 
		 */		
		private function TradeGoods():void
		{
			tempEventData.source.IsLock = true;
			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 	
			BagData.SelectedItem = BagData.GridUnitList[tempEventData.index];
			facade.sendNotification(EventList.GOTRADEVIEW, tempEventData.source.Id);
			UIConstData.useItemTimer.reset();
			UIConstData.useItemTimer.start();
			tempEventData = null;
//			BagData.SelectedItem.Item.IsLock = true;
//			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
//			facade.sendNotification(EventList.GOTRADEVIEW, BagData.SelectedItem.Item.Id);
		}
		/**
		 * 物品出售
		 * 
		 */		
		private function saleGoods():void
		{
			tempEventData.source.IsLock = true;
			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
			facade.sendNotification(NPCShopEvent.BAGTONPCSHOP, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
			UIConstData.useItemTimer.reset();
			UIConstData.useItemTimer.start();
			tempEventData = null;
		}
		/**
		 * 物品捐献
		 * 
		 */		
		private function contributeDealGoods():void
		{
			tempEventData.source.IsLock = true;
			BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
			facade.sendNotification(UnityEvent.BAGTOCONTRIBUTE, BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
			UIConstData.useItemTimer.reset();
			UIConstData.useItemTimer.start();
			tempEventData = null;
		}
		private function cancelUse():void
		{
			tempEventData = null;
		}
		//放回到原来的位置
		public function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			BagData.SelectedItem = BagData.GridUnitList[source.Pos]; 	
			facade.sendNotification(BagEvents.SHOWBTN, true);			
		}
		
		private function doubleClickHandler(event:MouseEvent):void
		{ 
			if(!BagData.SelectedItem) return;
			if(!BagData.SelectedItem.Item) {
				return;
			}
			if(BagData.SelectedItem.Item.IsLock == true) return;
			if(BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]==undefined)
			{
				sendNotification(EventList.UPDATEBAG);
				return;
			}
			if(!CooldownController.getInstance().cooldownReady(BagData.SelectedItem.Item.Type)) return;
			
			facade.sendNotification(UseItemCommand.NAME);
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
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_1" ], color:0xffff00});  // 职业不符，不能使用 
				return false;
			}
			//使用等级
			if(GameCommonData.Player.Role.Level<obj.Level){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_2" ], color:0xffff00});  // 你的等级不够，不能使用
				return false;
			}
			//姓别
			if(obj.Sex!=0 && GameCommonData.Player.Role.Sex!=(obj.Sex-1)){ 
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_3" ], color:0xffff00});  // 性别不符，不能使用 
				return false;
			}
			//耐久
			if(objInfo.amount==0){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_4" ], color:0xffff00});  // 耐久不足，不能使用
				return false;
			}
			//职业等级
			if ( obj.PlayerLevel>0 )
			{
				var curJobLevel:uint = (GameCommonData.Player.Role.RoleList [GameCommonData.Player.Role.CurrentJob-1] as RoleJob).Level;
				if ( curJobLevel < obj.PlayerLevel )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_use_ise_5" ], color:0xffff00});  // 职业等级不够，不能使用
					return false;
				}
			}
			return true;
		} 
		
	}
}