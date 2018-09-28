package OopsEngine.Graphics
{
	import flash.filters.GlowFilter;
	
	public class Font
	{
		public static function Stroke(fontStrokeColor:uint=0):Array
		{
			var txtGF:GlowFilter = new GlowFilter(fontStrokeColor,1,2,2,12);
			var filters:Array = new Array();
			filters.push(txtGF);
			return filters;
		}
	}
}