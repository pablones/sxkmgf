package Net.ActionProcessor
{
	import GameUI.Command.SelectRoleCommand;
	import GameUI.ConstData.CommandList;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Login.Data.CreateRoleConstData;
	import GameUI.Modules.Login.StartMediator.TestLogin;
	
	import Net.GameAction;
	import Net.GameNet;
	
	import OopsFramework.Debug.Logger;
	import OopsFramework.Net.BaseSocket;
	
	import Vo.RoleVo;
	
	import flash.external.ExternalInterface;
	import flash.system.Security;
	import flash.utils.ByteArray;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	/** 连接游戏服务器  */
	public class GameServerInfo extends GameAction
	{
		private var securSocket:BaseSocket;
		private var timeOutId:uint;
		
		public function GameServerInfo(isUsePureMVC:Boolean = true)
		{
			super(true);
		}
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position     = 4;
			var szReg:RegExp = /com/g;
			var idAccount:uint = bytes.readUnsignedInt(); 				// idAccount
			var dwData:uint    = bytes.readUnsignedInt();				// dwData
			var gsIndex:uint = bytes.readUnsignedInt();      			//游戏服务器索引
			var szInfo:String  = bytes.readMultiByte(320,GameCommonData.CODE); 		    
			var obj:Object = null;
			
			Logger.Info(this,"Processor",szInfo + " 索引：" + gsIndex + " bytes.length " + bytes.length); 
																		
			//获取游戏服务器信息
			if (!GameCommonData.IsConnectAcc) 																		
			{	
				GameCommonData.GameServerArr = szInfo.split(";");
				var gsIp:String = GameCommonData.GameServerArr[gsIndex].split(":")[1];									//游戏服务器ip
				initGameNetInfo(gsIndex);
				
				obj 	      = new Object();
				obj.len	      = 336;															
				obj.ip 	      = GameConfigData.GameSocketIP;
				obj.dwData    = dwData;
				obj.idAccount = idAccount;
				GameCommonData.GServerInfo = obj;
				CreateRoleConstData.playerobj.id = obj.idAccount;	                //将游戏ID保存在创建人物的数据里
				
				if(GameCommonData.Tiao)
				{
					if ( GameCommonData.wordVersion == 1 )
					{
						GameCommonData.Tiao.content_txt.text = "正在连接游戏服务器.....";
					}
					else if ( GameCommonData.wordVersion == 2 )
					{
						GameCommonData.Tiao.content_txt.text = "正在連接遊戲服務器.....";
					}
				}
				GameCommonData.IsConnectAcc = true;
				GameCommonData.isReceiveAcc = true;
				Logger.Info(this,"Processor","正在连接游戏服务器IP：" + obj.ip + ":" + GameConfigData.GameSocketPort);
				
				if(!gsIp)
				{
					if(GameCommonData.Tiao)
					{
						GameCommonData.Tiao.content_txt.text = szInfo;
					}
					return;
				}
				var checkArr:Array = gsIp.split( "." );
				if ( checkArr[ checkArr.length-1 ] == "com" || gsIp.split(".").length == 4 || szReg.test(gsIp) || checkArr[ checkArr.length-1 ] == "cn" || checkArr[ checkArr.length-1 ] == "tw" )
//				if (gsIp.split(".")[2] == "com" || gsIp.split(".").length == 4 || szReg.test(gsIp))
				{
					GameCommonData.AccNets.Close();
					GameCommonData.AccNets = null;
					
					timeOutId = setTimeout( connetGameServe,500 );			//延迟500毫秒连游戏服务器
				} 
				return;
			} 
			else 
			{
				if(GameCommonData.isReceive1052)
				{
					return;
				}
//				callJava();
				 
				if(GameCommonData.Tiao)
				{
					if ( GameCommonData.wordVersion == 1 )
					{
						GameCommonData.Tiao.content_txt.text = "正在获取角色信息.....";	
					}
					else if ( GameCommonData.wordVersion == 2 )
					{
						GameCommonData.Tiao.content_txt.text = "正在獲取角色信息.....";	
					}
				}	
				
				Logger.Info(this,"Processor","开始获取角色信息");
		
				var roleNum:int = bytes.readUnsignedInt();

				GameCommonData.RoleList			  = new Array(); 
				for( var i:int = 0; i<roleNum; i++ ) 
				{
					var role:RoleVo = new RoleVo();
					role.UserId = bytes.readUnsignedInt();
					role.Level  = bytes.readUnsignedShort();
					role.Photo  = bytes.readUnsignedShort();                  //头像
					role.FirJob = bytes.readUnsignedShort();    			 //主职业
					role.FirLev = bytes.readUnsignedShort();               	 //主职业等级
					role.SecJob = bytes.readUnsignedShort();                 //副职业
					role.SecLev = bytes.readUnsignedShort();                 //副职业等级
					
					role.Coattype 	     = bytes.readUnsignedInt();			 //外装
					role.Wapon 		     = bytes.readUnsignedInt(); 		 //武器
					role.Mount		     = bytes.readUnsignedInt();          //坐骑
					role.sexindex        = bytes.readUnsignedInt();          //第二职业改为性别 
					bytes.readUnsignedInt();
					bytes.readUnsignedInt();				
					role.SzName		= bytes.readMultiByte(16,GameCommonData.CODE); 
					GameCommonData.RoleList.push(role);	
				}

				/** 移除测试登录 */
				TestLogin.removeLogin();
				
				GameCommonData.isReceive1052 = true;
				facade.registerCommand(CommandList.SELECTROLECOMMAND, SelectRoleCommand);
//				if(!ChgLineData.isChgLine)
//				{
//					AudioController.SoundLoginOn();												//播放背景音乐
//				}
				
				if(GameCommonData.RoleList.length == 1)										//如果已有一个角色，加载资源后进入游戏
				{
					facade.sendNotification(CommandList.SELECTROLECOMMAND,0);
				}else if(GameCommonData.RoleList.length > 1){                                //如果有多个角色，按原来选择的角色加载资源后进入游戏
					facade.sendNotification(CommandList.SELECTROLECOMMAND,GameCommonData.SelectedRoleIndex);
				}else if(GameCommonData.RoleList.length == 0)								//如果没有角色，直接进入创建角色界面
				{
					if ( GameCommonData.wordVersion == 1 )
					{
						sendNotification(HintEvents.RECEIVEINFO, { info:"服务器繁忙！", color:0xffff00 } );
					}
					else if ( GameCommonData.wordVersion == 2 )
					{
						sendNotification(HintEvents.RECEIVEINFO, { info:"服務器繁忙！", color:0xffff00 } );
					}
					
//					var gameInit:GameRoleManager = new GameRoleManager(GameCommonData.GameInstance);			//没有角色就加载角色管理资源
				}
				/** 显示选择角色面板，后期可能需要恢复，务删 
				facade.sendNotification(EventList.SHOWSELECTROLE);
				**/
			}
		}
		
		private function connetGameServe():void
		{
			clearTimeout(timeOutId);
			Security.loadPolicyFile("xmlsocket://"+GameConfigData.GameSocketIP+":843");
			GameCommonData.GameNets = new GameNet(GameConfigData.GameSocketIP,GameConfigData.GameSocketPort);
		}
		
		private function callJava():void
		{
			var severId:int = ChgLineData.getLineId(GameConfigData.GameSocketName); 
			try
			{
				ExternalInterface.call("changeLine",severId);
			}
			catch(e:Error)
			{
				
			}
		}
		
		//选择要进入的游戏服务器
		private function initGameNetInfo(index:int):void
		{
			if ( GameConfigData.GameSocketName  == "" )
			{
				GameConfigData.GameSocketName = GameCommonData.GameServerArr[index].split(":")[0];	
				GameConfigData.GameSocketIP = GameCommonData.GameServerArr[index].split(":")[1];						//游戏服务器ip
				GameConfigData.GameSocketPort = uint(GameCommonData.GameServerArr[index].split(":")[2]);				//游戏服务器端口
				GameConfigData.GameSeverNum = uint(GameCommonData.GameServerArr[index].split(":")[3]);				//游戏服务器当前人数
			}
			else
			{
				try
				{
					for ( var i:uint=0; i<GameCommonData.GameServerArr.length-1; i++ )
					{
						var aTotalInfo:Array = GameCommonData.GameServerArr[i].split(":");
						if ( aTotalInfo[0] == GameConfigData.GameSocketName )
						{
							GameConfigData.GameSocketIP = aTotalInfo[1];						//游戏服务器ip
							GameConfigData.GameSocketPort = uint(aTotalInfo[2]);			//游戏服务器端口
							GameConfigData.GameSeverNum = uint(aTotalInfo[3]);
//							trace ( "切线服务器："+GameConfigData.GameSocketIP+" port:"+GameConfigData.GameSocketPort );
						}
					}
				}
				catch (e:Error)
				{
					
				}
			}

		}
	}
}