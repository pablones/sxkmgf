package OopsFramework.Debug
{
    /** 只需通过跟踪日志转储活动。*/
    public class TraceAppender implements ILogAppender
    {
        public function addLogMessage(level:String, loggerName:String, message:String):void
        {
            trace(level + ": " + loggerName + " - " + message);
        }
    }
}