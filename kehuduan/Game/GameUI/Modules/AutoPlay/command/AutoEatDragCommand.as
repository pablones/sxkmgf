package GameUI.Modules.AutoPlay.command
{
	import Controller.CooldownController;
	
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.mediator.AutoPlayMediator;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.events.TimerEvent;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//挂机吃药
	public class AutoEatDragCommand extends SimpleCommand
	{
		public static const NAME:String = "AutoEatDragCommand";
		private var autoPlayMediator:AutoPlayMediator;
		private var dataDic:Dictionary;
		private var petHappy:int;
		private var type:int = 0;
		
		public function AutoEatDragCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			if ( GameCommonData.Player.IsAutomatism )
			{
				autoPlayMediator = facade.retrieveMediator( AutoPlayMediator.NAME ) as AutoPlayMediator;
				dataDic = autoPlayMediator.dataDic;
			
				if ( notification.getBody() )
				{
					type = notification.getBody().type;
				}
				else 
				{
					type = 0;
				}
			
				switch ( type )
				{
					case 0:
						start();
					break;
					case 1:
						checkHpTick();
					break;
					case 2:
						checkMpTick();
					break;
					case 3:
						checkPetHpTick();
					break;
					case 4:
						checkPetHappyTick();
						if ( notification.getBody().happy )
						{
							petHappy = notification.getBody().happy;							//传进来的快乐值
						}
					break;
				}
			}
			else
			{
				stop();
			}
		}
		
		private function start():void
		{
			if ( !AutoPlayData.autoTimer.hasEventListener( TimerEvent.TIMER ) )
			{
				AutoPlayData.autoTimer.addEventListener( TimerEvent.TIMER,onTimer );
			}
			AutoPlayData.autoTimer.start();
			
			checkHpTick();
			checkMpTick();
			checkPetHpTick();
			checkPetHappyTick();
		}
		
		private function checkHpTick():void
		{
			if ( AutoPlayData.aSaveTick[0] == 1 && GameCommonData.Player.Role.HP>0 )
			{
				eatRed();
			}
		}
		
		private function checkMpTick():void
		{
			if ( AutoPlayData.aSaveTick[1] == 1 && GameCommonData.Player.Role.HP>0 )
			{
				eatBlue();
			}
		}
		
		private function checkPetHpTick():void
		{
			if ( AutoPlayData.aSaveTick[6] == 1 )
			{
				petEatRed();
			}
		}
		
		private function checkPetHappyTick():void
		{
			if ( AutoPlayData.aSaveTick[7] == 1 )
			{
				petEatHappy();
			}
		}
		
		private function onTimer(evt:TimerEvent):void
		{
			//start();
			var haveRedDrag:Boolean = haveDrag( AutoPlayData.aSaveType.slice(0,3) );
			var haveBlueDrag:Boolean = haveDrag( AutoPlayData.aSaveType.slice(3,6) );
			var havePetRedDrag:Boolean = haveDrag( AutoPlayData.aSaveType.slice(6,9) );
			var havePetHappyDrag:Boolean = haveDrag( AutoPlayData.aSaveType.slice(9,11) );

			if ( haveRedDrag ) checkHpTick();
			if ( haveBlueDrag ) checkMpTick();
			if ( havePetRedDrag ) checkPetHpTick();
			if ( havePetHappyDrag ) checkPetHappyTick();
		}
		
		private function stop():void
		{
			AutoPlayData.autoTimer.stop();
			AutoPlayData.autoTimer.removeEventListener( TimerEvent.TIMER,onTimer );
		}
		
		private function eatRed():void
		{
			var limit:uint = AutoPlayData.aSaveNum[0];
			var scale:Number = (GameCommonData.Player.Role.HP/(GameCommonData.Player.Role.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP)*100);
			if ( scale<=limit )
			{
				var arr:Array = AutoPlayData.aSaveType.slice( 0,3 );
				var useItem:UseItem = this.getUseItem( arr );
				if ( useItem )
				{
					eatItem( useItem );
				}
			}
		}
		
		private function eatBlue():void
		{
			var limit:uint = AutoPlayData.aSaveNum[1];
			var scale:Number = (GameCommonData.Player.Role.MP/GameCommonData.Player.Role.MaxMp*100);
			if ( scale<=limit )
			{
				var arr:Array = AutoPlayData.aSaveType.slice( 3,6 );
				var useItem:UseItem = this.getUseItem( arr );
				if ( useItem )
				{
					eatItem( useItem );
				}
			}
		}
		
		private function petEatRed():void
		{
			if ( GameCommonData.Player.Role.UsingPet != null )
			{
				var limit:uint = AutoPlayData.aSaveNum[5];
				var id:uint = GameCommonData.Player.Role.UsingPet.Id;
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id] as GamePetRole;
				var scale:Number = pet.HpNow/pet.HpMax*100;
				if ( scale<=limit )
				{
					var arr:Array = AutoPlayData.aSaveType.slice( 6,9 );
					var useItem:UseItem = this.getUseItem( arr );
					if ( useItem && CooldownController.getInstance().cooldownReady(useItem.Type) ) NetAction.presentRoseToFriend( QuickBarData.getInstance().getItemIdFromBagNoBind( useItem ),id,0 );
				}
			}
		}
		
		private function petEatHappy():void
		{
			if ( GameCommonData.Player.Role.UsingPet != null )
			{
				var limit:uint = AutoPlayData.aSaveNum[6];
				var id:uint = GameCommonData.Player.Role.UsingPet.Id;
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id] as GamePetRole;
				var scale:Number;
				if ( this.petHappy )
				{
					scale = petHappy/pet.HappyMax*100;
				}
				else
				{
					scale = pet.HappyNow/pet.HappyMax*100;
				}
				if ( scale<=limit )
				{
					var arr:Array = AutoPlayData.aSaveType.slice( 9,11 );
					var useItem:UseItem = this.getUseItem( arr );
					if ( useItem && CooldownController.getInstance().cooldownReady(useItem.Type) ) NetAction.presentRoseToFriend( QuickBarData.getInstance().getItemIdFromBagNoBind( useItem ),id,0 );
				}
			}
		}
		
		private function getUseItem(arr:Array):UseItem
		{
			var len:uint=arr.length;
			for(var i:uint=0;i<len;i++)
			{
				if( arr[i]==undefined || arr[i]==0 )continue;
				if(this.dataDic[arr[i]]!=null)
				{
					return  this.dataDic[arr[i]];
				}
			}
			return null;
		}
		
		private function eatItem( item:UseItem ):void
		{
			if ( item  && CooldownController.getInstance().cooldownReady(item.Type) )
			{
				NetAction.UseItem( OperateItem.USE,1,0,QuickBarData.getInstance().getItemIdFromBagNoBind( item ) );
			}
		}
		
		//是否还有药品
		private function haveDrag(arr:Array):Boolean
		{
			for ( var i:uint=0; i<arr.length; i++ )
			{
				if ( arr[i] != 0 )
				{
					return true;
				}
			}
			return false;
		}
		
	}
}