package GameUI.Modules.LotteryCardFor360
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LotteryCardFor360Mediator extends Mediator
	{
		public static const NAME:String = "LotteryCardFor360";
		public static const SHOW_LOTTERYCARD_PANEL:String = "SHOW_LOTTERYCARD_PANEL";			//打开界面
		private var panelBase:PanelBase;
		private var loader:Loader;
		
		public function LotteryCardFor360Mediator(mediatorName:String=null, viewComponent:Object=null)
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
					SHOW_LOTTERYCARD_PANEL
				];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case SHOW_LOTTERYCARD_PANEL:
					if(!panelBase)
					{
						loader = new Loader();
						loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoadComplete); 
						loader.load(new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ActivityCard360.swf"));
					}
					else
					{
						showPanel();
					}
				break;
			}
		}
		
		private function onLoadComplete(e:Event):void
		{
			var tempMc:MovieClip = new (loader.contentLoaderInfo.applicationDomain.getDefinition("lotteryCard"))();
			setViewComponent(tempMc);
			panelBase = new PanelBase(tempMc, tempMc.width - 26, tempMc.height+14);
			panelBase.x = UIConstData.DefaultPos2.x - 300;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_hero_med_lot_lott" ]);//御剑江湖抽奖活动
			panelBase.addEventListener(Event.CLOSE,onClose);
			mainView.txt_input.multiline = false;
			
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
			mainView.txt_input.text = "";
			mainView.btn_send.addEventListener(MouseEvent.CLICK,onMouseClick); 
			mainView.btn_cancel.addEventListener(MouseEvent.CLICK,onMouseClick); 
			GameCommonData.GameInstance.stage.focus = mainView.txt_input;
			UIUtils.addFocusLis(mainView.txt_input);
			UIConstData.FocusIsUsing = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
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
			var aStr:Array = sendMsg.split("-");
			if ( aStr[0].length != 4  )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_2" ], color:0xffff00});//"卡号的格式不正确"
				return;
			}
			
			Zippo.Send360Lottery(sendMsg );
			onClose(null);
			
		}
		
		private function onClose(e:Event):void
		{
			mainView.btn_send.removeEventListener(MouseEvent.CLICK,onMouseClick); 
			mainView.btn_cancel.removeEventListener(MouseEvent.CLICK,onMouseClick);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
		}
	}
}