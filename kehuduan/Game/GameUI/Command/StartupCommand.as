package GameUI.Command
{
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.LibsComponentMediator;
	import GameUI.Mediator.UILayerMediator;
	import GameUI.Modules.Alert.AlertMediator;
	import GameUI.Modules.Alert.TickAlertMediator;
	import GameUI.Modules.Answer.AnswerMediator;
	import GameUI.Modules.Answer.MasterAnswer.MasterAnswerMediator;
	import GameUI.Modules.AutoPathFind.AutoPathMediator;
	import GameUI.Modules.AutoPlay.mediator.AutoPlayMediator;
	import GameUI.Modules.AutoPlay.mediator.OffLinePlayMediator;
	import GameUI.Modules.AutoResize.Mediator.AutoSizeMediator;
	import GameUI.Modules.Bag.Mediator.BagMediator;
	import GameUI.Modules.BigDrug.Mediator.BigDrugMediator;
	import GameUI.Modules.Buff.Mediator.BuffMediator;
	import GameUI.Modules.Campaign.Mediator.CalendarMediator;
	import GameUI.Modules.Campaign.Mediator.CampaignMediator;
	import GameUI.Modules.CastSpirit.Mediator.CastSpiritMediator;
	import GameUI.Modules.CastSpirit.Mediator.CastSpiritTransferMediator;
	import GameUI.Modules.CastSpirit.Mediator.CastSpiritUpMediator;
	import GameUI.Modules.ChangeLine.ChangeLineMediator;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Mediator.BigLeoMediator;
	import GameUI.Modules.Chat.Mediator.ChatMediator;
	import GameUI.Modules.Chat.Mediator.ScrollNoticeMediator;
	import GameUI.Modules.CopyLead.Mediator.CopyLeadMediator;
	import GameUI.Modules.DragonEgg.Mediator.DragonEggMediator;
	import GameUI.Modules.Equipment.mediator.EquipmentMediator;
	import GameUI.Modules.Friend.view.mediator.FriendChatMediator;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.GmTools.Mediator.GmToolsMediator;
	import GameUI.Modules.Help.Mediator.HelpMediator;
	import GameUI.Modules.HeroSkill.Mediator.LearnLifeSkillMediator;
	import GameUI.Modules.HeroSkill.Mediator.LearnUnitySkillMediator;
	import GameUI.Modules.HeroSkill.Mediator.SkillLearnMediator;
	import GameUI.Modules.HeroSkill.Mediator.SkillMediator;
	import GameUI.Modules.Hint.Mediator.HintMediator;
	import GameUI.Modules.Icon.Mediator.IconMediator;
	import GameUI.Modules.IdentifyTreasure.Command.ShowTreasureCommand;
	import GameUI.Modules.IdentifyTreasure.Mediator.IdentifyTreaMediator;
	import GameUI.Modules.Login.StartMediator.CreateRoleMediatorYl;
	import GameUI.Modules.MainSence.Command.ItemCdCommand;
	import GameUI.Modules.MainSence.Command.ProcessQuickBarCommand;
	import GameUI.Modules.MainSence.Mediator.MainSenceMediator;
	import GameUI.Modules.Maket.Mediator.MarketMediator;
	import GameUI.Modules.Manufactory.Mediator.ManufactoryMediator;
	import GameUI.Modules.Map.BigMap.BigMapMediator;
	import GameUI.Modules.Map.SenceMap.SenceMapMediator;
	import GameUI.Modules.Map.SmallMap.Mediator.SmallMapMediator;
	import GameUI.Modules.Master.Mediator.BetrayMediator;
	import GameUI.Modules.Master.Mediator.MasStuMainMediator;
	import GameUI.Modules.Master.Mediator.MasterAwardMediator;
	import GameUI.Modules.Master.Mediator.MasterMediator;
	import GameUI.Modules.Master.Mediator.StudentListMediator;
	import GameUI.Modules.MusicPlayer.Mediator.MusicPlayerMediator;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessConstData;
	import GameUI.Modules.NPCChat.Mediator.NPCChatMediator;
	import GameUI.Modules.NewPlayerCard.Mediator.NewPlayerCardMediator;
	import GameUI.Modules.NewPlayerSuccessAward.Mediator.NewAwardMediator;
	import GameUI.Modules.NewerHelp.Mediator.NewerHelpMediator;
	import GameUI.Modules.OnlineGetReward.Mediator.GainAwardMediator;
	import GameUI.Modules.OnlineGetReward.Mediator.OnlineRewardMediator;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Mediator.PetPropMediator;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnConstData;
	import GameUI.Modules.Pick.Mediator.PickMediator;
	import GameUI.Modules.Pk.Mediator.PkMediator;
	import GameUI.Modules.PlayerInfo.Command.AllRoleInfoUpdateCommand;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.PlayerInfo.Command.TeamPosProcessCommand;
	import GameUI.Modules.PlayerInfo.ToolTipMediator;
	import GameUI.Modules.PrepaidLevel.Mediator.PrepaidLevelMediator;
	import GameUI.Modules.PreventWallow.Mediator.PreventWallowMediator;
	import GameUI.Modules.Rank.Mediator.RankMediator;
	import GameUI.Modules.Relive.Mediator.ReliveMediator;
	import GameUI.Modules.RoleProperty.Mediator.RoleMediator;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Stall.Mediator.StallMediator;
	import GameUI.Modules.SystemMessage.Command.SystemMessageCommand;
	import GameUI.Modules.SystemMessage.Mediator.SysCotentMediator;
	import GameUI.Modules.SystemMessage.Mediator.SystemMediator;
	import GameUI.Modules.SystemSetting.mediator.SettingMediator;
	import GameUI.Modules.Task.Commamd.TaskSeverMediator;
	import GameUI.Modules.Task.Mediator.TaskFollowMediator;
	import GameUI.Modules.Task.Mediator.TaskInfoMediator;
	import GameUI.Modules.Task.Mediator.TaskMediator;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Team.Mediator.TeamMediator;
	import GameUI.Modules.TimeCountDown.Mediator.TimeCountDownMediator;
	import GameUI.Modules.ToolTip.Mediator.ItemPanelMediator;
	import GameUI.Modules.ToolTip.Mediator.ItemToolTipMediator;
	import GameUI.Modules.Trade.Mediator.TradeMediator;
	import GameUI.Modules.Unity.Mediator.*;
	import GameUI.Modules.UnityNew.Mediator.NewCreateUnityMediator;
	import GameUI.Modules.VipHeadIcon.view.mediator.VipHeadIconMediator;
	import GameUI.Modules.VipShow.VipShowMediator.VipShowMediator;
	import GameUI.Modules.Wish.Meidator.WishMediator;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillBuff;
	import OopsEngine.Skill.JobGameSkill;
	import OopsEngine.Skill.WeirdyGameSkill;
	
	import OopsFramework.Debug.Logger;
	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class StartupCommand extends SimpleCommand
	{
		public function StartupCommand()
		{
			super();
			this.initializeNotifier(UIFacade.FACADEKEY);
		}
		
		public override function execute(notification:INotification):void 
		{
			Logger.Info(this, "execute", "MVC已经启动");
			Logger.Info(this, "execute", "解析XML数据");
			//解析套装
			var coordinatesEquip_xml:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Cloth_XML_SWF).GetDisplayObject()["coordinatesEquip_xml"] as XML;
			analyseCoordinatesEquip( coordinatesEquip_xml );
			//解析经验
			var exp_xml:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["exp_xml"] as XML;
			analyseExpList( exp_xml );
			//解析武魂数据
			var soul:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["Soul_xml"] as XML;
			analyseSoul(soul);
			//解析技能
			var gameSkill_xml:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Skill_XML_SWF).GetDisplayObject()["gameSkill_xml"] as XML;
			analyseGameSkill( gameSkill_xml );
			//解析怪物技能
			var weirdyGameSkill_xml:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Skill_XML_SWF).GetDisplayObject()["weirdyGameSkill_xml"] as XML;
			analyseWeirdyGame( weirdyGameSkill_xml );
			//解析答题
			var question_xml:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["question_xml"] as XML;
			analyseQuestion( question_xml );
			//解析生活技能
			var lifeSkill_xml:XML = GameCommonData.GameInstance.Content.Load(GameConfigData.Skill_XML_SWF).GetDisplayObject()["lifeSkill_xml"] as XML;
			analyseLifeSkill( lifeSkill_xml );
			//解析称号
			var designation_xml:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.ModelOffset_XML_SWF ).GetDisplayObject()["designation_xml"];
			analyseDesignation( designation_xml );
			//解析通知
			if( GameCommonData.noticeFarmat == 2 )
			{
				var notice_xml:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.NOTICE_DATA[1] ).GetXML();
				analyseNotice( notice_xml );
			}
			/**宠物模型配置数据*/
			PetPropConstData.newPetModuleInfo = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["moduleInfo"];
			PetPropConstData.newPetModuleSwf = GameCommonData.GameInstance.Content.Load(GameConfigData.Other_XML_SWF).GetDisplayObject()["moduleSwf"];
//			analyseCoordinatesEquip(GameCommonData.GameInstance.Content.Load(GameConfigData.CoordinatesEquip).GetXML());
//			analyseExpList(GameCommonData.GameInstance.Content.Load(GameConfigData.ExpList).GetXML());
//			analyseGameSkill(GameCommonData.GameInstance.Content.Load(GameConfigData.GameSkillList).GetXML());
			analyseTaskInfo(UIConstData.TaskTempInfo);
//			analyseWeirdyGame(GameCommonData.GameInstance.Content.Load(GameConfigData.WeirdyGameSkillList).GetXML());
			analyseJobGame();
			
			/** 初始化宠物技能数据 */
			PetSkillLearnConstData.SKILL_DATA_PET = GameCommonData.GameInstance.Content.Load( GameConfigData.Skill_XML_SWF ).GetDisplayObject()["PET_SKILL_DATA"];
//			analyseQuestion(GameCommonData.GameInstance.Content.Load(GameConfigData.AnswerQuestion).GetXML());
//			analyseLifeSkill(GameCommonData.GameInstance.Content.Load(GameConfigData.LifeSkillList).GetXML());
//			analyseDesignation(GameCommonData.GameInstance.Content.Load(GameConfigData.Designation).GetXML());
			initNPCBusinessGoodConstData();
			register();
			/** 所有的角色注册完成 */
			facade.sendNotification(EventList.REGISTERCOMPLETE);
			Logger.Info(this, "execute", "MVC注册完成");
			//调用js，统计人数
			if ( UIConstData.Filter_Switch )
			{
				UIUtils.callJava( "joingamenum" );
			}
		}
		
		/** 初始化跑商商品字典 */
		private function initNPCBusinessGoodConstData():void
		{
			var dic:Dictionary = new Dictionary();
			dic[626001] = {base:1042, low:.06, middle:.14, high:.2};
			dic[626006] = {base:1042, low:.06, middle:.14, high:.2};
			dic[626007] = {base:1042, low:.06, middle:.14, high:.2};
			dic[626011] = {base:1042, low:.06, middle:.14, high:.2};
			dic[626014] = {base:1042, low:.06, middle:.14, high:.2};
			dic[626015] = {base:1042, low:.06, middle:.14, high:.2};
			
			dic[626002] = {base:1563, low:.06, middle:.14, high:.2};
			dic[626005] = {base:1563, low:.06, middle:.14, high:.2};
			dic[626008] = {base:1563, low:.06, middle:.14, high:.2};
			dic[626012] = {base:1563, low:.06, middle:.14, high:.2};
			dic[626019] = {base:1563, low:.06, middle:.14, high:.2};
			dic[626016] = {base:1563, low:.06, middle:.14, high:.2};
			
			dic[626003] = {base:3125, low:.06, middle:.14, high:.2};
			dic[626004] = {base:3125, low:.06, middle:.14, high:.2};
			dic[626017] = {base:3125, low:.06, middle:.14, high:.2};
			dic[626013] = {base:3125, low:.06, middle:.14, high:.2};
			dic[626020] = {base:3125, low:.06, middle:.14, high:.2};
			dic[626009] = {base:3125, low:.06, middle:.14, high:.2};
			
			dic[626018] = {base:5209, low:.06, middle:.14, high:.2};
			dic[626010] = {base:5209, low:.06, middle:.14, high:.2};
			
			NPCBusinessConstData.GOOD_DIC = dic;
		}
		
		/** 问答题 */
		protected function analyseQuestion(xml:XML):void{
			for(var i:int=0;i<xml.QuestionItem.length();i++){
				var obj:Object={};
				obj.Question=xml.QuestionItem[i].Question;
				obj.Answer1=xml.QuestionItem[i].Answer1;
				obj.Answer2=xml.QuestionItem[i].Answer2;
				obj.Answer3=xml.QuestionItem[i].Answer3;
				obj.Right=xml.QuestionItem[i].Right;
				obj.Id=xml.QuestionItem[i].Id;
				
				GameCommonData.QuestionDic[uint(obj.Id)]=obj;
			}
			for(var j:int = 0;j<xml.JobItem.length();j++)
			{
				var ob:Object = {};
				ob.Question = xml.JobItem[j].@Question;
				ob.SA = xml.JobItem[j].@SA;
				ob.SB = xml.JobItem[j].@SB;
				ob.Id = xml.JobItem[j].@Id;
				GameCommonData.SelectQueDic[uint(ob.Id)] = ob;
			}
		}
		
		protected function analyseJobGame():void
		{
			//唐门 
			var jobTM:JobGameSkill = new JobGameSkill();		
			jobTM.JobID 		      =  1;
			jobTM.SkillID             =  9501;
			jobTM.DressID             =  230601;
			GameCommonData.JobGameSkillList[jobTM.JobID] = jobTM;
			
			//全真
			var jobQZ:JobGameSkill = new JobGameSkill();		
			jobQZ.JobID 		      =  2;
			jobQZ.SkillID             =  9502;
			jobQZ.DressID             =  230201;
			GameCommonData.JobGameSkillList[jobQZ.JobID] = jobQZ;
			
			//峨眉
			var jobEM:JobGameSkill = new JobGameSkill();		
			jobEM.JobID 		      =  4;
			jobEM.SkillID             =  9503;
			jobEM.DressID             =  230101;
			GameCommonData.JobGameSkillList[jobEM.JobID] = jobEM;
			
			//丐帮
			var jobGB:JobGameSkill = new JobGameSkill();		
			jobGB.JobID 		      =  8;
			jobGB.SkillID             =  9504;
			jobGB.DressID             =  230301;
			GameCommonData.JobGameSkillList[jobGB.JobID] = jobGB;
			
			//少林
			var jobSL:JobGameSkill = new JobGameSkill();		
			jobSL.JobID 		      =  16;
			jobSL.SkillID             =  9505;
			jobSL.DressID             =  230501;
			GameCommonData.JobGameSkillList[jobSL.JobID] = jobSL;
			
			//点苍
			var jobDC:JobGameSkill = new JobGameSkill();		
			jobDC.JobID 		      =  32;
			jobDC.SkillID             =  9506;
			jobDC.DressID             =  230401;
			GameCommonData.JobGameSkillList[jobDC.JobID] = jobDC;
			
			//新手
			var jobXS:JobGameSkill = new JobGameSkill();		
			jobXS.JobID 		      =  4096;
			jobXS.SkillID             =  9507;
			jobXS.DressID             =  0;
			GameCommonData.JobGameSkillList[jobXS.JobID] = jobXS;

//            GameCommonData.ModelOffsetSkill["ZD002"] = new Point(25,30);
//            GameCommonData.ModelOffsetSkill["ZD001"] = new Point(26,0);
            
//			for(var i:int = 0; i<xml.Skill.length(); i++)
//			{
//				var jobGameSkill:JobGameSkill = new JobGameSkill();		
//				jobGameSkill.JobID 		      = xml.Skill[i].@JobID;
//				jobGameSkill.SkillID          = xml.Skill[i].@SkillID;
//				GameCommonData.JobGameSkillList[jobGameSkill.JobID] = jobGameSkill;
//			}
		}
		
		protected function analyseWeirdyGame(xml:XML):void
		{
			for(var i:int = 0; i<xml.Skill.length(); i++)
			{
				var weirdyGameSkill:WeirdyGameSkill = new WeirdyGameSkill();		
				weirdyGameSkill.SkillID 		    = xml.Skill[i].@SkillID;
				weirdyGameSkill.WeirdyID          	= xml.Skill[i].@WeirdyID;
				GameCommonData.WeirdyGameSkillList[weirdyGameSkill.WeirdyID] = weirdyGameSkill;
			}
		}
		
		/**
		 * 解析任务XML 
		 * @param xml
		 * 
		 */		
		protected function analyseTaskInfo(dic:Dictionary):void{
			for(var id:* in dic){
				var str:String="";
				var task:TaskInfoStruct=new TaskInfoStruct();
				task.id=dic[id].id;
				task.processMask=dic[id].processMask;
				task.isChain=dic[id].isChain;
				task.maskIndex=dic[id].maskIndex;
				task.step=dic[id].step;
				task.taskArea=dic[id].taskArea;
				task.taskDes=dic[id].taskDes;
				task.taskFinish=dic[id].taskFinish;
				task.taskCloneFinish=task.taskFinish;
				task.taskId=dic[id].taskId;
				task.taskLevel=dic[id].taskLevel;
				task.taskName=dic[id].taskName;
				task.taskType=dic[id].taskType;
				task.taskNpcId=dic[id].taskNpcId;
				task.taskCommitNpcId=dic[id].taskCommitNpcId;
				task.taskNPC=dic[id].taskNPC;
				task.taskCommitNPC=dic[id].taskCommitNPC;
				
				task.taskProcess1=dic[id].taskProcess1;
				task.taskProcess2=dic[id].taskProcess2;	
				task.taskProcess3=dic[id].taskProcess3;
				task.taskProcess4=dic[id].taskProcess4;
				task.taskProcess5=dic[id].taskProcess5;
				task.taskProcess6=dic[id].taskProcess6;
				task.taskTime=dic[id].taskTime;
				task.taskTip=dic[id].taskTip;
				task.taskCloneTip=task.taskTip;
				task.taskUnFinish=dic[id].taskUnFinish;
				task.taskCloneUnFinish=task.taskUnFinish;
				task.taskProcessFinish=dic[id].taskProcessFinish;
				task.taskCloneProcessFinish=task.taskProcessFinish;
				task.type=dic[id].type;
				task.taskPrize1=dic[id].taskPrize1;
				task.taskPrize2=dic[id].taskPrize2;
				str=dic[id].taskPrize3;
				var realTaskProze3:Array=[];
				
				if(str==null ||str==""){
					
				}else{
					var goods:Array=str.split("@");
					realTaskProze3.push(goods.shift());
					
					for each(var obj:String in goods){
						var tempArr:Array=obj.split("*");
						if(tempArr.length==2){
							realTaskProze3.push({goodId:tempArr[0],goodNum:tempArr[1]});
						}else if(tempArr.length==3){
							realTaskProze3.push({goodId:tempArr[0],goodNum:tempArr[1],goodJob:tempArr[2]});
						}
					}
				}
				task.taskProze3=realTaskProze3;
				task.taskExpandType=dic[id].taskExpandType;
				if(dic[id].taskRandBaseExp){
					task.taskRandBaseExp=dic[id].taskRandBaseExp;
				}
				if(dic[id].taskRandBaseMoney){
					task.taskRandBaseMoney=dic[id].taskRandBaseMoney;
				}
				if(id>10001000){
					task.taskExpX=dic[id].taskExpX;
					task.taskMoneyX=dic[id].taskMoneyX;
				}
				GameCommonData.TaskInfoDic[task.id]=task;	
			}
		
		}
			
		
		/**读取技能信息*/
		private function analyseGameSkill(xml:XML):void
		{
			for(var i:int = 0; i<xml.Skill.length(); i++)
			{
				var gameskill:GameSkill = new GameSkill();
				gameskill.SkillID 		= xml.Skill[i].@SkillID;
				gameskill.SkillName     = xml.Skill[i].@SkillName;
				gameskill.SkillReamark  = xml.Skill[i].@SkillReamark;	
				gameskill.Job           = xml.Skill[i].@Job;	
				gameskill.NeedLevel     = xml.Skill[i].@NeedLevel; 
				gameskill.Distance  	= xml.Skill[i].@Distance;
				gameskill.Effect 		= xml.Skill[i].@Effect;
				gameskill.HitEffect 	= xml.Skill[i].@HitEffect;			
				gameskill.Exp 	        = xml.Skill[i].@Exp;
				gameskill.SkillMode 	= xml.Skill[i].@SkillMode;
				gameskill.SkillArea     = xml.Skill[i].@SkillArea;	
				gameskill.BookID        = xml.Skill[i].@BookID;
				gameskill.SP            = xml.Skill[i].@SP;
				gameskill.CoolTime      = xml.Skill[i].@CoolTime; 	
				gameskill.LevelCoolTime = xml.Skill[i].@LevelCoolTime; 				
				gameskill.MP            = xml.Skill[i].@MP; 
				gameskill.LevelMP       = xml.Skill[i].@LevelMP; 
				gameskill.Attack        = xml.Skill[i].@Attack; 
				gameskill.LevelAttack   = xml.Skill[i].@LevelAttack; 
				gameskill.Buff          = xml.Skill[i].@Buff;
                gameskill.StartEffect   = xml.Skill[i].@StartEffect;
                
				if(gameskill.Buff != 0)
				{
				   var gameBuff:GameSkillBuff  = new GameSkillBuff();
				   gameBuff.BuffName = xml.Skill[i].@BuffName;
				   gameBuff.BuffType = xml.Skill[i].@BuffType;
				   gameBuff.TypeID   = xml.Skill[i].@BuffTypeID;	   
				   GameCommonData.BuffList[gameskill.Buff] = gameBuff;
				}	
				GameCommonData.Skills.push(gameskill.SkillID);
				GameCommonData.SkillList[gameskill.SkillID] = gameskill;
			}
			
			for(i= 0; i<xml.talk.length(); i++)
			{
				var bossTalk:Object = new Object();
				bossTalk.type 		= int(xml.talk[i].@type);
				bossTalk.skillID1   = xml.talk[i].@skillID1.toString();   
				bossTalk.skill1     = xml.talk[i].@skill1.toString();
				bossTalk.skillID2   = xml.talk[i].@skillID2.toString();   
				bossTalk.skill2     = xml.talk[i].@skill2.toString();      
				bossTalk.skillID3   = xml.talk[i].@skillID3.toString();   
				bossTalk.skill3     = xml.talk[i].@skill3.toString();    
				bossTalk.begin      = xml.talk[i].@begin.toString();  
				bossTalk.hp         = xml.talk[i].@hp.toString(); 
				bossTalk.dead       = xml.talk[i].@dead.toString(); 
				bossTalk.percent    = int(xml.talk[i].@percent); 
				GameCommonData.BossTalk[bossTalk.type] = bossTalk;
			}
		}
		
		private function analyseSoul(soul:XML):void
		{
			SoulData.other = new Object();
			SoulData.other.pcgold = soul.other[0].@pcgold;  // 属相重洗
			SoulData.other.scgold = soul.other[0].@scgold;  // 技能重洗 
			SoulData.other.addAttributes = soul.other[0].@addAttributes;  // 扩展属性所需金钱
			SoulData.other.studyAttributes = soul.other[0].@studyAttributes; // 学习扩展属性
		    SoulData.other.changeAttributes = soul.other[0].@changeAttributes; // 重洗扩展属性
		    SoulData.other.firstSkill = soul.other[0].@firstSkill; // 第1个技能所花金钱
		    SoulData.other.secondSkill = soul.other[0].@secondSkill;// 第2个技能所花金钱
		    SoulData.other.thirdSkill = soul.other[0].@thirdSkill;// 第3个技能所花金钱 
			
			// 宝石合成 
			for(var i:int = 0;i<soul.jade.lev.length(); i++)
			{
				var jadeInfo:Object = new Object();
				jadeInfo.lev        = soul.jade.lev[i].@lev; 
				jadeInfo.gold       = soul.jade.lev[i].@gold;
				SoulData.jade.push(jadeInfo);
			}
				
		    // 武魂合成
			for(var j:int = 0;j<soul.compound.lev.length(); j++)
			{
				var compoundInfo:Object = new Object();
				compoundInfo.lev       = soul.compound.lev[j].@lev;
				compoundInfo.gold      = soul.compound.lev[j].@gold;
				SoulData.compound.push(compoundInfo);
			}
			
			// 成长率
			for(var z:int = 0; z<soul.growth.com.length(); z++)
			{
				var growthInfo:Object = new Object();
				growthInfo.min        = soul.growth.com[z].@min;
				growthInfo.max        = soul.growth.com[z].@max;
				growthInfo.gold       = soul.growth.com[z].@gold;		
				SoulData.growth.push(growthInfo);													
			}	
			
			// 扩展属性
			for(var tag1:int = 0; tag1<soul.extendedAttributes.lev.length(); tag1++)
			{
				var attributesInfo:Object = new Object();
				attributesInfo.lev             = soul.extendedAttributes.lev[tag1].@lev;
				attributesInfo.addcombining    = soul.extendedAttributes.lev[tag1].@addcombining;//开启扩展属性成功率
				attributesInfo.upGold          = soul.extendedAttributes.lev[tag1].@upGold; //升级扩展属性金钱
				SoulData.attributesManagement.push(attributesInfo);																			
			}

			// 升级技能金钱
			for(var tag2:int = 0;tag2<soul.skill.lev.length(); tag2++)
			{
				var skillInfo:Object = new Object();
				skillInfo.gold       = soul.skill.lev[tag2].@gold;
				SoulData.skill.push(skillInfo);
			}
			

			// 技能换算属性
			for(var tag3:int=0;tag3<soul.skillAttack.skill.length(); tag3++)
			{
				var skillattackInfo:Object = new Object();
				skillattackInfo.skillid = soul.skillAttack.skill[tag3].@skillid;
				skillattackInfo.skillName = soul.skillAttack.skill[tag3].@skillName;
				skillattackInfo.levparam = soul.skillAttack.skill[tag3].@levparam;
				skillattackInfo.mixparam = soul.skillAttack.skill[tag3].@mixparam;
				skillattackInfo.isSoul = soul.skillAttack.skill[tag3].@isSoul;
				SoulData.skillAttack[int(skillattackInfo.skillid)] = skillattackInfo;
			}
		}
		
		private function analyseProperties(xml:XML):void
		{
			Logger.Print(this,"共加载" + xml.Properties.length() + "数据");
			for(var i:int = 0; i<xml.Properties.length(); i++)
			{
				var obj:Object  = new Object();
				obj.id 		    = int(xml.Properties[i].@Id);
				obj.type 	    = int(xml.Properties[i].@TypeId);
				obj.Name 	    = String(xml.Properties[i].@Name);
				obj.Job 	    = int(xml.Properties[i].@Job);
				obj.Sex		    = int(xml.Properties[i].@Sex);
				obj.Level 	    = int(xml.Properties[i].@Level); //物品的使用等级 
				obj.PlayerLevel = int(xml.Properties[i].@PlayerLevel);
				obj.UpperLimit  = int(xml.Properties[i].@UpperLimit);
				obj.Time 		= int(xml.Properties[i].@Time);
				obj.HP 			= int(xml.Properties[i].@HP); 
				obj.MP 			= int(xml.Properties[i].@MP);
				obj.SP 			= int(xml.Properties[i].@SP);
				obj.PriceIn 	= uint(xml.Properties[i].@PriceIn);		
				obj.PriceOut 	= uint(xml.Properties[i].@PriceOut);				
				obj.UseType		= xml.Properties[i].@UseType;
				obj.Registering = int(xml.Properties[i].@Registering);
				obj.BaseList = [
							int(xml.Properties[i].@BaseId1),
							int(xml.Properties[i].@BaseId2)
				];
				obj.AALIST = [
							int(xml.Properties[i].@AAId1),
							int(xml.Properties[i].@AAId2),
							int(xml.Properties[i].@AAId3),
							int(xml.Properties[i].@AAId4),
							int(xml.Properties[i].@AAId5),
							int(xml.Properties[i].@AAId6),
							int(xml.Properties[i].@AAId7),
							int(xml.Properties[i].@AAId8)	
				];
				obj.Effect = xml.Properties[i].@Effect;
				obj.description = xml.Properties[i];
			}	
		}
		
		private function analyseAppendAttribute(xml:XML):void
		{
			for(var i:int = 0; i<xml.AppendAttribute.length(); i++)
			{
				var obj:Object = new Object();
				obj.id = int(xml.AppendAttribute[i].@Id);
				obj.TypeId = int(xml.AppendAttribute[i].@TypeId);
				obj.Name = String(xml.AppendAttribute[i].@Name);
				obj.Value = int(xml.AppendAttribute[i].@Value);
				UIConstData.AppendAttribute[obj.id] = obj;
			}
		}
		
		private function analyseCoordinatesEquip(xml:XML):void
		{
			for(var i:int = 0; i<xml.CoordinatesEquip.length(); i++)
			{
				var obj:Object = new Object();
				obj.id = int(xml.CoordinatesEquip[i].@Id);
				obj.Count = int(xml.CoordinatesEquip[i].@Count);
				obj.Name = String(xml.CoordinatesEquip[i].@Name);
				obj.EquidList = [
							String(xml.CoordinatesEquip[i].@EId1),
							String(xml.CoordinatesEquip[i].@EId2),
							String(xml.CoordinatesEquip[i].@EId3),
							String(xml.CoordinatesEquip[i].@EId4),
							String(xml.CoordinatesEquip[i].@EId5),
							String(xml.CoordinatesEquip[i].@EId6),
							String(xml.CoordinatesEquip[i].@EId7),
							String(xml.CoordinatesEquip[i].@EId8)
				];
				obj.CeAAList = [
							int(xml.CoordinatesEquip[i].@CEAAId1),
							int(xml.CoordinatesEquip[i].@CEAAId2),
							int(xml.CoordinatesEquip[i].@CEAAId3),
							int(xml.CoordinatesEquip[i].@CEAAId4),
							int(xml.CoordinatesEquip[i].@CEAAId5),
							int(xml.CoordinatesEquip[i].@CEAAId6),
							int(xml.CoordinatesEquip[i].@CEAAId7),
							int(xml.CoordinatesEquip[i].@CEAAId8)
				]
				UIConstData.CoordinatesEquip[obj.id] = obj;
			}
		}
		
		private function analyseExpList(xml:XML):void
		{
			for(var i:int = 0; i<xml.Level.length(); i++)
			{
				var level:int = int(xml.Level[i].@Id);
				UIConstData.ExpDic[level] = int(xml.Level[i]);
			}
			var data:Dictionary = UIConstData.ExpDic;
		}
		
		// 解析生活采集技能数据
		private function analyseLifeSkill(xml:XML):void
		{
			for(var i:int = 0; i<xml.Skill.length(); i++)
			{
				var gameskill:GameSkill = new GameSkill();
				gameskill.SkillID 		= xml.Skill[i].@SkillID;
				gameskill.SkillName     = xml.Skill[i].@SkillName;
				gameskill.SkillReamark  = xml.Skill[i].@SkillReamark;	
				gameskill.Job           = xml.Skill[i].@Job;	
				gameskill.NeedLevel     = xml.Skill[i].@NeedLevel; 
				gameskill.SkillMode 	= xml.Skill[i].@SkillMode;
				gameskill.BookID        = xml.Skill[i].@BookID;
				gameskill.SP            = xml.Skill[i].@SP;
				gameskill.CoolTime      = xml.Skill[i].@CoolTime; 	
				gameskill.LevelCoolTime = xml.Skill[i].@LevelCoolTime; 				
				gameskill.MP            = xml.Skill[i].@MP; 
				gameskill.LevelMP       = xml.Skill[i].@LevelMP; 
				gameskill.Buff          = xml.Skill[i].@Buff;
                gameskill.StartEffect   = xml.Skill[i].@StartEffect;
                
				GameCommonData.LifeSkillList[gameskill.SkillID] = gameskill;
			}
		}
		
		/**解析称号xml*/
		private function analyseDesignation(xml:XML):void
		{
		 	var DesingnationArr:Array = GameCommonData.Designation;
			 for each(var xmlItem:XML in xml.children())
			{
				var objItem:Object = {};
				objItem.name = xmlItem.@name;
				objItem.num = xmlItem.@num;
				objItem.explain = xmlItem.@explain;
				objItem.data = [];
				for each(var xmlElement:XML in xmlItem.element)
				{
					var obj:Object = {};
					obj.id = xmlElement.@id;
					obj.extend = xmlElement.@extend;
					obj.level = xmlElement.@level;
					obj.color = xmlElement.@color;
					obj.borderColor = xmlElement.@borderColor;
					obj.name = xmlElement.@dName;
					obj.isSelect = xmlElement.@isSelect;
					obj.introduce = xmlElement.@introduce;
					obj.condition = xmlElement.@condition;
					objItem.data.push(obj);
				}
				DesingnationArr.push(objItem);
			} 
		}
		
		/**解析通知xml*/
		private function analyseNotice(xml:XML):void
		{
			var reg:RegExp = /FAT_URL/g;
			ChatData.CHAT_COLORS = new Array();
			for each(var element1:XML in xml.notice_colors.color)
			{
				ChatData.CHAT_COLORS.push( uint(element1.toString()) );
			}
			
			ChatData.NOTICE_ARR = new Array();
			for each(var element2:XML in xml.notice_help.arr)
			{
				ChatData.NOTICE_ARR.push(element2.toString());
			}
			
			ChatData.WELCOME_ARR = new Array();
			for each(var element3:XML in xml.welcome.arr)
			{
				ChatData.WELCOME_ARR.push(element3.toString());
			}
			
			ChatData.WELCOME_INTERVAL = xml.welcome.interval[0];
			ChatData.NOTICE_HELP_INTERVAL = xml.notice_help.interval[0];
			
			ChatData.CHEAT_CHAT_FILTER = new Array();
			for each(var element4:XML in xml.chat_filter.obj)
			{
				ChatData.CHEAT_CHAT_FILTER.push( {rep:element4.@rep, point:element4.@point} );
			}
			
			ChatData.SERVICE_BUSINESS_ID = xml.service_business_id;
			ChatData.FAT_URL = xml.fat.fat_test_url;
//			var str:String = xml.fat.fat_url;
			var s:String = xml.fat.fatigue_str;
			ChatData.FATIGUE_STR = s.replace( reg, xml.fat.fat_url );
			ChatData.OFFICIAL_WEBSITE_ADDR = xml.official_website;
			ChatData.FORUM_WEBSITE_ADDR = xml.forum_website;
			ChatData.DEPOSIT_WEBSITE_ADDR = xml.deposit_website;
			ChatData.GM_INTERFACE_ADDR = xml.gm;
			ChatData.NEWER_CARD_INTERFACE_ADDR = xml.newer_card;
			ChatData.FAT_TEST_URL = xml.fat.fat_url;
			ChatData.FAT_CODE = xml.fat.fat_code;
			
			ChatData.GAME_SCROLL_NOTICE_DIC = new Dictionary();
			var str:String;
			for each(var element5:XML in xml.scroll_notices.scroll_notice)
			{
				str = element5.@key;
				ChatData.GAME_SCROLL_NOTICE_DIC[str] = element5.toString();
			}
		}
		
		private function register():void
		{
			facade.registerProxy(new DataProxy());
			/**注册UILayer*/
			facade.registerMediator(new UILayerMediator());
			/** 注册资源管理器  */
			facade.registerMediator(new LibsComponentMediator());
////////////////////////////////////////////////////////////////////////
//WK jinian
			/**注册Login*/
//			facade.registerMediator(new LoginMediator());
			/**注册选择角色*/
//			facade.registerMediator(new SelectRoleMediator());	
////////////////////////////////////////////////////////////////////////
			/**注册主场景*/
			facade.registerMediator(new MainSenceMediator());				
			/**注册背包*/
			facade.registerMediator(new BagMediator());
			/** 注册组队UI */
			facade.registerMediator(new TeamMediator());
			/** 注册交易UI */
			facade.registerMediator(new TradeMediator());
			/** 注册摆摊 */
			facade.registerMediator(new StallMediator());
			/** 注册聊天 */
			facade.registerMediator(new ChatMediator());
			/** 注册警告 */
			facade.registerMediator(new AlertMediator());
			/** 注册任务属性面板  */ 
			facade.registerMediator(new RoleMediator());	
			/** 注册好友 */
			facade.registerMediator(new FriendManagerMediator());			
			/** 注册玩家技能  */
			facade.registerMediator(new SkillMediator());
			/** 注册学习技能  */
			facade.registerMediator(new SkillLearnMediator());
			/** 宠物  */
			facade.registerMediator(new PetPropMediator());
			/** 任务  */
			facade.registerMediator(new TaskMediator());
			/** 接收与提交任务*/
			facade.registerMediator(new TaskInfoMediator());
			/** 帮派  */
//			facade.registerMediator(new UnityMediator());
			/** 申请帮派 */
//			facade.registerMediator(new ApplyUnityMediator());
			/** 创建帮派 */
//			facade.registerMediator(new CreateUnityMediator()); 
			/** 新创建帮派 */
			facade.registerMediator(new NewCreateUnityMediator() ); 
			/** 帮派信息 */
//			facade.registerMediator(new UnityInfoMediator());
			/** 申请列表 */
//			facade.registerMediator(new ApplyListMediator());
			/** 帮派任命功能 */
//			facade.registerMediator(new OrdainMediator());
			/** 修改帮派通知功能 */
//			facade.registerMediator(new PerfectNoticeMediator());
			/** 帮派捐献功能 */
//			facade.registerMediator(new ContributeMediator());
			/** 响应帮派 */
//			facade.registerMediator(new RespondUnityMediator());
			/** 创建中的帮派面板 */
//			facade.registerMediator(new CreatintMediator());
			///////////////////////////////////
//			/** 收消息*/
//			facade.registerMediator(new ReceiveMsgMediator());
			/** 场景提示  */
			facade.registerMediator(new HintMediator());
			/** 拾取界面  */
			facade.registerMediator(new PickMediator());
			/** 注册提示面板 */
			facade.registerMediator(new ToolTipMediator());
			/** 注册toolTip  */
			facade.registerMediator(new ItemToolTipMediator());		
			/** 注册NPC对话*/
			facade.registerMediator(new NPCChatMediator());	
			/** 角色信息更新*/
			facade.registerCommand(EventList.ALLROLEINFO_UPDATE, AllRoleInfoUpdateCommand);
			/** 显示宠物系列面板命令 */
			facade.registerCommand(EventList.SHOW_PET_PLAYRULE_VIEW, ShowPetRuleBaseCommond); 
			/** 商城 */
			facade.registerMediator(new MarketMediator()); 
			
			/** 死亡复活 */
			facade.registerMediator(new ReliveMediator());
			/** 物品提示框  */
			facade.registerMediator(new ItemPanelMediator());
			/** 任务跟踪 */
			facade.registerMediator(new TaskFollowMediator());
			/** 小地图UI  */
			facade.registerMediator(new SmallMapMediator());
			/** 大地图UI  */
			facade.registerMediator(new BigMapMediator());
			/** 场景地图UI  */
			facade.registerMediator(new SenceMapMediator());
			
			/** 排行榜 */
			facade.registerMediator(new RankMediator());
		
			/** 答题*/
			facade.registerMediator(new AnswerMediator());
			/** 自动寻路*/
			facade.registerMediator(new AutoPathMediator());
			/** 收到快捷栏信息*/
			facade.registerCommand(EventList.RECEIVE_QUICKBAR_MSG,ProcessQuickBarCommand);
			/** 注册任务模拟服务器  */
			facade.registerMediator(new TaskSeverMediator());
			 
			facade.registerMediator(new CreateRoleMediatorYl());
//			facade.registerMediator(new SelectRoleMediatorYL());			//暂时不用
			/** 江湖指南 */
			facade.registerMediator(new HelpMediator());
			/** 注册滚屏公告  */
			facade.registerMediator(new ScrollNoticeMediator());
			/** 注册宝石强化  */
//			facade.registerMediator(new StoneComposeMediator());
			/** 注册 BUFF UI */
			facade.registerMediator(new BuffMediator());
			/** 注册PK */
			facade.registerMediator(new PkMediator());
			/** 注册大血大蓝显示 */
			facade.registerMediator(new BigDrugMediator()); 
			/** 注册CD时间 */
			facade.registerCommand(EventList.RECEIVE_COOLDOWN_MSG,ItemCdCommand);
			/** 注册换线小界面 */
			facade.registerMediator(new ChangeLineMediator());
			/** 新手指导 */
			facade.registerMediator(new NewerHelpMediator()); 
			/** 挂机面板 */
			facade.registerMediator(new AutoPlayMediator());
			/** 离线挂机面板 */
			facade.registerMediator(new OffLinePlayMediator());
			
//			//下面的东西不能删
			/** 师徒面板 */
			facade.registerMediator(new MasterMediator());
			/** 师徒奖励面板 */
			facade.registerMediator(new MasterAwardMediator());
			/** 背叛师门 */
			facade.registerMediator(new BetrayMediator());
			/** 倒计时面板 */
			facade.registerMediator(new TimeCountDownMediator());
			/** 大喇叭 */
			facade.registerMediator(new BigLeoMediator());
			/** 新手成就大礼包 */
			facade.registerMediator(new NewAwardMediator());
			/** 新手卡 */
			facade.registerMediator(new NewPlayerCardMediator());
			/** 在线倒计时 */
			facade.registerMediator(new OnlineRewardMediator());
			/** 领取在线奖励 */
			facade.registerMediator(new GainAwardMediator());
			/** 日常活动 */
			facade.registerMediator(new CampaignMediator());
			/** Gm问题反馈 */
			facade.registerMediator(new GmToolsMediator());
			/**系统设置面板*/
			facade.registerMediator(new SettingMediator());
			/** 队伍位置处理命令*/
			facade.registerCommand(PlayerInfoComList.MSG_TEAMPOS_PROCESS,TeamPosProcessCommand);

			/** 注册好友聊天面板*/
			facade.registerMediator(new FriendChatMediator());
			/** 系统消息列表面板 */
			facade.registerMediator(new SystemMediator());
			/** 系统消息内容面板 */
			facade.registerMediator(new SysCotentMediator());
			/** 发送系统消息命令 */
			facade.registerCommand(EventList.SENDSYSTEMMESSAGE, SystemMessageCommand);
			/** 装备打造 */
			facade.registerMediator( new ManufactoryMediator() );
			/**vip头像*/
			facade.registerMediator(new VipHeadIconMediator()); 
			
			/** 注册防沉迷 */
			facade.registerMediator(new PreventWallowMediator());
			/** 注册副本引导 */
			facade.registerMediator(new CopyLeadMediator()); 
			/** 生活技能学习 */
			facade.registerMediator(new LearnLifeSkillMediator());
			/** 装备玩法 */
			facade.registerMediator(new EquipmentMediator());
			/** 日程表 */
			facade.registerMediator(new CalendarMediator());
			/** 开箱子 */
			facade.registerMediator( new IdentifyTreaMediator() );
			/** 帮派技能学习 */
			facade.registerMediator( new LearnUnitySkillMediator() );
			/** 有打勾的小提示框 */
			facade.registerMediator( new TickAlertMediator() );
			/** 许愿 */
			facade.registerMediator( new WishMediator() ); 
			/**师徒答题 */
			facade.registerMediator( new MasterAnswerMediator() ); 
			
			facade.registerMediator( new MasStuMainMediator() );
			facade.registerMediator( new StudentListMediator() ); 
			/** 全屏自适应 */
			facade.registerMediator( new AutoSizeMediator() ); 
			/** VIP界面  */
			facade.registerMediator( new VipShowMediator() );
            /** 提示图标  */
			facade.registerMediator( new IconMediator() ); 
			/** 开箱子策略 */
			facade.registerCommand( ShowTreasureCommand.NAME , ShowTreasureCommand );	
			/** 注册帮派发送请求 */
//			facade.registerCommand(UnityStartUpCommand.NAME , UnityStartUpCommand);		
			/** 注册平台控制 */
			facade.registerCommand(TerraceController.NAME , TerraceController);
			/** 答题 */
//			facade.registerCommand( QuestionCommand.NAME, QuestionCommand); 
			 /** 肉肉龙蛋  */
			facade.registerMediator( new DragonEggMediator() ); 
			/** 音乐播放器 */
			facade.registerMediator( new MusicPlayerMediator() );   
//			/** 竞技场面板 */
//			facade.registerMediator(new ArenaPanelMediator());
			/** 充值等级 */
			facade.registerMediator( new PrepaidLevelMediator() );
			
//			/**  登陆 选择角色 创建角色  (这个需要最后注册，其他的都加在这个上面)*/
//			if(GameCommonData.isLoginFromLoader) {
//				GameCommonData.GameNets = new GameNet(GameCommonData.ServerIp, 4808)
//			} else {
//				facade.registerMediator(new LoginMediatorYL());
//			}
			/** 关闭所有界面 */
			facade.registerCommand(CloseAllViewCommand.NAME,CloseAllViewCommand);
			/** 铸灵 */
			facade.registerMediator( new CastSpiritMediator() );
			/** 铸灵升级 */
			facade.registerMediator( new CastSpiritUpMediator() );
			/** 铸灵转移 */
			facade.registerMediator( new CastSpiritTransferMediator() );
		}
	}
}