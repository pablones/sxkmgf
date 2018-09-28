package GameUI.View.items
{
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
//	import flash.display.DisplayObjectContainer;
//	import flash.display.Shape;
	
	import flash.display.DisplayObjectContainer;
	import flash.filters.GlowFilter;
	
	/**
	 * 带颜色框的装备格子
	 * @author:Ginoo
	 * @version:1.0
	 * @langVersion:3.0
	 * @playerVersion:10.0
	 * @since:11/18/2010
	 */
//-------------------------------颜色框-----------------------------------------------------------------------
//	public class EquipItem extends UseItem
//	{   
//		/** 颜色下标 */
//		private var _color:uint = 0;
//		/** 颜色框 */
//		private var _colorShape:Shape = null;
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//constructor
//		
//		public function EquipItem(pos:int, icon:String, parent:DisplayObjectContainer, color:uint)
//		{
//			super(pos, icon, parent);
//			_color = IntroConst.ITEM_COLORS_HEX[color];
//			if(color > 1) {
//				initShape();
//			}
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//public function 
//		
//		/** 设置框颜色 */
//		public function setColor(color:uint):void
//		{
//			_color = IntroConst.ITEM_COLORS_HEX[color];
//			if(color < 2) {
//				if(_colorShape) {
//					this.removeChild(_colorShape);
//					_colorShape = null;
//				}
//			} else {
//				if(!_colorShape) {
//					_colorShape = new Shape();
//					this.addChild(_colorShape);
//				}
//				_colorShape.graphics.clear();
//				draw();
//			}
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//private function 
//		
//		/** 初始化颜色框 */
//		private function initShape():void
//		{
//			_colorShape = new Shape();
//			draw();
//			this.addChild(_colorShape);
//		}
//		
//		
//		/** 绘制色框 */
//		private function draw():void
//		{
//			_colorShape.graphics.lineStyle(1.5, _color, 0.8, true);
////			_colorShape.graphics.lineGradientStyle(GradientType.LINEAR, [_color, 0xffffff], [100, 100], [0, 255]);  
//			_colorShape.graphics.beginFill(_color , 0);
////			 _colorShape.graphics.beginGradientFill(GradientType.LINEAR, [_color, 0xffffff], [100, 100], [0, 255]);
////			  _colorShape.graphics.lineTo(-1,35);
////			  _colorShape.graphics.lineTo(35,35);
////			  _colorShape.graphics.lineTo(35,-1);
////			  _colorShape.graphics.lineTo(-1,-1);
//			_colorShape.graphics.drawRoundRect(-1, -1, 34, 34, 5, 5);
//			_colorShape.graphics.endFill();
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//getter
//		
//		public function get color():uint
//		{
//			return _color;
//		}
//	}

//---------------------Item内发光-----------------------------------------------------------------------------------------

	public class EquipItem extends UseItem
	{   
		/** 颜色下标 */
		private var _color:uint = 0;
		/** 发光滤镜 */
		private var _colorFilter:GlowFilter = null;
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//constructor
		
		public function EquipItem(pos:int, icon:String, parent:DisplayObjectContainer, color:uint)
		{
			super(pos, icon, parent);
			_color = IntroConst.ITEM_COLORS_HEX[color];
			if(color > 1) {
				initColor();
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//public function 
		
		/** 设置滤镜颜色 */
		public function setColor(color:uint):void
		{
			_color = IntroConst.ITEM_COLORS_HEX[color];
			if(!_colorFilter) {
				_colorFilter = new GlowFilter(_color, 1, 4, 4, 2.5, 3, true);
			} else {
				_colorFilter.color = _color;
			}
			if(color < 2) {
				this.filters = null;
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//private function 
		
		/** 初始化颜色滤镜 */
		private function initColor():void
		{
			_colorFilter = new GlowFilter(_color, 1, 4, 4, 2.5, 3, true);
			this.filters = [_colorFilter];
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//getter
		
		public function get color():uint
		{
			return _color;
		}
	}

//-------------------图片内发光------------------------------------------------------------------------

//	public class EquipItem extends UseItem
//	{   
//		/** 颜色下标 */
//		private var _color:uint = 0;
//		/** 发光滤镜 */
//		private var _colorFilter:GlowFilter = null;
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//constructor
//		
//		public function EquipItem(pos:int, icon:String, parent:DisplayObjectContainer, color:uint)
//		{
//			super(pos, icon, parent);
//			_color = IntroConst.ITEM_COLORS_HEX[color];
//			if(color > 1) {
//				initColor();
//			}
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//public function 
//		
//		/** 设置滤镜颜色 */
//		public function setColor(color:uint):void
//		{
//			_color = IntroConst.ITEM_COLORS_HEX[color];
//			if(!_colorFilter) {
//				_colorFilter = new GlowFilter(_color, 1, 7, 7, 2.5, 1, true);
//			} else {
//				_colorFilter.color = _color;
//			}
//			if(color < 2) {
//				this.icon.filters = null;
//			}
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//private function 
//		
//		/** 初始化颜色滤镜 */
//		private function initColor():void
//		{
//			_colorFilter = new GlowFilter(_color, 1, 7, 7, 2.5, 1, true);
//			this.icon.filters = [_colorFilter];
//		}
//		
//		/////////////////////////////////////////////////////////////////////////////////////////////////
//		//getter
//		
//		public function get color():uint
//		{
//			return _color;
//		}
//	}
}