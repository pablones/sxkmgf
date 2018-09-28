package GameUI.Modules.NPCBusiness.Proxy
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessConstData;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessEvent;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class NPCBusinessGridManager extends Proxy
	{	
		public static const NAME:String = "NPCBusinessGridManager";
		
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function NPCBusinessGridManager(list:Array, gridSprite:MovieClip)
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
			if(NPCBusinessConstData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == NPCBusinessConstData.SelectedItem.Index) return;
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
				var displayObj:DisplayObject = event.target as DisplayObject;
				if(displayObj.mouseX <= 2 || displayObj.mouseX >= displayObj.width - 2){
					return;
				}
				if(displayObj.mouseY <= 2 || displayObj.mouseY >= displayObj.height - 2){
					return;
				}
//				gridList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				NPCBusinessConstData.TmpIndex = gridList[index].Index;
				gridList[index].Item.onMouseDown();
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
//				NPCBusinessConstData.SelectedItem = gridList[index];
//				var i:Object = NPCBusinessConstData.SelectedItem;
//				return;
			}
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":
					sendNotification(EventList.BAGITEMUNLOCK, e.Data.source.Id);
					removeItem(e.Data.source.Id);
					for(var i:int = 0; i < NPCBusinessConstData.goodSaleList.length; i++) {
						if(NPCBusinessConstData.goodSaleList[i].id == e.Data.source.Id) {
							NPCBusinessConstData.goodSaleList.splice(i, 1);
							break;
						}
					}
					sendNotification(NPCBusinessEvent.UPDATE_NPCBUSINESS_SALE_MONEY);
					removeAllItem();
					showItems(NPCBusinessConstData.goodSaleList);
					break;
				default:
					returnItem(e.Data.source);
				break;
			}
		}
		
		private function doubleClickHandler(e:MouseEvent):void
		{
			var index:int = int(e.target.name.split("_")[1]);
			if(!gridList[index].Item || gridList[index].Item.IsLock) {
				return;
			}
			var item:Object = gridList[index].Item;
			var goodId:int = item.Id;
			for(var i:int = 0; i < NPCBusinessConstData.goodSaleList.length; i++) {
				if(NPCBusinessConstData.goodSaleList[i].id == goodId) {
					NPCBusinessConstData.goodSaleList.splice(i, 1);
					break;
				}
			}
			removeItem(goodId);
			sendNotification(NPCBusinessEvent.UPDATE_NPCBUSINESS_SALE_MONEY);
			removeAllItem();
			showItems(NPCBusinessConstData.goodSaleList);
			//发送给背包解锁消息
			sendNotification(EventList.BAGITEMUNLOCK, goodId);
			facade.sendNotification(EventList.REMOVEITEMTOOLTIP);
		}
		
		//放回到原来的位置 
		private function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			source.parent.addChild(yellowFrame);
			NPCBusinessConstData.SelectedItem = NPCBusinessConstData.GridUnitList[source.Pos]; 	
		}
		/**  初始化物品 */
		public function showItems(list:Array):void
		{
			if(NPCBusinessConstData.GridUnitList.length == 0) return;
			for(var j:int = 0; j < list.length; j++)
			{
				NPCBusinessConstData.GridUnitList[j].HasBag = true;
				if(list[j] == undefined) 
				{
					continue;
				}
				var useItem:UseItem = new UseItem(list[j].index, list[j].type, gridSprite);
				list[j].type < 300000 ? useItem.Num = 1 : useItem.Num = list[j].amount;
				useItem.x = NPCBusinessConstData.GridUnitList[j].Grid.x + 2;
				useItem.y = NPCBusinessConstData.GridUnitList[j].Grid.y + 2;
				useItem.Id = list[j].id;
				useItem.IsBind = list[j].isBind;
				useItem.Type = list[j].type;
				useItem.IsLock = false;
				NPCBusinessConstData.GridUnitList[j].Item = useItem;
				NPCBusinessConstData.GridUnitList[j].IsUsed = true;
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
				if(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			NPCBusinessConstData.GridUnitList[i].Grid.parent.removeChild(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				NPCBusinessConstData.GridUnitList[i].Grid.parent.removeChild(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				NPCBusinessConstData.GridUnitList[i].Item = null;
				NPCBusinessConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		public function addItem(index:int):void
		{
			var useItem:UseItem = new UseItem(NPCBusinessConstData.goodSaleList[index].index, NPCBusinessConstData.goodSaleList[index].type, gridSprite);
			if(int(NPCBusinessConstData.goodSaleList[index].type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(NPCBusinessConstData.goodSaleList[index].type) >= 300000)
			{
				useItem.Num = NPCBusinessConstData.goodSaleList[index].amount;
			}
			useItem.x = NPCBusinessConstData.GridUnitList[index].Grid.x + 2;
			useItem.y = NPCBusinessConstData.GridUnitList[index].Grid.y + 2;
			useItem.Id = NPCBusinessConstData.goodSaleList[index].id;
			useItem.IsBind = NPCBusinessConstData.goodSaleList[index].isBind;
			useItem.Type = NPCBusinessConstData.goodSaleList[index].type;
			useItem.IsLock = false;
			NPCBusinessConstData.GridUnitList[index].Item = useItem;
			NPCBusinessConstData.GridUnitList[index].IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		public function removeItem(id:int):void
		{
			for( var i:int = 0; i < NPCBusinessConstData.GridUnitList.length; i++)
			{
				if(NPCBusinessConstData.GridUnitList[i].Item)
				{
					if(NPCBusinessConstData.GridUnitList[i].Item.Id == id)
					{
						removeChild(NPCBusinessConstData.GridUnitList[i].Item);
						NPCBusinessConstData.GridUnitList[i].Item = null;	
						NPCBusinessConstData.GridUnitList[i].IsUsed = false;
						if(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
			    		{
			    			NPCBusinessConstData.GridUnitList[i].Grid.parent.removeChild(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
			    		}
			    		if(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
		    			{
		    				NPCBusinessConstData.GridUnitList[i].Grid.parent.removeChild(NPCBusinessConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
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
		
		
	}
}