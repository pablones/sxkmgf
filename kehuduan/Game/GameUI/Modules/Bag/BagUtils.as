package GameUI.Modules.Bag
{
	import GameUI.Modules.Bag.Proxy.BagData;
	
	public class BagUtils
	{
		public function BagUtils()
		{
		}
		
		public static function TestBagIsFull(index:int):uint
		{
			var bagList:Array = BagData.AllItems[index];	
			var count:uint = 0;		
			for(var i:int = 0; i<BagData.BagNum[index]; i++)
			{
				if(bagList[i] != undefined)
				{								
					count++;
				}
			}
			return count;
		}
		
		/** 找到背包中的空格子 */
		public static function getNullItemIndex(bagIndex:int):int
		{
			var bagList:Array = BagData.AllItems[bagIndex];	
			var res:int = -1;		
			for(var i:int = 0; i < BagData.BagNum[bagIndex]; i++) {
				if(bagList[i] == undefined) {
					res = i;
					break;
				}
			}
			return res;
		}
		
	}
}