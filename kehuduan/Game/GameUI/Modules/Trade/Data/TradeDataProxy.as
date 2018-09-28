package GameUI.Modules.Trade.Data
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class TradeDataProxy extends Proxy
	{
		public static const NAME:String = "TradeDataProxy";
		
		private var gridList:Array = new Array();
		
		private var gridOpList:Array = new Array();		
		
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public var moneyOp:uint 	  = 0;					/** 对方金钱 */
		public var moneySelf:uint  	  = 0;					/** 己方金钱 */
		public var goodOpList:Array   = new Array(5);		/** 对方物品列表 */
		//public var goodSelfList:Array = new Array(5);		/** 己方物品列表 */
//		
//		public var petOpList:Array 	  = [];					/** 对方交易的宠物列表 */
//		public var petSelfList:Array  = [];					/** 己方交易的宠物列表 */
//		public var petList:Array	  = [];					/** 自己的宠物列表 */
//		public var petTradeSelected:Object = new Object();	/** 当前选中的交易栏中宠物 */
//		public var petListSelected:Object  = new Object();	/** 当前选中的宠物列表中宠物 */

		public var goodPetOperating:Object = new Object();	/** 正在进行加减的物品或宠物 (数据缓存保证安全)*/
		public var opLocked:Boolean = false;				/** 对方锁定状态 */
		public var sfLocked:Boolean = false;				/** 自己锁定状态 */
		
		public function TradeDataProxy(list:Array, listOp:Array)
		{
			super(NAME);
			gridList = list;
			gridOpList = listOp;
			init();
		}
		
		private function init():void
		{
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			redFrame.name = "redFrame";
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
			yellowFrame.name = "yellowFrame";
		
			for( var i:int = 0; i<gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				
				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
				
				//////////////
				//对方物品监听鼠标事件
//				var gridUintOp:GridUnit = gridOpList[i] as GridUnit;
//				gridUintOp.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
//				gridUintOp.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
//				gridUintOp.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
//				
//				gridUintOp.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
//			if(BagData.SelectedItem)
//			{
//				if(event.currentTarget.name.split("_")[1] == BagData.SelectedItem.Index) return;
//			}
			event.currentTarget.parent.addChild(redFrame);
			redFrame.x = event.currentTarget.x;
			redFrame.y = event.currentTarget.y; 		
		}
		
		private function onMouseOut(event:MouseEvent):void
		{
    		if(event.currentTarget.parent.getChildByName("redFrame")) 
    		{
    			event.currentTarget.parent.removeChild(event.currentTarget.parent.getChildByName("redFrame"));
    		}
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
//			trace("name = ", event.target.name);
			var count:int = event.currentTarget.parent.numChildren-1;
			 while(count)
			{
				if(event.currentTarget.parent.getChildByName("yellowFrame")) 
    			{
    				event.currentTarget.parent.removeChild(event.currentTarget.parent.getChildByName("yellowFrame"));
    			}
    			count--;
   			}
			var index:int = int(event.target.name.split("_")[1]);
//			trace("onMouseDown = ", gridList);
			var TmpgridList:Array = gridList
			
			if(gridList[index].Item)
			{		
				if(gridList[index].Item.IsLock) return;	
//				gridList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				TradeConstData.TmpIndex = gridList[index].Index;
				TradeConstData.SelectedItem = gridList[index];
				gridList[index].Item.onMouseDown();
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
				return;			
			}
			TradeConstData.SelectedItem = null;
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
//			trace("Type = ", e.Data.type);
			switch(e.Data.type)
			{
				case "bag":
					if(!sfLocked) {
						facade.sendNotification(EventList.GOBAGVIEW, e.Data.source.Id);
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_dat_tra_dra_1" ], color:0xffff00});       //交易已锁定，无法操作
					}
				break;
				default:
					returnItem(e.Data.source);
				break;
			}
		}
		
		/** 移除所有黄色、红色框 */
		public function removeAllFrames():void
		{
			for( var i:int = 0; i < 5; i++ ) 
			{
				if(TradeConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			TradeConstData.GridUnitList[i].Grid.parent.removeChild(TradeConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(TradeConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				TradeConstData.GridUnitList[i].Grid.parent.removeChild(TradeConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
			}
		}
		
		//放回到原来的位置    UIConstData.SelectedItem 
		private function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			TradeConstData.SelectedItem = TradeConstData.GridUnitList[source.Pos]; 	
		}
		
		private function doubleClickHandler(event:MouseEvent):void
		{
			
		}
		
		
		
	}
}