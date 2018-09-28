package GameUI.Modules.ChangeLine.Command
{
	import GameUI.Modules.ChangeLine.ChangeLineMediator;
	import GameUI.View.Components.countDown.CountDownEvent;
	import GameUI.View.Components.countDown.CountDownText;
	
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//切线成功
	public class ChgLineSucCommand extends SimpleCommand
	{
		public static const NAME:String = "ChgLineSucCommand";
		private var chgLineMediator:ChangeLineMediator;
//		private var countDownText:CountDownText;
		public static var countDownText:CountDownText;
		private var main_mc:MovieClip;
		
		public function ChgLineSucCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			chgLineMediator = facade.retrieveMediator( ChangeLineMediator.NAME ) as ChangeLineMediator;
			main_mc = chgLineMediator.changeLineView;
//			main_mc.visible = false; 
			main_mc.curServer_txt.visible = false;
			main_mc.mouseEnabled = false;
			
			if ( countDownText && GameCommonData.GameInstance.WorldMap.contains( countDownText ) )
			{
				GameCommonData.GameInstance.WorldMap.removeChild( countDownText );
				countDownText.removeEventListener( CountDownEvent.TIME_OVER,timeOverHandler );
				countDownText.dispose();
				countDownText = null;
			}
			
			countDownText = new CountDownText( 10 );
			countDownText.x = main_mc.x+36;
			countDownText.y = main_mc.y+2;
			countDownText.addEventListener( CountDownEvent.TIME_OVER,timeOverHandler );
			GameCommonData.GameInstance.WorldMap.addChild( countDownText );
			countDownText.start();
		}
		
		private function timeOverHandler( evt:CountDownEvent ):void
		{
			countDownText.removeEventListener( CountDownEvent.TIME_OVER,timeOverHandler );
			if ( GameCommonData.GameInstance.WorldMap.contains( countDownText ) )
			{
				GameCommonData.GameInstance.WorldMap.removeChild( countDownText );
				countDownText.dispose();
				countDownText = null;
			}
//			main_mc.visible = true;
			main_mc.curServer_txt.visible = true;
			main_mc.mouseEnabled = true;
		}

	}
}