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

	public class PetFantasyMediator extends Mediator
	{
		public static var NAME:String = "PetFantasyMediator ";
		
		private var panelBase:PanelBase;
		public static var isFantasySend:Boolean;
		public function PetFantasyMediator( viewComponent:DisplayObject)
		{
			super(NAME,viewComponent);
		}
		
		private function get mainView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		override public function onRegister():void
		{
			var fastPurchase:FastPurchase = new FastPurchase("630033");	 	//幻化九章
			fastPurchase.y = 0;
			fastPurchase.x = 296;
			mainView.addChild(fastPurchase);
			panelBase = new PanelBase( mainView,375,mainView.height+12 );
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_med_petF_onR_1"]);   //"宠物幻化" 
			(mainView.txt_explain as TextField).mouseEnabled = false;
			(mainView.txt_explainExtend as TextField).mouseEnabled = false;
			(mainView.txt_explainExtend as TextField).htmlText = GameCommonData.wordDic[ "mod_pet_med_petF_onR_2"];	//"提示：幻化后的宠物无法再进行繁殖。";
		}
		override public function listNotificationInterests():Array
		{
			return [
				PetEvent.SHOW_PET_FANTASY_VIEW,
				PetEvent.CLOSE_PET_DETAIL_INFO,
				PetEvent.PET_FANTASY_FEEDBACK,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetEvent.SHOW_PET_FANTASY_VIEW:
					showView();
				break;
				case PetEvent.CLOSE_PET_DETAIL_INFO:
					onPanelClose(null);
				break;
				case PetEvent.PET_FANTASY_FEEDBACK:	//幻化成功
					if(isFantasySend)
					{
						isFantasySend = false;
						onPanelClose(null);
					}
				break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if(this.panelBase.parent)
					{
						setExplain();
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
			setExplain();
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		private function setExplain():void
		{
			var num:int = BagData.hasItemNum(630033);
//			var expStr:String =  "<font color = '#E2CCA5'>　　悟性达到<font color = '#00FF00'>5</font>即可幻化，幻化后可以开启宠物的灵性、默契属性，提升资质和附体技能的威力。对宠物进行幻化需要消耗<font color = '#00FF00'>1</font>个<font color = '#00FFFF'>幻化九章</font>，您当前拥有<font color = '#00FF00'>"+num+"</font>个。</font>";;
			var expStr:String =  GameCommonData.wordDic[ "mod_pet_med_petF_setE_1"]+num+"</font>"+GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ask_1" ]+"。</font>";
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
			if(pet.Savvy < 5) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petF_onM_1"], color:0xffff00});	//"您的宠物悟性值不足5，无法幻化"
				return;
			}
			if(pet.State == 1)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petF_onM_2"], color:0xffff00});	//"出战中的宠物不能进行幻化"
				return;
			}
			if(pet.LifeNow <= 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petF_onM_3"], color:0xffff00});	//"寿命为0的宠物不能进行幻化"
				return;
			}
			if(!BagData.hasItemNum( 630033 )) 	//幻化九章
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petF_onM_4"], color:0xffff00});	//"您的幻化九章不足，无法幻化"
				return;
			} 
			if(2000000 > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petF_onM_5"], color:0xffff00});   // "碎银不足200金"
				return;
			}
			sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:new Function(), info:GameCommonData.wordDic[ "mod_pet_med_petF_onM_6"] ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//"提 示" "确 定"  "取 消"  <font color = '#FF0000'>幻化后的宠物无法再进行繁殖，是否确认进行幻化？</font>
		}
		
		private function beSureToCommit():void
		{
			isFantasySend = true;
			PetNetAction.newPetOperate(PlayerAction.NEWPET_FANTASY_TAG, PetPropConstData.selectedPet.Id, 0,0);
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