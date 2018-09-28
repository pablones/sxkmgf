package OopsEngine
{
	import OopsEngine.GameComponents.UIComponent;
	import OopsEngine.Scene.GameSceneComponent;
	
	import OopsFramework.Game;
	
	import flash.display.Sprite;
	import flash.display.Stage;

	public class Engine extends Game
	{
		/** 引擎版本号（管理地图数据资源下载） */
		public static var UILibrary:String;
			
		private var gameUI:UIComponent;
		private var toolTipLayer:Sprite;
		private var worldMap:Sprite;
		private var systemCursor:Sprite;
		private var gameScene:GameSceneComponent;
		private var weather:Sprite;
		
		public function Engine(stage:Stage = null)
		{
			super(stage);
					
			// 初始化场景管理组件
			this.gameScene = new GameSceneComponent(this);			
			this.Components.Add(this.gameScene);
			
			this.weather = new Sprite();
			this.weather.name		      = "Weather";
			this.weather.tabEnabled       = false;
			this.weather.tabChildren      = false;
			this.weather.mouseEnabled     = false;
			this.weather.mouseChildren    = false;
			this.addChild(weather);
			
			this.gameUI = new UIComponent(this);
			this.Components.Add(this.gameUI);
			
			// 世界地图层
			this.worldMap 			       = new Sprite();
			this.worldMap.name		       = "WorldMap";
			this.worldMap.tabEnabled       = false;
			this.worldMap.tabChildren      = false;
			this.worldMap.mouseEnabled     = true;
			this.addChildAt(this.worldMap,3);
			
			// 初始化浮动窗口层
			this.toolTipLayer 			   = new Sprite();
			this.toolTipLayer.name		   = "ToolTipLayer";
			this.toolTipLayer.mouseEnabled = false;
			this.addChildAt(this.toolTipLayer,4);
			

			// 初始化鼠标图标层
			this.systemCursor				= new Sprite();
			this.systemCursor.mouseEnabled  = false;
			this.systemCursor.mouseChildren = false;
			this.addChildAt(this.systemCursor,5);
		}
		
		/** 获取游戏场景组件 */
		public function get GameScene():GameSceneComponent
		{
			return this.gameScene;
		}
		
		/** 获取游戏UI组件 */
		public function get GameUI():UIComponent
		{
			return this.gameUI;
		}
		
		/** 获取游戏UI组件 */
		public function get TooltipLayer():Sprite
		{
			return this.toolTipLayer;
		}
		
		/** 世界地图层 */
		public function get WorldMap():Sprite
		{
			return this.worldMap;
		}
		
		/** 鼠标层 */
		public function get CursorLayer():Sprite
		{
			return this.toolTipLayer;
		}
		
		/**天气**/
		public function get Weather():Sprite
		{
			return this.weather;
		}
	}
}