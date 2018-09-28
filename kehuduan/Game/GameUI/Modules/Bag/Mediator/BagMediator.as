package GameUI.Modules.Bag.Mediator
{
	import Controller.CooldownController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Command.DealItemCommand;
	import GameUI.Modules.Bag.Command.SetCdData;
	import GameUI.Modules.Bag.Command.SplitCommand;
	import GameUI.Modules.Bag.Command.UseItemCommand;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Mediator.DealItem.SplitItem;
	import GameUI.Modules.Bag.Mediator.DealItem.ThrowItem;
	import GameUI.Modules.Bag.Mediator.DealItem.Use;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridManager;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Proxy.DataProxy;
	import GameUI.SetFrame;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class BagMediator extends Mediator
	{
		public static const NAME:String = "BagMediator";
		public static const MAXPAGE:uint = 3;
		public static const MAXBTN:uint = 5;
		public static const STARTPOS:Point = new Point(7, 24);
		
		public var comfrim:Function = comfrimDrop;
		public var cancel:Function = cancelDrop;
		
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var isGet:Boolean = false;
		private var gridManager:GridManager = null;
		private var splitMediator:SplitItemMediator = null;
		private var throwItem:ThrowItem = new ThrowItem();
		private var useItem:Use = new Use();
		private var splitItem:SplitItem = new SplitItem();
		
		private var cacheCells:Array=[];
		protected var moneyBind:MoneyItem;
		protected var moneyUnBind:MoneyItem;
		protected var moneyRmb:MoneyItem;
		
		/** 点击整理延时计时器 */
		private var dealItemDelayTime:Number=0;
		
		public function BagMediator()
		{
			super(NAME);
		}
		
		private function get bag():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [	
				EventList.INITVIEW,					//初始化
				EventList.ENTERMAPCOMPLETE,			//进入场景
				EventList.SHOWBAG,					//显示背包
				EventList.GETITEMS,					//得到物品
				EventList.UPDATEBAG,				//更新背包物品
				EventList.GOTRADEVIEWFAULT,			//将物品放入交易栏失败
				EventList.BAGITEMUNLOCK,			//物品解锁
				EventList.TRADECOMPLETE,			//交易完成
//				EventList.TRADEFAULT,				//交易失败物品解锁
				EventList.CLOSEBAG,					//关闭背包
				BagEvents.DROPITEM,					//丢弃物品
				BagEvents.ADDITEM,					//增加物品
				EventList.UPDATEMONEY,				//更新背包钱
				BagEvents.SHOWBTN,
				BagEvents.UPDATEITEMNUM,
				BagEvents.EXTENDBAG,
				BagEvents.BAG_STOP_DROG,			//背包禁止拖动
				BagEvents.BAG_INIT_POS,				//背包还原位置
				BagEvents.BAG_GOTO_SOME_INDEX		//背包跳页
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			
			
			switch(notification.getName())
			{
				case EventList.INITVIEW:															//初始化
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.BAG});
					panelBase = new PanelBase(bag, bag.width+8, bag.height+12);
					panelBase.name = "Bag";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
//					panelBase.x = 586;//UIConstData.DefaultPos2.x;
//					panelBase.y = 55;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_bag_med_bag_han_1" ] );	 // 背 包
					gridManager = new GridManager();
					facade.registerProxy(gridManager);
					BagData.bagView = bag;
					facade.registerCommand(UseItemCommand.NAME, UseItemCommand);
					facade.registerCommand(DealItemCommand.NAME, DealItemCommand);
					facade.registerCommand(SplitCommand.NAME, SplitCommand);
					
					this.moneyBind=new MoneyItem();
					this.moneyRmb=new MoneyItem();
					this.moneyUnBind=new MoneyItem();
					
					this.bag.addChild(this.moneyBind);
					
					this.moneyBind.x=54;
					this.moneyBind.y=274;
					this.bag.addChild(this.moneyRmb);
					this.moneyRmb.x=52;
					this.moneyRmb.y=321;
					this.bag.addChild(this.moneyUnBind);
					this.moneyUnBind.x=54;
					this.moneyUnBind.y=297;
					
					initGrid();
				break;
				case EventList.ENTERMAPCOMPLETE:													//进入场景
//					NetAction.RequestItems();
//					facade.sendNotification(RoleEvents.GETFITOUTBYBAG);
					GameCommonData.GameInstance.GameUI.tabChildren = false;
					GameCommonData.GameInstance.GameUI.tabEnabled = false;
				break;
				case EventList.SHOWBAG:	
//					SoundManager.PlaySound(SoundList.PANEOPEN);																				//显示背包
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = 586 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						panelBase.y = 55 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}else{
						panelBase.x = 586;
						panelBase.y = 55;
					}
					GameCommonData.GameInstance.GameUI.addChild(panelBase); 
					panelBase.height = int(panelBase.height);
					showItems(BagData.AllItems[BagData.SelectIndex]);
					if(NewerHelpData.newerHelpIsOpen)	//打开背包通知新手指导
						sendNotification(NewerHelpEvent.OPEN_BAG_NOTICE_NEWER_HELP);
				break;
				case EventList.GETITEMS:															//得到物品
					showItems(BagData.AllItems[BagData.SelectIndex]);
				break;
				case EventList.UPDATEBAG:															//更新背包物品
					showItems(BagData.AllItems[BagData.SelectIndex]);
				break;
				case EventList.GOTRADEVIEWFAULT:													//将物品放入交易栏失败
					BagData.SelectedItem.Item.IsLock = false;
				break;
				case EventList.BAGITEMUNLOCK:														//物品解锁
					var id:int = notification.getBody() as int;
					bagItemUnLock(id);
				break;
				case EventList.TRADECOMPLETE:														//交易完成
					showItems(BagData.AllItems[BagData.SelectIndex]);
				break;
//				case EventList.TRADEFAULT:															//交易失败，所有的物品都解锁
//					for(var j:int = 0; j<BagData.GridUnitList.length; j++)
//					{
//						if(BagData.GridUnitList[j].Item)
//						{
//							BagData.AllLocks[BagData.SelectIndex][j] = false;
//							BagData.GridUnitList[j].Item.IsLock = false;
//						}
//					}
//					for(var i:int = 0; i<BagData.AllItems.length; i++)
//					{
//						for(var n:int = 0; n < BagData.AllItems[i].length; n++)
//						{
//							if(BagData.AllItems[i][n] == undefined) continue;
//							BagData.AllLocks[i][n] = false
//						}
//					}
//				break;
				case EventList.CLOSEBAG:															//交易完成
					panelCloseHandler(null);
				break;
				case BagEvents.DROPITEM:															//丢弃物品
					removeItem(notification.getBody());
				break;
				case BagEvents.ADDITEM:																//增加物品
					addItem(notification.getBody());
				break;
				case EventList.UPDATEMONEY:															//更新钱
//					trace(notification.getBody().money);
					switch (notification.getBody().target){
						case "mcBind":
							this.moneyBind.update(notification.getBody().money);
							break;
						case "mcUnBind":
							this.moneyUnBind.update(notification.getBody().money);
							break;
						case "mcRmb":
							this.moneyRmb.update(notification.getBody().money);
							break;
						default:			
							break;
					}
				break;
				case BagEvents.SHOWBTN:
					var bool:Boolean = notification.getBody() as Boolean;							//变更操作物品按钮的状态
					showBtn(bool);
				break;
				case BagEvents.UPDATEITEMNUM:														//更新物品数量
					updateNum();
				break;
				case BagEvents.EXTENDBAG:															//背包扩展
					extendGrid();
				break;
				case BagEvents.BAG_STOP_DROG:														//背包禁用拖动
					if(panelBase)
						panelBase.IsDrag = false;
				break;
				case BagEvents.BAG_INIT_POS:														//背包位置还原
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = 586 + (GameCommonData.GameInstance.MainStage.stageWidth - GameConfigData.GameWidth)/2;;
						panelBase.y = 55 + (GameCommonData.GameInstance.MainStage.stageHeight - GameConfigData.GameHeight)/2;;
					}else{
						panelBase.x = 586;
						panelBase.y = 55;
					}
				break;
				case BagEvents.BAG_GOTO_SOME_INDEX:													//背包跳页
					var toIndex:uint = uint(notification.getBody());
					bagToIndex(toIndex);
				break;
			}
		}
		
		private function initView():void
		{
			for(var i:int = 0; i<MAXPAGE; i++)
			{
				bag["mcPage_"+i].buttonMode = true;
				bag["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
				if(i==BagData.SelectIndex) 
				{		
					bag["mcPage_"+i].gotoAndStop(1);	
					bag["mcPage_"+i].removeEventListener(MouseEvent.CLICK, choicePageHandler);			
					continue;
				}
				bag["mcPage_"+i].gotoAndStop(2);
			}
			showBtn(false);
			bag.btnSall.addEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnSplit.addEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnDrop.addEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnDeal.addEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnUse.addEventListener(MouseEvent.CLICK, btnClickHandler);			
			//显示钱
			

			this.moneyBind.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.moneyUnBind.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
			this.moneyRmb.update(GameCommonData.Player.Role.UnBindRMB + "\\ab");
			
			

		}
		
		/** 背包跳页(跳到指定页) */
		private function bagToIndex(index:uint):void
		{
			if(BagData.SelectIndex == index) return;
			BagData.SelectIndex = index;
			for(var i:int = 0; i<MAXPAGE; i++) {
				bag["mcPage_"+i].gotoAndStop(2);
				bag["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			bag["mcPage_"+index].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			BagData.SelectedItem = null;
			showBtn(false);
			bag["mcPage_"+index].gotoAndStop(1);
			showLock();
			SetFrame.RemoveFrame(bag);
			showItems(BagData.AllItems[BagData.SelectIndex]);
		}
		
		//背包翻页
		private function choicePageHandler(event:MouseEvent):void
		{
			for(var i:int = 0; i<MAXPAGE; i++)
			{
				bag["mcPage_"+i].gotoAndStop(2);
				bag["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			var index:uint = uint(event.target.name.split("_")[1]);
			bag["mcPage_"+index].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			BagData.SelectIndex = index;
			BagData.SelectedItem = null;
			showBtn(false);
			bag["mcPage_"+index].gotoAndStop(1);
			showLock();
			/* if(bag.getChildByName("yellowFrame"))
			{
				bag.removeChild(bag.getChildByName("yellowFrame"));
			} */
			SetFrame.RemoveFrame(bag);
			sendNotification(BagEvents.REMOVE_BAG_EXTENDS);
			showItems(BagData.AllItems[BagData.SelectIndex]);	
		}
		
		//物品设置锁定锁定
		private function showLock():void
		{
			for(var i:int = 0; i<BagData.AllItems[BagData.SelectIndex].lenght; i++)
			{
				BagData.GridUnitList[i].Item.IsLock = BagData.AllLocks[BagData.SelectIndex][i];
			}
		}
		
		/**  初始化格子 */
		private function initGrid():void
		{
			for( var i:int = 0; i<36; i++ ) 
			{
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6) + STARTPOS.x;
				gridUnit.y = (gridUnit.height) * int(i/6) + STARTPOS.y;
				gridUnit.name = "bag_" + i.toString();
				bag.addChild(gridUnit);
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = bag;										//设置父级
				gridUint.Index = i;											//格子的位置		
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				BagData.GridUnitList.push(gridUint);
			}		
		}
		
		/**  初始化物品 */
		private function showItems(list:Array):void
		{			
			//移除所有界面上的物品	
			
			removeAllItem();
			//显示钱
			initView();			
			//管理器初始化				
			gridManager.Initialize();	
			if(BagData.GridUnitList.length == 0) return;
			for(var i:int = 0; i<list.length; i++)
			{
				BagData.GridUnitList[i].HasBag = true;
				//添加未开的格子数据
				if(i > BagData.BagNum[BagData.SelectIndex]-1)
				{
					var noBag:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NoBag");
					noBag.x = BagData.GridUnitList[i].Grid.x;
					noBag.y = BagData.GridUnitList[i].Grid.y;
					noBag.mouseEnabled = false;
					noBag.mouseChildren= false;
					noBag.name = "noBag";
					BagData.GridUnitList[i].HasBag = false;	
					BagData.GridUnitList[i].Item = noBag;	
					bag.addChild(noBag);
				} 
				//无数据,背包为空
				if(list[i] == undefined) 
				{
					continue;
				};
				//添加物品
				addItem(list[i]);
			}
			//目前有锁定的物品则显示操作按钮，加外框		
			if(BagData.SelectedItem)
			{
				showBtn(true);
				SetFrame.UseFrame(BagData.SelectedItem.Grid);
			}	
		}
		
		protected function getCells(pos:int, icon:String, parent:DisplayObjectContainer):UseItem{
			var useItem:UseItem=this.cacheCells.shift();
//			if(useItem==null){
			 	useItem=new UseItem(pos,icon,parent);
//			}else{
//				useItem.reset();
//				useItem.init(icon,parent,pos);
//			}
			return useItem;
		}
		
		//添加物品，初始化格子数组,如果有物品在cd添加cd
		private function addItem(item:Object):void
		{
			var gList:Array = BagData.GridUnitList;			
			var useItem:UseItem = this.getCells(item.index, item.type, bag);
			if(int(item.type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(item.type) > 300000)
			{
				useItem.Num = item.amount;
			}
			useItem.x = gList[item.index].Grid.x + 2;
			useItem.y = gList[item.index].Grid.y + 2;
			useItem.Id = item.id;
			useItem.IsBind = item.isBind;
			useItem.Type = item.type;
			useItem.IsLock = BagData.AllLocks[BagData.SelectIndex][item.index];
			gList[item.index].Item = useItem;
			gList[item.index].IsUsed = true;
			
			var cdObj:Object=SetCdData.searchCdObjByType(item.type);
			if(cdObj!=null){
//				useItem.startCd(cdObj.cdtimer,cdObj.count);
				//CooldownController.getInstance().triggerCD(useItem.Type, cdObj.cdtimer);
			}
				
			if(SetCdData.cdItemCollection[BagData.SelectIndex][item.index] != undefined)
			{
				if(SetCdData.cdItemCollection[BagData.SelectIndex][item.index].isCd)
				{
//					useItem.startCd(SetCdData.cdItemCollection[BagData.SelectIndex][item.index].cdtimer, SetCdData.cdItemCollection[BagData.SelectIndex][item.index].count+1);
				}
			}
			bag.addChild(useItem);	
			CooldownController.getInstance().registerCDItem(useItem.Type, useItem);
		}
		
		//扩展背包，先移除所有的未开启背包的，再添加
		private function extendGrid():void
		{
			var count:int = 0;
			removeAllnoBag();
			for(var i:int  = 0; i<36; i++)
			{
				BagData.GridUnitList[i].HasBag = true;
				if(BagData.GridUnitList[i].Item && BagData.GridUnitList[i].Item.name == "noBag") BagData.GridUnitList[i].Item = null;
				if(i > BagData.BagNum[BagData.SelectIndex]-1)
				{
					var noBag:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NoBag");
					noBag.x = BagData.GridUnitList[i].Grid.x;
					noBag.y = BagData.GridUnitList[i].Grid.y;
					noBag.mouseEnabled = false;
					noBag.mouseChildren= false;
					noBag.name = "noBag";
					BagData.GridUnitList[i].HasBag = false;	
					BagData.GridUnitList[i].Item = noBag;	
					bag.addChild(noBag);
				}
			}
		}
		
		//更新数量，根据type, 如果是装备不用更新
		private function updateNum():void
		{
			var curList:Array = BagData.AllItems[BagData.SelectIndex];
			for(var i:int = 0; i<curList.length; i++)
			{
				if(curList[i] == undefined) continue;
				if(BagData.GridUnitList[i].HasBag == false) continue;
				if(curList[i].type < 300000)
				{
					BagData.GridUnitList[i].Item.Num = 1;
				}
				else if(curList[i].type > 300000)
				{
					BagData.GridUnitList[i].Item.Num = curList[i].amount;
				}
			}
		}
		
		private function removeItem(item:Object):void
		{
			for( var i:int = 0; i<BagData.GridUnitList.length; i++)
			{
				if(BagData.GridUnitList[i].Item)
				{
					if(BagData.GridUnitList[i].Item.Id == item.id)
					{
						if(BagData.SelectedItem)
						{
							if(BagData.SelectedItem.Index == i)
							{
								SetFrame.RemoveFrame(bag);
								BagData.SelectedItem = null;
								showBtn(false);
							}
						}
						if(bag.contains(BagData.GridUnitList[i].Item))
						{
							bag.removeChild(BagData.GridUnitList[i].Item);
//							if(BagData.GridUnitList[i].Item is UseItem){
//							
////								(BagData.GridUnitList[i].Item as UseItem).reset();
////								BagData.GridUnitList[i].Item=null;
////								this.cacheCells.push(BagData.GridUnitList[i].Item);
//							}
						}
						BagData.GridUnitList[i].Item = null;	
						BagData.GridUnitList[i].IsUsed = false;
					}
				}		
			}
		}

		//物品的快捷操作		
		private function btnClickHandler(event:MouseEvent):void
		{
			switch(event.target.name)
			{
				case "btnSall":
					//摆摊
					facade.sendNotification(EventList.SHOWSTALL);
				break;
				case "btnSplit":
					//拆分
					if(BagData.SplitIsOpen) return;
					if(BagData.SelectedItem) {
						if(BagData.SelectedItem.Item.IsLock)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_1" ], color:0xffff00});  // 此物品已锁定，无法拆分
							return;
						}
						facade.sendNotification(SplitCommand.NAME);
					}
				break;
				case "btnDrop":
					if(BagData.SelectedItem) {
						var obj:Object = UIConstData.getItem(BagData.SelectedItem.Item.Type); 
						if(obj != null) {
							var mask:uint = obj.Monopoly & 0x40;
							if(mask > 0){
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_2" ], color:0xffff00});   // 此物品不能丢弃
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
						if(BagData.SelectedItem.Item.IsLock) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_2" ], color:0xffff00});    // 此物品不能丢弃 
							return;
						}
						// 物品丢弃将消失，确定要丢弃么？
						facade.sendNotification(EventList.SHOWALERT, {comfrim:comfrimDrop, cancel:cancelDrop, info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_3" ] });  // 物品丢弃将消失，确定要丢弃么？
					}
				break;
				//整理
				case "btnDeal":
					var curTime:Number=getTimer();
					if(curTime-this.dealItemDelayTime>5000){
						this.dealItemDelayTime=curTime;
						facade.sendNotification(DealItemCommand.NAME, bag);
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_5" ], color:0xffff00});   // 物品整理过快，请稍后再整理
					}
				break;
				//使用
				case "btnUse":
					if(BagData.SelectedItem) 
					{
						if(BagData.SelectedItem.Item.IsLock)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_med_bag_btn_4" ], color:0xffff00});   // 此物品已锁定，无法使用
							return;
						}
						facade.sendNotification(UseItemCommand.NAME);
					}
				break;
			}
		}
		
		//丢弃物品处理
		public function comfrimDrop():void { ThrowItem.Throw(BagData.SelectedItem); }
		
		public function cancelDrop():void {}
		/**
		 * 做全刷新
		 * 移除所有的物品 
		 * 将所有的格子都初始化 
		 * */
		private function removeAllItem():void
		{
			var count:int = bag.numChildren - 1;
			while(count>=0)
			{
				if(bag.getChildAt(count) is ItemBase)
				{
					var item:ItemBase = bag.getChildAt(count) as ItemBase;
					CooldownController.getInstance().unregisterCDItem(item.Type, item);
					bag.removeChild(item);
					item=null;
//					this.cacheCells.push(item);
//					item.reset();
				}
				count--;
			}
			removeAllnoBag();
			SetFrame.RemoveFrame(bag);
			for( var i:int = 0; i<36; i++ ) 
			{
				if(BagData.GridUnitList[i] == null) continue;
				BagData.GridUnitList[i].Item = null;
				BagData.GridUnitList[i].IsUsed = false;
			}
//			System.CollectEMS();
			
		}
		
		private function removeAllnoBag():void
		{
			var count2:int = bag.numChildren - 1;
			while(count2>=0)
			{
				if(bag.getChildAt(count2).name == "noBag")
				{
					var noBag:MovieClip = bag.getChildAt(count2) as MovieClip;
					bag.removeChild(noBag);
					noBag = null;
				}
				count2--; 
			}
		}
				
		private function panelCloseHandler(event:Event):void
		{
			panelBase.IsDrag = true;
			if(panelBase.parent)
			{
				panelBase.parent.removeChild(panelBase);
			}
//			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			if(BagData.SplitIsOpen) sendNotification(BagEvents.REMOVE_SPLIT);
//			SoundManager.PlaySound(SoundList.PANECLOSE);
			for(var i:int = 0; i<MAXPAGE; i++)
			{
				bag["mcPage_"+i].removeEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			bag.btnSall.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnSplit.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnDrop.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnDeal.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			bag.btnUse.removeEventListener(MouseEvent.CLICK, btnClickHandler);	
			gridManager.Gc();
			dataProxy.BagIsOpen = false;
			if(NewerHelpData.newerHelpIsOpen)
				sendNotification(NewerHelpEvent.CLOSE_BAG_NOTICE_NEWER_HELP);		//通知新手帮助系统做关闭类处理
		}
		
		/** 根据ID对物品解锁  */
		private function bagItemUnLock(id:int):void
		{
			for(var i:int = 0; i<BagData.AllItems.length; i++)
			{
				if(i == BagData.SelectIndex)
				{
					for(var j:int = 0; j<BagData.GridUnitList.length; j++)
					{
						if(BagData.GridUnitList[j].Item)
						{
							if(id == BagData.GridUnitList[j].Item.Id)
							{
								BagData.GridUnitList[j].Item.IsLock = false;
							}
						}
					}
				}
				for(var n:int = 0; n<BagData.AllItems[i].length; n++)
				{
					if(BagData.AllItems[i][n] == undefined) continue;
					if(id == BagData.AllItems[i][n].id)
					{
						BagData.AllLocks[i][n] = false
					}
				}
			}
		}
		
		private function showBtn(bool:Boolean):void
		{
			bag.btnSplit.visible = bool;
			bag.btnDrop.visible = bool;
			bag.btnUse.visible = bool;
			if(bool)
			{
				if(BagData.SelectedItem.Item)
				{
					if(BagData.SelectedItem.Item.Type < 300000)
					{
						bag.btnSplit.visible = false;
					}
				}
			}
		}
	}
}