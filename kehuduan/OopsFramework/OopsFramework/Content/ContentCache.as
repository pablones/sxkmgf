package OopsFramework.Content
{
	import OopsFramework.Collections.DictionaryCollection;
	
	import flash.system.System;
	import flash.utils.Dictionary;

	/** 资源缓冲管理 */
	public class ContentCache
	{
		public  var permanentStorage:Dictionary 		  = new Dictionary();				// 永久缓冲区
		public var strategyStorage:DictionaryCollection  = new DictionaryCollection();		// 策略缓冲区
		
		/** 策略缓冲区容量 */
		public var StrategyStorageMemory:int = 524288000;
		
		/** 策略缓冲区资源数量 */
		public function get StrategyStorageCount():int
		{
			return this.strategyStorage.Count;
		}
		
		/** 删除策略缓冲区所有资源 */
		public function ClearStrategyStorage():void
		{
			this.strategyStorage.Clear();
		}
		
		/** 获取策略缓冲区数据 */
		public function GetStrategyStorage(key:String):ContentTypeReader
		{
			if(this.strategyStorage[key]!=null)
			{
				return this.strategyStorage[key];
			}
			return null;
		}
		
		/** 添加策略缓冲区数据 */
		public function AddStrategyStorage(key:String, data:* = null):void
		{
			var ctr:ContentTypeReader = this.GetStrategyStorage(key);
			if(ctr == null)
			{
				if(data is ContentTypeReader)
				{
					this.strategyStorage.Add(key, data);
				}
				else
				{
					ctr				 		  = new ContentTypeReader();
					ctr.Name				  = key;
					ctr.Content				  = data;
					this.strategyStorage.Add(key, ctr);
				}
			}
			else
			{
				ctr.Name				  = key;
				ctr.Content 			  = data;
				this.strategyStorage[key] = ctr;
			}
			
			// 回收内存
//			this.CallbackCache();
		}
		
		/** 回收内存（内存使用超过512MB时，删除最长时候没使用过的资源） */
		private function CallbackCache():void
		{
			if(System.totalMemory > this.StrategyStorageMemory)
			{
				var delKey:ContentTypeReader;
				for each(var ctr:ContentTypeReader in this.strategyStorage)
				{
					if(delKey == null)
					{
						delKey = ctr;
					}
					else
					{
						if(delKey.UseIntervals < ctr.UseIntervals)		// 删除最长时间没使用的资源
						{
							delKey = ctr;
						}
					}
				}
	
				if(delKey!=null && delKey.State == ContentTypeReader.STATE_USED)
				{
					this.RemoveStrategyStorage(delKey.Name);
				}
			}
		}
		
		/** 删除策略缓冲区数据 */
		public function RemoveStrategyStorage(key:String):void
		{
			this.strategyStorage.Remove(key);
		}
		
		/** 添加永久缓冲区数据 */
		public function AddPermanentStorage(key:String, data:ContentTypeReader):void
		{
			this.permanentStorage[key] = data;
		}
		
		/** 删除永久缓冲区数据 */
		public function RemovePermanentStorage(key:String):void
		{
			delete this.permanentStorage[key];
		}
	}
}