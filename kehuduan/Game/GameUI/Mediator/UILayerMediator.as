package GameUI.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.BigDrug.Data.BigDrugData;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.view.MeridiansMediator;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessConstData;
	import GameUI.Modules.NPCShop.Data.NPCShopConstData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnConstData;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpConstData;
	import GameUI.Modules.Pick.Mediator.PickMediator;
	import GameUI.Modules.Pk.Data.PkData;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Modules.Trade.Data.TradeConstData;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.items.UseItem;
	
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.utils.Dictionary;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class UILayerMediator extends Mediator
	{
		public static const NAME:String = "UILayerMediator";
		private var dataProxy:DataProxy;
		private var delayNum:Number = 0;
		private var isEquip:Boolean = false;
		private var tmpObj:Object = null;
		
		public function UILayerMediator()
		{
			super(NAME, GameCommonData.GameInstance.GameUI);
		}
		
		private  function get uiLayer():Sprite
		{
			return viewComponent as Sprite;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.REGISTERCOMPLETE,
				EventList.ALLROLESREADY,
				EventList.ENTERMAPCOMPLETE,
				EventList.SHOWONLY,
				EventList.SHOWONLY_CENTER_FIVE_PANEL,
				EventList.ITEMREMOVED,
				EventList.GETINFOCOMPLETE
			]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.REGISTERCOMPLETE:
					/** 所有的注册结束初始化UI  */
					facade.sendNotification(EventList.INITVIEW);
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
				break;
				case EventList.ALLROLESREADY:
					/** 移除登录面板 */
					facade.sendNotification(EventList.REMOVELOGIN);
					/** 显示选择角色 */
					facade.sendNotification(EventList.SHOWSELECTROLE);
				break;
				case EventList.ENTERMAPCOMPLETE:
					/** 移除选择角色面板 */
//					facade.sendNotification(EventList.REMOVEELECTROLE);
					addListenerUIComponet();
				break;
				case EventList.SHOWONLY:
					showOnly(notification.getBody() as String);
				break;
				case EventList.SHOWONLY_CENTER_FIVE_PANEL:
					showOnlyFive(notification.getBody() as String);
				break;
				case EventList.ITEMREMOVED:
					var flag:String = notification.getBody().flag as String;
					switch(flag)
					{
						case "CounterWorker":
							uiLayer.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						break;
						case "Role":
							uiLayer.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						break;
					}					
				break;
				case EventList.GETINFOCOMPLETE:
					getInfoComplete(notification.getBody());
				break;
			}
		}
		
		private function addListenerUIComponet():void
		{
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_OVER, getToolTip);
		}                              
		
		private function getToolTip(event:MouseEvent):void
		{			
			var name:Array = event.target.name.split("_");
			
//			trace(event.target.name);
			var data:Object;
			UIConstData.ToolTipShow = true;
			var quickKeyItem:*;
			switch(name[0])
			{
				case "bag":
					clearInterval(delayNum);
					if(BagData.GridUnitList[int(name[1])].HasBag == false)
					{
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[16]}); 
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						return;
					}
					data= BagData.AllItems[BagData.SelectIndex][int(name[1])];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					if(data.type == 351001)	//元宝票,为解决悬浮框显示不准确bug
					{
						if(IntroConst.ItemInfo[data.id])
						{
							IntroConst.ItemInfo[data.id] = data;
						}
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "castSpiritItem":
					clearInterval(delayNum);
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:IntroConst.ItemInfo[uint(name[1])].type, isEquip:true, data:IntroConst.ItemInfo[uint(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					break;
				case "bagQuickKey":
					clearInterval(delayNum);
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:IntroConst.ItemInfo[uint(name[1])].type, isEquip:true, data:IntroConst.ItemInfo[uint(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					break;
				case "fourthStiletto":
					clearInterval(delayNum); 
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, isEquip:true, data:IntroConst.IntroDic[107]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "bagQuickKeyItem":
					clearInterval(delayNum);
					data= BagData.getItemById(int(name[1]));
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
					
					break;	
				case "key":
						clearInterval(delayNum);
						var quickDatas:Dictionary = QuickBarData.getInstance().quickKeyDic;
						quickKeyItem=QuickBarData.getInstance().quickKeyDic[name[1]];
						if(quickKeyItem==null || quickKeyItem==0)return;
//						delayNum = setInterval(showQuickBarToolTip, 100, quickKeyItem as UseItem);
						this.showQuickBarToolTip(quickKeyItem as UseItem);
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;		
				case "keyF":
						clearInterval(delayNum);
						var expDatas:Dictionary = QuickBarData.getInstance().expandKeyDic;
						quickKeyItem=QuickBarData.getInstance().expandKeyDic[name[1]];
						if(quickKeyItem==null || quickKeyItem==0)return;
//						delayNum = setInterval(showQuickBarToolTip, 100, quickKeyItem as UseItem);
						this.showQuickBarToolTip(quickKeyItem as UseItem);
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtIntro":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[int(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnShowChat":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[105], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnHideChat":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[106], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtPetIntro":
					var tooltipStr:String;
					if(name[1] <= 12)
					{
						tooltipStr = IntroConst.PetInfoArr[int(name[1])];
					}
					else
					{
						if(int(name[1]) == 16)
						{
							tooltipStr = IntroConst.PetInfoArr[13];
						}
						else
						{ 
							tooltipStr = IntroConst.IntroDic[int(name[1])-13];
						}
					}
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:tooltipStr});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "MarketSaleItem":
					clearInterval(delayNum);
					data = MarketConstData.curPageData[int(name[1])];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "MarketPrivilegeItem":
					clearInterval(delayNum);
					data = null;
					if(MarketConstData.specialDiscount)
					{
						data = MarketConstData.specialDiscount[int(name[1])];
					}
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "MarketShopCarItem":
					clearInterval(delayNum);
					data = MarketConstData.shopCarData[int(name[1])];
					if(!data)
					{
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "depot":
					clearInterval(delayNum);
					data = DepotConstData.goodList[int(name[1])];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "hero":
					clearInterval(delayNum);
					data = RolePropDatas.ItemList[int(name[1])-1];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, true, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "skill":
					var skillData:Object = null;
					if(GameCommonData.Player.Role.SkillList[int(name[1])] != undefined)
					{
						skillData = GameCommonData.Player.Role.SkillList[int(name[1])];
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:skillData, isLearn:true});	
					}
					else
					{
						skillData = GameCommonData.SkillList[int(name[1])];
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:skillData, isLearn:false});
					}
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);					
				break;
				case "lifeSkill":
					var lifeSkillData:Object = null;
					if(GameCommonData.Player.Role.LifeSkillList[int(name[1])] != undefined)
					{
						lifeSkillData = GameCommonData.Player.Role.LifeSkillList[int(name[1])];
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:lifeSkillData, isLearn:true});	
					}
					else
					{
						lifeSkillData = GameCommonData.LifeSkillList[int(name[1])];
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:lifeSkillData, isLearn:false});
					}
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);					
				break;
				case "petPropSkillShow":	//宠物属性面板 技能栏
					var petPropSkillData:Object = null;
					petPropSkillData = PetPropConstData.gridSkillList[int(name[1])];
					if(petPropSkillData) {
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:petPropSkillData, isLearn:true});
					} else {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "fx":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_1" ]});//"点击可以直接传送到目标，消耗一个小飞鞋"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "stall":
					clearInterval(delayNum);
					if(name.length == 1) {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					data = StallConstData.goodList[int(name[1])];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
//					delayNum = setInterval(delayGetToolTip, 100, data, false, 0, StallConstData.stallOwnerName); 
					delayNum = setInterval( delayGetToolTip, 100, data, false, StallConstData.stallOwnerIdDic[ StallConstData.stallIdToQuery ], StallConstData.stallOwnerName ); 
				break;
				case "Strengen":
					clearInterval(delayNum);
					delayNum = setInterval(delayGetToolTip, 100,{id:name[1]} , false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				
				case "EDK":
					clearInterval(delayNum);
					delayNum = setInterval(delayGetToolTip, 100,{id:name[1]} , false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "mcPhoto":
					clearInterval(delayNum);
					if(name.length == 1) {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					data = TradeConstData.goodSelfList[int(name[1])];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
				break;
				case "mcOpPhoto":
					clearInterval(delayNum);
					if(name.length == 1) {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					data = TradeConstData.goodOpList[int(name[1])];
					if(!data)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
//					delayNum = setInterval(delayGetToolTip, 100, data, false, 0, data.userName);
					delayNum = setInterval(delayGetToolTip, 100, data, false, TradeConstData.traderId, data.userName);
				break;
				case "mcItem":
					clearInterval(delayNum);
					var pickMediator:PickMediator = facade.retrieveMediator(PickMediator.NAME) as PickMediator;
					var pickObj:Object = new Object();			
					pickObj.type = pickMediator.itemList[pickMediator.currentPage * pickMediator.preNum + int(name[1])];
					if(!pickObj)
					{ 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					delayNum = setInterval(delayGetToolTip, 100, pickObj); 
				break;				
				case "Equ":
					clearInterval(delayNum);
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					data = dataProxy.equipments[int(name[1])-1];
					if(!data) { 
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					if(IntroConst.ItemInfo[data.id]) {
						data = IntroConst.ItemInfo[data.id];
					}
					delayNum = setInterval(delayGetToolTip, 100, data, false, data.ownerId);
					
//					delayNum = setInterval(delayGetToolTip, 100, data, true);
//					if(GameCommonData.TargetAnimal && GameCommonData.TargetAnimal.Role) {
//						delayNum = setInterval(delayGetToolTip, 100, data, true, GameCommonData.TargetAnimal.Role.Id, GameCommonData.TargetAnimal.Role.Name);
//					} else {
//					}
				break;
				case "NPCShopSale":		//NPC商店出售栏物品
					clearInterval(delayNum);
					var goodShopSaleIndex:uint = uint(name[1]);
					if(NPCShopConstData.goodSaleList[goodShopSaleIndex]) {
						data = BagData.getItemById(NPCShopConstData.goodSaleList[goodShopSaleIndex].id);
						if(!data) {
							facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
							return;
						}
						delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
					}
				break;
				case "NPCBusinessSale":		//跑商商店出售栏物品
					clearInterval(delayNum);
					var goodBusiSaleIndex:uint = uint(name[1]);
					if(NPCBusinessConstData.goodSaleList[goodBusiSaleIndex]) {
						data = BagData.getItemById(NPCBusinessConstData.goodSaleList[goodBusiSaleIndex].id);
						if(!data) {
							facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
							return;
						}
						delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
					}
				break;
				case "petSkillLearnShow":	//宠物技能学习面板 技能栏
					var petSkillLearnData:Object = null;
					petSkillLearnData = PetSkillLearnConstData.skillDataList[int(name[1])];
					if(petSkillLearnData) {
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:petSkillLearnData, isLearn:true});
					} else {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "petSkillUpShow":	//宠物技能升级面板 技能栏
					var petSkillUpData:Object = null;
					petSkillUpData = PetSkillUpConstData.skillDataList[int(name[1])];
					if(petSkillUpData) {
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:petSkillUpData, isLearn:true});
					} else {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "SelfRole":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999991, role:"SelfRole"});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "petRole":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999991, role:"petRole"});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;	
				case "mcExp":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999993});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "Role":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999991, role:"Role", index:name[1]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "map":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.mapInfoArr[int(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "SMALLMAP":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:name[1]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				
				case "AutoPlay":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP,{type:name[1],data:{type:name[1]}});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnSetLeo":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[17], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				
				case "buffIcon":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:name[1], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;	
				
				case "btnSelectColor":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[18], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnCreateChannel":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[19], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnFilter":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[20], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnClear":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[21], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnSetHeight":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[22], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "bthSetMouse":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[23], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnSelectCh":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[24], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnFace":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[25], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "mcCheckBox":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[10], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnSend":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[28], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
//				case "shop":
//					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[63], change:true});
//					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
//				break;
				case "huodong":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[77], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "getBigMap":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[60], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "autoplay":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[61], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "getRank":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[62], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnQuickLanUp":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[59], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnQuickLanDown":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[59], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtSysSet":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.SYS_SET_INTRO_INFO[int(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				//人物火冰玄毒四属性
				case "mcFire":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_2" ]+" " + int(GameCommonData.Player.Role.AttendPro[0])+"\n"+GameCommonData.wordDic[ "med_lost_1" ]+" "+int(GameCommonData.Player.Role.AttendPro[4]),change:true});//"火攻"	"火防"
				break;
				case "mcIce":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_3" ]+" "+ int(GameCommonData.Player.Role.AttendPro[1])+"\n"+GameCommonData.wordDic[ "med_lost_2" ]+" "+int(GameCommonData.Player.Role.AttendPro[5]), change:true});//"冰攻"		"冰防"
				break;
				case "mcLight":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_4" ]+" "+ int(GameCommonData.Player.Role.AttendPro[2])+"\n"+GameCommonData.wordDic[ "med_lost_3" ]+" "+int(GameCommonData.Player.Role.AttendPro[6]), change:true});//"玄攻"	"玄防"
				break;
				case "mcPoison":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_5" ]+" "+ int(GameCommonData.Player.Role.AttendPro[3])+"\n"+GameCommonData.wordDic[ "med_lost_4" ]+" "+int(GameCommonData.Player.Role.AttendPro[7]), change:true});//"毒攻"	"毒防" 
				break;
				////小地图元素
				case "btn":
					switch(name[1])
					{
						case "rank":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[62], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "sceneMap":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[61], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "bigMap":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[71], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "help":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[67], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "pk":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:PkData.dataArr[GameCommonData.Player.Role.PkState].data.type, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "gm":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[69], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "autoPlay":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[68], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "SoundOn":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[72], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "SoundOff":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[72], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "expandTeam":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[74], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;	
						

						/** 经脉按钮提示悬浮框 */
						case "meridians":
							if( name[2] != null && name[2] > 0 && name[2] <= 8)
							{
								var meridiansId:int = name[2] ;
								var str:String = (facade.retrieveMediator( MeridiansMediator.NAME ) as MeridiansMediator).getNextMSG( meridiansId );
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:str});
							}
						break;

					}
				break;
				
				case "mcHelp":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_6" ], change:true});//"使用辅助宝符可以提高成功率"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnQuickAutoPlay":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_7" ], change:true});//"快速挂机"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnSysMessage":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_4" ], change:true});//"系统消息"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnQuickSys":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "mod_too_con_int_int_71" ], change:true});//"系统设置" 
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "fullScreen":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_14" ]/** = "全屏";*/, change:true});//"全屏"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					break;
				case "btnVIP":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:" VIP ", change:true});//"VIP"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					break;
				case "BtnTeam":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_15" ]/** = "组队";*/ + " T", change:true});//"组队"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					break;	
					
				//"全部经脉修炼到X层时，进入下一境界" 
				case "alllMeridiansGrade":
					var nAllLev:int = 5;
					var nAllLevGrade:int = MeridiansData.getLowest( MeridiansData.meridiansVO.nAllLevGrade );
					if(nAllLevGrade > 0)
					{
						if(nAllLevGrade%30 == 1)
						{
							nAllLev = nAllLevGrade + 4;
						}
						else if(nAllLevGrade%30 == 0)
						{
							nAllLev = nAllLevGrade + 1;
						}
						else
						{
							nAllLev = nAllLevGrade + 5;
						}
						if(nAllLev > 60 )
						{
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_1" ]+  //"全部经脉修炼到无上·"
								MeridiansData.numbers[nAllLev%60] +GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_2" ]});//"层时，进入下一境界"
						}
						else if(nAllLev > 30)
						{
							if(nAllLev == 60)
							{
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_3" ]+ //"全部经脉修炼到真·"
									MeridiansData.numbers[30] +GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_2" ]});//"层时，进入下一境界"
							}
							else
							{
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_3" ]+ //"全部经脉修炼到真·"
									MeridiansData.numbers[nAllLev%30] +GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_2" ]});//"层时，进入下一境界"
							}
						}
						else
						{
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_4" ]+ //"全部经脉修炼到"
								MeridiansData.numbers[nAllLev] + GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansGrade_2" ]});//"层时，进入下一境界"
						}
					}
				break;
				//"  全部经脉强化到X时，进入下一灵根" 
				case "alllMeridiansStrength":
					var nAllStrengthLev:int = 4;
					if(MeridiansData.meridiansVO.nAllStrengthLevAdd > 0 && MeridiansData.meridiansVO.nAllStrengthLevAdd < 5)
					{
						switch(MeridiansData.meridiansVO.nAllStrengthLevAdd)
						{
							case 1: nAllStrengthLev = 4; break;
							case 2: nAllStrengthLev = 7; break;
							case 3: nAllStrengthLev = 9; break;
							case 4: nAllStrengthLev = 10;break;
						}
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansStrength_1" ]+ nAllStrengthLev + GameCommonData.wordDic[ "gameui_med_uil_getT_alllMeridiansStrength_2" ]});//"全部经脉强化到"     "时，进入下一灵根"
					}
				break;

				case "autoRoad":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[70], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "Blood":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:BigDrugData.drugDataList[0].name, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "Blue":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:BigDrugData.drugDataList[1].name, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "Pet":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:BigDrugData.drugDataList[2].name, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				
//				case "btnReduce":
//				case "btnExtend":
//					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[64], change:true});
//					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
//				break;
				case "mcRole":
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(int(name[1]) == 0)
					{
						if(int(GameCommonData.Player.Role.MainJob.Job) > 4000)
						{
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[26]});
						}	
						else
						{
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_8" ]});//"主职业"
						}
					}
					else
					{
						if(int(GameCommonData.Player.Role.ViceJob.Job) != 0)
						{
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_9" ]});//"副职业"
						}
						else
						{
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[27]});
						}						
					}
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "mcNPCGoodPhoto":
					clearInterval(delayNum);
					var typeNPCGood:int = int(name[1]); 
					delayNum = setInterval(delayGetNPCShopToolTip, 100, {type:typeNPCGood});
				break;
				case "npcGoodToGood":		//NPC商店 物品换物品
					UIConstData.ToolTipShow = true;
					clearInterval(delayNum);
					delayNum = setInterval(delayGetToolTip, 100, {type:int(name[1])});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					break;
				break;
				case "TaskEqu":
					clearInterval(delayNum);
					var type:int = int(name[1]);
					delayNum = setInterval(delayGetToolTip, 100, {type:type});
				break; 
				case "txtMarketIntro":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.moneyIntro[int(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtMarketIntro1":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.moneyIntro[int(name[1])]});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "skillArrow":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[73], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;	
				case "onLineTimeAward":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[75], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnRight":
					switch(name[1])
					{
						case "0":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[76], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "1":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[77], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "2":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_10" ], change:true});//"生产"
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break; 
						case "3":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[79], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "4":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[78], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "5":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_11" ], change:true});//"万兽谱"
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "6":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[103], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "7":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[80], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "8":
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "expansion_master_master" ], change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
							
					}
				break;
				case "txtPower":		//活力 
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[81], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtEnergy":		//精力
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[82], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
//				case "heart":			//心情
//					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[83], change:true});
//					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
//				break;
				case "unityAtt":		//帮派贡献度
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[84], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "impart":		//传道解惑值
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[85], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "mainSchool":		//主门派贡献
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[86], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "viceSchool":		//副门派贡献
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[87], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "amok":		//杀气值
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[83], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				
				case "redName":		//红名时间
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[104], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				
				case "vipLevel":		//VIP等级
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[88], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "bagLevel":		//背包等级
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[89], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "ally":		//武林同盟
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[90], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "doubleTime":		//冻结双倍时间
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[91], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "stuffBagLevel":		//材料包等级
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[92], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtPhyAttack":		//外攻
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[93], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtMagicAttack":		//内攻
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[94], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtPhyDef":		//外防
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[95], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtMagicDef":		//内防
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[96], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtHit":		//命中
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[98], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtHide":		//闪避
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[99], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtCrit":		//暴击
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[100], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtToughness":		//坚韧
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[101], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtSp":		//怒气
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.IntroDic[102], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtCurTime":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_12" ]+"："+GameCommonData.netDelayTime.toString()+"ms", change:true});//网络延迟
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnPreventWallow":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "gUI_med_uil_getT_13" ], change:true});//"防沉迷验证"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "reNameBtn":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:GameCommonData.wordDic[ "mod_ren_med_ren_onc_1" ], change:true});//"角色改名"
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "mcUnityLevel":		//帮派主堂升级
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.levelUpData, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "btnUnityLevel":		//帮派分堂升级
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.levelUpData, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtUnityBooming":		//繁荣度
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoBooming, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtUnityBuilt":		//建设度
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoBuilt, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "mcUnityBind":			//帮派资金
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoMoney, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtMenberNum":		//成员数量
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoMenber, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtCraftsmanNum":		//建筑工匠
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoCraftsman, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtMasterNum":		//武学大师
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoMasterNum, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtBusinessmanNum":	//贸易商人
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoBusinessman, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "bulitBar":			//建设度进度条
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoBuiltBar, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "boomingBar":			//繁荣度进度条
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoBoomingBar, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtSkillLevel":		//分堂技能等级
					switch(name[1])
					{
						case "0":
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSkill_1, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "1":
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSkill_2, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "2":
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSkill_3, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
					}
				break;
				case "mcUnitySyn":				//分堂悬浮框
					switch(name[1])
					{
						case "1":		//青龙
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSynQ, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "2":		//白虎
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSynB, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "3":		//玄武
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSynX, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
						case "4":		//朱雀
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSynZ, change:true});
							uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						break;
					}
				break;
				case "txtUnityState":			//帮派停止维护
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoUnityState, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "txtSubState":				//分堂关闭
					GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:UnityConstData.infoSubState, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "soul":			//武魂的技能
					if(name[1] == "skill")
					{
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:getSoulSkillStr(name[2]), change:true});
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
						
						if ( SoulMediator.soulVO.soulSkills[ name[2] ] )
						{
							if((SoulMediator.soulVO.soulSkills[name[2]]  as SoulSkillVO).state == 0)
							{
								GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWITEMTOOLTIP, {type:33338888, data:SoulMediator.soulVO.soulSkills[ name[2] ], change:true}); 
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							}
						}
					}
					/* else if(name[1] == "ext")
					{
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:getSoulExtStr(name[2]), change:true});
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					} */
					else if(name[1] == "style")
					{
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[0], change:true});
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					}
					else if(name[1] == "notUse" || name[1] == "hasLearn" || name[1] == "canLearn" || name[1] == "canUseToLearn" || name[1] == "upProperty")
					{
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:getSoulExtStr(name[2]), change:true});
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					}
					else if(name[1] == "txt")
					{
						switch(name[2])
						{
							case "style":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[0], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "belong":
								if(RolePropDatas.ItemList[15])
								{
									var belongStr:String = "";
									if(SoulMediator.soulVO.belong == 1)
									{
										belongStr = SoulData.soulToolTipInfo[17];
									}
									else if(SoulMediator.soulVO.belong == 2)
									{
										belongStr = SoulData.soulToolTipInfo[18];
									}
									
									facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:belongStr, change:true});
									uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
								}
							break;	
							case "life":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[15], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "level":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[16], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "exp":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[2], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro1":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro2":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro3":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro4":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro5":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro6":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "pro7":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[5], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "grow":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[3], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
							case "compose":
								facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:SoulData.soulToolTipInfo[4], change:true});
								uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
							break;	
						}
					}
						
				break;
				case "masterHintTxt":
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:MasterData.MASTER_TOOLTIP_HINT[ int(name[1]) ], change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				case "musicPlayerPlaylistItemRenderer":
					clearInterval(delayNum);
					var mTipText:Array = IntroConst.MUSICPLAYER_TOOLTIP_STRING.split("$");
					var l:int = int(event.target.data.length);
					var m:String = int(l / 60).toString();
					var s:String = int(l % 60).toString();
					if (m.length == 1) m = "0" + m;
					if (s.length == 1) s = "0" + s;
					var mTipStr:String = mTipText[0] + event.target.data.name  // 歌名
													+ mTipText[1] + m + ":" + s  // 时间
													+ ((event.target.data.sceneID == -1 ) ? "" : (mTipText[2] + SmallConstData.getInstance().mapItemDic[event.target.data.sceneID.split(",")[0]].name));  // 场景
//					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999995, data: mTipStr, anchor:event.target});
					delayNum = setInterval(delayGetToolTip, 1000, {type:999995, data: mTipStr, anchor:event.target});
				break;
				case "musicPlayerBtn":
					clearInterval(delayNum);
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999992, data:IntroConst.MUSICPLAYER_NAME, change:true});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				break;
				/** 补偿仓库 */
				case "CompensateStorageItem":
					clearInterval(delayNum);
					var type1:int = int(name[1]) + 10000000;
					delayNum = setInterval(delayGetToolTip, 100, {type:type1});
					break;
				default:
					clearInterval(delayNum);
					uiLayer.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
					UIConstData.ToolTipShow = false;	//ToolTip开关，防止服务器数据返回时鼠标已经不在图标上了。
				break;
			}
			if(String(name[0]).indexOf("goodQuickBuy") == 0) {		//快速购买ToolTip
				UIConstData.ToolTipShow = true;
				clearInterval(delayNum);
				var GoodQuicktype:int = int(name[1]);
				delayNum = setInterval(delayGetToolTip, 100, {type:GoodQuicktype});
				uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			}
		}
		
		/**
		 *获取魂魄技能ToolTip 
		 * @param num
		 * @return 
		 * 
		 */		
		private function getSoulSkillStr(num:int):String
		{
			if(num == 10)
			{
				return SoulData.soulToolTipInfo[13];
			}
			var skillStr:String = "";
			if(RolePropDatas.ItemList[15])	//装备了魂魄
			{
				var sVo:Object = SoulMediator.soulVO.soulSkills[num];
				if(sVo == false)
				{
					skillStr =  SoulData.soulToolTipInfo[12];
				}
				else if(sVo is SoulSkillVO)
				{
					if((sVo as SoulSkillVO).state == 0)
					{
						skillStr = SoulData.soulToolTipInfo[11];
					}
					else if((sVo as SoulSkillVO).state == 1)
					{
						skillStr = SoulData.soulToolTipInfo[12];
					}
				}
			}
			else
			{
				skillStr =  SoulData.soulToolTipInfo[12];
			}
			return skillStr;
		}
		/**
		 * 获取魂魄扩展属性ToolTip 
		 * @param useItem
		 * 
		 */		
		private function getSoulExtStr(numStr:String):String
		{
			var tag:int = int(numStr.substr(numStr.length - 1));
			var extStr:String = "";
			var extVo:Object = SoulMediator.soulVO.extProperties[tag];
			if(extVo == false)
			{
				extStr = SoulData.soulToolTipInfo[9];
			}
			else if(extVo is SoulExtPropertyVO)
			{
				if((extVo as SoulExtPropertyVO).state == 0)
				{
					extStr = SoulData.soulToolTipInfo[6];
				} 
				else if((extVo as SoulExtPropertyVO).state == 1)
				{
					extStr = SoulData.soulToolTipInfo[7];
				} 
				else if((extVo as SoulExtPropertyVO).state == 2)
				{
					extStr = SoulData.soulToolTipInfo[8];
				}
			}
			return extStr;
		}
		
		private function showQuickBarToolTip(useItem:UseItem):void
		{
			if(useItem.Type < 100000){				//技能
				if(useItem.Type >= 7000 && useItem.Type < 9000) {			//宠物技能
					if(!GameCommonData.Player.Role.UsingPet) {
						return;
					}
					var petSkillId:uint = useItem.Type;
					var petSkillDataKey:Object = null;
					var petSkillList:Array = GameCommonData.Player.Role.UsingPet.SkillLevel;
					for(var i:int = 0; i < petSkillList.length; i++) {
						if(!petSkillList[i]) continue;
						if((petSkillList[i] as GameSkillLevel).gameSkill.SkillID == petSkillId) {
							petSkillDataKey = petSkillList[i];
							break; 
						}
					}
					if(petSkillDataKey) {
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:petSkillDataKey, isLearn:true});
					} else {
						facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
						return;
					}
				} else {												//人物技能
					if(useItem.Type > 6000 && useItem.Type < 7000) {			//生活技能
						var roleSkillData_life:Object = GameCommonData.Player.Role.LifeSkillList[useItem.Type];
						var isLearn_life:Boolean = true;
						if(!roleSkillData_life) {
							roleSkillData_life = GameCommonData.LifeSkillList[useItem.Type];
							isLearn_life = false;
						}
						if(roleSkillData_life) {
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:roleSkillData_life, isLearn:isLearn_life});
						} else {
							facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
							return;
						}
					} else {																				//职业技能
						var roleSkillData:Object = GameCommonData.Player.Role.SkillList[useItem.Type];
						var isLearn:Boolean = true;
						if(!roleSkillData) {
							roleSkillData = GameCommonData.SkillList[useItem.Type];
							isLearn = false;
						}
						if(roleSkillData) {
							facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:999994, data:roleSkillData, isLearn:isLearn});
						} else {
							facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
							return;
						}
					}
				}
			} else {							//其他物品
				var data:Object = new Object();
				for(var n:int = 0; n < BagData.AllItems.length; n++) {
					var find:Boolean = false;
					for(var m:int = 0; m < BagData.AllItems[n].length; m++) {
						if(BagData.AllItems[n][m] == undefined) continue;
						if(BagData.AllItems[n][m].type == useItem.Type && BagData.AllItems[n][m].isBind==useItem.IsBind) {
							data = BagData.AllItems[n][m];
							delayNum = setInterval(delayGetToolTip, 100, data, false, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
							return;
						}
					}
				}
			}
		}
		
		private function delayGetNPCShopToolTip(data:Object):void
		{
			clearInterval(delayNum);
			if(dataProxy.NPCBusinessIsOpen) {	//跑商商店打开
				for(var i:int = 0; i < NPCBusinessConstData.goodList.length; i++) {
					if(data.type == NPCBusinessConstData.goodList[i].type) {
						data = NPCBusinessConstData.goodList[i];
						break;
					}
				}
				sendNotification(EventList.SHOW_NPC_SHOP_TOOLTIP, {type:data.type, data:data});
			} else {	//NPC商店
				sendNotification(EventList.SHOW_NPC_SHOP_TOOLTIP, {type:data.type, data:data});
			}
			uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
		}
		
		private function delayGetToolTip(obj:Object, isEquied:Boolean = false, playerID:int = 0, playerName:String=""):void
		{
			clearInterval(delayNum);
			isEquip = isEquied;
			if(obj.id == undefined)
			{
				facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:obj.type, data:obj});
				uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			}
			else
			{
				if(obj.type > 300000 && obj.type < 400000 && obj.type != 351001) //351001不是元宝票
				{
					facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:obj.type, data:obj});
					uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				}
				else
				{
					if( obj.type>=250000 && obj.type<300000 )
					{
						if ( !SoulData.SoulDetailInfos[obj.id] )
						{
//							SoulProxy.getSoulDetailInfoFromBag(obj.id); 
							SoulProxy.getPeopleSoulDetail( obj.id, playerID );
							UiNetAction.GetItemInfo(obj.id, playerID, playerName);
							tmpObj = obj;
							return;
						}
					}
					if(IntroConst.ItemInfo[obj.id])   //!= undefined;
					{
						facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:IntroConst.ItemInfo[obj.id].type, isEquip:isEquip, data:IntroConst.ItemInfo[obj.id]});
						uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
					}
					else
					{
						UiNetAction.GetItemInfo(obj.id, playerID, playerName);
						tmpObj = obj;
					}
				}
			}
		}
		
		private function getInfoComplete(data:Object):void
		{
			if(tmpObj==null || !tmpObj.id || tmpObj.id != data.id) return;
			facade.sendNotification(EventList.SHOWITEMTOOLTIP, {type:data.type, isEquip:isEquip, data:data});
			uiLayer.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
		}
	
		private function onMouseMove(event:MouseEvent):void
		{
			facade.sendNotification(EventList.MOVEITEMTOOLTIP);
		}
			
		private function showOnly(flag:String):void
		{
			if(dataProxy.BigMapIsOpen)		sendNotification(EventList.CLOSEBIGMAP);
			switch(flag)
			{
				case "hero":
					if(dataProxy.SkillIsOpen)
					{
						facade.sendNotification(EventList.CLOSESKILLVIEW);
					}
					if(dataProxy.PetIsOpen)
					{
						facade.sendNotification(EventList.CLOSEPETVIEW);
					}
					if(dataProxy.TaskIsOpen)
					{
						facade.sendNotification(EventList.CLOSETASKVIEW);
					} 					
				break;
				case "pet":
					if(dataProxy.SkillIsOpen)
					{
						facade.sendNotification(EventList.CLOSESKILLVIEW);
					}
					if(dataProxy.HeroPropIsOpen)
					{
						facade.sendNotification(EventList.CLOSEHEROPROP);
					}
					if(dataProxy.TaskIsOpen)
					{
						facade.sendNotification(EventList.CLOSETASKVIEW);
					} 
				break;
				case "skill":
					if(dataProxy.HeroPropIsOpen)
					{
						facade.sendNotification(EventList.CLOSEHEROPROP);
					}
					if(dataProxy.PetIsOpen)
					{
						facade.sendNotification(EventList.CLOSEPETVIEW);
					}
					if(dataProxy.TaskIsOpen)
					{
						facade.sendNotification(EventList.CLOSETASKVIEW);
					} 
				break;
				case "task":
					if(dataProxy.SkillIsOpen)
					{
						facade.sendNotification(EventList.CLOSESKILLVIEW);
					}
					if(dataProxy.PetIsOpen)
					{
						facade.sendNotification(EventList.CLOSEPETVIEW);
					}
					if(dataProxy.HeroPropIsOpen)
					{
						facade.sendNotification(EventList.CLOSEHEROPROP);
					} 
				break;
				
				default:
					if(dataProxy.SkillIsOpen)
					{
						facade.sendNotification(EventList.CLOSESKILLVIEW);
					}
					if(dataProxy.PetIsOpen)
					{
						facade.sendNotification(EventList.CLOSEPETVIEW);
					}
					if(dataProxy.HeroPropIsOpen)
					{
						facade.sendNotification(EventList.CLOSEHEROPROP);
					} 
					if(dataProxy.TaskIsOpen)
					{
						facade.sendNotification(EventList.CLOSETASKVIEW);
					}
					break;
			}
		}
		
		/** 显示 背包，帮派，组队，排行榜，大地图，好友 */
		private function showOnlyFive(flag:String):void
		{
			switch(flag) {
				case "bag":			//打开背包
					if(dataProxy.RankIsOpen)      	sendNotification(EventList.CLOSERANKVIEW);
					if(dataProxy.UnityIsOpen)     	sendNotification(EventList.CLOSEUNITYVIEW);
					if(dataProxy.UnitInfoIsOpen)  	sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
					if(dataProxy.TeamIsOpen) 		sendNotification(EventList.REMOVETEAM);
					if(dataProxy.BigMapIsOpen)		sendNotification(EventList.CLOSEBIGMAP);
//					if(dataProxy.FriendsIsOpen)		sendNotification(FriendCommandList.HIDEFRIEND);
					break;
				case "unity":		//打开帮派
					if(dataProxy.RankIsOpen)      	sendNotification(EventList.CLOSERANKVIEW);
					if(dataProxy.TeamIsOpen) 		sendNotification(EventList.REMOVETEAM);
					if(dataProxy.BigMapIsOpen)		sendNotification(EventList.CLOSEBIGMAP);
					if(dataProxy.BagIsOpen)			{sendNotification(EventList.CLOSEBAG); dataProxy.BagIsOpen = false;};
//					if(dataProxy.FriendsIsOpen)		sendNotification(FriendCommandList.HIDEFRIEND);
					break;
				case "team":		//打开组队
					if(dataProxy.RankIsOpen)      	sendNotification(EventList.CLOSERANKVIEW);
					if(dataProxy.UnityIsOpen)     	sendNotification(EventList.CLOSEUNITYVIEW);
					if(dataProxy.UnitInfoIsOpen)  	sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
					if(dataProxy.BigMapIsOpen)		sendNotification(EventList.CLOSEBIGMAP);
					if(dataProxy.BagIsOpen)			{sendNotification(EventList.CLOSEBAG); dataProxy.BagIsOpen = false;};
//					if(dataProxy.FriendsIsOpen)		sendNotification(FriendCommandList.HIDEFRIEND);
					break;
				case "rank":		//打开排行榜
					if(dataProxy.UnityIsOpen)     	sendNotification(EventList.CLOSEUNITYVIEW);
					if(dataProxy.UnitInfoIsOpen)  	sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
					if(dataProxy.TeamIsOpen) 		sendNotification(EventList.REMOVETEAM);
					if(dataProxy.BigMapIsOpen)		sendNotification(EventList.CLOSEBIGMAP);
					if(dataProxy.BagIsOpen)			{sendNotification(EventList.CLOSEBAG); dataProxy.BagIsOpen = false;};
//					if(dataProxy.FriendsIsOpen)		sendNotification(FriendCommandList.HIDEFRIEND);
					break;
				case "bigMap":		//打开大地图
					if(dataProxy.RankIsOpen)      	sendNotification(EventList.CLOSERANKVIEW);
					if(dataProxy.UnityIsOpen)     	sendNotification(EventList.CLOSEUNITYVIEW);
					if(dataProxy.UnitInfoIsOpen)  	sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
					if(dataProxy.TeamIsOpen) 		sendNotification(EventList.REMOVETEAM);
					if(dataProxy.BagIsOpen)			{sendNotification(EventList.CLOSEBAG); dataProxy.BagIsOpen = false;};
//					if(dataProxy.FriendsIsOpen)		sendNotification(FriendCommandList.HIDEFRIEND);
					break;
//				case "friend":		//打开好友
//					if(dataProxy.RankIsOpen)      	sendNotification(EventList.CLOSERANKVIEW);
//					if(dataProxy.UnityIsOpen)     	sendNotification(EventList.CLOSEUNITYVIEW);
//					if(dataProxy.UnitInfoIsOpen)  	sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
//					if(dataProxy.TeamIsOpen) 		sendNotification(EventList.REMOVETEAM);
//					if(dataProxy.BigMapIsOpen)		sendNotification(EventList.CLOSEBIGMAP);
//					if(dataProxy.BagIsOpen)			{sendNotification(EventList.CLOSEBAG); dataProxy.BagIsOpen = false;};
//					break; 
			}
		}
		
	}
}