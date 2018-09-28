package GameUI.View.Components
{
	import flash.display.*;
	import flash.events.*;
	import flash.geom.Rectangle;
	import flash.text.*;
	import flash.utils.Timer;

	public class UIScrollPane extends UISprite
	{
		private var thumbRect:Rectangle;				//拖动范围
		//Asset
		private var view:DisplayObject;					//显示对象
		private var scrollBar:Sprite;					//拖动条
		private var barBackGround:Sprite				//拖动条背景
		private var grip:Sprite;						//把手
		private var upArrow:SimpleButton;				//上按钮
		private var downArrow:SimpleButton;				//下按钮
		private var thumb:Sprite;						//滑块
		private var viewContainer:Sprite;				//显示对象容器
		private var textField:TextField;				//文本区
		private var viewMask:Sprite;					//遮罩层
		//
		private var arrowHeight:int;
		private var pos:int;							//显示对象所处的位置(y)
		private var max:int;							//显示对象的height
		private var step:int;
		private var paddings:Object;
		private var timer:Timer; 
		private var thumbDragging:Boolean;
		private var scrollDir:int;						//卷动方向
		private var programatic:Boolean;
		private var policy:int;
		
		public static const SCROLLBAR_NEVER:int = 2;
        public static const SCROLLBAR_ALWAYS:int = 0;
        public static const SCROLLBAR_AS_NEEDED:int = 1;
		
		public function UIScrollPane(child:DisplayObject) : void
        {
			thumbRect = new Rectangle();
			view = child;
			scrollBar = new Sprite();
			this.addChild(scrollBar);
			barBackGround = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BarBackground");//new BarBackground();
			barBackGround.addEventListener(MouseEvent.MOUSE_DOWN, onBarMouseDown);
			scrollBar.addChild(barBackGround);
			upArrow = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("UpArrow");//new UpArrow();
			upArrow.addEventListener(MouseEvent.MOUSE_DOWN, onUpArrowMouseDown);
			scrollBar.addChild(upArrow);
			downArrow = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("DownArrow");//new DownArrow();
			downArrow.addEventListener(MouseEvent.MOUSE_DOWN, onDownArrowMouseDown);
			scrollBar.addChild(downArrow);
			thumb = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Thumb");//new Thumb();
			thumb.y = arrowHeight;
			thumb.x = 1;
			thumb.visible = false;
			thumb.addEventListener(MouseEvent.MOUSE_DOWN, onThumbMouseDown);
			scrollBar.addChild(thumb);
			grip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Grip");//new Grip();
			grip.mouseEnabled = false;
			grip.visible = false;
			grip.x = int((thumb.width - grip.width) / 2) + 1;
			scrollBar.addChild(grip);
			viewContainer = new Sprite();
			viewContainer.name = "scrollBarViewContainer";
			//
			//viewContainer.mouseChildren = false;
			viewContainer.mouseEnabled = false;////////////////fcq
			//viewContainer.mouseChildren = false;
			if(view is TextField)
			{
				textField = new TextField();
				textField.addEventListener(Event.SCROLL, onTextScroll);
			}
			else
			{
				viewMask = new Sprite();
				viewMask.mouseEnabled = false;
				viewMask.graphics.lineStyle(0, 0, 0);
				viewMask.graphics.beginFill(0);
				viewMask.graphics.drawRect(0,0,view.width,view.height);
				viewMask.graphics.endFill();
				viewContainer.addChild(viewMask);
				viewContainer.mask = viewMask;
			}
			this.addChild(viewContainer);
			viewContainer.addChild(view);
			
			arrowHeight = upArrow.height + 1;
			thumbRect.x = thumb.x;
			thumbRect.y = arrowHeight;
			thumbRect.width = 0;
			pos = 0;
			max = 0;
			step = 15;
			paddings = {left:0, right:0, top:0, bottom:0};
			setPaddings(paddings);
			timer = new Timer(300, 0);
			timer.addEventListener(TimerEvent.TIMER, onTimer);
			this.addEventListener(Event.ADDED_TO_STAGE, onAddedToStage);
		}	
		
		protected var r:Sprite = new Sprite();
		
		//_____________________________________________________________________
		//EVENTS
		//拖动条在鼠标点击处
		private function onBarMouseDown(event:MouseEvent):void
		{
			var curMouseY:Number = this.mouseY;
			thumb.y = curMouseY - thumb.height/2;
			if(thumb.y < arrowHeight)	//小于拖动高度
			{
				thumb.y = arrowHeight;
			}
			if(thumb.y + thumb.height > this.iBackground.height - arrowHeight) //大于拖动高度
			{
				thumb.y = iBackground.height - arrowHeight - thumb.height;
			}
			grip.y = int(thumb.y + (thumb.height - grip.height) / 2);	//更新grip的位置
			updateScrollPos();
		}
		
		private function onThumbMouseDown(event:MouseEvent):void
		{
			thumb.startDrag(false, thumbRect);
			thumbDragging = true;
			stage.addEventListener(MouseEvent.MOUSE_MOVE, onStageMouseMove);
			event.stopPropagation();
		}
		
		private function onStageMouseMove(event:MouseEvent):void
		{
			if(thumbDragging)
			{
				grip.y = int(thumb.y + (thumb.height - grip.height) / 2);
				updateScrollPos();
			}
		}
		
		private function onUpArrowMouseDown(event:MouseEvent):void
		{
			if(max>0)
			{
				scrollUp();
                scrollDir = -1;
               	timer.start();
			}
			event.stopPropagation();
		}
		
		private function onDownArrowMouseDown(event:MouseEvent):void
		{
			if(max>0)
			{
				scrollDown();
                scrollDir = 1;
                timer.start();
			}
			event.stopPropagation();
		}
		
		private function onAddedToStage(event:Event):void
		{
			if(event.target == this)
			{
				stage.addEventListener(MouseEvent.MOUSE_UP, onStageMouseUp);
			}
		}
		
		private function onStageMouseUp(event:MouseEvent):void
		{
			if(scrollDir != 0)
			{
				timer.delay = 300;
				timer.reset();
				scrollDir = 0;
			}
			if(thumbDragging)
			{
				thumb.stopDrag();
				thumbDragging = false;
				if(stage != null)
				{
					stage.removeEventListener(MouseEvent.MOUSE_MOVE, onStageMouseMove);
				}
				grip.y = int(thumb.y + (thumb.height - grip.height)/2);
				updateScrollPos();
				thumb.y = this.arrowHeight + pos*(this.iBackground.height - thumb.height - 2 * arrowHeight) / max;
			}
		}
		
		private function onTextScroll(event:Event):void
		{
			if(!programatic)
			{
				//scrollPos = textField.scrollV--;
			}
		}
		
		private function onTimer(event:TimerEvent):void
		{
			if (timer.delay == 300)
            {
                timer.delay = 100;
            }
            if (scrollDir > 0)
            {
                if (pos == max)
                {
                    return;
                }
                pos = pos + step;
                if (pos > max - step / 2)
                {
                    pos = max;
                }
                updateThumbPos();
            }
            else
            {
                if (pos == 0)
                {
                    return;
                }
                pos = pos - step;
                if (pos < step / 2)
                {
                    pos = 0;
                }
                updateThumbPos();
            }
		}
		//_______________________________________________________________________________
		private function scrollUp():void
		{
			pos = pos - step;
			if(pos < step/2)
			{
				pos = 0;
			}
			updateThumbPos();
		}
		
		private function scrollDown():void
		{
			pos = pos + step;
			if(pos > max - step/2)
			{
				pos = max;
			}
			updateThumbPos();
		}
		
		//_______________________________________________________________________________
		//Protected
		protected function updateUI():void
		{
			var tmp:Number
			if(max>0)
			{
				if(this.policy == SCROLLBAR_AS_NEEDED)
				{
					scrollBar.visible = true;
				}
				if(textField != null)
				{
					tmp = (iBackground.height - arrowHeight * 2) * (textField.bottomScrollV - textField.scrollV) / (textField.bottomScrollV - textField.scrollV + max);
				}
				else
				{	
					tmp = (iBackground.height - arrowHeight * 2) * (iBackground.height - (paddings.top + paddings.bottom)) / view.height;
				}
				if(tmp < arrowHeight * 2)
				{
					tmp = arrowHeight * 2;
				}
				if(tmp > iBackground.height - arrowHeight * 2)
				{
					thumb.visible = false;
					grip.visible = false;
				}
				else
				{
					thumb.height = tmp;
					thumbRect.bottom = Math.ceil(iBackground.height - arrowHeight - thumb.height);
					grip.visible = true;
                    thumb.visible = true;
				}
				updateThumbPos();
			}
			else 
			{
				if(this.policy == SCROLLBAR_AS_NEEDED)
				{
					scrollBar.visible = false;
				}
				programatic = true;
				if(textField != null)
				{
					textField.scrollV = 0;
				}
				else
				{
					view.y = 0;
				}
				programatic = false;
                thumb.visible = false;
                grip.visible = false;
			}
		}
		
		//______________________________________________________________________________________
		//
		public function scrollToView(view:DisplayObject):void
		{
			if (view.y < pos)
            {
                scrollPos = view.y;
            }
            else if (view.y + view.height > pos + viewMask.height)
            {
                scrollPos = view.y + view.height - viewMask.height;
            }
		}
		
		public function setPaddings(obj:Object):void
		{
			if(obj)
			{
				this.paddings.left = obj.left;
				this.paddings.right = obj.right;
				this.paddings.top = obj.top;
				this.paddings.bottom = obj.bottom;
			}
			this.width = paddings.right;
			this.height = paddings.bottom;
			viewContainer.x = paddings.left;
            viewContainer.y = paddings.top;
		}
		
		//卷动条置顶
		public function scrollTop() : void
        {
            pos = 0;
            updateUI();
        }
		
		//卷动条置底
		public function scrollBottom() : void
        {
            pos = max;
//            trace("scrollBottom = ", pos);
//            trace("max = ", max);
            updateUI();
        }
		
		//刷新
		public function refresh() : void
        {
            if (textField != null)
            {
				var ms:Number = textField.maxScrollV;
                max = ms--;
            }
            else
            {
                max = view.height - (iBackground.height - (paddings.top + paddings.bottom));
            }
            if (max < 0)
            {
                max = 0;
            }
            if (pos > max)
            {
                pos = max;
            }
            updateUI();
        }
		
		//设置卷动条的位置，依情况定
		public function setScrollPos():void
		{			
			scrollBar.x = 0;
			viewContainer.x = scrollBar.width;
		}
	
		public function get Content():DisplayObject
		{
			return view;
		}
		
		/** 滑动条是否已经滑到最下 */
		public function isBottom():Boolean
		{
			var ret:Boolean = false;
			if(pos >= max - step/2) {
				ret = true;
			}
			return ret;
		}
		
		//______________________________________________________________________________________
		//update
		//更新卷动条位置
		private function updateScrollPos():void
		{
			pos = (thumb.y - arrowHeight)/((iBackground.height - thumb.height - 2 * arrowHeight) / max );
			if(pos <= this.step/2)
			{
				pos = 0;
			}
			if(pos >= max - step/2)
			{
				pos = max;
			}
			programatic = true;
			 if (textField != null)
            {
                textField.scrollV = pos + 1;
            }
            else
            {
                view.y = -pos;
            }
            programatic = false;
		}
		
		//更新滑块的位置
		private function updateThumbPos():void
		{
			thumb.y = arrowHeight + pos*(this.barBackGround.height - thumb.height - 2*this.arrowHeight) / max;
			grip.y = int(thumb.y + (thumb.height - grip.height)/2);
			programatic = true;
			if(textField != null)
			{
				textField.scrollV = pos + 1;
			}
			else
			{
				view.y = -pos;
			}
			programatic = false;
		}
		
		//__________________________________________________________________
		//GETTER&SETTER
		public override function set width(v:Number):void
		{
			super.width = v;
			scrollBar.x = iBackground.width - scrollBar.width;
			if(scrollBar.visible)
			{
				if(textField != null)
				{
					textField.width = iBackground.width - scrollBar.width - (paddings.left + paddings.right);
				}
				else
				{
					view.width = iBackground.width - scrollBar.width - (paddings.left + paddings.right);
				}
			}
			else if(textField != null)
			{
				textField.width = iBackground.width - (paddings.left + paddings.right);
			}
			else 
			{
				view.width = iBackground.width - (paddings.left + paddings.right);
			}
//			viewContainer.width = v;
		}
		
		public override function set height(v:Number):void
		{
			super.height = v;
			this.barBackGround.height = this.iBackground.height;
			if(v < this.arrowHeight*2)
			{
				//upArrow.height = v / 2;
                //downArrow.height = v / 2;
			}
			else
			{
				//upArrow.height = arrowHeight--;
				//downArrow.height = arrowHeight--;
			}
			downArrow.y = this.barBackGround.height - this.downArrow.height;
			if(this.textField != null)
			{
				this.textField.height = this.iBackground.height - (paddings.top + paddings.bottom);
			}
			else 
			{
				this.viewMask.height = this.iBackground.height - (paddings.top + paddings.bottom);
			}
			
//			viewContainer.height = v;
		}
		
		public function set scrollPos(v:Number):void
		{
			pos = v;
			if(pos > max)
			{
				pos = max;
			}
			if(pos < 0)
			{
				pos = 0;
			}
			updateUI();
		}
		
		public function set scrollPolicy(v:int):void
		{
			if(policy == v)
			{
				return;
			}
			policy = v;
			if(v == SCROLLBAR_AS_NEEDED)
			{
				scrollBar.visible = max > 0;
			}
			else if(v == SCROLLBAR_NEVER)
			{
				scrollBar.visible = false;
			}
			else 
			{
				scrollBar.visible = true;
			}
		}
		
		public function set scrollBarOpaque(bool:Boolean):void
		{
			if(bool)
			{
				this.barBackGround.alpha = 1;
			}
			else
			{
				this.barBackGround.alpha = 0;
			}
		}
		
		public function get hasScrollBar():Boolean
		{
			return scrollBar.visible;
		}
		
		public function get scrollMax():Number
		{
			return max;
		}
		
		public function get scrollPos():Number
		{
			return pos;
		}
		
		public function set scrollStep(v:int) : void
        {
            step = v;
        }
		
		public function get scrollStep():int
		{
			return step;
		}
	}
}