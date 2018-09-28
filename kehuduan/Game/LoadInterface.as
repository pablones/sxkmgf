package
{
	import GameUI.UICore.UIFacade;
	
	import flash.display.Sprite;
	import flash.events.ContextMenuEvent;
	import flash.system.Security;

	public class LoadInterface extends Sprite
	{
		public function Run(sprite:Sprite, info:Object):void
		{
			Security.allowDomain("*");
			
			var GameMain:Game = new Game(sprite.stage, info);
			sprite.addChild(GameMain);
		}
		
		/** 江湖指南回调 */
		public function HelpFun(e:ContextMenuEvent):void
		{
			UIFacade.GetInstance(UIFacade.FACADEKEY).openHelpView();
		}
	}
}