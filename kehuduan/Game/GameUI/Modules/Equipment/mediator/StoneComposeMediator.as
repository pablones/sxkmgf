package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Equipment.command.ComposeCommand;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.SetFrame;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StoneComposeMediator extends Mediator
	{
		public static const NAME:String="StoneComposeMediator";
		private var basePanel:PanelBase;
		private var currentType:String;														//当前要合成的宝石type 初始为0
		private var index:uint;
		private var aFull:Array;																	//判断格子是否有东西
		private var currentLevel:uint;														//当前级别
		private var maxLevel:uint;															//宝石的最高等级，现在为9
		private var needMoney:uint;
		private var numStone:uint;																	//宝石数量
		private var hasFu:Boolean = false;													//有符吗
		private var aStoneId:Array;															//宝石Id数组
		private var fuId:uint;																	//符id
		private var percent:uint;
		
		private var itemContainer:Sprite;														
		private var allItem:Array;																//添加进面板的所有图标
		private var currentMoney:int;															//自己有的银两
		private var aStoneBind:Array;																//所有的宝石是否绑定数组
		private var dataProxy:DataProxy;
		
		public function StoneComposeMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get viewUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [EventList.INITVIEW,
					EventList.CLOSE_NPC_ALL_PANEL,
					EventList.STONE_COMPOSE_UI,
					EquipCommandList.COMPOSE_STONE,					//物品拖动事件
					EquipCommandList.COMPOSE_STONEFU,
					EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE,	//转场景时关闭面板
					//新加的2个功能
					EquipCommandList.DBCLICK_STONE_COMPOSE,
					EquipCommandList.DBCLICK_STONE_FU_COMPOSE
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW: 
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"StoneCompose"});
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					this.viewUI.mouseEnabled=false;
					this.basePanel=new PanelBase(viewUI,this.viewUI.width+8,this.viewUI.height+12);
					basePanel.addEventListener(Event.CLOSE, panelCloseHandler);
					basePanel.x = UIConstData.DefaultPos1.x;
					basePanel.y = UIConstData.DefaultPos1.y;
					basePanel.SetTitleTxt("宝石合成");
					this.showShop();
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					if(dataProxy.StoneCompIsOpen) panelCloseHandler(null);
					break;
				case EventList.STONE_COMPOSE_UI:
					if(dataProxy.StoneCompIsOpen) return;
					facade.registerCommand(ComposeCommand.NAME,ComposeCommand);
					EquipDataConst.getInstance().openPanel(1);
					dataProxy.StoneCompIsOpen = true;
					initUI();
					break;
				case EquipCommandList.COMPOSE_STONE:
					var dragItem:Object = notification.getBody().useItem;
					index = notification.getBody().index;
					checkItem(dragItem);
					break;
				case EquipCommandList.COMPOSE_STONEFU:
					var fuItem:Object = notification.getBody();
					checkFu(fuItem);
					break;
				case EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE:		//转场景时关闭面板
					if(dataProxy.StoneCompIsOpen){
						panelCloseHandler(null);
					}
					break;
				case EquipCommandList.DBCLICK_STONE_COMPOSE:
					var num:int = getIndex();
					if ( num == -1 )
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"已达到合成最高数量", color:0xffff00});
						return;
					}
					else
					{
						var dragItemDB:Object = notification.getBody().useItem;
						index = num;
						checkItem(dragItemDB);
					}
					break;
				case EquipCommandList.DBCLICK_STONE_FU_COMPOSE:
					if ( hasFu )
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"已放入宝石合成符", color:0xffff00});
						return;
					}
					else
					{
						var fuItemDB:Object = notification.getBody();
						checkFu(fuItemDB);
					}
					break;
			}
		}
		
		private function initUI():void
		{
			initData();
			showPercent();
			showMoney();									//显示钱
	
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);

			this.viewUI.btn_compose.addEventListener(MouseEvent.CLICK,composeHandler);
			this.viewUI.btn_cancel.addEventListener(MouseEvent.CLICK,cancelHandler);
			
			for(var i:uint = 0;i<5;i++)
			{
				this.viewUI["compose_"+i].addEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
				this.viewUI["compose_"+i].addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			}
			
			this.viewUI.getChildByName("aFu_0").addEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
			this.viewUI.getChildByName("aFu_0").addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
		}
		
		private function initData():void
		{
			currentType = "0";
			needMoney = 0;
			numStone = 0;
			hasFu = false;
			aFull = [false,false,false,false,false];
			maxLevel = 9;
			aStoneId = [];
			allItem = [];
			aStoneBind = [];
			fuId = 0;
			percent = 0;
			currentMoney = GameCommonData.Player.Role.BindMoney;
			viewUI.suiMoney_mc.txtMoney.textColor = 0xffffff;
			viewUI.yinMoney_mc.txtMoney.textColor = 0xffffff;
			itemContainer = new Sprite();
			this.viewUI.addChild(itemContainer);
			this.itemContainer.mouseChildren = false;
			this.itemContainer.mouseEnabled = false;
		}
		
		private function showShop():void{
			 var arr:Array=UIConstData.MarketGoodList[12] as Array;
			 var len:uint=arr.length;
			 for(var i:int=0;i<len;i++){
			 	var face:FaceItem=new FaceItem(arr[i].type);
			 	face.setEnable(true);
			 	face.name="goodQuickBuy_"+arr[i].type;
			 	if(this.viewUI["shopItem_"+i].mc_goodIcon.numChildren>0){
			 		this.viewUI["shopItem_"+i].mc_goodIcon.removeChildAt(0);
			 	}
			 	this.viewUI["shopItem_"+i].mc_goodIcon.addChild(face);
			 	this.viewUI["shopItem_"+i].txt_goodName.text=arr[i].Name;
			 	this.viewUI["shopItem_"+i].txt_goodName.mouseEnabled=false;
			 	this.viewUI["shopItem_"+i].txt_goodPrice.text=arr[i].PriceIn;
			 	this.viewUI["shopItem_"+i].txt_goodPrice.mouseEnabled=false;
			 	(this.viewUI["shopItem_"+i].btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
			 }		
		}
		
		/**
		 *  购买商品 
		 * @param e
		 * 
		 */		
		private  function buyGoodClickHandler(e:MouseEvent):void{
//			trace(e.currentTarget.parent.name); 
			var arrShop:Array=UIConstData.MarketGoodList[12] as Array;
			var arr:Array=e.currentTarget.parent.name.split("_");
			if(arrShop.length>arr[1]){
				sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:arrShop[arr[1]].type});	
			}
		}
		private function checkItem(dragItem:Object):void
		{
			var tp:String = dragItem.type.toString();
			//如果拖进来的不是同级别的同类宝石，提示信息
			if(currentType == "0")
			{
				if(tp.substr(0,2) == "41" || tp.substr(0,2) == "42")
				{
					if(tp.length >= 6)
					{
						currentLevel = uint(tp.substr(5,1));
//						trace("currentLevel ==" +currentLevel);
					}
					if(currentLevel < 9)
					{
						currentType = tp;
						addItem(dragItem);
						countMoney();
					}else if(currentLevel == maxLevel)
					{
						showHint(dragItem,"已经是最高等级了，不需要合成！");
						return;
					}		
				}else
				{
					showHint(dragItem,"放入宝石的类型不正确！");
					return;
				}
			}else											//格子里已经有东西了
			{
				if(!aFull[index])
				{
					if(tp != currentType)
					{
						showHint(dragItem,"只有同种类同等级的宝石才能合成！");
						return;
					}else
					{
						addItem(dragItem);
						countMoney();
					}
				}else
				{
//					trace("不好意思，这个格子里有东西了");
//					dragItem.IsLock=false;
					sendNotification(EventList.BAGITEMUNLOCK,dragItem.id);
					return;
				}
			}
		}
		
		private function addItem(dragItem:Object):void
		{
			var faceItem:UseItem = new UseItem(dragItem.index, dragItem.type,  viewUI);
			faceItem.Id = dragItem.id;
			faceItem.Type = dragItem.type;
			faceItem.IsBind = dragItem.isBind;
			faceItem.IsLock = false;
			faceItem.name="composeItem_"+dragItem.Id;
//			faceItem.mouseEnabled = false;
//			faceItem.mouseChildren = false;
//			faceItem.setEnable(false);
			var container:MovieClip=this.viewUI.getChildByName("compose_"+index) as MovieClip;
//			container.addEventListener(MouseEvent.CLICK,onContainerMouseClickHandler);
//			this.viewUI.addChild(faceItem);
			this.itemContainer.addChild(faceItem);
			faceItem.x=container.x +2;
			faceItem.y=container.y + 2;
			aFull[index] = true;
			container.addEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
			container.addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			
			numStone++;
			allItem.push(faceItem);
			aStoneBind.push(faceItem.IsBind);
			aStoneId.push(faceItem.Id);
			showPercent();
		}
		
		private function countMoney():void
		{
//			needMoney = EquipDataConst.aComposeMon[currentLevel];
			needMoney = 5000;
			showMoney();
			if(GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney < needMoney)
			{
				trace("钱不够了怎么办呢");
				viewUI.yinMoney_mc.txtMoney.textColor = 0xff0000;
				viewUI.suiMoney_mc.txtMoney.textColor = 0xff0000;
			}
		}
		
		private function showMoney():void
		{
			viewUI.suiMoney_mc.txtMoney.text = UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]);
			ShowMoney.ShowIcon(viewUI.suiMoney_mc, viewUI.suiMoney_mc.txtMoney,false);
			viewUI.yinMoney_mc.txtMoney.text = UIUtils.getMoneyStandInfo(currentMoney, ["\\ce","\\cs","\\cc"]);
			ShowMoney.ShowIcon(viewUI.yinMoney_mc, viewUI.yinMoney_mc.txtMoney,false);
			viewUI.needMoney_mc.txtMoney.text = UIUtils.getMoneyStandInfo(needMoney, ["\\se","\\ss","\\sc"]);
			ShowMoney.ShowIcon(viewUI.needMoney_mc, viewUI.needMoney_mc.txtMoney,false);
		}
		
		private function showPercent():void
		{
			if(numStone < 3)
			{
				percent = 0;
			}else
			{
				percent = (numStone-2)*25;
				if(hasFu)
				{
					percent += 25;
				}
			}
			viewUI.txt_successful.htmlText = "成功率："+percent + "%";
		}
		
		//处理放宝石合成符
		private function checkFu(item:Object):void
		{
//			trace(item.type);
			if(currentType == "0")
			{
				showHint(item,"请先放入宝石！");
			}else
			{
				if(item.type == 610016)
				{
					if(currentLevel > 3)
					{
						showHint(item,"3级以上的宝石需要高级宝石合成符才能合成！");
					}else
					{
						addFuItem(item);
					}
				}else if(item.type == 610017)
				{
					addFuItem(item);
				}else
				{
					showHint(item,"合成符的类型不正确！");
					return;
				}
			}
		}
		
		private function addFuItem(item:Object):void
		{
			var faceItem:UseItem = new UseItem(item.index, item.type,  itemContainer);
			faceItem.Id = item.id;
			faceItem.Type = item.type;
			faceItem.IsBind = item.isBind;
			faceItem.IsLock = false;
			
			var container:MovieClip=this.viewUI.getChildByName("aFu_0") as MovieClip;
//			container.addEventListener(MouseEvent.CLICK,onContainerMouseClickHandler);
			this.itemContainer.addChild(faceItem);
//			container.addChild(faceItem);
			faceItem.x=container.x+2;
			faceItem.y=container.y+2;
			
			hasFu = true;
			allItem.push(faceItem);
			fuId = faceItem.Id;
			showPercent();
		}
		
		protected function onMouseOverHandler(e:MouseEvent):void
		{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.UseFrame(mc,"RedFrame");
		}
		
		protected function onMouseOutHandler(e:MouseEvent):void
		{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.RemoveFrame(mc.parent,"RedFrame");
		}
		
		private function showHint(item:Object, hint:String):void
		{
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:hint, color:0xffff00});
//			facade.sendNotification(EventList.SHOWALERT,{comfrim:new Function(),cancel:null,info:hint});
//			item.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,item.id);
		}
		//这里开始合成
		private function composeHandler(evt:MouseEvent):void
		{
			if(currentType == "0")
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先放入宝石", color:0xffff00});
				return;
			}
			
			if(aStoneId.length < 3)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"放入宝石的个数不正确，不能进行合成", color:0xffff00});
				return;
			}
			if(GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney < needMoney)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你没有足够的金钱，不能进行合成", color:0xffff00});
				return;
			}
			
			//判断是否有绑定的宝石
			for(var i:uint = 0;i < aStoneBind.length;i++)
			{
				if(aStoneBind[i] == 1)
				{
					sendNotification(EventList.SHOWALERT,{comfrim:sendStoneData,cancel:new Function(),info:"待合成宝石中有绑定的，合成后的新宝石也将绑定，是否继续？"});
					return;
				}
			}
			sendStoneData();
		}
		
		private function sendStoneData():void
		{
			facade.sendNotification(ComposeCommand.NAME,{idStoneArr:aStoneId,idFu:fuId});
			needMoney = 0;
			currentMoney -= needMoney;
			showMoney();
			removeSomeThing();
			initData();
			showPercent();
		}
		
		private function cancelHandler(evt:MouseEvent):void
		{
			panelCloseHandler(null);
		}
			
		private function cleanItems():void
		{
			if(allItem && allItem.length > 0)
			{
				for(var j:uint = 0;j < allItem.length;j++)
				{
					var item:Object = allItem[j];
//					item.IsLock=false;
					sendNotification(EventList.BAGITEMUNLOCK,int(item.Id));
					item = null;
				}
			}
		}
		
		//双击取一个序列号
		private function getIndex():int
		{
			for ( var i:uint=0; i<5; i++ )
			{
				if ( !aFull[i] )
				{
					return i;
				}
			}
			return -1;
		}
		
		private function panelCloseHandler(evt:Event):void
		{
			removeSomeThing();
			dataProxy.StoneCompIsOpen = false;
			
			for(var i:uint = 0;i < 5;i++)
			{
				var container:MovieClip=this.viewUI.getChildByName("compose_"+i) as MovieClip;
				container.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
				container.removeEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			}
			this.viewUI.getChildByName("aFu_0").removeEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
			this.viewUI.getChildByName("aFu_0").removeEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			
			if(basePanel && GameCommonData.GameInstance.GameUI.contains(this.basePanel))
			{
				GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			}
			this.viewUI.btn_compose.removeEventListener(MouseEvent.CLICK,composeHandler);
			this.viewUI.btn_cancel.removeEventListener(MouseEvent.CLICK,cancelHandler);
			EquipDataConst.getInstance().closePanel(1);
		}
		
		private function removeSomeThing():void
		{
			cleanItems();	
			if(itemContainer && this.viewUI.contains(itemContainer))
			{
				viewUI.removeChild(itemContainer);
				itemContainer = null;
			}
			
		}
		
	}
}