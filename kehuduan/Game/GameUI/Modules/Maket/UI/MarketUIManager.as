package GameUI.Modules.Maket.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.Components.ItemList.ItemListBase;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.text.TextField;
	
	public class MarketUIManager
	{
		private var view:MovieClip;
//		private var listView:ListComponent = null;
		private var iScrollPane:UIScrollPane = null;
		private var goodsViewList:Array;
		private var pageArr:Array = new Array(); 
		
		public function MarketUIManager(view:MovieClip)
		{
			this.view = view;
			init();
		}
		
		private function init():void
		{
			for(var j:int = 0; j < 8; j++) {
				(view["mcPage_"+j] as MovieClip).buttonMode    = true;
				(view["mcPage_"+j] as MovieClip).useHandCursor = true;
			}
			if(GameCommonData.cztype == 0) {
				view.btnDeposit.visible = true;
				view.txtDeposit.visible = true;
				view.txtDeposit.mouseEnabled = false;
			} else {
				view.txtDeposit.visible = false;
				view.btnDeposit.visible = false;
			}
			
			goodsViewList = new Array();
			var i:int;
			var marketGoodsItem:MarketGoodsItem;
			for(i = 0;i < 15; i++)
			{
				marketGoodsItem = new MarketGoodsItem(i);
				goodsViewList.push(marketGoodsItem);
				view.addChild(marketGoodsItem);
			}
			ItemListBase.setLayout(goodsViewList,5,2,178,59,14,67,ItemListBase.LAYOUT_ROW)
			
			view.txtPageInfo.mouseEnabled = false;  
			
			if(GameCommonData.wordVersion == 2 || GameCommonData.couponsCanOpen == false)
			{
				view["mcPage_7"].visible = false; //禁用点卷按钮
				view["txtPage_7"].visible = false;
				pageArr.push( view["mcPage_8"].x );
				pageArr.push( view["mcPage_8"].y );
				pageArr.push( view["txtPage_8"].x );
				pageArr.push( view["txtPage_8"].y );
				view["mcPage_8"].x = view["mcPage_7"].x;
				view["mcPage_8"].y = view["mcPage_7"].y;
				view["txtPage_8"].x = view["txtPage_7"].x;
				view["txtPage_8"].y = view["txtPage_7"].y;
			}
			clearData();
			(view.txtPage_8 as TextField).mouseEnabled = false;
			if(view["txtPage_7"])
			{
				view["txtPage_7"].mouseEnabled = false;
			}
		}
		
		public function setCouponsOpen( bool:Boolean ):void
		{
			if( GameCommonData.wordVersion != 2 )
			{
				view["mcPage_7"].visible = bool;          //点卷按钮
				view["txtPage_7"].visible = bool;
				if( bool )
				{
					view["mcPage_8"].x = pageArr[0];
					view["mcPage_8"].y = pageArr[1];
					view["txtPage_8"].x = pageArr[2];
					view["txtPage_8"].y = pageArr[3];
				}
				else
				{
					view["mcPage_8"].x = view["mcPage_7"].x;
					view["mcPage_8"].y = view["mcPage_7"].y;
					view["txtPage_8"].x = view["txtPage_7"].x;
					view["txtPage_8"].y = view["txtPage_7"].y;
				}
			}
		}
		
		/** 清除当前页数据 */
		public function clearData():void
		{
			var i:int;
			var marketGoodsItem:MarketGoodsItem;
			for(i = 0; i < MarketConstData.curMaxGoodsNum; i++) {
				marketGoodsItem = goodsViewList[i] as MarketGoodsItem;
				marketGoodsItem.clear();
				
				for ( var j:uint=0; j<view.numChildren; j++ )
				{
					var des:DisplayObject = view.getChildAt( j );
					if ( des is MoneyItem && view.contains( des )  )
					{
						( des as MoneyItem ).clear();
						view.removeChild( des );
						des = null;
						break;
					}
				}
			}
			view.txtPageInfo.text = "";
		}
		
		/** 显示当前页数据 */
		public function showData():void
		{
			var i:int;
			var marketGoodsItem:MarketGoodsItem;
			
			for(i = 0; i < MarketConstData.curPageData.length; i++) {
				var good:Object = MarketConstData.curPageData[i];
				
				marketGoodsItem = goodsViewList[i] as MarketGoodsItem;
				marketGoodsItem.setdata(good);
				
			}
			setLayout()
		}
		
		/** 设置布局 */
		public function setLayout():void
		{
			
			if(0 == MarketConstData.curPageIndex)
			{
				ItemListBase.setLayout(goodsViewList,5,2,178,59,14,67,ItemListBase.LAYOUT_ROW);
			}
			else
			{
				ItemListBase.setLayout(goodsViewList,5,3,178,59,14,67,ItemListBase.LAYOUT_ROW);
			}
			var i:int;
			for(i = 10; i< 15; i++)
			{
				if(goodsViewList[i])
				{
					goodsViewList[i].visible = (0 == MarketConstData.curPageIndex) ? false : true; 
				}
			}
		}
		
		//显示打折商品信息
		public function showCountData():void
		{
//			trace ( "打折物品的数据" );
			if ( !MarketConstData.curPageData ) return;
			var oldMoneyItem:MoneyItem;
//			var shape:Shape 
			
			for ( var i:uint=0; i<MarketConstData.curPageData.length; i++ )
			{
				var good:Object = MarketConstData.curPageData[i];
				var color:uint = UIConstData.getItem(good.type).Color; 
				view["txtMarketGoodName_"+i].htmlText = '<font color="' + IntroConst.itemColors[color] + '">' + good.Name + '</font>'; 
				var priceStr:String = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
				view["mcMoney_"+i].txtMoney.text = priceStr;
				ShowMoney.ShowIcon(view["mcMoney_"+i], view["mcMoney_"+i].txtMoney, true);
				
//				trace ( view["mcMoney_"+i].x,"     "+view["mcMoney_"+i].y );
				if ( i%2 != 0 )
				{
					view["mcMoney_"+i].x = 422;
				}
				else
				{
					view["mcMoney_"+i].x = 236;
				}
				
				oldMoneyItem = new MoneyItem();
				oldMoneyItem.x = view["mcMoney_"+i].x - 111;
				oldMoneyItem.y = view["mcMoney_"+i].y-22;
				view.addChild( oldMoneyItem );
				if ( good.oldPrice )
				{
//					good.oldPrice = 999;
//					oldMoneyItem.update( UIUtils.getMoneyStandInfo( good.oldPrice, ["\\ce","\\cs","\\cc"] ) );
//					oldMoneyItem.updateHtml( "<font color='#ffffff'>"+good.oldPrice + "</font>\\ab" );
					oldMoneyItem.updateHtml( good.oldPrice + "\\ab" );
				}
			}
		}
		
		/** 更新显示钱 0-需花费，1-自己携带*/
		public function showMoney(type:int):void
		{
			if(type == 0) {
				
			} else {
				var ybSelf:Number = GameCommonData.Player.Role.UnBindRMB;	//自己的元宝
				var zbSelf:Number = GameCommonData.Player.Role.BindRMB;		//自己的珠宝
				var dqSelf:Number = GameCommonData.Player.Role.GiveAway;	//自己的点券
				view["mcHave_0"].txtMoney.text = ybSelf+"\\ab";
				view["mcHave_1"].txtMoney.text = zbSelf+"\\zb";
				if(GameCommonData.wordVersion != 2)
				{
					view["mcHave_2"].txtMoney.text = dqSelf+"\\dq";
				}
				ShowMoney.ShowIcon(view["mcHave_0"], view["mcHave_0"].txtMoney, true);
				ShowMoney.ShowIcon(view["mcHave_1"], view["mcHave_1"].txtMoney, true);
				if(GameCommonData.wordVersion != 2)
				{
					ShowMoney.ShowIcon(view["mcHave_2"], view["mcHave_2"].txtMoney, true);
				}
				
			}
		}
		
	}
}

