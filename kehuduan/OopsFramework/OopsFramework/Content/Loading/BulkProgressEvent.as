package OopsFramework.Content.Loading
{
	import flash.events.*;

	public class BulkProgressEvent extends ProgressEvent 
	{
		public static const PROGRESS  : String = "progress";
		public static const COMPLETE  : String = "complete";
    	public static const ERROR     : String = "error";
		public static const OPEN      : String = "open";
		
		public var ItemsLoaded        : int;			// 已加载完成数
	    public var ItemsTotal         : int;			// 下载项目总数
	    public var ItemBytesLoaded    : int;			// 当前项已加载完成数
 	    public var ItemBytesTotal     : int;			// 当前项加载总数
	    public var ItemsSpeed         : Number;			// 项目下载速度
 	    public var Item			      : LoadingItem;
 	    public var ErrorMessage		  : String;
        
        private var weightPercent     : Number;
        
        public function get WeightPercent():Number
        {
        	return ValidateNumber(weightPercent);
        }
        public function set WeightPercent(value:Number):void
        {
        	weightPercent = value;
        }
		
		public function get BytesLoaded():int
        {
        	return bytesLoaded;
        }
        public function set BytesLoaded(value:int):void
        {
        	bytesLoaded = value;
        }
        
        public function get BytesTotal():int
        {
        	return bytesTotal;
        }
        public function set BytesTotal(value:int):void
        {
        	bytesTotal = value;
        }
        
        public function get PercentLoaded():Number
        {
        	return ValidateNumber(bytesTotal > 0 ? (bytesLoaded / bytesTotal) : 0)
        }

		public function BulkProgressEvent(name : String, bubbles:Boolean = true, cancelable:Boolean = false)
		{
			super(name, bubbles, cancelable);
		}
		
		public override function toString():String
		{
		    var names : Array = [];
		    names.push("ItemBytesLoaded: "    + ItemBytesLoaded);
		    names.push("ItemBytesTotal: "     + ItemBytesTotal);
            names.push("BytesLoaded: "        + BytesLoaded);
		    names.push("BytesTotal: "         + BytesTotal);
            names.push("ItemsLoaded: "        + ItemsLoaded);
            names.push("ItemsTotal: "         + ItemsTotal);
		    names.push("ItemsSpeed: "         + ItemsSpeed);
		    names.push("PercentLoaded: "      + BulkLoader.TruncateNumber(PercentLoaded));
		    names.push("WeightPercent: "      + BulkLoader.TruncateNumber(WeightPercent));
		    return "BulkProgressEvent "	      + names.join(", ") + ";"
		}
		
		private function ValidateNumber(value:Number):Number
		{
			if (isNaN(value) || !isFinite(value)) 
        		return 0;
        	return value;
		}
	}
}
