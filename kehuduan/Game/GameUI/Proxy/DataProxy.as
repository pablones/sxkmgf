package GameUI.Proxy
{
	import GameUI.UICore.UIFacade;
	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class DataProxy extends Proxy
	{
		public static const NAME:String = "DataProxy";
		
		public function DataProxy()
		{
			super(NAME);
			this.initializeNotifier(UIFacade.FACADEKEY);
			RolesListDic[0]    = GameCommonData.wordDic[ "often_used_zw" ];   
			RolesListDic[1]    = GameCommonData.wordDic[ "often_used_tm" ];              //唐门     近战 
			RolesListDic[2]    = GameCommonData.wordDic[ "often_used_qz" ];              //全真     远战
			RolesListDic[4]    = GameCommonData.wordDic[ "often_used_em" ];              //峨嵋     远战  
			RolesListDic[8]    = GameCommonData.wordDic[ "often_used_gb" ];              //丐帮     近战
			RolesListDic[16]   = GameCommonData.wordDic[ "often_used_sl" ];              //少林     近战
			RolesListDic[32]   = GameCommonData.wordDic[ "often_used_dc" ];              //点苍     远战
//			RolesListDic[64]   = "法师";
//			RolesListDic[128]  = "法师";
//			RolesListDic[256]  = "牧师";
//			RolesListDic[512]  = "刺客";
//			RolesListDic[1024] = "战士";
//			RolesListDic[2048] = "弓手";
			RolesListDic[4096] = GameCommonData.wordDic[ "often_used_xs" ];               //少林     近战
//			RolesListDic[8192] = "新手";
		}
		
		/** 人物面板已经打开 */
		public var HeroPropIsOpen:Boolean = false;
		/** 背包面板已经打开 */
		public var BagIsOpen:Boolean = false;
		/** 技能面板已经打开 */
		public var SkillIsOpen:Boolean = false;
		/** 技能学习面板已经打开 */
		public var LearnSkillIsOpen:Boolean = false;
		/** 任务面板已经打开 */
		public var TaskIsOpen:Boolean = false;
		/** 任务追踪面板打开 */
		public var TaskFollowIsOpen:Boolean = true;
		/** 可接任务面板打开 */
		public var TaskAccIsOpen:Boolean = false;
		/** 浮动任务面板是否已经关闭（收缩） */
		public var TaskFollowUIIsFolded:Boolean = false;
		/** 好友面板已经打开 */
		public var FriendsIsOpen:Boolean = false;
		/** 组队面板已经打开 */
		public var TeamIsOpen:Boolean = false;
		/** 挂机面板已经打开 */
		public var HangIsOpen:Boolean = false;
		/** 系统面板已经打开 */
		public var GameSetIsOpen:Boolean = false;
		/** 帮派面板已经打开 */
		public var UnityIsOpen:Boolean = false;
		/** 创建帮派是否打开 */
		public var CreateUnitIsOpen:Boolean = false;
		/** 帮派信息面板是否打开 */
		public var UnitInfoIsOpen:Boolean = false;
		/** 交易面板已经打开 */
		public var TradeIsOpen:Boolean = false;
		/** 摆摊面板已经打开 */
		public var StallIsOpen:Boolean = false;
		/** 仓库面板已经打开 */
		public var DepotIsOpen:Boolean = false;
		/** 好友信息面板已打开 */
		public var FriendInfoIsOpen:Boolean=false;
		/** 发信息面板已经打开  */
		public var FriendSendMsgIsOpen:Boolean=false;
		/** 收信息面板已经打开 */
		public var FriendReveiveMsgIsOpen:Boolean=false;
		/** NPC商店面板已经打开 */
		public var NPCShopIsOpen:Boolean = false;
		/** NPC跑商商店面板已经打开 */
		public var NPCBusinessIsOpen:Boolean = false;
		/** 鼠标是否正在寻找好友*/
		public var isSelectFriend:Boolean=false;
		/** 队伍角色数据数组*/
		public var roleDatas:Array=[];
		/** 是否是队长*/
		public var isTeamLeader:Boolean=false;
		/** 是否是队员*/
		public var isTeamMember:Boolean=false;
		/** 是否选中自己*/
		public var isSelectSelf:Boolean=false;
		/** 职业名称  */
		public var RolesListDic:Dictionary = new Dictionary();
		/** 装备属性  */
		public var equipments:Array = []; 
		/** 排行榜面板已经打开 */
		public var RankIsOpen:Boolean = false;
		/** 大地图已打开 */
		public var BigMapIsOpen:Boolean = false;
		/** 场景地图  */
		public var SenceMapIsOpen:Boolean = false;
		/** 帮派已经打开 */
		public var ApplyUnitIsOpen:Boolean = false;
		/** 商城是否打开 */
		public var MarketIsOpen:Boolean = false;
		//////////////////////////////////////////////////////////
		//宠物系列面板
		/** 宠物基本属性面板已经打开 */
		public var PetIsOpen:Boolean = false;
		/** 宠物单人繁殖面板已经打开*/
		public var PetBreedSingleIsOpen:Boolean = false;
		/** 宠物双人繁殖面板已经打开*/
		public var PetBreedDoubleIsOpen:Boolean = false;
		/** 宠物还童面板已经打开*/
		public var PetToBabyIsOpen:Boolean = false;
		/** 宠物技能学习面板已经打开*/
		public var PetSkillLearnIsOpen:Boolean = false;
		/** 宠物技能升级面板已经打开*/
		public var PetSkillUpIsOpen:Boolean = false;
		/** 宠物悟性提升面板已经打开*/
		public var PetSavvyUseMoneyIsOpen:Boolean = false;
		/** 宠物合成面板已经打开*/
		public var PetSavvyJoinIsOpen:Boolean = false;
		/** 查询的返回是否是可以操作的宠物 */
		public var PetCanOperate:Boolean = true;
		/** 是否是查询宠物技能 */
		public var PetQuerySkill:Boolean = false;
		/** 当前查询的宠物快照 */
		public var PetEudemonTmp:Object = null;
		//////////////////////////////////////////////////////////
		
		/** 装备强化*/
		public var EquipStrengenIsOpen:Boolean=false;
		/** 装备升星*/
		public var EquipAddStarIsOpen:Boolean=false;
		/** 装备打孔与镶嵌宝石*/
		public var EquipStilettoIsOpen:Boolean=false; 
		/** 宝石合成 */
		public var StoneCompIsOpen:Boolean = false;
		/** 自动寻路打开*/
		public var AutoRoadIsOpen:Boolean=false;
		/** 江湖指南打开 */
		public var HelpViewOpen:Boolean = false;
		/** Pk是否打开*/
		public var isPkOpen:Boolean = false;
		/** 宝石合成打开*/
		public var composeStoneIsOpen:Boolean=false;
		/** 答题面板打开*/
		public var answerPanelIsOpen:Boolean=false;
		/** 捡包面板打开*/
		public var pickBagIsOpen:Boolean=false;
		/** 挂机面板打开*/
		public var autoPlayIsOpen:Boolean=false;
		/** 师徒面板打开*/
		public var masterPanelIsOpen:Boolean=false;
		/** 师徒奖励面板打开*/
		public var masterAwardIsOpen:Boolean=false;
		/** 生活技能学习面板打开*/
		public var liftLearnPanelIsOpen:Boolean=false;
		/** 在线奖励面板打开*/
		public var GainAwardPanIsOpen:Boolean=false;
		/** 活动日程面板打开*/
		public var CampaignPanIsOpen:Boolean=false;
		/**装备打造打开*/
		public var EquipManufatoryIsOpen:Boolean=false;
		/**系统设置面板打开*/
		public var settingPanIsOpen:Boolean = false;
		/**  修炼面板是否打开*/
		public var buildPanelIsOpen:Boolean =false;
		/** 称号面板是否打开*/
		public var designationPanIsOpen:Boolean = false;
		/**vip头像面板是否打开*/
		public var vipHeadIconPanelIsOpen:Boolean = false;
		/** 宠物玩法界面是否打开 */
		public var petRuleIsOpen:Boolean = false;
		/** 装备玩法面板是否打开*/
		public var equipPanelIsOpen:Boolean=false;
		/** 开宝箱面板是否打开*/
		public var treasurePanelIsOpen:Boolean=false;
		/** 开宝箱包裹是否打开*/
		public var treasurePackageIsOpen:Boolean=false;
		/** 任务捐装备面板是否打开 */
		public var taskEquipReturnIsOpen:Boolean=false;
		/** 帮派技能学习面板是否打开 */
		public var learnUnitySkillIsOpen:Boolean = false;
		/**徒弟列表界面打开*/
		public var studentListPaneIsOpen:Boolean = false; 
		/** 经脉学习确定界面打开 */ 
		public var meridianLearnPromptIsOpen:Boolean = false; 
		/**经脉强化界面打开*/
		public var meridianStrengthIsOpen:Boolean = false; 
		/** 经脉修炼队列界面 */
		public var meridiansLearnListIsOpen:Boolean = false;
		/** 答题界面 */
		public var questionIsOpen:Boolean = false;
		/** 新帮派界面 */
		public var newUnityIsOpen:Boolean = false;
		/** 音乐播放器是否打开 */
		public var musicPlayerIsOpen:Boolean = false;
		/** 竞技分数面板是否打开 */
		public var arenaPanelIsOpen:Boolean = false;
		/** 竞技分数小面板是否打开 */
		public var arenaSmallPanelIsOpen:Boolean = false;
		/** 竞技场聊天小面板是否打开 */
		public var arenaMsgPanelIsOpen:Boolean = false;
		/** 竞技场聊天小面板是否正在输入 */
		public var arenaMsgPanelIsTyping:Boolean = false;
		/** 玩家按 ENTER 后，焦点给哪个面板？（chat/arena） */
		public var lastUsedMsgPanel:String = "chat";
	}
}