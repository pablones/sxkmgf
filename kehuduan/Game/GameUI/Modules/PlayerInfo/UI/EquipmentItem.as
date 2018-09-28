package GameUI.Modules.PlayerInfo.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	import flash.display.DisplayObjectContainer;
	import flash.filters.GlowFilter;

	public class EquipmentItem extends ItemBase
	{
		private var icon:Bitmap;
		/** 装备的类型，用于载图标用*/
		public var type:uint;
		/** 装备的真实ID*/
		public var equipId:uint;
		
		//------------------------------------
		/** 颜色下标 */
		private var _color:uint = 0;
		/** 发光滤镜 */
		private var _colorFilter:GlowFilter = null; 
		//------------------------------------ 
		
		public function EquipmentItem(icon:String, parent:DisplayObjectContainer, color:uint)
		{
			this.name="Equip";
			var iconName:String;			/** 储存图片名 */ 
			if(uint(icon) > 100000 && uint(icon)!=800001) {	//410101
				if(UIConstData.getItem(uint(icon)).img != null)  
				{
					iconName = String(UIConstData.getItem(uint(icon)).img);
				}
			}
			super(iconName, parent);
			//------------------------------------------------
			_color = IntroConst.ITEM_COLORS_HEX[color];
			if(color > 1) {
				initColor();
			}
			//------------------------------------------------
		}
		
		
		/**
		 * 图标加载完成 
		 * 
		 */		
		protected override function onLoabdComplete():void{
			this.icon=ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory+"Resources/icon/"+this.iconName+".png");
			if(this.icon==null){
				this.icon = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
			}
			this.addChild(this.icon);
		}
		
		//------------------------------------------------------------------------------
		
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
		
		/** 初始化颜色滤镜 */
		private function initColor():void
		{
			_colorFilter = new GlowFilter(_color, 1, 4, 4, 2.5, 3, true);
			this.filters = [_colorFilter];
		}
		
		public function get color():uint
		{
			return _color;
		}
		
		//------------------------------------------------------------------------------
		
	}
}