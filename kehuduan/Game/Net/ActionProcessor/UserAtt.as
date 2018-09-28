package Net.ActionProcessor
{
	import Controller.AutomatismController;
	import Controller.FloorEffectController;
	import Controller.PKController;
	import Controller.PlayerController;
	import Controller.PlayerSkinsController;
	import Controller.TalkController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.BigDrug.Data.BigDrugEvent;
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.CopyLead.Data.CopyLeadEvent;
	import GameUI.Modules.CopyLead.Mediator.CopyLeadMediator;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Designation.Data.DesignationChangeCommand;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardEvent;
	import GameUI.Modules.NewPlayerSuccessAward.Mediator.NewAwardMediator;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Mediator.EquipMediator;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUp;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUpMessage;
	import GameUI.Modules.RoleProperty.Net.NetAction;
	import GameUI.Modules.SmallWindow.Data.SmallWindowData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.MouseCursor.RepeatRequest;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Role.SkinNameController;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.utils.ByteArray;
	
	/** 玩家、怪、宠物基本信息  */
	public class UserAtt extends GameAction
	{
		private var bigDrugData:Array = [];		//大药临时数组
		private var newPlayerAwardData:Array = [];	//新手成就大礼包数组 
		private var copyLeadData:Array = [];	//副本引导数组
		private var calendarData:Array = [];	//日常表数组
		
		public function UserAtt(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{		
			var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			bytes.position = 4;
			
			var obj:Object 			 = new Object();
			obj.idUser     			 = bytes.readUnsignedInt(); 
			var dwAttributeNum:uint  = bytes.readUnsignedInt(); 
			var ucAttributeType:uint = 0; 
			var dwAttributeData:uint = 0; 
			var szInfo:String = new String();
			
//			var MainJobID:int = -1;            //主职业编号
//			var ViceJobID:int = -1;            //副职业编号
//			var CurrentJobID:int = -1;         //当前编号
			
			if(GameCommonData.SameSecnePlayerList == null || GameCommonData.Player == null || GameCommonData.Player.Role.RoleList == null)
			{
				return ;
			}
			var element:GameElementAnimal = GameCommonData.SameSecnePlayerList[obj.idUser] as GameElementAnimal;
			var tempRole:GameRole 		  = null;
			if(!GameCommonData.Player) return;
			var curJob:RoleJob 			  = GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob - 1];
			if(element!=null)
			{
				tempRole=element.Role
			}
			else
			{
				tempRole    = new GameRole();
				tempRole.Id = obj.idUser;
			}
			
			for(var i:int=0 ; i < dwAttributeNum ; i ++)
			{ 
				ucAttributeType = bytes.readUnsignedInt();  
				dwAttributeData = bytes.readUnsignedInt();
				
				switch(ucAttributeType)
				{					
					case 5:
						RepeatRequest.getInstance().otherCount=1;
						break;
					case 48:
					
						{
							//铜钱
							if(GameCommonData.Player.Role.Id != obj.idUser)
							{
								if(dwAttributeData == 0)
								{
									element.Role.isHidden = false;
									element.Visible = true;
								}
								else
								{
									element.Role.isHidden = true;
									element.Visible = false;
								}
							}				
						}
						break;
					case 77: //修改称号后，收到服务端信息
						{
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{					
								sendNotification(DesignationChangeCommand.NAME,{type:0,id:obj.idUser,value:dwAttributeData});
								facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"designation_txt",value:dwAttributeData} );
							}
							else
							{
								sendNotification(DesignationChangeCommand.NAME,{type:1,id:obj.idUser,value:dwAttributeData});
							}
							break;
						}
					case 100:
						{
							//绑定铜钱		
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
//								trace("绑定铜钱");
								GameCommonData.Player.Role.BindMoney = dwAttributeData;
								facade.sendNotification(StallEvents.REFRESHMONEYSEFLSTALL);
								facade.sendNotification(EventList.UPDATEMONEY, {money:UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]), target:"mcUnBind"});
							}else{
								tempRole.BindMoney=dwAttributeData;
							}		
						}
						break;
					case 101:
						{
							//铜钱
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
//								trace("铜钱");
								GameCommonData.Player.Role.UnBindMoney = dwAttributeData;
								facade.sendNotification(EventList.UPDATEMONEY, {money:UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]), target:"mcBind"});
							}else{
								tempRole.UnBindMoney=dwAttributeData;
							}				
						}
						break;
					case 102:
						{
							//赠卷
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.GiveAway = dwAttributeData;
								sendNotification(MarketEvent.UPDATE_MONEY_MARKET);		//更新商城中钱
							}else{
								tempRole.GiveAway=dwAttributeData;
							}					
						}
					break;			    
					case 103:
						{
							//绑定元宝
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.BindRMB = dwAttributeData;
								sendNotification(MarketEvent.UPDATE_MONEY_MARKET);		//更新商城中钱
							}else{
								tempRole.BindRMB=dwAttributeData;
							}				
						}
						break;
					case 104:
						{
							//元宝	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.UnBindRMB = dwAttributeData;
								facade.sendNotification(EventList.UPDATEMONEY, {money:UIUtils.getMoneySingle(GameCommonData.Player.Role.UnBindRMB, "\\ab"), target:"mcRmb"});  
							}else{
								tempRole.UnBindRMB=dwAttributeData;
							}
						}
						break;
					case 105:
						{
							//存款
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.SaveMoney = dwAttributeData;
								sendNotification(MarketEvent.UPDATE_MONEY_MARKET);		//更新商城中钱
							}else{
								tempRole.SaveMoney=dwAttributeData;
							}				
						}
						break;
					case 106:
						{
							//PK值	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{		
								GameCommonData.Player.Role.PkValue = dwAttributeData;
								facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"amok_txt",value:dwAttributeData} );	//更新其他信息里的杀气度
								var redNameTime:String = RolePropDatas.getDoubleTimeStr( dwAttributeData*1800 ).toString();
								facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"redName_txt",value:redNameTime} );	//更新红名时间
								if(GameCommonData.Player.Role.NameColor != "#fd70ff")
								{
									GameCommonData.Player.Role.NameColor       = PKController.GetFontColor(dwAttributeData);
									GameCommonData.Player.Role.NameBorderColor = PKController.GetBorderColor(dwAttributeData);
									GameCommonData.Player.UpdatePersonName();
								}
							}else{
								element.Role.PkValue = dwAttributeData;
								if(element.Role.NameColor != "#fd70ff")
								{
									element.Role.NameColor 		 = PKController.GetFontColor(dwAttributeData);
									element.Role.NameBorderColor = PKController.GetBorderColor(dwAttributeData);
									element.UpdatePersonName();
								}
							}
						}
						break;
					case 107:
						{						 	
							//等级	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								var tmpRoleLev:uint = GameCommonData.Player.Role.Level;
								GameCommonData.Player.Role.Level = dwAttributeData;
								var eqMed:EquipMediator = facade.retrieveMediator(EquipMediator.NAME) as EquipMediator;
								eqMed.playerAttribute.levUpLock = false;
								eqMed.playerAttribute.checkLevUp();
								if(tmpRoleLev != dwAttributeData)
								{
									RoleLevUp.PlayLevUp(obj.idUser , true);				//播放升级特效
								}
								facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtLevel", data:dwAttributeData+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]});     //"级"
								facade.sendNotification(TaskCommandList.UPDATE_ACCTASK_UITREE);
								sendNotification(EventList.PLAYER_TASK_LEVEL,1);
								sendNotification(EventList.UPDATE_MAINSECEN_EXP);
								if(dwAttributeData == 10 && NewerHelpData.newerHelpIsOpen&& GameCommonData.GameInstance.GameScene.GetGameScene.name != "2002") {	//
									sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 28);
								}
								if(dwAttributeData == 13) {
									sendNotification(NewerHelpEvent.SHOW_I_KNOW_NEWER_HELP, 36);
									sendNotification(SmallWindowData.SHOW_WINDOW,  GameCommonData.wordDic[ "med_lost_10" ] );  // 恭喜你达到13级！点击这里立即开始开始修炼经脉！
								} else if(dwAttributeData == 17) {
									sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 38);
								} else if(dwAttributeData == 18) {
									sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 44);
								}
								if(GameCommonData.wordVersion == 2)	//台服
								{
									if(dwAttributeData%10 == 0)
									{
										sendNotification(TaskCommandList.SEND_FB_AWARD,3);
									}
								}
//								if(dwAttributeData == 11 && NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.ROLE_LEVUP_NOTICE_NEWER_HELP, 0);
//								if(dwAttributeData == 12 && NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.ROLE_LEVUP_NOTICE_NEWER_HELP, 2);
							}else{
								tempRole.Level=dwAttributeData;
								RoleLevUp.PlayLevUp(obj.idUser , true);
							}
						}
						break;		
					case 108:
						{
							//经验
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								var addExp:uint = dwAttributeData;								
								GameCommonData.Player.Role.Exp = dwAttributeData;
								var exp:String = GameCommonData.Player.Role.Exp + "/" + (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);							
								facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtExp", data:exp});
								facade.sendNotification(EventList.UPDATE_MAINSECEN_EXP);
								if(GameCommonData.Player.Role.Level > 24) facade.sendNotification(NewerHelpEvent.UPDATE_EXP_NOTICE_NEWER_HELP);	//通知新手引导系统
							}else{
								tempRole.Exp=dwAttributeData;
							}
						}
						break;
					case 109:
						{
							//主职业可分配点
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Potential = dwAttributeData;
								var count:int = 0;
								for(var iM:int = 0; iM<GameCommonData.Player.Role.MainJob.Points.length; iM++)
								{									
									count += GameCommonData.Player.Role.MainJob.Points[iM];
								}
								GameCommonData.Player.Role.MainJob.Potential = dwAttributeData - count;
								if(RolePropDatas.selectedPageIndex==1){	
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPotential", data:GameCommonData.Player.Role.MainJob.Potential});
								}
							}else{
								tempRole.MainJob.Potential=dwAttributeData;
							}					
						}
						break;
					case 110:
						{
							//头像
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.Face = dwAttributeData;
							}else{
								tempRole.Face=dwAttributeData;
							}					
						}
						break;
					case 111:
						{
							//主职业
//						    MainJobID = dwAttributeData;	
						    
							//主职业
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{						
								if(GameCommonData.Player.Role.CurrentJob == 1 && GameCommonData.Player.Role.CurrentJobID != dwAttributeData)
								{
									GameCommonData.Player.Role.CurrentJobID = dwAttributeData;	
									
//									//武器是否有光影
//									if(GameCommonData.Player.Role.WeaponEffectModel != 0)
//									{
//										GameCommonData.Player.Role.WeaponEffectName      = PlayerSkinsController.GetWeapenEffect(GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player.Role.Sex,GameCommonData.Player.Role.CurrentJobID);
//										GameCommonData.Player.Role.WeaponEffectModelName = PlayerSkinsController.GetWeapenEffectModel(GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player.Role.CurrentJobID,GameCommonData.Player.Role.WeaponEffectModel);  				
//										GameCommonData.Player.Role.WeaponDiaphaneity     = PlayerSkinsController.GetWeapenEffectDiaphaneity(GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player.Role.CurrentJobID,GameCommonData.Player.Role.WeaponEffectModel);
//									}
//									else
//									{
//										GameCommonData.Player.Role.WeaponDiaphaneity     = 1;
//										GameCommonData.Player.Role.WeaponEffectName      = null;
//										GameCommonData.Player.Role.WeaponEffectModelName = null;				
//									}
									
									
									var skinNameController:SkinNameController = PlayerSkinsController.GetSkinName(GameCommonData.Player);
									GameCommonData.Player.Role.WeaponDiaphaneity     = 1;
									GameCommonData.Player.Role.WeaponEffectName      = skinNameController.WeaponEffectSkinName;
									GameCommonData.Player.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;	
					    			PlayerSkinsController.SetSkinData(GameCommonData.Player,skinNameController); 
											
//									PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_PERSON,GameCommonData.Player.Role.PersonSkinID,GameCommonData.Player);
//							        PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_WEAOIB,GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player);					        
								}
											
								GameCommonData.Player.Role.MainJob.Job = dwAttributeData;
								sendNotification(ChatEvents.HAS_MAINJOB_CHANNEL, {type:1});			//加入门派  创建职业聊天频道			
								if(GameCommonData.Player.Role.CurrentJob==1){
									sendNotification(RoleEvents.PLAYER_CHANGE_JOB);	
								}								
								var roleName:String = dataProxy.RolesListDic[dwAttributeData] + GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_4" ];              //"(主)"
								facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMainRole", data:roleName});						

							}
							else
							{
								
								element.Role.MainJob.Job = dwAttributeData; 
								
//								if(element.Role.CurrentJobID != dwAttributeData)
//								{
//									//武器是否有光影
//									if(element.Role.WeaponEffectModel != 0)
//									{
//										element.Role.WeaponEffectName      = PlayerSkinsController.GetWeapenEffect(element.Role.WeaponSkinID,element.Role.Sex,element.Role.CurrentJobID);
//										element.Role.WeaponEffectModelName = PlayerSkinsController.GetWeapenEffectModel(element.Role.WeaponSkinID,element.Role.CurrentJobID,element.Role.WeaponEffectModel);  				
//										element.Role.WeaponDiaphaneity     = PlayerSkinsController.GetWeapenEffectDiaphaneity(element.Role.WeaponSkinID,element.Role.CurrentJobID,element.Role.WeaponEffectModel);
//									}
//									else
//									{
//										element.Role.WeaponDiaphaneity     = 1;
//										element.Role.WeaponEffectName      = null;
//										element.Role.WeaponEffectModelName = null;				
//									}
//									
//									
//									element.Role.CurrentJobID = dwAttributeData;
//									PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_PERSON, element.Role.PersonSkinID,element);
//							        PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_WEAOIB, element.Role.WeaponSkinID,element);
//						  		}
						  		
						  		skinNameController = PlayerSkinsController.GetSkinName(element);
						  		element.Role.WeaponDiaphaneity     = 1;
								element.Role.WeaponEffectName      = skinNameController.WeaponEffectSkinName;
								element.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
						  		
					    		PlayerSkinsController.SetSkinData(element,skinNameController);
							}					
						}
						break;
					case 112:
						{						
							//HP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{				
								GameCommonData.Player.Role.HP = dwAttributeData;
								
								AutomatismController.AddHpSkill();
								
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHp", data:dwAttributeData+"/"+(GameCommonData.Player.Role.MainJob.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP)});
								}else{
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHp", data:dwAttributeData+"/"+(GameCommonData.Player.Role.ViceJob.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP)});
								}
								if(GameCommonData.Player.IsAutomatism){
									sendNotification(AutoPlayEventList.ATT_CHANGE_EVENT,1);
								}
							}else{
								
								if(tempRole.Type == GameRole.TYPE_ENEMY)
								{
									if(GameCommonData.BossTalk[tempRole.PersonSkinID] != null)
									{
										if(dwAttributeData ==  tempRole.MaxHp)
										{
											GameCommonData.Scene.begin = true;
											GameCommonData.Scene.hp = true;
										}
										
									    player  = PlayerController.GetPlayer(tempRole.Id);
										if(tempRole.HP == tempRole.MaxHp)
										{
											TalkController.BossTalk(1,player);
										}
										else
										{
											TalkController.BossTalk(3,player);
										}
									}
								}
								  
								 tempRole.HP=dwAttributeData;
								 //判断是否是队友
								 if(GameCommonData.TeamPlayerList!=null && GameCommonData.TeamPlayerList[tempRole.Id]!=null)
								 {
									 AutomatismController.AddTeamSkill(tempRole.Id);
								 }
							}			
						}
						break;
					case 113:
						{
							//MP				
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MP = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMp", data:dwAttributeData+"/"+(GameCommonData.Player.Role.MainJob.MaxMp+GameCommonData.Player.Role.AdditionAtt.MaxMP)});
								}else{
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMp", data:dwAttributeData+"/"+(GameCommonData.Player.Role.ViceJob.MaxMp+GameCommonData.Player.Role.AdditionAtt.MaxMP)});
								}
								
								if(GameCommonData.Player.IsAutomatism){
									sendNotification(AutoPlayEventList.ATT_CHANGE_EVENT,2);
								}
							}else{
								tempRole.MP=dwAttributeData;
							}
						}
						break;
					case 114:
						{
							//SP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.SP = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSp", data:dwAttributeData+"/"+(GameCommonData.Player.Role.MainJob.MaxSP+GameCommonData.Player.Role.AdditionAtt.MaxSP)});
								}else{
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSp", data:dwAttributeData+"/"+(GameCommonData.Player.Role.ViceJob.MaxSP+GameCommonData.Player.Role.AdditionAtt.MaxSP)});
								}
							}else{
								tempRole.SP=dwAttributeData;
							}					
						}
						break;
					case 115:
						{
							//MAXHP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								if(GameCommonData.Player.Role.CurrentJob==1){
									GameCommonData.Player.Role.MaxHp = dwAttributeData;
								}
								if(RolePropDatas.selectedPageIndex==1){
									var maxHP:int = dwAttributeData + GameCommonData.Player.Role.AdditionAtt.MaxHP;
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHp", data:Math.min(GameCommonData.Player.Role.HP,maxHP)+"/"+maxHP});
								}
								
								GameCommonData.Player.Role.MainJob.MaxHp=dwAttributeData;
								
							}else{
								tempRole.MaxHp=dwAttributeData;
							}				
						}
						break;
					case 116:
						{
							//MAXMP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								if(GameCommonData.Player.Role.CurrentJob==1){
									GameCommonData.Player.Role.MaxMp = dwAttributeData;
								}
								if(RolePropDatas.selectedPageIndex==1){
									GameCommonData.Player.Role.MaxMp = dwAttributeData;
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMp", data:Math.min(GameCommonData.Player.Role.MP,dwAttributeData)+"/"+dwAttributeData});
								}
								GameCommonData.Player.Role.MainJob.MaxMp=dwAttributeData;
							}else{
								tempRole.MaxMp=dwAttributeData;
							}				
						}
						break;
					case 117:
						{
							//MAXSP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								
								if(GameCommonData.Player.Role.CurrentJob==1){
									GameCommonData.Player.Role.MaxSp = dwAttributeData;
								}
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSp", data:Math.min(GameCommonData.Player.Role.SP,dwAttributeData)+"/"+dwAttributeData});
								}
								GameCommonData.Player.Role.MainJob.MaxSP=dwAttributeData;
								
							}else{
								tempRole.MaxSp=dwAttributeData;
							}				
						}
						break;
					case 118:
						{
							//    主职业物攻	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.PhyAttack = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPhyAttack", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.PhyAttack=dwAttributeData;
							}				
						}
						break;
					case 119:
						{
							//魔攻
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.MagicAttack = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMagicAttack", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.MagicAttack=dwAttributeData;
							}				
						}
						break;
					case 120:
						{
							//物防	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.PhyDef = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPhyDef", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.PhyDef=dwAttributeData;
							}				
						}
						break;
					case 121:
						{
							//魔防	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.MagicDef = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMagicDef", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.MagicDef=dwAttributeData;
							}			
						}
						break;
					case 122:
						{
							//命中
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Hit = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHit", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Hit=dwAttributeData;
							}					
						}
						break;
					case 123:
						{
							//闪避		
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Dodge = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHide", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Dodge=dwAttributeData;
							}		
						}
						break;
					case 124:
						{
							//暴击
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Crit = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtCrit", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Crit=dwAttributeData;
							}				
						}
						break;
					case 125:
						{
							//韧性	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Toughness = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtToughness", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Toughness=dwAttributeData;
							}				
						}
						break;
					case 126:
						{
							//力量	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Force = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtForce", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Force=dwAttributeData;
							}			
						}
						break;
					case 127:
						{
							//灵力
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.SpiritPower = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSpiritPower", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.SpiritPower=dwAttributeData;
							}				
						}
						break;
					case 128:
						{
							//体力
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Physical = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPhysical", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Physical=dwAttributeData;
							}						
						}
						break;
					case 129:
						{
							//定力
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Constant = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtConstant", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Constant=dwAttributeData;
							}					
						}
						break;
					case 130:
						{
							//身法
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.MainJob.Magic = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==1){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMagic", data:dwAttributeData});
								}
							}else{
								tempRole.MainJob.Magic=dwAttributeData;
							}					
						}
						break;
					case 131:
						{
							//职业2经验					
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Exp = dwAttributeData;
							}else{
								tempRole.ViceJob.Exp=dwAttributeData;
							}	
						}
						break;
					case 132:
						{
							//职业2可分配点
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Potential = dwAttributeData;
								var count2:int = 0;
								for(var iV:int = 0; iV<GameCommonData.Player.Role.ViceJob.Points.length; iV++)
								{
									count2 += GameCommonData.Player.Role.ViceJob.Points[iV];
								}
								GameCommonData.Player.Role.ViceJob.Potential = dwAttributeData - count2;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPotential", data:GameCommonData.Player.Role.ViceJob.Potential });
								}
							}else{
								tempRole.ViceJob.Potential=dwAttributeData;
							}					
						}
						break;
					case 133:
						{
							//职业2头像
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.Face = dwAttributeData;
							}else{
								tempRole.Face=dwAttributeData;
							}				
						}
						break;
					case 134:
						{							
							//副职业
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{ 
								var preJob:uint=GameCommonData.Player.Role.ViceJob.Job;
								GameCommonData.Player.Role.ViceJob.Job = dwAttributeData;
								if(preJob==0 && GameCommonData.Player.Role.ViceJob.Job!=0){
									//todo请求副职业信息
									NetAction.GetRoleInfo(2);
								}
								sendNotification(ChatEvents.HAS_MAINJOB_CHANNEL, {type:2});			//加入门派  创建职业聊天频道			
								if(GameCommonData.Player.Role.CurrentJob==2){
									sendNotification(RoleEvents.PLAYER_CHANGE_JOB);	
								}
								var viceRoleName:String = dataProxy.RolesListDic[dwAttributeData]+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_5" ];                   //"（副）"
								facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSubRole", data:viceRoleName});
							}else{
								tempRole.ViceJob.Job=dwAttributeData;
							}					 
						}
						break;
					case 135:
						{
							//职业2HP
						}
						break;
					case 136:
						{
							//职业2MP
				
						}
						break;
					case 137:
						{
							//职业2SP
			
						}
						break;
					case 138:
						{
							//职业2MAXHP
							
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								if(GameCommonData.Player.Role.CurrentJob==2){
									GameCommonData.Player.Role.MaxHp = dwAttributeData;
								}
								trace(RolePropDatas.selectedPageIndex);
								if(RolePropDatas.selectedPageIndex==2){
									var secMaxHP:int = dwAttributeData + GameCommonData.Player.Role.AdditionAtt.MaxHP;
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHp", data:Math.min(GameCommonData.Player.Role.HP,secMaxHP)+"/"+secMaxHP});
								}
								GameCommonData.Player.Role.ViceJob.MaxHp=dwAttributeData;
								
							}else{
								tempRole.MaxHp=dwAttributeData;
							}				
				
						}
						break;f
					case 139:
						{
							//职业2MAXMP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								
								if(GameCommonData.Player.Role.CurrentJob==2){
									GameCommonData.Player.Role.MaxMp = dwAttributeData;
								}
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMp", data:Math.min(GameCommonData.Player.Role.MP,dwAttributeData)+"/"+dwAttributeData});
								}
								GameCommonData.Player.Role.ViceJob.MaxMp=dwAttributeData;
							}else{
								tempRole.MaxMp=dwAttributeData;
							}	
						}
						break;
					case 140:
						{
							//职业2MAXSP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								if(GameCommonData.Player.Role.CurrentJob==2){
									GameCommonData.Player.Role.MaxSp = dwAttributeData;
								}
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSp", data:Math.min(GameCommonData.Player.Role.SP,dwAttributeData)+"/"+dwAttributeData});
								}
								GameCommonData.Player.Role.ViceJob.MaxSP=dwAttributeData;
							}else{
								tempRole.MaxSp=dwAttributeData;
							}		
						}
						break;
					case 141:
						{
							///职业2物攻
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.PhyAttack = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPhyAttack", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.PhyAttack=dwAttributeData;
							}			
						}
						break;
					case 142:
						{
							//职业2魔攻
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.MagicAttack = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMagicAttack", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.MagicAttack=dwAttributeData;
							}					
						}
						break;
					case 143:
						{
							//职业2物防
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.PhyDef = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPhyDef", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.PhyDef=dwAttributeData;
							}				
						}
						break;
					case 144:
						{
							//职业2魔防
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.MagicDef = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMagicDef", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.MagicDef=dwAttributeData;
							}				
						}
						break;
					case 145:
						{
							//职业2命中
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Hit = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHit", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Hit=dwAttributeData;
							}					
						}
						break;
					case 146:
						{
							//职业2闪避
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Dodge = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHide", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Dodge=dwAttributeData;
							}					
						}
						break;
					case 147:
						{
							//职业2暴击
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Crit = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtCrit", data:dwAttributeData});
								}
							}else{	
								tempRole.ViceJob.Crit=dwAttributeData;
							}			
								
						}
						break;
					case 148:
						{
							//职业2韧性
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Toughness = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtToughness", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Toughness=dwAttributeData;
							}					
						}
						break;
					case 149:
						{
							//职业2力量	
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Force = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtForce", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Force=dwAttributeData;
							}					
						}
						break;
					case 150:
						{
							//职业2灵力
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.SpiritPower = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtSpiritPower", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.SpiritPower=dwAttributeData;
							}					
						}
						break;
					case 151:
						{
							//职业2体力
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Physical = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtPhysical", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Physical=dwAttributeData;
							}					
						}
						break;
					case 152:
						{
							//职业2定力
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Constant = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtConstant", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Constant=dwAttributeData;
							}				
						}
						break;
					case 153:
						{
							//职业2身法
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.ViceJob.Magic = dwAttributeData;
								if(RolePropDatas.selectedPageIndex==2){
									facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtMagic", data:dwAttributeData});
								}
							}else{
								tempRole.ViceJob.Magic=dwAttributeData;
							}					
						}
						break;
					case 154:
						{
							//当前职业附加MAXHP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.MaxHP = dwAttributeData;
								//facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtHp", data:GameCommonData.Player.Role.HP+"/"+(GameCommonData.Player.Role.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP)});
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtHp", data:GameCommonData.Player.Role.HP+"/"+(GameCommonData.Player.Role.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP)});
							}					
						}
					break;
					case 155:
						{
							//当前职业附加MAXMP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.MaxMP = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtMp", data:GameCommonData.Player.Role.MP+"/"+(GameCommonData.Player.Role.MaxMp+GameCommonData.Player.Role.AdditionAtt.MaxMP)});
							
							}					
						}
						break;
					case 156:
						{
							//当前职业附加MAXSP
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.MaxSP = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtSp", data:GameCommonData.Player.Role.SP+"/"+(GameCommonData.Player.Role.MaxSp+GameCommonData.Player.Role.AdditionAtt.MaxSP)});
							}					
						}
						break;
					case 157:
						{
							//当前职业附加物攻
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								
								GameCommonData.Player.Role.AdditionAtt.PhyAttack = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtPhyAttack", data:curJob.PhyAttack+dwAttributeData});
							}						
						}
						break;
					case 158:
						{
							//当前职业附加魔攻
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
	//							trace("txtMagicAttack = ", dwAttributeData);
	//							trace("curJob.MagicAttack = ", curJob.MagicAttack);
								GameCommonData.Player.Role.AdditionAtt.MagicAttack = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtMagicAttack", data:curJob.MagicAttack+dwAttributeData});
							}				
						}
						break;
					case 159:
						{
							//当前职业附加物防
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.PhyDef = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtPhyDef", data:curJob.PhyDef+dwAttributeData});
							}					
						}
						break;
					case 160:
						{
							//当前职业附加魔防
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.MagicDef = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtMagicDef", data:curJob.MagicDef+dwAttributeData});
							}				
						}
						break;
					case 161:
						{
							//当前职业附加命中
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Hit = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtHit", data:curJob.Hit+dwAttributeData});
							}				
						}
						break;
					case 162:
						{
							//当前职业附加暴击
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Dodge = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtHide", data:curJob.Dodge+dwAttributeData});
							}				
						}
						break;	
					case 163:
						{
							//当前职业附加闪避
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Crit = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtCrit", data:curJob.Crit+dwAttributeData});
							}				
						}
						break;
					case 164:
						{
							//当前职业附加韧性
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Toughness = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtToughness", data:curJob.Toughness+dwAttributeData});
							}					
						}
						break;
					case 165:
						{
							//当前职业附加属性1
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Force = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtForce", data:curJob.Force+dwAttributeData});
							}					
						}
						break;
					case 166:
						{
							//当前职业附加属性2
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.SpiritPower = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtSpiritPower", data:curJob.SpiritPower+dwAttributeData});
							}
						}
						break;
					case 167:
						{
							//当前职业附加属性3
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Physical = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtPhysical", data:curJob.Physical+dwAttributeData});
							}					
						}
						break;
					case 168:
						{
							//当前职业附加属性4
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Constant = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtConstant", data:curJob.Constant+dwAttributeData});
							}				
						}
						break;
					case 169:
						{
							//当前职业附加属性5
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.AdditionAtt.Magic = dwAttributeData;
								facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtMagic", data:curJob.Magic+dwAttributeData});
							}					
						}
						break;
					case 170:
						//主职业 职业等级
						if(GameCommonData.Player.Role.Id == obj.idUser)
						{
							var tmpMainLev:uint = GameCommonData.Player.Role.MainJob.Level;
							GameCommonData.Player.Role.MainJob.Level = dwAttributeData;
							if(tmpMainLev != dwAttributeData)
							{
								var eqMedMain:EquipMediator = facade.retrieveMediator(EquipMediator.NAME) as EquipMediator;
								eqMedMain.playerAttribute.levUpLock = false;
								eqMedMain.playerAttribute.checkLevUp();
								if(dwAttributeData > 10) {
									RoleLevUp.PlayLevUp(obj.idUser , true);				//播放升级特效
								}
							}
							facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtJobLevel", data:dwAttributeData+GameCommonData.wordDic[ "mod_rp_med_ui_pa_7" ]});          //"级"
							facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtProExp", data:UIConstData.ExpDic[dwAttributeData+1000]});
							RoleLevUpMessage.sendUpMessage(GameCommonData.Player.Role.MainJob);
//							if(dwAttributeData == 10 && NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.ROLE_LEVUP_NOTICE_NEWER_HELP, 1);
						}
						else 
						{
							if(dwAttributeData > 10) { 
								RoleLevUp.PlayLevUp(obj.idUser , true);
							}
						}
					break;
					case 171:
						//副职业 职业等级
						if(GameCommonData.Player.Role.Id == obj.idUser)
						{
							if(GameCommonData.Player.Role.ViceJob.Level != dwAttributeData)
							{
								RoleLevUp.PlayLevUp(obj.idUser , true);				//播放升级特效
							}
							GameCommonData.Player.Role.ViceJob.Level = dwAttributeData;
							facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtJobLevel", data:dwAttributeData+GameCommonData.wordDic[ "mod_rp_med_ui_pa_7" ]});
							facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtProExp", data:UIConstData.ExpDic[dwAttributeData+2000]});
							RoleLevUpMessage.sendUpMessage(GameCommonData.Player.Role.ViceJob);
						}
						else
						{
							RoleLevUp.PlayLevUp(obj.idUser , true);
						}
					break;	
					case 172:
						{
							//战力评分
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.Score = dwAttributeData;
								var str:String = UIUtils.GetScoreStr(dwAttributeData);
								facade.sendNotification(EventList.UPDATEATTRIBUATT, {target:"txtScore", data:str});
							}					
						}
						break;
					case 173://背包格子
						if(GameCommonData.Player.Role.Id == obj.idUser)
						{
							GameCommonData.Player.Role.BagLevel = dwAttributeData;
							BagData.BagNum = [	int(dwAttributeData/10000)%100+24,
							int(dwAttributeData/100)%100+24,
							int(dwAttributeData%100)+24	];
							
							PetPropConstData.petBagNum = (dwAttributeData/1000000)%100 + 3;		//宠物背包等级 
							/** 宠物背包增量  */
							PetPropConstData.addPetBagNum( PetPropConstData.petBagNum );
							
							facade.sendNotification(BagEvents.EXTENDBAG);
							var bagLevel:String = ( int(dwAttributeData / 10000) % 100 ).toString();
							var stuffBagLevel:String = ( int(dwAttributeData / 100) % 100 ).toString();
							facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"bagLevel_txt",value:bagLevel} );
							facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"stuffBagLevel_txt",value:stuffBagLevel} );
						}
						break;	
					 case 174:
						{
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.MaskLow=dwAttributeData;
								
								
							}					
						}
						break;
					case 175:
						{
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{	
								GameCommonData.MaskHi=dwAttributeData;
								
								sendNotification(TaskCommandList.UPDATE_MASK);
							}					
						}
						break;
					case 176:
						{
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.DayMaskLow=dwAttributeData;
								
							}					
						}
						break;
					case 177:
						{
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.DayMaskHi=dwAttributeData;
								
							}					
						}
						break; 
				
					case 180://仓库格子
						{
							if(GameCommonData.Player.Role.Id == obj.idUser)
							{
								GameCommonData.Player.Role.DepotLevel = dwAttributeData;
								DepotConstData.gridCount = int(dwAttributeData % 100) * 6 + 24;		//仓库物品格子等级
								DepotConstData.petDepotNum = (dwAttributeData / 100) % 100 + 2 		//仓库宠物栏等级
								facade.sendNotification(DepotEvent.EXTITEMDEPOT);
							}
						}
						break;
					
					case 181://人物火攻属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [0 , dwAttributeData]);
						}
						break;
					case 182://人物火防属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [1 , dwAttributeData]);
						}
						break;
					
					case 183://人物冰攻属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [2 , dwAttributeData]);
						}
						break;
						
					case 184://人物冰防属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [3 , dwAttributeData]);
						}
						break;
					
					case 185://人物玄功属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [4 , dwAttributeData]);
						}
						break;
					case 186://人物玄防属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [5 , dwAttributeData]);
						}
						break;
					
					case 187://人物毒攻属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [6 , dwAttributeData]);
						}
						break;
					case 188://人物毒防属性
						{
							facade.sendNotification(RoleEvents.ATTENDPROPELEMENT , [7 , dwAttributeData]);
						}
						break;
					case 189: //心情的广播	
						var e:GameElementAnimal = GameCommonData.SameSecnePlayerList[obj.idUser] as GameElementAnimal;
						if(GameCommonData.Player.Role.Id==obj.idUser)
						{
							RepeatRequest.getInstance().quickKeyCount=1;
						  	GameCommonData.Player.Role.IsShowFeel = dwAttributeData;
						  	if(dwAttributeData==0)
						  	{
					  			if(GameCommonData.Player.Role.StallId == 0)
					  			{
							  		GameCommonData.Player.ShowTitle();
					  			}
						  	}
						  	else
						  	{
						  		GameCommonData.Player.HideTitle();
						  	}
							sendNotification(EventList.UPDATE_FEEL_STATUS,dwAttributeData);
						}
						else
						{
						  	if(e!=null && e.Role!=null)
						  	{
						  		e.Role.IsShowFeel = dwAttributeData;
						  		if(dwAttributeData==0)
						  		{
						  			if(e.Role.StallId == 0)
						  			{
							  			e.ShowTitle();
						  			}
						  		}
						  		else
						  		{
						  			e.HideTitle();	
						  		}	
						  	}
						}
						break;
					
					case 190://当前职业
	//				     CurrentJobID = dwAttributeData;
						if(GameCommonData.Player.Role.Id==obj.idUser)
						{
							var preJob1:uint=GameCommonData.Player.Role.CurrentJob;
							
							if(dwAttributeData!=GameCommonData.Player.Role.CurrentJob)
							{
								GameCommonData.Player.Role.CurrentJob=dwAttributeData;
								RolePropDatas.selectedPageIndex=dwAttributeData;
								sendNotification(RoleEvents.PLAYER_CHANGE_JOB);
								sendNotification(EventList.CHANGE_QUICKBAR_UI);
								NetAction.GetRoleInfo(preJob1);
								
								//判断主 副职业
								if(GameCommonData.Player.Role.CurrentJob == 1)
								{
									GameCommonData.Player.Role.CurrentJobID = GameCommonData.Player.Role.MainJob.Job;
								}
								else
								{
									GameCommonData.Player.Role.CurrentJobID = GameCommonData.Player.Role.ViceJob.Job;
								}	
								sendNotification(TaskCommandList.UPDATE_LEVEL_TASK);  //处理换职业对环任务的影响
								if(!PlayerSkinsController.IsCanUse(GameCommonData.Player.Role.MountSkinID,GameCommonData.Player.Role.CurrentJobID))
								{
									PlayerSkinsController.SetSkinMountData(GameCommonData.Player);	
								}
								
//								//武器是否有光影
//								if(GameCommonData.Player.Role.WeaponEffectModel != 0)
//								{
//									GameCommonData.Player.Role.WeaponEffectName      = PlayerSkinsController.GetWeapenEffect(GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player.Role.Sex,GameCommonData.Player.Role.CurrentJobID);
//									GameCommonData.Player.Role.WeaponEffectModelName = PlayerSkinsController.GetWeapenEffectModel(GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player.Role.CurrentJobID,GameCommonData.Player.Role.WeaponEffectModel);  				
//									GameCommonData.Player.Role.WeaponDiaphaneity     = PlayerSkinsController.GetWeapenEffectDiaphaneity(GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player.Role.CurrentJobID,GameCommonData.Player.Role.WeaponEffectModel);
//								}
//								else
//								{
//									GameCommonData.Player.Role.WeaponDiaphaneity     = 1;
//									GameCommonData.Player.Role.WeaponEffectName      = null;
//									GameCommonData.Player.Role.WeaponEffectModelName = null;				
//								}
//								
//								PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_PERSON,GameCommonData.Player.Role.PersonSkinID,GameCommonData.Player);
//						        PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_WEAOIB,GameCommonData.Player.Role.WeaponSkinID,GameCommonData.Player);

							    skinNameController = PlayerSkinsController.GetSkinName(GameCommonData.Player);
							    
								GameCommonData.Player.Role.WeaponDiaphaneity     = 1;
								GameCommonData.Player.Role.WeaponEffectName      = skinNameController.WeaponEffectSkinName;
								GameCommonData.Player.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;	
				    			PlayerSkinsController.SetSkinData(GameCommonData.Player,skinNameController); 
							}
						}
						else
						{
						    element.Role.CurrentJob=dwAttributeData;
							//判断主 副职业
							if(element.Role.CurrentJob == 1)
							{
								element.Role.CurrentJobID = element.Role.MainJob.Job;
							}
							else
							{
								element.Role.CurrentJobID = element.Role.ViceJob.Job;
							}		
							
							if(!PlayerSkinsController.IsCanUse(element.Role.MountSkinID,element.Role.CurrentJobID))
							{
								PlayerSkinsController.SetSkinMountData(element);	
							}
							
//							//武器是否有光影
//							if(element.Role.WeaponEffectModel != 0)
//							{
//								element.Role.WeaponEffectName      = PlayerSkinsController.GetWeapenEffect(element.Role.WeaponSkinID,element.Role.Sex,element.Role.CurrentJobID);
//								element.Role.WeaponEffectModelName = PlayerSkinsController.GetWeapenEffectModel(element.Role.WeaponSkinID,element.Role.CurrentJobID,element.Role.WeaponEffectModel);  				
//								element.Role.WeaponDiaphaneity     = PlayerSkinsController.GetWeapenEffectDiaphaneity(element.Role.WeaponSkinID,element.Role.CurrentJobID,element.Role.WeaponEffectModel);
//							}
//							else
//							{
//								element.Role.WeaponDiaphaneity     = 1;
//								element.Role.WeaponEffectName      = null;
//								element.Role.WeaponEffectModelName = null;				
//							}
//							
							skinNameController = PlayerSkinsController.GetSkinName(element);
							
							element.Role.WeaponDiaphaneity     = 1;
							element.Role.WeaponEffectName      = skinNameController.WeaponEffectSkinName;
							element.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
					        PlayerSkinsController.SetSkinData(element,skinNameController);
							
//	 						PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_PERSON,element.Role.PersonSkinID,element);
//					        PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_WEAOIB,element.Role.WeaponSkinID,element);
						}
						break;
					case 191:		//大红
						bigDrugData = [];
						//type 类型	1-大瓶，2-小瓶          data  剩余量
						bigDrugData.push({type:dwAttributeData % 10, data:uint(dwAttributeData / 10)});  
						break;
					case 192:		//大蓝
						bigDrugData.push({type:dwAttributeData % 10, data:uint(dwAttributeData / 10)});  
						break;
					case 193:		//宠物大红
						bigDrugData.push({type:1, data:dwAttributeData});  
						break; 
					case 194:
						if(GameCommonData.Player.Role.Id == obj.idUser)
						{
							GameCommonData.Player.Role.Ene = dwAttributeData;
							facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtPower", data:GameCommonData.Player.Role.Ene+"/"+GameCommonData.Player.Role.MaxEne});
						}		
						break;
					case 195:		//新手成就礼包状态掩码
						newPlayerAwardData = [];
						newPlayerAwardData.push(dwAttributeData);
						copyLeadData.push(dwAttributeData);
					break;
					
					case 196:		//新手成就礼包建号时间
						newPlayerAwardData.push(dwAttributeData);
					break;
					case 197:		//新手成就礼包入帮时间
						newPlayerAwardData.push(dwAttributeData);
					break;
					case 198:		//人物精力值
						GameCommonData.Player.Role.Vit = dwAttributeData;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
						facade.sendNotification(RoleEvents.UPDATEADDATT, {target:"txtEnergy", data:GameCommonData.Player.Role.Vit+"/"+GameCommonData.Player.Role.MaxVit});
					break;
						
					//下面的接收人物基本信息
					case 201:  //称号
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"designation_txt",value:dwAttributeData} );
						break;	
					case 203:	//师傅
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"master_txt",value:dwAttributeData} );
						break;
					case 204:	//配偶
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"mate_txt",value:dwAttributeData} );
						break;
					case 205:	//成亲日期
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"mateDate_txt",value:dwAttributeData} );
						break;
					case 209:	//武林同盟
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"ally_txt",value:dwAttributeData} );
						break;
					case 210:	//江湖侠义值
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"errantry_txt",value:dwAttributeData} );
						break;
					case 211:	//大宋荣誉
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"honor_txt",value:dwAttributeData} );
						break;
					case 212:	//帮派贡献
						GameCommonData.Player.Role.unityContribution = dwAttributeData;
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"unityAtt_txt",value:dwAttributeData} );
						break;
					case 213:	//传道解惑
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"impart_txt",value:dwAttributeData} );
						break;
					case 214:	//主门派贡献
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"mainSchool_txt",value:dwAttributeData} );
						break;
					case 215:	//副门派贡献
						if ( (GameCommonData.Player.Role.RoleList[1] as RoleJob).Job != 0 )
						{
							facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"viceSchool_txt",value:dwAttributeData} );	
						}
						break;
					case 216:	//冻结双倍
//						trace ("冻结双倍:"+dwAttributeData);
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"doubleTime_txt",value:RolePropDatas.getDoubleTimeStr(dwAttributeData)} );
						break;
					case 217:	//vip剩余时间
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"vipTime_txt",value:RolePropDatas.getVipTime(dwAttributeData*60)} );
						break;
					case 219:	//当前活跃度
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"active_txt",value:dwAttributeData} );
						break;
					case 220://领取在线奖励是否成功
						//下面的 文本用于测试
//						trace ( "收到领奖次数: " + dwAttributeData );
						GameCommonData.Player.Role.OnLineAwardTime = dwAttributeData;
						facade.sendNotification( OnLineAwardData.NEXT_ONLINE_GIFT );
						break;
					case 221:	//是否显示时装
						if(obj.idUser == GameCommonData.Player.Role.Id)
						{
							if(dwAttributeData == 1)
							{
								GameCommonData.Player.Role.IsShowDress = true;
							}
							else
							{
								GameCommonData.Player.Role.IsShowDress = false;
							}
							facade.sendNotification(RoleEvents.ISSHOWDRESS , dwAttributeData); 
							
							if(!PlayerSkinsController.IsShowWeaponEffect(GameCommonData.Player.Role.WeaponSkinID))
							{
								GameCommonData.Player.Role.WeaponEffectModel = 0;
							}											
						    skinNameController = PlayerSkinsController.GetSkinName(GameCommonData.Player);
						    GameCommonData.Player.Role.WeaponEffectName = skinNameController.WeaponEffectSkinName;
						    GameCommonData.Player.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
						    GameCommonData.Player.Role.WeaponDiaphaneity = skinNameController.WeaponDiaphaneity;
						    
						    PlayerSkinsController.SetSkinData(GameCommonData.Player,skinNameController);
						}
						else
						{
							if(element != null)
							{
								if(dwAttributeData == 1)
								{
								   element.Role.IsShowDress = true;
								}
								else
								{
									element.Role.IsShowDress = false;
								}
								
								if(!PlayerSkinsController.IsShowWeaponEffect(element.Role.WeaponSkinID))
								{
									element.Role.WeaponEffectModel = 0;
								}											
							    skinNameController = PlayerSkinsController.GetSkinName(element);
							    element.Role.WeaponEffectName = skinNameController.WeaponEffectSkinName;
							    element.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
							    element.Role.WeaponDiaphaneity = skinNameController.WeaponDiaphaneity;
							    
							    PlayerSkinsController.SetSkinData(element,skinNameController);
							}
						}
					break;
					case 222:		//副本引导是否可领掩码
					    copyLeadData.push(dwAttributeData);
					break;
					case 223:		//日程表是否完成掩码前32位
						calendarData.push(dwAttributeData);
					break;
					case 224:		//日程表是否完成掩码后32位
						calendarData.push(dwAttributeData);
						break;	
					case 226:
				        if(obj.idUser == GameCommonData.Player.Role.Id)
						{
							GameCommonData.Player.Role.archaeus = dwAttributeData;
							sendNotification(MeridiansEvent.UPDATA_ARCHEAUS);
						}
						break;
					case 227:
					   var player:GameElementAnimal = PlayerController.GetPlayer(obj.idUser);
					   if(player != null)
					   {
						   player.Role.PKteam = dwAttributeData;
						   player.Role.NameColor  = PKController.GetPKTeamFontColor(player.Role.PKteam);
						   player.Role.NameBorderColor =  PKController.GetPKTeamBorderColor(player.Role.PKteam);
						   player.UpdatePersonName();
						   if(player.Role.UsingPetAnimal != null)
						   {
						       player.Role.UsingPetAnimal.Role.PKteam = dwAttributeData;
							   player.Role.UsingPetAnimal.Role.NameColor  = PKController.GetPKTeamFontColor(player.Role.PKteam);
							   player.Role.UsingPetAnimal.Role.NameBorderColor =  PKController.GetPKTeamBorderColor(player.Role.PKteam);
							   player.Role.UsingPetAnimal.UpdatePersonName();
						   }
					   }
					   break;
					case 228:		// 荣誉值
						var me:GameElementAnimal = PlayerController.GetPlayer(obj.idUser);
						if (me)
						{
							me.Role.arenaScore = dwAttributeData;
							sendNotification(RoleEvents.UPDATE_OTHER_INFO, {target: "honor_txt", value: me.Role.arenaScore});
						}
						break;
					case 229:																							
					   me = PlayerController.GetPlayer(obj.idUser);
					   if(dwAttributeData == 1)
					   {
					   		FloorEffectController.AddElementEffect(me);
					   		me.Role.isAddEffect = true;
					   }
					   else
					   {				   	    
					   		FloorEffectController.RemoveElementEffect(me);
					   		me.Role.isAddEffect = false;
					   		if(me.Role.Type == GameRole.TYPE_OWNER)
					   		{
					   			if(PetPropConstData.isNewPetVersion && GameCommonData.Player.Role.UsingPet)
					   			{
//					   				if(sendNotification(PetEvent.RETURN_TO_SHOW_PET_INFO, eudemoninfo);
					   			}
					   		}
					   }
					   
					   
					   break
				}
			
				if(element!=null)
				{
					element.Role = tempRole;
					GameCommonData.SameSecnePlayerList[obj.idUser] = element;
					if(GameCommonData.TargetAnimal!=null && GameCommonData.TargetAnimal.Role.Id==obj.idUser)
					{
						GameCommonData.TargetAnimal=element;
					}
				}	
				sendNotification(EventList.ALLROLEINFO_UPDATE,{id:obj.idUser,type:1001});
				
			}
			
			if(bigDrugData.length > 0) {	//大红大蓝有更新
				var tmp:Array = bigDrugData;
				sendNotification(BigDrugEvent.BIG_DRUG_UPDATE, tmp);
				bigDrugData = [];
			}
			if(newPlayerAwardData.length > 0)	//新手成就大礼包
			{
				if(NewAwardMediator.SIMBOL_TAG)
				{
					NewAwardMediator.SIMBOL_TAG = false;
					sendNotification(NewAwardEvent.HANDLERDATA, newPlayerAwardData);
					newPlayerAwardData = [];
				}
			}
			if(copyLeadData.length > 0)		//玩法引导
			{
				if(CopyLeadMediator.SIMBOL_TAG)
				{
					CopyLeadMediator.SIMBOL_TAG = false;
					facade.sendNotification(CopyLeadEvent.HANDLERDATA , copyLeadData);
					copyLeadData = [];
				}
			}
			if(calendarData.length > 0)		// 日程表
			{
//				if(CalendarMediator.SIMBOL_TAG)
//				{
//					CalendarMediator.SIMBOL_TAG = false;
					facade.sendNotification(CampaignData.HANDLERDATA , calendarData);
					calendarData = [];
//				}
			}
		}
		
		
	}
}