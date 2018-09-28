package OopsFramework.Net
{
	import OopsFramework.IDisposable;
	
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.net.URLVariables;
	
	public class BaseHttp implements IDisposable
	{
		private var dataUrlvariables : URLVariables;
		private var sendUrlrequest   : URLRequest;
		private var handlerLoader	 : URLLoader;
		
		/** 请求完成事件 */
		public var RequestComplete:Function;
		
		public function BaseHttp(url:String, requestMethod:String = "POST")
		{
			this.sendUrlrequest		   = new URLRequest();
			this.sendUrlrequest.url    = url;
			this.sendUrlrequest.method = requestMethod;   
			
			this.dataUrlvariables      = new URLVariables();
		}
		
		/** 加载Form提交的参数 */
		public function AddUrlVariables(name:String, data:*):void
		{
			this.dataUrlvariables[name] = data;
		}
		
		/** 加载Form提交的参数 */
		public function Submit():void
		{
			this.sendUrlrequest.data = this.dataUrlvariables;
			this.handlerLoader		 = new URLLoader();
			this.handlerLoader.addEventListener(Event.COMPLETE       , onLoadCompleteHandler);
			this.handlerLoader.addEventListener(IOErrorEvent.IO_ERROR, onIoError);
			this.handlerLoader.load(this.sendUrlrequest); 
		}
		
		protected function onLoadCompleteHandler(e:Event):void 
		{
			if(RequestComplete!=null)
			{
				RequestComplete(e.target.data as String);
			}
			
			this.Dispose();
		}   
		
		protected function onIoError(e:IOErrorEvent):void
		{
			this.Dispose();
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.handlerLoader.removeEventListener(IOErrorEvent.IO_ERROR, onIoError);
			this.handlerLoader.removeEventListener(Event.COMPLETE       , onLoadCompleteHandler);
			this.handlerLoader.close();
			this.handlerLoader    = null;
			this.dataUrlvariables = null;
			this.sendUrlrequest   = null;
			this.RequestComplete  = null;
		}
		/** IDisposable End */
	}
}