package OopsFramework.Content.Provider
{
	import OopsFramework.Collections.DictionaryCollection;
	import OopsFramework.Content.ContentTypeReader;
	import OopsFramework.Game;
	import OopsFramework.IDisposable;
	
    /** 该ResourceProviderBase类可以扩展到创建ResourceProvider将自动注册的ResourceManager */
	public class ResourceProvider implements IResourceProvider, IDisposable
	{
		public var LoadComplete : Function = null;				// 资源提供器加载完有资源完成事件
		protected var isLoaded  : Boolean  = false;				// 资源提供者是否加载资源完成
        protected var resources : DictionaryCollection;			// 资源集合
		private var game:Game;
		
		public function get Games():Game
		{
			return this.game;
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.resources.Dispose();							// 经验：如果此处报错，一般为同前对象的 Dispose() 方法被调用了两次，导致 resources 对象已为空了。
			this.resources    = null;
			this.LoadComplete = null;
		}
		/** IDisposable End */
        
        /** 
		 * 资源提供器
		 * game 						游戏主体
		 * registerResourceProviders 	是否自动注册到资料管理器中 － ContentManager
		 **/
		public function ResourceProvider(game:Game = null)
		{
			if(game!=null)
			{
				this.game = game;
				this.game.Content.ResourceProviders.push(this);
			}
			resources = new DictionaryCollection();
		}
		
        /** 此方法将检查资料在资料集合中是否存在（true 为存在） */
		public function IsResourceExist(name:String):Boolean
		{
			return (resources[name]!=null)
		}
		
        /** 获取指定资料对象 */
		public function GetResource(name:String):ContentTypeReader
		{
			return resources[name] as ContentTypeReader; 
			
		}
		
		/** 开始加载资料 */
		public virtual function Load():void { }

        /** 添加指定资料对象 */
        protected function addResource(res:ContentTypeReader):void
        {
            resources[res.Name] = res;        	
        }
        
        /** 是否加载资源完成 */
        public function get IsLoaded():Boolean
        {
        	return this.isLoaded;
        }
	}
}