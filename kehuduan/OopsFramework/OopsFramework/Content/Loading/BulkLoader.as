/**
 *  var a:BulkLoader = new BulkLoader();
	a.addEventListener(BulkProgressEvent.PROGRESS,PROGRESS);
	a.addEventListener(BulkProgressEvent.COMPLETE,COMPLETE);
	a.Add("http://hiphotos.baidu.com/%D4%CAzai/pic/item/f1927dbdde4cb12518d81fad.jpg");			
	a.Add("http://hiphotos.baidu.com/chaduopaopao/pic/item/825ce9a63c861ebfd043583e.jpg");
	a.Start(1);	// 设置最大并行连接数
 */
package OopsFramework.Content.Loading
{
	import OopsFramework.Debug.Logger;
	import OopsFramework.Game;
	import OopsFramework.IDisposable;
	
	import flash.events.ErrorEvent;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.ProgressEvent;
	import flash.utils.Dictionary;
	
	[Event(name="open"     , type="BulkProgressEvent")]
	[Event(name="progress" , type="BulkProgressEvent")]
	[Event(name="complete" , type="BulkProgressEvent")]
	[Event(name="error"    , type="ErrorEvent")]
	public class BulkLoader extends EventDispatcher implements IDisposable
	{
		public static var version:String;
		
		public function BulkLoader(withConnections : int = 1)
		{
			this.maxConnections = withConnections;
		} 
		/** public Function **************************************************/
		 
        /** 启动加载以前添加的所有项目 */
        public function Load() : void
        {
            if(currentConnections > 0) currentConnections--;
			
			while(currentConnections < maxConnections && workItems.length > 0)
			{
				var item:LoadingItem = workItems.shift() as LoadingItem;
				item.load();
				currentConnections++;
			}
        }
        
        /** 下载项目总数  */
        public function get ItemsTotal():int
        {
        	return this.itemsTotal;
        }
        
		/**
		 * 添加加载网址 
		 * @param url 			下载地址
		 * @param preventCache  是否加载缓冲中数据（true为不用IE缓存中数据）
		 * @param name			资源名
		 * @param weight		资源大小分量值
		 */		
        public function Add(url:String, preventCache:Boolean = false, name:String = null, weight:int = 1):void
        {
            if(name==null || name =="")
            {
				name = url;
            }
            
            var item : LoadingItem = GetLoadingItem(url);
            if( item ) return; 								// 当前URL资源已存在
   
            var type : String = GuessType(url);
            item 			  = new TYPE_CLASSES[type] (url, type , name);
            item.preventCache = preventCache;
            item.weight       = weight;
            item.Version      = version;
            // 事件弱引用有可能导致下列事件失效
//			item.addEventListener(BulkProgressEvent.OPEN    , onOpen      , false , 0 , true);
//			item.addEventListener(BulkProgressEvent.COMPLETE, onComplete  , false , 0 , true);
//			item.addEventListener(BulkProgressEvent.PROGRESS, onProgress  , false , 0 , true);
//			item.addEventListener(BulkProgressEvent.ERROR   , onItemError , false , 0 , true);
			item.addEventListener(BulkProgressEvent.OPEN    , onOpen);
			item.addEventListener(BulkProgressEvent.COMPLETE, onComplete);
			item.addEventListener(BulkProgressEvent.PROGRESS, onProgress);
			item.addEventListener(BulkProgressEvent.ERROR   , onItemError);
            items.push(item);      
			workItems.push(item);
     		dictItems[item.name] = item;
            
            totalWeight += item.weight;
            itemsTotal  ++;
        }
        
        /** 获取指定 URL 的 LoadingItem 实例 */
        public function GetLoadingItem(key:String) : LoadingItem
        {
        	return this.dictItems[key] as LoadingItem;
        }
		
		/** IDisposable Start */
        public function Dispose():void
        {
        	items.forEach(function(item:LoadingItem, ...rest):void
            {
				item.removeEventListener(BulkProgressEvent.OPEN    , onOpen		, false);
				item.removeEventListener(BulkProgressEvent.COMPLETE, onComplete , false);
				item.removeEventListener(BulkProgressEvent.PROGRESS, onProgress , false);
				item.removeEventListener(BulkProgressEvent.ERROR   , onItemError, false);
            	item.destroy();
				item = null;
            });
			
			items    			  = null;
			dictItems			  = null;
			workItems 			  = null;

//        	items.forEach(function(item:LoadingItem, ...rest):void
//            {
//				item.removeEventListener(BulkProgressEvent.OPEN    , onOpen);
//				item.removeEventListener(BulkProgressEvent.COMPLETE, onComplete);
//				item.removeEventListener(BulkProgressEvent.PROGRESS, onProgress);
//				item.removeEventListener(BulkProgressEvent.ERROR   , onItemError);
//            	item.destroy();
//            });
        }
		/** IDisposable End */
        
        public function Stop():void
        {
        	items.forEach(function(item:LoadingItem, ...rest):void
            {
            	if(item.status == LoadingItem.STATUS_WAIT || item.status == LoadingItem.STATUS_STARTED)
            	{
            		item.stop();
            	}
            	
            	currentConnections = 0;
            });
        }
        
        /** Private Function **************************************************/
		private function onOpen(e:Event):void
		{
			var item : LoadingItem		= e.target as LoadingItem;     
			var eOpen:BulkProgressEvent = new BulkProgressEvent(BulkProgressEvent.OPEN);
			eOpen.Item 		 			= item;
			eOpen.BytesLoaded 			= bytesLoaded;
			eOpen.BytesTotal  			= bytesTotal;
			eOpen.ItemsLoaded 			= itemsLoaded;
			eOpen.ItemsTotal  			= itemsTotal;
			eOpen.ItemsSpeed  			= itemsSpeed;
			eOpen.WeightPercent 		= weightPercent;
			
			dispatchEvent(eOpen);
		}
        
        /** 下载完成事件（itemsLoaded==itemsTotal为所有项下载完成）*/
        private function onComplete(e:Event):void
        {
        	itemsLoaded++;
        	
        	var item : LoadingItem			= e.target as LoadingItem;     
            var eComplete:BulkProgressEvent = new BulkProgressEvent(BulkProgressEvent.COMPLETE);
            eComplete.Item 		 			= item;
            eComplete.BytesLoaded 			= bytesLoaded;
            eComplete.BytesTotal  			= bytesTotal;
            eComplete.ItemsLoaded 			= itemsLoaded;
            eComplete.ItemsTotal  			= itemsTotal;
            eComplete.ItemsSpeed  			= itemsSpeed;
            eComplete.WeightPercent 		= weightPercent;
            
	        dispatchEvent(eComplete);
     
            if(itemsLoaded!=itemsTotal)
            { 
           		this.Load();
            }
        }
        
        /** 项下载出错 */
        private function onItemError(e:ErrorEvent):void
        {
        	this.Load();
        	
        	var item : LoadingItem       = e.target as LoadingItem;     
            var eError:BulkProgressEvent = new BulkProgressEvent(BulkProgressEvent.ERROR);
            eError.Item 		 		 = item;
            eError.BytesLoaded 			 = bytesLoaded;
            eError.BytesTotal  			 = bytesTotal;
            eError.ItemsLoaded 			 = itemsLoaded;
            eError.ItemsTotal  			 = itemsTotal;
            eError.ItemsSpeed  			 = itemsSpeed;
            eError.WeightPercent 		 = weightPercent;
            eError.ErrorMessage   		 = e.text;
            
	        dispatchEvent(eError);
	        
	        Logger.Error(this,"onItemError",e.text);
        }
        
        /** 下载进度处理 */
        private function onProgress(e:ProgressEvent):void
        {
            var bpe : BulkProgressEvent = GetProgressForItems(e);
            dispatchEvent(bpe);
        }
        
        /** 计算下载进度状态 */
        private function GetProgressForItems(pe:ProgressEvent):BulkProgressEvent
        {
            bytesLoaded = bytesTotal = 0;
            var localWeightPercent     : Number = 0;
            var localWeightLoaded      : Number = 0;							
            var localWeightTotal       : int    = 0;
            var localitemsStarted      : int    = 0;					// 开始下载的对象数
            var localItemsTotal        : int    = 0;					// 对象总数
            var localItemsLoaded       : int    = 0;					// 已下载完成对象数
            var localBytesLoaded       : int    = 0;					// 已下载字节数
            var localBytesTotal        : int    = 0;					// 字节总数
            var localBytesTotalCurrent : int    = 0;					// 所有下载对象字节总数
            var localSpeed			   : Number = 0;					// 下载每秒速度
            
            for each (var item:LoadingItem in items)
            {
            	if (!item) continue;
                localItemsTotal++;
                localitemsStarted++;
                localWeightTotal += item.weight;
                if (item.status == LoadingItem.STATUS_STARTED  || 		// 下载启动 (LoadingItem.onStartedHandler)
                    item.status == LoadingItem.STATUS_FINISHED || 		// 下载完成 (LoadingItem.onCompleteHandler)
                    item.status == LoadingItem.STATUS_STOPPED)			// 下载停止 (LoadingItem.stop())
                {
                    // 新加防止网速很快，FALSH API没及时获取到下载文件大小而导致的计算错误    2010.12.17
                    if ( item.bytesLoaded > item.bytesTotal ) 
                    {
                   		item.bytesLoaded = item.bytesTotal;
                    }
                    
                	localSpeed 			   += item.speed;
                    localBytesLoaded       += item.bytesLoaded;
                    localBytesTotalCurrent += item.bytesTotal;
                    localWeightLoaded	   += (item.bytesLoaded / item.bytesTotal) * item.weight;
                    if(item.status == LoadingItem.STATUS_FINISHED) 
                    {
                        localItemsLoaded ++;
                    }
                }
            }

            // 只设置字节总数如果所有的项目已开始加载
            if (localitemsStarted != localItemsTotal)
            {
                localBytesTotal = Number.POSITIVE_INFINITY;
            }
            else
            {
                localBytesTotal = localBytesTotalCurrent;
            }
            localWeightPercent = localWeightLoaded / localWeightTotal;			// 所对下载对象的完成总比例
            if(localWeightTotal == 0) 
            {
            	localWeightPercent = 0;
            }
            
            localSpeed = localSpeed / (itemsLoaded + 1);
            itemsSpeed = localSpeed;
          
            var e:BulkProgressEvent = new BulkProgressEvent(BulkProgressEvent.PROGRESS);
            e.ItemBytesLoaded = pe.bytesLoaded;
            e.ItemBytesTotal  = pe.bytesTotal;
            e.ItemsLoaded	  = localItemsLoaded;
            e.ItemsTotal      = localItemsTotal;
            e.ItemsSpeed      = localSpeed;
            e.BytesLoaded     = localBytesLoaded;
            e.BytesTotal      = localBytesTotal;
            e.WeightPercent   = localWeightPercent;
            
            return e;
        }
      
        /** 判断资源文件类型 */
        private function GuessType(urlAsString : String) : String
        {
            // 将删除URL的查询字符串
            var searchString : String = urlAsString.indexOf("?") > -1 ? 
                    urlAsString.substring(0, urlAsString.indexOf("?")) : urlAsString;
                    
            var finalPart : String = searchString.substring(searchString.lastIndexOf("/"));;
            var extension : String = finalPart.substring(finalPart.lastIndexOf(".") + 1).toLowerCase();
            var type      : String;
            if(!Boolean(extension) )
            {
                extension = BulkLoader.TYPE_TEXT;
            }
            if(extension == BulkLoader.TYPE_IMAGE || BulkLoader.IMAGE_EXTENSIONS.indexOf(extension) > -1)
            {
                type = BulkLoader.TYPE_IMAGE;
            }
            else if (extension == BulkLoader.TYPE_SOUND ||BulkLoader.SOUND_EXTENSIONS.indexOf(extension) > -1)
            {
                type = BulkLoader.TYPE_SOUND;
            }
            else if (extension == BulkLoader.TYPE_VIDEO ||BulkLoader.VIDEO_EXTENSIONS.indexOf(extension) > -1)
            {
                type = BulkLoader.TYPE_VIDEO;
            }
            else if (extension == BulkLoader.TYPE_XML ||BulkLoader.XML_EXTENSIONS.indexOf(extension) > -1)
            {
                type = BulkLoader.TYPE_XML;
            }
            else if (extension == BulkLoader.TYPE_MOVIECLIP ||BulkLoader.MOVIECLIP_EXTENSIONS.indexOf(extension) > -1)
            {
                type = BulkLoader.TYPE_MOVIECLIP;
            }
            else
            {
                // 是否为自定义新扩展名
                for(var checkType : String in CustomTypesExtensions)
                {
                	if (checkType == extension)
                	{
                		type = CustomTypesExtensions[checkType];
                		break;
                	}
                }
                if (!type) 
                {
                	type = BulkLoader.TYPE_TEXT;
                }
            }
            return type;
        }
        
        /** Private Variable **************************************************/

		private var dictItems:Dictionary = new Dictionary();	// 加载对象集合
        private var items:Array          = [];	    			// 加载对象数组
		private var workItems:Array      = [];

        private var maxConnections 		 : int    = 12;
        private var currentConnections   : int    = 0;			// 当前已用连接数
        private var itemsTotal 		     : int    = 0;			// 下载项总数
        private var itemsLoaded 	     : int    = 0;			// 下载项完成数
        private var totalWeight          : int    = 0;			// 下载对象总比例系数
        private var bytesTotal           : int    = 0;			// 总下载字段数
        private var bytesLoaded          : int    = 0;			// 当前已下载字段数
        private var itemsSpeed           : Number = 0;			// 平均下载速度
        private var weightPercent        : Number = 0;			// 总下载比例
        
        public var Version : String;
		
		/** Public Static **************************************************/
		
		public static const TYPE_BINARY     : String = "binary";
        public static const TYPE_IMAGE      : String = "image";
        public static const TYPE_MOVIECLIP  : String = "movieclip";
        public static const TYPE_SOUND      : String = "sound";
        public static const TYPE_TEXT       : String = "text";
        public static const TYPE_XML        : String = "xml";
        public static const TYPE_VIDEO      : String = "video";
        public static const AVAILABLE_TYPES : Array  = [TYPE_VIDEO, TYPE_XML, TYPE_TEXT, TYPE_SOUND, TYPE_MOVIECLIP, TYPE_IMAGE, TYPE_BINARY]
        
        /** 支持的文件扩展名 */
        public static var AVAILABLE_EXTENSIONS : Array = ["swf", "jpg", "jpeg", "gif", "png", "flv", "mp3", "xml", "txt", "js"];
        public static var IMAGE_EXTENSIONS     : Array = ["jpg", "jpeg", "gif", "png"];
        public static var MOVIECLIP_EXTENSIONS : Array = ['swf'];
        public static var TEXT_EXTENSIONS      : Array = ["txt", "js", "php", "asp", "py"];
        public static var VIDEO_EXTENSIONS     : Array = ["flv", "f4v", "f4p", "mp4"];
        public static var SOUND_EXTENSIONS     : Array = ["mp3", "f4a", "f4b"];
        public static var XML_EXTENSIONS       : Array = ["xml"];
        
        /** 自定义加载地址扩展名 */
        public var CustomTypesExtensions : Dictionary = new Dictionary();
    	
    	public static const CAN_BEGIN_PLAYING : String = "canBeginPlaying";
        
        public static var TYPE_CLASSES : Object = 
        {
            image     : ImageItem,
            movieclip : ImageItem,
            xml       : XMLItem,
            video     : VideoItem,
            sound     : SoundItem,
            text      : URLItem,
            binary    : BinaryItem
        }
        
        public static function TruncateNumber(raw:Number, decimals:int=2):Number 
        {
            var power:int = Math.pow(10, decimals);
           return Math.round(raw * ( power )) / power;
        }
	}
}