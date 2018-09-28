package OopsFramework.Net
{
    import flash.events.Event;
    import flash.events.EventDispatcher;
    import flash.events.IOErrorEvent;
    import flash.events.ProgressEvent;
    import flash.events.SecurityErrorEvent;
    import flash.net.Socket;
    import flash.utils.ByteArray;
    import flash.utils.Endian;  
    
  	[Event(name="decode_error" , type="BaseSocketEvent")]
  	[Event(name="received"     , type="BaseSocketEvent")]
  	[Event(name="sending"      , type="BaseSocketEvent")]
  	[Event(name="securityError", type="BaseSocketEvent")]
  	[Event(name="ioError"      , type="BaseSocketEvent")]
    [Event(name="close"        , type="BaseSocketEvent")]	
    [Event(name="connect"      , type="BaseSocketEvent")]
    public class BaseSocket extends EventDispatcher
    {            
        private const key:int = 121;

        private var host:String;  
        private var port:uint;  
        private var socket:Socket;
        private var receBytes:ByteArray;
          
        public function BaseSocket(host:String, port:uint = 80, _socket:Socket = null) 
        {  
            this.host  			  = host;  
            this.port   		  = port;  
            if ( _socket != null )
            {
            	this.socket 		  = _socket;  
            }  
            else
            {
            	this.socket 		  = new Socket();      	
            } 
            this.receBytes 		  = new ByteArray();
            this.receBytes.endian = Endian.LITTLE_ENDIAN;
            this.socket.addEventListener(Event.CONNECT, Handler);  
            this.socket.addEventListener(Event.CLOSE, Handler);  
            this.socket.addEventListener(IOErrorEvent.IO_ERROR, Handler);  
            this.socket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, Handler);  
            this.socket.addEventListener(ProgressEvent.SOCKET_DATA, Handler);
//			this.socket.objectEncoding = ObjectEncoding.AMF3; 
        }
        
		/** 主机名（IP或域名) */
        public function get Host():String 
        {  
            return host;  
        }  
        
        /** 主机端口 */
        public function get Port():uint 
        {  
            return port;  
        }  
       
		/** 是否已连接 */
        public function get Connected():Boolean 
        {  
            return this.socket.connected;  
        }  
          
        /** 执行连接 */
        public function Connect():void 
        {         
//			Security.loadPolicyFile("xmlsocket://" + this.host + ":" + this.port);						// 这个是原来的
            this.socket.connect(host, port);  
        }  
          
        /** 关闭连接 */
        public function Close():void 
        {  
        	this.socket.removeEventListener(Event.CONNECT, Handler);  
			this.socket.removeEventListener(Event.CLOSE, Handler);  
			this.socket.removeEventListener(IOErrorEvent.IO_ERROR, Handler);  
			this.socket.removeEventListener(SecurityErrorEvent.SECURITY_ERROR, Handler);  
			this.socket.removeEventListener(ProgressEvent.SOCKET_DATA, Handler);
            this.socket.close();
			this.socket = null;
        }  
        
        /** 发送信息 */
        public function Send(bytes:ByteArray=null):void 
        {  
            if(!this.Connected || bytes == null)
            {  
                return;  
            }  
            if(bytes) 
            {
				this.socket.writeBytes(this.Encryption(bytes), 0, bytes.length);
				this.socket.flush();
			}           
        }  
        
//		private var tempArray1:Array = new Array();
//		private var tempArray2:Array = new Array();
		 
        /** 接收信息 */
        private function Received():void 
        {
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.endian		= Endian.LITTLE_ENDIAN;
			socket.readBytes(tempBytes);
			
			// 如果为空包不取
			if (tempBytes.length==0) 
			{
				return;
			}

			receBytes.writeBytes(tempBytes, 0, tempBytes.length);	//连接前面的数据包

			// 取包不完整
			if (receBytes.length < 4) return;
		
			//得到长度
			receBytes.position = 0;
			var firstPackageLenght:int = receBytes.readShort();
			if(firstPackageLenght > 1024 || firstPackageLenght < 4)
			{
				var check1:uint  = receBytes.readShort();
				receBytes.length = 0;
//				throw new ArgumentError("socket error 1" + check1);
				return;
			}
			
//			var tempLen:uint = 0;
			
			while (receBytes.length >= firstPackageLenght) 
			{
				// 剩余部分为0
				receBytes.position 	   = 0;
				// 取出完整包
				var endBytes:ByteArray = new ByteArray();
				endBytes.endian		   = Endian.LITTLE_ENDIAN;
				endBytes.writeBytes(receBytes, 0, firstPackageLenght);

				// 取出剩余部分
				tempBytes		 = new ByteArray();
				tempBytes.endian = Endian.LITTLE_ENDIAN;
				if(receBytes.length - firstPackageLenght > 0) 
				{
					tempBytes.writeBytes(receBytes, firstPackageLenght, receBytes.length - firstPackageLenght);
				}
				
				if(endBytes.length > 4) 
				{
					endBytes.position 	   = 0;
					if(protocolLenght != endBytes.length) 
					{
						receBytes.length = 0;
//						throw new ArgumentError("socket error 2");
					}
					var encryptBytes:ByteArray = this.Encryption(endBytes);
					encryptBytes.position      = 0;
					var protocolLenght:int 	   = encryptBytes.readShort();		// 协议长度
					var protocolNumber:int     = encryptBytes.readShort();		// 协议号
					
//					tempArray1[tempLen] = protocolLenght;
//					tempArray2[tempLen] = protocolNumber;
//					tempLen++;
//					if(tempLen>1000)
//					{
//						tempLen    = 0;
//						tempArray1 = [];
//						tempArray2 = [];
//					}
					this.dispatchEvent(new BaseSocketEvent(BaseSocketEvent.RECEIVED, {protocol:protocolNumber, data:encryptBytes}));		
				} 
				
				// 写入剩余部分
//				receBytes		 = new ByteArray();
//				receBytes.endian = Endian.LITTLE_ENDIAN;
				receBytes.length = 0;
				if (tempBytes.length > 0) 
				{
					receBytes.writeBytes(tempBytes, 0, tempBytes.length);
					receBytes.position = 0;
					if(receBytes.length < 4)  //剩余少于4 保持缓冲退出
					{
						receBytes.position = tempBytes.length;
						return;
					}
					else
					{
						firstPackageLenght = receBytes.readShort(); //剩余大于4  继续处理
						if(firstPackageLenght > 1024 || firstPackageLenght < 4)
						{
//							var check1:uint = receBytes.readShort();
							receBytes.length = 0;
//							throw new ArgumentError("socket error 3" + check1);
							return;
						}
						if(receBytes.length < firstPackageLenght)
						{
							receBytes.position = tempBytes.length;
							return;
						}
					}
				}
				else
				{
					return;	//发现剩余为0 跳出
				}
			}
        }  
        
        /** 异或加密 */
        private function Encryption(bytes:ByteArray):ByteArray
        {
        	var targetBytes:ByteArray = new ByteArray();
        	targetBytes.endian		  = Endian.LITTLE_ENDIAN;
        	bytes.position 			  = 0;
        	targetBytes.writeShort(bytes.readShort());
        	var position:int = bytes.position;
        	for(var i:int = position; i < bytes.length; i++)
        	{
        		var byte:int   = bytes.readUnsignedByte();
        		var target:int = int(byte ^ this.key);				// 异或值为0时为不加密
        		targetBytes.writeByte(target);
        	}
        	targetBytes.position = 4;
        	return targetBytes;
        }
        
        private function Handler(event:Event):void 
        {  
            switch(event.type) 
            {  
                case Event.CLOSE:
                    this.dispatchEvent(new BaseSocketEvent(BaseSocketEvent.CLOSE));  
                    break;  
                case Event.CONNECT:                   
                case IOErrorEvent.IO_ERROR:  
                case SecurityErrorEvent.SECURITY_ERROR:
                    this.dispatchEvent(new BaseSocketEvent(event.type));  
                    break;  
                case ProgressEvent.SOCKET_DATA:  
                    this.Received();  
                    break;  
            }  
        }  
    }  
}
