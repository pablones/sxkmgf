package GameUI.Modules.DragonEgg.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.DragonEgg.Data.DragonEggData;
	import GameUI.Modules.DragonEgg.Data.DragonEggVo;
	import GameUI.View.BaseUI.AutoPanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LookDragonEggInfoMediator extends Mediator
	{
		public static const NAME:String = "LookDragonEggInfoMediator";
		
		private var main_mc:MovieClip;
		private var panelBase:AutoPanelBase;
		private var dragonVo:DragonEggVo;
		
		
		public function LookDragonEggInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							DragonEggData.SHOW_LOOK_DRAGONEGG_INFO,
							DragonEggData.CLOSE_LOOK_DRAGONEGG_INFO
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case DragonEggData.SHOW_LOOK_DRAGONEGG_INFO:
					dragonVo = notification.getBody() as DragonEggVo;
					showMe();
				break;
				case DragonEggData.CLOSE_LOOK_DRAGONEGG_INFO:
					closeMe(null);
				break;
			}
		}
		
		private function showMe():void
		{
			if ( !main_mc )
			{
				main_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip( "LookDragonEggQua" );
				
				panelBase = new AutoPanelBase( main_mc,main_mc.width+8,main_mc.height+12 );
				panelBase.x = UIConstData.DefaultPos2.x;
				panelBase.y = UIConstData.DefaultPos2.y;
				panelBase.SetTitleTxt( "查看资质" );
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				panelBase.addEventListener( Event.CLOSE,closeMe );
				
				main_mc.close_btn.addEventListener( MouseEvent.CLICK,closeMe );
			}
			initUI();
		}
		
		private function initUI():void
		{
			for ( var i:uint=0; i<5; i++ )
			{
				main_mc[ "txt_"+i ].mouseEnabled = false;
			}
			main_mc[ "txt_0" ].text = dragonVo.streng + "/" + dragonVo.maxStreng;
			main_mc[ "txt_1" ].text = dragonVo.force + "/" + dragonVo.maxForce;
			main_mc[ "txt_2" ].text = dragonVo.physical + "/" + dragonVo.maxPhysical;
			main_mc[ "txt_3" ].text = dragonVo.concentration + "/" + dragonVo.maxConcentration;
			main_mc[ "txt_4" ].text = dragonVo.waza + "/" + dragonVo.maxWaza;
			
			main_mc[ "bar_0" ].width = dragonVo.streng / dragonVo.maxStreng * 150;
			main_mc[ "bar_1" ].width = dragonVo.force / dragonVo.maxForce * 150;
			main_mc[ "bar_2" ].width = dragonVo.physical / dragonVo.maxPhysical * 150;
			main_mc[ "bar_3" ].width = dragonVo.concentration / dragonVo.maxConcentration * 150;
			main_mc[ "bar_4" ].width = dragonVo.waza / dragonVo.maxWaza * 150;
		}
		
		private function closeMe( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeMe );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				main_mc.close_btn.removeEventListener( MouseEvent.CLICK,closeMe );
				main_mc = null;
				panelBase = null;
			}
		}
	}
}