package OopsFramework.Content.Provider
{
	import OopsFramework.Content.ContentTypeReader;
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.BulkProgressEvent;
	import OopsFramework.Game;
	
    /** HTTP方式大型文件批量下载模板 */
	public class BulkLoaderResourceProvider extends ResourceProvider
	{
		private var itemsErrored:uint = 0;
		private var itemsLoaded:uint  = 0;
		private var itemsTotal:uint   = 0;
	    private var download:BulkLoader  = null;
	    
	    public function get Download():BulkLoader
	    {
	    	return this.download;
	    }
	
		/** 
		 * 创建一个BulkLoader资源读取器(registerAsProvider为true为自动注册当前读取器) 
		 * game 						游戏主体
		 * Connections					下载资源并行连接数	
		 * registerResourceProviders 	是否自动注册到资料管理器中 － ContentManager
		 **/
		public function BulkLoaderResourceProvider(ConnectionCount:int = 1, game:Game = null)
		{
			super(game);
			
			download = new BulkLoader(ConnectionCount);
			download.addEventListener(BulkProgressEvent.OPEN    , onOpen);
			download.addEventListener(BulkProgressEvent.PROGRESS, onBulkProgress);
			download.addEventListener(BulkProgressEvent.COMPLETE, onBulkComplete);
			download.addEventListener(BulkProgressEvent.ERROR   , onBulkError);
		}
		
		public override function Load():void
		{	
			if(this.download.ItemsTotal > 0)
			{
				this.download.Load();
				super.Load();
			}
			else
			{
				if(LoadComplete!=null)
		    	{
		    		LoadComplete();
		    	}
			}
		}
		
		/** 下载开始事件 */
		protected virtual function onOpen(e:BulkProgressEvent):void { }
		
		/** 下载进度事件 */
	    protected virtual function onBulkProgress(e:BulkProgressEvent):void { }
		
		/** 下载项完成事件 */
		protected virtual function onBulkComplete(e:BulkProgressEvent):void
	    {
	    	// 添加下载的资源到此资源提供器的集合中
	    	var res:ContentTypeReader = new ContentTypeReader();
	    	res.Name			 	  = e.Item.name;
	    	res.Content               = e.Item.content;
        	addResource(res);
        	
        	BulkCompleteAll(e);
	    } 
	   
		/** 所有下载项完成事件 */
	    protected virtual function onBulkCompleteAll():void 
	    { 
			this.Download.Dispose();
			
	    	if(LoadComplete!=null)
	    	{
	    		LoadComplete();
	    	}
	    }
		
		/** 下载出错事件 */
		protected virtual function onBulkError(e:BulkProgressEvent):void 
		{
			itemsErrored++;
			BulkCompleteAll(e);
		}
		
		private function BulkCompleteAll(e:BulkProgressEvent):void
		{
			itemsLoaded  = e.ItemsLoaded;
        	itemsTotal   = e.ItemsTotal;

			if(itemsTotal == itemsLoaded + itemsErrored)
	    	{
	    		Download.removeEventListener(BulkProgressEvent.PROGRESS, onBulkProgress);
				Download.removeEventListener(BulkProgressEvent.COMPLETE, onBulkComplete);
				Download.removeEventListener(BulkProgressEvent.ERROR   , onBulkError);
	    		this.isLoaded = true;
	    	
	    		onBulkCompleteAll();
	    	}
		}
	}
}