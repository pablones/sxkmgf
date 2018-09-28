package GameUI.View
{
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.BulkProgressEvent;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.utils.Dictionary;
	
	
	/**
	 * 异步资源生成工厂
	 * @author felix
	 * 
	 */	
	public class ResourcesFactory
	{
		/**单例 */
		private static var _instance:ResourcesFactory;
		/** 回调处理集合 [url]=[fun,fun,fun]*/
		private var _callBackDic:Dictionary;  
		/** 资源字典 [url]=[contentReader,contentReader] */
		private var _resourceDic:Dictionary;
		/** 加载标记字典 [url]=1 ：正在加载  [url]=2:已经加载完毕 */  
		private var _loadDic:Dictionary;
		/** 多资源加载器 */
		private var bulkLoader:BulkLoader;
		
	
		public function ResourcesFactory(simgle:Simgle)
		{
			this._callBackDic=new Dictionary();
			this._loadDic=new Dictionary();
			this._resourceDic=new Dictionary();
			this.bulkLoader=new BulkLoader();
			this.bulkLoader.addEventListener(BulkProgressEvent.COMPLETE,onCompleteHandler);
		}
	
	
		/**
		 * 资源加载完毕处理器 
		 * @param e 
		 * 
		 */		
		protected function onCompleteHandler(e:BulkProgressEvent):void{
			var url:String=e.Item.url;
			this._resourceDic[url]=e.Item.content;
			this._loadDic[url]=2;
			var arr:Array=this._callBackDic[url];
			if(arr!=null && arr.length>0){
				var len:uint=arr.length;
				for(var i:uint=0;i<len;i++){
					if(arr[i]==null)continue;
					arr[i]();
				}
				delete this._callBackDic[url];
			}	
		}
		
	
		/**
		 * 获取工厂单例 
		 * @return  ：工厂实例
		 * 
		 */		
		public static function getInstance():ResourcesFactory{
			if(_instance==null)_instance=new ResourcesFactory(new Simgle());
			return _instance;
		}
		
		/**
		 * 获取指定URL下的资源 
		 * @param url ：资源url地址
		 * @param callBack ：资源加载完成回调处理方法
		 * 
		 */
		
		public function getResource(url:String,callBack:Function):void{
			//新资源加载
			if(this._loadDic[url]==null){
				this._loadDic[url]=1;
				this._callBackDic[url]=[callBack];
				this.bulkLoader.Add(url);
				this.bulkLoader.Load();
			}else{
				//正在加载中。。。
				if(this._loadDic[url]==1){
					var arr:Array=this._callBackDic[url]==null ? [] : this._callBackDic[url];
					if(arr.indexOf(callBack)==-1){
						arr.push(callBack);
					}
				//已经加载完毕	
				}else if(this._loadDic[url]==2){
					callBack();
				}
			}
		}
		
		/**
		 * 找到该资源下的BitMap 
		 * @param url :资源地址
		 * @return 
		 * 
		 */		
		public function getBitMapResourceByUrl(url:String):Bitmap{
		
			var bitMap:Bitmap=Bitmap(this._resourceDic[url].content);
			if(bitMap!=null){
				return new Bitmap(bitMap.bitmapData.clone());	
			}
			return null;
		}
		
		/**
		 * 删除回调funtion 
		 * @param url
		 * @param callBack
		 * 
		 */		
		public function deleteCallBackFun(url:String,callBack:Function):void{
			var arr:Array=this._callBackDic[url] as Array;
			if(arr!=null){
				var index:int=arr.indexOf(callBack);
				if(index!=-1){
					arr.splice(index,1);
					this._callBackDic[url]=arr;
				}
			}			
		}
		
		/**
		 * 获取异步加载的MC 
		 * @param url
		 * @return 
		 * 
		 */		
		public function getMovieClip(url:String):MovieClip{
			var mc:MovieClip=MovieClip(this._resourceDic[url].content);
			return mc;
		}
		

	}
	
}
class Simgle{};