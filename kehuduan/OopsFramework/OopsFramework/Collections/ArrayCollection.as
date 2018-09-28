package OopsFramework.Collections
{
	import OopsFramework.Exception.ExceptionResources;
	import OopsFramework.IDisposable;
	
	import flash.utils.Proxy;
	import flash.utils.flash_proxy;
	
	/** 暂时没用  */
	public dynamic class ArrayCollection extends Proxy implements IDisposable
	{
	    private var array     : Array;
		private var allowSame : Boolean;
	    
	    /** 添加组件完成事件 */
		public var Added:Function;
		/** 删除组件完成事件 */
		public var Removed:Function;
	
	    public function ArrayCollection(allowSame:Boolean = true) 
	    {
			this.allowSame = allowSame;
			this.array     = [];
	    }
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.Clear();			
			this.array   = null;
			this.Added   = null;
			this.Removed = null;
		}
		/** IDisposable End */
	    
	    public function Add(item:*):void
	    {
			if(this.allowSame == false)
			{
				var index:int = this.indexOf(item);
				if(index != -1)
				{
					throw new ArgumentError(ExceptionResources.CannotAddSameComponent);
				}
			}
			
	    	this.push(item);
	    	
	    	if(Added != null)
			{
				Added(item);
			}
	    }
	    
	    public function Remove(item:*):Boolean
	    {
			var index:int = this.indexOf(item);
			return this.RemoveAt(index);
	    }
		
		public function RemoveAt(index:int):Boolean
		{
			if(index > -1)
			{
				if(Removed != null)
				{
					Removed(this.array[index]);
				}
				
				if(this.array[index] is IDisposable)
				{
					IDisposable(this.array[index]).Dispose();
				}
				this.array[index] = null;
				this.splice(index,1);
				return true;
			}
			return false;
		}
		
		public function Clear():void
		{
			while(this.length != 0)
			{
				this.RemoveAt(this.length - 1);
			}
		}
	    
	    public function Shift():* 
	    {
	    	return this.shift();
	    }
	    
	    public function get Length():uint
	    {
	    	return this.length;
	    }
	    
	    public function Concat():Array
	    {
	    	return this.concat();
	    }
	    
	    public function SortOn(fieldName:Object, options:Object = null):Array
	    {
	    	return this.sortOn(fieldName,options);
	    }
	    
	    public function IndexOf(searchElement:Object,fromIndex:int = 0):int
	    {
	    	return this.indexOf(searchElement,fromIndex);
	    }
		    
	    //--------------------------------------------------//
	    
	    override flash_proxy function callProperty(methodName:*, ... args):* 
	    {
	        var res:*;
	        switch (methodName.toString()) 
	        {
	            default:
	                res = array[methodName].apply(array, args);
	                break;
	        }
	        return res;
	    }
	
	    override flash_proxy function getProperty(name:*):* 
	    {
	        return array[name];
	    }
	
	    override flash_proxy function setProperty(name:*, value:*):void 
	    {
			array[name] = value;
	    }
	}
}