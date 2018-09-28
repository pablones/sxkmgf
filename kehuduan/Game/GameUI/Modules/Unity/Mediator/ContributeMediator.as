package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Command.SendActionCommand;
	import GameUI.Modules.Unity.Data.*;
	import GameUI.Modules.Unity.UnityUtils.ComboBoxUtil;
	import GameUI.Modules.Unity.UnityUtils.ContributeGridManager;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ContributeMediator extends Mediator
	{
		public static const NAME:String = "ContributeMediator";
		
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var contributeView:MovieClip;
		private var listView:ListComponent;
		private var iscomboBoxShow:Boolean;
		private var gridSprite:MovieClip;
		private var contributeType:int = 0;							/** 当前捐献类型,默认为木材 */
		private var contributeGridManager:ContributeGridManager;	/** 格子管理 */
		private var gridMoney:Number = 0;						 	/** 物品增加的金钱 */
		private var addMoney:Number =0;								/** 捐献的金钱 */
		private var comboBoxUtil:ComboBoxUtil;
		
		public function ContributeMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			 		EventList.INITVIEW,
			       	UnityEvent.SHOWCONTRIBUTEVIEW,
			       	UnityEvent.CLOSECONTRIBUTETVIEW,
			       	UnityEvent.BAGTOCONTRIBUTE
//			       	UnityEvent.UPDATECDATA
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					contributeView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ContributeView");
					panelBase = new PanelBase(contributeView, contributeView.width -14, contributeView.height+12);
					panelBase.name = "ApplyUnityList";
					panelBase.x = UIConstData.DefaultPos2.x - 500;
					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_con_han_1" ] ); // 捐献
					if(contributeView != null)
					{
						contributeView.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWCONTRIBUTEVIEW:
					UnityConstData.contributeIsOpen = true;
			    	viewInit();
					showContribute();
			    	addLis();
					facade.sendNotification(EventList.SHOWBAG);
		    	break;
		    	
		    	case UnityEvent.CLOSECONTRIBUTETVIEW:
		    		gcAll();
		    	break;
		    	case UnityEvent.BAGTOCONTRIBUTE:
					var goodToSale:Object = notification.getBody();
					var mask:uint;
					var maskObj:Object = UIConstData.getItem(goodToSale.type);
					if(maskObj!=null){
						mask=maskObj.Monopoly & 0x20;	
					}
					if(UnityConstData.goodSaleList.length >= 8) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_con_han_2" ], color:0xffff00});  // 一次最多捐献8件物品
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);
					}
					else if(String(goodToSale.type).indexOf("62") == 0)			//不能是任务物品 
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_con_han_3" ], color:0xffff00});  // 不能捐献任务物品
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);//解锁
					} 
					else if(goodToSale.isBind == 2)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_con_han_4" ], color:0xffff00});  // 不能捐献魂印物品
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);//解锁
					}
					else if(mask > 0)				//不能交易的物品
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_con_han_5" ], color:0xffff00});  // 此物品不能捐献
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);//解锁
					}
					else {
						UnityConstData.goodSaleList.push(goodToSale);
						updateSaleData();
					}
				break;
		    	
//		    	case UnityEvent.UPDATEAPPLYDATA:
//		    	break;
			}
		}
		
		private function showContribute():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			UnityConstData.contributeIsOpen = true;
			iscomboBoxShow					= false;
		}
		private function viewInit():void
		{
			showMoney(0 , true);
			(contributeView.txtMoney_2 as TextField).maxChars = 4;
			(contributeView.txtMoney_1 as TextField).maxChars = 2;
			(contributeView.txtMoney_0 as TextField).maxChars = 2;
			for(var i:int = 0;i < 3;i++)
			{
				contributeView["txtMoney_" + i].restrict = "0-9";
				contributeView["txtMoney_" + i].text = "0";
			}
			(contributeView.txtAddBuild as TextField).text 		= "0";
			(contributeView.txtAddBooming as TextField).text 	= "0";
			(contributeView.txtAddContribute as TextField).text = "0";
			gridSprite = new MovieClip();
			contributeView.addChild(gridSprite);
			gridSprite.x = 22;
			gridSprite.y = 158;
			initGrid();
		}
		private function addLis():void
		{
			 panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			 contributeView.btnSure.addEventListener(MouseEvent.CLICK,comfrimHandler);
			 for(var i:int = 0;i < 3;i++)
			 {
			 	UIUtils.addFocusLis(contributeView["txtMoney_" + i]);
				contributeView["txtMoney_" + i].addEventListener(MouseEvent.CLICK , focusInHandler); 
			 	contributeView["txtMoney_" + i].addEventListener(Event.CHANGE , inputMoneyHandler);
			 }
		}
		
		private function gcAll():void
		{
			UnityConstData.contributeIsOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			contributeView.btnSure.removeEventListener(MouseEvent.CLICK,comfrimHandler);
			for(var n:int = 0;n < 3;n++)
			 {
				contributeView["txtMoney_" + n].removeEventListener(MouseEvent.CLICK , focusInHandler); 
			 	contributeView["txtMoney_" + n].removeEventListener(Event.CHANGE , inputMoneyHandler);
			 }
			for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) {
				sendNotification(EventList.BAGITEMUNLOCK ,  UnityConstData.goodSaleList[i].id);			//物品解锁
			}
			UnityConstData.goodSaleList = [];
			UnityConstData.GridUnitList = [];
			contributeView.txtMoney_0.text = "0";
			contributeView.txtMoney_1.text = "0";
			contributeView.txtMoney_2.text = "0";
			this.gridMoney = 0;
			this.addMoney = 0;
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 点击确定按钮 */
		private function comfrimHandler(e:MouseEvent):void
		{
			saleGood();
		}
		/** 点击取消按钮 */
		private function cancelHandler(e:MouseEvent):void
		{
			facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);
		}
		/** 文本框获得焦点事件 */
		private function focusInHandler(e:MouseEvent):void
		{
			(e.currentTarget as TextField).setSelection(0 , (e.currentTarget as TextField).length);//(e.currentTarget as TextField).text.length -1);
		}
		/** 输入金钱事件 */
		private function inputMoneyHandler(e:Event):void
		{
			e.currentTarget.stage.focus=e.currentTarget;
			var type:int = e.currentTarget.name.split("_")[1];
			var typeArr:Array = [];
			if(e.currentTarget.length > 1 ) 
			{
				if(int(String(e.currentTarget.text).charAt(0)) == 0) 
				{
					e.currentTarget.text = String(e.currentTarget.text).slice(1);
				}
			}
			for(var i:int = 0;i < 3;i++)
			{
				if(i != type)
				{
					typeArr.push(i);
				}
			}
			showMoney(0 , true);
			this.addMoney = Number((e.currentTarget as TextField).text) * Math.pow(100 , type) + Number(contributeView["txtMoney_" + typeArr[0]].text) * Math.pow(100 , typeArr[0]) + Number(contributeView["txtMoney_" + typeArr[1]].text) * Math.pow(100 , typeArr[1]);
			showMoney(this.gridMoney + this.addMoney);
			//更新贡献繁荣建设
			updateUnityVaule();
		}
		/** 初始化格子 */
		private function initGrid():void
		{
			for(var i:int = 0; i < 8; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%8);
				gridUnit.y = (gridUnit.height) * int(i/8);
				gridUnit.name = "Contribute_" + i.toString();//
				gridSprite.addChild(gridUnit);	//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = gridSprite;								//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				UnityConstData.GridUnitList.push(gridUint);
			}
			contributeGridManager = new ContributeGridManager(UnityConstData.GridUnitList, gridSprite);
			contributeGridManager.updateSaleMoney = updateSaleMoney;
			facade.registerProxy(contributeGridManager);
		}
		
		/** 出售物品 */
		private function saleGood():void
		{
//			if()		
//			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"绑定物品不可捐献", color:0xffff00});
//				return;
//			}
			if(this.addMoney > 0)
			{
				if(UnityConstData.goodSaleList.length == 0)
				{
					facade.sendNotification(SendActionCommand.SENDACTION , {type:230 , data:[0, int(this.addMoney * 10 + 1)]});		//发送金钱,没有物品的情况
				}
				else facade.sendNotification(SendActionCommand.SENDACTION , {type:230 , data:[0, this.addMoney * 10 ]});			//发送金钱
//				UnityConstData.curContributeTotal = 1;											//发送总数为1
			}
			if(UnityConstData.goodSaleList.length > 0)
			{
//				UnityConstData.curContributeTotal += UnityConstData.goodSaleList.length;		//发送总数加上物品个数
				//发送出售物品命令  要出售的物品ID
				for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) {
					if( i == int(UnityConstData.goodSaleList.length - 1))
					{
						facade.sendNotification(SendActionCommand.SENDACTION , {type:230 , data:[1, UnityConstData.goodSaleList[i].id * 10 + 1]});	//发送最后一个物品
					}
					else facade.sendNotification(SendActionCommand.SENDACTION , {type:230 , data:[1, UnityConstData.goodSaleList[i].id * 10]});	//发送物品
				}
				for(var j:int = 0; j < UnityConstData.goodSaleList.length; j++){
					//解锁
					sendNotification(EventList.BAGITEMUNLOCK, UnityConstData.goodSaleList[j].id);
				}
				UnityConstData.goodSaleList = [];
				showMoney(0);
			}
		}
		
		/** 更新出售栏物品数据 */
		private function updateSaleData():void
		{
			var index:int = UnityConstData.goodSaleList.length - 1;
			contributeGridManager.addItem(index);
			//更新总售价
			updateSaleMoney();
			//更新贡献繁荣建设
			//updateUnityVaule();
		}
		
		/** 更新出售栏总钱数 */
		private function updateSaleMoney():void
		{
			gridMoney = 0;
			var moneyType:int =0;
			for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) {
				var price:Number = (UIConstData.getItem(UnityConstData.goodSaleList[i].type) ? UIConstData.getItem(UnityConstData.goodSaleList[i].type).PriceOut : 10) * 2;//普通商店价格的两倍
//				var price:Number = UIConstData.ItemDic[NPCShopConstData.goodSaleList[i].type].PriceOut;
				if(UnityConstData.goodSaleList[i].type >= 660000 && UnityConstData.goodSaleList[i].type < 670000)//如果是材料
				{
					continue;
				}
				//如果是可叠加物品，则 price = peice * amount
				if(UnityConstData.goodSaleList[i].type >= 300000)
					price = price * UnityConstData.goodSaleList[i].amount;
					gridMoney += price;
				if(String(UnityConstData.goodSaleList[i].type).indexOf("6500") == 0) moneyType = 1;
			}
			showMoney(0,true);
			showMoney(this.gridMoney + this.addMoney);
			//更新贡献繁荣建设
			updateUnityVaule();
		}
		/** 更新繁荣度，建设度，帮派贡献度 */
		private function updateUnityVaule():void
		{
			var txtAddBuild:int;
			var txtAddBooming:int;
			var txtAddContribute:int;
			txtAddContribute 	= int((this.gridMoney + this.addMoney)/1000);			//先处理金钱的贡献度
			for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) 
			{
				if(UnityConstData.goodSaleList[i].type >= 660000 && UnityConstData.goodSaleList[i].type < 670000)		//材料物品加 建设度 ，繁荣度
				{
					var price:Number = (UIConstData.getItem(UnityConstData.goodSaleList[i].type) ? UIConstData.getItem(UnityConstData.goodSaleList[i].type).PriceOut : 10)
					price = price * UnityConstData.goodSaleList[i].amount;
					txtAddBuild   	+= int(price * 10);
					txtAddBooming 	+= int(price * 10);
					txtAddContribute+= int((price * 10) /1000);
				}
			}
			
			contributeView.txtAddBuild.text 		= txtAddBuild;
			contributeView.txtAddBooming.text 		= txtAddBooming;
			contributeView.txtAddContribute.text 	= txtAddContribute;
		}
		/** 显示金钱 */
		public function showMoney(num:int , isclear:Boolean = false):void
		{
			contributeView.mcBind.txtMoney.text   = UIUtils.getMoneyStandInfo(num, ["\\se","\\ss","\\sc"]);					//初始化为0
			ShowMoney.ShowIcon(contributeView.mcBind, contributeView.mcBind.txtMoney , isclear);										//显示金钱
		}
	}
}