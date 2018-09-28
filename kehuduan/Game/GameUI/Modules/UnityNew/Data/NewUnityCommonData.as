package GameUI.Modules.UnityNew.Data
{
	import GameUI.Modules.UnityNew.Proxy.NewUnityResProvider;
	import GameUI.Modules.UnityNew.Proxy.UnityBaseInfo;
	
	public class NewUnityCommonData
	{
		public static var newUnityResProvider:NewUnityResProvider = new NewUnityResProvider();
		
		public static const NEW_UNITY_INITVIEW:String = "NEW_UNITY_INITVIEW";				//初始化帮派
		public static const CLICK_NEW_UNITY_BTN:String = "CLICK_NEW_UNITY_BTN";		//点击帮派按钮
		
		public static const CHANG_NEW_UNITY_PAGE:String = "CHANG_NEW_UNITY_PAGE";		//发送点击页签事件
		public static const CLEAR_UNITY_LAST_OPEN_PANEL:String = "CLEAR_UNITY_LAST_OPEN_PANEL";		//清除上一次打开的面板事件
		
		public static const CLICK_MEMBER_PAGE_COME:String = "CLICK_MEMBER_PAGE_COME";			//开始成员管理的页签
		public static const CLEAR_MEMBER_PAGE_GO:String = "CLEAR_MEMBER_PAGE_GO";				//清理成员管理的页签
		
		public static const UPDATA_UNITY_MEMBERLIST_DATA:String = "UPDATA_UNITY_MEMBERLIST_DATA";
		public static const UPDATA_UNITY_CONTRIBUTE_LIST_DATA:String = "UPDATA_UNITY_CONTRIBUTE_LIST_DATA";
		public static const UPDATA_UNITY_APPLY_LIST_DATA:String = "UPDATA_UNITY_APPLY_LIST_DATA";
		public static const RECEIVE_ALL_UNITY_MEMBERS:String = "RECEIVE_ALL_UNITY_MEMBERS";					//接收到所有的帮派成员数据
		
		public static const SHOW_JOIN_UINTY_NEW:String = "SHOW_JOIN_UINTY_NEW";										//打开申请入帮界面
		public static const CLOSE_JOIN_UNITY_NEW:String = "CLOSE_JOIN_UNITY_NEW";									//关闭申请入帮界面
		
		public static const RECEIVE_ALL_UNITYS_LIST:String = "RECEIVE_ALL_UNITYS_LIST";								//获取所有帮派列表
		public static const UPDATE_NEW_UNITY_BASE_INFO:String = "UPDATE_NEW_UNITY_BASE_INFO";			//收到帮派基本信息
		
		public static const UPDATE_SYN_PLACE_INFO:String = "UPDATE_SYN_PLACE_INFO";				//更新分堂信息
		public static const CLOSE_NEW_UNITY_MAIN_PANEL:String = "CLOSE_NEW_UNITY_MAIN_PANEL";				//关闭主面板
		
		public static const SHOW_NEW_UNITY_LOOK_INFO:String = "SHOW_NEW_UNITY_LOOK_INFO";				//打开查看帮派信息界面
		public static const UPDATE_NEW_UNITY_LOOK_INFO:String = "UPDATE_NEW_UNITY_LOOK_INFO";			//更新查看帮派信息界面
		
		public static const OPEN_NEW_UNITY_ORDER_PANEL:String = "OPEN_NEW_UNITY_ORDER_PANEL";				//打开帮派任命
		public static const CLOSE_NEW_UNITY_ORDER_PANEL:String = "CLOSE_NEW_UNITY_ORDER_PANEL";       //关闭帮派任命 
		
		public static const SHOW_SINGLE_MEMBLER_SKIRT:String = "SHOW_SINGLE_MEMBLER_SKIRT";				//显示文本条的下拉列表
		public static const RE_UNITY_NAME_SUCCESS:String = "RE_UNITY_NAME_SUCCESS";
		
		public static const UPDATE_ON_TIME_UNITY_BASEINFO:String = "UPDATE_ON_TIME_UNITY_BASEINFO";			//即时更新帮派基本信息
		
		public static var unityResIsDone:Boolean = false;					//资源是否加载完成
		public static var currentPage:uint = 0;										//当前页
		public static var currentMemPage:uint = 0;								//成员列表当前页
		
		public static var allUnityMemberArr:Array = [];								//所有帮派成员列表
		public static var allApplyMemberArr:Array = [];
		public static var allUnityArr:Array = [];											//所有的帮派列表数据
		public static var unityLevel:int = 1;												//我的当前帮的等级，默认1级
		public static var unityPlaceLevelArr:Array = new Array(4);				//分堂等级 青龙 白虎 玄武 朱雀
		public static var aUnitySkillId:Array = [ [ 2101,2102,2103 ],[ 2201,2202,2203 ],[ 2301,2302,2303 ],[ 2401,2402,2403 ] ];  //分堂技能 青龙 白虎 玄武 朱雀
//		public static const aPlaceName:Array = [ GameCommonData.wordDic[ "mod_uni_dat_uni_green_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_white_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_red_1" ] ];     //青龙堂     白虎堂     玄武堂     朱雀堂
//		public static const aPlaceName:Array = [ "青龙堂","白虎堂","玄武堂","朱雀堂" ];     //青龙堂     白虎堂     玄武堂     朱雀堂
		
		public static var nextComeUnityTime:uint;				//回帮时间
		public static var lastComeUnityDate:Date = new Date();
		public static var reUnityName:uint;			//0不行1行
		
		public static var myUnityInfo:UnityBaseInfo = new UnityBaseInfo();			//我的分堂信息
		public static var closeUnity:Boolean = false;							//屏蔽帮派
		
		public static function get aPlaceName():Array
		{
			if ( GameCommonData.wordVersion == 1 )
			{
				return [ "青龙堂","白虎堂","玄武堂","朱雀堂" ];
			}
			else if ( GameCommonData.wordVersion == 2 )
			{
				return [ "青龍堂","白虎堂","玄武堂","朱雀堂" ];
			}
			return [ "青龙堂","白虎堂","玄武堂","朱雀堂" ];
		}
		
		
	}
}