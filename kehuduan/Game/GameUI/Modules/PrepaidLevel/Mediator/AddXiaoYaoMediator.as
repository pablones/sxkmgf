package GameUI.Modules.PrepaidLevel.Mediator
{
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidDataProxy;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class AddXiaoYaoMediator extends Mediator
	{
		public static const NAME:String = "xiaoyaoMediator";
		private var dataProxy:PrepaidDataProxy;
		private var panelBase:PanelBase;
		private var fastPurchase:FastPurchase;
		private var archeausNum:uint;
		
		public function AddXiaoYaoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		private function get XiaoYaoView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					PrepaidUIData.INIT_ADDXIAOYAO,
					PrepaidUIData.SHOW_ADDXIAOYAO,
					PrepaidUIData.CLOSE_ADDXIAOYAO,
					PrepaidUIData.UPDATE_ADDXIAOYAO,
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case PrepaidUIData.INIT_ADDXIAOYAO:
					dataProxy = facade.retrieveProxy( PrepaidDataProxy.NAME ) as PrepaidDataProxy;
					initUI( notification.getBody() as MovieClip );
					break;
				case PrepaidUIData.SHOW_ADDXIAOYAO:
					showUI( notification.getBody() as uint );
					break;
				case PrepaidUIData.CLOSE_ADDXIAOYAO:
					closeUI(null);
					break;
				case PrepaidUIData.UPDATE_ADDXIAOYAO:
					if( dataProxy.xiaoyaoIsOpen == 0 ) return;
					archeausNum = notification.getBody() as uint;
					XiaoYaoView.xiaoyao_txt.htmlText = "使用<font color='#00FFFF'>远古秘药</font>可以加倍领取离线挂机奖励的经验，当前拥有数量为<font color='#00FF00'>"+archeausNum+"</font>个，是否确认使用<font color='#00FF00'>"+dataProxy.xiaoyaoIsOpen+"</font>个<font color='#00FFFF'>远古秘药</font>来领取离线挂机经验？";
					break;
			}
		}
		
		private function initUI( xiaoyao:MovieClip ):void
		{
			this.viewComponent = xiaoyao;
			fastPurchase = new FastPurchase("630014");
			fastPurchase.y = 2;
			fastPurchase.x = 260;
			XiaoYaoView.addChild(fastPurchase);
			panelBase = new PanelBase( XiaoYaoView,XiaoYaoView.width+80,XiaoYaoView.height+12 );
			panelBase.SetTitleTxt( "使用远古秘药" );//""
			panelBase.x = (GameCommonData.GameInstance.stage.stageWidth - panelBase.width)/2;
			panelBase.y = (GameCommonData.GameInstance.stage.stageHeight - panelBase.height)/2;
			XiaoYaoView.xiaoyao_txt.mouseEnabled = false;
		}
		
		private function showUI( type:uint ):void
		{
			switch( type )
			{
				case 1:
					if( dataProxy.xiaoyaoIsOpen == 1 ) return;
					if( dataProxy.xiaoyaoIsOpen == 0 )
					{
						GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
						panelBase.addEventListener( Event.CLOSE,closeUI );
						XiaoYaoView.btn_ok.addEventListener( MouseEvent.CLICK,onButtonClick );
					}
					archeausNum = BagData.hasItemNum( 630014 );
					XiaoYaoView.xiaoyao_txt.htmlText = "使用<font color='#00FFFF'>远古秘药</font>可以加倍领取离线挂机奖励的经验，当前拥有数量为<font color='#00FF00'>"+archeausNum+"</font>个，是否确认使用<font color='#00FF00'>1</font>个<font color='#00FFFF'>远古秘药</font>来领取离线挂机经验？";
					dataProxy.xiaoyaoIsOpen = 1;
					break;
				case 2:
					if( dataProxy.xiaoyaoIsOpen == 2 ) return;
					if( dataProxy.xiaoyaoIsOpen == 0 )
					{
						GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
						panelBase.addEventListener( Event.CLOSE,closeUI );
						XiaoYaoView.btn_ok.addEventListener( MouseEvent.CLICK,onButtonClick );
					}
					archeausNum = BagData.hasItemNum( 630014 );
					XiaoYaoView.xiaoyao_txt.htmlText = "使用<font color='#00FFFF'>远古秘药</font>可以加倍领取离线挂机奖励的经验，当前拥有数量为<font color='#00FF00'>"+archeausNum+"</font>个，是否确认使用<font color='#00FF00'>2</font>个<font color='#00FFFF'>远古秘药</font>来领取离线挂机经验？";
					dataProxy.xiaoyaoIsOpen = 2;
					break;
			}
		}
		
		private function onButtonClick(event:MouseEvent):void
		{
			if( dataProxy.xiaoyaoIsOpen == 1 )
			{
				if(archeausNum > 0)
				{		
					sendNotification( AutoPlayEventList.SHOW_OFFLINEPLAY_FROM_PREPAID, 2 );
				}
				else
				{
					sendNotification(HintEvents.RECEIVEINFO, {info:"你的远古秘药数量不足", color:0xffff00});
				}
				closeUI(null);
			}
			else if( dataProxy.xiaoyaoIsOpen == 2 )
			{
				if(archeausNum > 1)
				{		
					sendNotification( AutoPlayEventList.SHOW_OFFLINEPLAY_FROM_PREPAID, 3 );
				}
				else
				{
					sendNotification(HintEvents.RECEIVEINFO, {info:"你的远古秘药数量不足", color:0xffff00});
				}
				closeUI(null);
			}
		}
		
		private function closeUI( evt:Event ):void
		{
			dataProxy.xiaoyaoIsOpen = 0;
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeUI );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				XiaoYaoView.removeEventListener( MouseEvent.CLICK,onButtonClick );
			}
		}
	}
}