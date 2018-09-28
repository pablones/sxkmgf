package
{
	/** 数据配置数据 */
	public class GameConfigData
	{
		/** 账号服务器服务器IP */
		public static var AccSocketIP:String; 
		
		/** 账号服务器服务器端口 */
		public static var AccSocketPort:uint = 8846;
		
		/** 游戏服务器服务器名称 */
		public static var GameSocketName:String = "";								//sanco   测试  英雄之门 封测一服
		
		public static const LINE_ARR:Array = [
												"一线",
												"二线",
												"三线",
												"四线"
												];
		
		/** 游戏服务器服务器IP */
		public static var GameSocketIP:String;							
		
		/** 游戏服务器服务器端口 */
		public static var GameSocketPort:uint;	
		
		/** 游戏服务器服务器当前人数 */
		public static var GameSeverNum:uint;						
		
		/** 游戏场景宽  */
		public static const GameWidth:Number = 1000;
		
		/** 游戏场景高  */
		public static const GameHeight:Number = 580;
		
		/** UI库  */
		public static const UILibrary:String           = "Resources/UILibrary_1.swf";
		
		/** UI角色管理库  */
		public static const UILibraryRole:String       = "Resources/GameDLC/UILibrary_Role.swf";
		
		/** 新手成就大礼包swf*/
		public static const NewPlayerAward:String       = "Resources/GameDLC/NewPlayerAward.swf";
		
		/** 15-25新手帮助swf */
		public static const MC_NEWERHELP_ADDR_ARR:Array = [
															"Resources/GameDLC/NewerHelpFri.swf",
															"Resources/GameDLC/NewerHelpPet.swf",
															"Resources/GameDLC/NewerHelpEqu.swf",
															"Resources/GameDLC/NewerHelpLearnSK.swf"
															];
		
		/** 物品基本属性swf_1  */
		public static const Properties_1:String          = "Resources/Properties_1.swf";
		
		/** 物品基本属性swf_2  */
		public static const Properties_2:String          = "Resources/Properties_2.swf";
		
		/** 过滤字典swf */
		public static const FilterDic:String           = "Resources/Filter.swf";
		
		/** 商城数据swf */
		public static const Market:String              = "Resources/Market.swf";
		
		/** 公告数据xml */
		public static const NOTICE_DATA:Array		   = ["Resources/Notice.swf", "Resources/GameData/Notice.xml"];
		
		 /** 宠物模型swf目录 */
		public static const PetModelDic:String         = "Resources/EnemyIcon/";
		/** 技能SWF */
		public static const GameSkillListSWF:String    = "Resources/GameSkill/";
		/** 技能声音 */
		public static const GameSkillAudio:String      = "Resources/GameMusic/Skill/";

		/**任务xml */
		public static const GameTaskInfoList:String	   = "Resources/taskInfo.swf";
		/** 江湖指南SWF  */
		public static const Help:String          		= "Resources/GameDLC/Help.swf";
		/** 活动日程SWF  */
		public static const CampaignSWF:String          = "Resources/GameDLC/campaignInfo.swf";
		/** 装备打造SWF  */
		public static const ManufatorySWF:String        = "Resources/GameDLC/Manufacory.swf";
		/** GM问题反馈 */
		public static const GmToolsSWF:String 			= "Resources/GameDLC/GmTools.swf"
		/** GM PHP地址 */
		public static var GmPhpPath:String			    = "Interface/userask.php";							//http://192.168.1.127/yjjh_ADM1/interface/userask.php  http://yjjh1-bak.my4399.com/interface/userask.php
		/**vip会员头像*/
		public static var VipHeadIconSWF:String 		= "Resources/GameDLC/VipHeadIcon.swf";
		/**投票卡*/ 
		public static var VoteCardSWF:String 			= "Resources/GameDLC/VoteCard.swf";
		/** 防沉迷swf*/
		public static const PreventWallow:String       	= "Resources/GameDLC/PreventWallow.swf";
		/** 副本引导 */
		public static var CopyLeadSWF:String       		= "Resources/GameDLC/CopyLead.swf";
		/** 开箱子（又名宝图鉴定） */
		public static var OpenTreaSWF:String       		= "Resources/GameDLC/OpenTreasure.swf";
		
		//改版后的xmlswf加载地址
		/**技能*/
		public static var Skill_XML_SWF:String = "Resources/GameData/SkillXML.swf";
		/**套装*/
		public static var Cloth_XML_SWF:String = "Resources/GameData/ClothXML.swf";
		/**偏移模型*/
		public static var ModelOffset_XML_SWF:String = "Resources/GameData/ModelOffsetXML.swf";
		/**答题，exp，maptree*/
		public static var Other_XML_SWF:String = "Resources/GameData/OtherXML.swf";
		
		/** 简体繁体文字配置swf */
		public static const Word_Config_SWF:String		   = "Resources/WordConfig.swf"; 
		
		/** 服务器传过来的专线名 */
		public static const specialLineName:String = "s";
		/** 服务器传过来的专线名 */
		public static var IsLoginChangeLine:Boolean = false;
	}
}