package GameUI.Modules.PetPlayRule.PetSkillUp.Proxy
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnConstData;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpConstData;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpEvent;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.SkillItem;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PetSkillUpGridManager extends Proxy
	{
		public static const NAME:String = "PetSkillUpGridManager";
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetSkillUpGridManager(list:Array, gridSprite:MovieClip)
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
			if(PetSkillUpConstData.SelectedItem)
			{
//				if(event.currentTarget.name.split("_")[1] == PetSkillUpConstData.SelectedItem.Index) return;
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
				if(!(gridList[index].Item is SkillItem)) return;
				if(gridList[index].Item.IsLock) return;	
//				if(gridList[index].Item.Type == 3) {
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该技能已达最高级", color:0xffff00});
//					return;
//				} else {
//				if(gridList[index].Item.Type == 1) {
//					PetSkillUpConstData.breedCost = 5800;
//				} else if(gridList[index].Item.Type == 2) {
//					PetSkillUpConstData.breedCost = 50000;
//				}
//				sendNotification(PetSkillUpEvent.REFRESH_MONEY_COST_PET_SKILLUP, 0);
//				}
//				removeSkillSelect();
//				gridList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
//				gridList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
//				PetSkillUpConstData.TmpIndex = gridList[index].Index;
//				gridList[index].Item.onMouseDown();
				PetSkillUpConstData.SelectedItem = gridList[index];
				var skillId:uint = PetSkillUpConstData.SelectedItem.Item.Id;
				if(PetSkillLearnConstData.getSkillData(skillId)) {
					var skillData:Object = PetSkillLearnConstData.getSkillData(skillId);
					if(skillData.drugType > 0) {
						sendNotification(PetSkillUpEvent.SHOW_DRUG_PET_SKILLUP, skillData.drugType);
						yellowFrame.x = event.currentTarget.x;
						yellowFrame.y = event.currentTarget.y;
						event.currentTarget.parent.addChild(yellowFrame);
					} else {
						PetSkillUpConstData.SelectedItem = null;
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psu_pro_pets_onm_1" ], color:0xffff00}); // 该技能已经达到最高等级
						sendNotification(PetSkillUpEvent.REMOVE_DRUG_PET_SKILLUP);
					}
				}
//				addSkillSelect();
//				return;
			} else {
				PetSkillUpConstData.SelectedItem = null;
				sendNotification(PetSkillUpEvent.REMOVE_DRUG_PET_SKILLUP);
			}
		}
		
		/** 清除选中技能 */
		public function clearSelectSkill():void
		{
			for( var i:int = 0; i < 12; i++ ) 
			{
				if(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSkillUpConstData.GridUnitList[i].Grid.parent.removeChild(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSkillUpConstData.GridUnitList[i].Grid.parent.removeChild(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
			}
   			PetSkillUpConstData.SelectedItem = null;
		}
		
		/**  初始化格子数据 */
		public function showItems(list:Array):void
		{
			if(PetSkillUpConstData.GridUnitList.length == 0) return;
			if(PetPropConstData.isNewPetVersion)
			{
				for(var j:int = 0; j < 12; j++)
				{
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
						useItemNew.Id = list[j].gameSkill.SkillID;	//技能ID
						useItemNew.IsLock = false;
						useItemNew.Num = 1;
						useItemNew.Type = list[j].Level;	//技能等级
						useItemNew.IsLock = false;
					}
					useItemNew.x = PetSkillUpConstData.GridUnitList[j].Grid.x + 2;
					useItemNew.y = PetSkillUpConstData.GridUnitList[j].Grid.y + 2;
					PetSkillUpConstData.GridUnitList[j].Item = useItemNew;
					PetSkillUpConstData.GridUnitList[j].IsUsed = true;
					gridSprite.addChild(useItemNew as DisplayObject);
				}
			}
			else
			{
				for(var i:int = 0; i < list.length; i++)
				{
					PetSkillUpConstData.GridUnitList[i].HasBag = true;
					if(list[i] == undefined) 
					{
						continue;
					}
					var useItem:SkillItem = new SkillItem(list[i].gameSkill.SkillID, gridSprite);		//new UseItem(list[j].index, list[j].type, gridSprite);
					useItem.Num = 1;
					useItem.x = PetSkillUpConstData.GridUnitList[i].Grid.x + 2;
					useItem.y = PetSkillUpConstData.GridUnitList[i].Grid.y + 2;
					
					useItem.Id = list[i].gameSkill.SkillID;	//技能ID
	//				useItem.IsBind = list[j].isBind;
					useItem.Type = list[i].Level;	//技能等级
					
					useItem.IsLock = false;
					PetSkillUpConstData.GridUnitList[i].Item = useItem;
					PetSkillUpConstData.GridUnitList[i].IsUsed = true;
					gridSprite.addChild(useItem);
				}
			}
		}
		
		/** 移除所有技能格子数据 */
		public function removeAllItem():void
		{
//			var count:int = gridSprite.numChildren - 1;
//			while(count >= 0)
//			{
//				if(gridSprite.getChildAt(count) is ItemBase && gridSprite.getChildAt(count).name && String(gridSprite.getChildAt(count).name).indexOf("petSkillUpShow_") == 0)
//				{
//					var item:ItemBase = gridSprite.getChildAt(count) as ItemBase;
//					gridSprite.removeChild(item);
//					item = null;
//				}
//				count--;
//			} 
			for( var i:int = 0; i < 12; i++ ) 
			{
				if(PetSkillUpConstData.GridUnitList[i].Item) {
					if(PetPropConstData.isNewPetVersion)
					{
						gridSprite.removeChild(PetSkillUpConstData.GridUnitList[i].Item);
					}
					else
					{
						gridSprite.removeChild(PetSkillUpConstData.GridUnitList[i].Item as ItemBase);
					}
				}
				if(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSkillUpConstData.GridUnitList[i].Grid.parent.removeChild(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSkillUpConstData.GridUnitList[i].Grid.parent.removeChild(PetSkillUpConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				PetSkillUpConstData.GridUnitList[i].Item = null;
				PetSkillUpConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		/** 添加道具 */
		public function addItem():void
		{
			var useItem:UseItem = new UseItem(PetSkillUpConstData.itemData.index, PetSkillUpConstData.itemData.type, gridSprite);
			if(int(PetSkillUpConstData.itemData.type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(PetSkillUpConstData.itemData.type) >= 300000)
			{
				useItem.Num = PetSkillUpConstData.itemData.amount;
			}
			useItem.x = PetSkillUpConstData.GridItemUnit.Grid.x + 2;
			useItem.y = PetSkillUpConstData.GridItemUnit.Grid.y + 2;
			useItem.Id = PetSkillUpConstData.itemData.id;
			useItem.IsBind = PetSkillUpConstData.itemData.isBind;
			useItem.Type = PetSkillUpConstData.itemData.type;
			useItem.IsLock = false;
			PetSkillUpConstData.GridItemUnit.Item = useItem;
			PetSkillUpConstData.GridItemUnit.IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		/** 移除道具 */
		public function removeItem():void
		{
			if(PetSkillUpConstData.itemData && PetSkillUpConstData.GridItemUnit && PetSkillUpConstData.GridItemUnit.Item) {
				sendNotification(EventList.BAGITEMUNLOCK, PetSkillUpConstData.GridItemUnit.Item.Id);
				removeChild(PetSkillUpConstData.GridItemUnit.Item as ItemBase);
				PetSkillUpConstData.GridItemUnit.Item = null;
				PetSkillUpConstData.GridItemUnit.IsUsed = false;
				PetSkillUpConstData.itemData = null;
				if(PetSkillUpConstData.GridItemUnit.Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSkillUpConstData.GridItemUnit.Grid.parent.removeChild(PetSkillUpConstData.GridItemUnit.Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSkillUpConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSkillUpConstData.GridItemUnit.Grid.parent.removeChild(PetSkillUpConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame"));
    			}
			}
		}
		
		/** 添加选中技能 */
		public function addSkillSelect():void
		{
			var useItem:SkillItem = new SkillItem(PetSkillUpConstData.SelectedItem.Item.Id, gridSprite);		//new UseItem(list[j].index, list[j].type, gridSprite);
			useItem.Num = 1;
			useItem.x = PetSkillUpConstData.GridSkillUnit.Grid.x + 2;
			useItem.y = PetSkillUpConstData.GridSkillUnit.Grid.y + 2;
			
			useItem.Id = PetSkillUpConstData.SelectedItem.Item.Id;
//			useItem.IsBind = list[j].isBind;
			useItem.Type = PetSkillUpConstData.SelectedItem.Item.Id;
			
			useItem.IsLock = false;
			
			PetSkillUpConstData.GridSkillUnit.Item = useItem;
			PetSkillUpConstData.GridSkillUnit.IsUsed = true;
			
			gridSprite.addChild(useItem);
		}
		
		/** 移除选中技能 */
		public function removeSkillSelect():void
		{
			if(PetSkillUpConstData.SelectedItem && PetSkillUpConstData.GridSkillUnit && PetSkillUpConstData.GridSkillUnit.Item) {
//				removeChild(PetSkillUpConstData.GridSkillUnit.Item as ItemBase);
				PetSkillUpConstData.GridSkillUnit.Item = null;
				PetSkillUpConstData.GridSkillUnit.IsUsed = false;
//				sendNotification(EventList.BAGITEMUNLOCK, PetSkillUpConstData.GridSkillUnit.Item.Id);
//				PetSkillUpConstData.skillDataSelect = null;
				PetSkillUpConstData.SelectedItem = null;
				if(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSkillUpConstData.GridSkillUnit.Grid.parent.removeChild(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSkillUpConstData.GridSkillUnit.Grid.parent.removeChild(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("yellowFrame"));
    			}
			}
		}
		
//		/** 移除选中技能 */
//		public function removeSkillSelect():void
//		{
//			if(PetSkillUpConstData.SelectedItem && PetSkillUpConstData.GridSkillUnit && PetSkillUpConstData.GridSkillUnit.Item) {
//				sendNotification(EventList.BAGITEMUNLOCK, PetSkillUpConstData.GridSkillUnit.Item.Id);
//				removeChild(PetSkillUpConstData.GridSkillUnit.Item as ItemBase);
//				PetSkillUpConstData.GridSkillUnit.Item = null;
//				PetSkillUpConstData.GridSkillUnit.IsUsed = false;
//				PetSkillUpConstData.skillDataSelect = null;
//				PetSkillUpConstData.SelectedItem = null;
//				if(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("redFrame")) 
//	    		{
//	    			PetSkillUpConstData.GridSkillUnit.Grid.parent.removeChild(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("redFrame"));
//	    		}
//	    		if(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("yellowFrame")) 
//    			{
//    				PetSkillUpConstData.GridSkillUnit.Grid.parent.removeChild(PetSkillUpConstData.GridSkillUnit.Grid.parent.getChildByName("yellowFrame"));
//    			}
//			}
//		}
		
		
		private function removeChild(child:DisplayObject):void
		{
			if(gridSprite.contains(child))
			{
				gridSprite.removeChild(child);
			}
		}
	}
}