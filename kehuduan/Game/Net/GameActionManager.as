package Net
{
	import GameUI.UICore.UIFacade;
	
	import Net.ActionProcessor.*;
	
	import flash.utils.Dictionary;
	
	/** 游戏网站动作管理 */
	public class GameActionManager
	{
		private var actionList:Dictionary = new Dictionary();
		
		/** 网站动做列表 */
		public function get ActionList():Dictionary
		{
			return this.actionList;
		}
		
		public function GameActionManager()
		{ 
			GameCommonData.UIFacadeIntance	  = UIFacade.GetInstance(UIFacade.FACADEKEY);
			this.actionList[Protocol.GAMESERVER_INFO]	= new GameServerInfo();
			this.actionList[Protocol.PLAYER_CHAT] 		= new Chat();
			this.actionList[Protocol.PLAYER_ACTION] 	= new PlayerAction();
			this.actionList[Protocol.USER_INFO]			= new UserInfo();
			this.actionList[Protocol.PLAYER_WALK]		= new PlayerWalk();
			this.actionList[Protocol.PLAYER_INFO]		= new PlayerInfo();
			this.actionList[Protocol.TEAM_ACTION]		= new TeamAction();
			this.actionList[Protocol.TEAM_INFO]			= new TeamInfoAction();
			this.actionList[Protocol.TRADE_INFO]		= new TradeAction();
			this.actionList[Protocol.MSG_INTERACT]      = new PlayerCombat();
			this.actionList[Protocol.OPERATE_ITEMS]     = new OperateItem();
			this.actionList[Protocol.FRIEND_ACTION]     = new FriendAction();
			this.actionList[Protocol.PLAYER_MAPITEM]    = new MapItem();
			this.actionList[Protocol.DEPOT_OPERATE]		= new DepotAction();
			this.actionList[Protocol.USER_ATT]			= new UserAtt();
			this.actionList[Protocol.MSG_PLAYDETAIL]    = new PlayerDetail();
			this.actionList[Protocol.MSG_A_GAMESKILL]   = new PlayerSikllList();
			this.actionList[Protocol.MSG_MAGICEFFECT]   = new PlayerUseSkill();
			this.actionList[Protocol.MSG_COOLDOWN]      = new PlayerSkillCoolDown();
			this.actionList[Protocol.MSG_BUFF]          = new PlayerSkillBuff();
			this.actionList[Protocol.MSG_TASK]          = new TaskDetail();
			this.actionList[Protocol.MSG_ITEMINFO]   	= new ItemInfo();
			this.actionList[Protocol.MSG_NPCSHOP]		= new NPCShopAction();
			this.actionList[Protocol.MSG_PET_EUDEMON]	= new PetEudemonAction();
			this.actionList[Protocol.MSG_PET_INFO_ALL]  = new PetInfoAllAction();
			this.actionList[Protocol.MSG_PET_ATT]		= new PetAtt();
			this.actionList[Protocol.MSG_NPCDIALOG]     = new NPCDialog();
            this.actionList[Protocol.MSG_TOPLIST]       = new TopList();
            this.actionList[Protocol.MSG_EQUIP]         = new EquipAction();
            this.actionList[Protocol.MSG_UNITY]			= new SynAction();
			this.actionList[Protocol.MSG_GETINFOLIST]  	= new SynInfo(); 
            this.actionList[Protocol.MSG_GETDATAUNITY]  = new SynList();
            this.actionList[Protocol.MSG_GETMENBERLIST] = new SynMenberList();
          	this.actionList[Protocol.MSG_GETINFOSUB]  	= new SynSubInfo();	
            this.actionList[Protocol.MSG_MARKET]		= new MarketAction();
           	this.actionList[Protocol.MSG_QUICKSWITCH]   = new QuickSwitchAction();
           	this.actionList[Protocol.MASTER_LIST_ACTION]= new Tutor();
    		this.actionList[Protocol.MSG_TEAM_POS]      = new TeamPos();
           	this.actionList[Protocol.MSG_NPCBUSINESS]	= new NPCBusinessAction();                          
           	this.actionList[Protocol.MSG_ENCRYPT]		= new EncyptAction();
           	this.actionList[Protocol.MSG_REGISTER]		= new Register();
           	this.actionList[Protocol.MSG_AUTO_PLAY]     = new AutoPlayAction();
           	this.actionList[Protocol.MSG_OPEN_TREASURE] = new TreasureAction();
           	this.actionList[Protocol.MSG_DESIGNATION]	= new UserTitle();
           	this.actionList[Protocol.MSG_OPEN_GOTOCOPY]	= new GotoCopyAction();
           	this.actionList[Protocol.MSG_SOUL_DETAIL]	= new SoulDetailInfoAction();
           	this.actionList[Protocol.MSG_SOUL_PICTURE]	= new SoulPictureAction();
           	
            this.actionList[Protocol.MSG_MERIDIANS]	    = new MeridiansAction();	
            this.actionList[Protocol.MSG_VIP_SHOW]	    = new VipList();
            this.actionList[Protocol.MSG_PET_EGG]	    = new PetEgg();
            this.actionList[Protocol.ARENA_SCORE]		= new WarGame();
            this.actionList[Protocol.MSG_GENERAL]       = new PrepaidLevelAction();
//             this.actionList[/** Protocol.MSG_VIP_SHOW */ 111111]	    = new QuestionAction(); 
			this.actionList[Protocol.MSG_COMPENSATE_LIST]= new CompensateStorageAction();
			this.actionList[Protocol.MSG_COMPENSATE_LOG]= new CompensateStorageLogAction();
		}
	}
}