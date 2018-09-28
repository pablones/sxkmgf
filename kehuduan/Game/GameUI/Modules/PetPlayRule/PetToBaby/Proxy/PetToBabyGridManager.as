package GameUI.Modules.PetPlayRule.PetToBaby.Proxy
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.SkillItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PetToBabyGridManager extends Proxy
	{
		public static const NAME:String = "PetToBabyGridManager";
		private var gridList:Array = new Array();
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetToBabyGridManager(list:Array, gridSprite:MovieClip)
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
			}
//			PetBreedSingleConstData.GridItemUnit.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
//			PetBreedSingleConstData.GridItemUnit.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
//			PetBreedSingleConstData.GridItemUnit.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
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
		
		/**  初始化格子数据 */
		public function showItems(list:Array):void
		{
			if(PetToBabyConstData.GridUnitList.length == 0) return;
			for(var j:int = 0; j < list.length; j++)
			{
				PetToBabyConstData.GridUnitList[j].HasBag = true;
				if(list[j] == undefined) 
				{
					continue;
				}
				var useItem:SkillItem = new SkillItem(list[j].gameSkill.SkillID, gridSprite);		//new UseItem(list[j].index, list[j].type, gridSprite);
				useItem.Num = 1;
				useItem.x = PetToBabyConstData.GridUnitList[j].Grid.x + 2;
				useItem.y = PetToBabyConstData.GridUnitList[j].Grid.y + 2;
				
				useItem.Id = list[j].gameSkill.SkillID;			//技能ID
//				useItem.IsBind = list[j].isBind;
				useItem.Type = list[j].Level;					//技能等级		
				
				useItem.IsLock = false;
				PetToBabyConstData.GridUnitList[j].Item = useItem;
				PetToBabyConstData.GridUnitList[j].IsUsed = true;
				gridSprite.addChild(useItem);
			}
		}
		
		/** 移除所有技能格子数据 */
		public function removeAllItem():void
		{
			for( var i:int = 0; i < 6; i++ ) {
				if(PetToBabyConstData.GridUnitList[i].Item) {
					gridSprite.removeChild(PetToBabyConstData.GridUnitList[i].Item as ItemBase);
				}
				if(PetToBabyConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetToBabyConstData.GridUnitList[i].Grid.parent.removeChild(PetToBabyConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetToBabyConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetToBabyConstData.GridUnitList[i].Grid.parent.removeChild(PetToBabyConstData.GridUnitList[i].Grid.parent.getChildByName("yellowFrame"));
    			}
				PetToBabyConstData.GridUnitList[i].Item = null;
				PetToBabyConstData.GridUnitList[i].IsUsed = false;
			}
//			PetToBabyConstData.skillDataList = [];
		}
		
		
//		public function addItem():void
//		{
//			var useItem:UseItem = new UseItem(PetToBabyConstData.itemData.index, PetToBabyConstData.itemData.type, gridSprite);
//			if(int(PetToBabyConstData.itemData.type) < 300000)
//			{
//				useItem.Num = 1;
//			}
//			else if(int(PetToBabyConstData.itemData.type) >= 300000)
//			{
//				useItem.Num = PetToBabyConstData.itemData.amount;
//			}
//			useItem.x = PetToBabyConstData.GridItemUnit.Grid.x + 2;
//			useItem.y = PetToBabyConstData.GridItemUnit.Grid.y + 2;
//			useItem.Id = PetToBabyConstData.itemData.id;
//			useItem.IsBind = PetToBabyConstData.itemData.isBind;
//			useItem.Type = PetToBabyConstData.itemData.type;
//			useItem.IsLock = false;
//			PetToBabyConstData.GridItemUnit.Item = useItem;
//			PetToBabyConstData.GridItemUnit.IsUsed = true;
//			gridSprite.addChild(useItem);
//		}
//		
//		/** 移除还童道具 */
//		public function removeItem():void
//		{
//			if(PetToBabyConstData.itemData && PetToBabyConstData.GridItemUnit && PetToBabyConstData.GridItemUnit.Item) {
//				sendNotification(EventList.BAGITEMUNLOCK, PetToBabyConstData.GridItemUnit.Item.Id);
//				removeChild(PetToBabyConstData.GridItemUnit.Item as ItemBase);
//				PetToBabyConstData.GridItemUnit.Item = null;
//				PetToBabyConstData.GridItemUnit.IsUsed = false;
//				PetToBabyConstData.itemData = null;
//				if(PetToBabyConstData.GridItemUnit.Grid.parent.getChildByName("redFrame")) 
//	    		{
//	    			PetToBabyConstData.GridItemUnit.Grid.parent.removeChild(PetToBabyConstData.GridItemUnit.Grid.parent.getChildByName("redFrame"));
//	    		}
//	    		if(PetToBabyConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame")) 
//    			{
//    				PetToBabyConstData.GridItemUnit.Grid.parent.removeChild(PetToBabyConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame"));
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