package GameUI.Modules.Bag.Proxy
{
	import flash.display.MovieClip;
	
	
	
	public class GridUnit
	{
		
		/**格子背景 */
		public var Grid:MovieClip = null;
		/** useItem */
		public var Item:Object = null;
		public var IsUsed:Boolean = false;
		public var HasBag:Boolean = true;
		public var parent:MovieClip= null;
		public var Index:int = -1;
		
		public function GridUnit(grid:MovieClip, isDoubleClick:Boolean = false)
		{
			this.Grid = grid;
			this.Grid.doubleClickEnabled = isDoubleClick;
		}
			
	}
}