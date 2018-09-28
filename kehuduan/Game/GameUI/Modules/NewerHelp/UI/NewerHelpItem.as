package GameUI.Modules.NewerHelp.UI
{
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.utils.Timer;
	
	/**
	 * 新手指导UI
	 * @author:Ginoo
	 * @date:8/31/2010
	 */
	public class NewerHelpItem extends Sprite
	{
//		/** 任务编号 */
//		private var _id:uint 		 = 0;
//		
//		/** 指示方向 */
//		private var _direction:uint  = 0;
		
		/** 显示文字 */
		private var _text:String 	 = "";
		
		/** 文字颜色 */
		private var _txtColor:String = "";
		
		/** UI */
		private var _view:MovieClip  = null;
		
		/** height */
		private var _height:uint = 0;
		
		private var _width:uint = 0;

		/** tf */
		private var _tf:TextField = null;
		
		private var _scaleX:Number = 0;
		
		private var _scaleY:Number = 0;
		
		private var _useBtn:Boolean = false;
		
		private var _tfX:Number = 0;
		private var _tfY:Number = 0;
		
		private var _btn:SimpleButton = null;
		
		private var _callBack:Function = null;
		
		private var _curTask:uint = 0;
		private var _curStep:uint = 0;
		
		private var _timer:Timer = null
		private var _glowFilter:GlowFilter = null;
		
		/** 编号 */
		private var _logo:String = "";
		
		/**
		 * @param id: 编号
		 * @param direction:箭头指示方向
		 * @param text: 显示文字
		 * @param txtColor:文字颜色
		 */
		public function NewerHelpItem(text:String, scaleX:Number=0, scaleY:Number=0, useBtn:Boolean=false, callBack:Function=null, curTask:uint=0, curStep:uint=0, height:uint=0, tfWidth:uint=124, tfX:Number=6, tfY:Number=6.7, txtColor:String="#00ff00")
		{
//			this._id = id ;
//			this._direction = direction;
			this._text = text;
			this._useBtn = useBtn;
			this._callBack = callBack;
			this._curTask = curTask;
			this._curStep = curStep;
			this._scaleX = scaleX;
			this._scaleY = scaleY;
			this._height = height;
			this._width = tfWidth;
			this._tfX = tfX;
			this._tfY = tfY;
			this._txtColor = txtColor;
			this._glowFilter = new GlowFilter(0x000000,1,0,0);
			init();
		}
		
		/** 初始化 */
		private function init():void
		{
			if(_scaleY != 0) {
				_view = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NewerHelpView_2");
			} else {
				_view = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NewerHelpView");
			}
			_view.mouseEnabled = false;
			_view.filters = [];
			
			if(_height > 0) {
				_view.height = _height;
			}

			if(_scaleX != 0) {
				_view.scaleX = _scaleX;
			}
			
			_tf = new TextField();
			_tf.defaultTextFormat = new TextFormat("宋体", 12, 0x00FF00, null,null,null,null,null,"center",null,null,null,1);//"宋体"
			_tf 
			_tf.text = _text;
//			_tf.htmlText = "<font color='"+_txtColor+"'>" + _text + "</font>";
			_tf.mouseEnabled = false;
			_tf.width = _width;
			_tf.x = _tfX;
			_tf.y = _tfY;
			
			if(_useBtn) {
				_btn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("BtnIknow");
				_btn.x = ((_view.width - _btn.width) >> 1) - 3; 
				_btn.y = _view.height - _btn.height - 12;
				_btn.addEventListener(MouseEvent.CLICK, clickHandler);
			}
			
			if(_scaleX == -1) {
				_tf.x = -_view.width + _tf.x + 16;
				if(_btn) {
					_btn.x = -_view.width + _btn.x + 11;
				}
			}
				
			if(_scaleY == -1) {
				_tf.y += 10;
				if(_btn) {
					_btn.y += 10;
				}
			}
			
			addChild(_view);
			addChild(_tf);
			if(_btn) addChild(_btn);
			
			mouseEnabled = false;
			
			_timer = new Timer(500);
			_timer.addEventListener(TimerEvent.TIMER, timerHandler);	
			_timer.start();
		}
		
		private function timerHandler(e:TimerEvent):void
		{
			if(_view.filters.length == 0) {
				_view.filters = [_glowFilter];
			} else {
				_view.filters = [];
			}
		}
		
		private function clickHandler(e:MouseEvent):void
		{
			if(_logo) {
				if(NewerHelpData.iKnowItemDic[_logo]) {
					delete NewerHelpData.iKnowItemDic[_logo];
				}
			}
			dispose();
		}
		
		public function dispose():void
		{
			_btn.removeEventListener(MouseEvent.CLICK, clickHandler);
			if(_callBack != null) {
				_callBack(_curTask, _curStep);
			}
			_timer.stop();
			if(GameCommonData.GameInstance.TooltipLayer.contains(this)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(this);
			}
		}
		
		private function getPos(dis:DisplayObject):Point
		{
			return null;
		}
		
		public function set logo(logo:String):void
		{
			this._logo = logo;
		}
		
	}
}