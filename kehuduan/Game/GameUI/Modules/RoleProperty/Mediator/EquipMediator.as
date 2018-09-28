package GameUI.Modules.RoleProperty.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Mediator.UI.PlayerAttribute;
	import GameUI.Modules.RoleProperty.Net.NetAction;
	import GameUI.Modules.RoleProperty.Prxoy.ItemManager;
	import GameUI.Modules.RoleProperty.Prxoy.ItemUnit;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.EquipItem;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.PlayerActionSend;
	
	import OopsEngine.Role.RoleJob;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class EquipMediator extends Mediator
	{
		public static const NAME:String = "EquipMediator";
		public static const TYPE:int = 0;
		private var itemManager:ItemManager;
		private var isGet:Boolean = false;
		private var parentView:MovieClip = null;
		private var curJob:RoleJob = null;
		public var playerAttribute:PlayerAttribute = null;
		
		private var lastRingPos:int = 0;	//上次装备的戒指位置
		private var lastJadePos:int = 0;	//上次装备的饰品位置
		
		private var tmpItem:Object = null;	//穿装备时临时数据
		
		private var startTime:Number;		//切换显示时装的上次时间
		private const HANDLERTIME:int = 10;	//显示换装的间隔，10秒
		
		public function EquipMediator(parent:MovieClip)
		{
			parentView = parent;
			super(NAME);
		}
		
		private function get equip():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				RoleEvents.INITROLEVIEW,
				RoleEvents.SHOWPROPELEMENT,
				RoleEvents.GETOUTFIT,
				EventList.CLOSEHEROPROP,
				EventList.GOHEROVIEW,
				RoleEvents.UPDATEOUTFIT,
				RoleEvents.GETOUTFITBYCLICK,
				RoleEvents.GETFITOUTBYBAG,
				EventList.UPDATEATTRIBUATT,
				RoleEvents.UPDATEADDATT,
				RoleEvents.ATTENDPROPELEMENT,
				RoleEvents.PLAYER_CHANGE_JOB,
				RoleEvents.ISSHOWDRESS
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case RoleEvents.INITROLEVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.EQUIPPANE});
					this.equip.mouseEnabled=false;
					itemManager = new ItemManager();
					facade.registerProxy(itemManager);
					playerAttribute = new PlayerAttribute(equip);
					initView();
				break;
				case RoleEvents.GETFITOUTBYBAG:
					if(!isGet)
					{
//						NetAction.RequestOutfit();
						
						if(GameCommonData.Player.Role.ViceJob.Job != 0)
						{
							var cur:uint=GameCommonData.Player.Role.CurrentJob==1 ? 2 : 1;
							NetAction.GetRoleInfo(cur);
						}
						isGet = true;
					}
				break;
				case RoleEvents.SHOWPROPELEMENT:
					if(notification.getBody() as int != TYPE) return;					
					parentView.addChildAt(equip, 4);
					if(!isGet)
					{
						NetAction.RequestOutfit();
						isGet = true;	
					} else {
						initItem();	
						playerAttribute.ShowPropData();		
					}
					facade.sendNotification(RoleEvents.ISSHOWDRESS , GameCommonData.Player.Role.IsShowDress);
				break;
				case RoleEvents.GETOUTFIT:
					initItem();
					playerAttribute.ShowPropData();		
				break;
				case RoleEvents.UPDATEOUTFIT:
					initItem();
				break;
				case EventList.GOHEROVIEW:
					getoutFit(notification.getBody());
				break;
				case RoleEvents.GETOUTFITBYCLICK:
					getOutFitByClick(notification.getBody());
				break;
				case EventList.UPDATEATTRIBUATT:
					playerAttribute.UpDateAttribute(notification.getBody());
				case RoleEvents.UPDATEADDATT:
					playerAttribute.UpDateExtendAttribute(notification.getBody());
				break;
				case RoleEvents.ATTENDPROPELEMENT:		//人物追加的冰火玄毒属性
					var attendArray:Array = notification.getBody() as Array;
					for(var i:int =0; i < 8 ; i++)
					{
						if(i == attendArray[0]) GameCommonData.Player.Role.AttendPro[i] = attendArray[1] as int;
					}
				break;
				
				case RoleEvents.PLAYER_CHANGE_JOB:
					initItem();	
					playerAttribute.ShowPropData();
				break;		
				case RoleEvents.ISSHOWDRESS:			//是否显示时装
					if(notification.getBody())
					{
						GameCommonData.Player.Role.IsShowDress = true;
						if(equip) 
						{
						 equip.mcCheckBox.gotoAndStop(2);
							
						}
					}
					else
					{
						GameCommonData.Player.Role.IsShowDress = false;
						if(equip) equip.mcCheckBox.gotoAndStop(1);
					}
				break;
			}
			
		}
		
		private function initView():void
		{
			for(var i:int = 1; i<=16; i++)
			{
				var itemUnit:ItemUnit = new ItemUnit();
				if(i != 16)
				{
					itemUnit.Grid = equip["hero_"+i];
				}
				else
				{
					itemUnit.Grid = (facade.retrieveMediator(SoulMediator.NAME) as SoulMediator).soulView["hero_"+i];
				}
				itemUnit.Item = null;
				itemUnit.Index  = i-1;
				itemUnit.IsUsed = false;
				RolePropDatas.ItemUnitList.push(itemUnit);
			}
			equip.mcCheckBox.addEventListener(MouseEvent.CLICK, showDress);
			itemManager.Initialize();
		}
				
		/**
		 * 初始化装备图标或更新装备图标 
		 * 
		 */					
		private function initItem():void
		{
			removeAllItem();
			if(RolePropDatas.ItemUnitList.length == 0) return;
			for(var i:int = 0; i<RolePropDatas.ItemList.length; i++)
			{
				if(RolePropDatas.ItemList[i] == undefined) continue; 
				var useItem:EquipItem;
//				var useItem:UseItem = new UseItem(RolePropDatas.ItemList[i].position - 1, RolePropDatas.ItemList[i].type, equip);
				if(i != RolePropDatas.ItemList.length -1)
				{
					
					useItem = new EquipItem(RolePropDatas.ItemList[i].position - 1, RolePropDatas.ItemList[i].type, equip, RolePropDatas.ItemList[i].color);
				}
				else
				{
					var parentDis:DisplayObjectContainer = (facade.retrieveMediator(SoulMediator.NAME) as SoulMediator).soulView;
					useItem = new EquipItem(RolePropDatas.ItemList[i].position - 1, RolePropDatas.ItemList[i].type, parentDis, RolePropDatas.ItemList[i].color);
					useItem.name = "soulHer_16";
				}
				useItem.x = RolePropDatas.ItemUnitList[i].Grid.x + 2;
				useItem.y = RolePropDatas.ItemUnitList[i].Grid.y + 2;
				useItem.Id = RolePropDatas.ItemList[i].id;
				useItem.IsBind = RolePropDatas.ItemList[i].isBind;
				useItem.Type = RolePropDatas.ItemList[i].type;
				useItem.Pos = i;
				RolePropDatas.ItemUnitList[RolePropDatas.ItemList[i].position - 1].Item = useItem;
				RolePropDatas.ItemUnitList[RolePropDatas.ItemList[i].position - 1].IsUsed = true;
				var obj:Object=UIConstData.getItem(useItem.Type); 
				var objInfo:Object=IntroConst.ItemInfo[useItem.Id];
				//职业不符漂红
				useItem.setNoFitJobShape(false);
				if(GameCommonData.Player.Role.MainJob.Job!=4096){
					if(GameCommonData.Player.Role.CurrentJob==1){
						if(obj!=null && obj.Job!=0 && obj.Job!=GameCommonData.Player.Role.MainJob.Job){
							useItem.setNoFitJobShape(true);
						}
					}else if(GameCommonData.Player.Role.CurrentJob==2){
						if(obj!=null && obj.Job!=0 && obj.Job!=GameCommonData.Player.Role.ViceJob.Job){
							useItem.setNoFitJobShape(true);
						}
					}	
				}
				//耐久不够漂红
				if(objInfo!=null){
					//耐久不够漂红
					if(objInfo.type != 250000)
					{
						if(objInfo.amount==0){
							useItem.setNoFitJobShape(true);
						}
					}
					//过期也漂红
					if(objInfo.isActive==2){
						useItem.setNoFitJobShape(true);
					}
				}
				useItem.IsLock = false;
				if(i != RolePropDatas.ItemList.length - 1)
				{
					equip.addChild(useItem); 
				}
				else
				{
					(facade.retrieveMediator(SoulMediator.NAME) as SoulMediator).soulView.addChild(useItem);
				}
			}
		}
		
		/** 确定装备上物品 */
		private function dressOn():void
		{
			if(tmpItem) {
				//点击背包中魂魄时，打开魂魄界面，并装备
				if(int(BagData.SelectedItem.Item.Type/10) == SoulData.soulType/10)
				{
					var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.HeroPropIsOpen)
					{
						dataProxy.HeroPropIsOpen = true;
						facade.sendNotification(EventList.SHOWHEROPROP);
						sendNotification(EventList.OPEN_PANEL_TOGETHER);
					}
					if(RolePropDatas.CurView != SoulMediator.TYPE)
					{
						(facade.retrieveMediator(RoleMediator.NAME) as RoleMediator).heroProp["prop_2"].dispatchEvent(new MouseEvent(MouseEvent.CLICK));
					}
				}
				
				BagData.lockBagGridUnit(false);
				BagData.lockBtnCleanAndPage(false);
				if(RolePropDatas.ItemPos[tmpItem.tmpIndex-1] == 21) {
					lastRingPos = tmpItem.tmpIndex-1;
				} else if(RolePropDatas.ItemPos[tmpItem.tmpIndex-1] == 22) {
					lastJadePos = tmpItem.tmpIndex-1;
				}
				NetAction.UseItem(OperateItem.USE, 1, tmpItem.tmpIndex, tmpItem.source.Id);
				tmpItem = null;
			}
		}
		
		/** 不装备物品 */
		private function cancelDress():void
		{
			if(tmpItem) {
				sendNotification(EventList.BAGITEMUNLOCK, tmpItem.source.Id);
				tmpItem = null;
			}
		}
		
		private function getoutFit(item:Object):void
		{
			var pos:int = int(item.source.Type/10000);
			var index:int = item.index;
			if(RolePropDatas.ItemPos[index-1] == pos)
			{
				if(UIConstData.getItem(item.source.Type).Sex != GameCommonData.Player.Role.Sex + 1 && UIConstData.getItem(item.source.Type).Sex != 0)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_em_1" ], color:0xffff00});   //"性别不符，不能使用"
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = false; 	
					BagData.SelectedItem = null;
					facade.sendNotification(BagEvents.SHOWBTN, false);			
					return;
				}
				var o:Object = {}; 
				o.id = item.source.Id;
				o.type = item.source.Type;
				o.isBind = item.source.IsBind;
				if(IntroConst.ItemInfo[item.Id])
					o = IntroConst.ItemInfo[item.Id];
				if(UIUtils.getBindShow(o) == GameCommonData.wordDic[ "mod_rp_med_em_2" ]) {		//装备后绑定的道具先提示    //"装备后绑定"
					tmpItem = item;
					tmpItem.tmpIndex = index;
					facade.sendNotification(EventList.SHOWALERT, {comfrim:dressOn, cancel:cancelDress, extendsFn:cancelDress, info:GameCommonData.wordDic[ "mod_rp_med_em_3" ], title:GameCommonData.wordDic[ "often_used_warning" ]});  //"此物品装备后会绑定，确定要装备上吗？"  //"警 告"
				} else {										//直接装备
					BagData.lockBagGridUnit(false);
					BagData.lockBtnCleanAndPage(false);
					NetAction.UseItem(OperateItem.USE, 1, index, item.source.Id);
					if(NewerHelpData.newerHelpIsOpen) noticeNewerHelp(item.source.Type);	//通知新手指导系统
				}
			} else {
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = false; 	
				item.source.IsLock = false;
			}
		}
		
		private function getOutFitByClick(item:Object):void
		{
			var a:Array = RolePropDatas.ItemPos;
			var type:int = int(item.Type/10000);
			for(var i:int = 0; i<RolePropDatas.ItemPos.length; i++)
			{
				if(RolePropDatas.ItemPos[i] == type)
				{
					if(RolePropDatas.ItemPos[i] == 21)
					{
						if(i == 10 && RolePropDatas.ItemList[i] == undefined) {
							lastRingPos = 11;
						} else if(i == 10 && RolePropDatas.ItemList[11] == undefined) {
							continue;
						} else if(i == 11 && RolePropDatas.ItemList[i] == undefined) {
							lastRingPos = 10;
						}
						if(i == lastRingPos) {
							continue;
						}
					}
					if(RolePropDatas.ItemPos[i] == 22)
					{
						if(i == 12 && RolePropDatas.ItemList[i] == undefined) {
							lastJadePos = 13;
						} else if(i == 12 && RolePropDatas.ItemList[13] == undefined) {
							continue;
						} else if(i == 13 && RolePropDatas.ItemList[i] == undefined) {
							lastJadePos = 12;
						}
						if(i == lastJadePos) {
							continue;
						}
					}
					if(UIConstData.getItem(item.Type).Sex != GameCommonData.Player.Role.Sex + 1 && UIConstData.getItem(item.Type).Sex != 0)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_em_1" ], color:0xffff00});   //"性别不符，不能使用"
						BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = false; 	
						item.IsLock = false;
						BagData.SelectedItem = null;
						facade.sendNotification(BagEvents.SHOWBTN, false);			
						return;
					}
					var o:Object = {}; 
					o.id = item.Id;
					o.type = item.Type;
					o.isBind = item.IsBind;
					if(IntroConst.ItemInfo[item.Id])
						o = IntroConst.ItemInfo[item.Id];
					if(UIUtils.getBindShow(o) == GameCommonData.wordDic[ "mod_rp_med_em_2" ]) {		//装备后绑定的道具先提示     //"装备后绑定"
						tmpItem = {};
						tmpItem.source = item;
						tmpItem.tmpIndex = i+1;
						facade.sendNotification(EventList.SHOWALERT, {comfrim:dressOn, cancel:cancelDress, extendsFn:cancelDress, info:GameCommonData.wordDic[ "mod_rp_med_em_3" ], title:GameCommonData.wordDic[ "often_used_warning" ]});  //"此物品装备后会绑定，确定要装备上吗？"   "警 告"  
					} else {				
						//点击背包中魂魄时，打开魂魄界面，并装备
						if(int(BagData.SelectedItem.Item.Type/10) == SoulData.soulType/10)
						{
							var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
							if(!dataProxy.HeroPropIsOpen)
							{
								dataProxy.HeroPropIsOpen = true;
								facade.sendNotification(EventList.SHOWHEROPROP);
								sendNotification(EventList.OPEN_PANEL_TOGETHER);
							}
							if(RolePropDatas.CurView != SoulMediator.TYPE)
							{
								(facade.retrieveMediator(RoleMediator.NAME) as RoleMediator).heroProp["prop_2"].dispatchEvent(new MouseEvent(MouseEvent.CLICK));
							}
						}						//直接装备
						BagData.lockBagGridUnit(false);
						BagData.lockBtnCleanAndPage(false); 
						if(RolePropDatas.ItemPos[i] == 21) {
							lastRingPos = i;
						} else if(RolePropDatas.ItemPos[i] == 22) {
							lastJadePos = i;
						}
						NetAction.UseItem(OperateItem.USE, 1, i+1, item.Id);
						BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = false; 	
						item.IsLock = false;
						if(NewerHelpData.newerHelpIsOpen) noticeNewerHelp(item.Type);	//通知新手指导系统
					}
					break;
				}
			}
		}
		
		/** 通知新手指导系统 */
		private function noticeNewerHelp(type:uint):void
		{
			if(type == 142000) {	//侠义剑
				sendNotification(NewerHelpEvent.DRESSON_NOTICE_NEWER_HELP, 1);	//sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 1);
				return;
			}
			if(type == 120000) {	//侠义袍
				sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 3);
				return;
			}
			if(type == 130016) {	//侠义冠
				sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 5);
				return;
			}
			if(type == 144000) {	//湛卢剑
				sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 8);
				return;
			}
			var pos:int = NewerHelpData.getTypePos(type);
			if(pos >= 0) {			//穿套装
				sendNotification(NewerHelpEvent.LOOP_DELETE_GRID_NOTICE_NEWER_HELP, {type:11, pos:pos});
				return;
			}
			pos = NewerHelpData.getFirstBagPos(type);
			if(pos >= 0) {
				sendNotification(NewerHelpEvent.LOOP_DELETE_GRID_NOTICE_NEWER_HELP, {type:41, pos:pos});
				return;
			}
			pos = NewerHelpData.getSecondBagPos(type);
			if(pos >= 0) {
				sendNotification(NewerHelpEvent.LOOP_DELETE_GRID_NOTICE_NEWER_HELP, {type:44, pos:pos});
				return;
			}
		}
		
		/** 点击显示时装 */
		private function showDress(event:MouseEvent):void
		{
			var list:Array = RolePropDatas.ItemList;
			var date:Date = new Date();
			if(date.getTime() - this.startTime < this.HANDLERTIME * 1000)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_em_4" ] + (this.HANDLERTIME - int((date.getTime() - this.startTime) / 1000)) +GameCommonData.wordDic[ "mod_rp_med_em_5" ], color:0xffff00});   //"离下次操作还需等待"   "秒" 
				date = null;
				return;
			} 
			this.startTime = date.getTime();
			date = null;
			for(var i:int = 0; i< RolePropDatas.ItemList.length; i++)
			{
				if(RolePropDatas.ItemList[i] && int(RolePropDatas.ItemList[i].type / 10000) == 23 )
				{
					var data:Array = [];
					//显示 或 隐藏时装的代码
					if(GameCommonData.Player.Role.IsShowDress)
					{
						data =[0,GameCommonData.Player.Role.Id,0,0,0,0,299,0,0];
//						facade.sendNotification(RoleEvents.ISSHOWDRESS , false);
					}
					else
					{
						data =[0,GameCommonData.Player.Role.Id,0,0,1,0,299,0,0];
//						facade.sendNotification(RoleEvents.ISSHOWDRESS , true);
					}
					var obj:Object={type:1010,data:data};
					PlayerActionSend.PlayerAction(obj);
					data = null;
					obj = null;
					//
					return;
				}
			}
			
			//目前没有，测试
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_em_6" ], color:0xffff00});    //"请先穿上时装"
		}
		
		private function removeAllItem():void
		{
			var count:int = equip.numChildren - 1;
			while(count>=0)
			{
				if(equip.getChildAt(count) is ItemBase)
				{
					var item:ItemBase = equip.getChildAt(count) as ItemBase;
					equip.removeChild(item);
					item = null;
				}
				count--;
			}
			if(RolePropDatas.ItemUnitList.length == 0) return;
			for( var i:int = 0; i<RolePropDatas.ItemUnitList.length; i++ ) 
			{
				if( i == RolePropDatas.ItemUnitList.length - 1)
				{
					var soulItem:DisplayObject = RolePropDatas.ItemUnitList[i].Item;
					if(soulItem)
					{
						if(soulItem.parent)
						{
							soulItem.parent.removeChild(soulItem);
						}
					}
				}
				RolePropDatas.ItemUnitList[i].Item = null;
				RolePropDatas.ItemUnitList[i].IsUsed = false
			}
		}
	}
}