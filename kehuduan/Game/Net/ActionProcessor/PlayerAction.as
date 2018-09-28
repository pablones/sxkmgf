package Net.ActionProcessor
{
	import Controller.FloorEffectController;
	import Controller.FlutterController;
	import Controller.PlayerController;
	import Controller.SceneController;
	
	import GameUI.Command.CloseAllViewCommand;
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Artifice.Mediator.ArtificeMediator;
	import GameUI.Modules.Artifice.data.ArtificeConst;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayPetCommand;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.CardFiles.NewerCardMediator;
	import GameUI.Modules.CardFiles.NewerCardNewMediator;
	import GameUI.Modules.CardForTaiWan.NewCardForTaiWanMediator;
	import GameUI.Modules.CardForTaiWan.StoreMoneyCardMediator;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.Modules.Depot.Mediator.DepotMediator;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.GotoCopy.data.GotoCopyCommand;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyTreasure.Net.TreasureNet;
	import GameUI.Modules.LotteryCardFor360.LotteryCardFor360Mediator;
	import GameUI.Modules.Master.Command.AgreementMasterCommand;
	import GameUI.Modules.Master.Command.ReLeaseMasterCommand;
	import GameUI.Modules.Master.Command.TutorGraduateCommand;
	import GameUI.Modules.MediaCard.*;
	import GameUI.Modules.Meridians.Components.MeridiansTimeOutComponent;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.model.MeridiansTypeVO;
	import GameUI.Modules.Meridians.tools.Tools;
	import GameUI.Modules.NewPlayerCard.Data.NewerCardData;
	import GameUI.Modules.NewPlayerSuccessAward.Mediator.NewAwardMediator;
	import GameUI.Modules.NewSocietyCard.Data.NewSocietyCardData;
	import GameUI.Modules.NewSocietyCard.Mediator.NewSocietyCardMediator;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Mediator.PetPlayMediator;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleEvent;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.Data.PetSavvyJoinEvent;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyEvent;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnEvent;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpEvent;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyEvent;
	import GameUI.Modules.PetPlayRule.PetWinningUp.Data.PetWinningEvent;
	import GameUI.Modules.Pk.Data.PkEvent;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Modules.PreventWallow.Data.PreventWallowEvent;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Mediator.EquipMediator;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Mediator.*;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.StoneMoney.Mediator.StoneMoneyMediator;
	import GameUI.Modules.StrengthenTransfer.Mediator.StrengthenTransferMediator;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferConst;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownEvent;
	import GameUI.Modules.TimeCountDown.TimerNewComponent;
	import GameUI.Modules.TimelinesBox.Data.TimelinesBoxData;
	import GameUI.Modules.TimelinesBox.Mediator.TimelinesBox;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.VipHeadIcon.data.VipHeadIconData;
	import GameUI.Modules.Wish.Data.WishData;
	import GameUI.MouseCursor.DelayOperation;
	import GameUI.MouseCursor.RepeatRequest;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.FriendSend;
	import Net.ActionSend.PlayerActionSend;
	import Net.ActionSend.UnityActionSend;
	import Net.GameAction;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Skill.GameSkill;
	
	import OopsFramework.Debug.Logger;
	
	import flash.display.DisplayObject;
	import flash.utils.ByteArray;
	import flash.utils.getTimer;

	/** 
	 * 玩家控制角色上线（14）  
	 * 服务器打回服务器的正确坐标点(57)
	 * 其它玩家从屏幕上删除（97） 
	 * 获取背包里的所有物品信息（15） 
	 * 已开始攻击了(1022)
	 **/
	public class PlayerAction extends GameAction
	{
		public static const PLAYER_ATTACK:int = 102;	// 已开始攻击了
		public static const PLAYER_END_ATTACK:int = 103;// 已经结束攻击
		public static const PLAYER_ONLINE:int = 14;		// 玩家控制角色上线
		public static const PLAYER_DELETE:int = 97;		// 其它玩家从屏幕上删除
		public static const WALK_RETURN:int   = 57;		// 服务器打回服务器的正确坐标点
		public static const OPERATE_ITEM:int  = 15;		// 玩家控制角色上线
		public static const GET_OUTFIT:int    = 27;		// 获取玩家装备
				
		public static const BEGIN_STALL:int   = 43;		// 开始摆摊（C请求摆摊，S返回则开始摆摊）
		public static const DESTROY_STALL:int = 46;		// 撤销摆摊（C-S)
		public static const CHANGE_MAP:int    = 8;		// 转场
		public static const PetMomentMove:int = 19;		// 瞬间移动
		
		public static const DIE:int     = 11;			// 死亡
		public static const RELIVE:int  = 128;			// 复活		
		public static const STANDBY:int = 125;			// 原地站立，相当于重置（变为初始站立状态）
		public static const OPEN_DIALOG:int = 69;		// 打开界面
		public static const QUERY_MONEY:uint = 268;		// 打开商城时 查询玩家元宝、珠宝、点券
		
		public static const actionTransPos:int = 96;
		
		public static const HEARTPOINT:int = 267;		// 游戏心跳
		
		//////////操作宠物
		public static const PET_RENAME:int        = 244;			// 改名
		public static const PET_EATHP:int         = 245;			// 吃血药
		public static const PET_TRAIN:int         = 246;			// 驯养
		public static const PET_DEOP:int          = 247;			// 放生
		public static const PET_ADDPOINTS:int     = 248;			// 加点
		public static const PET_RENAME_FAILE:int  = 249;			// 改名失败
		
		public static const PET_ADDEUM_BREEDDOUBLE:int    = 250;	// 自己添加快照   （双繁）
		public static const PET_LOCKSELF_BREEDDOUBLE:int  = 251;	// 自己锁定 		（双繁）
		public static const PET_SURE_BREED:int  	  	  = 252;	// 确定开始繁殖  （双繁、单繁(单繁时两个用户ID相同即可)）
		public static const PET_CANCEL_BREED:int  	  	  = 253;	// C-S取消繁殖  	（双繁)
		
		public static const PET_TO_BABY:int  	  	      = 254;	// 还童
		public static const PET_SAVVY_USEMONEY:int  	  = 255;	// 提升悟性
		public static const PET_SAVVY_JOIN:int  	  	  = 256;	// 合成
		public static const PET_SKILL_LEARN:int  	  	  = 257;	// 学习技能
		public static const PET_SKILL_LEARN_FAIL:int	  = 261;	// 遗忘了某技能
		public static const PET_SKILL_UP:int  	  	      = 258;	// 提升技能
		
		public static const PET_GOTO_FIGHT:int			  = 259;	//出战
		public static const PET_GOTO_REST:int			  = 260;	//休息
		public static const PET_EXT_LIFE:int			  = 276;	//延寿
		public static const NEWPET_FANTASY_TAG:int			  = 316;	//宠物幻化
		public static const NEWPET_WINNING_TAG:int			  = 317;	//宠物提升灵性
		public static const NEWPET_PRIVITY_TAG:int			  = 318;	//宠物提升默契
		public static const NEWPET_DEPENDENCE_TAG:int		  = 320;	//宠物附体
		public static const NEWPET_CANCEL_DEPENDENCE_TAG:int	 = 321;	//宠物取消附体（分离）
		public static const NEWPET_CHANGE_SEX_DEPENDENCE_TAG:int = 322;	//宠物取消附体（分离）
		public static const NEWPET_PLAY_SELECT:int				 = 323; //选择宠物玩耍
		public static const NEWPET_PLAY_LOCKED:int				 = 324; //选择宠物锁定
		public static const NEWPET_PLAY_SURE:int				 = 325; //确定玩耍
		public static const NEWPET_PLAY_CANCEL:int				 = 326; //取消玩耍
		/////////////////////////////////////////////////////////////////////////////////////
		
		public static const PK_SWITCH:int              	  = 208;	//pk状态
		public static const START_LIFE_SEEK:int                    = 282;				//开始生活采集
		public static const STOP_LIFE_SEEK:int         = 283;				//停止生活采集
		
		//师徒
		public static const MASTER_OK:uint = 72;
		public static const PRENTICE_OK:uint = 74;
		public static const GRADUATE_OK:uint = 73;					//徒弟出师
		public static const RELEASE_MASTER:uint = 225;
		
		public static const REQUEST_AUTO_FIGHT_INFO:uint = 292;		//请求自动挂机信息
		
		public static const CLIENT_INIT_ALL_COMP:uint = 293;		//客户端初始化全部完成
		
		public static const REQUEST_DESIGNATION_INFO:uint = 294;	//客户端请求称号信息
		public static const SEND_DESIGNATION_INFO:uint = 295;    //请求修改称号信息
		public static const SEND_GOTOCOPY_INFO:uint = 309;    //请求队员副本条件信息
		public static const DEAL_AFTER_COMOSE_SOUL_STONE:uint = 312;    //魂魄润魂石（通灵玉）合成成功
			
		private var dataProxy:DataProxy; 
		
		private var isSpeedCloth:Boolean = true;					//true为开启防加速外挂
		
		public function PlayerAction(isUsePureMVC:Boolean = true)
		{
			super(isUsePureMVC);
		}
		
		/** 收到服务器发来的消息  */
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			var obj:Object = new Object();
			obj.nTimeStamp = bytes.readUnsignedInt(); 
			obj.nRoleId    = bytes.readUnsignedInt(); 		// 角色编号（玩家，NPC，怪物）
			obj.nPosX	   = bytes.readUnsignedShort(); 	// 起点A*格X坐标
			obj.nPosY 	   = bytes.readUnsignedShort(); 	// 起点A*格Y坐标
			obj.nDir  	   = bytes.readUnsignedShort(); 	// 方向
			bytes.readUnsignedShort();
			obj.nMapId     = bytes.readUnsignedInt();		// 地图编号
			obj.nAction    = bytes.readUnsignedShort();		// 动做编号
			bytes.readUnsignedShort();
			obj.nNPC 	   = bytes.readUnsignedInt();       //
			
			/** 触发UI显示  */
//			facade.sendNotification(CommandList.PLAYERACTIONCOMMAND, obj);
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
//			try
//			{ 
				/** 触发人协议动做  */
				switch(obj.nAction)
				{
					
					case PLAYER_ONLINE:
						GameCommonData.Player.Role.TileX = obj.nPosX;
						GameCommonData.Player.Role.TileY = obj.nPosY;
						GameCommonData.enterGameObj = obj;

			            if(GameCommonData.Tiao)
                        {
//				          GameCommonData.Tiao.content_txt.text = GameCommonData.wordDic[ "net_ap_pa_proc_1" ];             //"准备加载游戏资源....." 
							if ( GameCommonData.wordVersion == 1 )
							{
				          		GameCommonData.Tiao.content_txt.text = "准备加载游戏资源.....";  		
							}
							else if ( GameCommonData.wordVersion == 2 )
							{
								GameCommonData.Tiao.content_txt.text = "準備加載遊戲資源.....";  		
							}
			       		}			
			       		
			       		if(GameCommonData.IsFirstLoadGame)
						{				
							var gameInit:GameInit = new GameInit(GameCommonData.GameInstance);			//加载游戏资源
						}
						else
						{
//							GameCommonData.Scene = new SceneController(GameCommonData.GameInstance.GameScene.GetGameScene.name,
//																	   GameCommonData.GameInstance.GameScene.GetGameScene.MapId);		// 初始化游戏场景 
							GameCommonData.Scene = new SceneController( obj.nMapId,
																	   obj.nRoleId );		// 初始化游戏场景																	   
						}
						break;
					case PLAYER_DELETE:			// 可以删除地图上任保可视物件	//0玩家，宠物  1地图物品 2摊位  宠物
						if(obj.nMapId == 0 || obj.nMapId == 2) 
						{
							if(GameCommonData.SameSecnePlayerList != null)
							{
								var player:GameElementAnimal = GameCommonData.SameSecnePlayerList[obj.nRoleId];						
								if(player != null && player.handler != null)
								{
									player.handler.Clear();
								}
								
								if(player != null)
								{
									GameCommonData.Scene.DeletePlayer(obj.nRoleId);	
								}
							}											
						}
						else if(obj.nMapId == 1)
						{
							GameCommonData.Scene.DeletePackage(obj.nRoleId);
						}
						break;
					case 214:
					    if(GameCommonData.Player.Role.UsingPetAnimal != null)
					    {
					    	GameCommonData.Scene.DeletePlayer(GameCommonData.Player.Role.UsingPetAnimal.Role.Id);							
					    }
					    GameCommonData.Player.Role.UsingPetAnimal = null; 
					    break;
					case WALK_RETURN:
						GameCommonData.Player.Role.Direction = obj.nDir			// 打回才有方向
						GameCommonData.Player.Role.TileX     = obj.nPosX;
						GameCommonData.Player.Role.TileY     = obj.nPosY;
						GameCommonData.Scene.StopPlayerMove(GameCommonData.Player);
						if(GameCommonData.TargetScene.length > 0)
						{	
							if(GameCommonData.TargetScene != GameCommonData.GameInstance.GameScene.GetGameScene.name)
							{	
								GameCommonData.Scene.MoveNextScenePoint();
							}
							else
							{
								GameCommonData.Scene.playerdistance = GameCommonData.TargetDistance;
								GameCommonData.Scene.PlayerMove(MapTileModel.GetTilePointToStage(GameCommonData.TargetPoint.x,GameCommonData.TargetPoint.y));
								GameCommonData.TargetScene = "";
							}
						}  
						trace(obj.nPosX,obj.nPosY,"ickback!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
						break;
					case OPERATE_ITEM:
                         RepeatRequest.getInstance().removeDelayProcessFun("bag");
                         if(obj.nMapId==RepeatRequest.getInstance().bagItemCount)
                         {
                         	Logger.Info(this,"Processor_OPERATE_ITEM","获得背包成功");       //"获得背包成功"
                         	
                         	if(RepeatRequest.getInstance().successFlags[0])
                         	{
                         		return;
                         	}
                         	else
                         	{
                         		RepeatRequest.getInstance().successFlags[0] = true;
                         	}
							facade.sendNotification(EventList.GETITEMS);
							RepeatRequest.getInstance().petItemCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("pet",NetAction.requestPet,20000);
							NetAction.requestPet();
//							facade.sendNotification(RoleEvents.GETFITOUTBYBAG);
                         }else{
							RepeatRequest.getInstance().addDelayProcessFun("bag",NetAction.RequestItems,20000);				
							NetAction.RequestItems();
                         }                   
						break;	
					case PLAYER_ATTACK:
					    GameCommonData.IsAttack = true;
						break;		
					case PLAYER_END_ATTACK:
						GameCommonData.IsAttack = false;
						break;	
					
					case GET_OUTFIT:	// 获取了装备
						facade.sendNotification(RoleEvents.GETOUTFIT);
						break;	
					case BEGIN_STALL:	// 成功开始摆摊
						sendNotification(EventList.BEGINSTALL, obj.nNPC);
						break;
					case STANDBY:		// 复活站立
						if(obj.nRoleId == GameCommonData.Player.Role.Id)
						{
							UIFacade.UIFacadeInstance.removeRelive();
							GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
							GameCommonData.Scene.AddPlayer(GameCommonData.Player);
							
							if(GameCommonData.Player.IsAutomatism && GameCommonData.PetID != 0)
							{
								  UIFacade.GetInstance(UIFacade.FACADEKEY ).sendNotification(AutoPlayPetCommand.NAME,{id: GameCommonData.PetID});
							}
						}
						if(GameCommonData.SameSecnePlayerList[obj.nRoleId] != null)
						{
							GameCommonData.SameSecnePlayerList[obj.nRoleId].SetAction(GameElementSkins.ACTION_STATIC);
							GameCommonData.Scene.AddPlayer(GameCommonData.SameSecnePlayerList[obj.nRoleId]);
							
							if(GameCommonData.TargetAnimal!=null && GameCommonData.TargetAnimal.Role.Id == GameCommonData.SameSecnePlayerList[obj.nRoleId].Role.Id)
							{
								GameCommonData.TargetAnimal.IsSelect(true);
							}
						}

						break;
					case actionTransPos:
						GameCommonData.Player.Role.Direction = obj.nDir			// 打回才有方向
						GameCommonData.Player.Role.TileX     = obj.nPosX;
						GameCommonData.Player.Role.TileY     = obj.nPosY;
						GameCommonData.Scene.StopPlayerMove(GameCommonData.Player);
						break;
					case CHANGE_MAP:
						GameCommonData.Player.Role.TileX = obj.nPosX;
						GameCommonData.Player.Role.TileY = obj.nPosY;
						GameCommonData.Scene.TransferScene(obj.nMapId,obj.nNPC.toString());
						break; 
					case OPEN_DIALOG:
						GameCommonData.NPCDialogIsOpen = true;
//						trace("open dialog",obj.nMapId,obj.nNPC);
						switch(obj.nMapId) {
							case 2: 															//NPC商店
								sendNotification(EventList.SHOWNPCSHOPVIEW);
								break;
							case 3:																//任务
								sendNotification(TaskCommandList.RECEIVE_TASK,{id:obj.nNPC});
								break;
							case 4:																//宠物单人繁殖
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:1});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETBREEDSINGLE); 
								break;
							case 5:																//宠物双人繁殖
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_DOUBLE_BREED, index:6});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETBREEDDOUBLE);
								break;
							case 6:																//宠物还童
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:0});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETTOBABY);
								break;
							case 7:																//宠物悟性提升
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:2});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETSAVVYUSEMONEY);
								break;
							case 8:																//宠物合成
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:3});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETSAVVYJOIN);
								break;
							case 9:																//宠物技能学习
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:4});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETSKILLLEARN);
								break;  
							case 10:															//宠物技能提升
								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:5});
//								sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, UIConstData.PETSKILLUP);
								break;
							case 11:																//创建帮派
								if ( NewUnityCommonData.closeUnity )
								{
									facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_7" ]/** = "帮派功能正在调试，暂时关闭";*/, color:0xffff00});	
									return;
								} 
								if(GameCommonData.Player.Role.unityId == 0)
								{
									UnityConstData.isOpenNpcView = true;							//NPC面板打开
									sendNotification(UnityEvent.SHOWCREATEUNITYVIEW);
									if(!dataProxy.BagIsOpen) {
										facade.sendNotification(EventList.SHOWBAG);
										dataProxy.BagIsOpen = true;
									}
								} 
								else
								{
									showHint( "net_ap_pa_proc_3" );
								}
//								facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "net_ap_pa_proc_3" ], color:0xffff00});    //"你已加入了帮派，不能再创建帮派"
								
								break;
							case 12:																//响应帮派
								if ( NewUnityCommonData.closeUnity )
								{
									facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_7" ]/** = "帮派功能正在调试，暂时关闭";*/, color:0xffff00});	
									return;
								} 
								UnityConstData.iscreating = int(GameCommonData.Player.Role.unityJob-1) / 100;
								if(GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0)
									showHint( "net_ap_pa_proc_4" );
//									facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "net_ap_pa_proc_4" ], color:0xffff00});		//"你已加入了帮派，不能再响应帮派"
								else
								{
									UnityConstData.isOpenNpcView = true;							//NPC面板打开
									sendNotification(UnityEvent.SHOWRESPONDUNITYVIEW);
								}
								break;
							case 13:																//申请入帮
								if ( NewUnityCommonData.closeUnity )
								{
									facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_7" ]/** = "帮派功能正在调试，暂时关闭";*/, color:0xffff00});	
									return;
								} 
								if(GameCommonData.Player.Role.unityId == 0)
								{
									UnityConstData.isOpenNpcView = true;							//NPC面板打开
									sendNotification(EventList.SHOWUNITYVIEW);
								} 
								else
								showHint( "net_ap_pa_proc_5" );
//								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "net_ap_pa_proc_5" ], color:0xffff00});	    //"你已加入了帮派，不能再申请帮派"	
								break;
							case 14:																//打开仓库
								if(!dataProxy.DepotIsOpen) {
									if(GameCommonData.Player.Role.Id) {
										facade.registerMediator(new DepotMediator());
										sendNotification(EventList.SHOWDEPOTVIEW);
									}
								} else {
									sendNotification(EventList.CLOSEDEPOTVIEW);
								}
								break;
							case 15:		//强化界面
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,0);
								break;
							case 16:		//升星界面
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,1);
								break;
							case 17:		//打孔界面
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,3);
								break;
							case 18:		//镶嵌界面
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,4);
								break;
							case 19:		//取出界面
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,5);
								break;
							case 20:		//人物技能学习（唐门）
								sendNotification(SkillConst.LEARNSKILL,{ID:1});
								break;
							case 21:		//人物技能学习（丐帮）
								sendNotification(SkillConst.LEARNSKILL,{ID:8});
								break;
							case 22:		//人物技能学习（少林）
								sendNotification(SkillConst.LEARNSKILL,{ID:16});
								break;
							case 23:		//人物技能学习（峨眉）
								sendNotification(SkillConst.LEARNSKILL,{ID:4});
								break;
							case 24:		//人物技能学习（全真）
								sendNotification(SkillConst.LEARNSKILL,{ID:2});
								break;
							case 25:		//人物技能学习（点苍）
								sendNotification(SkillConst.LEARNSKILL,{ID:32});
								break;
							case 26:       //宝石合成界面打开
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,2);
								break;
							case 27:       //答题界面
								sendNotification(EventList.DISPLAYANSWER,{id:obj.nNPC});
								break;
							case 28:      //闪动  obj.nNPC：1-人物, 2-技能, 3-宠物, 4-任务, 5-背包，6-好友,7-帮派, 8-组队, 9-活动, 10-功能, 11-大箭头指向任务追踪, 12-大箭头消失								
//								if(obj.nNPC > 0) sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, obj.nNPC); 
								if(obj.nNPC > 0) sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, obj.nNPC); 
								break;
							case 29:  //打开任务未完成面板
								sendNotification(EventList.SHOW_UNFINISH_TASK,{taskId:obj.nNPC});
								break;
							case 30: //门派传送命令
								sendNotification(EventList.TIMEUP,{taskId:obj.nNPC});
//								sendNotification(NPCChatComList.TRANSLATE_TO_UNITY,obj.nNPC);
								break;		
							case 31:  //元宝票交换面板
								if(!facade.hasMediator(StoneMoneyMediator.NAME))
								{
									facade.registerMediator(new StoneMoneyMediator());
								}
								sendNotification(StoneMoneyMediator.SHOW_STONEMONEY_UI);
								break;	
							case 32:		//生活技能学习
								sendNotification(SkillConst.LEARN_LIFE_SKILL_PAN); 
								break;
							case 33:		//倒计时面板
								sendNotification(TimeCountDownEvent.SHOWTIMECOUNTDOWN , {taskId:obj.nNPC}); 
								break;
							case 34:			//打开NPC跑商商店 
//								facade.registerMediator(new NPCBusinessMediator());
//								sendNotification(EventList.SHOW_NPC_BUSINESS_SHOP_VIEW);
								break;
							case 35:			//新手卡
								sendNotification(NewerCardData.SHOW_NEW_CARD_PAN);
								break;
//							case 36:		//单方解除师徒关系
//								sendNotification(MasterData.BETRAY_MASTER);
//								break; 
//							case 37:		//师徒双方协议解除师徒关系
//								sendNotification(RavelMasterCommand.NAME);
//								break;
							case 38:		//人民币头像
								facade.sendNotification(VipHeadIconData.LOADINT_VIPHEADICON_VIEW);
								break;
							case 39:     //17173投票卡
//								trace ( "脚本生效" );
//								if ( !facade.hasMediator( VoteCardMediator.NAME ) ) facade.registerMediator( new VoteCardMediator() );
//								sendNotification( VoteData.CLICK_GIFT_GIRL );
								if(!facade.hasMediator(NewerCardMediator.NAME))
								{
									facade.registerMediator(new NewerCardMediator());
								}
								facade.sendNotification(NewerCardMediator.SHOW_NEWER_CARD,{showNum:39,inputLength:18,sendNum:3,inputRule:"YJTP"});//sendNum 16 pptv新手卡发送编号
								break;
							case 40:   //魂印
								sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI,6);
								break;	
							case 41:	//媒体卡
								 if( !facade.hasMediator(NewSocietyCardMediator.NAME) ) 
								{
									facade.registerMediator( new NewSocietyCardMediator() );
								}
								sendNotification( NewSocietyCardData.SHOW_SOCIETY_CARD_PAN ); 
								break; 
							case 42:	//进入副本判断
								 if(!facade.hasCommand(GotoCopyCommand.NAME))
								 {
								 	facade.registerCommand(GotoCopyCommand.NAME,GotoCopyCommand)
								 }	
					 			 sendNotification(GotoCopyCommand.NAME,obj.nNPC);
								 break;
							case 43:
								sendNotification(WishData.WISH_OPEN);
								break;
							case 44:
								sendNotification(WishData.SEND_310); 
								break;
							case 45:
								if ( !facade.hasCommand( AgreementMasterCommand.NAME ) )
								{
									facade.registerCommand( AgreementMasterCommand.NAME,AgreementMasterCommand );
								}
								sendNotification( AgreementMasterCommand.NAME );
								break;
								/////开启魂魄相关面板  
							 case 46:	//提升扩展属性
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											if(SoulMediator.soulVO.extProperties[0])
											{
												var panel1:DisplayObject = (facade.retrieveMediator(ImproveExtendProMediator.NAME) as ImproveExtendProMediator).panelBase;
												if(!GameCommonData.GameInstance.GameUI.contains(panel1))
												{
													facade.sendNotification(ImproveExtendProMediator.SHOWVIEW,0);
												}
											}
										}
									}
								}
								break;  
							case 47:	//魂魄技能升级
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											if(SoulMediator.soulVO.soulSkills[0])
											{
												if((SoulMediator.soulVO.soulSkills[0] as SoulSkillVO).state == 0)
												{
													var panel2:DisplayObject = (facade.retrieveMediator(ImproveSkillMediator.NAME) as ImproveSkillMediator).panelBase;
													if(!GameCommonData.GameInstance.GameUI.contains(panel2))
													{
														facade.sendNotification(ImproveSkillMediator.SHOWVIEW,0);
													}
												}
												else
												{
													facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_1" ], color:0xffff00});//"暂无魂魄技能"
												}
											}
											else
											{
												facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_1" ], color:0xffff00});//"暂无魂魄技能"
											}
										}
									}
								}
								break;  
							case 48:	//提高成长率
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											var panel3:DisplayObject = (facade.retrieveMediator(GrowUpPercentMediator.NAME) as GrowUpPercentMediator).panelBase;
											if(!GameCommonData.GameInstance.GameUI.contains(panel3))
											{
												facade.sendNotification(GrowUpPercentMediator.SHOWVIEW,0);
											}
										}
									}
								}
								break;  
							case 49:	//学习魂魄扩展属性
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											if(SoulMediator.soulVO.composeLevel > 1)
											{
												var tag:int;
												for each(var obj1:Object in SoulMediator.soulVO.extProperties)
												{
													if(obj1 is SoulExtPropertyVO)
													{
														if((obj1 as SoulExtPropertyVO).state == 1)
														{
															tag = (obj1 as SoulExtPropertyVO).number;
															break;
														}
													}
												}
												if(tag)
												{
													var panel4:DisplayObject = (facade.retrieveMediator(LearnExtendProMediator.NAME) as LearnExtendProMediator).panelBase;
													if(!GameCommonData.GameInstance.GameUI.contains(panel4))
													{
														facade.sendNotification(LearnExtendProMediator.SHOWVIEW,tag);
													}
												}
												else
												{
													facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_2" ], color:0xffff00});//"暂无可学习的扩展属性槽"
												}
											}
											else
											{
												facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_2" ], color:0xffff00});//"暂无可学习的扩展属性槽"
											}
										}
									}
								}
								break;  
							case 50:	//重置魂魄扩展属性
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											var panel5:DisplayObject = (facade.retrieveMediator(RepeatExtendProMediator.NAME) as RepeatExtendProMediator).panelBase;
											if(!GameCommonData.GameInstance.GameUI.contains(panel5))
											{
												facade.sendNotification(RepeatExtendProMediator.SHOWVIEW,0);
											}
										}
									}
								}
								break;  
							case 51:	//重置魂魄技能
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											if(SoulMediator.soulVO.soulSkills[0])
											{
												if((SoulMediator.soulVO.soulSkills[1] as SoulSkillVO).state == 0)
												{
													var panel6:DisplayObject = (facade.retrieveMediator(RepeatSkillMediator.NAME) as RepeatSkillMediator).panelBase;
													if(!GameCommonData.GameInstance.GameUI.contains(panel6))
													{
														facade.sendNotification(RepeatSkillMediator.SHOWVIEW,0);
													}
												}
												else
												{
													facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_1" ], color:0xffff00});//"暂无魂魄技能"
												}
											}
											else
											{
												facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_1" ], color:0xffff00});//"暂无魂魄技能"
											}
										}
									}
								}
								break;  
							case 52:	//改变魂魄属相
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											if(SoulMediator.soulVO.composeLevel >= 5)
											{
												var panel7:DisplayObject = (facade.retrieveMediator(RepeatStyleMediator.NAME) as RepeatStyleMediator).panelBase;
												if(!GameCommonData.GameInstance.GameUI.contains(panel7))
												{
													facade.sendNotification(RepeatStyleMediator.SHOWVIEW,0);
												}
											}
											else
											{
												facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_3" ], color:0xffff00});//"暂无魂魄属相"
											}
										}
									}
								}
								break;   
							case 53:	//开启扩展属性槽
								{
									if(RolePropDatas.ItemList[15])
									{
										if(!SoulData.SoulDetailInfos[RolePropDatas.ItemList[15].id])
										{
											SoulProxy.getSoulDetailInfo();
										}
										else
										{
											if(SoulMediator.soulVO.composeLevel > 1)
											{
												var boo2:Boolean;
												for each(var obj2:Object in SoulMediator.soulVO.extProperties)
												{
													if(obj2 is SoulExtPropertyVO)
													{
														if((obj2 as SoulExtPropertyVO).state == 2)
														{
															boo2 = true;
															break;
														}
													}
												} 
												if(boo2)
												{
													var panel8:DisplayObject = (facade.retrieveMediator(UseExtendGrooveMediator.NAME) as UseExtendGrooveMediator).panelBase;
													if(!GameCommonData.GameInstance.GameUI.contains(panel8))
													{
														facade.sendNotification(UseExtendGrooveMediator.SHOWVIEW,0);
													}
												}
												else
												{
													facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_4" ], color:0xffff00});//"暂无可开启的扩展属性槽"
												}
											}
											else
											{
												facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "Net_ActionPro_PlayAction_soul_4" ], color:0xffff00});//"暂无可开启的扩展属性槽"
											}
										}
									}
								}
								break;
							case 54:
								if( !facade.hasMediator( TimelinesBox.NAME ) )
								{
									facade.registerMediator( new TimelinesBox() );
								}
								facade.sendNotification( TimelinesBoxData.SHOW_TIMELINESBOX_STONE, obj );
								break;
							case 55:
									if( !facade.hasMediator( TimelinesBox.NAME ) )
									{
										facade.registerMediator( new TimelinesBox() );
									}
									facade.sendNotification( TimelinesBoxData.SHOW_TIMELINESBOX_ARTIFACT, obj );
									break;
							case 56:
									if( !facade.hasMediator( TimelinesBox.NAME ) )
									{
										facade.registerMediator( new TimelinesBox() );
									}
									facade.sendNotification( TimelinesBoxData.SHOW_TIMELINESBOX_ARTIFACT3, obj );
									break;  
							case 57:	//360抽奖活动
								if(!facade.hasMediator(LotteryCardFor360Mediator.NAME))
								{
									facade.registerMediator(new LotteryCardFor360Mediator());
								}
								sendNotification(LotteryCardFor360Mediator.SHOW_LOTTERYCARD_PANEL);
								break;    
							case 58:	//台服新手卡
								if(GameCommonData.wordVersion == 2)
								{
									if(!facade.hasMediator(NewCardForTaiWanMediator.NAME))
									{
										facade.registerMediator(new NewCardForTaiWanMediator());
									}
									else
									{
										sendNotification(NewCardForTaiWanMediator.SHOW_TW_CARD_PANEL);
									}
								}
								break;  
							case 59:	//台服虚拟产包
								if(GameCommonData.wordVersion == 2)
								{
									if(!facade.hasMediator(StoreMoneyCardMediator.NAME))
									{
										facade.registerMediator(new StoreMoneyCardMediator());
									}
									else
									{
										sendNotification(StoreMoneyCardMediator.SHOW_STORE_MONEY_CARD);
									}
								}
								break;
							case 60: 	//pptv新手卡
								if(!facade.hasMediator(NewerCardMediator.NAME))
								{
									facade.registerMediator(new NewerCardMediator());
								}
								facade.sendNotification(NewerCardMediator.SHOW_NEWER_CARD,{showNum:60,inputLength:16,sendNum:16});//sendNum 16 pptv新手卡发送编号
								break;
							 case 61:	//补偿仓库
								CompensateStorageData.getCompensateList();
								break;
							case 62: //宠物玩耍
								if(!facade.hasMediator(PetPlayMediator.NAME))
								{
									facade.registerMediator(new PetPlayMediator());
								}
								sendNotification(PetEvent.SHOW_PET_PLAY_VIEW);
								break;   
							case 63:	//台服中信酷碰包
								if(!facade.hasMediator(NewerCardNewMediator.NAME))
								{
									facade.registerMediator(new NewerCardNewMediator());
								}
								facade.sendNotification(NewerCardNewMediator.SHOW_NEWER_CARD_NEW,63);//中信酷碰包
								break;
							case 64:	//台服好禮回饋卡
								if(!facade.hasMediator(NewerCardNewMediator.NAME))
								{
									facade.registerMediator(new NewerCardNewMediator());
								}
								facade.sendNotification(NewerCardNewMediator.SHOW_NEWER_CARD_NEW,64);
								break;
							case 65: //台服會員體驗卡
								if(!facade.hasMediator(NewerCardNewMediator.NAME))
								{
									facade.registerMediator(new NewerCardNewMediator());
								}
								facade.sendNotification(NewerCardNewMediator.SHOW_NEWER_CARD_NEW,65);
							break;
							case 67:	//装备分解
								facade.sendNotification(CloseAllViewCommand.NAME);
								if(!facade.hasMediator(ArtificeMediator.NAME))
								{
									facade.registerMediator(new ArtificeMediator());
								}
								facade.sendNotification(ArtificeConst.SHOW_ARTIFICE_VIEW);
								break;

							case 66: 
								sendNotification(CloseAllViewCommand.NAME);
								sendNotification(CastSpiritData.SHOW_CASTSPIRIT_VIEW);
								break;
							case 68:
								sendNotification(CloseAllViewCommand.NAME);
								sendNotification(CastSpiritData.SHOW_CASTSPIRIT_UP_VIEW);
								break;
							case 69:
								sendNotification(CloseAllViewCommand.NAME);
								sendNotification(CastSpiritData.SHOW_CASTSPIRIT_TRANSFER_VIEW);
								break;
							case 70:	//装备强化转移
								facade.sendNotification(CloseAllViewCommand.NAME);
								if(!facade.hasMediator(StrengthenTransferMediator.NAME))
								{
									facade.registerMediator(new StrengthenTransferMediator());
								}
								facade.sendNotification(StrengthenTransferConst.SHOW_STRENGTHENTRANSFER_VIEW);
								break;
						}
						break;
					case PET_RENAME_FAILE:										//宠物改名失败  2010.9.3 冯昌权 
						sendNotification(PetEvent.PET_RENAME_FAIL, obj.nRoleId);
						break;
					case PET_SKILL_LEARN_FAIL:									//宠物遗忘了某技能
						var skillId:uint = obj.nMapId;
						var petId:uint = obj.nRoleId;
						var name:String = (GameCommonData.SkillList[skillId] as GameSkill).SkillName;
						if(name) {
							if ( GameCommonData.wordDic )
							{
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "net_ap_pa_proc_6" ]+"["+name+"]", color:0xffff00});  //"宠物遗忘了技能"
							}
							sendNotification(PetSkillLearnEvent.FORGET_SKILL_PET, {petId:petId, skillId:skillId})
						}
						break;
					case 236:		// 进入新场		
						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,140);	    
						/* if(GameCommonData.isFirstTimeEnterGame) 
						{
							GameCommonData.isFirstTimeEnterGame = false;
							trace("收到236：请求背包")
							//todo 加定时器（进行第一次的背包物品的请求）
							RepeatRequest.getInstance().bagItemCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("bag",NetAction.RequestItems,2000);
							//  end to do
							
							NetAction.RequestItems();
						} */
//						if(GameCommonData.TargetScene.length > 0)
//						{	
//							if(GameCommonData.TargetScene != GameCommonData.GameInstance.GameScene.GetGameScene.name)
//							{	
//								GameCommonData.Scene.MoveNextScenePoint();
//							}
//							else
//							{
//								GameCommonData.Scene.playerdistance = GameCommonData.TargetDistance;
//								GameCommonData.Scene.PlayerMove(MapTileModel.GetTilePointToStage(GameCommonData.TargetPoint.x,GameCommonData.TargetPoint.y));
//								GameCommonData.TargetScene = "";
//							}
//						}   
//						GameCommonData.Scene.IsCanController = true;
						break;
					case PK_SWITCH:
						facade.sendNotification(PkEvent.UPDATEDATA , obj.nPosX);
//						facade.sendNotification(PkEvent.GETCDTIME , obj.nPosY); 
						break; 
					case HEARTPOINT:		//游戏心跳
						var newTime:int = getTimer();
						if(newTime > GameCommonData.netDelayStartTime)
						{
							GameCommonData.netDelayTime = newTime - GameCommonData.netDelayStartTime;
						}
						if(!ChgLineData.isChooseLine)
						{
							var d1:Date = new Date(GameCommonData.gameYear,
								GameCommonData.gameMonth - 1,
								GameCommonData.gameDay,
								GameCommonData.gameHour,
								GameCommonData.gameMinute,
								GameCommonData.gameSecond);
							var d2:Date = new Date(obj.nTimeStamp,
								obj.nPosX - 1,
								obj.nPosY,
								obj.nDir,
								obj.nMapId,
								obj.nNPC);
							
							if ( isSpeedCloth )
							{
								if(!TimerNewComponent.isInstanceInited)
								{
									TimerNewComponent.getInstance(d2);
								} 
								TimerNewComponent.updateTag ++;
								  
								if(TimerNewComponent.updateTag%12 == 0)
								{
									TimerNewComponent.getInstance().modifyDate(d2);
								}
								var c:int = d2.time - GameCommonData.nowDate.time;
								if(c/10 < -2000)
								{
									GameCommonData.GameNets.endGameNet();
								} 
							}
							
							UIFacade.GetInstance(UIFacade.FACADEKEY).gameHeartPoint(null,1);
							GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOW_TIME, 
								[obj.nTimeStamp, obj.nPosX, obj.nPosY, obj.nDir, obj.nMapId, obj.nNPC]); 	//服务器时间
						
//							trace(obj.nTimeStamp, obj.nPosX , obj.nPosY, obj.nDir, obj.nMapId, obj.nNPC)
						}
						break;
					case PET_SKILL_LEARN:		//宠物学习技能反馈
						var skillIdLearn:uint = obj.nMapId;
						var petIdLearn:uint = obj.nRoleId;
						if(skillIdLearn > 0) {
							var nameLearn:String = (GameCommonData.SkillList[skillIdLearn] as GameSkill).SkillName;
							if(nameLearn && GameCommonData.wordDic) facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "net_ap_pa_proc_7" ]+"["+nameLearn+"]", color:0xffff00});      //"宠物学会了技能"
						}
						facade.sendNotification(PetSkillLearnEvent.LEARN_SKILL_SUCCESS_PET, {id:petIdLearn, flag:skillIdLearn});
						break;
					case PET_SKILL_UP:			//宠物提升技能反馈
						var skillIdUp:uint = obj.nMapId;
						var petIdUp:uint = obj.nRoleId;
						if(skillIdUp > 0) {
							var nameUp:String = (GameCommonData.SkillList[skillIdUp] as GameSkill).SkillName;
							if(nameUp && GameCommonData.wordDic) facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "net_ap_pa_proc_8" ]+"["+nameUp+"]", color:0xffff00});      //"宠物技能提升到"
						}
						facade.sendNotification(PetSkillUpEvent.UP_SKILL_SUCCESS_PET, {id:petIdUp, flag:skillIdUp});
						break;
					case PET_TO_BABY:			//宠物还童反馈
						var petIdBaby:uint = obj.nMapId;
						sendNotification(PetToBabyEvent.PET_TO_BABY_RETURN, petIdBaby);
						break;
					case PET_SURE_BREED:		//宠物单繁反馈
						var petSingleFlag:uint = obj.nMapId;
						sendNotification(PetBreedSingleEvent.PET_BREED_SINGLE_RETURN, petSingleFlag);
						break;
					case PET_SAVVY_USEMONEY:	//宠物提升悟性反馈
						var petIdSavvyMoney:uint = obj.nMapId;
						sendNotification(PetSavvyUseMoneyEvent.PET_SAVVY_USE_MONEY_RETURN, petIdSavvyMoney);
						break;
					case PET_SAVVY_JOIN:		//宠物合成反馈
						var petIdSavvyJoin:uint = obj.nMapId;
						sendNotification(PetSavvyJoinEvent.PET_SAVVY_JOIN_RETURN, petIdSavvyJoin);
						break;
					case NEWPET_FANTASY_TAG:	//宠物幻化反馈
						var petIdFantasy:int = obj.nMapId;
						sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:petIdFantasy, ownerId:GameCommonData.Player.Role.Id});
						sendNotification(PetEvent.PET_FANTASY_FEEDBACK);
					break;
					case NEWPET_WINNING_TAG:	//宠物灵性提示反馈
						var petIdWinning:int = obj.nMapId;
						sendNotification(PetWinningEvent.PET_WINNING_FEEDBACK,petIdWinning);
					break;
					case NEWPET_PRIVITY_TAG:	//宠物默契提升反馈
						var petIdPrivity:int = obj.nMapId;
						sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:petIdPrivity, ownerId:GameCommonData.Player.Role.Id});
						sendNotification(PetEvent.PET_PRIVITY_FEEDBACK);
					break;
					case NEWPET_DEPENDENCE_TAG:	//宠物附体反馈
						var petIdDependence:int = obj.nMapId;
						sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:petIdDependence, ownerId:GameCommonData.Player.Role.Id});
					break;
					case NEWPET_CANCEL_DEPENDENCE_TAG:	//宠物分离反馈
						var petIdCancelDependence:int = obj.nMapId;
						sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:petIdCancelDependence,ownerId:GameCommonData.Player.Role.Id});
					break;
					case 240: //NPC对话解锁
//						trace("NPC对话解锁");
						DelayOperation.getInstance().unLockNpcTalk();
						break;
						
					case 28:
						RepeatRequest.getInstance().skillItemCount=0;
						RepeatRequest.getInstance().cdCount=0;
						RepeatRequest.getInstance().taskCount=0;
						break;	
					case 269://宠物确认信息
						RepeatRequest.getInstance().removeDelayProcessFun("pet");	
						if(obj.nMapId==RepeatRequest.getInstance().petItemCount){
							//todo  成功
							if(RepeatRequest.getInstance().successFlags[1]){
                         		return;
                         	}else{
//								trace("获得宠物信息成功");
                         		RepeatRequest.getInstance().successFlags[1]=true;
                         	}
							RepeatRequest.getInstance().skillItemCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("skill",NetAction.requestSkill,20000); 
							NetAction.requestSkill();
						}else{
							RepeatRequest.getInstance().addDelayProcessFun("pet",NetAction.requestPet,20000);
							RepeatRequest.getInstance().petItemCount=0;
							NetAction.requestPet(); 
						}
						break;
					case 270://请求技能信息
						RepeatRequest.getInstance().removeDelayProcessFun("skill");
						if(obj.nMapId==RepeatRequest.getInstance().skillItemCount){
//							trace("获得拔能信息成功");
							if(RepeatRequest.getInstance().successFlags[2]){
                         		return;
                         	}else{
                         		RepeatRequest.getInstance().successFlags[2]=true;
                         	}
							RepeatRequest.getInstance().quickKeyCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("quickKey",NetAction.requestQuickKey,20000);
							NetAction.requestQuickKey();
						}else{
							RepeatRequest.getInstance().skillItemCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("skill",NetAction.requestSkill,20000);
							NetAction.requestSkill();
						}
						break;	
					case 271: //请求快捷键信息
						RepeatRequest.getInstance().removeDelayProcessFun("quickKey");
						if(RepeatRequest.getInstance().quickKeyCount>0){
//							trace("获得快捷键信息成功");
							if(RepeatRequest.getInstance().successFlags[3]){
                         		return;
                         	}else{
                         		RepeatRequest.getInstance().successFlags[3]=true;
                         	}
						
							RepeatRequest.getInstance().cdCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("cd",NetAction.requestCd,20000);
							NetAction.requestCd();
						}else{
							RepeatRequest.getInstance().quickKeyCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("quickKey",NetAction.requestQuickKey,20000);
							NetAction.requestQuickKey();
						}
						break;	
					case 272:  //请求CD信息
						RepeatRequest.getInstance().removeDelayProcessFun("cd");
						if(RepeatRequest.getInstance().cdCount==obj.nMapId){
//							trace("获得cd信息成功");
							if(RepeatRequest.getInstance().successFlags[4]){
                         		return;
                         	}else{
                         		RepeatRequest.getInstance().successFlags[4]=true;
                         	}
							
							//cd 成功后请求236
							GameCommonData.Scene.SendOnLoadComplete();
							GameCommonData.Scene.IsFirstLoad = true;
							
							if(PetPropConstData.needRequestPetInfo.needToReq) {	//刚上线时如果有出战宠物，需在发236之后再发送请求出战宠物的详细信息 才能获得宠物技能=信息
								sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetPropConstData.needRequestPetInfo.petId, ownerId:GameCommonData.Player.Role.Id});
							}
							//请求组队信息
							sendNotification(EventList.ASKTEAMINFO, obj); 
							
							RepeatRequest.getInstance().taskCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("task",NetAction.requestTask,20000);
							NetAction.requestTask();

						}else{
							RepeatRequest.getInstance().cdCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("cd",NetAction.requestCd,20000);
							NetAction.requestCd();
						}
						break;
					case 273: //请求任务
						RepeatRequest.getInstance().removeDelayProcessFun("task");
						if(RepeatRequest.getInstance().taskCount==obj.nMapId){
//							trace("任务接收完成");
							if(RepeatRequest.getInstance().successFlags[5]){
                         		return;
                         	}else{
                         		RepeatRequest.getInstance().successFlags[5]=true;
                         		sendNotification(EventList.PLAYER_TASK_LEVEL);
                         	}
							
							RepeatRequest.getInstance().taskCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("other",NetAction.requestOther,20000);
							NetAction.requestOther();
						}else{
							RepeatRequest.getInstance().taskCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("task",NetAction.requestTask,20000);
							NetAction.requestTask();
						}
						break;	
						
					case 274:
						RepeatRequest.getInstance().removeDelayProcessFun("other");
						if(RepeatRequest.getInstance().otherCount==1){
//							trace("获得其它信息成功");
							if(RepeatRequest.getInstance().successFlags[6]){
                         		return;
                         	}else{
                         		RepeatRequest.getInstance().successFlags[6]=true;
                         	}
                         	RepeatRequest.getInstance().clearAllInterval();
							FriendSend.getInstance().sendAction(FriendSend.getInstance().getFriendListParam()); //请求好友
							facade.sendNotification(RoleEvents.GETFITOUTBYBAG);
							NetAction.requestBuff();
							NetAction.requestAutoPlay();
							NetAction.requestDesignation();
							NetAction.requestMeridiansData();
							PlayerActionSend.PlayerAction({type:1010,data:[0,0,0,0,0,0,277,0,0]});				//发送打开离线挂机请求
							UnityActionSend.SendSynAction({type:1107 , data:[0 , 0 , 208, 0 , 0]});					//请求帮派详细信息
							NewAwardMediator.SIMBOL_TAG = true;  	//发送新手成就信息后标记
							PlayerActionSend.PlayerAction({type:1010 , data:[0 , 0 , 0 , 0 , 0 , 0 , 284, 0 , 0]});	//请求新手成就大礼包数据
							TreasureNet.requestTreaPack();
							facade.sendNotification(PkEvent.ADDUPDATE);												//一上线就开始PK倒计时 
							UIFacade.sendHeartPoint();																//发送心跳包
							sendNotification(PreventWallowEvent.CHECT_IS_OPEN_FCM);									//检查是否启动防沉迷
							PlayerActionSend.PlayerAction({type:1010,data:[0,0,0,0,0,0,CLIENT_INIT_ALL_COMP,0,0]});
//							if(GameCommonData.Player && GameCommonData.Player.Role && GameCommonData.Player.Role.Id) TeamNetAction.sendTeamOrder(TeamAction._MSGTEAM_ASKINFO, GameCommonData.Player.Role.Id);
							var equMed:EquipMediator = facade.retrieveMediator(EquipMediator.NAME) as EquipMediator;
							equMed.playerAttribute.checkLevUp();	//检查升级信息
						}else{
							RepeatRequest.getInstance().otherCount=0;
							RepeatRequest.getInstance().addDelayProcessFun("other",NetAction.requestOther,20000);
							NetAction.requestOther();
						}
						break;	
						
						//注释的东西不要删掉
					case MASTER_OK:
						var masterName:String = bytes.readMultiByte(16,GameCommonData.CODE);
//						facade.sendNotification(ReceiveMasterCommand.NAME,{sName:masterName});	
					break;
						
					case GRADUATE_OK:
						facade.sendNotification(TutorGraduateCommand.NAME);	
					break;
							
					case RELEASE_MASTER:
						var maName:String = bytes.readMultiByte(16,GameCommonData.CODE);
						facade.sendNotification(ReLeaseMasterCommand.NAME,{sName:maName});
					break;
											
					case 277:
							facade.sendNotification(AutoPlayEventList.GET_OFFLINEPLAYDATA , obj.nMapId);
					break;
					
					case START_LIFE_SEEK:
						SkillData.isLifeSeeking = true;
							//头上飘字：正在采集。。。。
						GameCommonData.Player.SetMissionPrompt(4);
					break;
					
					case STOP_LIFE_SEEK:
						SkillData.isLifeSeeking = false;
						GameCommonData.Player.SetMissionPrompt(0);
							//停止采集，去掉头上飘字					
					break;
					//圣诞快乐   （任务捐装备）
					case 296:
					  if(obj.nMapId == 1 || obj.nMapId == 2 || obj.nMapId == 3)
					  {
						   player = PlayerController.GetPlayer(obj.nRoleId);
						   FloorEffectController.AddFlootEffect(player,obj.nMapId);
					  }
					  if(obj.nMapId == 650003)
					  {
					  	   FlutterController.SetRose(); 
					  }
					  /* if(10 < obj.nMapId < 17){
					  	sendNotification(TaskCommandList.SHOW_COLLECT_SPECIAL,obj.nMapId);
					  } */
					break;  
					//领取离线经验成功
					case 303:
						facade.sendNotification(AutoPlayEventList.GET_OFFLINEAWARD , true);
					break;
					//领取离线经验失败
					case 304:
						facade.sendNotification(AutoPlayEventList.GET_OFFLINEAWARD , false);
					break;
					case 311:
						facade.sendNotification(WishData.RETURN_CLICK_GRID,obj);
					break;
					//小鑫的许愿
					case 315:
						sendNotification( PrepaidUIData.SHOW_WISH_VIEW, obj.nDir );
					break;
					case 310:
						facade.sendNotification(WishData.OPEN_RETURN_GRID,obj);
					break;
					//经脉升级完成 /* ok */
					case 148:
						(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nState = 3;
						facade.sendNotification(MeridiansEvent.COMPLETE_MERIDIANS_UPGRADE,obj.nPosX);
						break;
					//经脉自动开始修炼
					case 149:
						MeridiansTimeOutComponent.getInstance().addFun1("upDataTime",MeridiansData.upDataTime);
						
						(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nState = 2;
						facade.sendNotification(MeridiansEvent.AUTO_MERIDIANS_LEARN,obj.nPosX);
						break;
					//经脉操作结果
					case 150:
//						facade.sendNotification(MeridiansEvent.RESULT_MERIDIANS_OPERATE,{type:obj.nPosX,time:obj.nPosY});
						switch(obj.nDir)
						{
							//加入等待队列失败 
							case 0:
							
								facade.sendNotification(MeridiansEvent.RESULT_ADDWAITQUEUE_FAIL,obj.nPosX);
								break;
							//加入等待队列成功  /* 需要： 加入队列时间、 修炼时需要时间*/
							case 1:
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nState = 1;
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nLeaveTime = obj.nMapId;
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nOrderTimer = obj.nNPC;
								facade.sendNotification(MeridiansEvent.RESULT_ADDWAITQUEUE_SUC,obj.nPosX);
								break;
							//开始修炼失败
							case 2:       /* ok */
								facade.sendNotification(MeridiansEvent.RESULT_STARTLEARN_FAIL,obj.nPosX);
								break;
								//开始修炼成功 /* 需要：  */
							case 3:
								MeridiansTimeOutComponent.getInstance().addFun1("upDataTime",MeridiansData.upDataTime);
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nState = 2;
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nLeaveTime = obj.nMapId;
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nOrderTimer = obj.nNPC;
								facade.sendNotification(MeridiansEvent.RESULT_STARTLEARN_SUC,obj.nPosX);
								break;
								 //升级失败 加速后的返回结果
							case 4:
								facade.sendNotification(MeridiansEvent.RESULT_UPLEV_FAIL,obj.nPosX);
								break;
								//升级成功 完全加速 直接升级完成  /* ok */
							case 5:
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nState = 0;
								
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nLev++;
								MeridiansData.upDataNAllLevGrade();
								
								facade.sendNotification(MeridiansEvent.RESULT_UPLEV_SUC,obj.nPosX);
								break;
								//经脉加速 /* 需要： 剩余时间 */
							case 6:
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nLeaveTime = obj.nMapId;
								
								facade.sendNotification(MeridiansEvent.RESULT_UPLEV_PART,obj.nPosX);
								break;
								//强化失败 /*  */
							case 7:
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nStrengthLev = obj.nPosY;
								MeridiansData.upDataNAllStrengthLevAdd();
								facade.sendNotification(MeridiansEvent.RESULT_STRENGTHJINMEI_FAIL,obj.nPosX);
								break;
								//强化成功
							case 8:
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nStrengthLev++;
								MeridiansData.upDataNAllStrengthLevAdd();
								
								facade.sendNotification(MeridiansEvent.RESULT_STRENGTHJINMEI_SUC,obj.nPosX);
								break;
								//移除失败
							case 9:
								facade.sendNotification(MeridiansEvent.RESULT_MOVEWAITQUEUE_FAIL,obj.nPosX);
								break;
								 //移除成功 /* ok */
							case 10:
								(MeridiansData.meridiansVO.meridiansArray[obj.nPosX - 1] as MeridiansTypeVO).nState = 0;
								facade.sendNotification(MeridiansEvent.RESULT_MOVEWAITQUEUE_SUC,obj.nPosX);
								break;
						}
						break;
					//可以同时修炼和等待的经脉数目
					case 151:
						facade.sendNotification(MeridiansEvent.INIT_LRARN_NUM,obj.nPosX);
						break;
						//领取真元结果 0 失败 1 成功
					case 152:
						switch(obj.nPosX)
						{
							case 0:
								facade.sendNotification(MeridiansEvent.GET_ARCHEAUS_FAIL);
								break;
							case 1:
								facade.sendNotification(MeridiansEvent.GET_ARCHEAUS_SUC);
								break;
						}
						break;
					case DEAL_AFTER_COMOSE_SOUL_STONE:
						facade.sendNotification(EventList.SOUL_STONE_COMPOSE_SUCCED);
						break;
				}
		}
		
		private function showHint( hs:String ):void
		{
			if ( GameCommonData.wordDic[ hs ] )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ hs ], color:0xffff00});	
			}
		}
	}
}