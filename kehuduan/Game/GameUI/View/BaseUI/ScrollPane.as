package GameUI.View.BaseUI
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	public class ScrollPane 
	{
		private var stage:Sprite 		= null;
		private var scrollBar:MovieClip = null;
		private var container:MovieClip = null;
		private var curMouseY:Number 	= 0;
		private var tmpY:Number 		= 0;
		
		public function ScrollPane(stage:Sprite)
		{
			this.stage = stage;
		}
		
		/**
		 *	设置元件 
		 *  scrollBar 拖动块  
		 * 			 |- btnUp 向上按钮
		 * 			 |-	btnDown 向上按钮	
		 * 			 |-	mcDragBar 拖动条	
		 *  container 拖动块  
		 * 			 |- 遮罩层
		 * 			 |- 显示层
		 **/  
		public function SetSymbol(scrollBar:MovieClip, container:MovieClip):void
		{
			this.scrollBar = scrollBar;
			this.container = container;
			configureListeners();
			countScrollBarHeight();
		}
		
		private function configureListeners():void
		{
			scrollBar.btnUp.addEventListener( MouseEvent.CLICK, btnUpHandler );
			scrollBar.btnDown.addEventListener( MouseEvent.CLICK, btnDownHandler );
			scrollBar.mcDragBar.addEventListener( MouseEvent.MOUSE_DOWN, mcDragBarMouseDownHandler );
		}
		
		//计算滑块的高度
		private function countScrollBarHeight():void
		{
			var h:Number = show.height;
			if (h>container.mcMask.height) 
			{
				scrollBar.mcDragBar.height = container.mcMask.height * ( container.mcMask.height / h );
				scrollBar.visible = true;
				if (scrollBar.mcDragBar.height+scrollBar.mcDragBar.y > container.mcMask.height) 
				{
					scrollBar.mcDragBar.y=container.mcMask.height-scrollBar.mcDragBar.height;
				}
				else 
				{
					scrollBar.visible=false;
				}
			}
		}
		
		//鼠标按下滑动条
		private function mcDragBarMouseDownHandler( evt:MouseEvent ):void 
		{
			curMouseY = stage.mouseY-scrollBar.dragBar.y;
			stage.addEventListener( MouseEvent.MOUSE_MOVE, moveHandler );
			stage.addEventListener( MouseEvent.MOUSE_UP,     upHandler );
		}
		
		//鼠标移开滑动条
		private function upHandler( evt:MouseEvent ):void 
		{
			stage.removeEventListener( MouseEvent.MOUSE_MOVE, moveHandler );
			stage.removeEventListener( MouseEvent.MOUSE_UP,     upHandler );
			moveHandler();
		}
		
		//滑块移动
		function moveHandler( evt:MouseEvent = null ):void {
			scrollBar.dragBar.y = stage.mouseY - curMouseY;
			if(this.tmpY > scrollBar.mcDragBar.y) 
			{
				countChildPlace(true);
			} 
			else 
			{
				countChildPlace(false);
			}
		}
		
		//向上按钮
		private function upBtnHandler( evt:MouseEvent ):void 
		{
			scrollBar.mcDragBar.y -= 10;
			countChildPlace(true);
		}

		//向下按钮
		private function downBtnHandler( evt:MouseEvent ):void 
		{
			scrollBar.mcDragBar.y += 10;
			countChildPlace(false);
		}
		
		//计算出mcDragBar的位置
		private function countChildPlace(bool:Boolean):void 
		{
			if ( scrollBar.mcDragBar.y + scrollBar.btnDown.height > container.mackMc.height - scrollBar.mcDragBar.height ) 
			{ 
				scrollBar.mcDragBar.y = container.mackMc.height - scrollBar.mcDragBar.height - scrollBar.btnDown.height; 
			}
			if ( scrollBar.mcDragBar.y < scrollBar.btnUp.height) 
			{ 
				scrollBar.mcDragBar.y =  scrollBar.btnUp.height; 
			}
			tmpY = scrollBar.mcDragBar.y;
			if(bool) 
			{
				show.y = -( (scrollBar.mcDragBar.y-scrollBar.btnUp.height) * show.height / container.mcMask.height );
			} 
			else 
			{
				show.y = -( (scrollBar.mcDragBar.y+scrollBar.btnUp.height) * show.height / container.mcMask.height );
			}
		}
	}
}