package OopsFramework.Content
{
	import OopsFramework.Content.Provider.IResourceProvider;
	
	import flash.utils.Dictionary;
	
	/** 管理内存中所有资源提共者对象中的资源引用 */
	public class ContentManager
	{
		/** 资源路径（file、http 路径格式) */
		public var RootDirectory:String;
		
		private var resourceProviders:Array;				// 资源供应者集合（IResourceProvider）
//		private var loadedAssets:Dictionary;				// 已载入资源集合（缓存中资源或对象数据）
		private var permanentStorage:Dictionary;			// 永久缓存资源
		
		public var Cache:ContentCache = new ContentCache();

		public function ContentManager()
		{
			this.resourceProviders = [];
			this.permanentStorage  = new Dictionary();
		}
		
		/** 从所有注册了的资源提供器中获取资源 */
		public function Load(assetName:String):ContentTypeReader
		{
			var Resource:* = this.permanentStorage[this.RootDirectory + assetName];
			if(Resource == null)
			{
				for (var i:int = 0; i < this.resourceProviders.length; i++)
				{
					var IProvider:IResourceProvider = this.resourceProviders[i] as IResourceProvider;
					if(IProvider!=null)
					{	
						if (IProvider.IsLoaded && IProvider.IsResourceExist(this.RootDirectory + assetName))
		                {
		                	this.permanentStorage[this.RootDirectory + assetName] = IProvider.GetResource(this.RootDirectory + assetName);
		               		Resource										      = this.permanentStorage[this.RootDirectory + assetName];
		                }
					}
				}
			}
			return Resource as ContentTypeReader;
		}
		
//		/** 缓存资源  */
//		public function get CacheResource():Dictionary
//		{
//			if(this.loadedAssets==null)
//			{
//				this.loadedAssets = new Dictionary();
//			}
//			return this.loadedAssets;
//		}
//		public function set CacheResource(value:Dictionary):void
//		{
//			this.loadedAssets = value;
//		}
		
		/** 资源提供者集合（注：提供 ResourceProvider 对象将加载的数据保存到缓存中 */
		public function get ResourceProviders():Array
        {
        	return this.resourceProviders;
        }
	}
}