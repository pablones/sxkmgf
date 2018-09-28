package GameUI.Modules.CardForTaiWan
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.CardFiles.NewerCardNewMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewCardForTaiWanMediator extends Mediator
	{
		public static const NAME:String = "NewCardForTaiWanMediator";
		public static const SHOW_TW_CARD_PANEL:String = "SHOW_TW_CARD_PANEL";			//打开界面
		private var panelBase:PanelBase;
		private var loader:Loader;
		private var inputRuleData:Array;
		
		public function NewCardForTaiWanMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		private function get mainView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		public override function listNotificationInterests():Array
		{
			return [
					SHOW_TW_CARD_PANEL
				];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case SHOW_TW_CARD_PANEL:
					showPanel();
				break;
			}
		}
		
		override public function onRegister():void
		{
			loader = new Loader();
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoadComplete); 
			loader.load(new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewerCardForTaiWan.swf"));
		}
		private function onLoadComplete(e:Event):void
		{
			var tempMc:MovieClip = new (loader.contentLoaderInfo.applicationDomain.getDefinition("NewerCard"))();
			setViewComponent(tempMc);
			panelBase = new PanelBase(tempMc, tempMc.width+8, tempMc.height+12);
			panelBase.SetTitleTxt("新手大禮包");
			panelBase.addEventListener(Event.CLOSE,onClose);
			mainView.txt_input.multiline = false;
			(mainView.txt_input as TextField).mouseWheelEnabled = false;
			inputRuleData = ["YJXS","BHMT","JDXS","YXXS","FSJM","XSLL"];
			loader.unload();
			loader.contentLoaderInfo.removeEventListener(Event.COMPLETE,onLoadComplete); 
			loader = null;
			showPanel();
		}
		
		private function showPanel():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				onClose(null); 
				return;
			}
			this.panelBase.name = NewerCardNewMediator.CARD_NAEM;
			mainView.txt_input.text = "";
			mainView.btn_send.addEventListener(MouseEvent.CLICK,onMouseClick); 
			mainView.btn_cancel.addEventListener(MouseEvent.CLICK,onMouseClick); 
			GameCommonData.GameInstance.stage.focus = mainView.txt_input;
			UIUtils.addFocusLis(mainView.txt_input);
			UIConstData.FocusIsUsing = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			panelBase.x = 100;
			panelBase.y = 100;
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "btn_send":
					onSend();
				break;
				case "btn_cancel":
					onClose(null);
				break;
			}
		}
		
		private function onSend():void
		{
			var sendMsg:String = mainView.txt_input.text;
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "")
			{
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_1" ], color:0xffff00});//"请输入卡号"
				return;
			}
			
			if ( sendMsg.length<18 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			if(inputRuleData)
			{
				var tempStr:String = sendMsg.substr(0,inputRuleData[1].length);
				
				if(!checkRule(tempStr))
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
					return;
				}
			}
			
			Zippo.SendCard(sendMsg ,11);
			if(GameCommonData.wordVersion == 2)	//台服
			{
				sendNotification(TaskCommandList.SEND_FB_AWARD,10);	//fb成就10
			}
			onClose(null);
			
		}
		
		private function checkRule(msgStr:String):Boolean
		{
			var boo:Boolean;
			var tempStr:String;
			for each(var data:String in inputRuleData)
			{
				tempStr = msgStr.substr(0,data.length);
				if(data == tempStr)
				{
					boo = true;
					break;
				}
			}
			return boo;
		}
		
		private function onClose(e:Event):void
		{
			mainView.btn_send.removeEventListener(MouseEvent.CLICK,onMouseClick); 
			mainView.btn_cancel.removeEventListener(MouseEvent.CLICK,onMouseClick);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
		}
	}
}