package OopsFramework
{
	import flash.net.LocalConnection;

	public class Memory
	{
		/** 强制回收内存 */
		public static function CollectEMS():void
		{   
			var conn1:LocalConnection;
			var conn2:LocalConnection;			
			try
			{
				conn1 = new LocalConnection();
				conn1.connect("sban garbage collection 1");
				conn2 = new LocalConnection();
				conn2.connect("sban garbage collection 1");
			}
			catch(error : Error)
			{
				
			}
			finally
			{
				conn1 = null;
				conn2 = null;
			}
		}
	}
}