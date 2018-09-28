package GameUI.Modules.Unity.UnityUtils
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class ContributeGridManager extends Proxy
	{	
		public static const NAME:String = "NPCShopGridManager";
		
		public var updateSaleMoney:Function;
		
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function ContributeGridManager(list:Array, gridSprite:MovieClip)
		{
			super(NAME);
			this.gridList = list;
			this.gridSprite = gridSprite;
			init();
		}
		
		private function init():void
		{
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			redFrame.name = "redFrame";
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
			yellowFrame.name = "yellowFrame";
		    redFrame.mouseEnabled = false;
		    yellowFrame.mouseEnabled = false;
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			if(UnityConstData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == UnityConstData.SelectedItem.Index) return;
			}
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
			
			if(gridList[index].Item)
			{
				if(gridList[index].Item.IsLock) return;	
//				gridList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				UnityConstData.TmpIndex = gridList[index].Index;
				gridList[index].Item.onMouseDown();
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
				UnityConstData.SelectedItem = gridList[index];
				return;
			}
			UnityConstData.SelectedItem = null;
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":
					sendNotification(EventList.BAGITEMUNLOCK, e.Data.source.Id);
					removeItem(e.Data.source.Id);
					for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) {
						if(UnityConstData.goodSaleList[i].id == e.Data.source.Id) {
							UnityConstData.goodSaleList.splice(i, 1);
							break;
						}
					}
					if(updateSaleMoney != null) updateSaleMoney();
//					sendNotification(NPCShopEvent.UPDATENPCSALEMONEY);
					removeAllItem();
					showItems(UnityConstData.goodSaleList);
					break;
				default:
					returnItem(e.Data.source);
				break;
			}
		}
		
//		private function doubleClickHandler(e:MouseEvent):void
//		{
//			if(!UnityConstData.SelectedItem) return;
//			if(!UnityConstData.SelectedItem.Item) return;
//			var goodUnit:GridUnit = UnityConstData.SelectedItem;
//			var goodId:int = goodUnit.Item.Id;
//			removeItem(goodId);
//			for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) {
//				if(UnityConstData.goodSaleList[i].id == goodId) {
//					UnityConstData.goodSaleList.splice(i, 1);
//					break;
//				}
//			}
//			UnityConstData.SelectedItem = null;
//			//发送给背包解锁消息
//			sendNotification(EventList.BAGITEMUNLOCK, goodId);
//		}
		
		//放回到原来的位置 
		private function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			source.parent.addChild(yellowFrame);
			UnityConstData.SelectedItem = UnityConstData.GridUnitList[source.Pos]; 	
		}
		/**  初始化物品 */
		public function showItems(list:Array):void
		{
			if(UnityConstData.GridUnitList.length == 0) return;
			for(var j:int = 0; j < list.length; j++)
			{
				UnityConstData.GridUnitList[j].HasBag = true;
				if(list[j] == undefined) 
				{
					continue;
				}
				var useItem:UseItem = new UseItem(list[j].index, list[j].type, gridSprite);
				list[j].type < 300000 ? useItem.Num = 1 : useItem.Num = list[j].amount;
				useItem.x = UnityConstData.GridUnitList[j].Grid.x + 2;
				useItem.y = UnityConstData.GridUnitList[j].Grid.y + 2;
				useItem.Id = list[j].id;
				useItem.IsBind = list[j].isBind;
				useItem.Type = list[j].type;
				useItem.IsLock = false;
				UnityConstData.GridUnitList[j].Item = useItem;
				UnityConstData.GridUnitList[j].IsUsed = true;
				gridSprite.addChild(useItem);
			}
		}
		
		public function removeAllItem():void
		{
			var count:int = gridSprite.numChildren - 1;
			while(count >= 0)
			{
				if(gridSprite.getChildAt(count) is ItemBase)
				{
					var item:ItemBase = gridSprite.getChildAt(count) as ItemBase;
					gridSprite.removeChild(item);
					item = null;
				}
				count--;
			}
			for( var i:int = 0; i < 8; i++ ) 
			{
				if(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			UnityConstData.GridUnitList[i].Grid.parent.removeChild(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				UnityConstData.GridUnitList[i].Grid.parent.removeChild(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				UnityConstData.GridUnitList[i].Item = null;
				UnityConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		public function addItem(index:int):void
		{
			var useItem:UseItem = new UseItem(UnityConstData.goodSaleList[index].index, UnityConstData.goodSaleList[index].type, gridSprite);
			if(int(UnityConstData.goodSaleList[index].type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(UnityConstData.goodSaleList[index].type) >= 300000)
			{
				useItem.Num = UnityConstData.goodSaleList[index].amount;
			}
			useItem.x = UnityConstData.GridUnitList[index].Grid.x + 2;
			useItem.y = UnityConstData.GridUnitList[index].Grid.y + 2;
			useItem.Id = UnityConstData.goodSaleList[index].id;
			useItem.IsBind = UnityConstData.goodSaleList[index].isBind;
			useItem.Type = UnityConstData.goodSaleList[index].type;
			useItem.IsLock = false;
			UnityConstData.GridUnitList[index].Item = useItem;
			UnityConstData.GridUnitList[index].IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		public function removeItem(id:int):void
		{
			for( var i:int = 0; i < UnityConstData.GridUnitList.length; i++)
			{
				if(UnityConstData.GridUnitList[i].Item)
				{
					if(UnityConstData.GridUnitList[i].Item.Id == id)
					{
						removeChild(UnityConstData.GridUnitList[i].Item);
						UnityConstData.GridUnitList[i].Item = null;	
						UnityConstData.GridUnitList[i].IsUsed = false;
						if(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
			    		{
			    			UnityConstData.GridUnitList[i].Grid.parent.removeChild(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
			    		}
			    		if(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
		    			{
		    				UnityConstData.GridUnitList[i].Grid.parent.removeChild(UnityConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
		    			}
					}
				}
			}
		}
		
		private function removeChild(child:DisplayObject):void
		{
			if(gridSprite.contains(child))
			{
				gridSprite.removeChild(child);
			}
		}
		
		public function gc():void
		{
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.removeEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
			var des:*;
			while ( this.gridSprite.numChildren>0 )
			{
				des = gridSprite.removeChildAt( 0 );
				des = null;
			}
		}
		
		private function doubleClickHandler( evt:MouseEvent ):void
		{
			var index:int = int(evt.target.name.split("_")[1]);
//			var a:* = evt.target;
			
			if(gridList[index].Item)
			{
				var itemId:int = gridList[index].Item.Id;
				if(gridList[index].Item.IsLock) return;	
				
				sendNotification(EventList.BAGITEMUNLOCK, itemId );
					removeItem(gridList[index].Item.Id);
					for(var i:int = 0; i < UnityConstData.goodSaleList.length; i++) 
					{
						if(UnityConstData.goodSaleList[i].id == itemId ) 
						{
							UnityConstData.goodSaleList.splice(i, 1);
							break;
						}
					}
					if(updateSaleMoney != null) updateSaleMoney();
//					sendNotification(NPCShopEvent.UPDATENPCSALEMONEY);
					removeAllItem();
					showItems(UnityConstData.goodSaleList);
			}
		}
		
		
	}
}