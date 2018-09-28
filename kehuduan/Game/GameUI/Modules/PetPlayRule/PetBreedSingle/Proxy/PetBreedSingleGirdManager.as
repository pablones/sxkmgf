package GameUI.Modules.PetPlayRule.PetBreedSingle.Proxy
{
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleConstData;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;
	
	public class PetBreedSingleGirdManager extends Proxy
	{
		public static const NAME:String = "PetBreedSingleGridManager";
		private var gridSprite:MovieClip;
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		public function PetBreedSingleGirdManager(gridSprite:MovieClip)
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
			var useItem:UseItem = new UseItem(PetBreedSingleConstData.itemData.index, PetBreedSingleConstData.itemData.type, gridSprite);
			if(int(PetBreedSingleConstData.itemData.type) < 300000)
			{
				useItem.Num = 1;
			}
			else if(int(PetBreedSingleConstData.itemData.type) >= 300000)
			{
				useItem.Num = PetBreedSingleConstData.itemData.amount;
			}
			useItem.x = PetBreedSingleConstData.GridItemUnit.Grid.x + 2;
			useItem.y = PetBreedSingleConstData.GridItemUnit.Grid.y + 2;
			useItem.Id = PetBreedSingleConstData.itemData.id;
			useItem.IsBind = PetBreedSingleConstData.itemData.isBind;
			useItem.Type = PetBreedSingleConstData.itemData.type;
			useItem.IsLock = false;
			PetBreedSingleConstData.GridItemUnit.Item = useItem;
			PetBreedSingleConstData.GridItemUnit.IsUsed = true;
			gridSprite.addChild(useItem);
		}

	}
}