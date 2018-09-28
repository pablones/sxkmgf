package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Alert.AlertMediator;
	import GameUI.Modules.Arena.Command.ArenaPanelCommandList;
	import GameUI.Modules.Arena.Data.ArenaScore;
	
	import Net.ActionSend.WarGameSend;
	import Net.GameAction;
	
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;
	
	public class WarGame extends GameAction
	{
		public function WarGame(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}

		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			var obj:Object = new Object();
			
			obj.idUser  = bytes.readUnsignedInt();				//用户ID
			obj.nAction	= bytes.readUnsignedShort();			//action
			obj.nPage	= bytes.readUnsignedByte();				//页数 页码
			obj.nAmount = bytes.readUnsignedByte();				//数量
			
			if (obj.nAction == 4)
			{
				var id:uint = bytes.readUnsignedInt();
				if(obj.nPage==1)
				{
					sendNotification(EventList.SHOWALERT, {title: "", info: GameCommonData.wordDic[ "net_act_war_pro_1" ], // 星浮宫竞技活动已经开启，请确认是否进入。
																					confirmText: GameCommonData.wordDic[ "net_act_war_pro_2" ], // 确定进入
																					cancelText: GameCommonData.wordDic[ "net_act_war_pro_3" ], // 取消参与
																					comfrim: function():void{WarGameSend.sendWarGameAction({action:4, memID:id});},
																					cancel: function():void{},
																					timeout: 60* 1000
																					});
				}
				else if (obj.nPage == 2)
				{
					sendNotification(EventList.SHOWALERT, {title: "", info: GameCommonData.wordDic[ "net_act_war_pro_4" ], // 星浮宫竞技场中仍有空位，请问是否进入？
																					confirmText: GameCommonData.wordDic[ "net_act_war_pro_5" ],
																					cancelText: GameCommonData.wordDic[ "net_act_war_pro_6" ],
																					comfrim: function():void{WarGameSend.sendWarGameAction({action:4, memID:id});},
																					cancel: function():void{},
																					timeout: 60 * 1000
																					});
				}
				
				return;
			}
			else if (obj.nAction == 5)
			{
				ArenaScore.initialized = true;
				bytes.position += 4 * 2;
				ArenaScore.camp1Score = bytes.readUnsignedInt();
				ArenaScore.camp2Score = bytes.readUnsignedInt();
				bytes.position += 4;
				ArenaScore.camp3Score = bytes.readUnsignedInt();
				
//				trace(">>> RECV: ");
//				while (bytes.position < bytes.length - 1)
//				{
//					trace(bytes.readUnsignedInt());
//				}
				sendNotification(ArenaPanelCommandList.ARENASMALLPANEL_UPDATE);
			}
			else // obj.nAction == 3
			{
				ArenaScore.initialized = true;
				for(var i:uint = 0 ; i < obj.nAmount ; i ++)
				{
					var idUser:uint = bytes.readUnsignedInt();			//玩家ID
					var nRank:uint = bytes.readUnsignedShort(); 		//排名
					var nLev:uint = bytes.readUnsignedShort(); 			//等级
					var nCurScore:uint = bytes.readUnsignedInt();		//当前积分
					var nAwardScore:uint = bytes.readUnsignedInt();		//奖励积分
					var nKill:uint = bytes.readUnsignedShort(); 		//杀人数
					var nCamp:int = bytes.readUnsignedByte();			//阵营
					var nVipLev:int = bytes.readUnsignedByte();			//VIP等级
					var nPro:uint = bytes.readUnsignedInt();			//职业
					var name:String = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
					
					if(idUser == 1)//各阵营积分
					{
						ArenaScore.camp1Score = nCurScore;
						ArenaScore.camp2Score = nAwardScore;
						ArenaScore.camp3Score = nPro;
					}
					else if(idUser == 2)//积分榜数据结束
					{
						//所有数据按积分排名次然后显示
						facade.sendNotification(ArenaPanelCommandList.ARENAPANEL_UPDATE);
					}
					else if (idUser == 3)//竞技场结束
					{
						facade.sendNotification(ArenaPanelCommandList.ARENAPANEL_UPDATE);
						facade.sendNotification(ArenaPanelCommandList.ARENAPANEL_SHOW, {update: false});
						facade.sendNotification(ArenaPanelCommandList.ARENASMALLPANEL_HIDE);
					}
					else if(idUser == GameCommonData.Player.Role.Id)//自己的
					{
						ArenaScore.myCamp = nCamp;
						ArenaScore.myScore = nCurScore;
						ArenaScore.myAwardScore = nAwardScore;
						ArenaScore.myKill = nKill;
						ArenaScore.myRank = nRank;
						ArenaScore.myLevel = nLev;
						ArenaScore.myVIPLevel = nVipLev;
						ArenaScore.myPro = nPro;
						ArenaScore.myName = name;
						
						ArenaScore.listFull.addItem({id: GameCommonData.Player.Role.Id,
																	rank:nRank,
																	name: name,
																	currentScore: nCurScore,
																	awardScore: nAwardScore,
																	kill: nKill,
																	camp: nCamp,
																	level: nLev,
																	pro: nPro,
																	vipLevel: nVipLev});
					}
					else
					{
						ArenaScore.listFull.addItem({id: idUser,
																	rank: nRank,
																	name: name,
																	currentScore: nCurScore,
																	awardScore: nAwardScore,
																	kill: nKill,
																	camp: nCamp,
																	level: nLev,
																	pro: nPro,
																	vipLevel: nVipLev});
					}
				}
				facade.sendNotification(ArenaPanelCommandList.ARENAPANEL_UPDATE);
			}

//			switch ( obj.nAction )
//			{
//				case 3://积分榜数据
//					
//				break;
//				case 4:
//					if(nPage==1)//是否进入
//					else if(nPage==2)//有空位
//				break;
//			}
		}
	}
}