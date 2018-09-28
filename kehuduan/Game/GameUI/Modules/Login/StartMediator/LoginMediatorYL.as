package  GameUI.Modules.Login.StartMediator
{ 
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	
	import Net.AccNet;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.facade.Facade;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LoginMediatorYL extends Mediator
	{
		public static const NAME:String = "LoginMediatorYL";
		private var loginPanel:MovieClip;
		
		public function LoginMediatorYL()
		{
			super(NAME);
		}
		
//		private function get loginPanel():MovieClip
//		{
//			return viewComponent as MovieClip
//		}
		
		override public function listNotificationInterests():Array 
		{
			return [
//				EventList.INITVIEW,
				EventList.SHOWLOGIN,
				EventList.REMOVELOGIN
			];
		}
		
		override public function handleNotification(notification:INotification):void 
		{
			switch(notification.getName())
			{
//				case EventList.INITVIEW:
//					loginPanel = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip("LoginPanel");
////					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.LOGINPANEL});
//					GameCommonData.GameInstance.GameUI.addChild(loginPanel);
//					this.loginPanel.userid.text = GameCommonData.LoginName;
//					this.loginPanel.pass.text   = "1";
//					this.loginPanel.confrim.addEventListener(MouseEvent.CLICK, loginHandler);
//				break;
				case EventList.SHOWLOGIN:
					loginPanel = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip("LoginPanel");
//					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.LOGINPANEL});
					GameCommonData.GameInstance.GameUI.addChild(loginPanel);
					this.loginPanel.userid.text = GameCommonData.LoginName;
					this.loginPanel.pass.text   = GameCommonData.LoginPassword;
					this.loginPanel.confrim.addEventListener(MouseEvent.CLICK, loginHandler);
				break;
				case EventList.REMOVELOGIN:
					gc();
					facade.removeMediator(LoginMediatorYL.NAME);
			
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
//			trace("GameCommonData.RoleList.length",GameCommonData.RoleList.length)
			
		}
//		/** 进入游戏 */
//		public static function enterGame():void
//		{
//			if(GameCommonData.RoleList.length == 1)										//加载了UI并登陆了游戏服务器后，如果已有一个角色，进入游戏
//			{
//				facade.sendNotification(CommandList.SELECTROLECOMMAND, 0);				//进入游戏
//			}
//			else if(GameCommonData.RoleList.length == 0)								//如果没有角色，直接进入创建角色界面
//			{
//				facade.sendNotification(EventList.SHOWCREATEROLE);
//			}
//		}
	}
}