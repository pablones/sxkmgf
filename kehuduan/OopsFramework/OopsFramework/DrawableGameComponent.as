package OopsFramework
{
	import OopsFramework.Content.Provider.ResourceProvider;
	
	/** 游戏可视组件模板 */
	public class DrawableGameComponent extends GameComponent implements IDrawable
	{
		private var initialized    : Boolean = false;					// 是否初始化完成
		private var isLocalContent : Boolean = false;					// 是否为本地资源
		protected var ResourceProviderEntity:ResourceProvider = null;	// 资源提供器
		
		private var drawOrderChanged:Function;
		private var visibleChanged:Function;
		
		public function DrawableGameComponent(game:Game)
		{
			super(game);
		}
		
		/** IDisposable Start */
		public override function Dispose():void
		{
			this.drawOrderChanged = null;
			this.visibleChanged   = null;
			
			if(this.ResourceProviderEntity != null)
			{
				this.ResourceProviderEntity.Dispose();
				this.ResourceProviderEntity = null;
			}
			
			if(this.parent != null)
			{
				this.parent.removeChild(this);
			}
			
			super.Dispose();
		}
		/** IDisposable End */
		
		public override function Initialize():void
		{
			if(!this.initialized)
			{
				if(this.ResourceProviderEntity == null)
				{
					this.LoadContent();
				}
				else
				{
					this.ResourceProviderEntity.LoadComplete = LoadContent;
					this.ResourceProviderEntity.Load();
				}
			}
			this.initialized = true;
		}
		
		protected virtual function LoadContent():void
		{
			this.Enabled = true;		// 资原加载完成后，逻辑更新和重绘在主游戏循环中执行
		}
		
		/** IDrawable Start */
		/** 游戏组件是否可见 */
		public function get Visible():Boolean
		{
			return this.visible;
		}
		public function set Visible(value:Boolean):void
		{
			if (this.visible != value)
			{
				this.visible = value;
				if(visibleChanged!=null) visibleChanged(this);
			}
		}
		
		/** 绘制优先级 */
		public function get DrawOrder():int
		{
			if(this.parent != null)
			{
				return this.parent.getChildIndex(this);
			}
			return -1;
		}
		public function set DrawOrder(value:int):void
		{
			if(this.parent != null)
			{
				var totalCount:int = this.parent.numChildren - 1
				if(value >= totalCount)
				{
					value = totalCount;
				}
				this.parent.setChildIndex(this,value);
				
				if(drawOrderChanged!=null) drawOrderChanged(this);
			}
		}
		
		/** 游戏组件是否可见开关修改事件 */
		public function get DrawOrderChanged():Function
		{
			return this.drawOrderChanged;
		}
		public function set DrawOrderChanged(value:Function):void
		{
			this.drawOrderChanged = value
		}
		
		/** 游戏更新开关值修改事件 */
		public function get VisibleChanged():Function
		{
			return this.visibleChanged;
		}
		public function set VisibleChanged(value:Function):void
		{
			this.visibleChanged = value
		}
		/** IDrawable End */
	}
}