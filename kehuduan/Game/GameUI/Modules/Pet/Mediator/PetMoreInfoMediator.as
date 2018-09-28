package GameUI.Modules.Pet.Mediator
{
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetMoreInfoMediator extends Mediator
	{
		public static const NAME:String = "PetMoreInfoMediator";
		
		private var panelBase:PanelBase = null;
		public function PetMoreInfoMediator(viewComponent:Object)
		{
			super(NAME, viewComponent);
		}
		
		public function get mainView():MovieClip
		{
			return viewComponent as MovieClip;	
		}
		
		override public function onRegister():void
		{
			panelBase = new PanelBase(mainView, mainView.width+8, mainView.height+12);
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_med_morePetInfo_1"]);   //"宠物更多信息" 
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				PetEvent.LOOK_FOR_PET_DETAIL_INFO,
				PetEvent.CLOSE_PET_DETAIL_INFO,
				PetEvent.LOOK_FOR_OTHER_PET_DETAIL_INFO
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetEvent.LOOK_FOR_PET_DETAIL_INFO:
					showView();
				break;
				case PetEvent.CLOSE_PET_DETAIL_INFO:
					onPanelClose(null);
				break;
				case PetEvent.LOOK_FOR_OTHER_PET_DETAIL_INFO:
					PetPropConstData.setViewPosition("PetBag",this.panelBase);
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
				break;
			}
		}
		
		private function showView():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.onPanelClose(null);
				return;
			}
			sendNotification(PetEvent.CLOSE_PET_DETAIL_INFO);
			PetPropConstData.setViewPosition("PetBag",this.panelBase);
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		
		private function dealEventListeners(isAddEventLis:Boolean):void
		{
			if(isAddEventLis)
			{
				this.panelBase.addEventListener(Event.CLOSE,onPanelClose);
			}
			else
			{
				this.panelBase.removeEventListener(Event.CLOSE,onPanelClose);
				
			}
		}
		private function onPanelClose(e:Event):void
		{
			dealEventListeners(false);
			if(this.panelBase.parent)
			{
				this.panelBase.parent.removeChild(this.panelBase);
			}
		}
	}
}