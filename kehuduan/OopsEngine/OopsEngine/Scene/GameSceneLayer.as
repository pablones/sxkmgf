package OopsEngine.Scene
{
	import OopsFramework.Collections.ArrayCollection;
	import OopsFramework.GameTime;
	import OopsFramework.IDisposable;
	import OopsFramework.IUpdateable;
	
	import flash.display.Sprite;
	
	/** 场景图层数据 */
	public class GameSceneLayer extends Sprite implements IDisposable
	{
		private var drawableElements:Array;				// 绘制元件集合
		private var elements:ArrayCollection;		// 图层实体元件集合(GameElement)

		/** 图层元件集合 */
		public function get Elements():ArrayCollection
		{
			return this.elements;
		}
		
		public function GameSceneLayer()
		{
			this.mouseEnabled	  = false;
			this.elements 		  = new ArrayCollection();
			this.elements.Added   = onGameElementAdded;
			this.drawableElements = [];
		}
		
		/** 添加元件成功事件 */
		private function onGameElementAdded(item:GameElement):void
		{
			item.Initialize();
//			item.Disposed = onItemDisposed;
			this.addChild(item);
			this.DepthSort();
		}
		
//		private function onItemDisposed(e:EntityElement):void
//		{
//			this.Elements.Remove(e);
//		}
		
		/** 元件深度交换 */
		public function DepthSort(e:GameElement = null):void
		{
			if(this.elements.Length > 1)
			{
				this.elements.SortOn("Depth",Array.NUMERIC);
				
				var endIndex:int;
				if(e!=null && this.contains(e))
				{
					endIndex = this.getChildIndex(e);
				}
				else
				{
					endIndex = this.elements.Length - 1;
				}
				
				var array:Array = this.elements.Concat();
				for (var i:int = 0; i < endIndex; i++)
	            {
	            	// 经验：如果直接用GameElement的Dispose方法，只会删除层中对象，不会删除elements中的对象，
	            	//      所以程序会出错（现在在EntityElement对象中用事件通知本对象删除elements中对相数据）
	            	if(array[i] != null && array[i+1] != null && array[i].Depth != array[i + 1].Depth)
	            	{
            			if(this.numChildren > i && this.contains(array[i]))
            			{
            				this.setChildIndex(this.elements[i],i);
            			}
	            	}		
	            }
                if(this.numChildren > endIndex && this.contains(array[endIndex]))
            	{
					this.setChildIndex(array[endIndex],endIndex);
            	}         
			}
 		}
		
		public function Update(gameTime:GameTime):void
		{
            this.drawableElements = this.elements.Concat();
			for each(var updateable:IUpdateable in  this.drawableElements)
			{
				if (updateable.Enabled)
				{
					updateable.Update(gameTime);
				}
			}
			this.drawableElements = [];
		}
		
		/** 清除层上的元件  */
		public function ClearElement():void
		{
//			for(var i:int = 0;i < this.elements.Count ; i++)
//			{
//				this.elements[i].Dispose();
//			}
			this.elements.Clear();
			this.drawableElements = [];
			
			while(this.numChildren!=0)
			{
				this.removeChildAt(this.numChildren - 1);
			}
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
//			for(var i:int = 0;i < this.elements.Count ; i++)
//			{
//				this.elements[i].Dispose();
//			}
			this.elements.Dispose();
			this.elements 		  = null;
			this.drawableElements = null;
			
			while(this.numChildren!=0)
			{
				this.removeChildAt(this.numChildren - 1);
			}
			
			// 父级对象移除
			if(this.parent != null)
			{
				this.parent.removeChild(this);
			}
		}
		/** IDisposable End */
	}
}