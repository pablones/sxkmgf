package OopsFramework.Content.Loading
{
	import flash.display.*;
	import flash.events.*;
	import flash.net.*;
	import flash.utils.*;
	
    /** 图片、SWF文件加载 */
	public class ImageItem extends LoadingItem 
	{
        public var loader : Loader;
        
		public function ImageItem(url : String, type : String, name : String)
		{
			super(url, type, name);
		}
		
		override public function load() : void
		{
		    super.load();
		    loader = new Loader();
			
            loader.contentLoaderInfo.addEventListener(Event.INIT					   , onInitHandler   	   , false, 0, true);
            loader.contentLoaderInfo.addEventListener(Event.OPEN					   , onStartedHandler	   , false, 0, true); 
            loader.contentLoaderInfo.addEventListener(IOErrorEvent.IO_ERROR 		   , onErrorHandler        , false, 0, true);
            loader.contentLoaderInfo.addEventListener(SecurityErrorEvent.SECURITY_ERROR, onSecurityErrorHandler, false, 0, true);    
		    loader.contentLoaderInfo.addEventListener(ProgressEvent.PROGRESS		   , onProgressHandler	   , false, 0, true);
            loader.contentLoaderInfo.addEventListener(Event.COMPLETE				   , onCompleteHandler	   , false, 0, true);

            loader.contentLoaderInfo.addEventListener(HTTPStatusEvent.HTTP_STATUS, super.onHttpStatusHandler);
            try
            {
            	loader.load(urlRequest, context);
            }
            catch(e:SecurityError)
            {
            	super.onSecurityErrorHandler(createErrorEvent(e));
            }
		}
        
        override public function onErrorHandler(evt : ErrorEvent) : void
        {
            super.onErrorHandler(evt);
        }
        
        public function onInitHandler(evt : Event) :void
        {
            dispatchEvent(evt);
        }
        
        override public function onCompleteHandler(evt : Event) : void 
        {
        	try
        	{
	            content = loader.contentLoaderInfo;
	        }
	        catch(e : SecurityError)
	        {
	        	content = null;
	        }
	        super.onCompleteHandler(evt);
        }
        
        override public function stop() : void
        {
            try
            {
                if(loader)
                {
                    loader.close();
                    loader.unload();
                }
            }
            catch(e : Error){}
            super.stop();
        }
        
        override protected function cleanListeners() : void 
        {
            if (loader)
            {
                loader.contentLoaderInfo.removeEventListener(Event.INIT					       , onInitHandler   	  		 , false);
				loader.contentLoaderInfo.removeEventListener(Event.OPEN					       , super.onStartedHandler	     , false); 
				loader.contentLoaderInfo.removeEventListener(IOErrorEvent.IO_ERROR 		       , super.onErrorHandler        , false);
				loader.contentLoaderInfo.removeEventListener(SecurityErrorEvent.SECURITY_ERROR , super.onSecurityErrorHandler, false);    
				loader.contentLoaderInfo.removeEventListener(ProgressEvent.PROGRESS		       , super.onProgressHandler	 , false);
				loader.contentLoaderInfo.removeEventListener(HTTPStatusEvent.HTTP_STATUS       , super.onHttpStatusHandler   , false);
				loader.contentLoaderInfo.removeEventListener(Event.COMPLETE				       , onCompleteHandler	   		 , false);
            }
        }
        
        override public function isImage(): Boolean
        { 
        	return (type == BulkLoader.TYPE_IMAGE); 
        }
        
        override public function isSWF(): Boolean
        {
            return (type == BulkLoader.TYPE_MOVIECLIP);
        }
        
        override public function destroy() : void
        {
            stop();
            cleanListeners();
            content = null;
            loader  = null;
        }
        
        /** 获取一个SWF类包中指定多名称的类 */
        public function GetDefinitionByName(className : String) : Class
        {
            if (loader.contentLoaderInfo.applicationDomain.hasDefinition(className))
            {
                return loader.contentLoaderInfo.applicationDomain.getDefinition(className) as Class;
            }
            return null;
        }
	}
}
