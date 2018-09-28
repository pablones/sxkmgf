package GameUI.Modules.Artifice.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Artifice.data.ArtificeConst;
	import GameUI.Modules.Artifice.data.ArtificeData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ArtificeMediator extends Mediator
	{
		public static const NAME:String="ArtificeMediator";

		private var panelBase:PanelBase;
		private var mainView:MovieClip;

		/** 格子类 */
		private var ArtificeGrid:Class;
		private var grid:MovieClip;
		/** 装备图标  */
		protected var useItem:UseItem;
		/** 装备数据 */
		protected var useItemData:Object;
		
		private var loader:Loader;
		/** true：正在加载中 */
		private var loading:Boolean=false;
		/** true：加载成功 */
		private var loadSucceed:Boolean=false;

		public function ArtificeMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME);
		}

		public override function listNotificationInterests():Array
		{
			return [ArtificeConst.SHOW_ARTIFICE_VIEW, 
					ArtificeConst.CLOSE_ARTIFICE_VIEW,
					ArtificeConst.ADD_ITEM_TO_ARTIFICE_VIEW,
					ArtificeConst.ARTIFIC_SUC,
					ArtificeConst.ARTIFIC_MOUSEUP,
					EventList.CLOSE_NPC_ALL_PANEL,
					];
		}

		public override function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case ArtificeConst.SHOW_ARTIFICE_VIEW:
					show();
					break;
				case ArtificeConst.CLOSE_ARTIFICE_VIEW:
					close();
					break;
				case ArtificeConst.ADD_ITEM_TO_ARTIFICE_VIEW:
					addItemToView(notification.getBody());
					break;
				case ArtificeConst.ARTIFIC_SUC:
					removeItemToView();
					break;
				case ArtificeConst.ARTIFIC_MOUSEUP:
					onMouseUp();
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
			panelBase=new PanelBase(mainView, mainView.width + 8, mainView.height + 12);
			panelBase.SetTitleTxt(ArtificeData.StrengthenTransStr1);
			ArtificeData.isInit=true;
			
			grid = new ArtificeGrid();
			grid.type = "ArtificeGrid";
			grid.x = ArtificeData.gridSite[0];
			grid.y = ArtificeData.gridSite[1];
			mainView.addChild(grid);
			mainView.btn_commit.visible = false
			
			show();
		}

		private function show():void
		{
			if (!ArtificeData.isInit)
			{
				init();
				return;
			}
			if (!ArtificeData.isShowView)
			{
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
				addLis();

				panelBase.x=UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth) / 2 + ArtificeData.panelBaseSite[0];
				panelBase.y=UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight) / 2 + ArtificeData.panelBaseSite[1];
			}
			ArtificeData.isShowView=true;
			sendNotification(EventList.SHOWBAG);
		}
		
		private function addItemToView(data:Object):void
		{
			var useItemdata:Object = IntroConst.ItemInfo[data.id];
			if(!checkItem(useItemdata)) return;
						
			if(!useItem)
			{
				useItemData = useItemdata;
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true;
				
				useItem = new UseItem(data.index, "" + data.type, grid);
				
				useItem.x=ArtificeData.useItemSite[0];
				useItem.y=ArtificeData.useItemSite[1];
				
				useItem.Id = data.id;
				useItem.IsBind = data.isBind;
				useItem.Type = data.type;
				
				
				grid.name="bagQuickKey_" + data.id;
				grid.addChild(useItem);
				grid.addEventListener(MouseEvent.CLICK,onMouseClick);
				grid.addEventListener(MouseEvent.MOUSE_DOWN,onMouseDown);
				
			}
			else
			{
				removeItemToView();
				
				useItemData = useItemdata;
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true;
				
				useItem = new UseItem(data.index, "" + data.type, grid);
				
				useItem.x=ArtificeData.useItemSite[0];
				useItem.y=ArtificeData.useItemSite[1];
				
				useItem.Id = data.id;
				useItem.IsBind = data.isBind;
				useItem.Type = data.type;
				
				grid.name="bagQuickKey_" + data.id;
				
				grid.addChild(useItem);
				grid.addEventListener(MouseEvent.CLICK,onMouseClick);
				grid.addEventListener(MouseEvent.MOUSE_DOWN,onMouseDown);
			}
			if(useItem) mainView.btn_commit.visible = true;
			else mainView.btn_commit.visible = false;
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
			if(useItem)
			{
				useItem.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				useItem.onMouseDown();
			}
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":
 					removeItemToView();
					break;
			}
		}
		
		private function checkItem(useItemData:Object):Boolean
		{
			/** 必须是装备 */
			if (!useItemData || !ArtificeData.isEquip(useItemData.type))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:ArtificeData.StrengthenTransStr2, color:0xffff00});
				return false;
			}
			var color:int = 0;
			if(useItemData.color && useItemData.id != undefined) {
				color = useItemData.color;
			} else { 
				color = UIConstData.getItem(useItemData.type).Color;
			}
			/** 需要有蓝色品质以上（包含蓝色）的装备  */
			if(color < ArtificeData.artificeNeed[0])
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:ArtificeData.StrengthenTransStr3, color:0xffff00});
				return false;
			}
			var item:Object = UIConstData.getItem(useItemData.type);
			/** 装备等级必须大于50级 */
			if(item.Level < ArtificeData.artificeNeed[1])
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:ArtificeData.StrengthenTransStr4, color:0xffff00});
				return false;
			}
			/** 打了魂印宝石的装备不能被分解 */
			if(useItemData.isBind == ArtificeData.artificeNeed[2])
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:ArtificeData.StrengthenTransStr5, color:0xffff00});
				return false;
			}
			return true;
		}
		
		private function removeItemToView():void
		{
			if(useItem)
			{
				sendNotification(EventList.BAGITEMUNLOCK, useItemData.id);
				useItem.parent.removeChild(useItem);
				useItem.removeEventListener(MouseEvent.CLICK,onMouseClick);
				useItem = null;
				useItemData = null;
				grid.name="";
				mainView.btn_commit.visible = false;
			}
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			var dis:DisplayObject = me.target as DisplayObject;
			if(grid === dis)
			{
				removeItemToView();
			}
		}
		
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, close);
			mainView.btn_commit.addEventListener(MouseEvent.CLICK, onClick);
//			grid.addEventListener(MouseEvent.MOUSE_UP,onMouseUp);
		}

		private function removeLis():void
		{
			panelBase.removeEventListener(Event.CLOSE, close);
			mainView.btn_commit.removeEventListener(MouseEvent.CLICK, onClick);
//			grid.removeEventListener(MouseEvent.MOUSE_UP,onMouseUp);
		}
		
		public function onMouseUp(event:MouseEvent=null):void
		{
			if(!BagData.SelectedItem)	//未选择物品
			{
				return;
			}
			if(BagData.SelectedItem.Item.IsLock) //物品已锁
			{
				return;
			}
			if(useItemData)
			{
				if(useItemData.Id == BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index].id)
				{
					return;
				}
			}
			
			addItemToView(BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index]);
			
		}
		
		private function onClick(event:MouseEvent):void
		{
			var name:String = event.currentTarget.name;
			switch(name)
			{
				case "btn_commit":
					onCommit();
					break;
			}
		}
		
		private function onCommit():void
		{
			var b:Boolean = true;
			var str:String = "";
			if(!useItemData) return;
			if(useItemData.stoneBaseList.length >= ArtificeData.promptNeed[0])
			{
				str += ArtificeData.StrengthenTransStr6;
				b = false;
			}
			if(useItemData.star >= ArtificeData.promptNeed[1])
			{
				str += ArtificeData.StrengthenTransStr7;
				b = false;
			}
			if(useItemData.level >= ArtificeData.promptNeed[2])
			{
				str += ArtificeData.StrengthenTransStr8;
				b = false;
			}
			var color:int = 0;
			if(useItemData.color && useItemData.id != undefined) {
				color = useItemData.color;
			} else { 
				color = UIConstData.getItem(useItemData.type).Color;
			}
			/** 需要有蓝色品质以上（包含蓝色）的装备  */
			if(color >= ArtificeData.promptNeed[3])
			{
				str += ArtificeData.StrengthenTransStr9;
				b = false;
			}
			/** 已经铸灵 */
			if(int(useItemData.castSpiritLevel) > 0)
			{
				b = false;
			}
			if(!b)
			{
				str += ArtificeData.StrengthenTransStr10;
				facade.sendNotification(EventList.SHOWALERT, {comfrim: onSureToBuy, cancel: new Function(), info: "<font color='#E2CCA5'>"+str+"</font>", title: GameCommonData.wordDic["often_used_tip"]}); //"提 示"
				return;
			}
			ArtificeData.artificeById(useItemData.id);
		}
		
		private function onSureToBuy():void
		{
			ArtificeData.artificeById(useItemData.id);
		}
		
		private function txtChangeHandler(e:Event):void
		{
			var newCount:uint=uint(mainView.txt_inputNum.text);
			var count:uint=Math.min(999, newCount);
			count=Math.max(count, 1);
			mainView.txt_inputNum.text="" + count;
		}

		/* 加载资源 */
		private function load():void
		{
			loading=true;
			loader=new Loader();
			var request:URLRequest=new URLRequest();
			var adr:String=GameCommonData.GameInstance.Content.RootDirectory + ArtificeData.resourcePath;
			request.url=adr;
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onComplete);
			loader.load(request);
		}

		private function onComplete(event:Event):void
		{
			var domain:ApplicationDomain=event.target.applicationDomain as ApplicationDomain;

			if (domain.hasDefinition("Artifice"))
			{
				var Artifice:Class=domain.getDefinition("Artifice") as Class;
				mainView=new Artifice();
				
				ArtificeGrid = domain.getDefinition("ArtificeGrid") as Class;
				
				ArtificeData.useItemSite=loader.contentLoaderInfo.content["useItemSite"] as Array;
				ArtificeData.gridSite=loader.contentLoaderInfo.content["gridSite"] as Array;
				ArtificeData.panelBaseSite=loader.contentLoaderInfo.content["panelBaseSite"] as Array;
				ArtificeData.artificeNeed=loader.contentLoaderInfo.content["artificeNeed"] as Array;
				ArtificeData.promptNeed=loader.contentLoaderInfo.content["promptNeed"] as Array;
				
				
				loader.contentLoaderInfo.removeEventListener(Event.COMPLETE, onComplete);
				loader=null;
				loading=false;
				loadSucceed=true;
				init();
			}
		}

		private function close(event:Event=null):void
		{
			if (ArtificeData.isShowView)
			{
				removeItemToView();
				
				GameCommonData.GameInstance.GameUI.removeChild(panelBase)
				removeLis();
				ArtificeData.isShowView=false;
			}
		}

	}
}
