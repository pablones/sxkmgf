package GameUI.Modules.AutoPlay.command
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//自动挂机宠物设置
	public class AutoPlayPetCommand extends SimpleCommand
	{
		public static const NAME:String = "AutoPlayPetCommand";
		private var petId:uint;
		private var timeId:int;
		
		public function AutoPlayPetCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			petId = notification.getBody().id;
			if ( petId<2000000000 || petId>3999999999 ) return;
			if ( GameCommonData.Player.IsAutomatism )
			{
				start();
			}
		}
		
		//宠物死了后自动出战
		private function start():void
		{
			var tick:uint = AutoPlayData.aSaveTick[8];
			clearTimeout( timeId );
			if ( tick == 1 )			//1为打勾
			{
				timeId = setTimeout( petGo,12000 );				//延迟12秒放宠物
			}
		}
		
		private function petGo():void
		{
			clearTimeout( timeId );
			if ( !GameCommonData.Player.IsAutomatism ) return;
			var happy:int = ( GameCommonData.Player.Role.PetSnapList[ petId ] as GamePetRole ).HappyNow;
			if ( happy<60 )
			{
				eatDrag();
			}
			else
			{
				sendNotification( PetEvent.PET_COMEOUT_FIGHT_OUTSIDE_INTERFACE,petId );
			}
		}
		
		private function eatDrag():void
		{
			var drag:Object = BagData.getItemByType( 330000 );
			if ( drag  )
			{
				NetAction.presentRoseToFriend( drag.id,petId,0 );
				sendNotification( PetEvent.PET_COMEOUT_FIGHT_OUTSIDE_INTERFACE,petId );
			}
			else 
			{
				drag = BagData.getItemByType( 330001 );
				if ( drag )
				{
					NetAction.presentRoseToFriend( drag.id,petId,0 );
					sendNotification( PetEvent.PET_COMEOUT_FIGHT_OUTSIDE_INTERFACE,petId );
				}
			}
		}
		
	}
}