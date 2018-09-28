package OopsEngine.Scene
{
	import OopsEngine.Entity.EntityElement;
	
	import OopsFramework.Game;

	/** 游戏场景中的元件抽象类。所有游戏场景中的元件都必须继承此类 */
	public class GameElement extends EntityElement
	{
		protected var excursionX:int = 0;		// 人物偏移X坐标
		protected var excursionY:int = 0;		// 人物偏移Y坐标
		
		public var Prams:String;				// 元件参数
		
		public function get Depth():Number
		{
			return this.Y + this.excursionY;
		}
		
		/** 游戏中人物偏移X坐标 */
		public function get ExcursionX():Number
		{
			return this.excursionX;
		}
		
		/** 游戏中人物偏移Y坐标 */
		public function get ExcursionY():Number
		{
			return this.excursionY;
		}
		
		public function GameElement(game:Game)
		{
			super(game);
		}
	}
}