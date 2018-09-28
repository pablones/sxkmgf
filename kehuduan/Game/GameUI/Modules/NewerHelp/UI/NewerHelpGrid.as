package GameUI.Modules.NewerHelp.UI
{
	import flash.display.Shape;
	import flash.display.Sprite;
	
	/**
	 * 绿色格子
	 */ 
	public class NewerHelpGrid extends Sprite
	{
		private var _grid:Shape  = null;
		private var _width:uint  = 0;
		private var _height:uint = 0;
		private var _color:uint  = 0;
		private var _type:uint   = 0;
		private var _lineThick:uint = 0;
		
		/**
		 * @param width
		 * @param height
		 * @param color
		 * @param type: 0-圆角 1-方角
		 */
		public function NewerHelpGrid(width:uint=50, height:uint=50, color:uint=0x00ff00, type:uint=0, lineThick:uint=3)
		{
			_width  = width;
			_height = height;
			_color  = color;
			_type	= type;
			_lineThick = lineThick;
			init();
		}
		
		/** 初始化绿格 */
		private function init():void
		{
			_grid = new Shape();
			_grid.graphics.lineStyle(_lineThick, _color, 1);
			_grid.graphics.beginFill(_color , 0);
			_type == 0 ? _grid.graphics.drawRoundRect(0, 0, _width, _height, 5, 5) : _grid.graphics.drawRect(0, 0, _width, _height);
			_grid.graphics.endFill();
			this.addChild(_grid);
			this.mouseEnabled = false;
		}
		
	}
}