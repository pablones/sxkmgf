package GameUI.Modules.Pet.Proxy
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.SkillItem;
	
	import OopsEngine.Skill.GameSkillMode;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PetSkillGridManager extends Proxy
	{
		public static const NAME:String = "PetSkillGridManager";
		
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetSkillGridManager(list:Array, gridSprite:MovieClip)
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
//				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			if(PetPropConstData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == PetPropConstData.SelectedItem.Index) return;
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
		
		/**  初始化物品 */
		public function showItems(list:Array):void
		{
			if(PetPropConstData.GridUnitList.length == 0) return;
			if(PetPropConstData.isNewPetVersion)
			{
				for(var j:int = 0; j < 12; j++)
				{
					PetPropConstData.GridUnitList[j].HasBag = true;
					var useItemNew:Object;
					if(!list[j])
					{
						if(j < 10)
						{
							useItemNew = PetPropConstData.getExplainShow(true);
						}
						else
						{
							useItemNew = PetPropConstData.getExplainShow(false);
						}
					}
					else
					{
						useItemNew = new SkillItem(list[j].gameSkill.SkillID, gridSprite);		//new useItemNew(list[j].index, list[j].type, gridSprite);
						useItemNew.Id = list[j].gameSkill.SkillID;
						useItemNew.IsLock = false;
						useItemNew.Num = 1;
					} 
					useItemNew.name = "petSkillItem_"+j;
					useItemNew.x = PetPropConstData.GridUnitList[j].Grid.x + 2;
					useItemNew.y = PetPropConstData.GridUnitList[j].Grid.y + 2;
					PetPropConstData.GridUnitList[j].Item = useItemNew;
					PetPropConstData.GridUnitList[j].IsUsed = true;
					gridSprite.addChild(useItemNew as DisplayObject);
				}
			}
			else
			{
				for(var i:int = 0; i < list.length; i++)
				{
					PetPropConstData.GridUnitList[i].HasBag = true;
					if(list[i] == undefined) 
					{
						continue;
					}
	
					var useItem:SkillItem = new SkillItem(list[i].gameSkill.SkillID, gridSprite);		//new UseItem(list[j].index, list[j].type, gridSprite);
					useItem.name = "petSkillItem_"+i;
					useItem.Num = 1;
					useItem.x = PetPropConstData.GridUnitList[i].Grid.x + 2;
					useItem.y = PetPropConstData.GridUnitList[i].Grid.y + 2;
					
					useItem.Id = list[i].gameSkill.SkillID;
	//				useItem.IsBind = list[j].isBind;
	//				useItem.Type = list[j].Level;
					
					useItem.IsLock = false;
					PetPropConstData.GridUnitList[i].Item = useItem;
					PetPropConstData.GridUnitList[i].IsUsed = true;
					gridSprite.addChild(useItem);
				}
			}
		}
		
		
		public function removeAllItem():void
		{
			var count:int = gridSprite.numChildren - 1;
			while(count >= 0)
			{
				var item:DisplayObject = gridSprite.getChildAt(count);
				if(item.name.split("_")[0] == "petSkillItem")
				{
					gridSprite.removeChild(item);
					item = null;
				}
				count--;
			}
			for( var i:int = 0; i < 12; i++ ) 
			{
				if(PetPropConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetPropConstData.GridUnitList[i].Grid.parent.removeChild(PetPropConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetPropConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetPropConstData.GridUnitList[i].Grid.parent.removeChild(PetPropConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				PetPropConstData.GridUnitList[i].Item = null;
				PetPropConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		private function removeChild(child:DisplayObject):void
		{
			if(gridSprite.contains(child))
			{
				gridSprite.removeChild(child);
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
				if(!(gridList[index].Item is SkillItem)) return;
				if(gridList[index].Item.IsLock) return;	
//				gridList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
//				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
				if(!GameSkillMode.IsPetPull(PetPropConstData.gridSkillList[index].gameSkill.SkillMode)) { //判断技能是否可以拖动
					return;
				} else {
					var displayObj:DisplayObject = event.target as DisplayObject;
					if(displayObj.mouseX <= 2 || displayObj.mouseX >= displayObj.width - 2){
						return;
					}
					if(displayObj.mouseY <= 2 || displayObj.mouseY >= displayObj.height - 2){
						return;
					}
					gridList[index].Item.onMouseDown();
				}
				PetPropConstData.TmpIndex = gridList[index].Index;
				PetPropConstData.SelectedItem = gridList[index];
				event.currentTarget.parent.addChild(yellowFrame);
				yellowFrame.x = event.currentTarget.x;
				yellowFrame.y = event.currentTarget.y;
				return;
			}
			PetPropConstData.SelectedItem = null;
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "quickF":		//快捷栏
//					returnItem(e.Data.source);
//					if(e.Data.source.Type != 0) {
//						facade.sendNotification(EventList.DROPINQUICK, {target:e.Data.target, source:e.Data.source});
//					} else {
//						sendNotification(HintEvents.RECEIVEINFO, {info:"该技能是被动技能", color:0xffff00});
//					}
					break;
				default:
					returnItem(e.Data.source);
				break;
			}
		}
		
		private function doubleClickHandler(event:MouseEvent):void
		{
//			var index:int = int(event.target.name.split("_")[1]);
			if(!PetPropConstData.SelectedItem) return;
			//调用 使用宠物技能接口 传技能ID
		}
		
		private function returnItem(source:ItemBase):void
		{
			source.ItemParent.addChild(source);
			source.x = source.tmpX;
			source.y = source.tmpY;
			source.parent.addChild(yellowFrame);
			PetPropConstData.SelectedItem = PetPropConstData.GridUnitList[source.Pos]; 	
		}
		
		/** 加/解锁所有格子 */
		public function lockAllGrid(canOperate:Boolean):void
		{
			if(canOperate) {
				for( var i:int = 0; i < gridList.length; i++ ) {
					var gridUint:GridUnit = gridList[i] as GridUnit;
					if(!gridUint.Grid.hasEventListener(MouseEvent.MOUSE_OVER)) gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
					if(!gridUint.Grid.hasEventListener(MouseEvent.MOUSE_OUT)) gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
					if(!gridUint.Grid.hasEventListener(MouseEvent.MOUSE_DOWN)) gridUint.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
	//				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
				}
			} else {
				for( var j:int = 0; j < gridList.length; j++ ) {
					var gridUintR:GridUnit = gridList[j] as GridUnit;
					if(gridUintR.Grid.hasEventListener(MouseEvent.MOUSE_OVER)) gridUintR.Grid.removeEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
					if(gridUintR.Grid.hasEventListener(MouseEvent.MOUSE_OUT)) gridUintR.Grid.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
					if(gridUintR.Grid.hasEventListener(MouseEvent.MOUSE_DOWN)) gridUintR.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
	//				gridUint.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
				}
			}
		}
		
	}
}