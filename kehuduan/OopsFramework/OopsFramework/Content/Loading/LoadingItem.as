package OopsFramework.Content.Loading
{
    import flash.display.*;
    import flash.events.*;
    import flash.net.*;
    import flash.utils.*;
    
    [Event(name="progress", type="flash.events.ProgressEvent")]
    [Event(name="complete", type="flash.events.Event")]
    [Event(name="open"    , type="flash.events.Event")]
    [Event(name="init"    , type="flash.events.Event")]
    public class LoadingItem extends EventDispatcher 
    {
        public function LoadingItem(url : String, type : String, name : String)
        {
            if(!url || !String(url))
            {
                throw new Error("[BulkLoader] 不能添加一个空的 URL 的项目")
            }
            
        	this.status = STATUS_WAIT;
            this.type   = type;
            this.name   = name;
            this.url    = url;
            urlRequest  = new URLRequest(this.url);
            parsedURL   = new SmartURL(url);
        }
        
        /** 开始加载资料 */
		public function load() : void
        {
            if (preventCache)
            {
				urlRequest.url = this.CreateUrl(this.url, "Cache=" + getTimer());
            }
            else if(this.Version!=null)
            {
				urlRequest.url = this.CreateUrl(this.url, this.Version);
            }
            startTime = getTimer();
        }
        
        private function CreateUrl(url:String, v:String):String
        {
        	var urlNew : String;
            if(url.indexOf("?") == -1)
            {
                urlNew = url + "?" + v;
            }
            else
            {
                urlNew = url + "&" + v;
            }
            return urlNew;
        }
        
        /** Http状态事件 */
        public function onHttpStatusHandler(e : HTTPStatusEvent) : void
        {
            httpStatus = e.status;
            dispatchEvent(e);
        }
        
        /** 开始下载事件 */
        public function onStartedHandler(e : Event) : void
        {
            responseTime = getTimer();
            latency      = BulkLoader.TruncateNumber((responseTime - startTime)/1000);
            status 		 = STATUS_STARTED;
            dispatchEvent(e);
        }
        
        public function onProgressHandler(e:ProgressEvent) : void 
        {
            bytesLoaded         = e.bytesLoaded;
            bytesTotal          = e.bytesTotal;
            bytesRemaining      = bytesTotal - bytesLoaded;
            percentLoaded       = bytesLoaded / bytesTotal;
            weightPercentLoaded = percentLoaded * weight;
           
            CalculateSpeed();
            dispatchEvent(e);
        }
        
        /** 计算下载速度 */
        private function CalculateSpeed():void
        {
        	totalTime      = getTimer();
            timeToDownload = ((totalTime - responseTime) / 1000);
            if(timeToDownload == 0)
            {
                timeToDownload = 0.1;
            }
            speed = BulkLoader.TruncateNumber((bytesLoaded / 1024) / (timeToDownload));
        }
        
        /** 下载完成事件 */
        public function onCompleteHandler(e : Event) : void 
        {
            status   = STATUS_FINISHED;
			isLoaded = true;
			
			CalculateSpeed();			// 计算下载速度
            dispatchEvent(e);			// 抛事件
            cleanListeners();			// 下载完成后删除所有下载事件
        }

        /** 加载出错事件 */
        public function onErrorHandler(e : ErrorEvent) : void
        {
            numTries ++;
            
            if(numTries < maxTries)
            {
                status = null
                load();
                e.stopPropagation();
            }
            else
            {
                status     = STATUS_ERROR;
                errorEvent = e;
                dispatchErrorEvent(errorEvent);
                cleanListeners();					// 下载出错时删除所有下载事件
            }
        }

		/** 安全错误事件 */
        public function onSecurityErrorHandler(e : ErrorEvent) : void
        {
            status     = STATUS_ERROR;   
            errorEvent = e as ErrorEvent;
        	dispatchErrorEvent(errorEvent);
        	cleanListeners();
        }
        
        /** 报错事件反馈 */
        private function dispatchErrorEvent (e : ErrorEvent) : void
        {
            status = STATUS_ERROR;
            dispatchEvent(new ErrorEvent(BulkProgressEvent.ERROR, true, false, e.text));
        }

        /** 创建一个错误事件 */
        protected function createErrorEvent(e : Error) : ErrorEvent
        {
            return new ErrorEvent(BulkProgressEvent.ERROR, false, false, e.message);
        }
        
        public override function toString() : String
        {
            return "LoadingItem 地址: " + url + ", 文件类型:" + type + ", 状态: " + status;
        }
        
        /** 停止下载 */
        public function stop() : void
        {
            if(isLoaded)
            {
                return;
            }
            status = STATUS_STOPPED;
        }
        
        /** 释放资源 */
        public function destroy() : void
        {
            content = null;
        }
        
        /** 删除所有事件 */
        protected function cleanListeners() : void { }
        
        /** 是否为视频对象 */
        public function isVideo() 	   : Boolean { return false; }
        
        /** 是否为音频对象 */
        public function isSound() 	   : Boolean { return false; }
        
        /** 是否为文本对象 */
        public function isText() 	   : Boolean { return false; }
       
		/** 是否为 XML 对象 */
        public function isXML() 	   : Boolean { return false; }
        
		/** 是否为图片对象 */     
        public function isImage()	   : Boolean { return false; }
        
        /** 是否为 SWF 对象 */ 
        public function isSWF()   	   : Boolean { return false; }
        
        /** 如果使用装载机的实例返回true。 （如SWF和图片）。 */
        public function isLoader()     : Boolean { return false; }
       
        /** 如果此加载类型应该允许它的内容将尽快访问服务器响应启动返回true。如果属实，因为声音和视频类型。 */
        public function isStreamable() : Boolean { return false; }

		/** 主机头（例：域名）*/
		public function get hostName() : String { return parsedURL.host; }
		
        /** 与此加载项目的时间统计返回一个字符串。 */
        public function getStats() : String
        {
            return "Item url: " 	      + url	 						    + 
           		   "(s), total time: "    + (totalTime/1000).toPrecision(3) +
				   "(s), download time: " + (timeToDownload).toPrecision(3) +
            	   "(s), latency:" 		  + latency						    +
           	 	   "(s), speed: "  		  + speed							+ 
           		   " kb/s, size: " 		  + humanFiriendlySize;
        }
        
		private function get humanFiriendlySize():String
		{
			var kb : Number = bytesTotal/1024;
			if (kb < 1024)
			{
				return int(kb) + " kb"
			}
			else
			{
				return (kb/1024).toPrecision(3) + " mb"
			}
		}
        
    	public static const STATUS_WAIT     : String = "wait";
        public static const STATUS_STOPPED  : String = "stopped";
        public static const STATUS_STARTED  : String = "started";
        public static const STATUS_FINISHED : String = "finished";
        public static const STATUS_ERROR    : String = "error";

        public var url           	   : String;
        public var status	     	   : String;
        public var maxTries       	   : int = 3;			// 最大尝试次数
        public var numTries 	  	   : int = 0;			// 重加载次数
        public var weight		 	   : int = 1;			
        public var preventCache		   : Boolean;			// 防止缓冲加载没更新数据（true为防止缓冲）
        public var type           	   : String;			// 加载数据类型（binary、image、movieclip、sound、text、xml、video）
        public var name            	   : String;			// 防止缓冲加载没更新数据时用到
        public var priority       	   : int = 0;			// 优先级
        public var isLoaded       	   : Boolean;			// 是否加载成功能
        public var bytesTotal    	   : int = 0;			// 下载字段总数
        public var bytesLoaded    	   : int = 0;			// 下载字段
        public var bytesRemaining 	   : int = 10000000;	// 剩余下载量
        public var percentLoaded  	   : Number;			// 加载的百分比进行（从0到1）。
        public var weightPercentLoaded : Number;			// 下载快权力比例
        public var startTime    	   : int;				// 开始下载时间
        public var responseTime 	   : Number;			// 连接上URL资料时间
        public var latency      	   : Number;			// 时间（秒），服务器和发送开始了流媒体内容。
        public var totalTime      	   : int;					
        public var timeToDownload 	   : Number;
        public var speed 		 	   : Number;
        public var content 		 	   : *;					// 资源内容
        public var context 		 	   : *   = null;		
        public var httpStatus 	 	   : int = -1;			// HTTP连接状态
        public var errorEvent          : ErrorEvent;
        public var parsedURL	       : SmartURL;			// 解析 URL 地址
        public var Version 			   : String;
        
        protected var urlRequest       : URLRequest;
    }
}
