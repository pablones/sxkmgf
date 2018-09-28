package GameUI.Modules.Icon.Data
{
	import flash.filters.GlowFilter;
	
	public class IconData
	{
		public static const SHOW_ICON:String = "show_icon";
		public static const CLICK_BUTTON:String = "click_button";
		
		public static var Icon_1:Array = new Array();
		public static var Icon_2:Array = new Array();
		public static var Icon_3:Array = new Array();
//		public static var Icon_4:Array = new Array();
		
		public static var IconArray:Array = [Icon_1, Icon_2, Icon_3];
		
		/** 存储IconArray中各数组是否为空 */
		public static var hasIcon:Array = [false, false, false];
		
		public static const LENGTH:uint = 3;
		
		/** 存储IconArray中不为空的数组及其索引
		 *  object{ index:uint, arr:Array } 
		 * */
		public static var tempArray:Array = [];
		
		/** 存储图标的中不为空的数组及其索引
		 *  object{ x:Number, y:Number } 
		 * */
		public static var pointArray:Array;
		
		// 图标的width
		public static const WIDTH:Number = 20;
		
		// 图标的height
		public static const HEIGHT:Number = 20;
		
		public static const GLOWGILTER:Array = [
												new GlowFilter(0xFAFF72,.1,8,8),
												new GlowFilter(0xFAFF72,.2,8,8),
												new GlowFilter(0xFAFF72,.3,8,8),
												new GlowFilter(0xFAFF72,.4,8,8),
												new GlowFilter(0xFAFF72,.5,8,8),
												new GlowFilter(0xFAFF72,.6,8,8),
												new GlowFilter(0xFAFF72,.7,8,8),
												new GlowFilter(0xFAFF72,.8,8,8),
												new GlowFilter(0xFAFF72,.9,8,8),
												new GlowFilter(0xFAFF72,1,8,8),
												new GlowFilter(0xFAFF72,.9,8,8),
												new GlowFilter(0xFAFF72,.8,8,8),
												new GlowFilter(0xFAFF72,.7,8,8),
												new GlowFilter(0xFAFF72,.6,8,8),
												new GlowFilter(0xFAFF72,.5,8,8),
												new GlowFilter(0xFAFF72,.4,8,8),
												new GlowFilter(0xFAFF72,.3,8,8),
												new GlowFilter(0xFAFF72,.2,8,8),
												new GlowFilter(0xFAFF72,.1,8,8),
												new GlowFilter(0xFAFF72,0,8,8)
		                                        ];
	}
}