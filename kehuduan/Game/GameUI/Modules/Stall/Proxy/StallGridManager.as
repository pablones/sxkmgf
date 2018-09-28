package GameUI.Modules.Stall.Proxy
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class StallGridManager extends Proxy
	{
		public static const NAME:String = "StallGridManager";
		
		private var gridList:Array = new Array();
		private var gridSprite:Sprite;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function StallGridManager(list:Array, gridSprite:Sprite)
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
			redFrame.mouseEnabled = false;
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
			yellowFrame.name = "yellowFrame";
			yellowFrame.mouseEnabled = false;
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		public function addMouseDown():void
		{
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, enableMouseDown);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, disableMouseDown);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, enableMouseDown);
			}
		}
		
		public function removeMouseDown():void
		{
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, enableMouseDown);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, disableMouseDown);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, disableMouseDown);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			if(StallConstData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == StallConstData.SelectedItem.Index) return;
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
		
		private function enableMouseDown(event:MouseEvent):void
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
				//-------------
				var clickItem:Object = gridList[index].Item;
				if(event.ctrlKey)
				{
					if(UIConstData.getItem(clickItem.Type) == null) return;   
					var id:int = clickItem.Id;
					var type:int = clickItem.Type;
					var name:String = UIConstData.getItem(clickItem.Type).Name;
					var isBind:int = clickItem.IsBind;
					//当按下Ctrl键单击物品的时候，创建快速链接物品，目前只做装备的快速链接，以后需要再加上type区间
					//格式:id_type_name_玩家id_pos
					//id:物品ID，type:物品类型，name:物品名称, isBind:物品是否绑定
					var color:uint = 0;
					var obj:Object = BagData.getItemDataByIdInIntroConst(id);
					if(obj) {
						color = obj.color;
					} else {
						var itemData:Object = UIConstData.getItem(type); 
						if(itemData) {
							color = itemData.Color;
						}
					}
					
					if ( type>=250000 && type<300000 )					//魂魄的名字特殊处理
					{
						var soulVo:SoulVO = SoulData.SoulDetailInfos[ id ];
						if ( soulVo )
						{
							if ( soulVo.belong==1 )
							{
								name = "九阳之魄";
							}
							else
							{
								name = "九阴之魂";
							}
						}
					}
					
					if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
						facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<1_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_"+isBind+"_"+color+">");
					} else {
						facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<1_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_"+isBind+"_"+color+">");
					}
				}
				//-------------
//				gridList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				StallConstData.TmpIndex = gridList[index].Index;
				StallConstData.SelectedItem = gridList[index];
				sendNotification(StallEvents.REFRESHMONEY);		//刷新钱显示
				gridList[index].Item.onMouseDown();
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
				return;
			}
			StallConstData.SelectedItem = null;
			sendNotification(StallEvents.REFRESHMONEY);
		}
		
		private function disableMouseDown(event:MouseEvent):void {
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
//				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				//-------------
				var clickItem:Object = gridList[index].Item;
				if(event.ctrlKey)
				{
					if(UIConstData.getItem(clickItem.Type) == null) return;
					var id:int = clickItem.Id;
					var type:int = clickItem.Type;
					var name:String = UIConstData.getItem(clickItem.Type).Name;
					var isBind:int = clickItem.IsBind;
					var ownerId:int = 0; 
					
					for(var key:* in GameCommonData.SameSecnePlayerList) {
						var person:GameElementAnimal = GameCommonData.SameSecnePlayerList[key];
						if(person.Role.Name == StallConstData.stallOwnerName) {
							ownerId = person.Role.Id;
							break;
						}
					}
					//当按下Ctrl键单击物品的时候，创建快速链接物品，目前只做装备的快速链接，以后需要再加上type区间
					//格式:id_type_name_玩家id_pos
					//id:物品ID，type:物品类型，name:物品名称, isBind:物品是否绑定
					var color:uint = 0;
					var obj:Object = BagData.getItemDataByIdInIntroConst(id);
					if(obj) {
						color = obj.color;
					} else {
						var itemData:Object = UIConstData.getItem(type);
						if(itemData) {
							color = itemData.Color;
						}
					}
					if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
						facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<1_["+name+"]_"+id+"_"+type+"_"+ownerId+"_"+isBind+"_"+color+">");
					} else {
						facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<1_["+name+"]_"+id+"_"+type+"_"+ownerId+"_"+isBind+"_"+color+">");
					}
				}
				//-------------
				StallConstData.TmpIndex = gridList[index].Index;
				StallConstData.SelectedItem = gridList[index];
				sendNotification(StallEvents.REFRESHMONEY);		//刷新钱显示
//				gridList[index].Item.onMouseDown();
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
				return;
			}
			StallConstData.SelectedItem = null;
			sendNotification(StallEvents.REFRESHMONEY);
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":
 					facade.sendNotification(StallEvents.STALLTOBAG, e.Data.source.Id);
					break;
				default:
					returnItem(e.Data.source);
				break;
			}
		}
		
		//放回到原来的位置    UIConstData.SelectedItem 
		private function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			source.parent.addChild(yellowFrame);
			StallConstData.SelectedItem = StallConstData.GridUnitList[source.Pos]; 	
		}
		
		private function doubleClickHandler(event:MouseEvent):void
		{
			
		}
		
		/**  初始化物品 */
		public function showItems(list:Array):void
		{
			for(var i:int = 0; i<list.length; i++)
			{
				if(list[i] == undefined) 
				{
					continue;
				};
				var useItem:UseItem = new UseItem(list[i].index, list[i].type, gridSprite);
				list[i].type < 300000 ? useItem.Num = 1 : useItem.Num = list[i].amount;
				useItem.x = StallConstData.GridUnitList[list[i].index].Grid.x + 2;
				useItem.y = StallConstData.GridUnitList[list[i].index].Grid.y + 2;
				useItem.Id = list[i].id;
				useItem.IsBind = list[i].isBind;
				useItem.Type = list[i].type;
				useItem.IsLock = false;
				StallConstData.GridUnitList[list[i].index].Item = useItem;
				StallConstData.GridUnitList[list[i].index].IsUsed = true;
				gridSprite.addChild(useItem);
			}
		}
		
		public function removeAllItem():void
		{
			var count:int = gridSprite.numChildren - 1;
			while(count>=0)
			{
				if(gridSprite.getChildAt(count) is ItemBase)
				{
					var item:ItemBase = gridSprite.getChildAt(count) as ItemBase;
					gridSprite.removeChild(item);
					item = null;
				}
				count--;
			}
			for( var i:int = 0; i < 24; i++ ) 
			{
				if(StallConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			StallConstData.GridUnitList[i].Grid.parent.removeChild(StallConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(StallConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				StallConstData.GridUnitList[i].Grid.parent.removeChild(StallConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				StallConstData.GridUnitList[i].Item = null;
				StallConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		/** 移除所有黄色、红色框 */
		public function removeAllFrames():void
		{
			for( var i:int = 0; i < 24; i++ ) 
			{
				if(StallConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			StallConstData.GridUnitList[i].Grid.parent.removeChild(StallConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(StallConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				StallConstData.GridUnitList[i].Grid.parent.removeChild(StallConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
			}
		}
		
		public function lockGrids():void
		{
//			trace("StallGridManager lockGrids");
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.mouseEnabled = false;
			}
		}
		
		public function unLockGrids():void
		{
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.mouseEnabled = true;
			}
		}
		
	}
}





