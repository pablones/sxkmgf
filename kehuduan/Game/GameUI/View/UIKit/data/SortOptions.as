package GameUI.View.UIKit.data 
{
	/**
	 * ...
	 * @author statm
	 */
	public class SortOptions 
	{
		public var field:String;
		
		public var compareFunction:Function;
		
		public var param:Number;
		
		public function SortOptions(field:String, param:Number = 0, compareFunction:Function = null) 
		{
			this.field = field;
			this.compareFunction = compareFunction;
			this.param = param;
		}
		
		public function equal(options:SortOptions):Boolean
		{
			return (this.field == options.field
					&& this.compareFunction == options.compareFunction
					&& this.param == options.param);
		}
	}

}