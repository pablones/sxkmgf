package OopsEngine.GameComponents
{
	import OopsFramework.Collections.ArrayCollection;
	import OopsFramework.DrawableGameComponent;
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;

	public class UIComponent extends DrawableGameComponent
	{
		private var drawableElements:Array;			// 绘制元件集合
		private var elements:ArrayCollection;		// UI实体元件集合
		
		public function get Elements():ArrayCollection
		{
			return this.elements;
		}
		
		public function UIComponent(game:Game)
		{
			super(game);
			this.name 		  = "UILayer";
			this.mouseEnabled = false;
			this.elements = new ArrayCollection();
		}
		
		public override function Update(gameTime:GameTime):void
		{
			this.drawableElements = this.elements.Concat();
			for (var j:int = 0; j < this.drawableElements.length; j++)
			{
				var updateable:IUpdateable = this.drawableElements[j] as IUpdateable;
				if (updateable.Enabled)
				{
					updateable.Update(gameTime);
				}
			}
			this.drawableElements = [];
		}
	}
}