package GameUI.Modules.StrengthenTransfer.Mediator
{

	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferConst;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;

	import flash.display.DisplayObject;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.text.TextField;

	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StrengthenTransferMediator extends Mediator
	{
		public static const NAME:String="StrengthenTransferMediator";

		private var panelBase:PanelBase;
		private var mainView:MovieClip;

//		/** 挪移护符图标*/
//		protected var helpItem:EnableItem;
//		/** 挪移护符数据*/
//		protected var helpItemData:Object;
		/** 格子类 */
		private var StrengthenTransferGrid:Class;
		private var grid1:MovieClip;
		private var grid2:MovieClip;
		/** 装备1图标  */
		protected var useItem1:UseItem;
		/** 装备1数据 */
		protected var useItemData1:Object;
		/** 装备2图标  */
		protected var useItem2:UseItem;
		/** 装备2数据 */
		protected var useItemData2:Object;
		/** 快速购买 */
		private var fastPurchase:FastPurchase;

		private var select:int=1;

		private var loader:Loader;
		/** true：正在加载中 */
		private var loading:Boolean=false;
		/** true：加载成功 */
		private var loadSucceed:Boolean=false;

		public function StrengthenTransferMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME);
		}

		public override function listNotificationInterests():Array
		{
			return [StrengthenTransferConst.SHOW_STRENGTHENTRANSFER_VIEW, 
					StrengthenTransferConst.CLOSE_STRENGTHENTRANSFER_VIEW, 
					StrengthenTransferConst.ADD_ITEM_TO_STRENGTHENTRANSFER_VIEW, 
					StrengthenTransferConst.STRENGTHENTRANSFER_SUC, 
					StrengthenTransferConst.STRENGTHENTRANSFER_MOUSEUP, 
					EventList.ONSYNC_BAG_QUICKBAR,
					EventList.CLOSE_NPC_ALL_PANEL,
					];
		}

		public override function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case StrengthenTransferConst.SHOW_STRENGTHENTRANSFER_VIEW:
					show();
					break;
				case StrengthenTransferConst.CLOSE_STRENGTHENTRANSFER_VIEW:
					close();
					break;
				case StrengthenTransferConst.ADD_ITEM_TO_STRENGTHENTRANSFER_VIEW:
					addItemToView(notification.getBody());
					break;
				case StrengthenTransferConst.STRENGTHENTRANSFER_SUC:
					changeDes();
					removeItemToView();
					break;
				case StrengthenTransferConst.STRENGTHENTRANSFER_MOUSEUP:
					onMouseUp();
					break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if (StrengthenTransferData.isShowView && notification.getBody() == StrengthenTransferData.helpItem[0])
					{
						changeDes();
					}
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					close();
					break;
			}
		}

		private function init():void
		{
			if (loading)
				return;
			if (!loadSucceed)
			{
				load();
				return;
			}
			fastPurchase=new FastPurchase(StrengthenTransferData.helpItem[0]);
			fastPurchase.x=StrengthenTransferData.helpItem[1];
			fastPurchase.y=StrengthenTransferData.helpItem[2];
			mainView.addChild(fastPurchase);
			panelBase=new PanelBase(mainView, mainView.width + StrengthenTransferData.panelBaseWH[0] + 8, mainView.height + 12 + StrengthenTransferData.panelBaseWH[1]);
			panelBase.SetTitleTxt(StrengthenTransferData.StrengthenTransStr1);
			StrengthenTransferData.isInit=true;

			grid1=new StrengthenTransferGrid();
			grid2=new StrengthenTransferGrid();
			grid1.type="StrengthenTransferGrid";
			grid2.type="StrengthenTransferGrid";
			grid1.x=StrengthenTransferData.gridSite[0];
			grid1.y=StrengthenTransferData.gridSite[1];
			grid2.x=StrengthenTransferData.gridSite[2];
			grid2.y=StrengthenTransferData.gridSite[3];
			mainView.addChild(grid1);
			mainView.addChild(grid2);
			(mainView.txt_StrengthenTransfer as TextField).selectable=false;

			mainView.btn_commit.visible=false;

//			helpItemData=UIUtils.getGoodsAtMarket(StrengthenTransferData.helpItem[0]);

			show();
		}

		private function show():void
		{
			if (!StrengthenTransferData.isInit)
			{
				init();
				return;
			}
			if (!StrengthenTransferData.isShowView)
			{
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
				addLis();

				panelBase.x=UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth) / 2 + StrengthenTransferData.panelBaseSite[0];
				panelBase.y=UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight) / 2 + StrengthenTransferData.panelBaseSite[1];
			}
			StrengthenTransferData.isShowView=true;
			sendNotification(EventList.SHOWBAG);
			changeDes();
		}

		/**
		 * 刷新显示
		 *
		 */
		private function changeDes():void
		{

			var helpItemNum:int=BagData.hasItemNum(StrengthenTransferData.helpItem[0]);
			(mainView.txt_StrengthenTransfer as TextField).htmlText=StrengthenTransferData.StrengthenTransStr2 + "<font color='#00FF00'>1</font>" + StrengthenTransferData.StrengthenTransStr3 + "<font color='#00FFFF'>" + UIConstData.getItem(StrengthenTransferData.helpItem[0]).Name + "</font>" + StrengthenTransferData.StrengthenTransStr4 + "<font color='#00FF00'>" + helpItemNum + "</font>" + StrengthenTransferData.StrengthenTransStr5;

//			if (helpItem != null && mainView.StrengthenTransfer_grid_1.contains(helpItem))
//			{
//				mainView.StrengthenTransfer_grid_1.removeChild(helpItem);
//				helpItem=null;
//			}
//			(mainView.btn_commit as SimpleButton).visible=true;
//			helpItem=new EnableItem("" + StrengthenTransferData.helpItem[0], "icon");
//			helpItem.name="goodQuickBuy_" + StrengthenTransferData.helpItem[0];
//			helpItem.x=StrengthenTransferData.helpItem[1];
//			helpItem.y=StrengthenTransferData.helpItem[2];
//			mainView.StrengthenTransfer_grid_1.addChild(helpItem);
		}

		private function addItemToView(data:Object):void
		{
			var useItemData:Object=IntroConst.ItemInfo[data.id];
			if (!checkItem(useItemData))
				return;

			if (!useItem1)
			{
				useItemData1=useItemData;
				BagData.SelectedItem.Item.IsLock=true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index]=true;

				useItem1=new UseItem(data.index, "" + data.type, grid1);

				useItem1.x=StrengthenTransferData.useItemSite[0];
				useItem1.y=StrengthenTransferData.useItemSite[1];

				useItem1.Id=data.id;
				useItem1.IsBind=data.isBind;
				useItem1.Type=data.type;

				grid1.name="bagQuickKey_" + data.id;
				grid1.addChild(useItem1);
				grid1.addEventListener(MouseEvent.CLICK, onMouseClick);
				grid1.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				return;
			}
			if (!useItem2)
			{
				useItemData2=useItemData;
				BagData.SelectedItem.Item.IsLock=true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index]=true;

				useItem2=new UseItem(data.index, "" + data.type, grid2);

				useItem2.x=StrengthenTransferData.useItemSite[2];
				useItem2.y=StrengthenTransferData.useItemSite[3];

				useItem2.Id=data.id;
				useItem2.IsBind=data.isBind;
				useItem2.Type=data.type;

				grid2.name="bagQuickKey_" + data.id;
				grid2.addChild(useItem2);
				grid2.addEventListener(MouseEvent.CLICK, onMouseClick);
				grid2.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				if (useItem1 && useItem2)
				{
					mainView.btn_commit.visible=true;
				}
				else
				{
					mainView.btn_commit.visible=false;
				}
				return;
			}
		}

		private function onMouseDown(event:MouseEvent):void
		{
			if (event.target == grid1 && useItem1)
			{
				select=1;
				useItem1.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				useItem1.onMouseDown();
			}
			if (event.target == grid2 && useItem2)
			{
				select=2;
				useItem2.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				useItem2.onMouseDown();
			}
		}

		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch (e.Data.type)
			{
				case "bag":
					removeItemToView(select);
					break;
			}
		}

		private function removeItemToView(i:int=1):void
		{
			if (useItem1 && 1 == i)
			{
				sendNotification(EventList.BAGITEMUNLOCK, useItemData1.id);
				useItem1.parent.removeChild(useItem1);
				useItem1.removeEventListener(MouseEvent.CLICK, onMouseClick);
				useItem1=null;
				useItemData1=null;
				grid1.name="";
				mainView.btn_commit.visible=false;
			}
			if (useItem2)
			{
				sendNotification(EventList.BAGITEMUNLOCK, useItemData2.id);
				useItem2.parent.removeChild(useItem2);
				useItem2.removeEventListener(MouseEvent.CLICK, onMouseClick);
				useItem2=null;
				useItemData2=null;
				grid2.name="";
				mainView.btn_commit.visible=false;
			}
		}

		private function checkItem(useItemData:Object):Boolean
		{
			if (useItem1 && useItem2)
				return false;
			/** 必须是装备 */
			if (!useItemData || !StrengthenTransferData.isEquip(useItemData.type))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info: StrengthenTransferData.StrengthenTransStr6, color: 0xffff00});
				return false;
			}
			/** 第一个装备强化等级必须大于1 */
			if (!useItem1 && useItemData.level < 1)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info: StrengthenTransferData.StrengthenTransStr7, color: 0xffff00});
				return false;
			}
			/** 第一个装备强化等级必须比第二个装备高 */
			if (useItem1 && useItemData1.level <= useItemData.level)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info: StrengthenTransferData.StrengthenTransStr8, color: 0xffff00});
				return false;
			}
			/** 第一个装备与第二个装备类型必须相同*/
			if (useItem1 && !StrengthenTransferData.typeCompare(useItemData1.type, useItemData.type))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info: StrengthenTransferData.StrengthenTransStr9, color: 0xffff00});
				return false;
			}
			return true;
		}

		private function onMouseClick(me:MouseEvent):void
		{
			var dis:DisplayObject=me.target as DisplayObject;
			if (grid1 === dis)
			{
				removeItemToView();
			}
			else if (grid2 == dis)
			{
				if (useItem2)
				{
					sendNotification(EventList.BAGITEMUNLOCK, useItemData2.id);
					useItem2.parent.removeChild(useItem2);
					useItem2.removeEventListener(MouseEvent.CLICK, onMouseClick);
					useItem2=null;
					useItemData2=null;
					grid2.name="";
					mainView.btn_commit.visible=false;
				}
			}
		}

		private function addLis():void
		{
//			mainView.txt_inputNum.addEventListener(Event.CHANGE, txtChangeHandler);
//			UIUtils.addFocusLis(mainView.txt_inputNum);
			panelBase.addEventListener(Event.CLOSE, close);
//			(mainView.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK, onBuyItemHandler);
			mainView.btn_commit.addEventListener(MouseEvent.CLICK, onClick);
//			grid1.addEventListener(MouseEvent.MOUSE_UP,onMouseUp);
//			grid2.addEventListener(MouseEvent.MOUSE_UP,onMouseUp);
		}

		private function removeLis():void
		{
//			mainView.txt_inputNum.removeEventListener(Event.CHANGE, txtChangeHandler);
//			UIUtils.removeFocusLis(mainView.txt_inputNum);
			panelBase.removeEventListener(Event.CLOSE, close);
//			(mainView.btn_buy as SimpleButton).removeEventListener(MouseEvent.CLICK, onBuyItemHandler);
			mainView.btn_commit.removeEventListener(MouseEvent.CLICK, onClick);
//			grid1.removeEventListener(MouseEvent.MOUSE_UP,onMouseUp);
//			grid2.removeEventListener(MouseEvent.MOUSE_UP,onMouseUp);
		}

		private function onClick(event:MouseEvent):void
		{
			var name:String=event.currentTarget.name;
			switch (name)
			{
				case "btn_commit":
					if (BagData.hasItemNum(StrengthenTransferData.helpItem[0]) < 1)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info: StrengthenTransferData.StrengthenTransStr10, color: 0xffff00});
						return;
					}
					if (useItemData1 && useItemData2)
					{
						if(useItemData2.isBind^useItemData1.isBind)
						{
							if(useItemData2.level > 0)
							{
								facade.sendNotification(EventList.SHOWALERT, {comfrim: onSureToBuy, cancel: new Function(), info: "<font color='#E2CCA5'>"+StrengthenTransferData.StrengthenTransStr12+"</font>", title: GameCommonData.wordDic["often_used_tip"]}); //"提 示"
								return;
							}
							facade.sendNotification(EventList.SHOWALERT, {comfrim: onSureToBuy, cancel: new Function(), info: "<font color='#E2CCA5'>"+StrengthenTransferData.StrengthenTransStr13+"</font>", title: GameCommonData.wordDic["often_used_tip"]}); //"提 示"
							return;
						}
						if(useItemData2.level > 0)
						{
							facade.sendNotification(EventList.SHOWALERT, {comfrim: onSureToBuy, cancel: new Function(), info: "<font color='#E2CCA5'>"+StrengthenTransferData.StrengthenTransStr11+"</font>", title: GameCommonData.wordDic["often_used_tip"]}); //"提 示"
							return;
						}
						StrengthenTransferData.toStrengthenTransfer(useItemData1.id, useItemData2.id);
					}
					break;
			}
		}
		
		private function onSureToBuy():void
		{
			StrengthenTransferData.toStrengthenTransfer(useItemData1.id, useItemData2.id);
		}

		private function onMouseUp(event:MouseEvent=null):void
		{
			if (!BagData.SelectedItem) //未选择物品
			{
				return;
			}
			if (BagData.SelectedItem.Item.IsLock) //物品已锁
			{
				return;
			}
//			if(useItemData)
//			{
//				if(useItemData.Id == BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index].id)
//				{
//					return;
//				}
//			}

			addItemToView(BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);

		}

//		private function txtChangeHandler(e:Event):void
//		{
//			var newCount:uint=uint(mainView.txt_inputNum.text);
//			var count:uint=Math.min(999, newCount);
//			count=Math.max(count, 1);
//			mainView.txt_inputNum.text="" + count;
//		}

//		private function onBuyItemHandler(e:MouseEvent):void
//		{
//			var num:uint=uint(mainView.txt_inputNum.text);
//			if (num == 0)
//			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info: GameCommonData.wordDic["mod_equ_med_add_onBuyI_2"], color: 0xffff00}); //"请输入有效的购买数"
//				return;
//			}
//			var strInfo:String=GameCommonData.wordDic["often_used_cost"] + '<font color="#00ff00">' + helpItemData.PriceIn * num + '</font>\\ab,' + GameCommonData.wordDic["often_used_buy"] + '<font color="#00ff00">' + num + '</font>' + GameCommonData.wordDic["mod_cam_med_ui_UIC1_cre_1"] + '<font color="#00ff00">' + helpItemData.Name + '</font>';
//			//花费		购买		个
//			facade.sendNotification(EventList.SHOWALERT, {comfrim: onSureToBuy, cancel: new Function(), info: strInfo, title: GameCommonData.wordDic["often_used_tip"]}); //"提 示"
//		}

//		private function onSureToBuy():void
//		{
//			var num:uint=uint(mainView.txt_inputNum.text);
//			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type: helpItemData.type, count: num});
//		}

		/* 加载资源 */
		private function load():void
		{
			loading=true;
			loader=new Loader();
			var request:URLRequest=new URLRequest();
			var adr:String=GameCommonData.GameInstance.Content.RootDirectory + StrengthenTransferData.resourcePath;
			request.url=adr;
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onComplete);
			loader.load(request);
		}

		private function onComplete(event:Event):void
		{
			var domain:ApplicationDomain=event.target.applicationDomain as ApplicationDomain;

			if (domain.hasDefinition("StrengthenTransfer"))
			{
				var StrengthenTransfer:Class=domain.getDefinition("StrengthenTransfer") as Class;
				mainView=new StrengthenTransfer();

				StrengthenTransferGrid=domain.getDefinition("StrengthenTransferGrid") as Class;

				StrengthenTransferData.helpItem=loader.contentLoaderInfo.content["helpItem"] as Array;
				StrengthenTransferData.useItemSite=loader.contentLoaderInfo.content["useItemSite"] as Array;
				StrengthenTransferData.gridSite=loader.contentLoaderInfo.content["gridSite"] as Array;
				StrengthenTransferData.panelBaseSite=loader.contentLoaderInfo.content["panelBaseSite"] as Array;
				StrengthenTransferData.panelBaseWH=loader.contentLoaderInfo.content["panelBaseWH"] as Array;

				loader.contentLoaderInfo.removeEventListener(Event.COMPLETE, onComplete);
				loader=null;
				loading=false;
				loadSucceed=true;
				init();
			}
		}

		private function close(event:Event=null):void
		{
			if (StrengthenTransferData.isShowView)
			{
				removeItemToView();

				GameCommonData.GameInstance.GameUI.removeChild(panelBase)
				removeLis();
				StrengthenTransferData.isShowView=false;

			}
		}

	}
}
