package GameUI.Modules.OnlineGetReward.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	import GameUI.Modules.OnlineGetReward.Event.TimeOutEvent;
	import GameUI.Modules.OnlineGetReward.UI.TimeOutTxt;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	
	import OopsFramework.Utils.Timer;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class OnlineRewardMediator extends Mediator
	{
		public static const NAME:String = "OnlineRewardMediator";
		
		private var time:int;
		private var timer:Timer = new Timer();
		private var isDrag:Boolean = false;
		private var timeOutTxt:TimeOutTxt;
		private var cloneTxt:TimeOutTxt;
		private var dataProxy:DataProxy;
		 
		public function OnlineRewardMediator()
		{
			super(NAME);
		}
		
		public function get onLineAwardView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [EventList.INITVIEW,
						EventList.ENTERMAPCOMPLETE,
						OnLineAwardData.NEXT_ONLINE_GIFT
					];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:OnLineAwardData.ONLINE_TIMEOUT_PANEL});
				break;
				
				case EventList.ENTERMAPCOMPLETE:
					if ( (GameCommonData.Player.Role.OnLineAwardTime%10)>7 )
					{
						return;
					}
					initUI();
				break;
				
				case OnLineAwardData.NEXT_ONLINE_GIFT:
					showNextGiftTime();
				break;
			}
		}
		
		private function initUI():void
		{
			if ( !GameCommonData.GameInstance.GameUI.contains( onLineAwardView ) )
			{
				GameCommonData.GameInstance.GameUI.addChild( onLineAwardView );
				onLineAwardView.x = 400;
				onLineAwardView.y = 496;
			}
			onLineAwardView.name = "onLineTimeAward";
			onLineAwardView.buttonMode = true;
			( onLineAwardView.hint_txt as TextField ).mouseEnabled = false;
			
			onLineAwardView.addEventListener(MouseEvent.MOUSE_DOWN,dragView);
			onLineAwardView.addEventListener(MouseEvent.MOUSE_UP,dropView);

//			time = OnLineAwardData.awardTimeArr[ OnLineAwardData.giftIndex ];
			if ( GameCommonData.Player.Role.OnLineAwardTime>=10 && GameCommonData.Player.Role.OnLineAwardTime<=17 )
			{
				time = 0;
				( onLineAwardView.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_onl_ini_1" ];  // 点击领取奖励
				GameCommonData.Player.Role.OnLineAwardTime %= 10; 
			}
			else
			{
				( onLineAwardView.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_onl_ini_2" ];  // 距下次领奖还有
				time = OnLineAwardData.awardTimeArr[ GameCommonData.Player.Role.OnLineAwardTime ];	
			}
			timeOutTxt = new TimeOutTxt(time);
			timeOutTxt.x = 135;
			timeOutTxt.y = 5;
			timeOutTxt.mouseEnabled = false;
			onLineAwardView.addChild( timeOutTxt );
			timeOutTxt.addEventListener( TimeOutEvent.TIME_IS_ZERO,timeOutHandler );
			cloneTxt = timeOutTxt.clone();							//克隆一个时间同步文本，传出去
		}
		
		private function timeOutHandler(evt:TimeOutEvent):void
		{
			( onLineAwardView.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_onl_ini_1" ];  // 点击领取奖励
		}
		
		private function moveMouse(evt:MouseEvent):void
		{
			isDrag = true;
		}
		
		private function dragView(evt:MouseEvent):void
		{
			var w:Number = onLineAwardView.stage.stageWidth-onLineAwardView.width;
			var h:Number = onLineAwardView.stage.stageHeight-onLineAwardView.height;
			onLineAwardView.startDrag( false,new Rectangle(0,0,w,h) );
			onLineAwardView.addEventListener(MouseEvent.MOUSE_MOVE,moveMouse);
		}
		
		private function dropView(evt:MouseEvent):void
		{
			onLineAwardView.stopDrag();
			onLineAwardView.removeEventListener(MouseEvent.MOUSE_MOVE,moveMouse);
			if ( !isDrag )
			{
				facade.sendNotification( OnLineAwardData.ONLINE_GAINWARD_PANENL,{txt:cloneTxt} );	
			}
			isDrag = false;
		}
		
		private function showNextGiftTime():void
		{
			if ( GameCommonData.Player.Role.OnLineAwardTime < 8 )
			{
				OnLineAwardData.canGain = false; 
//				time = OnLineAwardData.awardTimeArr[ OnLineAwardData.giftIndex ];
				var t:uint = uint( OnLineAwardData.awardTimeArr[ GameCommonData.Player.Role.OnLineAwardTime ] );				//测试
				( onLineAwardView.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_onl_ini_2" ];  // 距下次领奖还有
				this.timeOutTxt.reStart( t );
				this.cloneTxt.reStart( t );
			}
			else if ( GameCommonData.Player.Role.OnLineAwardTime>9 && GameCommonData.Player.Role.OnLineAwardTime<18 )
			{
				( onLineAwardView.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_onl_ini_1" ];  // 点击领取奖励
				GameCommonData.Player.Role.OnLineAwardTime %= 10; 
				this.timeOutTxt.reStart( 0 );
				this.cloneTxt.reStart( 0 );
			}
			else
			{
				gc();
			}
		}
		
		private function gc():void
		{
			if ( onLineAwardView && GameCommonData.GameInstance.GameUI.contains(onLineAwardView) )
			{
				onLineAwardView.removeEventListener(MouseEvent.MOUSE_DOWN,dragView);
				onLineAwardView.removeEventListener(MouseEvent.MOUSE_UP,dropView);
				GameCommonData.GameInstance.GameUI.removeChild(onLineAwardView);
				this.timer = null;
				this.cloneTxt = null;
				this.timeOutTxt = null;
			}
			
			if ( dataProxy.GainAwardPanIsOpen )
			{
				facade.sendNotification( OnLineAwardData.CLOSE_GAINAWARD_PAN );
			}
		}
	}
}