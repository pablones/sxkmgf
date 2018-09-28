package GameUI.View
{
	import GameUI.UIConfigData;
	
	import OopsFramework.DrawableGameComponent;
	import OopsFramework.GameTime;
	
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LibsGameComponent extends DrawableGameComponent
	{
		public function LibsGameComponent(game:Game)
		{
			super(game);
		}
		
		public override function Initialize():void
		{
			
		}
		
		protected override function LoadContent():void
		{
			
		}
		
		public override function Update(gameTime:GameTime):void
		{
			
		}
		
		//_________________________________________________________
		/** 
		 * 取得资源文件  
		 * type:文件类型
		 * mediator:UI所在的mediator
		 * name:资源名称
		 * */
		public function GetLibrary(type:String, mediator:Mediator, name:String):void
		{
			switch(type)
			{
				case UIConfigData.MOVIECLIP:
					mediator.setViewComponent(this.Games.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(name));
				break;
				case UIConfigData.BUTTON:
				break;
				case UIConfigData.BMD:
				break
			}
		}		
	}
}