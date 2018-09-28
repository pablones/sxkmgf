package GameUI.Modules.CompensateStorage.view
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.SetFrame;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	public class CompensateStorageItemViewManager implements CSViewManager
	{
		public static const STARTPOS:Point = new Point(6, 23);
		
		private var _parent:DisplayObjectContainer;
		private var itemView:MovieClip;
	
		public function CompensateStorageItemViewManager(parent:DisplayObjectContainer)
		{
			_parent = parent;
		}
		
		public function init():void
		{
			if( CompensateStorageData.domain.hasDefinition( "CompensateStorageItem" ) )
			{
				var CompensateStorageItem:Class = CompensateStorageData.domain.getDefinition( "CompensateStorageItem" ) as Class;
				itemView = new CompensateStorageItem();
			}
			initGrid();
//			if(!CompensateStorageData.itemList)
//			{
//				CompensateStorageData.itemList = BagData.NormalItemList;
//			}
			CompensateStorageData.isInitItemView = true;
		}

		public function show(object:Object = null):void
		{
			if(!CompensateStorageData.isInitItemView)
			{
				init();
			}
			if(!CompensateStorageData.isShowItemView)
			{
				_parent.addChildAt(itemView,0);
				CompensateStorageData.isShowItemView = true;
			}
			showItems(CompensateStorageData.itemList);
			addLis();
			CompensateStorageData.selectedId = 0;
		}
		
		/**  初始化格子 */
		private function initGrid():void
		{
			CompensateStorageData.gridBackList = new Array();
			CompensateStorageData.gridUnitList = new Array();
			for( var i:int = 0; i < CompensateStorageData.MAXNUM; i++ ) 
			{
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i % 5) + STARTPOS.x;
				gridUnit.y = (gridUnit.height) * int(i / 5) + STARTPOS.y;
//				gridUnit.name = "bag_" + i.toString();
				gridUnit.name = "";
				itemView.addChild(gridUnit);
				CompensateStorageData.gridBackList.push(gridUnit);
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = itemView;										//设置父级
				gridUint.Index = i;											//格子的位置		
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				CompensateStorageData.gridUnitList.push(gridUint);
			}		
		}
		
		/**  显示物品 */
		private function showItems(list:Array):void
		{			
			//移除所有界面上的物品	
			removeAllItem();
			
			if(CompensateStorageData.gridUnitList.length == 0) return;
			for(var i:int = 0; i< CompensateStorageData.MAXNUM ; i++)
			{
				CompensateStorageData.gridUnitList[i].HasBag = true;
				//无数据,背包为空
				if(list[i] == undefined) 
				{
					continue;
				}
				list[i].index = i;
				//添加物品
				addItem(list[i]);
			}
			//目前有锁定的物品则显示操作按钮，加外框		
			if(CompensateStorageData.selectedItem)
			{
				SetFrame.UseFrame(CompensateStorageData.selectedItem.Grid);
			}	
		}
		
		//添加物品，初始化格子数组,如果有物品在cd添加cd
		private function addItem(item:Object):void
		{
			var gList:Array = CompensateStorageData.gridUnitList;	
				
			var useItem:UseItem = new UseItem(item.index, "" + int(item.type)%10000000, itemView);
			if(int(item.type)%10000000 < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(item.type)%10000000 > 300000)
			{
				useItem.Num = item.nAmount;
			}
			CompensateStorageData.gridBackList[item.index].name = "CompensateStorageItem_" + item.type%10000000 + "_" + item.id;
			useItem.x = gList[item.index].Grid.x + 2;
			useItem.y = gList[item.index].Grid.y + 2;
			useItem.Id = item.id;
			useItem.IsBind = item.isBind;
			useItem.Type = item.type%10000000;
			gList[item.index].Item = useItem;
			gList[item.index].IsUsed = true;
			
			itemView.addChild(useItem);	
		}
		
		/**
		 * 做全刷新
		 * 移除所有的物品 
		 * 将所有的格子都初始化 
		 * */
		private function removeAllItem():void
		{
			var count:int = itemView.numChildren - 1;
			while(count >= 0)
			{
				if(itemView.getChildAt(count) is ItemBase)
				{
					var item:ItemBase = itemView.getChildAt(count) as ItemBase;
					itemView.removeChild(item);
					item=null;
				}
				count--;
			}
			SetFrame.RemoveFrame(itemView);
			for( var i:int = 0; i < CompensateStorageData.MAXNUM; i++ ) 
			{
				if(CompensateStorageData.gridUnitList[i] == null) continue;
				CompensateStorageData.gridBackList[i].name = "";
				CompensateStorageData.gridUnitList[i].Item = null;
				CompensateStorageData.gridUnitList[i].IsUsed = false;
			}
			
		}
		private function addLis():void
		{
			for( var i:int = 0; i< CompensateStorageData.MAXNUM; i++ )
			{
				var gridUint:GridUnit = CompensateStorageData.gridUnitList[i] as GridUnit;
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		public function removeLis():void
		{
			var gridUint:GridUnit;
			for( var i:int = 0; i < CompensateStorageData.MAXNUM; i++ )
			{
				gridUint = CompensateStorageData.gridUnitList[i] as GridUnit;
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.removeEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			if(CompensateStorageData.selectedItem)
			{
				if(event.currentTarget.name.split("_")[2] == CompensateStorageData.selectedId) return;
			}
			SetFrame.UseFrame(event.currentTarget as DisplayObject, "RedFrame");			
		}
		
		private function onMouseOut(event:MouseEvent):void
		{
    		SetFrame.RemoveFrame(event.currentTarget.parent, "RedFrame");
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
			var index:int = 0;
			for(index = 0; index < CompensateStorageData.gridUnitList.length; index++ )
			{
				if(event.currentTarget.name.length > 0 && event.target == CompensateStorageData.gridUnitList[index].Grid)
				{
					SetFrame.RemoveFrame(event.currentTarget.parent);
		   			SetFrame.RemoveFrame(event.currentTarget.parent, "RedFrame");
					SetFrame.UseFrame(CompensateStorageData.gridUnitList[index].Grid);
					CompensateStorageData.selectedId = int(event.currentTarget.name.split("_")[2]);
					CompensateStorageData.selectedItem = CompensateStorageData.gridUnitList[index];
					break;
				} 
			}
   			
		}
		
		private function doubleClickHandler(event:MouseEvent):void
		{
			CompensateStorageData.getOut();
		}
		
		public function close(event:Event = null):void
		{
			if(CompensateStorageData.isShowItemView)
			{
				_parent.removeChild(itemView);
				CompensateStorageData.isShowItemView = false;
				removeLis();
			}
		}
	}
}