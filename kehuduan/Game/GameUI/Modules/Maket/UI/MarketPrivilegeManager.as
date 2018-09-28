package GameUI.Modules.Maket.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.text.TextField;

	public class MarketPrivilegeManager
	{
		private var marketPrivilegeView:MovieClip;
		private var privilegeGoods0:MovieClip;
		private var privilegeGoods1:MovieClip;
		private var privilegeGoods2:MovieClip;
		private var line1:MovieClip;
		private var line2:MovieClip;
		private var redLine1:Shape;
		private var redLine2:Shape;
		private var redLine3:Shape;
		private var _goodsList:Array;
		private var redFrame:MovieClip = null;
		private var useItemList:Array;
		
		public function MarketPrivilegeManager(view:MovieClip)
		{
			marketPrivilegeView = view;
			privilegeGoods0 = view["mc_market_privilege_1"];
			privilegeGoods1 = view["mc_market_privilege_2"];
			privilegeGoods2 = view["mc_market_privilege_3"];
			line1 = view["mc_market_line_1"];
			line2 = view["mc_market_line_2"];
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			redFrame.mouseEnabled = false;
			
			addGrid(privilegeGoods0,0);addGrid(privilegeGoods1,1);addGrid(privilegeGoods2,2);
		}
		
		public function addGrid(view:MovieClip,i:int):void
		{
			//初始化物品图标格子
			var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			gridUnit.name = "MarketPrivilegeItem_" + i;
			gridUnit.x = 5;
			gridUnit.y = 1;
			view.addChild(gridUnit);									//添加到画面
			var gridUint:GridUnit = new GridUnit(gridUnit, true);
			gridUint.parent = view;										//设置父级
			gridUint.Index = i;											//格子的位置
			gridUint.HasBag = true;										//是否是可用的背包
			gridUint.IsUsed	= false;									//是否已经使用
			gridUint.Item	= null;										//格子的物品
			MarketConstData.GridUnitList.push(gridUint);
		}
		
		public function updata():void
		{
			removeUseItem();
			hideAll();
			if(_goodsList[0])
			{
				setData(privilegeGoods0,_goodsList[0])
				drawLine(redLine1,privilegeGoods0["mcOldPrice"].txtMoney)
				privilegeGoods0.visible = true;
			}
			if(_goodsList[1])
			{
				setData(privilegeGoods1,_goodsList[1])
				drawLine(redLine2,privilegeGoods1["mcOldPrice"].txtMoney)
				privilegeGoods1.visible = true;
				line1.visible = true;
			}
			if(_goodsList[2])
			{
				setData(privilegeGoods2,_goodsList[2])
				drawLine(redLine3,privilegeGoods2["mcOldPrice"].txtMoney)
				privilegeGoods2.visible = true;
				line2.visible = true;
			}
		}
		
		public function setData(mc:MovieClip,good:Object):void
		{
			
			var color:uint = UIConstData.getItem(good.type).Color; 
			mc["txt_goodsName"].htmlText = '<font color="' + IntroConst.itemColors[color] + '">' + good.Name + '</font>'; 
		
			var oldPriceStr:String = good.oldPrice + MarketConstData.payWayStrList[good.PayType[0]];
			mc["mcOldPrice"].txtMoney.text = oldPriceStr;
			ShowMoney.ShowIcon(mc["mcOldPrice"], mc["mcOldPrice"].txtMoney, true);
			
			var newPriceStr:String = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
			mc["mcPriceIn"].txtMoney.text = newPriceStr;
			ShowMoney.ShowIcon(mc["mcPriceIn"], mc["mcPriceIn"].txtMoney, true);
			
			if(good.leftNum > 0)
			{
				mc["txt_leftNum"].htmlText = "<font color='#00FFFF'>" + GameCommonData.wordDic[ "Modules_Maket_UI_MarketPrivilegeManager_setData_1" ]/**"剩"*/ + good.leftNum + GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketGridManager_onMouseClick_4" ]/**"个"*/ + "</font>";
			}
			else
			{
				mc["txt_leftNum"].htmlText = "<font color='#FF0000'>" + GameCommonData.wordDic[ "Modules_Maket_UI_MarketPrivilegeManager_setData_1" ]/**"剩"*/ + "0" + GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketGridManager_onMouseClick_4" ]/**"个"*/ + "</font>";
			}
			useItemList = new Array();
			/** 添加图片 */
			var useItem:UseItem = new UseItem(good.index, good.type, mc);
			good.type < 300000 ? useItem.Num = 1 : useItem.Num = good.amount;
			useItem.x = 7;
			useItem.y = 3;
			useItem.Id = good.id;
			useItem.IsBind = good.isBind;
			useItem.Type = good.type;
			useItem.IsLock = false;
			mc.addChild(useItem);
			
			useItemList.push(useItem);
		}
		
		public function set goodsList(goodsList:Array):void
		{
			_goodsList = goodsList;
			updata();
		}
		
		public function get goodsList():Array
		{
			return _goodsList;
		}
		
		private function removeUseItem():void
		{
			var i:int;
			var useItem:UseItem;
			for(i = 0; useItemList && i < useItemList.length; i++ )
			{
				useItem = useItemList[i];
				useItem.parent.removeChild(useItem);
			}
			while(useItemList && useItemList.length) useItemList.pop();
			useItemList = null;
		}
		
		private function hideAll():void
		{
			privilegeGoods0.visible = false;
			privilegeGoods1.visible = false;
			privilegeGoods2.visible = false;
			line1.visible = false;
			line2.visible = false;
		}
		
		/** 画红线 */
		private function drawLine(redLine:Shape,txt:TextField):void
		{
			if(redLine == null) redLine = new Shape();
			var x1:Number = txt.width - txt.text.length * 6 - 4;
			var y1:Number = 2;
			var x2:Number = txt.width - 22;
			var y2:Number = 10;
			redLine.graphics.clear();
			redLine.graphics.lineStyle( 1.5 ,0xff0000 );
			redLine.graphics.moveTo(x1,y1);
			redLine.graphics.lineTo(x2,y2);
			txt.parent.addChildAt(redLine,txt.parent.numChildren - 1);
		}
	}
}