package GameUI.Modules.Depot.Proxy
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Depot.Mediator.ItemExtendsMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.DepotAction;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;
	
	/**
	 * 仓库物品格子数据管理
	 * @
	 * @
	 */ 
	public class ItemGridManager extends Proxy
	{
		public static const NAME:String = "ItemGridManager";
		
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		private var getOutTimer:Timer = new Timer(1000, 1);	//物品取出计时器
		
		public function ItemGridManager(list:Array, gridSprite:MovieClip)
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
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			if(DepotConstData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == DepotConstData.SelectedItem.Index) return;
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
			if(DepotConstData.GridUnitList[index].HasBag == false)
			{
				if(!DepotConstData.itemExtIsOpen)
				{
					var extendsMediator:ItemExtendsMediator = new ItemExtendsMediator();
					facade.registerMediator(extendsMediator);
					facade.sendNotification(DepotEvent.SHOWITEMEXT);
				}
				return;
			}
			if(gridList[index].Item)
			{
				if(gridList[index].Item.IsLock) return;
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
								name = GameCommonData.wordDic[ "mod_bag_pro_gridMa_onMouseDow_1" ];//"九阳之魄";
							}
							else
							{
								name = GameCommonData.wordDic[ "mod_bag_pro_gridMa_onMouseDow_2" ];//"九阴之魂";
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
				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				DepotConstData.TmpIndex = gridList[index].Index;
				DepotConstData.SelectedItem = gridList[index];
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
				gridList[index].Item.onMouseDown();
				return;
			}
			DepotConstData.SelectedItem = null;
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":				//拖向背包
					if(BagData.GridUnitList[e.Data.index].HasBag == false)
					{
						returnItem(e.Data.source);
						return;
					}
 					if(!startTimer()) return;
 					facade.sendNotification(DepotEvent.DEPOTTOBAG, {id:e.Data.source.Id, index:(e.Data.index + BagData.SelectIndex * 36)});
 					for( var i:int = 0; i < DepotConstData.GridUnitList.length; i++)
					{
						if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
			    		{
			    			DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
			    		}
			    		if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
		    			{
		    				DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
		    			}
					}
					DepotConstData.SelectedItem = null;
					break;
				case "depot":			//物品仓库内互换位置(拖到空位、互换位置)
					if(DepotConstData.GridUnitList[e.Data.index].HasBag == false)
					{
						returnItem(e.Data.source);
						return;
					}
					DroppedIsDepot(e.Data.index, e.Data.target, e.Data.source);
					break;
				default:
					returnItem(e.Data.source);
				break;
			}
		}
		
		private function DroppedIsDepot(index:int, target:MovieClip, source:UseItem):void
		{
			
			if(index == (source.Pos - 1 - DepotConstData.curDepotIndex * 36)) {		//未拖动到指定位置，不做任何处理  index是目标格子 下标
//				SetFrame.UseFrame(DepotConstData.GridUnitList[index].Grid);
				return;
			}
			if(!startTimer()) return;
			if(DepotConstData.GridUnitList[index].Item == null) {			//拖动到空位置    	(源物品ID，目标位置下标)
				DepotNetAction.sendDepotOrder(source.Id, 0, index+1+DepotConstData.curDepotIndex*36, DepotAction.MOVE_POS);
			} else {														//物品合并或互换位置	(源物品ID，目标位置下标)
				DepotNetAction.sendDepotOrder(DepotConstData.goodList[index].id, source.Id, 0, DepotAction.TRADE_POS);
			}
			for( var i:int = 0; i < DepotConstData.GridUnitList.length; i++)
			{
				if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
			}
			DepotConstData.SelectedItem = null;
		}
		
		//放回到原来的位置
		private function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			source.parent.addChild(yellowFrame);
			DepotConstData.SelectedItem = DepotConstData.GridUnitList[source.Pos-1-DepotConstData.curDepotIndex*36]; 	
		}
		
		private function doubleClickHandler(event:MouseEvent):void
		{
			var index:int = int(event.target.name.split("_")[1]);
			if(DepotConstData.GridUnitList[index].HasBag == false || !gridList[index].Item || gridList[index].Item.IsLock || !startTimer()) {
				return;
			}
			var item:Object = gridList[index].Item;
			facade.sendNotification(DepotEvent.DEPOTTOBAG, {id:item.Id, index:0});	//e.Data.source.Id    (e.Data.index + BagData.SelectIndex * 36)
		}
		
		/**  初始化物品 */
		public function showItems(list:Array):void
		{
			if(DepotConstData.GridUnitList.length == 0) return;
			
			var gridNum:int = getGridNum();
			var gridPageNum:int = getPageNum();
			
			if(gridPageNum == DepotConstData.curDepotIndex) {
				for(var i:int = 0; i < list.length; i++)
				{
					DepotConstData.GridUnitList[i].HasBag = true;
					
					if(i >= gridNum)
					{
						var noBag:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NoBag");
						noBag.x = DepotConstData.GridUnitList[i].Grid.x;
						noBag.y = DepotConstData.GridUnitList[i].Grid.y;
						noBag.mouseEnabled = false;
						noBag.mouseChildren= false;
						noBag.name = "noBag";
						DepotConstData.GridUnitList[i].HasBag = false;	
						DepotConstData.GridUnitList[i].Item = noBag;	
						gridSprite.addChild(noBag);
						continue;
					}
					if(list[i] == undefined) 
					{
						continue;
					}
					var useItem:UseItem = new UseItem(list[i].index, list[i].type, gridSprite);
					//
					if(list[i].type < 300000) {
						useItem.Num = 1;
					}
					else if(list[i].type > 300000) {
						useItem.Num = list[i].amount;
					}
					//
//					list[i].type < 300000 ? useItem.Num = 1 : useItem.Num = list[i].amount;
					useItem.x = DepotConstData.GridUnitList[i].Grid.x + 2;
					useItem.y = DepotConstData.GridUnitList[i].Grid.y + 2;
					useItem.Id = list[i].id;
					useItem.IsBind = list[i].isBind;
					useItem.Type = list[i].type;
					useItem.IsLock = false;
					DepotConstData.GridUnitList[i].Item = useItem;
					DepotConstData.GridUnitList[i].IsUsed = true;
					gridSprite.addChild(useItem);
				}
			} else {
				for(var j:int = 0; j < list.length; j++)
				{
					DepotConstData.GridUnitList[j].HasBag = true;
					if(list[j] == undefined) 
					{
						continue;
					}
					var useItem1:UseItem = new UseItem(list[j].index, list[j].type, gridSprite);
					list[j].type < 300000 ? useItem1.Num = 1 : useItem1.Num = list[j].amount;
					useItem1.x = DepotConstData.GridUnitList[j].Grid.x + 2;
					useItem1.y = DepotConstData.GridUnitList[j].Grid.y + 2;
					useItem1.Id = list[j].id;
					useItem1.IsBind = list[j].isBind;
					useItem1.Type = list[j].type;
					useItem1.IsLock = false;
					DepotConstData.GridUnitList[j].Item = useItem1;
					DepotConstData.GridUnitList[j].IsUsed = true;
					gridSprite.addChild(useItem1);
				}
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
			var count2:int = gridSprite.numChildren - 1;
			while(count2 >= 0)
			{
				if(gridSprite.getChildAt(count2).name == "noBag")
				{
					var noBag:MovieClip = gridSprite.getChildAt(count2) as MovieClip;
					gridSprite.removeChild(noBag);
					noBag = null;
				}
				count2--; 
			}
			for( var i:int = 0; i < 36; i++ ) 
			{
				if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				DepotConstData.GridUnitList[i].Item = null;
				DepotConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		public function addItem(index:int):void
		{
			var s:Array = DepotConstData.GridUnitList;
			var useItem:UseItem = new UseItem(DepotConstData.goodList[index].index, DepotConstData.goodList[index].type, gridSprite);
			if(int(DepotConstData.goodList[index].type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(DepotConstData.goodList[index].type) >= 300000)
			{
				useItem.Num = DepotConstData.goodList[index].amount;
			}
			useItem.x = DepotConstData.GridUnitList[index].Grid.x + 2;
			useItem.y = DepotConstData.GridUnitList[index].Grid.y + 2;
			useItem.Id = DepotConstData.goodList[index].id;
			useItem.IsBind = DepotConstData.goodList[index].isBind;
			useItem.Type = DepotConstData.goodList[index].type;
			useItem.IsLock = false;
			DepotConstData.GridUnitList[index].Item = useItem;
			DepotConstData.GridUnitList[index].IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		public function removeItem(id:int):void
		{
			for( var i:int = 0; i < DepotConstData.GridUnitList.length; i++)
			{
				if(DepotConstData.GridUnitList[i].Item)
				{
					if(DepotConstData.GridUnitList[i].Item.Id == id)
					{
						removeChild(DepotConstData.GridUnitList[i].Item);
						DepotConstData.GridUnitList[i].Item = null;	
						DepotConstData.GridUnitList[i].IsUsed = false;
						if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
			    		{
			    			DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
			    		}
			    		if(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
		    			{
		    				DepotConstData.GridUnitList[i].Grid.parent.removeChild(DepotConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
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
		
		public function lockGrids(val:Boolean):void
		{
			for( var i:int = 0; i < gridList.length; i++ )
			{
				var gridUint:GridUnit = gridList[i] as GridUnit;
				gridUint.Grid.mouseEnabled = val;
			}
		}
		
		public function getGridNum():int
		{
			var count:int = DepotConstData.gridCount;
			var num:int = 0;
			if(count > 72) {
				num = count - 72;
			} else if(count > 36) {
				num = count - 36;
			} else {
				num = count;
			}
			return num;
		}
		
		public function getPageNum():int
		{
			var count:int = DepotConstData.gridCount;
			var result:int = 0;
			if(count > 72) {
				result = 2;
			} else if(count > 36) {
				result = 1;
			} else {
				result = 0;
			}
			return result;
		}
		
		/** 取出计时器 */
		private function startTimer():Boolean
		{
			if(getOutTimer.running) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_pro_ite_sta" ], color:0xffff00});//"请稍后"
				return false;
			} else {
				getOutTimer.reset();
				getOutTimer.start();
				return true;
			}
		}
	}
}