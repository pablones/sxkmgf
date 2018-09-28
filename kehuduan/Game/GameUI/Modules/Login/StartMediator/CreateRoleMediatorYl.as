package GameUI.Modules.Login.StartMediator
{
	import Controller.AudioController;
	
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.Modules.GmTools.Utils.LoadSwfTool;
	import GameUI.Modules.Login.Data.CreateRoleConstData;
	import GameUI.Modules.Login.Data.CreateRoleEvent;
	import GameUI.Modules.Login.Jobchange.Jobchange;
	import GameUI.Modules.Login.SoundUntil.SoundController;
	import GameUI.Modules.Login.UITool.LoadManPng;
	import GameUI.Modules.Login.UITool.LoadUI;
	import GameUI.Modules.Login.UITool.RoleMegController;
	import GameUI.Modules.Login.UITool.RoleMegMemento;
	import GameUI.UIUtils;
	
	import Net.ActionSend.CreateRoleSend;
	
	import Vo.RoleVo;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.media.Sound;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CreateRoleMediatorYl extends Mediator
	{
		public static const NAME:String = "createRoleMediatorYl";
		private var playSound:Boolean = false;
		private var issee:Boolean = true;
		private var createRole:MovieClip;
		private var alertView:MovieClip;
		private var soundController:SoundController;
		private var loadSwfTool:LoadSwfTool;		/** 加载角色预览工具 */
		private var dictionary:Dictionary = new Dictionary();	/** 存有人物图像的哈希表 */
		private var rolePcNameObj:Object = new Object();	/** 人物图像哈希表的索引*/
		private var messageController:RoleMegController;	/** 角色消息控制器 */
		private const MESSAGEPOINT:Point = new Point(630, 395);/** 广播消息的位置 */
		public function CreateRoleMediatorYl()
		{
			super(NAME);
		}
		
//		private function get createRole():MovieClip
//		{
//			return viewComponent as MovieClip
//		}
		
		public override function onRegister():void
		{
//			trace("has Register");
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//			    EventList.INITVIEW,
				EventList.SHOWCREATEROLE,
				CommandList.CREATEOVER,
				EventList.REMOVECREATEROLE,
				CreateRoleEvent.ADDMESSAGE
			       ]
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				
				case EventList.SHOWCREATEROLE:  
				  createRole = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip("CreateRole") as MovieClip;                                                                                         
				  show();                                                                                                   //显示创建角色画面
				  soundController = new SoundController(GameConfigData.UILibraryRole , new Point(createRole.width - 40,5));
//				  if(GameCommonData.isOpenSoundSwitch == false)
//				  {
//		  			  GameCommonData.isOpenFightSoundSwitch = GameCommonData.isOpenSoundSwitch;
//				  	  soundController.soundOnOrOff(GameCommonData.isOpenSoundSwitch);
//				  }
				  addlis();                                                                                                 //增添事件
				  homeMcInit();
				break;
				
				case CommandList.CREATEOVER:  
				  var data:Object = notification.getBody();
				  var str:String = data.talkObj[3];
				  if(str == "ANSWER_OK")                                                                                      //如果创建成功，则加载角色数据
				  {
				  	createover();
				  }
				  else if(str == GameCommonData.wordDic[ "mod_log_sta_cre_han_1" ])//"角色删除成功"
				  {
				  	facade.sendNotification(SelectRoleMediatorYL.DELETEROLE);
				  }
				  else if(str == GameCommonData.wordDic[ "mod_log_sta_cre_han_2" ])//"角色删除失败"
				  {
				  	showaAlertView(GameCommonData.wordDic[ "mod_log_sta_cre_han_3" ]);//"删除角色失败"
				  }
				  else                                                                                                         //创建人物名的错误
				  {
				  	str = str.substring(3 , str.length);
				  	showaAlertView( str);
				  	CreateRoleConstData.playerobj.homeindex = 6;			//创建失败 默认为新手
				  }
				  break;
				  
				case EventList.REMOVECREATEROLE:
				  gc();
				break;
				case CreateRoleEvent.ADDMESSAGE:
					if(createRole != null && GameCommonData.GameInstance.GameUI.contains(createRole))
					{
						var messageObj:Object = notification.getBody();
						var messageItem:RoleMegMemento = new RoleMegMemento(messageObj.sex , messageObj.name);
						messageController.add(messageItem);
						messageObj = null;
					}
				break;
			}
		}
		
		/**  显示创建角色画面  */
		private function show():void        
		{
			GameCommonData.GameInstance.GameUI.addChildAt(createRole , 1);                                                         	//将MC添加到游戏画面中
			createRole.mcjobps.stop();
			createRole.txtname.maxChars = 7;																					//名字最多7个字符
//			(createRole.txtname as TextField).restrict = "/[^\s]/g";
			createRole.btnCreate.visible   = false;                                                                           	//创建按钮可用
			createRole.mcLightMove.visible   = false;
			createRole.mcLightMove.gotoAndStop(1);
			createRole.mcHightLight.stop();
			createRole.mcHightLight.mouseEnabled = false;
			createRole.mcLightMove.mouseEnabled = false;
			createRole.mcjobps.visible = true;                                                                               	//门派介绍一开始不显示
			lightInit();
			showRoleInit(0,2);                                                                                               	//显示初始角色
			createRole.stage.focus = createRole.txtname;																		//设置光标
			showaAlertView(GameCommonData.wordDic[ "mod_log_sta_cre_show" ]);//"请输入角色名"
			messageController = new RoleMegController();																		//创建角色广播容器
			createRole.addChild(messageController);
			messageController.x = MESSAGEPOINT.x;
			messageController.y = MESSAGEPOINT.y;
		}
		
		private function addlis():void
		{
			for(var i:int;i < 6;i++)
			{
				createRole["home_"+i].addEventListener(MouseEvent.CLICK,homeHandler);                                        //点击门派后调用数据
				createRole["home_"+i].addEventListener(MouseEvent.MOUSE_OVER,homeOverHandler);                                //点击门派后调用数据
				createRole["home_"+i].addEventListener(MouseEvent.MOUSE_OUT,homeOutHandler);
			}
			createRole.btnMan.addEventListener(MouseEvent.CLICK,manHandler);                                                  //点击男性别后调用数据 
			createRole.btnWomen.addEventListener(MouseEvent.CLICK,womanHandler);                                              //点击女性别后调用数据 
			createRole.btnCreate.addEventListener(MouseEvent.CLICK,createdHandler);                                           //人物创建
//			createRole.btnseedown.addEventListener(MouseEvent.CLICK,seedownHandler);                                         //取消预览按钮按下
			createRole.txtname.addEventListener(Event.CHANGE,textInputHandler);											//监听输入角色名时间
//			createRole.txtname.addEventListener(FocusEvent.FOCUS_IN,textInputHandler);											//一获取焦点就出提示
			createRole.txtname.addEventListener(FocusEvent.FOCUS_OUT,focusOutHandler);											//失去焦点
			createRole.btnCreate.addEventListener(MouseEvent.MOUSE_OVER , createOverHandler);
			createRole.btnCreate.addEventListener(MouseEvent.MOUSE_OUT , createOutHandler);
//			for(var n:int = 1;n < 10;n++)
//			{
//				createRole["mclook_"+n].addEventListener(MouseEvent.CLICK,mclookHandler);                                    //头像点击传送数据,加框
//				createRole["mclook_"+n].addEventListener(MouseEvent.MOUSE_OVER,lookoverHandler);                            //鼠标经过头像显示红框
//				createRole["mclook_"+n].addEventListener(MouseEvent.MOUSE_OUT,lookoutHandler);                              //鼠标移过后，红框消失
//			}
		}
		
		private function gc():void
		{
			createRole.txtname.removeEventListener(FocusEvent.FOCUS_OUT,focusOutHandler);											//失去焦点
			createRole.stage.focus = null;
			GameCommonData.GameInstance.GameUI.removeChild(createRole);
			for(var i:int;i < 6;i++)
			{
				createRole["home_"+i].removeEventListener(MouseEvent.CLICK,homeHandler);  
				createRole["home_"+i].removeEventListener(MouseEvent.MOUSE_OVER,homeOverHandler);  
				createRole["home_"+i].removeEventListener(MouseEvent.MOUSE_OUT,homeOutHandler);
			    createRole["home_"+i].visible = true;                                                                       //按钮弹起
			    
			}
			soundController.clearSoundSwitch();
			createRole.btnMan.visible = true;
			createRole.btnWomen.visible = true;
			createRole.btnMan.removeEventListener(MouseEvent.CLICK,manHandler);                                             //点击男性别后调用数据 
			createRole.btnWomen.removeEventListener(MouseEvent.CLICK,womanHandler);                                         //点击女性别后调用数据 
			createRole.btnCreate.removeEventListener(MouseEvent.CLICK,createdHandler);                                      //人物创建  
//			createRole.btnseedown.removeEventListener(MouseEvent.CLICK,seedownHandler);                                     //取消预览按钮按下
			createRole.txtname.removeEventListener(Event.CHANGE,textInputHandler);											//监听输入角色名时间
//			createRole.txtname.removeEventListener(FocusEvent.FOCUS_IN,textInputHandler);											//一获取焦点就出提示
			issee = false;
			AudioController.SoundHomeOff();																					//清除声音
		}
		
		/** 显示每个角色的数据*/
		private function showRoleInit(home:int,sex:int):void
		{
			createRole.txtname.text = "";                                                                                    //人物名称 
			createRole.mcBigMan.visible = false;
			createRole.mcBigWoman.visible = true;
			//游戏ID已经在GameServerInfo类里给出数据了
			CreateRoleConstData.playerobj.sexindex = 2;                                                                     //默认为女
			createRole.btnWomen.visible = false;                                                                            //性别女的按钮按下
			CreateRoleConstData.playerobj.homeindex = 4;                                                                    //默认门派为点苍
			CreateRoleConstData.playerobj.look      = Math.floor(Math.random() * 8) + 10;  									//默认头像为随即女
			var insd:Number = CreateRoleConstData.playerobj.look;
			showman(4,2,false)                                                                                              //默认为点苍女
//			showlook(2);                                                                                                    //默认头像为女
//			SetFrame.UseFrame(createRole["mclook_"+String(CreateRoleConstData.playerobj.look -9)],"YellowFrame")  
			createRole["light_" + 4].visible = true;																		//门派默认选中点苍
			createRole.mcjobps.gotoAndStop(("frame_" + 4));																	//门派介绍默认为点苍
//			createRole.btnseedown.visible = true;																			//取消预览默认弹起
			
		}
		
		/**点击创建按钮*/
		private function createdHandler(e:MouseEvent):void
		{
			var array:Array = new Array();
			 
			
			e.target.visible = false;   
			createRole.mcLightMove.visible   = false;
			createRole.mcLightMove.gotoAndStop(1);                                                                                   //点击按钮后不能进行创建,按钮变灰  
//			createRole.btnCreate.removeEventListener(MouseEvent.CLICK,createdHandler);                                     //移除创建按钮的侦听事件     
			
			CreateRoleConstData.playerobj.name =  createRole.txtname.text;                                                 //人物名称 
			
		    if(CreateRoleConstData.playerobj.name == "")
			{
			} 
			else if(CreateRoleConstData.playerobj.look == null)																//默认头像为空
			{
			}
			else if((createRole.txtname as TextField).length <2)																//角色名小于2个字
			{
			}
			else if(CreateRoleConstData.playerobj.name.indexOf(GameCommonData.wordDic[ "mod_log_sta_cre_cre" ]) > -1)//"御剑江湖"
			{
			}  
			else if(UIUtils.isPermitedRoleName(createRole.txtname.text) == false || UIUtils.legalRoleName(CreateRoleConstData.playerobj.name) == false)				//有不合格字符
			{
			}
			else
			{
				CreateRoleConstData.playerobj.homeindex = Jobchange.homechange(CreateRoleConstData.playerobj.homeindex);                                               //转换帮派协议数值
				array.push(CreateRoleConstData.playerobj.name);                                                                                            //人物名称
				array.push("123");                                                                                             //保护密码
				array.push(CreateRoleConstData.playerobj.id);                                                                                      //游戏ID
				array.push(CreateRoleConstData.playerobj.look);                                                                                    //人物图像
				array.push(CreateRoleConstData.playerobj.sexindex-1);                                                                              //传给服务器的性别需要-1
				CreateRoleSend.createMsgReg(array);                                                                                  //发送请求.返回是否创建成功
			}
		}
		
		/**点击返回按钮*/
		private function backHandler(e:MouseEvent):void
		{
			facade.sendNotification(EventList.REMOVECREATEROLE);
		}
		/** 鼠标经过门派事件 */
		private function homeOverHandler(e:MouseEvent):void
		{
			var index:int = e.target.name.split("_")[1];
	        createRole["light_" + index].visible = true;
		}
		/** 鼠标移出门派事件 */
		private function homeOutHandler(e:MouseEvent):void
		{
			var index:int = e.target.name.split("_")[1];
			if(index == CreateRoleConstData.playerobj.homeindex) return;
			 createRole["light_" + index].visible = false;
		}
		/**点击门派事件*/
		private function homeHandler(e:MouseEvent):void
		{
			if(CreateRoleConstData.roleLoading) return;						//如果正在加载人物图像的话，则不准点预览
			createRole.mcjobps.visible = true;                                                                              //门派介绍显示
			CreateRoleConstData.playerobj.homeindex = e.target.name.split("_")[1];                                                              //取得MC名字中的序列号
			for(var i:int = 0; i < 6; i++) {
				if(i == CreateRoleConstData.playerobj.homeindex)
				{
					createRole["home_"+i].visible = false
//					createRole["mcHomeCopy_" + i].visible = true;
				} 
				else
				{
					createRole["home_"+i].visible = true;
//					createRole["mcHomeCopy_" + i].visible = false;
				} 
			}
//			showman(CreateRoleConstData.playerobj.homeindex , CreateRoleConstData.playerobj.sexindex);                                                              //选中效果
		///////////////点击门派弹出预览效果	
			if(issee == false)
			{
				issee = true;
//				createRole.btnseedown.visible = true;
			}
			showman(CreateRoleConstData.playerobj.homeindex,CreateRoleConstData.playerobj.sexindex);
	        //点击门派出现高光
	        lightInit();																								//选中高光初始化
	        createRole["light_" + CreateRoleConstData.playerobj.homeindex].visible = true;
		}
		
		/**点击男性别事件*/
		
		private function manHandler(e:MouseEvent):void
		{
			if(CreateRoleConstData.roleLoading) return;						//如果正在加载人物图像的话，则不准点预览
			//var i:int = playerobj.look-9;                                                                                 //男头像对应mc的index
			CreateRoleConstData.playerobj.sexindex = 1;
			e.target.visible = false;
			createRole.btnWomen.visible = true;
			createRole.mcBigMan.visible = true;
			createRole.mcBigWoman.visible = false;
			if(CreateRoleConstData.playerobj.homeindex == 6) {																//6是新手
				showman(CreateRoleConstData.playerobj.homeindex,CreateRoleConstData.playerobj.sexindex, false);  
			} else {
				showman(CreateRoleConstData.playerobj.homeindex,CreateRoleConstData.playerobj.sexindex);  
			}
			showlook(CreateRoleConstData.playerobj.sexindex);                                                                                   //显示头像
//			CreateRoleConstData.playerobj.look = null;
			//////////////////改变性别，随即头像
			CreateRoleConstData.playerobj.look      = Math.floor(Math.random() * 8);  						                               //头像序号是随即0-8
//			 SetFrame.UseFrame(createRole["mclook_"+1],"YellowFrame")                                          									 //再添加黄色框
		}
		
	    /**点击女性别事件*/
		private function womanHandler(e:MouseEvent):void
		{
			if(CreateRoleConstData.roleLoading) return;						//如果正在加载人物图像的话，则不准点预览
			CreateRoleConstData.playerobj.sexindex = 2;
			e.target.visible = false;
			createRole.btnMan.visible = true;
			createRole.mcBigMan.visible = false;
			createRole.mcBigWoman.visible = true;
			if(CreateRoleConstData.playerobj.homeindex == 6) {
				showman(CreateRoleConstData.playerobj.homeindex,CreateRoleConstData.playerobj.sexindex, false);  
			} else {
				showman(CreateRoleConstData.playerobj.homeindex,CreateRoleConstData.playerobj.sexindex);                                                                //显示人物
			}
			showlook(CreateRoleConstData.playerobj.sexindex);                                                                                   //显示头像
//			CreateRoleConstData.playerobj.look = null;
			//////////////////改变性别，不默认头像
			CreateRoleConstData.playerobj.look      = Math.floor(Math.random() * 8) + 10;  														//头像为随即女                                                                                            //头像序号是1
//		    SetFrame.UseFrame(createRole["mclook_"+1],"YellowFrame")                                          									 //再添加黄色框
		}
		/** 输入角色名事件 */
		private function textInputHandler(e:Event):void
		{
			createRole.btnCreate.visible = true; 
			createRole.mcLightMove.visible  = true;
			createRole.mcLightMove.play();
			var nameTxt:TextField = e.currentTarget as TextField;
			if(alertView && GameCommonData.GameInstance.GameUI.contains(alertView)) GameCommonData.GameInstance.GameUI.removeChild(alertView);
			if(nameTxt.length == 0)
			{
				showaAlertView(GameCommonData.wordDic[ "mod_log_sta_cre_show" ]);//"请输入角色名"
				createRole.btnCreate.visible = false; 
				createRole.mcLightMove.visible   = false;
				createRole.mcLightMove.gotoAndStop(1);
			}
			else if(nameTxt.length < 2 || nameTxt.length > 7)
			{
				showaAlertView(GameCommonData.wordDic[ "mod_log_sta_cre_tex_1" ]);//"用户名必须是2~7字符"
				createRole.btnCreate.visible = false; 
				createRole.mcLightMove.visible   = false;
				createRole.mcLightMove.gotoAndStop(1);
			}
			else if(nameTxt.text.indexOf(GameCommonData.wordDic[ "mod_log_sta_cre_cre" ]) > -1)//"御剑江湖"
			{
				showaAlertView(GameCommonData.wordDic[ "mod_log_sta_cre_tex_2" ]);//"角色姓名不合法"
				createRole.btnCreate.visible = false; 
				createRole.mcLightMove.visible   = false;
				createRole.mcLightMove.gotoAndStop(1);
			}  
			else if(UIUtils.isPermitedRoleName(nameTxt.text) == false || UIUtils.legalRoleName(nameTxt.text) == false)				//有不合格字符
			{
				showaAlertView(GameCommonData.wordDic[ "mod_log_sta_cre_tex_2" ]);//"角色姓名不合法"
				createRole.btnCreate.visible = false; 
				createRole.mcLightMove.visible   = false;
				createRole.mcLightMove.gotoAndStop(1);
			}
			
		}
		/** 失去焦点 */
		private function focusOutHandler(e:FocusEvent):void
		{
			createRole.stage.focus = createRole.txtname;
//			textInputHandler(e);
		}
		
		/**预览按钮弹起来*/
		private function seedownHandler(e:MouseEvent):void
		{
			if(issee == true)
			{
				issee = false;
				e.target.visible = true;
			}
			//////////////门派按钮全部弹起
			for(var i:int ; i < 6 ; i++)
			{
				createRole["home_"+i].visible = true;                                                                        //门派的按钮弹起
//				homeMcInit();
				lightInit();																								//选中高光初始化
			} 
			e.target.visible = false;
			AudioController.SoundHomeOff();																					//预览按钮弹起，声音关闭
			createRole.mcjobps.gotoAndStop(1);																				//门派介绍为空
			CreateRoleConstData.playerobj.homeindex = 6;
			showman(CreateRoleConstData.playerobj.homeindex,CreateRoleConstData.playerobj.sexindex); 
		    ///////////////
		}
		/***/
		/**显示角色人物*/
		private function showman(homeindex:int ,sexindex:int, playSound:Boolean=true):void
		{
			var frame:String = "frmRole_" + homeindex * 10 + sexindex;
			this.playSound = playSound;
			if(issee == true)		//可以预览
			{
				var pathName:String = Jobchange.homechange(homeindex) + "_" +  int(sexindex-1) + ".swf";
				var loadInfo:LoadManPng = new LoadManPng("Resources/GameDLC/RoleImages/" + Jobchange.homechange(homeindex)+ "_2.swf" , this);		//门派简介
				var loadSwfTool:LoadManPng = new LoadManPng("Resources/GameDLC/RoleImages/"+ pathName, this);
				rolePcNameObj.type = Jobchange.homechange(homeindex) + "_" +  int(sexindex-1);
				loadInfo.sendShow = manShow;
				loadInfo.load();		//加载门派简介
				loadSwfTool.sendShow = manShow;
				loadSwfTool.load();		//加载人物
			}
			else manClear();
		} 
		/**显示人物头像*/
		private function showlook(index:int):void
		{
//			if(index == 2)                                          //如果选择的是女
//			{
//				for(var i:int = 1;i<10;i++)
//			    {
//				createRole["mclook_"+i].gotoAndStop(10+i);
//			    }
//			}
//			else if(index == 1)                                    //如果选择的是男
//			{
//				for(var n:int = 1;n<10;n++)
//			    {
//				createRole["mclook_"+n].gotoAndStop(n);
//			    }
//			}
			
		}
		
		/**点击头像后传送数据*/
		private function mclookHandler(e:MouseEvent):void
		{
//			trace(e.target.name);
//			SetFrame.UseFrame(e.target as DisplayObject);                            //再添加黄框
			//如果性别为女,头像的序列号为10——18
			if(CreateRoleConstData.playerobj.sexindex == 2)
			{
				CreateRoleConstData.playerobj.look = int(e.target.name.split("_")[1]) + 9; 
			}
			//如果性别为男,头像的序列号为1——9
			else if(CreateRoleConstData.playerobj.sexindex == 1)
			{
				CreateRoleConstData.playerobj.look = int(e.target.name.split("_")[1]);
			}
		}
		
		/**鼠标经过头像时显示红色框框*/
		private function lookoverHandler(e:MouseEvent):void
		{
			
			////////////有红框就不要黄框
//			if(CreateRoleConstData.playerobj.sexindex ==1 ||CreateRoleConstData.playerobj.sexindex ==2)                                                                               //如果为男或为女
//			{
//				if(e.target.name.split("_")[1] == CreateRoleConstData.playerobj.look ||e.target.name.split("_")[1] == (CreateRoleConstData.playerobj.look-9))                         //如果经过的mc等于头像的数值
//				{
//					SetFrame.RemoveFrame(createRole,"RedFrame");                                                                              //移除红色框框
//				}
//				else                                                                                                                          //如果没有经过已选中的头像
//				{
//					for(var i:int = 1 ; i<10 ; i++)
//			        {
//			            if(i == e.target.name.split("_")[1]) SetFrame.UseFrame(createRole["mclook_"+i],"RedFrame" , -2 , -1.4 , 54 );
//			         }
//				 }
//			}
		}
		
		
		/**将新创建的数据加载到角色数组里*/
		private function createover():void
		{
			var role:RoleVo = new RoleVo();
			role.UserId = CreateRoleConstData.playerobj.id;                              //游戏ID
			role.Level  = 1;                                         //新手等级为0
			role.Photo  = CreateRoleConstData.playerobj.look;                            //头像
			role.FirJob = 4096;    			                         //主职业
			role.FirLev = 1;                                    	 //主职业等级
			role.SecJob = 0;                                         //副职业
			role.SecLev = 1;                                         //副职业等级
			
			role.Coattype 	= 0;			                         //外装
			role.Wapon 		= 0; 			                         //武器
			role.Mount		= 0;                                     //坐骑
	        role.sexindex   = CreateRoleConstData.playerobj.sexindex -1;                 //第二职业改为性别 ,这里的性别都比服务器多1，发给服务器时要-1		
			role.SzName		= CreateRoleConstData.playerobj.name; 
			GameCommonData.RoleList.push(role);
			
			facade.sendNotification(EventList.REMOVECREATEROLE);
			
//			try
//			{
//				ExternalInterface.call( "cuser",role.UserId );
//			}
//			catch ( e:Error )
//			{
//				
//			}
//			trace("GameCommonData.RoleList.length",GameCommonData.RoleList.length)
			facade.sendNotification(CommandList.SELECTROLECOMMAND, 0);					//进入游戏
//			var gameInit:GameInit = new GameInit(GameCommonData.GameInstance);			//加载游戏资源
			
//			facade.sendNotification(EventList.REMOVECREATEROLE);                                                           //移除创建角色面板
// 			facade.sendNotification(EventList.SHOWSELECTROLE ,int(GameCommonData.RoleList.length)-1);                            //显示选择角色面板 , 默认选择刚创建的角色 
		}
		
		/** 高光初始化 */
		private function lightInit():void
		{
			for(var i:int = 0;i < 6;i++)
			{
				createRole["light_" + i].visible = false;
				createRole["light_" + i].mouseEnabled = false;
			}
		}
		/** 底层帮派图片mc初始化(点击后的效果图)*/
		private function homeMcInit():void
		{
//			for(var i:int;i < 6;i++)
//			{
//				createRole["mcHomeCopy_" + i].visible = false;
//			}
		}
		/** 不同门派性别出现不同的声音 */
		private function soundOn(homeindex:int , sexindex:int):void
		{
			var sound:Sound;
			var list:Array = CreateRoleConstData.roleSoundList;
			switch(sexindex)
			{
				//男
				case 1:
					switch(homeindex)
					{
						case null:
							AudioController.SoundHomeOff();																					//声音关闭
						break;
						case 0:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/16_0.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 1:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/8_0.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 2:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/4_0.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 3:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/2_0.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 4:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/32_0.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 5:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/1_0.swf"];
							AudioController.SoundHomeOn(sound);
						break;
					}
				break;
				//女
				case 2:
					switch(homeindex)
					{
						case 0:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/16_1.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 1:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/8_1.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 2:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/4_1.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 3:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/2_1.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 4:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/32_1.swf"];
							AudioController.SoundHomeOn(sound);
						break;
						case 5:
							AudioController.SoundHomeOff();
							sound = CreateRoleConstData.roleSoundList["Resources/GameDLC/RoleImages/1_1.swf"];
							AudioController.SoundHomeOn(sound);
						break;
					}
				break;
			}
			
		}
		private function showaAlertView(content:String ):void
		{
			if(GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole) == null) return;
			alertView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip("AlertView");
			GameCommonData.GameInstance.GameUI.addChild(alertView);
			alertView.txtInfo.text = content;
			alertView.x = createRole.txtname.x - 60;
			alertView.y = createRole.txtname.y - 65;
//			if(content == "请选择角色头像")
//			{
//				alertView.y = createRole.mclook_1.y + 10;
//			}
//			else
//			{
//				alertView.y = createRole.txtname.y +5;
//			}
		} 
		
//		private function comfrimHandler(e:MouseEvent):void
//		{
//			alertView.stage.removeEventListener(MouseEvent.CLICK , comfrimHandler );
//			if(alertView && GameCommonData.GameInstance.GameUI.contains(alertView))
//				GameCommonData.GameInstance.GameUI.removeChild(alertView);
//			
//			createRole.btnCreate.visible = true;                                                                                      //按钮可按  
//			
//			if(!createRole.btnCreate.hasEventListener(MouseEvent.CLICK))
//			createRole.btnCreate.addEventListener(MouseEvent.CLICK,createdHandler);                                     //添加创建按钮的侦听事件    
//		}
		private function manShow(man:MovieClip , info:MovieClip = null):void
		{
			manClear();
			createRole.addChildAt(man , alertView.numChildren);
			if(info) createRole.addChildAt(info , alertView.numChildren);
			dictionary[rolePcNameObj] = man;
			dictionary[rolePcNameObj+"1"] = info;
			var point:Point = LoadUI.getPoint(rolePcNameObj.type)
			man.x = point.x;
			man.y = point.y;
//			info.x = 20;
//			info.y = 50;
//			info.info.gotoAndPlay(1);
//			man.man.gotoAndPlay(1);
			
			var frame:String = "frmRole_" + CreateRoleConstData.playerobj.homeindex * 10 + CreateRoleConstData.playerobj.sexindex;
			if(frame != "frmRole_602" && frame != "frmRole_601")			//如果选择的不是新手，就播放门派介绍声音
			{
				if(playSound) soundOn(CreateRoleConstData.playerobj.homeindex , CreateRoleConstData.playerobj.sexindex);								//点击门派出现声音
				//////////////////////点击门派时，门派介绍随着变换
//	       		createRole.mcjobps.gotoAndStop(("frame_" + CreateRoleConstData.playerobj.homeindex));
			}
			else
			{
//				createRole.mcjobps.gotoAndStop(1);
				
			}
		}
		private function manClear():void
		{
			if(dictionary[rolePcNameObj] && createRole.contains(dictionary[rolePcNameObj]))  
			{
				createRole.removeChild(dictionary[rolePcNameObj]);
			}
			if(dictionary[rolePcNameObj + "1"] && createRole.contains(dictionary[rolePcNameObj + "1"]))
			{
				createRole.removeChild(dictionary[rolePcNameObj + "1"]);
			}
		}
		private function createOverHandler(e:MouseEvent):void
		{
			(createRole.mcHightLight as MovieClip).addEventListener(Event.ENTER_FRAME , frameHandler);
			(createRole.mcHightLight as MovieClip).play();
			
		}
		private function createOutHandler(e:MouseEvent):void
		{
			(createRole.mcHightLight as MovieClip).gotoAndStop(1);
		}
		private function frameHandler(e:Event):void
		{
			if((createRole.mcHightLight as MovieClip).currentFrame == (createRole.mcHightLight as MovieClip).totalFrames)
			{
				(createRole.mcHightLight as MovieClip).removeEventListener(Event.ENTER_FRAME , frameHandler);
				(createRole.mcHightLight as MovieClip).gotoAndStop(1);
			}
		}
	}
}