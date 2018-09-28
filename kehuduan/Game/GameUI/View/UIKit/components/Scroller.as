package GameUI.View.UIKit.components 
{
	import GameUI.View.UIKit.core.ScrollPolicy;
	import GameUI.View.UIKit.core.UIComponent;
	import GameUI.View.UIKit.events.ResizeEvent;
	import GameUI.View.UIKit.events.UIEvent;
	
	import flash.display.Graphics;
	import flash.events.Event;
	/**
	 * ...
	 * @author statm
	 */
	public class Scroller extends UIComponent
	{
		
		public function Scroller():void 
		{
			super();
		}
		
		// zhao: 现在没这需求先不写，回头再说。
		//protected var horizontalScrollBar:HScrollBar;
		
		protected var vsb:VScrollBar; // zhao: 这个需要抽象/接口化吗？需要的时候提取一下 VSB 吧。
		
		public function get verticalScrollBar():VScrollBar
		{
			return vsb;
		}
		
		public function set verticalScrollBar(value:VScrollBar):void
		{
			if (!value || (value == vsb)) return;
			
			if (vsb && this.contains(vsb))
			{
				this.removeChild(vsb);
				vsb.removeEventListener(UIEvent.VALUE_COMMIT, vsb_valueCommitHandler);
			}
			
			vsb = value;
			
			invalidateLayout();
		}
		
		
		// ----------------------------------------
		//
		//		内容区
		//
		// ----------------------------------------
		
		protected var _content:UIComponent;
		
		public function get content():UIComponent
		{
			return _content;
		}
		
		public function set content(value:UIComponent):void
		{
			if (value == _content) return;
			
			if (_content)
			{
				_content.removeEventListener(ResizeEvent.RESIZE, content_resizeHandler);
				if (this.contains(_content))
				{
					removeChild(_content);
				}
			}
			
			_content = value;
			
			if (_content)
			{
				_content.clipAndEnableScrolling = true;
				
				_content.addEventListener(ResizeEvent.RESIZE, content_resizeHandler);
				
				this.addChild(_content);
				invalidateLayout();
			}
		}
		
		protected function content_resizeHandler(event:ResizeEvent):void
		{
			invalidateLayout();
		}
		
		
		
		// ----------------------------------------
		//
		//		滚动条规则
		//
		// ----------------------------------------
		
		protected var hScrollPolicy:String = ScrollPolicy.AUTO;
		
		public function get horizontalScrollPolicy():String
		{
			return hScrollPolicy;
		}
		
		public function set horizontalScrollPolicy(value:String):void
		{
			if (value == hScrollPolicy) return;
			
			hScrollPolicy = value;
			
			invalidateLayout();
		}
		
		protected var vScrollPolicy:String = ScrollPolicy.AUTO;
		
		public function get verticalScrollPolicy():String
		{
			return vScrollPolicy;
		}
		
		public function set verticalScrollPolicy(value:String):void
		{
			if (value == vScrollPolicy) return;
			
			vScrollPolicy = value;
			
			invalidateLayout();
		}
		
		
		
		
		// ----------------------------------------
		//
		//		尺寸
		//
		// ----------------------------------------
		
		override public function set width(value:Number):void
		{
			if (super.width == value) return;
			
			super.width = value;
			
			invalidateLayout();
		}
		
		override public function set height(value:Number):void
		{
			if (super.height == value) return;
			
			super.height = value;
			
			invalidateLayout();
		}
		
		
		
		
		// ----------------------------------------
		//
		//		布局
		//
		// ----------------------------------------
		
		protected var layoutInvalidated:Boolean = false;
		
		override public function invalidateLayout():void
		{
			if (layoutInvalidated) return;
			
			layoutInvalidated = true;
			this.addEventListener(Event.ENTER_FRAME, _doLayout);
		}
		
		protected function _doLayout(event:Event):void
		{	
			// 这里完成滚动条的摆放控制
			layoutInvalidated = false;
			this.removeEventListener(Event.ENTER_FRAME, _doLayout);
			if (!content) return;
			
//			var g:Graphics = this.graphics;
//			g.lineStyle(1, 0xFF9900);
//			g.drawRect(0, 0, this.width, this.height);
			
			// 视口边界控制
			if (content.viewport.scrollPosX < 0)
			{
				content.viewport.scrollPosX = 0;
			}
			if (content.viewport.scrollPosX + content.viewport.width > content.width)
			{
				content.viewport.scrollPosX = content.width - content.viewport.width;
			}
			if (content.viewport.scrollPosY < 0)
			{
				content.viewport.scrollPosY = 0;
				if (vsb) vsb.setValue(0, false, true);
			}
			if (content.viewport.scrollPosY + content.viewport.height > content.height)
			{
				content.viewport.scrollPosY = content.height - content.viewport.height;
				if (vsb) vsb.setValue(content.viewport.scrollPosY, false, true);
			}
			
			// zhao: HSB 的部分以后再说。
			var needVSB:Boolean = (verticalScrollPolicy == ScrollPolicy.ALWAYS)
								   || ((content.height > this.height) && (verticalScrollPolicy == ScrollPolicy.AUTO));
			//trace(">>> needVSB=" + needVSB);
			if (needVSB)
			{
				if (!vsb)
				{
					vsb = new VScrollBar();
					this.addChild(vsb);
					vsb.addEventListener(UIEvent.VALUE_COMMIT, vsb_valueCommitHandler);
				}
				else if (!this.contains(vsb))
				{
					this.addChild(vsb);
					vsb.addEventListener(UIEvent.VALUE_COMMIT, vsb_valueCommitHandler);
				}
				
				vsb.x = this.width - vsb.width;
				vsb.height = this.height;
				vsb.minimum = 0;
				vsb.maximum = content.height - this.height;
				vsb.stepSize = 20;
				vsb.pageSize = this.height * .9;
				
				content.viewport.width = this.width - vsb.width;
				content.viewport.height = this.height;
			}
			else
			{
				if (vsb && this.contains(vsb))
				{
					this.removeChild(vsb);
					vsb.removeEventListener(UIEvent.VALUE_COMMIT, vsb_valueCommitHandler);
					vsb = null;
				}
				
				content.viewport.width = this.width;
				content.viewport.height = this.height;
			}
		}
		
		
		
		// ----------------------------------------
		//
		//		滚动条动作
		//
		// ----------------------------------------
		
		protected function vsb_valueCommitHandler(event:UIEvent):void
		{
			content.viewport.scrollPosY = vsb.value;
			stage.invalidate();
		}
	}

}