package GameUI.Modules.PreventWallow.Mediator
{
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.GmTools.Utils.LoadSwfTool;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PreventWallow.Data.PreventWallowData;
	import GameUI.Modules.PreventWallow.Data.PreventWallowEvent;
	import GameUI.Modules.PreventWallow.Data.PreventWallowID;
	import GameUI.Modules.ReName.Data.ReNameData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Chat;
	import Net.Protocol;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Net.BaseHttp;
	import OopsFramework.Utils.Timer;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.external.ExternalInterface;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.text.TextField;
	import flash.utils.clearInterval;
	import flash.utils.clearTimeout;
	import flash.utils.getTimer;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PreventWallowMediator extends Mediator implements IUpdateable
	{
		public static const NAME:String = "PreventWallowMediator";
		
		private var loadswfTool:LoadSwfTool;
		private var preventWallowView:MovieClip;
		private var panelBase:PanelBase;
		/** 防沉迷按钮 */
		private var btnPw:MovieClip; 
		private var intervalId:uint;
		private var baseHttp:BaseHttp;
		private var timer:Timer = new Timer();		/** 定时器 */
		private var enabled:Boolean = true;
		private var showCount:int; //防沉迷框的弹出次数 (play.php  中showAccount值为  2*(次数-1)-1 )
		private var pTimer:flash.utils.Timer = new flash.utils.Timer(10000,1);//每次10秒间隔 
		private var allName:String;												//姓名
		private var _ID:String;													//身份证
		private var textmc:MovieClip = new MovieClip();							//放身份证和姓名的容器
		private var exampleText:TextField=new TextField();
		private var _text:TextField = new TextField(); 
		private var isSendSucced:Boolean;	//是否已经提交
		private var isConnectBreak:Boolean;	//是否已经断开连接
		public function PreventWallowMediator()
		{
			super(NAME);
		}
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
        public function get Enabled():Boolean
		{
			return enabled;
		}
		public function Update(gameTime:GameTime):void
		{
			
			if(timer.IsNextTime(gameTime))
			{
				if(PreventWallowData.PreventWallowIsOpen || isConnectBreak) return;
				facade.sendNotification(PreventWallowEvent.SHOWPREVENTWALLOWVIEW);
			}
			if(!PreventWallowData.PREVENT_CHAT_DATA[4])
			{
				if(getTimer() > 3600000)
				{
					PreventWallowData.PREVENT_CHAT_DATA[4] = true;
					facade.sendNotification(CommandList.RECEIVECOMMAND,{info:PreventWallowData.PREVENT_CHAT_DATA[1], nAtt:77777});
				}
				return;
			}
			if(!PreventWallowData.PREVENT_CHAT_DATA[5])
			{
				if(getTimer() > 7200000)
				{
					PreventWallowData.PREVENT_CHAT_DATA[5] = true;
					facade.sendNotification(CommandList.RECEIVECOMMAND,{info:PreventWallowData.PREVENT_CHAT_DATA[2], nAtt:77777});
				}
			}
		}
		public override function listNotificationInterests():Array
		{
			return [
						PreventWallowEvent.SHOWPREVENTWALLOWBTN,
						PreventWallowEvent.CLOSEPREVENTWALLOWBTN,
						PreventWallowEvent.CLOSEPREVENTWALLOWVIEW,
						PreventWallowEvent.SHOWPREVENTWALLOWVIEW,
						PreventWallowEvent.STARTPWTIMER,
						PreventWallowEvent.OVERPWTIMER,
						PreventWallowEvent.CHECT_IS_OPEN_FCM
			        ];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				/** 显示防沉迷按钮 */
				case PreventWallowEvent.SHOWPREVENTWALLOWBTN:
				    if(btnPw == null)
				    {
				    	if(isSendSucced) return;
						btnPw = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("btnPreventWallow");
						btnPw.name = "btnPreventWallow";
						GameCommonData.GameInstance.GameUI.addChild(btnPw); 
						btnPw.x = 216;
						btnPw.y = 3;
//						btnPw.gotoAndStop("up");
					    btnPw.addEventListener(MouseEvent.CLICK , btnPwClickHandler);
					    btnPw.addEventListener(MouseEvent.MOUSE_DOWN , btnPwDownHandler);
					    btnPw.addEventListener(MouseEvent.MOUSE_OVER , btnPwOverHandler);
					    btnPw.addEventListener(MouseEvent.MOUSE_UP , btnPwUpHandler);
					    btnPw.addEventListener(MouseEvent.MOUSE_OUT , btnPwUpHandler);
					    facade.sendNotification(PreventWallowEvent.STARTPWTIMER);		//开始防沉迷计时
					    sendNotification( ReNameData.HIDE_RENAME_BUTTON ); 
					    //5.24号，端午前，版署检查，5分钟弹出一次，弹出两次任然不通过，关闭游戏，弹到官网
				    }
				    if((notification.getBody() as String) == GameCommonData.wordDic[ "mod_pre_med_pre_han_1" ] )  // 成功
				    {
				    	facade.sendNotification(PreventWallowEvent.CLOSEPREVENTWALLOWBTN);
				    	facade.sendNotification(PreventWallowEvent.CLOSEPREVENTWALLOWVIEW);
				    	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_2" ], color:0xffff00});  // 您已经通过御剑江湖的防沉迷验证
			    		facade.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_pre_med_pre_han_3" ] , content:PreventWallowData.wallowsucceed});  // 防沉迷通知
			    		facade.sendNotification(PreventWallowEvent.OVERPWTIMER);		//关闭防沉迷计时
				    	return;
				    }
				    else if((notification.getBody() as String) == GameCommonData.wordDic[ "mod_pre_med_pre_han_4" ] )  // 失败
				    {
				    	// 您没有通过御剑江湖的防沉迷验证，请重新填写资料
				    	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_5" ], color:0xffff00});
				    	facade.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_pre_med_pre_han_3" ], content:PreventWallowData.wallowFail});  //防沉迷通知
				    	facade.sendNotification(PreventWallowEvent.CLOSEPREVENTWALLOWVIEW);
				    	return;
				    }
				    var time:int = int(notification.getBody());
				    if(time >= 3)
				    {
				    	btnPw.play();
				    	PreventWallowData.mcIsPlay = true;
				    	if(time >= 3 && time < 5)
				    	{
				    		facade.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_pre_med_pre_han_3" ] , content:PreventWallowData.threeHours});  //防沉迷通知
				    	}
				    	else if(time >= 5)
				    	{
				    		facade.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_pre_med_pre_han_3" ] , content:PreventWallowData.sixHours});  // 防沉迷通知
				    	}
				    }
				break;
				/** 去掉防沉迷按钮 */
				case PreventWallowEvent.CLOSEPREVENTWALLOWBTN:
					if(!GameCommonData.GameInstance.GameUI.contains(btnPw)) return;
					GameCommonData.GameInstance.GameUI.removeChild(btnPw); 
					btnPw.removeEventListener(MouseEvent.CLICK , btnPwClickHandler);
				    btnPw.removeEventListener(MouseEvent.MOUSE_DOWN , btnPwDownHandler);
				    btnPw.removeEventListener(MouseEvent.MOUSE_OVER , btnPwOverHandler);
				    btnPw.removeEventListener(MouseEvent.MOUSE_UP , btnPwUpHandler);
				    btnPw.removeEventListener(MouseEvent.MOUSE_OUT , btnPwUpHandler); 
				    sendNotification( ReNameData.SHOW_RENAME_BUTTON );
				break;
				/** 显示防沉迷资料面板 */
				case PreventWallowEvent.SHOWPREVENTWALLOWVIEW:
					loadView();
				break;
				/** 关闭防沉迷资料面板 */
				case PreventWallowEvent.CLOSEPREVENTWALLOWVIEW:
					gcAll();
				break;
				/** 开始防沉迷计时 */
				case PreventWallowEvent.STARTPWTIMER:
					if(GameCommonData.preventWallowTime == 0) return;
					if(PreventWallowData.PwTimerSingle == false)
					{
						timer.DistanceTime = GameCommonData.preventWallowTime;											//循环时间
						PreventWallowData.PwTimerSingle = true;
						GameCommonData.GameInstance.GameUI.Elements.Add(this);			//第一次才会添加心跳
					}
				break;
				/** 删除防沉迷计时 */
				case PreventWallowEvent.OVERPWTIMER:
					if(PreventWallowData.PwTimerSingle == true)
					{
						PreventWallowData.PwTimerSingle = false;
						GameCommonData.GameInstance.GameUI.Elements.Remove(this);			//第一次才会添加心跳	
					}
				break;
				case PreventWallowEvent.CHECT_IS_OPEN_FCM:		//检查防沉迷是否启动
					if(GameCommonData.wordVersion == 1)  //国服
					{
						checkIsFcmOpen();
					}
				break;
			}
		}
		
		private function checkIsFcmOpen():void
		{
			if(GameCommonData.isNew >= 100)	//新的平台
			{
				if(GameCommonData.fcmPower == 1) //平台防沉迷开启
				{
					if(GameCommonData.fcmConfig == 0) //用户没通过
					{
						sendNotification(PreventWallowEvent.SHOWPREVENTWALLOWBTN);
						if(GameCommonData.isNew > 100) //大于100，进入游戏立刻弹出
						{
							if(PreventWallowData.PreventWallowIsOpen == true) return;
							facade.sendNotification(PreventWallowEvent.SHOWPREVENTWALLOWVIEW);
						}
					} 
				}
			}
		}
		
	   /*  private var tf:TextField;
		private  var tag:int;
		private function checkTF(str:String = null):void
		{
			 if(!tf)
			{
				tf = new TextField();
				tf.y = 300;
				tf.height = 100;
				tf.wordWrap = true;
				tf.width = 500;
				tf.background = true;
				tf.backgroundColor = 0xaabb11;
			}
			var shwst:String =  "fcmPower:"+GameCommonData.fcmPower  +"  tag:"+tag+ "  fcmConfig:"+ GameCommonData.fcmConfig + "  isNew: "+GameCommonData.isNew;
			tf.text = tf.text +"/n   " + shwst + str;
			GameCommonData.GameInstance.stage.addChild(tf); 
		}  */ 
		/** 打开界面 */
		private function sendShow(mc:DisplayObject):void
		{
			if(PreventWallowData.PreventWallowIsOpen == false)
			{
				PreventWallowData.PreventWallowIsOpen = true;
				preventWallowView = mc as MovieClip;
				initView();
				addLis();
				GameCommonData.GameInstance.GameUI.addChild(panelBase); 
				preventWallowView.stage.focus = preventWallowView.txtName;
			}
			else
			{
				gcAll();
			}
		}
		private function loadView():void
		{
			loadswfTool = new LoadSwfTool(GameConfigData.PreventWallow , this);
			loadswfTool.sendShow = sendShow;
		}
		private function initView():void
		{
			panelBase = new PanelBase(preventWallowView, preventWallowView.width - 30, preventWallowView.height + 14);
			panelBase.IsDrag = false;	
			panelBase.name = "preventWallowView";
			panelBase.x = UIConstData.DefaultPos2.x - 330;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pre_med_pre_ini_1" ] );  // 实名验证及防沉迷
			txtInit();
		}
		private function addLis():void
		{
			/* (preventWallowView.txtName as TextField).maxChars = 30;
			(preventWallowView.txtName as TextField).width = 80; */
			
			preventWallowView.btnSubmit.addEventListener(MouseEvent.CLICK , submitHandler);
			preventWallowView.btnClose.addEventListener(MouseEvent.CLICK , closeHandler);
			panelBase.addEventListener(Event.CLOSE , panelCloseHandler);
			preventWallowView.txtName.addEventListener(FocusEvent.FOCUS_IN , contentFocusIn);
			preventWallowView.txtId.addEventListener(FocusEvent.FOCUS_OUT , contentFocusOut);
		}
		private function gcAll():void
		{
			if(!preventWallowView) return;
			preventWallowView.stage.focus = null;
			UIUtils.removeFocusLis(preventWallowView.txtName);
			UIUtils.removeFocusLis(preventWallowView.txtId);
			PreventWallowData.PreventWallowIsOpen = false;
			preventWallowView.btnSubmit.removeEventListener(MouseEvent.CLICK , submitHandler);
			preventWallowView.btnClose.removeEventListener(MouseEvent.CLICK , closeHandler);
			panelBase.removeEventListener(Event.CLOSE , panelCloseHandler);
			preventWallowView.txtName.removeEventListener(FocusEvent.FOCUS_IN , contentFocusIn);
			preventWallowView.txtId.removeEventListener(FocusEvent.FOCUS_OUT , contentFocusOut);
//1			textmc.removeEventListener(MouseEvent.CLICK,textmcClickHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			loadswfTool = null;
			preventWallowView = null;
			if(isConnectBreak) return;
			if(showCount > GameCommonData.showAccount)	//5.24号做，弹出两次后任未通过防沉迷，将断开连接
			{
				isConnectBreak = true;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"您将断开连接，请填写正确的防沉迷信息，通过后可继续游戏！", color:0xffff00});
				var timerCount:int = setTimeout(breakLink,3000,timerCount);  
			}
			timer.DistanceTime = 2000;
			showCount += 2;	//弹出框次数增加  
		}
		
		private function breakLink(data:int):void
		{
			clearTimeout(data);
			GameCommonData.GameNets.endGameNet();
			navigateToURL(new URLRequest(ChatData.OFFICIAL_WEBSITE_ADDR), "_blank");
		}
		
		private function closeHandler(e:MouseEvent):void
		{
			gcAll();
		}
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 文本初始化 */
		private function txtInit():void
		{			
			UIUtils.addFocusLis(preventWallowView.txtName);
			UIUtils.addFocusLis(preventWallowView.txtId);
			(preventWallowView.txtName as TextField).multiline = false;
			(preventWallowView.txtId as TextField).multiline = false;
			(preventWallowView.txtName as TextField).text = "";
			(preventWallowView.txtId as TextField).text = "";
			(preventWallowView.txtName as TextField).maxChars = 5;
			(preventWallowView.txtId as TextField).maxChars = 18;
			(preventWallowView.txtId as TextField).restrict = "[a-zA-Z0-9]";
//			(preventWallowView.txtName as TextField).restrict = "[\u4e00-\u9fa5]";
		}
		/** 提交信息 */
		private function submitHandler(e:MouseEvent):void
		{
			if(checkUp())		//客户端通过检查
			{
//				sendAction();
				if(pTimer.running)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_sub_1" ], color:0xffff00});  // 每次操作需要10秒的间隔
					return;
				}
				pTimer.reset();
				pTimer.start();
				sendPhpAction();
			}
		}
		/** 客户端检查信息 */
		private function checkUp():Boolean
		{
			var id:String = (preventWallowView.txtId as TextField).text;
			if((preventWallowView.txtName as TextField).length < 2)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_che_1" ], color:0xffff00});  // 您填写的姓名不合法
				return false;
			}
			if(id.length == 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_che_2" ], color:0xffff00});  // 请填写您的身份证号码
				return false;
			}
			if(id.length != 15 && id.length != 18)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_che_3" ], color:0xffff00});  // 您的身份证号码填写错误
				return false;
			}
			if(PreventWallowData.isPosting == true)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_che_4" ], color:0xffff00});  // 您的资料正在验证中
				return false;
			}
			/* if(WallowUI.checkId(id) == false)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你输入的身份证号不存在", color:0xffff00});
				return false;
			}
			if(WallowUI.checkAge(id) == false)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你填写的资料不符合国家认证的防沉迷标准", color:0xffff00});
				return false;
			} */
			return true;
		}
		/** 向页面发送请求 */
		/* private function sendPhpAction():void
		{
			baseHttp = new BaseHttp(ChatData.FAT_TEST_URL);
			baseHttp.RequestComplete = requestComplete;
			var account:String = GameCommonData.Accmoute;																//账号
			var trueName:String = (preventWallowView.txtName as TextField).text;										//用户真名
			var card:String = (preventWallowView.txtId as TextField).text;	
			if(ChatData.SERVICE_BUSINESS_ID < 4)	// 4399		91wan	37wan	5awan
			{
				baseHttp.AddUrlVariables("account"	, encodeURIComponent(account));//encodeURIComponent(account));			//转编码
				baseHttp.AddUrlVariables("truename" , encodeURIComponent(trueName));
			}		
			else{	//快wan
				baseHttp.AddUrlVariables("account"	, account);//encodeURIComponent(account));			//转编码
				baseHttp.AddUrlVariables("truename" , trueName);
			}									//身份证号
			baseHttp.AddUrlVariables("card" 	, card);
//			baseHttp.AddUrlVariables(MD5.hash(
			
			//转MD5
			var Md5Http:BaseHttp = new BaseHttp("http://yjjh1-bak.my4399.com/interface/md5.php");
			Md5Http.RequestComplete = requestMD5Complete;
			Md5Http.AddUrlVariables("md5" , trueName + account + ChatData.FAT_CODE + card);
			Md5Http.Submit();
			
		} */
		
		/** 向页面发送请求 */
		private function sendPhpAction():void
		{
			if(ChatData.SERVICE_BUSINESS_ID == 3)//5awan 
			{
				baseHttp = new BaseHttp(ChatData.FAT_TEST_URL,"GET"); 
			}
			else
			{
				baseHttp = new BaseHttp(ChatData.FAT_TEST_URL); 
			}
			baseHttp.RequestComplete = requestComplete;
			var account:String = GameCommonData.Accmoute;		
			var trueName:String = (preventWallowView.txtName as TextField).text;										//用户真名
			var card:String = (preventWallowView.txtId as TextField).text;												//身份证号
			
			if(ChatData.SERVICE_BUSINESS_ID <3)		//4399  91wan   37wan   
			{
				baseHttp.AddUrlVariables("account"	, encodeURIComponent(account));//encodeURIComponent(account));			//转编码
				baseHttp.AddUrlVariables("truename" , encodeURIComponent(trueName));
				
			}
			else if(ChatData.SERVICE_BUSINESS_ID == 3)//5awan 
			{ 
				baseHttp.AddUrlVariables("account"	, account);//encodeURIComponent(account));			//转编码
				baseHttp.AddUrlVariables("truename" , trueName);
				baseHttp.AddUrlVariables("action" , "Fcm");
				baseHttp.AddUrlVariables("method" , "yjjh"); 
			}
			else if(ChatData.SERVICE_BUSINESS_ID > 3)//kuaiwan
			{	
				
				baseHttp.AddUrlVariables("account"	, account);//encodeURIComponent(account));			//转编码
				baseHttp.AddUrlVariables("truename" , trueName);
				if(ChatData.SERVICE_BUSINESS_ID == 10) //风行
				{
					var sid:String = GameCommonData.ServerId;
					if(!sid)
					{
						sid = "S1";
					}
					if(sid.substr(0,1).toLowerCase() == "s")
					{
						sid = sid.substr(1);
					}
					baseHttp.AddUrlVariables("serverid" , sid);
				}
			}
			
			baseHttp.AddUrlVariables("card", card);
			
			//转MD5
			 
			/*  var Md5Http:BaseHttp
			if(GameCommonData.isNew == 0)
			{
				if(GameCommonData.fcmPower == 111）
				{
					Md5Http = new BaseHttp("http://yjjh1-bak.my4399.com/interface/md5.php");
					Md5Http.RequestComplete = requestMD5Complete;
					Md5Http.AddUrlVariables("md5" , trueName + account + ChatData.FAT_CODE + card); 
				} 
			} */ 
//			if(GameCommonData.showAccount == 1111)
//			{
				requestComplete("1");
				if(GameCommonData.isNew == 100)
				{
					sendToFcmJS();
				}
				fcmAccAcross()
//			}
//			else
//			{
//				Md5Http.Submit();
//			}

		}
		
		/** 得到请求MD5 */
		private function requestMD5Complete(sign:String):void
		{
			if(ChatData.SERVICE_BUSINESS_ID < 5)	//5 360 6 奥盛
			{
				baseHttp.AddUrlVariables("sign" , sign);
			}
			baseHttp.Submit();
			PreventWallowData.isPosting = true;			//提交数据中
			intervalId = setTimeout(timeOut , 1000 * 40);		//40秒以后操作超时
		}
		/** 提交超时，请再次操作 */
		private function timeOut():void
		{
			PreventWallowData.isPosting = false;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_tim_1" ], color:0xffff00});  // 操作超时，请重新提交
		}
		/** 提交表单后返回的数据*/
		private function requestComplete(data:String):void
		{            
			switch(data)
			{
				case "1":			//成功登记并且年龄超过18岁
					sendAction();			//4399验证通过,通知服务器
				break;
				case "2":			//成功登记但年龄没有超过18岁
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_5" ], color:0xffff00});  // 您没有通过御剑江湖的防沉迷验证，请重新填写资料
				break;
				case "-1":			//参数不全
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_5" ], color:0xffff00});  //同上
				break;
				case "-2":			//验证失败
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_5" ], color:0xffff00});  //同上
				break;
				case "-3":			//身份证号码无效
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_5" ], color:0xffff00});  //同上
				break;
				case "-4":			//不允许重复登记
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_req_1" ], color:0xffff00});   // 不允许重复验证
				break;
				case "-5":			//登记失败
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_req_2" ], color:0xffff00});  // 验证失败
				break;
			}
			PreventWallowData.isPosting = false;
			clearInterval(intervalId);    		//清除操作超时的计时器
		}
		/**
		 *调用页面js方法，修改数据库，是玩家通过防沉迷 
		 * 
		 */		
		private function sendToFcmJS():void
		{
			var account:String = GameCommonData.Accmoute;
			try
			{
				ExternalInterface.call("fcmfuck",account);
			}
			catch(e:Error)
			{
//				trace(e.message,"fcm occur error!",account);
			}
		}
		/**关闭防沉迷*/
		private function fcmAccAcross():void
		{
			isSendSucced = true;
			facade.sendNotification(PreventWallowEvent.CLOSEPREVENTWALLOWBTN);
	    	facade.sendNotification(PreventWallowEvent.CLOSEPREVENTWALLOWVIEW);
	    	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_han_2" ], color:0xffff00});  // 您已经通过御剑江湖的防沉迷验证
    		facade.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_pre_med_pre_han_3" ] , content:PreventWallowData.wallowsucceed});  // 防沉迷通知
    		facade.sendNotification(PreventWallowEvent.OVERPWTIMER);		//关闭防沉迷计时
    		
		}
		/** 发送请求 */
		private function sendAction():void
		{
			var obj:Object = new Object();
			var parm:Array = new Array();
			
			obj.type = Protocol.PLAYER_CHAT;
			obj.data = parm;
			
			parm.push(GameCommonData.Player.Role.Name);		//玩家名
			parm.push("");		//玩家真实名字
			parm.push("");
			parm.push("");		//身份证号
			parm.push("");
			parm.push("");		//摊位ID
			parm.push(2038);	//action
			parm.push(1);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			Chat.SendChat(obj);
		}
		/** 点击防沉迷按钮 */
		private function btnPwClickHandler(e:MouseEvent):void
		{
			facade.sendNotification(PreventWallowEvent.SHOWPREVENTWALLOWVIEW);
		}
		/** 防沉迷按钮按下*/
		private function btnPwDownHandler(e:MouseEvent):void
		{
			btnPw.gotoAndStop("down");
		}
		/** 经过防沉迷按钮*/
		private function btnPwOverHandler(e:MouseEvent):void
		{
			btnPw.gotoAndStop("over");
		}
		/** 防沉迷按钮弹起*/
		private function btnPwUpHandler(e:MouseEvent):void
		{
			btnPw.gotoAndStop("up");
			if(PreventWallowData.mcIsPlay)
			{
				btnPw.play();
			}
		}
		private function contentFocusIn(e:FocusEvent):void
		{
			UIConstData.FocusIsUsing = true;
		}
		private function contentFocusOut(e:FocusEvent):void
		{
			UIConstData.FocusIsUsing = false;
		}
		
		private function textmcClickHandler(e:MouseEvent):void
		{
			if((preventWallowView.txtName as TextField).text !== "")
			{
				var preventWallowID:PreventWallowID = new PreventWallowID();
					allName = preventWallowID.allName;
					_ID = preventWallowID.id;
			}
				exampleText.text = "姓名："+allName+"  身份证："+_ID;
				(preventWallowView.txtName as TextField).text = allName;
				(preventWallowView.txtId as TextField).text = _ID;
		}
	}
}