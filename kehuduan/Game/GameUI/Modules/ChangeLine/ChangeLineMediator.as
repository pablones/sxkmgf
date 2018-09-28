package GameUI.Modules.ChangeLine
{
	import Controller.PlayerSkinsController;
	import Controller.ScreenController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.Buff.Data.BuffEvent;
	import GameUI.Modules.ChangeLine.Command.ChgLineSendCommand;
	import GameUI.Modules.ChangeLine.Command.ChgLineSucCommand;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.ChangeLine.View.ChgListCell;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.Team.Datas.TeamEvent;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownEvent;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UICore.UIFacade;
	
	import Net.AccNet;
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.system.Security;
	import flash.text.TextField;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class ChangeLineMediator extends Mediator
	{
		public static const NAME:String = "ChangeLineMediator";
		
		private var cellContainer:Sprite;
		private var serverList:Array;
		private var bgPanel:MovieClip = new MovieClip();
		private var newSname:String;
		private var nameArr:Array;
		private var load_mc:MovieClip = null;
		private var aCell:Array = [];
		private var keyScreen_mc:MovieClip;
		private var dataProxy:DataProxy;
		private var timeOutId:int;
		
		private const unityLine:String = GameCommonData.wordDic[ "mod_cha_dat_chg_get_6" ];				//专线
		
		//开始时间
		private var startTime:Date;
		
		public function ChangeLineMediator()
		{
			super(NAME);
		}
		
		public function get changeLineView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [EventList.INITVIEW,
						EventList.ENTERMAPCOMPLETE,
						ChgLineData.UPDATA_SERVER,
						ChgLineData.CHG_LINE_GO,
						ChgLineData.CHG_LINE_SUC,
						ChgLineData.ONE_KEY_HIDE,
						ChgLineData.GO_TO_TARGET_LINE,
						ChgLineData.REC_UNITY_MAP_ORDER
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.CHANGELINEUI});
					facade.registerCommand(ChgLineSendCommand.NAME,ChgLineSendCommand);
					facade.registerCommand( ChgLineSucCommand.NAME,ChgLineSucCommand );
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
				break;
				case EventList.ENTERMAPCOMPLETE:
					startTime = new Date();
					initData();
					initUI();
					addScreenIcon();						//添加屏蔽玩家小图标
					break;
				case ChgLineData.UPDATA_SERVER:
					refreData(notification.getBody().info);
					break;
				case ChgLineData.CHG_LINE_GO:
//					trace ( notification.getBody().info );
					goNewLine(notification.getBody().info);
//					refreData(notification.getBody().info);
					break;
				case ChgLineData.CHG_LINE_SUC:
					chgSucHandler();
					break;
				case ChgLineData.ONE_KEY_HIDE:
					setScreenTxt();
					break;
				case ChgLineData.GO_TO_TARGET_LINE:
					newSname = notification.getBody().toString();
					chooseServer();
					break;
				case ChgLineData.REC_UNITY_MAP_ORDER:
					goUnityMap( notification.getBody() as int );
				break;
			} 
		}
		
		//处理去帮派的消息
		private function goUnityMap( target:int ):void
		{
			if ( target == 20 )
			{
				newSname = GameConfigData.specialLineName; 
			}
			else if ( target == 0 )
			{
				var newLinesArr:Array = ChgLineData.gsNameArr.concat();
				var specialIndex:int = newLinesArr.indexOf( GameConfigData.specialLineName );
				if ( specialIndex>-1 )
				{
					newLinesArr.splice( specialIndex,1 );
				}
				
				var lIndex:int = Math.random() * ( newLinesArr.length - 1 );
				newSname = newLinesArr[ lIndex ];
			}
			chooseServer();
		}
		
		//更新服务器数据
		private function refreData(reInfo:String):void
		{
			serverList = [];
//			reInfo = reInfo.replace( "s",unityLine );
			for(var i:uint=0; i<reInfo.split(";").length-1; i++)
			{
				var arr:Array = reInfo.split(";")[i].split(":");
				var obj:Object = new Object();
				obj.name = arr[0];
				obj.num = arr[3];
				dealObj(obj);
				if ( obj.name != "" && obj.name != GameConfigData.specialLineName ) 
				{
					if ( obj.name == GameConfigData.GameSocketName )
					{
						GameConfigData.GameSeverNum = obj.num;
					}
					serverList.push(obj);
				}
			}
			initCell();
			
			//更新主文本
			upDateMainText();
		}
		
		private function upDateMainText():void
		{
			changeLineView.curServer_txt.htmlText = GameConfigData.GameSocketName.replace( GameConfigData.specialLineName, GameCommonData.wordDic[ "mod_cha_dat_chg_get_6" ] ) + isFull(GameConfigData.GameSeverNum);  //专线
		}
		
		//首次进入游戏之后的服务器列表
		private function initData():void
		{
			serverList = [];
			ChgLineData.gsNameArr = [];
			for(var i:uint=0; i<GameCommonData.GameServerArr.length-1; i++)
			{
				var obj:Object = new Object();
				var arr:Array = GameCommonData.GameServerArr[i].split(":");
				obj.name = arr[0];
				obj.num = arr[3];
				dealObj(obj);
				if ( obj.name != "" && obj.name != GameConfigData.specialLineName )
				{
					serverList.push(obj);
				}
				ChgLineData.gsNameArr.push(arr[0]);
			}
		}
		
		private function initUI():void
		{
			changeLineView.x = 723.5;
			changeLineView.y = 0;
			changeLineView.tabEnabled = false;
			changeLineView.tabChildren = false;
			changeLineView.curServer_txt.textColor = 0xBEA05F;
			changeLineView.curServer_txt.mouseEnabled = false;
			changeLineView.buttonMode = true;
			changeLineView.addEventListener(MouseEvent.MOUSE_DOWN,showList);
			changeLineView.addEventListener(MouseEvent.MOUSE_OVER,overMainHandler);
			GameCommonData.GameInstance.WorldMap.addChild(changeLineView);
			GameCommonData.GameInstance.stage.addEventListener(MouseEvent.MOUSE_DOWN,closeList);
			
			cellContainer = new Sprite();
			cellContainer.mouseEnabled = false;
			cellContainer.x = 724;
			cellContainer.y = 18.5;
			
			this.bgPanel=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ListBack");
			bgPanel.width = 90.5;
			bgPanel.x = 0.5;
			bgPanel.y = 0;
			cellContainer.addChild(bgPanel);
			
			initCell();
			upDateMainText();
			
			sendNotification(EventList.STAGECHANGE);		}
		
		private function initCell():void
		{
			clearCell();
			aCell = [];
			serverList.sortOn("index" , Array.NUMERIC );
//			bgPanel.height = 18*serverList.length+0.5;
			var count:uint = 0;
			for(var i:uint=0; i<serverList.length; i++)
			{
				var sName:String = serverList[i].name;
				//如果是六线，就不显示了
				if ( sName == unityLine )
				{
					continue;
				}
				if( GameCommonData.isLoginFromLoader == false ) GameCommonData.FilterGameServerArr = GameCommonData.GameServerArr;
				var bool:Boolean = true;
				if( GameCommonData.FilterGameServerArr && GameCommonData.FilterGameServerArr.length > 0 )
				{
					for each(var str:String in GameCommonData.FilterGameServerArr)
					{
						if( str.split(":")[0] == sName ) 
						{
							bool = false;
							break;
						} 
							
					}
				}
				
				if( bool ) 
				{
					count += 1;
					continue;
				}
				var sNum:uint = uint(serverList[i].num);
				var isFull:String = isFull(sNum);
				var cell:ChgListCell = new ChgListCell(sName,isFull);
				cell.addEventListener(MouseEvent.MOUSE_DOWN,clickCell);
				cell.x = 1;
				cell.y = (i-count)*18;
				aCell.push(cell);
				cellContainer.addChild(cell);
			}
			bgPanel.height = 18*aCell.length+0.5;
		}
		
		private function clearCell():void
		{
			if (aCell.length>0)
			{
				for (var i:uint=0; i<aCell.length; i++)
				{
					if (aCell[i] && cellContainer.contains(aCell[i]))
					{
						cellContainer.removeChild(aCell[i]);
						aCell[i] = null;
					}
				}
			}
		}
		
		//显示列表下拉菜单
		private function showList(evt:MouseEvent):void
		{
			if (!GameCommonData.GameInstance.WorldMap.contains(cellContainer))
			{
				evt.stopPropagation();
//				var deep:uint = GameCommonData.GameInstance.WorldMap.getChildIndex(this.changeLineView);
				GameCommonData.GameInstance.WorldMap.addChild(cellContainer);
				if( changeLineView.stage.stageWidth > GameConfigData.GameHeight ) 
				{
					cellContainer.x = 724 + changeLineView.stage.stageWidth - GameConfigData.GameWidth;
				}
				return;
			}
			else
			{
				GameCommonData.GameInstance.WorldMap.removeChild(cellContainer);
			}
		}
		//点击小按钮
		private function clickCell(evt:MouseEvent):void
		{
			this.newSname = evt.currentTarget.sName;
//			trace ( "aaaaaaaaaaaa: "+(new Date().getTime()-startTime.getTime()).toString() );
			
			if ( GameConfigData.GameSocketName == GameConfigData.specialLineName )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_cli_5" ], color:0xffff00});//"该场景不能切线"
				return;
			}
			
			if ( ( new Date().getTime()-startTime.getTime() )<10000 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_cli_1" ], color:0xffff00});//"十秒后才能切换服务器"
				return;
			}
			
			if(ChgLineData.isChgLine)
			{
				return;
			}
			if (!UIFacade.UIFacadeInstance.checkCanCL())
			{
				return;	
			}
			if (newSname == GameConfigData.GameSocketName)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_cli_2" ], color:0xffff00});//"你位于当前服务器中，不需要切换"
				return;
			}
			if(UnityConstData.isWorking)	facade.sendNotification(EventList.SHOWALERT, {comfrim:chooseServer, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_cha_cha_cli_3" ], title:GameCommonData.wordDic[ "often_used_tip" ]});
			//"切线后您的打工将被取消，真的要切换服务器吗？"		"提 示"
			else	facade.sendNotification(EventList.SHOWALERT, {comfrim:chooseServer, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_cha_cha_cli_4" ], title:GameCommonData.wordDic[ "often_used_tip" ]});
			//"真的要切换服务器吗？"		"提 示"
		}
		
		private function chooseServer():void
		{
			if(ChgLineData.isChooseLine)
			{
				return;
			}
//			trace("evt.name: " +this.newSname); 
			ChgLineData.flyServerInfo = newSname;

			if(GameConfigData.GameSocketName == ChgLineData.flyServerInfo)
			{
				return;
			}
			ChgLineData.isChooseLine = true;
			canHandle(false);
			facade.sendNotification(ChgLineSendCommand.NAME);
		}
		
		//去新服务器
		private function goNewLine(_info:String):void
		{
			var aNewName:Array = [];																				//新的服务器名字列表
			var aInfo:Array = _info.split(";");
			if (GameCommonData.IsChangeOnline)
			{
				return;
			}
			for (var i:uint=0; i<aInfo.length-1; i++)
			{
				var lineName:String = aInfo[i].split(":")[0];
				if (aInfo[i].split(":")[0] == ChgLineData.flyServerInfo)
				{
					if (aInfo[i].split(":")[3] > ChgLineData.maxServerPerson-20)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_goN_1" ], color:0xffff00});//"服务器爆满，无法切换！"
						canHandle(true);
						return;
					}
				}

				aNewName.push(lineName);
			}
			if (aNewName.indexOf(ChgLineData.flyServerInfo) == -1)								//要去的服务器已关闭
			{
				refreData( _info );
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_goN_2" ], color:0xffff00});//"服务器繁忙，无法切换！"
				canHandle(true);
				return;
			}else
			{
				try
				{
					initData();				//初始化服务器列表
//					var flyIndex:int = ChgLineData.gsNameArr.indexOf(ChgLineData.flyServerInfo);
//					GameConfigData.GameSocketIP = GameCommonData.GameServerArr[flyIndex].split(":")[1];						//游戏服务器ip
//					GameConfigData.GameSocketPort = uint(GameCommonData.GameServerArr[flyIndex].split(":")[2]);			//游戏服务器端口
					GameConfigData.GameSocketName = ChgLineData.flyServerInfo;
				}
				catch (e:Error)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_goN_3" ], color:0xffff00});//"服务器繁忙，请稍后再试！"
					canHandle(true);
					return;
				}
			}
			startCircle();
			changeLineView.removeEventListener(MouseEvent.MOUSE_DOWN,showList);							//去掉下拉菜单
			GameCommonData.Player.Role.MountSkinID = 0;
			PlayerSkinsController.SetSkinMountData(GameCommonData.Player);											//下坐骑
			UIFacade.UIFacadeInstance.closeOpenPanel();																														
			GameCommonData.GameNets.endGameNet();																				//断线
			GameCommonData.GameNets = null;
			
			if(GameCommonData.Player.Role.State == GameRole.TYPE_STALL)
		    {
		    	sendNotification(StallEvents.REMOVESTALLNOW);
		    } 
			
//			trace("去新服务器name:"+GameConfigData.GameSocketName,"  ip:"+GameConfigData.GameSocketIP," 端口："+GameConfigData.GameSocketPort);
			//重新连接账号服务器
			
			revertSomeData();
			
			timeOutId = setTimeout( reStartGame,3000 ); 							//延迟3秒重新连账号服务器			
		}
		
		private function reStartGame():void
		{
			clearTimeout( timeOutId );
			Security.loadPolicyFile("xmlsocket://" + GameConfigData.AccSocketIP+ ":843");
			GameCommonData.AccNets = new AccNet(GameConfigData.AccSocketIP, GameConfigData.AccSocketPort);
			
		}
		
		//重新连接账号服务器之前，还原一些数据
		private function revertSomeData():void
		{
			//清BUFF
			GameCommonData.Player.Role.DotBuff = new Array();
			GameCommonData.Player.Role.PlusBuff = new Array();
			GameCommonData.UIFacadeIntance.sendNotification(BuffEvent.DELETEALL);		//清空Buff	
			
			ChgLineData.isChgLine = true;
			GameCommonData.isReceive1052 = false;
			GameCommonData.IsConnectAcc = false;
			GameCommonData.isReceiveAcc = false;
			GameCommonData.isSend = true;
			GameCommonData.IsChangeOnline =true;
//			ChgLineData.flyServerInfo = "";
			
			GameCommonData.Scene.ResetMoveState();	
			GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
			if(GameCommonData.Player.Role.UsingPetAnimal != null)
			{							
		    	GameCommonData.Scene.PetResetMoveState();
		    	GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);
		 	}
		 	if ( dataProxy.GainAwardPanIsOpen )
		 	{
		 		facade.sendNotification( OnLineAwardData.CLOSE_GAINAWARD_PAN );
		 	}
		}
		
		private function isFull(_curNum:uint):String
		{
			var percent:Number = _curNum/ChgLineData.maxServerPerson*100;
			var s:String;
			if (percent>=0 && percent<=30)
			{
				s = "<font color='#00ff00'>（"+GameCommonData.wordDic[ "mod_cha_cha_isF_1" ]+"）</font>";//流畅
				return s;
			}
			else if (percent>30 && percent<=80)
			{
				s = "<font color='#ffff00'>（"+GameCommonData.wordDic[ "mod_cha_cha_isF_1" ]+"）</font>";//正常
				return s;
			}else
			{
				s = "<font color='#ff0000'>（"+GameCommonData.wordDic[ "mod_cha_cha_isF_1" ]+"）</font>";//繁忙
				return s;
			}
		}
		
		private function closeList(evt:MouseEvent):void
		{
			if (cellContainer && GameCommonData.GameInstance.WorldMap.contains(cellContainer))
			{
				GameCommonData.GameInstance.WorldMap.removeChild(cellContainer);
			}
		}
		
		//切线成功
		private function chgSucHandler():void
		{
//			trace("切线成功了！！！！");
			ChgLineData.flyServerInfo = "";      
			endCircle();
			changeLineView.addEventListener(MouseEvent.MOUSE_DOWN,showList);
			if ( GameConfigData.GameSocketName != GameConfigData.specialLineName )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cha_cha_chg" ]+GameConfigData.GameSocketName, color:0xffff00});//"成功切换至服务器："	
			}
			GameCommonData.Player.Role.UsingPetAnimal = null;																		
			sendNotification(TeamEvent.LEAVE_TEAM_AFTER_CHANGE_LINE);			//清除组队信息
			NetAction.requestBuff();                                            //发送buff           
			ChgLineData.isChgLine = false;
			ChgLineData.isChooseLine = false;
			SkillData.isLifeSeeking = false;
			sendNotification(FriendCommandList.FRIEND_INFO_CLEAR);
			sendNotification( ChgLineSucCommand.NAME );
			
			initData();
			initCell();
			//更新主文本
			upDateMainText();
			
			if ( GameCommonData.Player.Role.OnLineAwardTime%10 < 8 )
			{
				sendNotification( OnLineAwardData.NEXT_ONLINE_GIFT );								//在线奖励	
			}
			FriendSend.getInstance().sendAction(FriendSend.getInstance().getFriendListParam()); //请求好友
			UnityConstData.allMenberList = [];									//清除帮派缓存
			UnityConstData.mainUnityDataObj 	= null;
			facade.sendNotification(TimeCountDownEvent.CLOSEWORKCOUNTDOWN);		//关闭打工倒计时
			this.startTime = new Date();                                    
		    

//			NetAction.requestCd();
		}
		
		private function startCircle():void
		{
			//开始转圈
			if(load_mc == null)
			{		
				load_mc   = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("LoadCircle") as MovieClip;	
				load_mc.x = GameCommonData.GameInstance.ScreenWidth  / 2;	
				load_mc.y = GameCommonData.GameInstance.ScreenHeight / 2;		
				GameCommonData.GameInstance.GameUI.addChild(load_mc);
			}
			UIFacade.UIFacadeInstance.closeOpenPanel();
			facade.sendNotification( EventList.CLOSE_NPC_ALL_PANEL );
		}
		
		private function endCircle():void
		{
			//结束转圈
			if (load_mc != null && GameCommonData.GameInstance.GameUI.contains(load_mc))
			{
				GameCommonData.GameInstance.GameUI.removeChild(load_mc);
				load_mc = null;
			}
			canHandle(true);
		}
		
		private function overMainHandler(evt:MouseEvent):void
		{
			changeLineView.gotoAndStop(2);
			changeLineView.removeEventListener(MouseEvent.MOUSE_OVER,overMainHandler);
			changeLineView.addEventListener(MouseEvent.MOUSE_OUT,outMainHandler);
		}
		
		private function outMainHandler(evt:MouseEvent):void
		{
			changeLineView.gotoAndStop(1);
			changeLineView.removeEventListener(MouseEvent.MOUSE_OUT,outMainHandler);
			changeLineView.addEventListener(MouseEvent.MOUSE_OVER,overMainHandler);
		}
		
		private function cancelClose():void
		{
			
		}
		
		private function dealObj(_obj:Object):void
		{
			switch(_obj.name)
			{
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_1" ]://"一线"
					_obj.index = 1;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_2" ]://"二线"
					_obj.index = 2;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_3" ]://"三线"
					_obj.index = 3;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_4" ]://"四线"
					_obj.index = 4;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_5" ]://"五线"
					_obj.index = 5;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_6" ]://"六线"
					_obj.index = 6;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_7" ]://"七线"
					_obj.index = 7;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_8" ]://"八线"
					_obj.index = 8;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_9" ]://"九线"
					_obj.index = 9;
					break;
				case GameCommonData.wordDic[ "mod_cha_dat_chg_get_10" ]://"十线"
					_obj.index = 10;
					break;
				default:
					_obj.index = 100;														//防止发生意外
					break;
			}
		}
		
		//屏蔽玩家
		private function addScreenIcon():void
		{
			keyScreen_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("KeyScreen") as MovieClip;
			keyScreen_mc.x = 658.5;
			keyScreen_mc.y = 0;
			if ( !GameCommonData.GameInstance.WorldMap.contains(keyScreen_mc) )
			{
				GameCommonData.GameInstance.WorldMap.addChild(keyScreen_mc);
				keyScreen_mc.name = "keyScreen";
			}
			keyScreen_mc.buttonMode = true;
			( keyScreen_mc.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_cha_cha_add" ];//"一键屏蔽"
			( keyScreen_mc.hint_txt as TextField ).mouseEnabled = false;
			keyScreen_mc.addEventListener(MouseEvent.CLICK,clickKeyScreen);
//			ScreenController.SetScreen();
		}
		
		private function clickKeyScreen(evt:MouseEvent):void
		{
			setScreenTxt();
		}
		
		private function setScreenTxt():void
		{
			if ( ScreenController.IsCanChangeScreen )
			{
				ScreenController.SetScreen();	
			}
			
			if ( keyScreen_mc )
			{
				switch ( GameCommonData.Screen )
				{
					case 2:
						( keyScreen_mc.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_cha_cha_add" ];//"一键屏蔽"
					break;
				
					case 0:
						( keyScreen_mc.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_cha_cha_set_1" ];//"显示玩家"
					break;
				
					case 1:
						( keyScreen_mc.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_cha_cha_set_2" ];//"显示宠物"
					break;
				}
			}
		}
		
		private function canHandle(isHandle:Boolean):void
		{
			GameCommonData.GameInstance.GameUI.mouseEnabled = isHandle;
			GameCommonData.GameInstance.GameUI.mouseChildren = isHandle;
		}
	}
}