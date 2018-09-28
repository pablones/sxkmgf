package GameUI.Modules.Login.StartMediator
{
	import Net.AccNet;
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.system.Security;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	
	public class TestLogin
	{
		private static  var txtAccmoute:TextField;
		private static var Password:TextField;
		private static var btnLogin:Sprite;
		private static var txtAccmoute_label:TextField;
		private static var Password_label:TextField;
		private static var txtLogin_label:TextField;
		
		private static var acc_Label:TextField;
		private static var source_Label:TextField;
		
		private static var acc_txt:TextField = new TextField();
		private static var source_txt:TextField = new TextField();
		
		//测试完删除
		public static var test_txt:TextField = new TextField();
		public static  function login():void
		{
			txtAccmoute 					= new TextField();
			Password    					= new TextField();
			btnLogin                        = new Sprite();
			txtAccmoute.text  = GameCommonData.LoginName;
			Password.text     = GameCommonData.LoginPassword.toString();
			txtAccmoute.borderColor = 0x000000;
			txtAccmoute.background = true; 
			txtAccmoute.width  = 100;
			txtAccmoute.height = 20;
			txtAccmoute.x = 50; 
			txtAccmoute.y = 20; 
			txtAccmoute.type = TextFieldType.INPUT;
			Password.borderColor = 0x000000;
			Password.background = true; 
			Password.x = 50;
			Password.y = 50;
			Password.width  = 100;
			Password.height = 20;
			Password.type = TextFieldType.INPUT;
			btnLogin.graphics.beginFill(0x000000 , 1);
			btnLogin.graphics.moveTo(70 , 80);
			btnLogin.graphics.lineTo(70 , 105);
			btnLogin.graphics.lineTo(120 , 105);
			btnLogin.graphics.lineTo(120 , 80);
			btnLogin.graphics.lineTo(70 , 80);
			btnLogin.buttonMode = true;
			GameCommonData.GameInstance.GameUI.addChild(btnLogin);
			GameCommonData.GameInstance.GameUI.addChild(txtAccmoute);
			GameCommonData.GameInstance.GameUI.addChild(Password);
			btnLogin.mouseEnabled = true;
			btnLogin.addEventListener(MouseEvent.CLICK , loginHandler);
			
			
			//文本标签
			txtAccmoute_label = new TextField();
			Password_label    = new TextField();
			txtLogin_label    = new TextField();
			txtAccmoute_label.text = "账号";
			Password_label.text    = "密码";
			txtLogin_label.text    = "登录";
			txtAccmoute_label.mouseEnabled = false;
			Password_label.mouseEnabled    = false;
			txtLogin_label.mouseEnabled    = false;
			txtLogin_label.textColor       = 0xffffff;
			txtAccmoute_label.y = 20;
			Password_label.y    = 50;
			txtLogin_label.y    = 85;
			txtLogin_label.x    = 80;
			GameCommonData.GameInstance.GameUI.addChild(txtAccmoute_label);
			GameCommonData.GameInstance.GameUI.addChild(Password_label);
			GameCommonData.GameInstance.GameUI.addChild(txtLogin_label);
			GameCommonData.GameInstance.GameUI.setChildIndex(txtLogin_label , GameCommonData.GameInstance.GameUI.numChildren - 1);
			
			acc_Label = new TextField();
			acc_Label.text = "账号服地址："
			acc_Label.y = 130;
			acc_Label.mouseEnabled = false;
			source_Label = new TextField();
			source_Label.text = "资源地址：";
			source_Label.mouseEnabled = false;
			source_Label.y = 158;
			GameCommonData.GameInstance.GameUI.addChild( acc_Label );
			GameCommonData.GameInstance.GameUI.addChild( source_Label );
			
			acc_txt.borderColor = 0x000000;
			acc_txt.background = true; 
			acc_txt.width  = 180;
			acc_txt.height = 20;
			acc_txt.x = 78; 
			acc_txt.y = 130; 
			acc_txt.type = TextFieldType.INPUT;
			acc_txt.text = GameConfigData.AccSocketIP.toString();
			
			source_txt.borderColor = 0x000000;
			source_txt.background = true; 
			source_txt.width  = 180;
			source_txt.height = 20;
			source_txt.x = 78; 
			source_txt.y = 158; 
			source_txt.type = TextFieldType.INPUT;
			source_txt.text = GameCommonData.GameInstance.Content.RootDirectory.toString();
			
			GameCommonData.GameInstance.GameUI.addChild( acc_txt );
			GameCommonData.GameInstance.GameUI.addChild( source_txt );
			
			test_txt.x = 80;
			test_txt.y = 120;
			test_txt.autoSize = TextFieldAutoSize.LEFT;
			GameCommonData.GameInstance.GameUI.addChild(test_txt);
		}
		private static  function loginHandler(e:MouseEvent):void
		{
			if(GameCommonData.isReceiveAcc)
			{
				return;
			}
			GameCommonData.Accmoute = txtAccmoute.text;
			GameCommonData.Password = Password.text;
			
			GameConfigData.AccSocketIP = acc_txt.text;
			GameCommonData.GameInstance.Content.RootDirectory = source_txt.text;
			
			Security.loadPolicyFile("xmlsocket://"+GameConfigData.AccSocketIP+":843");
			GameCommonData.AccNets = new AccNet(GameConfigData.AccSocketIP, GameConfigData.AccSocketPort);
		}
		
		
		public static  function removeLogin():void
		{
			if(txtAccmoute_label && GameCommonData.GameInstance.GameUI.contains(txtAccmoute_label))
			GameCommonData.GameInstance.GameUI.removeChild(txtAccmoute_label);
			
			if(Password_label && GameCommonData.GameInstance.GameUI.contains(Password_label))
			GameCommonData.GameInstance.GameUI.removeChild(Password_label);
			
			if(Password_label && GameCommonData.GameInstance.GameUI.contains(txtLogin_label))
			GameCommonData.GameInstance.GameUI.removeChild(txtLogin_label);
			
			if(btnLogin && GameCommonData.GameInstance.GameUI.contains(btnLogin))
			GameCommonData.GameInstance.GameUI.removeChild(btnLogin);
			
			if(txtAccmoute && GameCommonData.GameInstance.GameUI.contains(txtAccmoute))
			GameCommonData.GameInstance.GameUI.removeChild(txtAccmoute);
			
			if(Password && GameCommonData.GameInstance.GameUI.contains(Password))
			GameCommonData.GameInstance.GameUI.removeChild(Password);
			
			////////////////////////////////
			if(acc_Label && GameCommonData.GameInstance.GameUI.contains(acc_Label))
			GameCommonData.GameInstance.GameUI.removeChild(acc_Label);
			if(acc_txt && GameCommonData.GameInstance.GameUI.contains(acc_txt))
			GameCommonData.GameInstance.GameUI.removeChild(acc_txt);
			if(source_Label && GameCommonData.GameInstance.GameUI.contains(source_Label))
			GameCommonData.GameInstance.GameUI.removeChild(source_Label);
			if(source_txt && GameCommonData.GameInstance.GameUI.contains(source_txt))
			GameCommonData.GameInstance.GameUI.removeChild(source_txt);
		}

	}
}