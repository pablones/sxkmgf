package GameUI.Modules.Maket.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Maket.Mediator.MarketPreviewMediator;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	/**
	 * 
	 * @author Administrator
	 * 商城中的物品列表项  包括名称 价格 
	 */	
	public class MarketGoodsItem extends MovieClip
	{
		private var itemView:MovieClip;
		private var _goods:Object;
		private var gridUnit:MovieClip;
		private var _count:int = 1;			//购买个数
		
		public function MarketGoodsItem(i:int)
		{
			itemView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MarketGoodsItem");
			itemView["mcMoney"].mouseEnabled = false;
			itemView["txtMarketGoodName"].mouseEnabled = false;
			itemView["txtMarketGoodName"].filters = Font.Stroke(0x000000);
			itemView["txt_Market_preview"].mouseEnabled = false;
			itemView["txt_Market_preview"].visible = false;
			itemView["btn_Market_preview"].visible = false;
			itemView.btnSub.visible = false;
			itemView.btnAdd.visible = false;
			itemView.txtCount.visible = false;
			addChild(itemView);

			//初始化物品图标格子
			gridUnit = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			gridUnit.name = "MarketSaleItem_" + i;
			gridUnit.x = 6;
			gridUnit.y = 8;
			addChild(gridUnit);											//添加到画面
			var gridUint:GridUnit = new GridUnit(gridUnit, true);
			gridUint.parent = this;										//设置父级
			gridUint.Index = i;											//格子的位置
			gridUint.HasBag = true;										//是否是可用的背包
			gridUint.IsUsed	= false;									//是否已经使用
			gridUint.Item	= null;										//格子的物品
			MarketConstData.GridUnitList.push(gridUint);
			this.count = 1;
		}
		
		public function setdata(good:Object):void
		{
			_goods = good;
			var color:uint = UIConstData.getItem(good.type).Color; 
			itemView["txtMarketGoodName"].htmlText = '<font color="' + IntroConst.itemColors[color] + '">' + good.Name + '</font>'; 
			var priceStr:String = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
			itemView["mcMoney"].txtMoney.text = priceStr;
			ShowMoney.ShowIcon(itemView["mcMoney"], itemView["mcMoney"].txtMoney, true);
			
			if(good.previewType != null)	//是否显示预览
			{
				itemView["txt_Market_preview"].visible = true;
				itemView["btn_Market_preview"].visible = true;
				itemView.btnSub.visible = false;
				itemView.btnAdd.visible = false;
				itemView.txtCount.visible = false;
				(itemView["btn_Market_preview"] as SimpleButton).addEventListener(MouseEvent.CLICK, onClick);
				removeLis();
			}
			else
			{
				itemView["txt_Market_preview"].visible = false;
				itemView["btn_Market_preview"].visible = false;
				itemView.btnSub.visible = true;
				itemView.btnAdd.visible = true;
				itemView.txtCount.visible = true;
				(itemView["btn_Market_preview"] as SimpleButton).removeEventListener(MouseEvent.CLICK, onClick);
				addLis();
			}
			this.count = 1;
		}
		
		private function onClick(event:MouseEvent):void
		{
			MarketPreviewMediator.location = new Point(event.stageX,event.stageY);
			GameCommonData.UIFacadeIntance.sendNotification(MarketEvent.SHOW_MARKET_PREVIEW ,_goods);
		}
		
		public function clear():void
		{
			removeLis();
			itemView["txt_Market_preview"].visible = false;
			itemView["btn_Market_preview"].visible = false;
			itemView.btnSub.visible = false;
			itemView.btnAdd.visible = false;
			itemView.txtCount.visible = false;
			(itemView["btn_Market_preview"] as SimpleButton).removeEventListener(MouseEvent.CLICK, onClick);
			itemView["txtMarketGoodName"].htmlText = ""; 
			itemView["mcMoney"].txtMoney.text = "";
			ShowMoney.ShowIcon(itemView["mcMoney"], itemView["mcMoney"].txtMoney, true);
		}
		
		private function addLis():void
		{
			itemView.btnSub.addEventListener(MouseEvent.CLICK, btnClickHandler);
			itemView.btnAdd.addEventListener(MouseEvent.CLICK, btnClickHandler);
			itemView.txtCount.addEventListener(Event.CHANGE, txtChangeHandler);
			UIUtils.addFocusLis(itemView.txtCount);
		}
		
		private function removeLis():void
		{
			itemView.btnSub.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			itemView.btnAdd.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			itemView.txtCount.removeEventListener(Event.CHANGE, txtChangeHandler);
			UIUtils.removeFocusLis(itemView.txtCount);
		}
		
		private function txtChangeHandler(e:Event):void
		{
			var index:uint = name.split("_")[1];
			var newCount:uint = uint(itemView.txtCount.text);
			count = Math.min(999 , newCount);
			count = Math.max(count , 1);
		}
		
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSub":									//减少数量
					if(count > 1) {
						count = count - 1;
					}
					break;
				case "btnAdd":									//增加数量
					if(count < 999) {
						count = count + 1;
					}
					break;
			}
		}
		
		public function set count(count:int):void
		{
			itemView.txtCount.text = count.toString();
			_count = count;
		}
		
		public function get count():int
		{
			return _count;
		}
	}
}