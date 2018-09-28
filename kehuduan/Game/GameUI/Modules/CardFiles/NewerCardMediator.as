package GameUI.Modules.CardFiles
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.CardBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewerCardMediator extends Mediator
	{
		public static const NAME:String = "PPTVNewerCardMediator";
		public static const SHOW_NEWER_CARD:String = "SHOW_NEWER_CARD";
		private var _showNum:int;
		private var isShowNumChange:Boolean;
		private var panelBase:PanelBase;
		private var inputLength:int;
		private var sendNum:int;
		private var inputRule:String;
		public function NewerCardMediator()
		{
			super(NAME);
		}
		
		private function get mainView():CardBase
		{
			return this.viewComponent as CardBase;
		}

		override public function listNotificationInterests():Array
		{
			return [
				SHOW_NEWER_CARD,
				EventList.CLOSE_NPC_ALL_PANEL
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case SHOW_NEWER_CARD:
					var body:Object = notification.getBody();
					inputLength = body.inputLength;
					sendNum = body.sendNum;
					inputRule = body.inputRule;
					dealShow(body.showNum);
				break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					if(GameCommonData.GameInstance.GameUI.contains(panelBase))
					{
						onClose(null);
					}
				break;		
			}
		}
		
		private function dealShow(showNum:int):void
		{
			if(showNum == _showNum)
			{
				isShowNumChange = false;
			}
			else
			{
				isShowNumChange = true;
			}
			_showNum = showNum;
			if(!this.mainView)
			{
				var cardBase:CardBase = new CardBase();
				this.setViewComponent(cardBase);
				cardBase.initCardFun = setCardContent
			}
			else
			{
				setCardContent();
			}
		}
		
		private function setCardContent():void
		{
			if(isShowNumChange)
			{
				mainView.expData = _showNum;
				if(!inputLength)
				{
					inputLength = 16;
				}
				mainView.txt2Length = inputLength;
				deletePanelBase();
				panelBase = new PanelBase(this.mainView,mainView.width + 8,mainView.height + 12);
				panelBase.SetTitleTxt(mainView.cardName);
				mainView.sureTxt = GameCommonData.wordDic[ "often_used_confim" ];//"确  认";
			}
			this.showView();
		}
		private function deletePanelBase():void
		{
			if(panelBase)
			{
				if(panelBase.hasEventListener(Event.CLOSE))
				{
					panelBase.removeEventListener(Event.CLOSE,onClose);
				}
				if(panelBase.parent)
				{
					panelBase.parent.removeChild(panelBase);
				}
				panelBase.dispose();
				panelBase = null;
			}
		}
		private function showView():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				onClose(null);
				return;
			}
			mainView.txt3 = "";
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			panelBase.x = 100;
			panelBase.y = 100;
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
			
			if (sendMsg.length < inputLength)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			if(_showNum != 60)	//不是pptv平台进入
			{
				if(inputRule)
				{
					var tempStr:String = sendMsg.substr(0,inputRule.length);
					if(tempStr != inputRule)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
						return;
					}
				}
			}
			
			Zippo.SendCard(sendMsg ,sendNum); //pptv新手卡
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