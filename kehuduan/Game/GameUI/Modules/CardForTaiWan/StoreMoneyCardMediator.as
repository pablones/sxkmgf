package GameUI.Modules.CardForTaiWan
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.CardFiles.NewerCardNewMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.CardBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StoreMoneyCardMediator extends Mediator
	{
		public static const NAME:String = "StoreMoneyCardMediator";
		public static const SHOW_STORE_MONEY_CARD:String = "StoreMoneyCardMediator";
		
		private var panelBase:PanelBase;
		public function StoreMoneyCardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME);
		}
		
		private function get mainView():CardBase
		{
			return this.viewComponent as CardBase;
		}
		override public function onRegister():void
		{
			var cardBase:CardBase = new CardBase();
			this.setViewComponent(cardBase);
			cardBase.initCardFun = setCardContent;
		}
		
		private function setCardContent():void
		{
			mainView.isHasTxt1 = false;
			mainView.txt2 = "請輸入卡號:";
			mainView.txt2Length = 32;
			panelBase = new PanelBase(this.mainView,mainView.width + 8,mainView.height + 12);
			panelBase.SetTitleTxt("虛擬產包活動");
			showView();
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				SHOW_STORE_MONEY_CARD
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case SHOW_STORE_MONEY_CARD:
					showView();
				break;
			}
		}
		
		private function showView():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				onClose(null);
				return;
			}
			this.panelBase.name = NewerCardNewMediator.CARD_NAEM;
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			panelBase.x = 100;
			panelBase.y = 100;
			mainView.txt3 = "";
			dealEventListeners(true);
			UIConstData.FocusIsUsing = true;
			this.mainView.setTxtFocus = true;
		}
		
		private function dealEventListeners(boo:Boolean):void
		{
			if(boo) 
			{
				panelBase.addEventListener(Event.CLOSE,onClose);
				this.mainView.sureBtn.addEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.cancleBtn.addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				panelBase.removeEventListener(Event.CLOSE,onClose);
				this.mainView.sureBtn.removeEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.cancleBtn.removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "send_btn":
					onSend();
				break;
				case "cancel_btn":
					onClose(null);
				break;
			}
		}
		
		private function onSend():void
		{			//TWA1-1708a1-393e30-df5ef6-d08125
			var sendMsg:String = mainView.inputContent;		
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "")
			{
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_1" ], color:0xffff00});//"请输入卡号"
				return;
			}
			
			if ( sendMsg.length < 32 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			var aStr:Array = sendMsg.split("-");
			if ( aStr[0].length != 4  )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			
			Zippo.SendCard(sendMsg ,13); //13 台服充值卡
			onClose(null);
			
		}
		private function onClose(e:Event):void
		{
			dealEventListeners(false);
			UIConstData.FocusIsUsing = true;
			this.mainView.setTxtFocus = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
		}
	}
}