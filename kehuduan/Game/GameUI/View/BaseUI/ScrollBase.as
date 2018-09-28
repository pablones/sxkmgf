package GameUI.View.BaseUI
{
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.Stage;
	import flash.events.MouseEvent;
	
	
	public class ScrollBase extends Sprite
	{
		private var s:Sprite = null;			/** 显示容器 */
		private var _stage:Stage = null;		/** 舞台 */
		private var maskMc:MovieClip = null;	/** 遮罩MC */
		private var scrollBar:MovieClip = null;	/** 滚动条+上下按钮MC */
		private var curMouseY:Number = 0;		/** 当前鼠标位置 */
		private var tmpY:Number = 0;			/** 缓存鼠标位置 */
		private var sbScale:Number = 0;			/** 滑动比例 */
		private var maskWidth:int = 0;			/** 遮罩宽度 */
		private var maskHeight:int = 0;			/** 遮罩高度 */
		private var barPosition:int = 0;		/** 滚动条位置，0-滚动条在右，1-滚动条在左 */
		private var itemList:Array = null;		/** 元素数据列表 */
		/////////////////////////////////////////////
		
		/**
		 * @param:stage		 舞台
		 * @param:maskWidth	 遮罩宽
		 * @param:maskHeight 遮罩高
		 * @param:barPos	 滚动条位置，0-滚动条在右，1-滚动条在左
		 */ 
		public function ScrollBase(stage:Stage, maskWidth:int, maskHeight:int, barPos:int=0){
			this._stage = stage;
			this.maskWidth = maskWidth;
			this.maskHeight = maskHeight;
			this.barPosition = barPos;
			init();
		}
		
		private function init():void {
			s = new Sprite();
			itemList = new Array();
			maskMc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MaskMc");
			scrollBar = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ScrollBar");
			maskMc.width = maskWidth;
			maskMc.height = maskHeight;
			
			s.mask = maskMc;
			
			if(barPosition == 0) {
				scrollBar.x = maskMc.width + 2;
			} else {
				s.x = scrollBar.width + 2;
				maskMc.x = scrollBar.width + 2;
			}
			
			this.addChild(maskMc);
			this.addChild(s);
			this.addChild(scrollBar);
			
			scrollBar.dragBar.addEventListener( MouseEvent.MOUSE_DOWN, downHandler );
			scrollBar.upBtn.addEventListener( MouseEvent.CLICK, upBtnHandler );
			scrollBar.downBtn.addEventListener( MouseEvent.CLICK, downBtnHandler );
		}
		
		/** 添加新条目 */
		public function addItem(child:DisplayObject):void
		{
			child.y = s.height;
			s.addChild(child);
			initPos();
			countScrollBarHeight();
		}
		
		/** 每次添加新条目 滚动条和内容都自动跳到最后 */
		private function initPos():void
		{
			if(s.height >= maskMc.height){
				s.y = -(s.height - maskMc.height);
				scrollBar.dragBar.y = maskMc.height - scrollBar.dragBar.height - scrollBar.downBtn.height; 
			}
		}
		
		/** 计算滑动比例 */
		private function countScrollBarHeight():void {
			var h:Number = s.height;
			if (h > maskMc.height) {
				sbScale = Number(maskMc.height-scrollBar.dragBar.height-scrollBar.upBtn.height-scrollBar.downBtn.height) / Number(h - maskMc.height);
				scrollBar.visible=true;
			} else {
				scrollBar.visible=false;
			}
		}
		
		/** 移动内容 */
		private function countChildPlace(bool:Boolean):void {
			if ( scrollBar.dragBar.y + scrollBar.downBtn.height > maskMc.height - scrollBar.dragBar.height ) { 
				scrollBar.dragBar.y = maskMc.height - scrollBar.dragBar.height - scrollBar.downBtn.height; 
			}
			if ( scrollBar.dragBar.y < scrollBar.upBtn.height) { 
				scrollBar.dragBar.y =  scrollBar.upBtn.height; 
			}
			tmpY = scrollBar.dragBar.y;
			if(bool) {
				var xx:Number = Number(scrollBar.dragBar.y - scrollBar.upBtn.height) / sbScale;
				s.y = s.y + (s.height - maskMc.height - xx);
				if(s.y > maskMc.y) s.y = maskMc.y;
			} else {
				var x:Number = Number(scrollBar.downBtn.y - (scrollBar.dragBar.y+scrollBar.dragBar.height)) /  sbScale; 
				s.y = s.y - (s.height - maskMc.height - x);
				if(s.y <= (maskMc.y + maskMc.height - s.height)) s.y = maskMc.y + maskMc.height - s.height;
			}
		}
		
		/** 鼠标按下 */
		private function downHandler( evt:MouseEvent ):void {
			curMouseY = _stage.mouseY - scrollBar.dragBar.y;
			_stage.addEventListener( MouseEvent.MOUSE_MOVE, moveHandler );
			_stage.addEventListener( MouseEvent.MOUSE_UP,     upHandler );
		}
		
		/** 鼠标移动 */
		private function moveHandler( evt:MouseEvent = null ):void {
			scrollBar.dragBar.y = _stage.mouseY - curMouseY;
			if(tmpY > scrollBar.dragBar.y) {
				countChildPlace(true);
			} else {
				countChildPlace(false);
			}
		}
		
		/** 鼠标抬起 */
		private function upHandler( evt:MouseEvent ):void {
			_stage.removeEventListener( MouseEvent.MOUSE_MOVE, moveHandler );
			_stage.removeEventListener( MouseEvent.MOUSE_UP,     upHandler );
			moveHandler();
		}
		
		/** 向上按钮 */
		private function upBtnHandler( evt:MouseEvent ):void {
			scrollBar.dragBar.y -= (maskMc.height-scrollBar.dragBar.height-scrollBar.upBtn.height-scrollBar.downBtn.height) / itemList.length;
			countChildPlace(true);
		}
		
		/** 向下按钮 */
		private function downBtnHandler( evt:MouseEvent ):void {
			scrollBar.dragBar.y += (maskMc.height-scrollBar.dragBar.height-scrollBar.upBtn.height-scrollBar.downBtn.height) / itemList.length;
			countChildPlace(false);
		}
		
		/** 三段缩放 1=1/3,  2=2/3,  3=1.0 */
		public function scaleHeight(scale:int=2):void
		{
			var ht:Number = 0;
			
			for(var i:int = 0; i < itemList.length; i++) {
				ht += itemList[i].height;
			}
			
			if(scale == 2) {			//缩小成 2/3
				var h:Number = Number(2/3) * maskHeight;
				maskMc.height = h;
				maskMc.y 	= maskHeight - h;
				s.y		 	= maskHeight - s.height; 
				scrollBar.y = maskHeight - h;
				scrollBar.downBtn.y = scrollBar.downBtn.y - maskMc.y;
				scrollBar.dragBar.y = scrollBar.downBtn.y - scrollBar.downBtn.height - scrollBar.dragBar.height;
				countScrollBarHeight();
			} else if(scale == 1) {		//缩小成 1/3
				var hh:Number = Number(1/3) * maskHeight;
				maskMc.height = hh;
				maskMc.y 	= maskHeight - hh;
				s.y		 	= maskHeight - s.height; 
				scrollBar.y = maskHeight - hh;
				scrollBar.downBtn.y = scrollBar.downBtn.y - hh;
				scrollBar.dragBar.y = scrollBar.downBtn.y - scrollBar.dragBar.height;
				countScrollBarHeight();
			} else if(scale == 3) {		//恢复初始大小
				maskMc.height = maskHeight;
				maskMc.y = 0;
				s.y	     = maskHeight - s.height;
				scrollBar.y = 0;
				scrollBar.downBtn.y = maskHeight - scrollBar.downBtn.height;
				scrollBar.dragBar.y = maskHeight - scrollBar.downBtn.height - scrollBar.dragBar.height;
				countScrollBarHeight();
			}
		}
		
	}
}