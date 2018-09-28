package GameUI.Modules.Login.Mediator
{ 
	import GameUI.ConstData.EventList;
	import GameUI.UIConfigData;
	
	import Net.AccNet;
	import Net.ActionSend.FriendSend;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LoginMediator extends Mediator
	{
		public static const NAME:String = "LoginMediator";
		
		public function LoginMediator()
		{
			super(NAME);
		}
		
		private function get loginPanel():MovieClip
		{
			return viewComponent as MovieClip
		}
		
		override public function listNotificationInterests():Array 
		{
			return [
				EventList.INITVIEW,
				EventList.REMOVELOGIN
			];
		}
		
		override public function handleNotification(notification:INotification):void 
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.LOGINPANEL});
					GameCommonData.GameInstance.GameUI.addChild(loginPanel);
					this.loginPanel.userid.text = GameCommonData.LoginName;
					this.loginPanel.pass.text   = "1";
					this.loginPanel.confrim.addEventListener(MouseEvent.CLICK, loginHandler);
				break;
				case EventList.REMOVELOGIN:
					gc();
					facade.removeMediator(LoginMediator.NAME);
			
				break;
				
			}
		}
		
		private function gc():void
		{
			this.loginPanel.confrim.removeEventListener(MouseEvent.CLICK, loginHandler);
			GameCommonData.GameInstance.GameUI.removeChild(loginPanel);
		}
		
		//提交登录信息
		private function loginHandler(e:MouseEvent):void
		{
			GameCommonData.Accmoute = this.loginPanel.userid.text;
			GameCommonData.Password = this.loginPanel.pass.text;
			GameCommonData.AccNets = new AccNet(GameConfigData.AccSocketIP, GameConfigData.AccSocketPort);
		}
	}
}