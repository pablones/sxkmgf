package Net.ActionProcessor
{
	import Controller.CombatController;
	import Controller.PKController;
	import Controller.PlayerController;
	import Controller.PlayerSkinsController;
	import Controller.TargetController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.UI.StallSkinManager;
	import GameUI.Modules.Task.Model.TaskProxy;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownEvent;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.UICore.UIFacade;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Role.SkinNameController;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementBanner;
	import OopsEngine.Scene.StrategyElement.Person.GameElementEnemy;
	import OopsEngine.Scene.StrategyElement.Person.GameElementNPC;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	import OopsEngine.Scene.StrategyElement.Person.GameElementStall;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	import flash.utils.ByteArray;
	
	/** 
	 * 场景可视区有怪刷怪
	 * 场景可视区有玩家刷玩家
	 */
	public class PlayerInfo extends GameAction
	{
		public function PlayerInfo(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}

		public override function Processor(bytes:ByteArray):void
		{
			if(GameCommonData.SameSecnePlayerList == null) return;
//			try
//			{             
				bytes.position = 4;
				var playerInfoObj:Object 	= new Object();
				playerInfoObj.roleId 		= bytes.readUnsignedInt(); 			//玩家ID
				playerInfoObj.usLookFace 	= bytes.readUnsignedShort();		//玩家头像
				playerInfoObj.usLevel 		= bytes.readUnsignedShort();		//玩家等级
				playerInfoObj.usMapID		= bytes.readUnsignedInt();			//地图ID
				playerInfoObj.usPosX 		= bytes.readUnsignedShort();		//玩家X位置
				playerInfoObj.usPosY 		= bytes.readUnsignedShort();		//玩家Y位置
				playerInfoObj.ucDir 		= bytes.readUnsignedShort();		//玩家方向
				playerInfoObj.pkValue  		= bytes.readUnsignedShort();		//PK值（只有玩家有用）
				playerInfoObj.isHidden 		= bytes.readUnsignedByte();			//是否隐身
				playerInfoObj.sex           = bytes.readUnsignedByte();			//性别
				bytes.readUnsignedShort();
				playerInfoObj.maxHP  		= bytes.readUnsignedInt();			//最大血量
				playerInfoObj.currentHP 	= bytes.readUnsignedInt();			//当前血量
				playerInfoObj.maxMP 		= bytes.readUnsignedInt();			//最大魔法
				playerInfoObj.currentMP 	= bytes.readUnsignedInt();			//当前魔法
				
				///////////////
				//下面两个是后添加 6.18（冯昌权）
				playerInfoObj.maxSP 		= bytes.readUnsignedInt();			//最大努气
				playerInfoObj.currentSP 	= bytes.readUnsignedInt();			//当前努气
				
				playerInfoObj.dwMountType	= bytes.readUnsignedInt();			//坐骑
				playerInfoObj.dwArmorType 	= bytes.readUnsignedInt();			//武器
				playerInfoObj.dwDressType 	= bytes.readUnsignedInt();			//时装
				playerInfoObj.dwWingType 	= bytes.readUnsignedInt();			//翅膀(暂没用）
				playerInfoObj.dwProfession 	= bytes.readUnsignedInt();			//职业
				playerInfoObj.idOwner 		= bytes.readUnsignedInt();			//所属人的ID（宠物）
				playerInfoObj.dwAttackUser 	= bytes.readUnsignedInt();			//怪物攻击模式（主动攻击怪、被动攻击）
				playerInfoObj.idMonsterType = bytes.readUnsignedInt();			//怪物的种类（怪物的皮肤判断，和XML关联相关数据）
				///////////////
				//下面两个是后添加 6.18（冯昌权）
				playerInfoObj.idTeam 		= bytes.readUnsignedInt();			//玩家队伍编号（如果没队伍编号为0）
				playerInfoObj.idTeamLeader	= bytes.readUnsignedInt();			//玩家所在队伍中的队长编号
				
				playerInfoObj.unityId 		= bytes.readUnsignedInt();			//玩家所在帮会ID
				playerInfoObj.unityJob 		= bytes.readUnsignedInt();			//玩家所在帮会职位
				
				if(playerInfoObj.roleId >= 2000000000 && playerInfoObj.roleId <= 3999999999)		
				{
    				playerInfoObj.idBooth		= 0;		
    				playerInfoObj.variation		= bytes.readUnsignedInt() - 1;				
    			}
    			else
    			{
    				playerInfoObj.idBooth		= bytes.readUnsignedInt();		
    				playerInfoObj.variation	 = 0;			
    			}
    			
    			playerInfoObj.vip 	        = bytes.readUnsignedInt();			//是否为VIP（0不是VIP，1为VIP）
    			playerInfoObj.IsShowDress 	= bytes.readUnsignedInt();	        //是否显示时装
    				
				var nDataSeeNum:int = bytes.readByte();
				var nDataSee:int = 0;
				var isChangeSkinsState:int = 0;	
				
				var currScene:String = playerInfoObj.usMapID;
				
				// 过滤不该是本场景显示的怪
		        if(GameCommonData.GameInstance.GameScene.GetGameScene!=null && currScene !=  GameCommonData.GameInstance.GameScene.GetGameScene.MapId)
		        {
		        	return;
		        }
				
				for(var i:int = 0;i < nDataSeeNum; i ++)
				{
					nDataSee = bytes.readByte();
					if(nDataSee != 0)
					{
						if(i == 0)
						{
							playerInfoObj.playerName = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); 	//玩家姓名
						}
						else if(i == 1)
						{
							playerInfoObj.Feel = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);			//心情
						}
						else if(i == 2)
						{
							playerInfoObj.teanName = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);		 //小队名称
						}
						else if(i == 3)
						{
							playerInfoObj.ownerName = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);	 //拥有者的名字
						}
					}		
				}
				
				/** 初始化上线玩家、NPC、怪物信息 */
				var player:GameElementAnimal;
				if(GameCommonData.SameSecnePlayerList[playerInfoObj.roleId]==null && 
				   GameCommonData.Player.Role.Id != playerInfoObj.roleId)						  	 // 新玩家上线
				{
					if(playerInfoObj.roleId >= 1 && playerInfoObj.roleId <= 299999)				 	 // NPC
					{
						if(playerInfoObj.usLookFace==30000)											
						{
							player 			 = new GameElementStall(GameCommonData.GameInstance); 	 				// 摊位
							player.Role		 = new GameRole();
							player.Role.Type = GameRole.TYPE_STALL;
						}
						else
						{
							player 		= new GameElementNPC(GameCommonData.GameInstance); 	  						// NPC
							player.Role = new GameRole();
							var npc:XML = GameCommonData.ModelOffsetNPC[playerInfoObj.idMonsterType.toString()];
							if(npc!=null)
							{
								player.Offset 			   = new Point(npc.@X,npc.@Y);
								player.OffsetHeight 	   = npc.@H;
								player.Role.Direction 	   = npc.@Dir;
								player.Role.Title		   = npc.@Title;
								player.Role.IsSimpleNPC = (npc.@Simple == "true");
								player.Role.PersonSkinName = "Resources\\NPC\\" + npc.@Swf + ".swf";
							}
							
							player.Role.MissionState = TaskProxy.getInstance().getNpcShowTaskType(playerInfoObj.roleId);
							
							player.Role.TitleColor       = 0x4affd2;
							player.Role.TitleBorderColor = 0x004940;
							player.Role.NameColor		 = "#46f0ff";
							player.Role.NameBorderColor  = 0x000000;
							player.Role.Type 		     = GameRole.TYPE_NPC;
						}
					}
					else if(playerInfoObj.roleId >= 400001 && playerInfoObj.roleId <= 699999)		  				// 怪物
					{
						if(playerInfoObj.idMonsterType ==9999)											
						{
							player 			 = new GameElementBanner(GameCommonData.GameInstance); 	 				// 旗帜
							player.Role		 = new GameRole();
							player.Role.Type = GameRole.TYPE_BANNER;
						}
						else
						{
							player 			 		   = new GameElementEnemy(GameCommonData.GameInstance);
							player.Role 			   = new GameRole()
							player.Role.NameColor	   = "#ffac63";
							var enemy:XML = GameCommonData.ModelOffsetEnemy[playerInfoObj.idMonsterType.toString()];
							if(enemy!=null)
							{
								player.Offset			   = new Point(enemy.@X,enemy.@Y);
								player.OffsetHeight		   = enemy.@H;
								player.Role.Title		   = enemy.@Title;
								player.Role.PersonSkinName = "Resources\\Enemy\\" + enemy.@Swf + ".swf";
							    player.Role.PersonSkinID   = enemy.@Swf;
								if(enemy.@Type==3 && (player.Role.Title == GameCommonData.wordDic[ "net_ap_pi_proc_1" ] || player.Role.Title == GameCommonData.wordDic[ "net_ap_pi_proc_2" ]))			// 首领头目主动怪的标识     "头目"      "首领"
								{
									player.Role.NameColor = "#ff00ff";
								}
								else if(enemy.@Type!=3 && (player.Role.Title == GameCommonData.wordDic[ "net_ap_pi_proc_1" ] || player.Role.Title == GameCommonData.wordDic[ "net_ap_pi_proc_2" ]))		// 首领头目非主动怪的标识
								{
									player.Role.NameColor = "#CC6600";
								}
								else if(enemy.@Type==3)																		// 普通主动怪的标识
								{
									player.Role.NameColor = "#ED3333";
								}
							}
							player.Role.idMonsterType    = playerInfoObj.idMonsterType;
							player.Role.Type 		     = GameRole.TYPE_ENEMY;
							player.Role.TitleColor       = 0xebd793;
							player.Role.TitleBorderColor = 0x351001;
							player.Role.NameBorderColor  = 0x220c00;
							player.Role.Direction	     = playerInfoObj.ucDir;
	
							player.SetMoveSpend(1);
						}
					}
					else if(playerInfoObj.roleId >= 2000000000 && playerInfoObj.roleId <= 3999999999)		  //宠物  10.7.19冯
					{
						player = GameCommonData.SameSecnePlayerList[playerInfoObj.roleId];
						if(player==null)player     = new GameElementPet(GameCommonData.GameInstance);
						player.Role 	 		   = new GameRole();
						player.Role.Savvy          = playerInfoObj.IsShowDress; 
					    var petData:XML;
					    
					    // 判断是是变异的宠物 
					    if(playerInfoObj.variation > 0)
					    {
					    	// 获取变异信息
					    	var xml:XML =PlayerSkinsController.GetPetV(playerInfoObj.idMonsterType.toString(),playerInfoObj.variation);			    	
					    	if(xml != null)
					    	{
					    		// 获取宠物信息
					    		var bianYiPet:String = xml.@M;
					    		petData = GameCommonData.ModelOffsetEnemy[ bianYiPet ]; 
					    	}
					    }
					    
					    // 如果不是变异
					    if(petData == null)
					    {
					    	// 通过悟性来影响宠物皮肤
						    if(playerInfoObj.IsShowDress >= 7)
						    {
						    	petData = GameCommonData.ModelOffsetEnemy[PlayerSkinsController.GetPetPersonSkinName(playerInfoObj.idMonsterType,1)];
						    }    
						    else
						    {
						    	petData = GameCommonData.ModelOffsetEnemy[PlayerSkinsController.GetPetPersonSkinName(playerInfoObj.idMonsterType,0)];
						    }
					    }
 
						if(petData!=null)
						{
							player.Offset			   = new Point(petData.@X,petData.@Y);						// 时装偏移值
							player.OffsetHeight		   = petData.@H;
							player.Role.Title		   = petData.@Title;
							player.Role.PersonSkinName = "Resources\\Enemy\\" + petData.@Swf + ".swf";
						}
						player.Role.idMonsterType   = playerInfoObj.idMonsterType;
						player.Role.Id              = playerInfoObj.roleId;
						player.Role.Type 	        = GameRole.TYPE_PET;
						player.Role.Face            = playerInfoObj.usLookFace;
						player.Role.Level           = playerInfoObj.usLevel;
						player.Role.Sex             = playerInfoObj.sex;
						player.Role.MaxHp           = playerInfoObj.maxHP;
						player.Role.HP              = playerInfoObj.currentHP;   
						player.Role.NameColor       = "#fff770"; 
						player.Role.NameBorderColor = 0x000000;
						player.Role.Direction	    = playerInfoObj.ucDir;								
						if(GameCommonData.Player.Role.Id == playerInfoObj.idOwner)
						{
							player.Role.Exp 						  = playerInfoObj.currentSP;
							player.Role.MasterPlayer 				  = GameCommonData.Player; 
							GameCommonData.Player.Role.UsingPetAnimal = player as GameElementPet;
							sendNotification(PlayerInfoComList.SHOW_PET_UI,player.Role);												
						}
						else
						{	
							//将宠物存在当前宠物信息列表里
						    GameCommonData.SameSecnePetList[playerInfoObj.idOwner] = player;
						    //获取宠物主人的信息
						    var petowner:GameElementPlayer = GameCommonData.SameSecnePlayerList[playerInfoObj.idOwner];
						    //判断宠物主人是否加载
						    if(petowner != null)
						    { 
						    	player.Role.MasterPlayer     = petowner;
						    	petowner.Role.UsingPetAnimal = player as GameElementPet;						       
						    }
						}
					}
					else														  														// 其余的属于玩家
					{
						player 			 		   = new GameElementPlayer(GameCommonData.GameInstance); 
						player.Role 	 		   = new GameRole();
						player.Role.Type 	       = GameRole.TYPE_PLAYER;		
						player.Role.Feel		   = playerInfoObj.Feel;
						player.Role.Direction	   = playerInfoObj.ucDir;
					    player.Role.PkValue        = playerInfoObj.pkValue; 	
					    player.Role.Sex            = playerInfoObj.sex;
					    player.Role.NameColor        = PKController.GetFontColor(player.Role.PkValue);
						player.Role.NameBorderColor  = PKController.GetBorderColor(player.Role.PkValue);
						player.Role.TitleColor       = 0x00fff6;
						player.Role.TitleBorderColor = 0x1b03ff;				
						player.Role.CurrentJobID     = playerInfoObj.dwProfession;
						player.Role.WeaponSkinID     = playerInfoObj.dwArmorType / 10;
						player.Role.WeaponEffectModel = playerInfoObj.dwArmorType % 10;
						player.Role.PersonSkinID     = playerInfoObj.dwDressType;
						player.Role.MountSkinID      = playerInfoObj.dwMountType;
						if((playerInfoObj.IsShowDress & 1) >0)
							player.Role.IsShowDress = true;
						else
						    player.Role.IsShowDress = false;					
						
                        
                        if(!PlayerSkinsController.IsShowWeaponEffect(player.Role.WeaponSkinID))             // 武器特效模式
						{
							player.Role.WeaponEffectModel = 0;
						} 
                        
                        var skinNameController:SkinNameController = PlayerSkinsController.GetSkinName(player);
                 
						player.Role.PersonSkinName   =  skinNameController.PersonSkinName;		 // 人物皮肤
						player.Role.WeaponSkinName   =  skinNameController.WeaponSkinName;       // 人物武器
						player.Role.WeaponEffectName =  skinNameController.WeaponEffectSkinName; // 武器光影
 					    player.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName; //武器特效模式
						player.Role.MountSkinName  = PlayerSkinsController.GetMount(playerInfoObj.dwMountType);	// 坐骑
					
					    var mountData:XML;
					    if(player.Role.IsShowDress == false || player.Role.PersonSkinID == 0)
						   mountData  = GameCommonData.ModelOffsetMount[playerInfoObj.dwMountType  + "_" + player.Role.CurrentJobID];		
						else
						   mountData  = GameCommonData.ModelOffsetMount[playerInfoObj.dwMountType  + "_" + player.Role.PersonSkinID];
						   														
						if(mountData!=null)
						{
							player.MountOffset = new Point(mountData.@X,mountData.@Y);
							player.MountHeight = mountData.@H;
						} 	
								

					    var playerData:XML;
					    if(playerInfoObj.IsShowDress == false || player.Role.PersonSkinID == 0)
					    	playerData = GameCommonData.ModelOffsetPlayer[PlayerSkinsController.GetOffsetPlayer(0,playerInfoObj.sex,player.Role.CurrentJobID)];	
					    else
					   		playerData = GameCommonData.ModelOffsetPlayer[PlayerSkinsController.GetOffsetPlayer(playerInfoObj.dwDressType,playerInfoObj.sex,player.Role.CurrentJobID)];					   
					   
					    if(playerData!=null)
						{
							player.Offset		= new Point(playerData.@X,playerData.@Y);												// 时装偏移值		
							player.OffsetHeight = playerData.@H;
						}
						
						
						//查找场景中的宠物 看该玩家是否有宠物
						var pet:GameElementPet =  GameCommonData.SameSecnePetList[playerInfoObj.roleId] as GameElementPet; 
						
					    if(pet != null)
					    { 
					    	pet.Role.MasterPlayer      = player as GameElementPlayer;
					    	player.Role.UsingPetAnimal = pet;
					    }
					}					
					
					if(playerInfoObj.usLookFace==30000)		// 摊位NPC显示
					{
						var stallSkin:MovieClip 	= StallSkinManager.getInstance().getStallSkin(playerInfoObj.roleId);
						StallConstData.stallOwnerIdDic[ playerInfoObj.roleId ] = playerInfoObj.currentHP;
//					    stallSkin 	playerInfoObj.currentHP;
						stallSkin.txtStallName.text = playerInfoObj.playerName;
						player.addChild(stallSkin);
					}
					else									// 玩家、怪物、其它NPC
					{
						player.Role.Name = playerInfoObj.playerName;
					}
					
					player.Role.StallId			 = playerInfoObj.idBooth;		// 摆位编号
					player.Role.TileX 			 = playerInfoObj.usPosX;
					player.Role.TileY 			 = playerInfoObj.usPosY;
					player.Role.Id  		     = playerInfoObj.roleId;				
					player.Role.PkValue          = playerInfoObj.pkValue;
					
	                player.Role.MonsterTypeID    = playerInfoObj.idMonsterType;										// 人物类型
					player.ChooseTarger   		 = onChooseTarger;													// 角色被选中事件
					player.MouseOutTarger 		 = onMouseOutTarger;
					player.MouseOverTarger		 = onMouseOverTarger;
					player.ActionPlayFrame		 = PlayerController.onActionPlayFrame;
					player.UpdateSkillEffect     = PlayerController.onUpdateSkillEffect;
					player.ActionPlayComplete    = PlayerController.onActionPlayComplete;
					player.MustMove              = PlayerController.MustMove;
	                player.MoveStep				 = onMoveStep;
	                
					///////////////////////////////////////////////////////////////////////////////
					/** （范加伟）*/
					player.Role.HP               = playerInfoObj.currentHP;
					player.Role.MaxHp            = playerInfoObj.maxHP;
					player.Role.MP               = playerInfoObj.currentMP;
					player.Role.MaxMp            = playerInfoObj.maxMP;
					player.Role.Level			 = playerInfoObj.usLevel;
					player.Role.SP               = playerInfoObj.currentSP;
					player.Role.MaxSp            = playerInfoObj.maxSP;
					player.Role.idTeam           = playerInfoObj.idTeam;
					player.Role.idTeamLeader     = playerInfoObj.idTeamLeader;
					player.Role.MainJob.Job      = playerInfoObj.dwProfession;
					player.Role.Face			 = playerInfoObj.usLookFace;
	                player.Role.VIP				 = playerInfoObj.vip;
	              
					if(playerInfoObj.isHidden == 1)			// GM隐身
					{
						player.Role.isHidden =  true;
					}
					///////////////////////////////////////////////////////////////////////////////
		
					if(GameCommonData.Scene !=null && GameCommonData.Scene.IsSceneLoaded == true)
					{
						GameCommonData.SameSecnePlayerList[player.Role.Id] = player;
						GameCommonData.Scene.AddPlayer(player);
					}
					
					// 更新主界面信息
					UIFacade.GetInstance(UIFacade.FACADEKEY).upDateInfo(0,playerInfoObj.roleId);
					
					//////////////////////////////////////////////////////
					//初始化场景中玩家 头上的 组队旗子   7.9（冯昌权）
					if(playerInfoObj.idBooth > 0 || playerInfoObj.roleId == GameCommonData.Player.Role.Id) 		// 摆摊时清除头上小旗子
					{			
						player.SetTeam(false);
						player.SetTeamLeader(false);
					} 
					else	//未摆摊，显示小旗子
					{								
						if(playerInfoObj.idTeam == 0)
						{
							player.SetTeam(false);
							player.SetTeamLeader(false);
						}
						else
						{
							if(playerInfoObj.idTeamLeader == player.Role.Id)
							{
								player.SetTeamLeader(true);
							}
							else
							{
								player.SetTeam(true);
							}
						}
					}
				}
				else				//////////////////////////////////////////////////////// 收到组队信息
				{
					var gameplayer:GameElementAnimal = PlayerController.GetPlayer(playerInfoObj.roleId);

					if( gameplayer != null && (gameplayer.Role.Type == GameRole.TYPE_OWNER || gameplayer.Role.Type == GameRole.TYPE_PLAYER ))
					{
						gameplayer.Role.WeaponSkinID      = playerInfoObj.dwArmorType / 10;
						gameplayer.Role.WeaponEffectModel = playerInfoObj.dwArmorType % 10;
						gameplayer.Role.PersonSkinID = playerInfoObj.dwDressType;
						gameplayer.Role.MountSkinID  = playerInfoObj.dwMountType;
						
						
						if(!PlayerSkinsController.IsShowWeaponEffect(gameplayer.Role.WeaponSkinID))
						{
							gameplayer.Role.WeaponEffectModel = 0;
						}											
					    skinNameController = PlayerSkinsController.GetSkinName(gameplayer);
					    gameplayer.Role.WeaponEffectName = skinNameController.WeaponEffectSkinName;
					    gameplayer.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
					    gameplayer.Role.WeaponDiaphaneity = skinNameController.WeaponDiaphaneity;
					    
					    PlayerSkinsController.SetSkinData(gameplayer,skinNameController);
										
					}
					
					if(GameCommonData.Player.Role.Id == playerInfoObj.roleId)
					{					
						player = GameCommonData.Player;
						player.Role.unityId  = playerInfoObj.unityId;
						player.Role.unityJob = playerInfoObj.unityJob;
						// 被踢出帮会后，关闭帮会主界面
						if(player.Role.unityId == 0)
						{
							if(UnityConstData.isWorking) facade.sendNotification(TimeCountDownEvent.CLOSEWORKCOUNTDOWN);		//关闭打工倒计时
							facade.sendNotification(UnityEvent.CLEARALL);
							facade.sendNotification(UnityEvent.CLOSEUNTIY);
						}
						// 被加入帮会后，关闭申请帮会面板
						else
						{
							facade.sendNotification(UnityEvent.CLOSEAPPLY);
							UnityConstData.dataSendState = false;		//数据传输结束
						}
						// 发送频道开启关闭通知
						facade.sendNotification(EventList.HASUINTY);	
					}
					else if(GameCommonData.SameSecnePlayerList[playerInfoObj.roleId]!=null)
					{
						if(GameCommonData.SameSecnePlayerList[playerInfoObj.roleId].getChildAt(0) && 
						   GameCommonData.SameSecnePlayerList[playerInfoObj.roleId].getChildAt(0) is MovieClip) 
						{
							// 摆摊 修改摊位名称 	2010.7.1 冯昌权
							player 					    = GameCommonData.SameSecnePlayerList[playerInfoObj.roleId];
							var playerMCC:MovieClip     = GameCommonData.SameSecnePlayerList[playerInfoObj.roleId].getChildAt(0);
							playerMCC.txtStallName.text = playerInfoObj.playerName;
						} 
						else 
						{
							// 退队或组对信息修改
							player = GameCommonData.SameSecnePlayerList[playerInfoObj.roleId];
							if(GameCommonData.TargetAnimal!=null && playerInfoObj.roleId==GameCommonData.TargetAnimal.Role.Id)
							{
								GameCommonData.TargetAnimal.Role.idTeam       = playerInfoObj.idTeam;
								GameCommonData.TargetAnimal.Role.idTeamLeader = playerInfoObj.idTeamLeader;
							}
						}
						
						if(playerInfoObj.usLookFace!=30000)
						{
							if(playerInfoObj.idBooth > 0) 			//摆摊时清除头上小旗子,
							{			
								player.SetTeam(false);
								player.SetTeamLeader(false);
							}
							else 									//未摆摊，显示小旗子
							{
								if(playerInfoObj.idTeam == 0)
								{
									player.SetTeam(false);
									player.SetTeamLeader(false);
								}
								else
								{
									if(playerInfoObj.idTeamLeader == player.Role.Id)
									{
										player.SetTeamLeader(true);
									}
									else
									{
										player.SetTeam(true);
									}
								}
							}
						}
						
						if(playerInfoObj.roleId >= 2000000000 && playerInfoObj.roleId <= 3999999999)			// 宠物改名
						{
							var currentPet:GameElementAnimal = GameCommonData.SameSecnePlayerList[playerInfoObj.roleId] as GameElementAnimal;
							if(currentPet!=null)
							{
								currentPet.SetName(playerInfoObj.playerName);
							}
						}
						else if(playerInfoObj.usLookFace!=30000 || player.Role.Type == GameRole.TYPE_PLAYER)	// 修改心情 、摆摊时玩家名显示和隐藏玩家名、
						{
							if(playerInfoObj.idBooth == 0)					
							{
								if(player.Role.IsShowFeel==0)				// 是否显示心情
								{
									player.SetTitle(playerInfoObj.Feel);
									player.ShowTitle();
								}
								player.ShowName();
							}
							else											// 隐藏其它玩家摆探时玩家名和称号
							{
								player.HideName();
								player.HideTitle();
							}
							
							// 摆摊效果
							player.Role.StallId = playerInfoObj.idBooth;
							player.PlayerStall();
						}
					}
					// 修改人：范加伟
					player.Role.idTeam		 = playerInfoObj.idTeam;
					player.Role.idTeamLeader = playerInfoObj.idTeamLeader;
					
					// VIP显示和隐藏
					player.Role.VIP = playerInfoObj.vip;
	                player.IsVip();
				}
//			}
//			catch(e:Error)
//			{
//				
//			}
		}
		
		private function onMoveStep(e:GameElementAnimal):void
		{
			UIFacade.UIFacadeInstance.updateSmallMap({type:2,id:e.Role.Id});
		}
	
		/** 选择目标对象  */
		private function onChooseTarger(e:GameElementAnimal):void
		{
			if(GameCommonData.TargetAnimal!=null)
			{
				if(GameCommonData.TargetAnimal.Role.Id != e.Role.Id)
				{
					CombatController.attackTime = 0;
					GameCommonData.autoPlayAnimalType = 0;
				}
				GameCommonData.TargetAnimal.IsSelect(false);
			}
			
			GameCommonData.IsMoveTargetAnimal = true;
			GameCommonData.TargetAnimal		  = e;
			GameCommonData.TargetAnimal.IsSelect(true);
		}
		
		private function onMouseOutTarger(e:GameElementAnimal):void
		{
			SysCursor.GetInstance().revert();
			switch(e.Role.Type)
			{
				case GameRole.TYPE_PLAYER:
					break;
				case GameRole.TYPE_ENEMY:
				 	break;
				case GameRole.TYPE_NPC: 
					 break;
			    case GameRole.TYPE_PET:
			        break;
			}
		}
		
		private function onMouseOverTarger(e:GameElementAnimal):void
		{
			var IsAttack:Boolean = false
			
			if(TargetController.IsPKTeam())
			{
				switch(e.Role.Type)
				{
					//怪物
					case GameRole.TYPE_ENEMY:		   
					     IsAttack = true;
					     break;
					//玩家
					case GameRole.TYPE_PLAYER:		              
				     	 if(e.Role.ActionState != GameElementSkins.ACTION_DEAD          					 //是否死亡
		   		          && e.Role.HP > 0)
		  		          {
			   			       if(GameCommonData.Player.Role.PKteam !=  e.Role.PKteam)
						       {
							      IsAttack = true;
							   }
						  }     
					     break;
					//宠物
					case GameRole.TYPE_PET:  
	     	      	   if(e.Role.ActionState != GameElementSkins.ACTION_DEAD           					 //是否死亡
	   		           && e.Role.HP > 0)
	  		           {
		   			        if(GameCommonData.Player.Role.PKteam != e.Role.MasterPlayer.Role.PKteam)
					        {
						       IsAttack = true;
						    }
					    }								     	      
					    break;   
				}		
				if(IsAttack)          
			     {
			     	SysCursor.GetInstance().setMouseType(SysCursor.ATTACK_CURSOR);
			     }   
		         else
			     {
			     	SysCursor.GetInstance().revert();
			     }  
			}
			else
			{
						if(TargetController.IsPKScene())
			    		{
			    			switch(e.Role.Type)
							{
								//怪物
								case GameRole.TYPE_ENEMY:		   
								     IsAttack = true;
								     break;
								//玩家
								case GameRole.TYPE_PLAYER:		              
							     	 if(e.Role.ActionState != GameElementSkins.ACTION_DEAD          					 //是否死亡
					   		          && e.Role.HP > 0)
					  		          {
						   			       if(GameCommonData.Player.Role.idTeam !=  e.Role.idTeam ||              		 //是否同1队              
										   GameCommonData.Player.Role.idTeam == 0)
									       {
										      IsAttack = true;
										   }
									  }     
								     break;
								//宠物
								case GameRole.TYPE_PET:  
				     	      	   if(e.Role.ActionState != GameElementSkins.ACTION_DEAD           					 //是否死亡
				   		           && e.Role.HP > 0)
				  		           {
					   			        if(GameCommonData.Player.Role.idTeam != e.Role.MasterPlayer.Role.idTeam ||      //是否同1队              
									    GameCommonData.Player.Role.idTeam==0)
								        {
									       IsAttack = true;
									    }
								    }								     	      
								    break;   
							}		
							if(IsAttack)          
						     {
						     	SysCursor.GetInstance().setMouseType(SysCursor.ATTACK_CURSOR);
						     }   
					         else
						     {
						     	SysCursor.GetInstance().revert();
						     }  
			    		}   		
						switch(e.Role.Type)
						{
							case GameRole.TYPE_PLAYER:		              
							     switch(GameCommonData.Player.Role.PkState)   
							     {
							     	//和平模式
							     	case 0:
							     		break;
							     	//除恶模式
							     	case 1: 
							     	      if(e.Role.PkValue > 0 || e.Role.NameColor == "#fd70ff")
							     	      {
							     	      	  if(e.Role.ActionState != GameElementSkins.ACTION_DEAD           				 //是否死亡
							   		          && e.Role.HP > 0)
							  		          {
								   			       if(GameCommonData.Player.Role.idTeam !=  e.Role.idTeam ||                 //是否同1队              
												   GameCommonData.Player.Role.idTeam==0)
											       {
												      IsAttack = true;
												   }
											  }
							     	      }
							     	      break;   
							     	 //杀戮模式
							     	 case 2:
								     	 if(e.Role.ActionState != GameElementSkins.ACTION_DEAD          					 //是否死亡
						   		          && e.Role.HP > 0)
						  		          {
							   			       if(GameCommonData.Player.Role.idTeam !=  e.Role.idTeam ||              		 //是否同1队              
											   GameCommonData.Player.Role.idTeam == 0)
										       {
											      IsAttack = true;
											   }
										  }
										  break;
								      case 3:
								          if(e.Role.unityId != GameCommonData.Player.Role.unityId 
								            || GameCommonData.Player.Role.unityId == 0)
								            {
							            	   if(GameCommonData.Player.Role.idTeam !=  e.Role.idTeam ||                 //是否同1队              
											   GameCommonData.Player.Role.idTeam==0)
										       {
											      IsAttack = true;
											   }
								            }	
								            break;			  
							     }       
							     if(IsAttack)          
							     {
							     	SysCursor.GetInstance().setMouseType(SysCursor.ATTACK_CURSOR);
							     }   
						         else
							     {
							     	SysCursor.GetInstance().revert();
							     }    
							     break;
							case GameRole.TYPE_PET:  
							     if(GameCommonData.Player.Role.UsingPetAnimal != null 
							     && GameCommonData.Player.Role.UsingPetAnimal.Role.Id == e.Role.Id)
							     {
							     	SysCursor.GetInstance().revert();
							     }
							     else
							     {  
									 switch(GameCommonData.Player.Role.PkState)   
								     {
								     	//和平模式
								     	case 0:break;
								     	//除恶模式
								     	case 1: 
								     	      if(e.Role.MasterPlayer != null && (e.Role.MasterPlayer.Role.PkValue > 0 || e.Role.MasterPlayer.Role.NameColor == "#fd70ff"))
								     	      {
								     	      	  if(e.Role.ActionState != GameElementSkins.ACTION_DEAD           								   //是否死亡
								   		          && e.Role.HP > 0)
								  		          {
									   			       if(GameCommonData.Player.Role.idTeam !=  e.Role.MasterPlayer.Role.idTeam ||                 //是否同1队              
													   GameCommonData.Player.Role.idTeam==0)
												       {
													      IsAttack = true;
													   }
												  }
								     	      }
								     	      break;   
								     	 //杀戮模式
								     	 case 2:
									     	 if(e.Role.ActionState != GameElementSkins.ACTION_DEAD           										//是否死亡
							   		          && e.Role.HP > 0)
							  		          {
								   			       if(GameCommonData.Player.Role.idTeam !=  e.Role.MasterPlayer.Role.idTeam ||                	 	//是否同1队              
												   GameCommonData.Player.Role.idTeam == 0)
											       {
												      IsAttack = true;
												   }
											  }
											  break;
									      case 3:
									          if(e.Role.MasterPlayer.Role.unityId != GameCommonData.Player.Role.unityId 
									            || GameCommonData.Player.Role.unityId == 0)
									            {
								            	   if(GameCommonData.Player.Role.idTeam !=  e.Role.MasterPlayer.Role.idTeam ||                 //是否同1队              
												   GameCommonData.Player.Role.idTeam==0)
											       {
												      IsAttack = true;
												   }
									            }	
									            break;			  
								     }  
								     if(IsAttack)          
								     {
								     	SysCursor.GetInstance().setMouseType(SysCursor.ATTACK_CURSOR);
								     }   
							         else
								     {
								     	SysCursor.GetInstance().revert();
								     }   
							     }
							     break;
							case GameRole.TYPE_ENEMY:
								 SysCursor.GetInstance().setMouseType(SysCursor.ATTACK_CURSOR);
								 break;
							case GameRole.TYPE_NPC: 
								 SysCursor.GetInstance().setMouseType(SysCursor.STALK_CURSOR);
								 break;		
						}
			}		
		}
	}
}