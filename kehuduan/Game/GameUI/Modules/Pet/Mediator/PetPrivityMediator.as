package GameUI.Modules.Pet.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.items.MoneyItem;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.Bitmap;
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetPrivityMediator extends Mediator
	{
		public static var NAME:String = "PetPrivityMediator ";
		
		private var panelBase:PanelBase;
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var moneyAll:int;	//需要的金钱总数
		private var numBitmap:Bitmap;	//数字图片
		private var iconName:int;
		private var spendMoney:int;
		public static var isAddSend:Boolean;
		private var timerDistance:Timer;
		public function PetPrivityMediator( viewComponent:DisplayObject)
		{
			super(NAME,viewComponent);
		}
		
		private function get mainView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		override public function onRegister():void
		{
			var fastPurchase:FastPurchase = new FastPurchase("630035"); 	//心神化一珠
			fastPurchase.y = 0;
			fastPurchase.x = 240;
			mainView.addChild(fastPurchase);
			panelBase = new PanelBase( mainView,320,mainView.height+12 );
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_med_petPri_onR_1"]);   //"提升默契" 
			initView();
		}
		override public function listNotificationInterests():Array
		{
			return [
				PetEvent.SHOW_PET_PRIVITY_VIEW,
				EventList.UPDATEMONEY,
				PetEvent.CLOSE_PET_DETAIL_INFO,
				PetEvent.PET_PRIVITY_FEEDBACK,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetEvent.SHOW_PET_PRIVITY_VIEW:
					showView();
				break;
				case EventList.UPDATEMONEY:
					upDataMoney();
				break;
				case PetEvent.CLOSE_PET_DETAIL_INFO:
					if(isAddSend) return;
					onPanelClose(null);
				break;
				case PetEvent.PET_PRIVITY_FEEDBACK:
					if(isAddSend)
					{
						initTxt();
						if(!timerDistance.running)
						{
							timerDistance.start();
						}
						else
						{
							timerDistance.reset();
						}
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
		
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false;
			(this.mainView.txt_percent as TextField).autoSize = TextFieldAutoSize.CENTER;
			(this.mainView.txt_field as TextField).mouseEnabled = false;
			(this.mainView.txt_haveTool as TextField).mouseEnabled = false;
			(this.mainView.txt_needTool as TextField).mouseEnabled = false;
		
			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 60;
			bindMoneyItem.y = 298;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 60;
			unBindMoneyItem.y = 319;
			needMoney = new MoneyItem();
			needMoney.x = 60;
			needMoney.y = 276;
			mainView.addChild(bindMoneyItem);
			mainView.addChild(unBindMoneyItem);
			mainView.addChild(needMoney);
			upDataMoney();
			upDateNeedMoney( 0 );
		}
		
		private function upDataMoney():void
		{
			this.bindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]) );
		}
		
		private function showView():void
		{
			if(PetPropConstData.newCurrentPet.State == 1)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPri_showV_1"], color:0xffff00});	//出战状态的宠物不能提升默契
				return;
			}
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.onPanelClose(null);
				return;
			}
			sendNotification(PetEvent.CLOSE_PET_DETAIL_INFO);
			PetPropConstData.setViewPosition("PetBag",this.panelBase);
			initTxt();
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			
		}
		
		private function initTxt():void
		{
			var pet:GamePetRole = PetPropConstData.newCurrentPet;
			var explain:Object = getDescribteTxt(pet);
			mainView.txt_percent.text = explain.percentTxt;
			mainView.txt_field.text = explain.describeTxt;
			upDateNeedMoney(explain.moneyTxt);
			mainView.txt_needTool.text = "1个";
			setTxtExplain();
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/NumberIcon/" + iconName + ".png",loadComplete);
		}
		private function setTxtExplain():void
		{
			mainView.txt_haveTool.text = BagData.hasItemNum(630035)+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];//"个";
		}
		private function loadComplete():void
		{
			disposeBitmap();
			numBitmap = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/NumberIcon/" + iconName + ".png");
			mainView.addChild(numBitmap);
			numBitmap.x = 86;
			numBitmap.y = 124;
		}
		private function disposeBitmap():void
		{
			if(numBitmap)
			{
				mainView.removeChild(numBitmap);
				numBitmap.bitmapData.dispose();
				numBitmap = null;
			}
		}
		private function getDescribteTxt(pet:GamePetRole):Object
		{
			var percent:String = "";
			var describe:String = "";
			var needMoney:int;
			if(pet.privity < 1)
			{
				iconName = 0;
				percent = "90%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_1"];//"提升失败不降";
				needMoney = 20000; 
			}
			else if(pet.privity < 2)
			{
				iconName = 1;
				percent = "75%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_1"];//"提升失败不降";
				needMoney = 20000;
			}
			else if(pet.privity < 3)
			{
				iconName = 2;
				percent = "60%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_2"]+1;//"提升失败降为1";
				needMoney = 40000;
			}
			else if(pet.privity < 4)
			{
				iconName = 3; 
				percent = "45%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_2"]+1;//"提升失败降为1";
				needMoney = 40000;
			}
			else if(pet.privity < 5)
			{
				iconName = 4; 
				percent = "30%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_1"];//"提升失败不降";
				needMoney = 60000;
			}
			else if(pet.privity < 6)
			{
				iconName = 5;
				percent = "30%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_2"]+4;//"提升失败降为4";
				needMoney = 60000;
			}
			else if(pet.privity < 7)
			{
				iconName = 6;
				percent = "30%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_2"]+4;//"提升失败降为4";
				needMoney = 80000;
			}
			else if(pet.privity < 8)
			{
				iconName = 7;
				percent = "15%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_1"];//"提升失败不降";
				needMoney = 80000;
			}
			else if(pet.privity < 9)
			{
				iconName = 8;
				percent = "10%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_2"]+7;//"提升失败降为7";
				needMoney = 100000;
			}
			else if(pet.privity < 10)
			{
				iconName = 9;
				percent = "20%";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_1"];//"提升失败不降";
				needMoney = 100000;
			}
			else if(pet.privity == 10)
			{
				iconName = 10;
				percent = "0";
				describe = GameCommonData.wordDic[ "mod_pet_med_petPri_getDes_3"];//"默契已满";
				needMoney = 0;
			}
			spendMoney = needMoney;
			return {percentTxt:percent,describeTxt:describe,moneyTxt:needMoney};
		}
		
		private function dealEventListeners(isAddEventLis:Boolean):void
		{
			if(isAddEventLis)
			{
				this.panelBase.addEventListener(Event.CLOSE,onPanelClose);
				mainView.btnSure.addEventListener(MouseEvent.CLICK,onMouseClick);
				mainView.btnCancel.addEventListener(MouseEvent.CLICK,onMouseClick);
				timerDistance = new Timer(1500,1);
				timerDistance.addEventListener(TimerEvent.TIMER,onTimer);
				timerDistance.start();
			}
			else
			{
				this.panelBase.removeEventListener(Event.CLOSE,onPanelClose);
				mainView.btnSure.removeEventListener(MouseEvent.CLICK,onMouseClick);
				mainView.btnCancel.addEventListener(MouseEvent.CLICK,onMouseClick);
				if(timerDistance.running)
				{
					timerDistance.stop();
				}
				timerDistance.removeEventListener(TimerEvent.TIMER,onTimer);
				timerDistance = null;
			}
		}
		
		
		private function onTimer(me:TimerEvent):void
		{
			isAddSend = false;
		}
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "btnSure":
					sendInfo();
				break;
				case "btnCancel":
					onPanelClose(null);
				break;
			}
			
		}
		private function sendInfo():void
		{
			var pet:GamePetRole = PetPropConstData.newCurrentPet; 
			if(pet.privity == 10)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPri_sendInfo_1"], color:0xffff00});//"该宠物默契已达到上限，无法继续提升"
				return;
			}
			if(pet.winning <= PetPropConstData.selectedPet.privity)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPri_sendInfo_2"], color:0xffff00});//"默契度不能高于灵性值，无法继续提升"
				return;
			}
			if(!BagData.hasItemNum( 630035))	//心神化一珠
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPri_sendInfo_3"], color:0xffff00});//"您的心神合一珠不足,请补充"
				return;
			}  
			if(spendMoney > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});   // 没有足够的银两
				return;
			}
			isAddSend = true;
			PetNetAction.newPetOperate(PlayerAction.NEWPET_PRIVITY_TAG, PetPropConstData.selectedPet.Id, 0,0);
		}
		private function onPanelClose(e:Event):void
		{ 
			if(this.panelBase.parent)
			{ 
				disposeBitmap();
				dealEventListeners(false);
				isAddSend = false;
				this.panelBase.parent.removeChild(this.panelBase);
			}
		}
	}
}