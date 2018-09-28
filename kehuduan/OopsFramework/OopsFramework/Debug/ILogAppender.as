package OopsFramework.Debug
{
    /** 记录添加接口 */
	public interface ILogAppender
	{
		function addLogMessage(level:String, loggerName:String, message:String):void;
	}
}