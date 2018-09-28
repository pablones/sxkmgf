package OopsFramework.Net
{
    import flash.events.Event;
    import flash.events.IOErrorEvent;
    import flash.events.SecurityErrorEvent;  
  
    public class BaseSocketEvent extends Event 
    {
        public static const SECURITY_ERROR:String = SecurityErrorEvent.SECURITY_ERROR;  
        public static const IO_ERROR:String       = IOErrorEvent.IO_ERROR;  
        public static const DECODE_ERROR:String   = "decode_error";  
        public static const RECEIVED:String		  = "received";  
        public static const SENDING:String 		  = "sending";  
        public static const CLOSE:String 		  = Event.CLOSE;  
        public static const CONNECT:String 		  = Event.CONNECT;  
          
        private var data:Object;  
          
        public function BaseSocketEvent(type:String, data:Object = null) 
        {  
            super(type, true);  
            this.data = data;            
        }  
          
        public function get Data():Object
        {  
            return data;  
        }  
          
        public override function toString():String 
        {  
            return formatToString("BaseSocketEvent", "type", "bubbles", "cancelable", "data");  
        }  
          
        public override function clone():Event
        {  
            return new BaseSocketEvent(type, data);  
        }  
    }
}  
