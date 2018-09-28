package OopsFramework.Content.Loading
{
	import flash.display.*;
	import flash.events.*;
	import flash.media.Sound;
	import flash.net.*;
	import flash.utils.*;
    
	public class SoundItem extends LoadingItem 
	{
        public var loader : Sound;
        
		public function SoundItem(url : String, type : String, name : String)
		{
			super(url, type, name);
		}
		
		override public function load() : void
		{
		    super.load();
		    loader = new Sound();
		    loader.addEventListener(ProgressEvent.PROGRESS, onProgressHandler, false, 0, true);
            loader.addEventListener(Event.COMPLETE, onCompleteHandler, false, 0, true);
            loader.addEventListener(IOErrorEvent.IO_ERROR, onErrorHandler, false, 0, true);
            loader.addEventListener(Event.OPEN, onStartedHandler, false, 0, true);
            loader.addEventListener(SecurityErrorEvent.SECURITY_ERROR, super.onSecurityErrorHandler, false, 0, true);
            try
            {
            	loader.load(urlRequest, context);
            }
            catch( e : SecurityError)
            {
            	super.onSecurityErrorHandler(createErrorEvent(e));
            }
		}
		
		override public function onStartedHandler(evt : Event) : void
		{
            content = loader;
            super.onStartedHandler(evt);
        }
        
        override public function onErrorHandler(evt : ErrorEvent) : void
        {
            super.onErrorHandler(evt);
        }
        
        override public function onCompleteHandler(evt : Event) : void 
        {
            content = loader
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
            if (loader)
            {
                loader.removeEventListener(ProgressEvent.PROGRESS, onProgressHandler, false);
                loader.removeEventListener(Event.COMPLETE, onCompleteHandler, false);
                loader.removeEventListener(IOErrorEvent.IO_ERROR, onErrorHandler, false);
                loader.removeEventListener(Event.OPEN, onStartedHandler, false);
                loader.removeEventListener(SecurityErrorEvent.SECURITY_ERROR, super.onSecurityErrorHandler, false);
            }
        }
        
        override public function isStreamable(): Boolean
        {
            return true;
        }
        
        override public function isSound(): Boolean{
            return true;
        }
        
        override public function destroy() : void
        {
            cleanListeners();
            stop();
            content = null;
            loader = null;
        }
	}
}
