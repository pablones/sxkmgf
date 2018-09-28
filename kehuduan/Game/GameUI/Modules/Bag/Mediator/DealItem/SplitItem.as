package GameUI.Modules.Bag.Mediator.DealItem
{
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Bag.Proxy.NetAction;
	
	import Net.ActionProcessor.OperateItem;
	
	public class SplitItem
	{
		public function SplitItem()
		{
		}
		
		public static function Split(grid:GridUnit, num:int):void
		{
			//先判断该物品是否能
			var index:int = grid.Index;
			NetAction.OperateItem(OperateItem.REQUEST_SPLITITEM, 1, num, BagData.AllItems[BagData.SelectIndex][index]);
		}
	}
}