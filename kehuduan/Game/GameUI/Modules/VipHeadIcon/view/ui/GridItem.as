package GameUI.Modules.VipHeadIcon.view.ui
{
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class GridItem extends Sprite
	{
		private var _data:Object;
		private var _contextMc:MovieClip;
		private var _bitMap:Bitmap;
		private var _iconName:String;
		
		public function GridItem()
		{
		}
		
		public function loadSource(iconNameNum:int):void
		{
			_iconName = iconNameNum.toString();
			try
			{
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Face/" + _iconName + ".png",onLoabdComplete);	
			}
			catch(e:Error)
			{
			}
		}
		
		private function onLoabdComplete():void
		{
			_bitMap = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Face/" + _iconName + ".png");
			if(_bitMap==null)
			{
	 			_bitMap = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
 			}
			_bitMap.x = 4;
			_bitMap.y = 4;
			this.addChild(_bitMap);
		}
		public function set contextMc(contextMc:MovieClip):void
		{
			_contextMc = contextMc;
			this.addChildAt(_contextMc,0);
		}
		public function get contextMc():MovieClip
		{
			return _contextMc;
		}
		public function set bitMap(bitMap:Bitmap):void
		{
			_bitMap = bitMap;
			if(_bitMap)
			{
				this.addChild(_bitMap);
			}
		}
		public function get bitMap():Bitmap
		{
			return _bitMap;
		}
		public function setData(data:Object):void
		{
			_data = data;
		}
		public function getData():Object
		{
			return _data;
		}
		public function gc():void
		{
			_data = null;
			_contextMc = null;
			_bitMap = null;
			_iconName = null;
		}
	}
}
