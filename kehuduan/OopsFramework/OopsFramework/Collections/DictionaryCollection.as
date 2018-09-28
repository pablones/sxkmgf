package OopsFramework.Collections
{
	import OopsFramework.IDisposable;
	
	import flash.utils.Dictionary;
	import flash.utils.Proxy;
	import flash.utils.flash_proxy;
	
	public class DictionaryCollection extends Proxy implements IDisposable
	{
		private var count:int = 0;
		private var weakKeys:Boolean;
		private var dict:Dictionary;
		
		public function DictionaryCollection(weakKeys:Boolean=false)
		{
			this.weakKeys = weakKeys;
			dict	      = new Dictionary(this.weakKeys);
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.Clear();
			this.dict = null;
		}
		/** IDisposable End */
		
		public function Add(key:*,item:*):void
		{
			this[key] = item;
			count++;
		}
		
		public function Remove(key:*):Boolean
		{
			if(this.dict[key]!=null)
			{
				if(this.dict[key] is IDisposable)
				{
					IDisposable(this.dict[key]).Dispose();
				}
				this.dict[key] = null;
				delete this.dict[key];
				count--;
				return true;
			}
			return false;
		}
		
		public function Clear():void
		{
			for(var key:* in this.dict)
			{
				this.Remove(key);
			}
		}
		
		public function get Count():int 
		{
			return count;
		}
		
		//--------------------------------------------------//
		
		override flash_proxy function callProperty(methodName:*, ... args):* 
		{
			var res:*;
			switch (methodName.toString()) 
			{
				default:
					res = dict[methodName].apply(dict, args);
					break;
			}
			return res;
		}
		
		override flash_proxy function getProperty(name:*):* 
		{
			return dict[name];
		}
		
		override flash_proxy function setProperty(name:*, value:*):void 
		{
			dict[name] = value;
		}
	}
}