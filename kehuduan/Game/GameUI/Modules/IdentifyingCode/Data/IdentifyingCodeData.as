package GameUI.Modules.IdentifyingCode.Data
{
	public class IdentifyingCodeData
	{
		public static var yanZhengName:String = GameCommonData.wordDic[ "Modules_YanZhengMa_Data_YanZhengMaData_1" ] /**"验证码"*/;
		public static var tiShi:String = GameCommonData.wordDic[ "Modules_YanZhengMa_Data_YanZhengMaData_2" ] /**"验证码不能为空"*/;
		public static var tiShi1:String = GameCommonData.wordDic[ "Modules_YanZhengMa_Data_YanZhengMaData_3" ] /**"你输入错误，请重新输入"*/;
		public static var tiShi2:String = GameCommonData.wordDic[ "Modules_YanZhengMa_Data_YanZhengMaData_4" ] /**"你连续2次输入错误，再次错误将被强制断线10分钟"*/;
		public static var str1:String = GameCommonData.wordDic[ "Modules_YanZhengMa_Data_YanZhengMaData_5" ] /**"剩余时间："*/;
		public static var str2:String = GameCommonData.wordDic[ "Modules_YanZhengMa_Data_YanZhengMaData_6" ] /**"秒"*/;
		
		public static var resourcePath:String = "Resources/GameDLC/IdentifyingCode.swf";
		
		public static var isInit:Boolean = false;
		public static var isShowView:Boolean = false;
		
		public static var str:String = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		public static var colors:Array=[0x00FF00];
		public static var codeLen:int = 4;
		public static var codeSize:int = 18;
		
		/** 验证码位置 */
		public static var YZMSite:Array = null;
		/** 验证码背景 */
		public static var YZMBackColor:Array = null;
		public static var YZM:Array = null;
		public static var YZMLEN:int = 3;	
		
		/** 最大时间 */
		public static var LEFTMAX:int = 60;
		/** 剩余时间 */
		public static var leftTime:int = 60;
		/** 验证次数 */
		public static var YanZhengTime:int = 0;	
		
		/** 间隔时间 */
		public static var IntervalTime:int = 600;
		/** 最后一次发送消息时间 */
		public static var lastTime:int = 0;
	}
}