package GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Proxy
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PetSavvyUseMoneyGridManager extends Proxy
	{
		public static const NAME:String = "PetSavvyUseMoneyGridManager";
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetSavvyUseMoneyGridManager(gridSprite:MovieClip)
		{
			super(NAME);
			this.gridSprite = gridSprite;
			init();
		}
		
		private function init():void
		{ 
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			redFrame.name = "redFrame";
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
			yellowFrame.name = "yellowFrame";
		
//			PetBreedSingleConstData.GridItemUnit.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
//			PetBreedSingleConstData.GridItemUnit.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
//			PetBreedSingleConstData.GridItemUnit.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
		}
		
		public function addItem():void
		{
			var useItem:UseItem = new UseItem(PetSavvyUseMoneyConstData.itemData.index, PetSavvyUseMoneyConstData.itemData.type, gridSprite);
			if(int(PetSavvyUseMoneyConstData.itemData.type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(PetSavvyUseMoneyConstData.itemData.type) >= 300000)
			{
				useItem.Num = PetSavvyUseMoneyConstData.itemData.amount;
			}
			useItem.x = PetSavvyUseMoneyConstData.GridItemUnit.Grid.x + 2;
			useItem.y = PetSavvyUseMoneyConstData.GridItemUnit.Grid.y + 2;
			useItem.Id = PetSavvyUseMoneyConstData.itemData.id;
			useItem.IsBind = PetSavvyUseMoneyConstData.itemData.isBind;
			useItem.Type = PetSavvyUseMoneyConstData.itemData.type;
			useItem.IsLock = false;
			PetSavvyUseMoneyConstData.GridItemUnit.Item = useItem;
			PetSavvyUseMoneyConstData.GridItemUnit.IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		public function removeItem():void
		{
			if(PetSavvyUseMoneyConstData.itemData && PetSavvyUseMoneyConstData.GridItemUnit && PetSavvyUseMoneyConstData.GridItemUnit.Item) {
				sendNotification(EventList.BAGITEMUNLOCK, PetSavvyUseMoneyConstData.GridItemUnit.Item.Id);
				removeChild(PetSavvyUseMoneyConstData.GridItemUnit.Item as ItemBase);
				PetSavvyUseMoneyConstData.GridItemUnit.Item = null;
				PetSavvyUseMoneyConstData.GridItemUnit.IsUsed = false;
				PetSavvyUseMoneyConstData.itemData = null;
				if(PetSavvyUseMoneyConstData.GridItemUnit.Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetSavvyUseMoneyConstData.GridItemUnit.Grid.parent.removeChild(PetSavvyUseMoneyConstData.GridItemUnit.Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetSavvyUseMoneyConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetSavvyUseMoneyConstData.GridItemUnit.Grid.parent.removeChild(PetSavvyUseMoneyConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame"));
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