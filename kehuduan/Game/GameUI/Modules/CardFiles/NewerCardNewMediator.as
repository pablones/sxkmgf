package GameUI.Modules.CardFiles
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.CardBaseNew;
	
	import Net.ActionSend.Zippo;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class NewerCardNewMediator extends Mediator
	{
		public static const NAME:String = "NewerCardNewMediator";
		public static const SHOW_NEWER_CARD_NEW:String = "SHOW_NEWER_CARD_NEW";
		public static const CARD_NAEM:String = "newCardBase";
		private var _showNum:int;
		private var isShowNumChange:Boolean;
		private var panelBase:PanelBase;
		private var inputLength:int;
		private var sendNum:int;
		private var inputRule:*;	//输入规则，字符串 或 array
		private var sendData:Object;
		public function NewerCardNewMediator()
		{
			super(NAME);
		}
		
		private function get mainView():CardBaseNew
		{
			return this.viewComponent as CardBaseNew;
		}
	
		override public function listNotificationInterests():Array
		{
			return [
				SHOW_NEWER_CARD_NEW,
				EventList.CLOSE_NPC_ALL_PANEL,
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case SHOW_NEWER_CARD_NEW:
					dealShow(int(notification.getBody()));
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
				var cardBase:CardBaseNew = new CardBaseNew();
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
			initShowData();
			if(isShowNumChange)
			{
				mainView.expData = _showNum;
				if(!inputLength)
				{
					inputLength = 16;
				}
				deletePanelBase();
				panelBase = new PanelBase(this.mainView,mainView.width + 8,mainView.height + 12);
				panelBase.SetTitleTxt(mainView.cardName);
				mainView.sureTxt = GameCommonData.wordDic[ "often_used_confim" ];//"确  认";
			}
			this.showView();
		}
		private function initShowData():void
		{
			var sendData:Object = mainView.getSendData(_showNum);
			inputLength = sendData.inputLength;
			sendNum = sendData.sendNum;
			inputRule = sendData.inputRule;
			mainView.txt2Length = inputLength;
		}
		private function deletePanelBase():void
		{
			if(panelBase)
			{
				if(panelBase.parent)
				{
					panelBase.parent.removeChild(panelBase);
				}
				dealEventListeners(false);
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
			this.panelBase.name = CARD_NAEM;
			mainView.txt3 = "";
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			panelBase.x = 100;
			panelBase.y = 100;
			dealEventListeners(true);
		}
		
		private function dealEventListeners(boo:Boolean):void
		{
			if(boo) 
			{
				UIConstData.FocusIsUsing = true;
				this.mainView.setTxtFocus = true;
				panelBase.addEventListener(Event.CLOSE,onClose);
				this.mainView.sureBtn.addEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.cancleBtn.addEventListener(MouseEvent.CLICK,onMouseClick);
			} 
			else
			{
				UIConstData.FocusIsUsing = false;
				this.mainView.setTxtFocus = false;
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
			if(inputRule)
			{
				if(!checkRule(sendMsg))
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
					return;
				}
			}
			
			Zippo.SendCard(sendMsg ,sendNum); //pptv新手卡
			onClose(null);
			
		}
		private function checkRule(msgStr:String):Boolean
		{
			var boo:Boolean;
			var tempStr:String;
			if(inputRule is String)
			{
				tempStr = msgStr.substr(0,data.length);
				if(tempStr == inputRule)
				{
					boo = true;
				}
			}
			else if(inputRule is Array)
			{
				for each(var data:String in inputRule)
				{
					tempStr = msgStr.substr(0,data.length);
					if(tempStr == data)
					{
						boo = true;
						break;
					}
				}
			}
			return boo;
		}
		private function onClose(e:Event):void
		{
			dealEventListeners(false);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
		}
	}
}