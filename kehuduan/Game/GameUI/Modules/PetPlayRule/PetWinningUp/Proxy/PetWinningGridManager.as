package GameUI.Modules.PetPlayRule.PetWinningUp.Proxy
{

	import GameUI.ConstData.EventList;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetWinningConstData;
	import GameUI.Modules.PetPlayRule.PetWinningUp.Data.PetWinningConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PetWinningGridManager extends Proxy
	{
		public static const NAME:String = "PetWinningGridManager";
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetWinningGridManager(gridSprite:MovieClip)
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
		}
		
		public function addItem():void
		{
			var useItem:UseItem = new UseItem(PetWinningConstData.itemData.index, PetWinningConstData.itemData.type, gridSprite);
			if(int(PetWinningConstData.itemData.type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(PetWinningConstData.itemData.type) >= 300000)
			{
				useItem.Num = PetWinningConstData.itemData.amount;
			}
			useItem.x = PetWinningConstData.GridItemUnit.Grid.x + 2;
			useItem.y = PetWinningConstData.GridItemUnit.Grid.y + 2;
			useItem.Id = PetWinningConstData.itemData.id;
			useItem.IsBind = PetWinningConstData.itemData.isBind;
			useItem.Type = PetWinningConstData.itemData.type;
			useItem.IsLock = false;
			PetWinningConstData.GridItemUnit.Item = useItem;
			PetWinningConstData.GridItemUnit.IsUsed = true;
			gridSprite.addChild(useItem);
		}
		
		public function removeItem():void
		{
			if(PetWinningConstData.itemData && PetWinningConstData.GridItemUnit && PetWinningConstData.GridItemUnit.Item) {
				sendNotification(EventList.BAGITEMUNLOCK, PetWinningConstData.GridItemUnit.Item.Id);
				removeChild(PetWinningConstData.GridItemUnit.Item as ItemBase);
				PetWinningConstData.GridItemUnit.Item = null;
				PetWinningConstData.GridItemUnit.IsUsed = false;
				PetWinningConstData.itemData = null;
				if(PetWinningConstData.GridItemUnit.Grid.parent.getChildByName("redFrame")) 
	    		{
	    			PetWinningConstData.GridItemUnit.Grid.parent.removeChild(PetWinningConstData.GridItemUnit.Grid.parent.getChildByName("redFrame"));
	    		}
	    		if(PetWinningConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame")) 
    			{
    				PetWinningConstData.GridItemUnit.Grid.parent.removeChild(PetWinningConstData.GridItemUnit.Grid.parent.getChildByName("yellowFrame"));
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