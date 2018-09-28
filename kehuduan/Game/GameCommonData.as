package
{
	import Controller.SceneController;
	
	import GameUI.UICore.UIFacade;
	
	import Net.AccNet;
	import Net.IGameNet;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	
	import Vo.RoleVo;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Point;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.media.SoundTransform;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	
	/** 游戏共公数据 */
	public class GameCommonData
	{
		public static var LoginName:String = "LoginName";
		public static var LoginPassword:String;
	    /** 场景中最大人数**/
	    public static var SameSecnePlayerMaxCount:int = int.MAX_VALUE;
		/** 登录场景 */
		public static const SCENE_LOGIN:String = "登录场景";
		/** 游戏场景 */
		public static const SCENE_GAME:String  = "游戏场景";	
		/** 游戏主体 */
		public static var GameInstance:Game;
		/** facade  */
		public static var UIFacadeIntance:UIFacade
		/** 账号Socket  */
		public static var AccNets:AccNet;
		/** 游戏Socket  */
		public static var GameNets:IGameNet;
		/** 游戏服务器列表 */
		public static var GameServerArr:Array = [];
		/** 过滤后的游戏服务器列表 */
		public static var FilterGameServerArr:Array;
		/** 是否连接账号服务器 */
		public static var IsConnectAcc:Boolean = false;
		/** 是否第一次登陆，是否要加载资源 */
		public static var IsFirstLoadGame:Boolean = true;
		
		/**屏蔽玩家**/ // 0 屏所有  1 显示玩家  2 显示所有
		public static var Screen:int = 2;
		/** 游戏玩家对象  */
		public static var Player:GameElementPlayer;
		/** 目标对象（玩家、NPC、怪物） */
		public static var TargetAnimal:GameElementAnimal;
		/** 攻击对象 **/
		public static var AttackAnimal:GameElementAnimal;
		
		/** 目标NPCID 如果自动寻路的位置不是NPC 该ID号为-1  */
		public static var targetID:int = -1;
		/** 宠物攻击的对象 */
		public static var PetTargetAnimal:GameElementAnimal;
		/** 跟随目标**/
		public static var IsFollow:Boolean =false;
		/** 默认技能对象**/
		public static var NextSkillTarget:GameElementAnimal;
		
		/** 是否向目标靠近  */
		public static var IsMoveTargetAnimal:Boolean = false;
		/** 宠物编号 **/
		public static var PetID:int = 0;
		/**宠物跳动时间**/
		public static var PetJump:Number = 0;
//		/**是否挂机**/
//		public static var IsAutomatism:Boolean = false;
		/**设置挂机点**/
		public static var AutomatismPoint:Point = null;
		/**挂机时间**/
		public static var AutomatismTime:Number = 0;
		/**必须要移动到的点**/
		public static var MustPoint:Point = null;
		
		/**转场成功**/
	    public static var IsLoadScene:Boolean = true;
		/**跨场景目标**/
		public static var TargetScene:String = "";
		/**跨场景目标点**/
		public static var TargetPoint:Point = new Point();
	    /**跨场景目距离**/
		public static var TargetDistance:int = 0;
		/**普通攻击目标**/
		public static var TargetCommon:int;
		/**攻击者目标**/
		public static var Attack:int = 0;
		/**是否切线**/
		public static var IsChangeOnline:Boolean = false;
		/**切磋*/
		public static var IsDuel:Boolean = false;
		/**切磋对象的编号**/
		public static var DuelAnimal:int = 0;
		/** 同场景在线玩家列表 */
		public static var SameSecnePlayerList:Dictionary;
		/** 同场景不加入场景的玩家列表**/
		public static var VisibleSameSecnePlayerList:Dictionary = new Dictionary();
		
		/** 同场景的宠物*/
		public static var SameSecnePetList:Dictionary = new Dictionary();
		/** 组队队员列表  */
		public static var TeamPlayerList:Dictionary = new Dictionary();
		/** 掉落包列表  */
		public static var PackageList:Dictionary    = new Dictionary();
		/** 游戏场景控制  */
		public static var Scene:SceneController;
		/** 游戏场景地效果**/
		public static var Rect:Sprite;
		/** 游戏场景地效果附带技能编号**/
		public static var RectSkillID:int;
		
		/** 角色选择数据 */
		public static var RoleList:Array;
		
		/** 问题字典  */
		public static var QuestionDic:Dictionary = new Dictionary();
		/** 选择职业问题字典  */
		public static var SelectQueDic:Dictionary = new Dictionary();
		
	    /** 技能数据*/
		public static var SkillList:Dictionary = new Dictionary();
		/** 技能数据*/
		public static var LifeSkillList:Dictionary = new Dictionary();
		/** 技能说话 */
		public static var BossTalk:Dictionary = new Dictionary();
	    /** 技能排序*/
		public static var Skills:Array = new Array();
		/** buff*/
	    public static var BuffList:Dictionary = new Dictionary();
	    /**技能效果列表**/
	    public static var SkillEffectList:Dictionary = new Dictionary();
	    /**技能效果是否加载列表**/
	    public static var SkillLoadEffectList:Dictionary = new Dictionary();
        /**技能效果已经下载**/
        public static var SkillOnLoadEffectList:Dictionary = new Dictionary();
        /**地面效果**/
        public static var FloorEffectList:Dictionary = new Dictionary();
        /**地面效果已经下载**/
        public static var FloorOnLoadEffectList:Dictionary = new Dictionary();
        /**技能声音是否加载列表**/
        public static var SkillLoadAudioEngine:Dictionary = new Dictionary();
		/**技能声音已经下载**/
        public static var SkillOnLoadAudioEngine:Dictionary = new Dictionary();	
		/**称号列表**/
		public static var AppellationList:Dictionary = new Dictionary();
		/**称号配置数据**/
		public static var  Designation:Array = new Array();
		/** 怪物技能列表*/
		public static var WeirdyGameSkillList:Dictionary = new Dictionary();
		/** 职业技能列表*/
		public static var JobGameSkillList:Dictionary = new Dictionary();
		/** 宠物特使模型列表*/
		public static var ModelPet:Dictionary = new Dictionary();
	    /** 宠物变异列表*/
	    public static var VariationPet:Array = new Array();
		/** 人物模型偏移列表*/
		public static var ModelOffsetPlayer:Dictionary = new Dictionary();
		/** 怪物模型偏移列表*/
		public static var ModelOffsetEnemy:Dictionary = new Dictionary();
		/** NPC模型偏移列表*/
		public static var ModelOffsetNPC:Dictionary = new Dictionary();
		/** 坐骑模型偏移列表*/
		public static var ModelOffsetMount:Dictionary = new Dictionary();
	    /** 技能数据偏移值*/
		public static var ModelOffsetSkill:Dictionary = new Dictionary();
		/**武器特效列表**/
		public static var WeaponEffect:Dictionary = new Dictionary();
		/** 地图关系表  */
		public static var MapTree:Dictionary = new Dictionary();
		/** 游戏服务数据  */
		public static var GServerInfo:Object;
		/** 玩家信息  */
		public static var UserInfo:Object;
		/** 玩家账号 */
		public static var Accmoute:String;
		/** 玩家密码 */ 
		public static var Password:String;
		/** 服务器名称 s1,s2 */
		public static var ServerId:String;
		/** 已选择的角色 */ 
		public static var SelectedRole:RoleVo;
		/** 职业名称  */
		public static var RolesListDic:Dictionary = new Dictionary();
		/** 任务列表 */
		public static var TaskInfoDic:Dictionary=new Dictionary();
		
		public static var MaskHi:uint;
		public static var MaskLow:uint;
		public static var DayMaskHi:uint;
		public static var DayMaskLow:uint;
		/**切换目标时间**/
		public static var Targettime:Number = 0;		
		public static var BigMapMaskLow:uint=33554432;
		public static var BigMapMaskHi:uint=0;
		
		/** Test Socket Bool   */
		public static var isSend:Boolean = true;		 
		
		/** 游戏服务器IP (由Loader传进来)*/
		public static var ServerIp:String = "";
		
		/** 游戏进度条 (由Loader传进来)*/
		public static var Tiao:MovieClip = null;
		
		/** 游戏背景图 (由Loader传进来)*/
		public static var BackGround:MovieClip = null;
		
		/** 是否从Loader登陆 */
		public static var isLoginFromLoader:Boolean = false;
		
		/** 是否输入文本框占用了焦点*/
		public static var isFocusIn:Boolean=false;
	
		/** NPC窗口是否已打开 */
		public static var NPCDialogIsOpen:Boolean = false;
		
		/** 自动寻路标志*/
		public static var isAutoPath:Boolean=false;
		
		/** 新手任务完成标志*/
		public static var isNewTaskEnd:Boolean=false;
		
		/** 是否第一次初始化(只有第一次才请求物品信息) */
		public static var isFirstTimeEnterGame:Boolean = true;
		
		/** 对话框状态标志*/
		public static var dialogStatus:uint=0;
		/** 与NPC对话的NPCID*/
		public static var talkNpcID:uint; 
		
		/** 这个完全是测试gameloader用，用完就删*/
		public static var loaderTxt:TextField;
	
		/** 初始化游戏场景需要的数据 */
		public static var enterGameObj:Object = new Object();					
		
		public static var flagTestYL:Boolean = false;
		
		//是否执行了UserInfo
		public static var IsLoadUserInfo:Boolean = false;
		
		/** 是否打开了音乐开启开关 */
		public static var isOpenSoundSwitch:Boolean = true;
		/** 是否打开了音效开启开关 */
		public static var isOpenFightSoundSwitch:Boolean = true;
		/** 喇叭开图片  从loader过来的*/
		public static var soundOn_bmp:Bitmap = null;
		/** 喇叭关图片  从loader过来的*/
		public static var soundOff_bmp:Bitmap = null;
		/** 登陆提示面板  从loader过来的*/
		public static var loginHint_mc:MovieClip = null;
		/** 是否收到1052消息*/
		public static var isReceive1052:Boolean = false;
		/** 是否收到账号服务器消息*/
		public static var isReceiveAcc:Boolean = false;
		/** 当前音乐音量 */
		public static var soundVolume:int = 100;
		/** 当前音效音量*/
		public static var fightSoundVolume:int = 100;
		/** 服务器当前时间(年) */
		public static var gameYear:int;
		/** 服务器当前时间(月) */
		public static var gameMonth:int;
		/** 服务器当前时间(日)*/
		public static var gameDay:int;
		/** 服务器当前时间(小时) */
		public static var gameHour:int;
		/** 服务器当前时间(分钟) */
		public static var gameMinute:int;
		/** 服务器当前时间(秒数)*/
		public static var gameSecond:int;
		/** 网络延时开始发心跳时间*/
		public static var netDelayStartTime:int;
		/** 网络延时时间毫秒数*/
		public static var netDelayTime:int;
		/**跟新后的服务器最新时间*/
		public static var nowDate:Date = new Date();
		/** 是否显示充值（1：不显示  0:显示）*/
		public static var cztype:int=0;
		/**是否是攻击状态**/
		public static var IsAttack:Boolean = false; 
		/**游戏版本号**/
		public static var GameVersion:String; 
		/** 防沉迷定时弹出的时间 */
		public static var preventWallowTime:int = 1000 * 10;
		/** 防沉迷弹出次数 */
		public static var showAccount:int = 10;
		/**防沉迷开关 1 开启  0 关闭  111 旧的平台*/
		public static var fcmPower:int = 0;
		/**单个用户防沉迷开关 0 没通过，要弹窗  1 已经通过，不弹*/
		public static var fcmConfig:int = 1;
		/**是否新平台 0 旧平台  大于等于100 为新php平台*/
		public static var isNew:int = 0;
		/** 自动寻路到指定位置打怪  */
		public static var autoPlayAnimalType:uint=0;
		/** 是否开箱子策略 1正常 打开 2打开包裹  3 不开放 */
		public static var openTreasureStragety:uint=1;
		/**攻击目标编号**/
		public static var AttackTargetID:int = 0;
		/**攻击时间**/
		public static var AttackTargetTime:Number = 0;
		/**解析动画控制**/
		public static var AnalyzeTime:Timer;
		/**从loader里过来的背景音乐**/
		public static var loginSoundFromLoader:Sound;
		/**从loader里过来的背景音乐**/
		public static var loginSoundChannel:SoundChannel;
		/**从loader里过来的背景音乐**/
		public static var loginSoundTransform:SoundTransform;
		/**游戏文本配置字典**/
		public static var wordDic:Dictionary;  
		/**游戏文字版本 1为简体  2为繁体**/
		public static var wordVersion:int = 1;  
		/**游戏文字编码**/
		public static var CODE:String = "ANSI";  
		/**是否为全屏 1为小窗口  2为全屏**/
		public static var fullScreen:int = 1;
		/**加载Notice的方式 1为swf  2为xml**/
		public static var noticeFarmat:int = 1;
		/**panelBase数组**/
		public static var panelBaseArr:Array = new Array();
		/**autoPanelBase数组**/
		public static var autoPanelArr:Array = new Array();
		/** 是否能改名 **/
		public static var canReName:int;
		/** 选择的第几个角色 **/
		public static var SelectedRoleIndex:uint;
		/** 充值链接 **/
		public static var payPath:String;
		/** 是否开启点卷商城 **/
		public static var couponsCanOpen:Boolean;
		/** 是否从 EXE 客户端登录 **/
		public static var isExe:Boolean = false;
		/** 版本控制类 **/
//		public static var globalizationInfo:GlobalizationInfo = new GlobalizationInfo();
	}
}