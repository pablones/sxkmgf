package GameUI.Modules.PetPlayRule.PetSkillLearn.Proxy
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.SkillItem;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PetSkillLearnGridManager extends Proxy
	{
		public static const NAME:String = "PetSkillLearnGridManager";
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetSkillLearnGridManager(list:Array, gridSprite:MovieClip)
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
			if(PetSkillLearnConstData.SelectedItem)
			{
				if(event.currentTarget.name.split("_")[1] == PetSkillLearnConstData.SelectedItem.Index) return;
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
			if(!gridList[index].Item) return;
			PetSkillLearnConstData.NewSelectGrid = gridList[index];
			event.currentTarget.parent.addChild(yellowFrame);
			yellowFrame.x = event.currentTarget.x;
			yellowFrame.y = event.currentTarget.y; 
		}
		
		/**  初始化格子数据 */
		public function showItems(list:Array):void
		{
			if(PetSkillLearnConstData.GridUnitList.length == 0) return;
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
						useItemNew = new SkillItem(list[j].gameSkill.SkillID, gridSprite);		//new UseItem(list[j].index, list[j].type, gridSprite);
						useItemNew.Id = list[j].gameSkill.SkillID;	//技能ID
						useItemNew.IsLock = false;
						useItemNew.Num = 1;
						useItemNew.Type = list[j].Level;	//技能等级
						useItemNew.IsLock = false;
					}
					
					useItemNew.x = PetSkillLearnConstData.GridUnitList[j].Grid.x + 2;
					useItemNew.y = PetSkillLearnConstData.GridUnitList[j].Grid.y + 2;
					PetSkillLearnConstData.GridUnitList[j].Item = useItemNew;
					PetSkillLearnConstData.GridUnitList[j].IsUsed = true;
					gridSprite.addChild(useItemNew as DisplayObject);
				}
			}
			else
			{
			 	for(var i:int = 0; i < list.length; i++)
				{
					PetSkillLearnConstData.GridUnitList[i].HasBag = true;
					if(list[i] == undefined) 
					{
						continue;
					}
					var useItem:SkillItem = new SkillItem(list[i].gameSkill.SkillID, gridSprite);		//new UseItem(list[j].index, list[j].type, gridSprite);
					useItem.Num = 1;
					useItem.x = PetSkillLearnConstData.GridUnitList[i].Grid.x + 2;
					useItem.y = PetSkillLearnConstData.GridUnitList[i].Grid.y + 2;
					
					useItem.Id = list[i].gameSkill.SkillID;			//技能ID
	//				useItem.IsBind = list[j].isBind;
					useItem.Type = list[i].Level;					//技能等级		
					
					useItem.IsLock = false;
					PetSkillLearnConstData.GridUnitList[i].Item = useItem;
					PetSkillLearnConstData.GridUnitList[i].IsUsed = true;
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
//				trace(gridSprite.getChildAt(count).name);
//				if(gridSprite.getChildAt(count) is MovieClip && gridSprite.getChildAt(count).name && gridSprite.getChildAt(count).name.indexOf("petSkillLearnShow_") == 0) {
//					var child:MovieClip = gridSprite.getChildAt(count) as MovieClip;
//					if(child.Item) {
//						gridSprite.removeChild(child.Item);
//						child.Item = null;
//					}
//				}
				////
//				for(var j:int = 0; j < 12; j++) {
//					if(PetSkillLearnConstData.GridUnitList[j].Item) {
//						gridSprite.removeChild(PetSkillLearnConstData.GridUnitList[j].Item as ItemBase);
//					}
//				}
//				if(gridSprite.getChildAt(count) is ItemBase && gridSprite.getChildAt(count).parent.name && String(gridSprite.getChildAt(count).parent.name).indexOf("petSkillLearnShow_") == 0)
//				{
//					var item:ItemBase = gridSprite.getChildAt(count) as ItemBase;
//					gridSprite.removeChild(item);
//					item = null;
//				}
//				count--;
//			}
			for( var i:int = 0; i < 12; i++ ) 
			{
				if(PetSkillLearnConstData.GridUnitList[i].Item) {
					if(PetPropConstData.isNewPetVersion)
					{
						gridSprite.removeChild(PetSkillLearnConstData.GridUnitList[i].Item);
					}
					else
					{
						gridSprite.removeChild(PetSkillLearnConstData.GridUnitList[i].Item as ItemBase);
					}
				}
				if(PetSkillLearnConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSkillLearnConstData.GridUnitList[i].Grid.parent.removeChild(PetSkillLearnConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSkillLearnConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSkillLearnConstData.GridUnitList[i].Grid.parent.removeChild(PetSkillLearnConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				PetSkillLearnConstData.GridUnitList[i].Item = null;
				PetSkillLearnConstData.GridUnitList[i].IsUsed = false;
			}
			PetSkillLearnConstData.skillDataList = [];
			PetSkillLearnConstData.NewSelectGrid = null;
		}
		
		/** 添加技能书道具 */
		public function addItem():void
		{
			var useItem:UseItem = new UseItem(PetSkillLearnConstData.itemData.index, PetSkillLearnConstData.itemData.type, gridSprite);
			if(int(PetSkillLearnConstData.itemData.type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(PetSkillLearnConstData.itemData.type) >= 300000)
			{
				useItem.Num = PetSkillLearnConstData.itemData.amount;
			}
			useItem.x = PetSkillLearnConstData.GridItemUnit.Grid.x + 2;
			useItem.y = PetSkillLearnConstData.GridItemUnit.Grid.y + 2;
			useItem.Id = PetSkillLearnConstData.itemData.id;
			useItem.IsBind = PetSkillLearnConstData.itemData.isBind;
			useItem.Type = PetSkillLearnConstData.itemData.type;
			useItem.IsLock = false;
			PetSkillLearnConstData.GridItemUnit.Item = useItem;
			PetSkillLearnConstData.GridItemUnit.IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		/** 移除技能书道具 */
		public function removeItem():void
		{
			if(PetSkillLearnConstData.itemData && PetSkillLearnConstData.GridItemUnit && PetSkillLearnConstData.GridItemUnit.Item) {
				sendNotification(EventList.BAGITEMUNLOCK, PetSkillLearnConstData.GridItemUnit.Item.Id);
				removeChild(PetSkillLearnConstData.GridItemUnit.Item as ItemBase);
				PetSkillLearnConstData.GridItemUnit.Item = null;
				PetSkillLearnConstData.GridItemUnit.IsUsed = false;
				PetSkillLearnConstData.itemData = null;
				if(PetSkillLearnConstData.GridItemUnit.Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSkillLearnConstData.GridItemUnit.Grid.parent.removeChild(PetSkillLearnConstData.GridItemUnit.Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSkillLearnConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSkillLearnConstData.GridItemUnit.Grid.parent.removeChild(PetSkillLearnConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame"));
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
