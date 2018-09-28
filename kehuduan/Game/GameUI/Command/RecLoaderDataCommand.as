package GameUI.Command
{
	import Controller.TransferSceneController;
	
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.UIConstData;
	import GameUI.UICore.UIFacade;
	
	import Net.GameNetRecive;
	
	import OopsFramework.Content.Loading.BulkLoader;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class RecLoaderDataCommand extends SimpleCommand
	{
		public static const NAME:String = "RecLoaderDataCommand";
		
		public function RecLoaderDataCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var info:Object = notification.getBody().info;
			GameCommonData.BackGround		 = info.BackGround as MovieClip;
			GameCommonData.Tiao				 = info.tiao as MovieClip;
			GameCommonData.isLoginFromLoader = true;
			GameCommonData.GameInstance.Content.RootDirectory 		 = info.SourceURL;
			GameCommonData.ServerId			 = info.serverid;
			GameCommonData.Accmoute          = info.userName;
			GameCommonData.Password          = info.password;
			GameConfigData.AccSocketIP 	     = info.AccSocketIP;
			GameConfigData.AccSocketPort     = info.AccSocketPort;
			GameCommonData.preventWallowTime = info.preventWallowTime;
			GameCommonData.showAccount 		 = info.showAccount;
			GameCommonData.fcmPower 		 = info.fcmPower;
			GameCommonData.fcmConfig 		 = info.fcmConfig;
			GameCommonData.isNew 			 = info.isNew;
			GameCommonData.isExe = info.isExe;
			if ( info.cztype )
			{
				GameCommonData.cztype 		 = info.cztype;	
			}
			GameConfigData.GmPhpPath		 = "http://" + info.AccSocketIP.toLowerCase().replace("-s.","-bak.") + "/" + GameConfigData.GmPhpPath;
			GameCommonData.GameVersion 		 = info.version;
			
			if ( info.isUpdataVersion == 1 )
			{				
				BulkLoader.version = GameCommonData.GameVersion;										// 有地图数据更新时太使用 
			}
			
			if(!info.userName || !info.password)
			{
				GameCommonData.isLoginFromLoader = false;
			}
			
			GameCommonData.FilterGameServerArr = info.fiterGameServerArr;
			GameCommonData.GameServerArr = info.GameServerArr;
			GameConfigData.GameSocketIP = info.GameSocketIP;
			GameConfigData.GameSocketName = info.GameSocketName;
			GameConfigData.GameSocketPort = info.GameSocketPort;
			GameConfigData.GameSeverNum = info.GameSeverNum;
			
			
			UIConstData.Filter_Switch = info.Filter_Switch;
			if ( UIConstData.Filter_Switch )
			{
				UIConstData.Filter_ad = info.Filter_ad;
				UIConstData.Filter_chat = info.Filter_chat;
				UIConstData.Filter_okName = info.Filter_okName;
				UIConstData.Filter_role = info.Filter_role;
			}
				
			//2个小喇叭
			GameCommonData.soundOn_bmp  = new Bitmap(); 
			GameCommonData.soundOff_bmp = new Bitmap();
			(GameCommonData.soundOn_bmp as Bitmap).bitmapData  = info.soundOn_bmp;
			(GameCommonData.soundOff_bmp as Bitmap).bitmapData = info.soundOff_bmp;
			GameCommonData.RoleList = new Array();
			GameCommonData.RoleList = info.RoleList as Array;
			
			var aRoles:Array = GameCommonData.RoleList.concat();
			
			GameCommonData.GameNets = new GameNetRecive( info.gameSocket,GameConfigData.GameSocketIP,GameConfigData.GameSocketPort );
			GameCommonData.GServerInfo = info.GServerInfo;
//			TransferSceneController.IsTransferScene = info.IsTransferScene;
			if ( info.IsTransferScene == 0 )
			{
				TransferSceneController.IsTransferScene = false;
			}
			else if ( info.IsTransferScene == 1 )
			{
				TransferSceneController.IsTransferScene = true;
			}
			if(GameCommonData.wordVersion == 2)		//台服
			{
				TransferSceneController.IsTransferScene = false;
			}
			GameCommonData.openTreasureStragety = info.openTreasureStragety;
			if ( info.loginSound )
			{
				GameCommonData.loginSoundFromLoader = info.loginSound;
				GameCommonData.loginSoundChannel = info.loginSoundChannel;
				GameCommonData.loginSoundTransform = info.loginSoundTransform;
			}
			GameCommonData.isOpenSoundSwitch = info.isOpenSoundSwitch;
			GameCommonData.wordVersion = info.language;
			try
			{
				GameCommonData.noticeFarmat = info.loadNoticeWay;	
			}
			catch ( e:Error )
			{
				
			}
			if ( !GameCommonData.noticeFarmat )
			{
				GameCommonData.noticeFarmat = 1;
			}
			if ( GameCommonData.wordVersion == 2 )
			{
				GameCommonData.CODE = "big5";
			}
			else
			{
				GameCommonData.CODE = "ANSI";
			}
			
			UIFacade.GetInstance( UIFacade.FACADEKEY ).removeCommand( NAME );
			
			facade.registerCommand( CommandList.SELECTROLECOMMAND, SelectRoleCommand );
			if ( info.selectRoleIndex )
			{
				sendNotification( CommandList.SELECTROLECOMMAND,info.selectRoleIndex );
				GameCommonData.SelectedRoleIndex = info.selectRoleIndex;
			}
			else
			{
				sendNotification( CommandList.SELECTROLECOMMAND,0 );
				GameCommonData.SelectedRoleIndex = 0;
			}
			info = null;
		}
		
	}
}