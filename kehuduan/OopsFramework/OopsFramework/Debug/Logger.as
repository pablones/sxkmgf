package OopsFramework.Debug
{
	import OopsFramework.Utils.Type;

	public class Logger
	{
		/** 是否显示调试信息 */
		public static var IsRun:Boolean = true;	
			
		/** 调试输出器（可继承 ILogAppender 接口任意开发适合自己的输出器） */				
		public static var LogAppender:ILogAppender = new TraceAppender();		
		
		public static function Print(reporter:*, message:String):void
        {
            var entry:LogEntry = new LogEntry();
            entry.Reporter     = Type.GetClass(reporter);
            entry.Message      = message;
            entry.Type         = LogEntry.MESSAGE;
            ProcessEntry(entry);
        }
        
        public static function Info(Reporter:*, method:String, Message:String):void
		{
			var entry:LogEntry = new LogEntry();
			entry.Reporter     = Type.GetClass (Reporter);
			entry.Method 	   = method;
			entry.Message 	   = method + " - " + Message;
			entry.Type 		   = LogEntry.INFO;
			ProcessEntry(entry);
		}
		
		public static function Debug(reporter:*, method:String, message:String):void
		{
			var entry:LogEntry = new LogEntry();
			entry.Reporter     = Type.GetClass(reporter);
			entry.Method 	   = method;
			entry.Message 	   = method + " - " + message;
			entry.Type     	   = LogEntry.DEBUG;
			ProcessEntry(entry);
		}
		
		public static function Warn(reporter:*, method:String, message:String):void
        {
            var entry:LogEntry = new LogEntry();
            entry.Reporter     = Type.GetClass(reporter);
            entry.Method 	   = method;
            entry.Message	   = method + " - " + message;
            entry.Type 		   = LogEntry.WARNING;
            ProcessEntry(entry);
        }
        
        public static function Error(reporter:*, method:String, message:String):void
        {
            var entry:LogEntry = new LogEntry();
            entry.Reporter     = Type.GetClass(reporter);
            entry.Method 	   = method;
            entry.Message	   = method + " - " + message;
            entry.Type 		  = LogEntry.ERROR;
            ProcessEntry(entry);
        }
        
        public static function PrintCustom(reporter:*, method:String, message:String, type:String):void
        {
            var entry:LogEntry = new LogEntry();
            entry.Reporter 	   = Type.GetClass(reporter);
            entry.Method       = method;
            entry.Message      = method + " - " + message;
            entry.Type   	   = type;
            ProcessEntry(entry);
        }
        
        private static function ProcessEntry(entry:LogEntry):void
        {
        	if(IsRun)
        	{
	        	LogAppender.addLogMessage(entry.Type, 
					        	  		  Type.GetObjectClassName(entry.Reporter), 
					        	 		  entry.Message);
			}
        }
	}
}