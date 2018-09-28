package OopsFramework.Debug
{
   public class LogEntry
   {
      public static const ERROR   : String = "ERROR";
      public static const WARNING : String = "WARNING";
	  public static const DEBUG   : String = "DEBUG";
	  public static const INFO    : String = "INFO";
      public static const MESSAGE : String = "MESSAGE";
      
      /** 目标类对象 */
      public var Reporter : Class = null;
      /** 目标方法 */
      public var Method   : String = "";
      /** 提示信息 */
      public var Message  : String = "";
      /** 信息类型 */
      public var Type:String = null;
      public var Depth:int   = 0;
      
      /** 完整的信息，格式化，包括目标类和方法，如果它们存在。 */
      public function get FormattedMessage():String
      {
         var deep:String = "";
         for (var i:int = 0; i < Depth; i++)
         {
            deep += "   ";
         }
         
         var reporter:String = "";
         if (reporter)
         {
            reporter = reporter + ": ";
         }
         
         var method:String = "";
         if (method != null && method != "")
         {
            method = method + " - ";
         }
         
         return deep + Reporter + Method + Message;
      }
   }
}