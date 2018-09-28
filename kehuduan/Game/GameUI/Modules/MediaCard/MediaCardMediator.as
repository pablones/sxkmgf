package GameUI.Modules.MediaCard
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.ResourcesFactory;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MediaCardMediator extends Mediator
	{
		/**
		 * 媒体卡 
		 */		
		public static const NAME:String = "MediaCardMediator";
		
		
		private var panelBase:PanelBase;
		
		public function MediaCardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get mediaCardView():MovieClip
		{
			return this.viewComponent as MovieClip
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				MediaData.MEDIATCARD_INIT
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case MediaData.MEDIATCARD_INIT:
					initView();
					break;
			}
		}
		
		private function initView():void
		{
			if(!this.panelBase)
			{
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/MediaCard.swf",onLoadComplete);
				LoadingView.getInstance().showLoading();
			}
			else{
				showView();
			}
		}
		
		private function onLoadComplete():void
		{
			LoadingView.getInstance().removeLoading();
			var MC:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/MediaCard.swf");
			var mediaCardMC:MovieClip = new (MC.loaderInfo.applicationDomain.getDefinition("MediaCardView") as Class);
			this.setViewComponent(mediaCardMC);
			
			//初始化面板
			
			
			(mediaCardView.txtInfo as TextField).maxChars = 18;
			(mediaCardView.txtInfo as TextField).multiline = false;
			
			
			  (mediaCardView.gainCard_txt as TextField).htmlText = '<a href="event:reload_'+GameCommonData.wordDic[ "mod_med_med_onL_1" ]+'">'+GameCommonData.wordDic[ "mod_med_med_onL_2" ]+'</a>';//"刷新"	"点此获取礼包兑换码"
			(mediaCardView.gainCard_txt as TextField).addEventListener(TextEvent.LINK,onGainCard); 
			
			 /* (mediaCardView.cardLost_txt as TextField).htmlText = '<a href="event:reload_'+"失效"+'">'+"若卡号无效，请点击"+'</a>';
			(mediaCardView.cardLost_txt as TextField).addEventListener(TextEvent.LINK,onUseless);   */
			
			panelBase = new PanelBase(this.mediaCardView,mediaCardView.width-26,mediaCardView.height+12);
			panelBase.x = UIConstData.DefaultPos2.x - 300;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_med_med_onL_3" ]);//"媒体大礼包"
			panelBase.addEventListener(Event.CLOSE,onClose);
			showView();
		}
		
		private function showView():void
		{
			
			(mediaCardView.txtInfo as TextField).text = "";
			(mediaCardView.send_btn as SimpleButton).addEventListener(MouseEvent.CLICK,onSend);
			(mediaCardView.cancel_btn as SimpleButton).addEventListener(MouseEvent.CLICK,onClose);
			UIUtils.addFocusLis(mediaCardView.txtInfo);
			UIConstData.FocusIsUsing = true;
			
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			
		}
		
		private function onSend(me:MouseEvent):void
		{
			var sendMsg:String = mediaCardView.txtInfo.text;
			sendMsg = sendMsg.split("\r").join("");
			if(sendMsg.replace(/^\s*|\s*$/g,"").split(" ").join("") == "")
			{
 				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_med_onS_1" ], color:0xffff00});//"请输入媒体卡卡号"
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
			
			Zippo.SendMediaCard(sendMsg );
			onClose(null);
			
		}
			
		   private function onGainCard(te:TextEvent):void
		{
			navigateToURL( new URLRequest("http://www.07073.com/2010top/votes_19.html" ), "_blank" );
		} 
		
		/*  private function onUseless(te:TextEvent):void
		{
			navigateToURL( new URLRequest("http://www41.53kf.com/webCompany.php?arg=91wan&style=7" ), "_blank" );
		}  */  
		
		 private function onClose(e:Event):void
		{
			(mediaCardView.send_btn as SimpleButton).removeEventListener(MouseEvent.CLICK,onSend);
			(mediaCardView.cancel_btn as SimpleButton).removeEventListener(MouseEvent.CLICK,onClose);
			UIUtils.removeFocusLis(mediaCardView.txtInfo);
			UIConstData.FocusIsUsing = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
		} 
	}
}