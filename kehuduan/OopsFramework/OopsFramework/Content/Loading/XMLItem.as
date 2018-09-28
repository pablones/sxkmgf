package OopsFramework.Content.Loading
{
	import flash.display.*;
	import flash.events.*;
	import flash.net.*;
	import flash.utils.*;

	public class XMLItem extends LoadingItem 
	{
        public var loader : URLLoader;
        
		public function XMLItem(url : String, type : String, name : String)
		{
			super(url, type, name);
		}
		
		override public function load() : void
		{
		    super.load();
		    loader = new URLLoader();
            loader.addEventListener(ProgressEvent.PROGRESS			 , super.onProgressHandler     , false, 0, true);
            loader.addEventListener(Event.COMPLETE					 , onCompleteHandler	       , false, 0, true);
            loader.addEventListener(IOErrorEvent.IO_ERROR			 , onErrorHandler		       , false, 0, true);
            loader.addEventListener(HTTPStatusEvent.HTTP_STATUS 	 , super.onHttpStatusHandler   , false, 0, true);
            loader.addEventListener(SecurityErrorEvent.SECURITY_ERROR, super.onSecurityErrorHandler, false, 0, true);
            loader.addEventListener(Event.OPEN					  	 , onStartedHandler			   , false, 0, true);
            try
            {
            	loader.load(urlRequest);
            }
            catch( e : SecurityError)
            {
            	onSecurityErrorHandler(createErrorEvent(e));
            }
		}
		
		override public function onErrorHandler(evt : ErrorEvent) : void
		{
            super.onErrorHandler(evt);
        }
        
		override public function onStartedHandler(evt : Event) : void
		{
            super.onStartedHandler(evt);
        }
        
        override public function onCompleteHandler(evt : Event) : void 
        {
            try
            {
                content = new XML(loader.data);
            }
            catch(e : Error)
            {
                content = null;
                status  = STATUS_ERROR;  
                dispatchEvent(createErrorEvent(e));
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
                }
            }
            catch(e : Error){}
            super.stop();
        }
        
        override protected function cleanListeners() : void 
        {
            if(loader)
            {
                loader.removeEventListener(ProgressEvent.PROGRESS, onProgressHandler, false);
                loader.removeEventListener(Event.COMPLETE, onCompleteHandler, false);
                loader.removeEventListener(IOErrorEvent.IO_ERROR, onErrorHandler, false);
                loader.removeEventListener(Event.OPEN, onStartedHandler, false);
                loader.removeEventListener(HTTPStatusEvent.HTTP_STATUS, super.onHttpStatusHandler, false);
                loader.removeEventListener(SecurityErrorEvent.SECURITY_ERROR, super.onSecurityErrorHandler, false);
            }
        }
        
        override public function isXML(): Boolean
        {
            return true;
        }
        
        override public function destroy() : void
        {
            stop();
            cleanListeners();
            content = null;
            loader  = null;
        }
	}
}
