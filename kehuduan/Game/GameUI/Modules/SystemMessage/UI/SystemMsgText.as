package GameUI.Modules.SystemMessage.UI
{
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	
	
	/**
	 *  系统消息文本面板
	 * @author felix
	 * 
	 */	
	public class SystemMsgText extends UISprite
	{
		protected var _w:uint;
		protected var _h:uint;
		
		/**
		 *  文本内容
		 */		
		protected var _desText:String;
		
		/**
		 * 文本组件 
		 */		
		protected var tf:TaskText;
		/**
		 * 文本容器 
		 */		  
		protected var container:UIScrollPane;
		
		
		public function SystemMsgText(w:uint,h:uint)
		{
			super();
			this._w=w;
			this._h=h;
			this.createChildren();
		}
		
		/**
		 * 创建子元素 
		 * 
		 */		
		protected function createChildren():void{
			this.tf=new TaskText(this._w-16);
			this.container=new UIScrollPane(this.tf);
			this.container.width=this._w;
			this.container.height=this._h;
			this.container.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.addChild(this.container);
			this.container.refresh();
			
		}
		
		/**
		 * 布局 
		 * 
		 */		 
		protected function doLayout():void{
			this.container.refresh();
		}
		
		
		/**
		 * 重绘 
		 * 
		 */		
		protected function toRepaint():void{
			this.tf.tfText=this._desText;
			this.doLayout();
			
		}
		
		/**
		 *  设置文本内容
		 * @param value :文本内容
		 * 
		 */		
		public function set desText(value:String):void{
			if(this._desText==value)return;
			this._desText=value;
			this.toRepaint();
		}
		
	}
}