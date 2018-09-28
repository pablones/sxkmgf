package GameUI.Modules.NPCShop.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NPCShop.Data.NPCShopConstData;
	import GameUI.Modules.NPCShop.Data.NPCShopEvent;
	import GameUI.Modules.NPCShop.Proxy.NPCShopGridManager;
	import GameUI.Modules.NPCShop.Proxy.NPCShopNetAction;
	import GameUI.Modules.NPCShop.UI.NPCShopBox;
	import GameUI.Modules.NPCShop.UI.NPCShopBoxEvent;
	import GameUI.Modules.NPCShop.UI.NPCShopUIManager;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NPCShopMediator extends Mediator
	{
		public static const NAME:String = "NPCShopMediator";
		private static const START_POS:Point = new Point();
		private var panelBase:PanelBase = null;
		private var gridSprite:MovieClip = null;
		private var dataProxy:DataProxy = null;
		private var shopUIManager:NPCShopUIManager = null;
		private var shopGridManager:NPCShopGridManager = null;
		private var yellowFilter:GlowFilter = null;
		private var redFilter:GlowFilter = null;
		
		private var npcId:int = 0;
		private var shopName:String = "";
		private var shopType:int = 0;
		private var repareCost:int = 0;	//修理花费
		
		private var pageCount:int = 0;
		private var curPage:int = 0;
		private var strArr:Array = ["\\ce","\\cs","\\cc"];
		private var strSyArr:Array = ["\\se","\\ss","\\sc"];
		private var isReparing:Boolean = false;
		
		private var shopBox:NPCShopBox = null;
		private var goodToSale:Object;
		
		public function NPCShopMediator()
		{
			super(NAME);
		}
		
		public function get shopView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOWNPCSHOPVIEW,			//打开商店
				EventList.CLOSENPCSHOPVIEW,			//关闭商店
				EventList.CLOSE_NPC_ALL_PANEL,		//关闭由NPC打开的面板
				NPCShopEvent.BAGTONPCSHOP,			//物品从背包过来
				NPCShopEvent.NPC_SHOP_STOP_DRAG,	//面板禁止拖动
				NPCShopEvent.UPDATENPCSALEMONEY 	//更新出售钱数
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOWNPCSHOPVIEW:															//打开商店
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!judge()) {
						gcAll();
						return;
					}
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.NPCSHOPVIEW});
					yellowFilter = UIUtils.getGlowFilter(0xffff00, 1, 2, 2, 4);
					redFilter = UIUtils.getGlowFilter(0xff0000, 1, 2, 2, 4);
					npcId = notification.getBody().npcId;
					shopName = notification.getBody().shopName;
					shopType = notification.getBody().shopType;
					initView();
					initData();
					addLis();
					dataProxy.NPCShopIsOpen = true;
					if(!dataProxy.BagIsOpen) {
						sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
					}
					if(dataProxy.equipPanelIsOpen) {
						sendNotification(EquipCommandList.CLOSE_EQUIP_PANEL);
					}
					break;
				case EventList.CLOSENPCSHOPVIEW:														//关闭商店
					gcAll();
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:														//关闭由NPC打开的面板
					if(dataProxy && dataProxy.NPCShopIsOpen) gcAll();
					break;
				case NPCShopEvent.BAGTONPCSHOP:
					goodToSale = notification.getBody();
					if(NPCShopConstData.goodSaleList.length >= 8) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_hn_1" ], color:0xffff00});      //"一次最多出售8件物品"
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);
					} else if(!isSaleType(goodToSale.type)){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_hn_2" ], color:0xffff00});   //"请单独出售银两物品"
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);
					} 
					else if( BagData.isDealGoods( goodToSale.id ) == true )
					{
						sendNotification(EventList.SHOWALERT, {comfrim:onComfrim, cancel:onCancel, info:GameCommonData.wordDic[ "mod_bag_com_useItemComm_exe_1" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//'此物品是贵重物品，确定要进行此操作？'	"提 示"	"确 定"	"取 消" 
					}
					else
					{
						scaleFun();
					}
					
					break;
				case NPCShopEvent.UPDATENPCSALEMONEY:													//更新出售总钱数
					updateSaleMoney();
					break;
				case NPCShopEvent.NPC_SHOP_STOP_DRAG:													//禁止拖动
					if(panelBase) {
						if( GameCommonData.fullScreen == 2 )
						{
							panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
							panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
						}else{
							panelBase.x = UIConstData.DefaultPos1.x;
							panelBase.y = UIConstData.DefaultPos1.y;
						}
						panelBase.IsDrag = false;
					}
					break;
					
			}
		}
		
		private function scaleFun():void
		{
			if( BagData.isUpAttribute( goodToSale.id ) == true )    //是否是提升过属性的装备
			{                                                                                                       //该装备已提升过属性，请确认是否卖出？
				sendNotification(EventList.SHOWALERT,{comfrim:onComfrim,cancel:onCancel,info:"<font color='#E2CCA5'>"+GameCommonData.wordDic["mod_unityNew_med_umm_han_3"]+"</font>",extendsFn:null,doExtends:0,canDrag:false} );
			}
			else
			{
				comfrimFun();
			}
		}
		
		private function comfrimFun():void
		{
			if( BagData.isHighEquip( goodToSale.id ) == true )
			{                                                                                                       //物品卖出将消失，请确认是否卖出？
				sendNotification(EventList.SHOWALERT,{comfrim:onComfrim,cancel:onCancel,info:"<font color='#E2CCA5'>"+GameCommonData.wordDic["mod_unityNew_med_umm_han_4"]+"</font>",extendsFn:null,doExtends:0,canDrag:false} );
			}
			else
			{
				onComfrim()
			}
		}
		
		private function onComfrim():void
		{
			NPCShopConstData.goodSaleList.push(goodToSale);
			updateSaleData();
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.NPC_SHOP_INGOOD_NOTICE_NEWER_HELP);
		}
		
		private function onCancel():void
		{
			sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);//解锁
		}
		
		/** 是否是同类物品，玫瑰花是银两物品，须单独出售 */
		private function isSaleType(type:int):Boolean
		{
			var  result:Boolean = true;
			if(String(type).indexOf("6500") == 0) {
				for(var i:int = 0; i < NPCShopConstData.goodSaleList.length; i++) {
					if(String(NPCShopConstData.goodSaleList[i].type).indexOf("6500") != 0) {
						result = false;
						break;
					}
				}
			} else {
				for(var j:int = 0; j < NPCShopConstData.goodSaleList.length; j++) {
					if(String(NPCShopConstData.goodSaleList[j].type).indexOf("6500") == 0) {
						result = false;
						break;
					}
				}
			}
			return result;
		}
		
		private function initView():void
		{
			panelBase = new PanelBase(shopView, shopView.width+8, shopView.height+12);
			panelBase.name = "NPCShop";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			if( GameCommonData.fullScreen == 2 )
			{
				this.panelBase.x=UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
			    this.panelBase.y=UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;	
			}else{
				this.panelBase.x=UIConstData.DefaultPos1.x;
				this.panelBase.y=UIConstData.DefaultPos1.y;
			}
			panelBase.SetTitleTxt(shopName);
			gridSprite = new MovieClip();
			shopView.addChild(gridSprite);
			gridSprite.x = 22;
			gridSprite.y = 328;
			
			shopUIManager = new NPCShopUIManager(shopView); 
			if(shopType == 0) shopUIManager.setModel(0);
			
			initGrid();
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			 
			///////
			for(var i:int = 0; i < 10; i++) {
//				shopView["mcNPCGood_"+i].mcMoneyPrice.mouseEnabled = false; 
//				shopView["mcNPCGood_"+i].mcMoneyPrice.mouseChildren = true;
				shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.mouseEnabled = false;
			}
			
			shopView.mcMoneyBuy.txtMoney.mouseEnabled = false;
			///////// 
			
			//测试 ShopCostItem
//			var item:ShopCostItem = new ShopCostItem("700002_d");
//			shopView.addChild(item);
			//
			
///测试物品数据
//			NPCShopConstData.goodTypeIdList = [];
//			for(var i:int = 0; i < 40; i++) {
//				if(i <= 9) NPCShopConstData.goodTypeIdList.push(300001);
//				if(10 <= i && i <= 19) NPCShopConstData.goodTypeIdList.push(111011);
//				if(20 <= i && i <= 29) NPCShopConstData.goodTypeIdList.push(121001);
//				if(30 <= i && i <= 39) NPCShopConstData.goodTypeIdList.push(141001);
//			}

		}
		
		/** 初始化格子 */
		private function initGrid():void
		{
			for(var i:int = 0; i < 8; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%8);
				gridUnit.y = (gridUnit.height) * int(i/8);
				gridUnit.name = "NPCShopSale_" + i.toString();
				gridSprite.addChild(gridUnit);	//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = gridSprite;								//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				NPCShopConstData.GridUnitList.push(gridUint);
			}
			shopView.addChild(gridSprite);
			shopGridManager = new NPCShopGridManager(NPCShopConstData.GridUnitList, gridSprite);
			facade.registerProxy(shopGridManager);
		}
		
		/** 初始化数据 */
		private function initData():void
		{
//			trace("NPCShopConstData.goodTypeIdList" +NPCShopConstData.goodTypeIdList);
			for(var i:int = 0; i < NPCShopConstData.goodTypeIdList.length; i++) {
				var good:Object = UIConstData.getItem(NPCShopConstData.goodTypeIdList[i]); 
				if(good) NPCShopConstData.goodList.push(good); 
			}
			pageCount = Math.ceil( (NPCShopConstData.goodList.length / 10) );	//向上取整: 2.3 = 3
			if(pageCount > 0) {
				clearData();
				curPage++;
				updateData();
				shopView.txtPageInfo.text = curPage+"/"+pageCount;
			} else {
				clearData();
				shopView.txtPageInfo.text = "1/1";
			}
		}
		
		private function gcAll():void {
			removLis();
			if(shopBox) {
				shopBox.gc();
				shopView.removeChild(shopBox);
				GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_DOWN, hideBox);
				shopBox = null;
			}
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) 
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			for(var i:int = 0; i < NPCShopConstData.goodSaleList.length; i++) {
				sendNotification(EventList.BAGITEMUNLOCK, NPCShopConstData.goodSaleList[i].id);
			}
			NPCShopConstData.SelectedItem = null;
			if(shopGridManager) shopGridManager.removeAllItem();
			viewComponent = null;
			yellowFilter = null;
			redFilter = null;
			panelBase = null;
			gridSprite = null;
			pageCount = 0;
			curPage = 0;
			npcId = 0;
			shopName = "";
			shopType = 0;
			NPCShopConstData.goodTypeIdList = [];
			NPCShopConstData.goodList = [];
			NPCShopConstData.goodSaleList = [];
			NPCShopConstData.GridUnitList = [];
			NPCShopConstData.selectedIndex - 1;
			NPCShopConstData.TmpIndex = 0;
			NPCShopConstData.payWay = 0;
			if(isReparing) {
				GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.CLICK, repareHandler);
				SysCursor.GetInstance().isLock = false;
				isReparing = false;
			}
			shopUIManager = null;
			shopGridManager = null;
			dataProxy.NPCShopIsOpen = false;
			dataProxy = null;
			facade.removeMediator(NPCShopMediator.NAME);
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.NPC_SHOP_CLOSE_NOTICE_NEWER_HELP);
		}
		
		/** 点击按钮 上页、下页、修理、修理全部、购买、出售 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnFrontPage":												//上页
					frontPage();
					break;
				case "btnBackPage":													//下页
					backPage();
					break;
				case "btnRepare":													//修理
					repare(e);
					break;
				case "btnRepareAll":												//修理全部
					repareAll();
					break;
				case "btnBuy":														//购买
					buyGood();
					break;
				case "btnSale":														//出售
					saleGood();
					break;
			}
		}
		
		/** 前一页 */
		private function frontPage():void
		{
			if(pageCount > 0 && curPage > 1) {
				removeAllFrames();
				curPage--;
				clearData();
				updateData();
				shopView.txtPageInfo.text = curPage+"/"+pageCount;
			}
		}
		/** 后一页 */
		private function backPage():void
		{
			if(pageCount > 0 && curPage < pageCount) {
				removeAllFrames();
				curPage++;
				clearData();
				updateData();
				shopView.txtPageInfo.text = curPage+"/"+pageCount;
			}
		}
		/** 单个修理 */
		private function repare(e:MouseEvent):void
		{
			//变化鼠标
			//设置一个标志（当前处在修理状态） 再点击装备的话就进行修理，点击其他就恢复正常状态。
			if(!isReparing) {
				SysCursor.GetInstance().setMouseType(SysCursor.REPAIR_CURSOR);
				SysCursor.GetInstance().isLock = true;
				isReparing = true;
				e.stopPropagation();
				GameCommonData.GameInstance.GameUI.addEventListener(MouseEvent.CLICK, repareHandler);
			} else {
				GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.CLICK, repareHandler);
				SysCursor.GetInstance().isLock = false;
				isReparing = false;
			}
		}
		/** 全部修理 */
		private function repareAll():void
		{
			repareCost = 0;
			//发送全部修理命令
			for(var i:int = 0; i < RolePropDatas.ItemList.length; i++) {
				if(RolePropDatas.ItemList[i] == undefined)
					continue;
				var eq:Object = RolePropDatas.ItemList[i];
				var repareAmount:Number = (eq.maxAmount - eq.amount)/eq.maxAmount * 0.1;	//耐久丢失比例
				var price:int = UIConstData.getItem(eq.type).PriceIn;		//价格
				var cost:int = repareAmount * price;
				repareCost += cost;	//修理花费
			}
			if(repareCost > 0) {
				var moneyCostStr:String = UIUtils.getMoneyInfo(repareCost);
				facade.sendNotification(EventList.SHOWALERT, {comfrim:applyRepareAll, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_1" ]+moneyCostStr+GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_2" ], title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ]});  
																											//"是否花费 "                                                                 " 修理身上全部装备？"         "提 示"  
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_4" ], color:0xffff00});    //"身上装备无需修理"
			}
		}
		
		/** 确定修理全部 */
		private function applyRepareAll():void
		{
			//发送修理全部的命令
			NPCShopNetAction.multiRepare(npcId);
		}
		
		/** 取消修理全部 */
		private function cancelClose():void
		{
			
		}
		
		/** 购买物品 */
		private function buyGood():void
		{
			if(NPCShopConstData.selectedIndex > -1) {
				var count:int = int(shopView.txtInputCount.text);
				if(count <= 0){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_bg_1" ], color:0xffff00});     //"请输入购买数量"
				} else {
					var index:int = (curPage-1) * 10 + NPCShopConstData.selectedIndex;
					var good:Object = NPCShopConstData.goodList[index];
					
					var moneyNeed:int = good.PriceIn * count;
					var moneyHave:int = 0;
					if(NPCShopConstData.payWay == 2) {			//碎银 支付方式
						moneyHave = GameCommonData.Player.Role.UnBindMoney;
						if(moneyNeed > moneyHave) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_bg_2" ], color:0xffff00});          //"身上碎银不足"
							return;
						}
					} else if(NPCShopConstData.payWay == 1) {	//银两支付
						moneyHave = GameCommonData.Player.Role.BindMoney;
						if(moneyNeed > moneyHave) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_bg_3" ], color:0xffff00});        //"身上银两不足"
							return;
						}
					}
					//发送购买命令
					NPCShopNetAction.buyNPCItem(good.type, npcId, count);
				}
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_bg_4" ], color:0xffff00});        //"请先选择物品"
			}
		}
		
		/** 出售物品 */
		private function saleGood():void
		{
			if(NPCShopConstData.goodSaleList.length <= 0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_bg_5" ], color:0xffff00});      //"当前没有要出售物品"
			} else {
				//发送出售物品命令  要出售的物品ID
				for(var i:int = 0; i < NPCShopConstData.goodSaleList.length; i++) {
					NPCShopNetAction.saleGood(NPCShopConstData.goodSaleList[i].id, npcId);
				}
				for(var j:int = 0; j < NPCShopConstData.goodSaleList.length; j++){
					//解锁
					sendNotification(EventList.BAGITEMUNLOCK, NPCShopConstData.goodSaleList[j].id);
				}
				shopGridManager.removeAllItem();
				NPCShopConstData.goodSaleList = [];
				shopView.mcMoneySale.txtMoney.text = "";
				ShowMoney.ShowIcon(shopView.mcMoneySale, shopView.mcMoneySale.txtMoney, true);
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.NPC_SHOP_CLICK_SALE_NOTICE_NEWER_HELP);
			}
		}
		
		/** 修理时点击UI层 判断点击对象 */
		private function repareHandler(e:MouseEvent):void
		{
			var targetName:String = e.target.name;
			var index:int = int(targetName.split("_")[1]);
			if(targetName.indexOf("bag_") > -1 && index < BagData.BagNum[BagData.SelectIndex] && BagData.SelectedItem && !BagData.AllLocks[BagData.SelectIndex][index]) {//背包有数据
				//进一步处理，分析名字，获取ID，发送修理命令 要修理的物品的ID
				var data:Object = BagData.AllItems[BagData.SelectIndex][index];
				if(data.type < 300000 && data.maxAmount != 0) {	//是装备 并且最大耐久不为0(可修)
					var amount:int = data.amount;		//当前耐久
					var maxAmount:int = data.maxAmount;	//最大持久
					if(amount < maxAmount) {	//装备需要修理
//						var repareAmount:Number = (maxAmount - amount)/maxAmount;	//耐久丢失比例
//						var price:int = UIConstData.ItemDic[data.type].PriceIn;		//价格
//						repareCost = repareAmount * 0.1 * price;	//修理花费
						//发送修理命令
						NPCShopNetAction.singleRepare(data.id, npcId);
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rph_1" ], color:0xffff00});   //"此物品无需修理"
					}
				} else {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rph_2" ], color:0xffff00});     //"此物品无法修理"
				}
			} else {
				GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.CLICK, repareHandler);
				SysCursor.GetInstance().isLock = false;
				isReparing = false;
			}
		}
		
		/** 更新出售栏物品数据 */
		private function updateSaleData():void
		{
			var index:int = NPCShopConstData.goodSaleList.length - 1;
			shopGridManager.addItem(index);
			//更新总售价
			updateSaleMoney();
		}
		
		private function panelCloseHandler(e:Event):void {
			gcAll();	
		}
		
		private function addLis():void {
			for(var i:int = 0; i < 10; i++) {
				shopView["mcNPCGood_"+i].addEventListener(MouseEvent.ROLL_OVER, mouseOverHandler);
				shopView["mcNPCGood_"+i].addEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
			}
			shopView.txtInputCount.addEventListener(Event.CHANGE, inputHandler);
			shopView.btnFrontPage.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnBackPage.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnRepare.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnBuy.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnRepareAll.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnSale.addEventListener(MouseEvent.CLICK, btnClickHandler);
			UIUtils.addFocusLis(shopView.txtInputCount);
		}
		
		private function removLis():void {
			if(shopView) {
				for(var i:int = 0; i < 10; i++) {
					shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.ROLL_OVER, mouseOverHandler);
					shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
				}
				shopView.txtInputCount.removeEventListener(Event.CHANGE, inputHandler);
				shopView.btnFrontPage.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnBackPage.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnRepare.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnBuy.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnRepareAll.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnSale.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				UIUtils.removeFocusLis(shopView.txtInputCount);
			}
		}
		
		/** 鼠标移入 */
		private function mouseOverHandler(e:MouseEvent):void
		{
			e.currentTarget.mcRed.visible = true;
		}
		/** 鼠标移出 */
		private function mouseOutHandler(e:MouseEvent):void
		{
			e.currentTarget.mcRed.visible = false;
		}
		
		/** 点击选择商品 */
		private function goodSelectHandler(e:MouseEvent):void
		{
			removeAllFrames();
			e.currentTarget.mcYellow.visible = true;
			var index:int = e.currentTarget.name.split("_")[1];
			if(index != NPCShopConstData.selectedIndex) {
				var good:Object = NPCShopConstData.goodList[(curPage-1) * 10 + index];
				if(good) {
					NPCShopConstData.selectedIndex = index;
					shopView.txtInputCount.text = "1";
					//------------------------先清除box
					if(shopBox) {
						shopBox.gc();
						shopView.removeChild(shopBox);
						NPCShopConstData.payWay = 0;
						GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_DOWN, hideBox);
						shopBox = null;
					}
					//--------------------------
					var money:String = "";
					if(good.payType > 8) {									//物品兑换道具
						NPCShopConstData.payWay = 0;
						shopView.mcMoneyBuy.txtMoney.text = good.PriceIn + "_" + good.payType;
						ShowMoney.showGoodCostItem(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney);
					} else if(good.payType == 1){							//铜板购买道具
						NPCShopConstData.payWay = 1;
						money = UIUtils.getMoneyStandInfo(good.PriceIn, strArr);
						shopView.mcMoneyBuy.txtMoney.text = money;
						ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
					} else if(good.payType == 2) {							//碎银购买道具
						NPCShopConstData.payWay = 2;
						money = UIUtils.getMoneyStandInfo(good.PriceIn, strSyArr);
						shopView.mcMoneyBuy.txtMoney.text = money;
						ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
					} else if(good.payType == 3) {							//可以进行支付方式选择（铜板或碎银）
						var type:uint = 0;
						type = (GameCommonData.Player.Role.UnBindMoney > 0 || GameCommonData.Player.Role.BindMoney == 0) ?  0 : 1;
						shopBox = new NPCShopBox(type);
						shopView.addChild(shopBox);
						shopBox.x = 247;
						shopBox.y = 281;
						shopBox.addEventListener(NPCShopBoxEvent.COMBOXEVENT_CLICKITEM, updClickMoney);
						GameCommonData.GameInstance.GameUI.addEventListener(MouseEvent.MOUSE_DOWN, hideBox);
						//-------------------------------------------------
						if(type == 0) {
							money = UIUtils.getMoneyStandInfo(good.PriceIn, strSyArr);
						} else {
							money = UIUtils.getMoneyStandInfo(good.PriceIn, strArr);
						}
						shopView.mcMoneyBuy.txtMoney.text = money;
						ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
					}
					/** 通过帮贡购买 */
					else if(good.payType == 7) 
					{
						money = GameCommonData.wordDic[ "mod_npcs_med_npc_goo_1" ] + good.PriceIn;          //帮贡:
						shopView.mcMoneyBuy.txtMoney.text = money;
					}
					/** 通过积分购买 */
					else if(good.payType == 8)
					{
						money = GameCommonData.wordDic[ "mod_npcs_med_npc_goo_2" ] + good.PriceIn;          //荣誉:
						shopView.mcMoneyBuy.txtMoney.text = money;
					}
//					trace ( shopView.mcMoneyBuy.x );
				}
			}
		}
		
		/** 输入检测 购买数量 */
		private function inputHandler(e:Event):void
		{
			switch(e.target.name)
			{
				case "txtInputCount":							//购买数量
					var num:int = int(shopView.txtInputCount.text);
					if(num <= 0) {
						shopView.txtInputCount.text = "1";
						num = 1;
					} 
					if(NPCShopConstData.selectedIndex > -1) { 
						var good:Object = NPCShopConstData.goodList[(curPage-1)*10+NPCShopConstData.selectedIndex];
						if(String(good.type).indexOf("1") == 0 || String(good.type).indexOf("2") == 0) {	//装备  最大购买个数是1
							shopView.txtInputCount.text = "1";
							num = 1;
						} else {						 		//其他物品  最大购买个数 = 该物品的叠加上限 
							if(num > good.UpperLimit) {
								shopView.txtInputCount.text = good.UpperLimit.toString();
								num = good.UpperLimit;
							} 
						}
						var price:Number = good.PriceIn;
						var money:Number = price * num;
						var moneyStr:String = "";
						if(good.payType > 8) {									//物品兑换道具
							NPCShopConstData.payWay = 0;
							shopView.mcMoneyBuy.txtMoney.text = int(money) + "_" + good.payType;
							ShowMoney.showGoodCostItem(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney);
						} else if(good.payType == 1) {							//铜板购买道具
							moneyStr = UIUtils.getMoneyStandInfo(money, strArr);
							shopView.mcMoneyBuy.txtMoney.text = moneyStr;
							ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
						} else if(good.payType == 2) {							//碎银购买道具
							moneyStr = UIUtils.getMoneyStandInfo(money, strSyArr);
							shopView.mcMoneyBuy.txtMoney.text = moneyStr;
							ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
						} else if(good.payType == 3) {							//可以进行支付方式选择（铜板或碎银）
							if(NPCShopConstData.payWay == 1) {	//银两支付
								moneyStr = UIUtils.getMoneyStandInfo(money, strArr);
							} else {							//碎银支付
								moneyStr = UIUtils.getMoneyStandInfo(money, strSyArr);
							}
							shopView.mcMoneyBuy.txtMoney.text = moneyStr;
							ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
						}
						/** 通过帮贡购买 */
						else if(good.payType == 7) 
						{
							moneyStr = GameCommonData.wordDic[ "mod_npcs_med_npc_goo_1" ] + money;         //帮贡:
							shopView.mcMoneyBuy.txtMoney.text = moneyStr;
						}
						/** 通过积分购买 */
						else if(good.payType == 8)
						{
							moneyStr = GameCommonData.wordDic[ "mod_npcs_med_npc_goo_2" ] + money;         //荣誉:
							shopView.mcMoneyBuy.txtMoney.text = moneyStr;
						}
					}
					break;
			}
		}
		
		/** 选择支付方式，更新钱 */
		private function updClickMoney(e:NPCShopBoxEvent):void
		{
			var num:int = int(shopView.txtInputCount.text);
			if(num <= 0) {
				shopView.txtInputCount.text = "1";
			} else if(NPCShopConstData.selectedIndex > -1) {
				var good:Object = NPCShopConstData.goodList[(curPage-1)*10+NPCShopConstData.selectedIndex];
				if(String(good.type).indexOf("1") == 0 || String(good.type).indexOf("2") == 0) {	//装备  最大购买个数是1
					shopView.txtInputCount.text = "1";
					num = 1;
				} else {								//其他物品  最大购买个数 = 该物品的叠加上限 
					if(num > good.UpperLimit) {
						shopView.txtInputCount.text = good.UpperLimit.toString();
						num = good.UpperLimit;
					}
				}
				var price:Number = good.PriceIn;
				var money:Number = price * num;
				var moneyStr:String = "";
				if(good.payType > 8) {									//物品兑换道具
					NPCShopConstData.payWay = 0;
					shopView.mcMoneyBuy.txtMoney.text = int(money) + "_" + good.payType;
					ShowMoney.showGoodCostItem(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney);
				} else if(good.payType == 1){							//铜板购买道具
					moneyStr = UIUtils.getMoneyStandInfo(money, strArr);
				} else if(good.payType == 2) {							//碎银购买道具
					moneyStr = UIUtils.getMoneyStandInfo(money, strSyArr);
				} else if(good.payType == 3){							//可以进行支付方式选择（铜板或碎银）
					if(NPCShopConstData.payWay == 1) {	//银两支付
						moneyStr = UIUtils.getMoneyStandInfo(money, strArr);
					} else {							//碎银支付
						moneyStr = UIUtils.getMoneyStandInfo(money, strSyArr);
					}
					shopView.mcMoneyBuy.txtMoney.text = moneyStr;
					ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
				}
			}
		}
		
		private function hideBox(e:MouseEvent):void
		{
			if(shopBox) shopBox.hide();
		}
		
		/** 移除所有红黄边框 */
		private function removeAllFrames():void
		{
			for(var i:int = 0; i < 10; i++) {
				shopView["mcNPCGood_"+i].mcRed.visible = false;
				shopView["mcNPCGood_"+i].mcYellow.visible = false;
			}
		}
		
		/** 清除画面数据 */
		private function clearData():void
		{
			for(var i:int = 0; i < 10; i++) {
				var count:int = 0;
				while(count < (shopView["mcNPCGood_"+i] as MovieClip).numChildren) {
					if((shopView["mcNPCGood_"+i] as MovieClip).getChildAt(count) is ItemBase) {
						(shopView["mcNPCGood_"+i] as MovieClip).removeChild((shopView["mcNPCGood_"+i] as MovieClip).getChildAt(count));
						break;
					}
					count++;
				}
				shopView["mcNPCGood_"+i].txtGoodName.text = "";
				shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = "";
				ShowMoney.ShowIcon(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney, true);
				shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.CLICK, goodSelectHandler);
			}
			NPCShopConstData.selectedIndex = -1;
			shopView.txtInputCount.text = "1";
			shopView.mcMoneyBuy.txtMoney.text = "";
			ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
			//---------------------------
			if(shopBox) {
				shopBox.gc();
				shopView.removeChild(shopBox);
				GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_DOWN, hideBox);
				shopBox = null;
			}
			NPCShopConstData.payWay = 0;
			//---------------------------
		}
		
		/** 更新数据 */
		private function updateData():void
		{
			var fromIndex:int = (curPage-1) * 10;
			var goodList:Array = NPCShopConstData.goodList;
			for(var i:int = 0; i < 10; i++) {
				if(goodList[fromIndex + i]) {
					var faceItem:FaceItem = new FaceItem(goodList[fromIndex + i].type, shopView["mcNPCGood_"+i]);
					faceItem.x = 2;
					faceItem.y = 2;
					faceItem.setEnable(true);
					faceItem.name = "mcNPCGoodPhoto_"+goodList[fromIndex + i].type;
					shopView["mcNPCGood_"+i].addChild(faceItem);
					var color:uint = UIConstData.getItem(goodList[fromIndex + i].type).Color;
					shopView["mcNPCGood_"+i].txtGoodName.htmlText =  '<font color="' + IntroConst.itemColors[color] + '">' + goodList[fromIndex + i].Name + '</font>'; 
					//////
					var money:String = "";
					if(goodList[fromIndex + i].payType > 8) {				//用物品兑换的道具
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = goodList[fromIndex + i].PriceIn + "_" + goodList[fromIndex + i].payType;			//修改后
						ShowMoney.showGoodCostItem(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney);
					} else if(goodList[fromIndex + i].payType == 1) {		//用铜板买的道具
						money = UIUtils.getMoneyStandInfo(goodList[fromIndex + i].PriceIn, strArr);
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = money;
						ShowMoney.ShowIcon(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney, true);
					} else if(goodList[fromIndex + i].payType == 2) {		//用碎银买的道具
						money = UIUtils.getMoneyStandInfo(goodList[fromIndex + i].PriceIn, strSyArr);
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = money;
						ShowMoney.ShowIcon(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney, true);
					} else if(goodList[fromIndex + i].payType == 3) {		//可选支付方式的道具 (铜板或碎银)
						money = UIUtils.getMoneyStandInfo(goodList[fromIndex + i].PriceIn, strSyArr);
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = money;
						ShowMoney.ShowIcon(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney, true);
					}
					/** 通过帮贡购买 */
					else if(goodList[fromIndex + i].payType == 7) 
					{
						var priceIn:int = goodList[fromIndex + i].PriceIn;
						money = GameCommonData.wordDic[ "mod_npcs_med_npc_goo_1" ] + goodList[fromIndex + i].PriceIn;       //帮贡:
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = money;
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.width = 60;
//						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.x = 54;
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.x = 45;
						(shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney as TextField).autoSize = TextFieldAutoSize.LEFT;		 
					}
					/** 通过积分购买 */
					else if(goodList[fromIndex + i].payType == 8)
					{
						money = GameCommonData.wordDic[ "mod_npcs_med_npc_goo_2" ] + goodList[fromIndex + i].PriceIn;       //荣誉:
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = money;
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.width = 60;
//						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.x = 54;
						shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.x = 45;
						(shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney as TextField).autoSize = TextFieldAutoSize.LEFT;		
					}
					//-------------------------------
					shopView["mcNPCGood_"+i].addEventListener(MouseEvent.CLICK, goodSelectHandler);
				} else {
					shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.CLICK, goodSelectHandler);
				}
			}
		}
		
		/** 更新出售栏总钱数 */
		private function updateSaleMoney():void
		{
			var money:Number = 0;
			var moneyType:int =0;
			for(var i:int = 0; i < NPCShopConstData.goodSaleList.length; i++) {
				var price:Number = UIConstData.getItem(NPCShopConstData.goodSaleList[i].type) ? UIConstData.getItem(NPCShopConstData.goodSaleList[i].type).PriceOut : 10;
//				var price:Number = UIConstData.ItemDic[NPCShopConstData.goodSaleList[i].type].PriceOut;
				//如果是可叠加物品，则 price = peice * amount
				if(NPCShopConstData.goodSaleList[i].type >= 300000)
					price = price * NPCShopConstData.goodSaleList[i].amount;
				money += price;
				if(String(NPCShopConstData.goodSaleList[i].type).indexOf("6500") == 0) moneyType = 1;
			}
			var moneyStr:String = "";
			money == 0 ? moneyStr = "" : moneyStr = UIUtils.getMoneyStandInfo(money, (moneyType == 0) ? strSyArr : strArr);
			shopView.mcMoneySale.txtMoney.text = moneyStr;
			ShowMoney.ShowIcon(shopView.mcMoneySale, shopView.mcMoneySale.txtMoney, true);
		}
		
		/** 判断当前打开商店条件是否合法 */
		private function judge():Boolean
		{
			var result:Boolean = true;
			if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_DEAD) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_judge_1" ], color:0xffff00});       //"死亡状态不能打开商店"
				result = false;
			} else if(dataProxy.TradeIsOpen) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_judge_2" ], color:0xffff00});            //"交易中不能打开商店"
				result = false;
			} else if(StallConstData.stallSelfId != 0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcs_med_npcsm_judge_3" ], color:0xffff00});     //"摆摊中不能打开商店"
				result = false;
			} else if(dataProxy.DepotIsOpen) {
				sendNotification(EventList.CLOSEDEPOTVIEW);
			}
			return result;
		}
	}
}
