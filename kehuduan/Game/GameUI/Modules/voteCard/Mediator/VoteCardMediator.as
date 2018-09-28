package GameUI.Modules.voteCard.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.voteCard.Data.VoteData;
	import GameUI.Modules.voteCard.Data.VoteResource;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class VoteCardMediator extends Mediator
	{
		public static const NAME:String = "VoteCardMediator";
		private var panelBase:PanelBase;
		private var main_mc:MovieClip;
		
		private var upState_cancel:DisplayObject;
		private var overState_cancel:DisplayObject;
		private var upState_send:DisplayObject;
		private var overState_send:DisplayObject;
		
		public function VoteCardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [VoteData.CLICK_GIFT_GIRL,
						VoteData.SHOW_VOTE_PANEL,
						VoteData.CLOSE_VOTE_PANEL
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case VoteData.CLICK_GIFT_GIRL:
					clickGirlHandler();
				break;
				
				case VoteData.SHOW_VOTE_PANEL:
					showPanel();
				break;
				
				case VoteData.CLOSE_VOTE_PANEL:
					panelClosed(null);
				break;
			}
		}
		
		private function clickGirlHandler():void
		{
			if ( VoteData.voteResoureLoaded )
			{
				showPanel();
			}
			else
			{
				new VoteResource();
			}
		}
		
		private function showPanel():void
		{
			if ( !panelBase )
			{
				main_mc = this.viewComponent as MovieClip;
//				panelBase = new PanelBase(main_mc, main_mc.width - 26, main_mc.height+10);
				panelBase = new PanelBase(main_mc, main_mc.width - 26, main_mc.height+14);
				panelBase.x = ChatData.tmpLeoPoint.x;
				panelBase.y = ChatData.tmpLeoPoint.y;
				panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_vot_med_vot_show_1" ] );   // 17173投票礼包
				this.overState_cancel = ( main_mc.cancel_btn as SimpleButton ).overState;
				this.upState_cancel = ( main_mc.cancel_btn as SimpleButton ).upState;
				this.overState_send = ( main_mc.send_btn as SimpleButton ).overState;
				this.upState_send = ( main_mc.send_btn as SimpleButton ).upState;
			}
			panelBase.addEventListener(Event.CLOSE, panelClosed);
			initUI();
		}
		
		private function initUI():void
		{
			if ( !main_mc ) return;
			main_mc.send_btn.addEventListener(MouseEvent.CLICK, send);
			main_mc.cancel_btn.addEventListener(MouseEvent.CLICK, cancel);
			
			///////////////////////////////防止按钮变态
			main_mc.send_btn.addEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
			main_mc.cancel_btn.addEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			
			main_mc.txtInfo.text = "";
			main_mc.txtInfo.maxChars = 18;
			main_mc.txtInfo.multiline = false;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			
			/* main_mc.gainCard_txt.htmlText = '<a href="event:reload_'+"刷新"+'">'+"点此获取礼包兑换码"+'</a>';
			main_mc.gainCard_txt.addEventListener(TextEvent.LINK,openPage);
			
			main_mc.cardLost_txt.htmlText = '<a href="event:reload_'+"失效"+'">'+"若卡号无效，请点击"+'</a>';
			main_mc.cardLost_txt.addEventListener(TextEvent.LINK,lostCard); */
			
			UIUtils.addFocusLis(main_mc.txtInfo);
			main_mc.stage.focus = main_mc.txtInfo;
			UIConstData.FocusIsUsing = true;
		}
		
		private function overSendBtnHandler( evt:MouseEvent ):void
		{
			main_mc.send_btn.removeEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
			( main_mc.send_btn as SimpleButton ).overState = this.overState_send;	
		}
		
		private function overCancelHandler( evt:MouseEvent ):void
		{
			main_mc.cancel_btn.removeEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			( main_mc.cancel_btn as SimpleButton ).overState = this.overState_cancel;
		}
		
		private function send(evt:MouseEvent):void
		{
			var sendMsg:String = main_mc.txtInfo.text;
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "")
			{
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_vot_med_vot_send_1" ], color:0xffff00});     //请输入投票卡卡号
				return;
			}
			
			if ( sendMsg.length<18 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_vot_med_vot_send_2" ], color:0xffff00});    //卡号的格式不正确
				return;
			}
			
			var aStr:Array = sendMsg.split("-");
//			if ( aStr.length != 2 )
//			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"卡号的格式不正确", color:0xffff00});
//				return;
//			}
//			else
//			{
			if ( (aStr[0] != "YJTP") )
//			if ( (aStr[0] != "LQIS") )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_vot_med_vot_send_2" ], color:0xffff00});    //卡号的格式不正确
				return;
			}
//			}

			Zippo.SendDevoteCard( sendMsg );
			panelClosed(null);
		}
		
		private function cancel(evt:MouseEvent):void
		{
			panelClosed(null);
		}
		
		private function panelClosed(evt:Event):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				main_mc.send_btn.removeEventListener(MouseEvent.CLICK, send);
				main_mc.cancel_btn.removeEventListener(MouseEvent.CLICK, cancel);
				panelBase.removeEventListener(Event.CLOSE, panelClosed);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				( main_mc.send_btn as SimpleButton ).overState = this.upState_send;
				( main_mc.cancel_btn as SimpleButton ).overState = this.upState_cancel;
				main_mc.send_btn.removeEventListener( MouseEvent.MOUSE_OVER, overSendBtnHandler );
				main_mc.cancel_btn.removeEventListener( MouseEvent.MOUSE_OVER, overCancelHandler );
			}
			UIConstData.FocusIsUsing = false;
		}
		
		/* private function openPage(evt:TextEvent):void
		{
			navigateToURL( new URLRequest ( "http://huodong.yaoguo.duowan.com/10yearend/voteSpread.jsp?gameCode=YJJH&gameServer=S1" ), "_blank" );
		}
		
		private function lostCard(evt:TextEvent):void
		{
//			navigateToURL( new URLRequest ( "http://web.4399.com/kefu/online/yjjh.html" ), "_blank" );
			navigateToURL( new URLRequest ( "http://www41.53kf.com/webCompany.php?arg=91wan&style=7" ), "_blank" );
		} */
		
	}
}