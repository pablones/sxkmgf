package GameUI.Modules.Pet.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetChangeSexMediator extends Mediator
	{
		public static const NAME:String = "PetChangeSexMediator";
		private var panelBase:PanelBase;
		public static var isChangeSexSend:Boolean;
		public function PetChangeSexMediator(viewComponent:DisplayObject)
		{ 
			super(NAME, viewComponent);
		}
		
		private function get mainView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		override public function onRegister():void
		{
			var fastPurchase:FastPurchase = new FastPurchase("630037");	 	//转性丹
			fastPurchase.y = 0;
			fastPurchase.x = 296;
			mainView.addChild(fastPurchase);
			panelBase = new PanelBase( mainView,375,mainView.height+12 );
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_med_onR_1"]); //"宠物变性" ); 
			(mainView.txt_explain as TextField).mouseEnabled = false;
			(mainView.txt_explainExtend as TextField).mouseEnabled = false;
			(mainView.txt_explainExtend as TextField).htmlText = GameCommonData.wordDic[ "mod_pet_med_onR_2"];	//"提示：宠物转换性别不限次数。"; 
			mainView.txt_explainExtend.y = mainView.txt_explainExtend.y - 12;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				PetEvent.SHOW_PET_CHANGE_SEX_VIEW,
				PetEvent.CLOSE_PET_DETAIL_INFO,
				PetEvent.PET_CHANGE_SEX_FEEDBACK,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetEvent.SHOW_PET_CHANGE_SEX_VIEW:
					showView();
				break;
				case PetEvent.CLOSE_PET_DETAIL_INFO:
					onPanelClose(null);
				break;
				case PetEvent.PET_CHANGE_SEX_FEEDBACK:
					if(isChangeSexSend)
					{
						isChangeSexSend = false;
						onPanelClose(null);
					}
				break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if(this.panelBase.parent)
					{
						setTxtExplain();
					}
				break;
			}
		}
		 
		private function showView():void
		{
			if(PetPropConstData.newCurrentPet.State == 4)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
					return;
			}
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.onPanelClose(null);
				return;
			}
			sendNotification(PetEvent.CLOSE_PET_DETAIL_INFO);
			PetPropConstData.setViewPosition("PetBag",this.panelBase);
			setTxtExplain(); 
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		private function setTxtExplain():void
		{
			var num:int = BagData.hasItemNum(630037);
//			var expStr:String =  "<font color = '#E2CCA5'>　　转换宠物性别需要消耗<font color = '#00FF00'>1</font>个<font color = '#00FFFF'>雌雄九转石</font>，您当前拥有数量为<font color = '#00FF00'>"+num+"个</font>。</font>";
			var expStr:String =  GameCommonData.wordDic[ "mod_pet_med_setT_1"] + num + "</font>"+GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ask_1" ]+"。</font>";
			expStr = num == 0 ? expStr : expStr + GameCommonData.wordDic[ "mod_pet_med_setT_2"];	//"<font color = '#E2CCA5'>是否确认使用？</font>";
			(mainView.txt_explain as TextField).htmlText = expStr;
			
		}
		
		private function dealEventListeners(isAddEventLis:Boolean):void
		{
			if(isAddEventLis)
			{
				this.panelBase.addEventListener(Event.CLOSE,onPanelClose);
				mainView.btnSure.addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				this.panelBase.removeEventListener(Event.CLOSE,onPanelClose);
				mainView.btnSure.removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		private function onMouseClick(me:MouseEvent):void
		{
			var pet:GamePetRole = PetPropConstData.newCurrentPet; 
			if(pet.State == 1)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_onM_1"], color:0xffff00});	//"出战中的宠物不能进行变性"
				return;
			}
			if(pet.LifeNow <= 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_onM_2"], color:0xffff00});	//"寿命为0的宠物不能进行变性"
				return;
			}
			if(!BagData.hasItemNum(630037)) 	//转性丹
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_onM_3"], color:0xffff00});	//"您的转性丹不足，无法转性"
				return;
			} 
			if(10 > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_onM_4"], color:0xffff00});   // "碎银不足10金"
				return;
			}
			isChangeSexSend = true;
			PetNetAction.newPetOperate(PlayerAction.NEWPET_CHANGE_SEX_DEPENDENCE_TAG, PetPropConstData.selectedPet.Id, 0,0);
			
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