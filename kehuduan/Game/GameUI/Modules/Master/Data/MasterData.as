package GameUI.Modules.Master.Data
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Master.Proxy.MasResProvider;
	
	import flash.display.DisplayObject;
	import flash.filters.ColorMatrixFilter;
	import flash.filters.GlowFilter;
	import flash.utils.Dictionary;
	
	public class MasterData
	{
		public static const SHOW_MAIN_VIEW:String = "SHOW_MAIN_VIEW";
		public static const SHOW_AWARD_VIEW:String = "SHOW_AWARD_VIEW";
		public static const UPDATA_MASTER_LIST:String = "UPDATA_MASTER_LIST";
		public static const SHOW_AWARD_LIST:String = "SHOW_AWARD_LIST";
		public static const BETRAY_MASTER:String = "BETRAY_MASTER";
		public static const BETRAY_PRENTICE:String = "BETRAY_PRENTICE";
		public static const SHOW_BETRAY:String = "SHOW_BETRAY";
		
		public static var maxInitiator:uint = 5000;                                                 //最大传授值
		public static var isRequestBetray:Boolean = false;
		public static var isRequestAward:Boolean = false;
		public static var isRequestList:Boolean = false;
		
		//新师徒
		public static const CLICK_MASTER_NPC:String = "CLICK_MASTER_NPC";					//点击了NPC
		public static const RECEIVE_REGIST_INFO:String = "RECEIVE_REGIST_INFO";			//收到了登记信息
		public static const MASTER_RES_LOAD_COM:String = "MASTER_RES_LOAD_COM";		// 师徒资源加载完毕
		public static const STUDENT_RES_LOAD_COM:String = "STUDENT_RES_LOAD_COM";			//师徒资源加载完毕，从徒弟列表加载
		public static const CLICK_STUDENT_NPC:String = "CLICK_STUDENT_NPC";					//点击打开徒弟列表
		public static const MASTER_STU_UI_INITVIEW:String = "MASTER_STU_UI_INITVIEW";		//我的师徒信息第二帧初始化
		
		public static const START_SHOW_MAS_PAGE:String = "START_SHOW_MAS_PAGE";				//发送显示当前页
		public static const CLEAR_MASTER_PANEL_PAGE:String = "CLEAR_MASTER_PANEL_PAGE";		//清空页面东西
		public static const REC_STU_APPLY_LIST_DATA:String = "REC_STU_APPLY_LIST_DATA";											//徒弟申请列表获取数据
		public static const REC_MAS_STU_INFO:String = "REC_MAS_STU_INFO";
		public static const GO_MS_MC_PAGE_CUR:String = "GO_MS_MC_PAGE_CUR";								//去到当前页
		public static const DELETE_STUDENT_APPLY:String = "DELETE_STUDENT_APPLY";						//删除徒弟申请列表
		public static const DELETE_MY_OWN_STUDENT:String = "DELETE_MY_OWN_STUDENT";				//删除我的徒弟
		
		public static const CLOSE_MASTER_VIEW:String = "CLOSE_MASTER_VIEW";					//关闭师徒界面
		public static const REC_MY_STUDENT_LIST:String = "REC_MY_STUDENT_LIST";			//收到我的徒弟信息
		public static const REC_CHECK_HAS_GRADUATE:String = "REC_CHECK_HAS_GRADUATE";				//收到是否有已出师的徒弟信息
		public static const REC_GRADUATE_STUDENT_LIST_DATA:String = "REC_GRADUATE_STUDENT_LIST_DATA";			//收到已出师的列表消息
		
		public static const MASTER_TOOLTIP_HINT:Array = [
																								GameCommonData.wordDic[ "mod_mas_dat_mas_mas_1" ],     //传授度决定传授等级，可通过完成师徒任务、出师徒弟获取传授度
																								GameCommonData.wordDic[ "mod_mas_dat_mas_mas_2" ],                 //传授等级1级时可收两名徒弟，每升1级可多收一名
																								GameCommonData.wordDic[ "mod_mas_dat_mas_mas_3" ],     //与徒弟组队杀死徒弟5级范围内的怪物，可获得师德。师德可用来领取成长奖励
																								GameCommonData.wordDic[ "mod_mas_dat_mas_mas_4" ]      //你的历史出师的徒弟总数。出师后的徒弟每升10级，可领取大量经验和师德
																								];
		
		public static var masterResIsDone:Boolean = false;							//师徒资源是否已经加载
		public static var startLoadMaster:Boolean = false;								//师徒资源是否正在加载
		public static var MasterCellClass:Class;
		public static var StudentCellClass:Class;
		public static var masResPro:MasResProvider = new MasResProvider();
		
		public static function getMasterDeno(rate:uint):uint
		{
			var num:uint;
			if ( (rate >= 0) && (rate <= 5000) )																			//一级师德
			{
				num = 5000;
				return num;
			}
			else if ( (rate >= 5001) && (rate <= 15000) )															//二级师德
			{
				num = 15000;
				return num;
			}
			else if ( (rate >= 15001) && (rate <= 30000) )															//三级师德
			{
				num = 30000;
				return num;
			}
			else if ( (rate >= 30001) && (rate <= 50000) )															//四级师德
			{
				num = 50000;
				return num;
			}
			else if ( (rate >= 50001) && (rate <= 75000) )															//五级师德
			{
				num = 75000;
				return num;
			}
			else if ( (rate >= 75001) && (rate <= 105000) )															//六级师德
			{
				num = 105000;
				return num;
			}
			else if ( (rate >= 105001) && (rate <= 140000) )															//七级师德
			{
				num = 140000;
				return num;
			}
			else if ( (rate >= 140001) && (rate <= 180000) )															//八级师德
			{
				num = 180000;
				return num;
			}
			else if ( (rate >= 180001) && (rate <= 225000) )															//九级师德
			{
				num = 225000;
				return num;
			}
			else if ( (rate >= 225001) && (rate <= 275000) )															//十级师德
			{
				num = 275000;
				return num;
			}else
			{
				return num;
			}
		}
		
		public static function getPrenticeRemark(rate:uint):String
		{
			var str:String;
			var level:int = getChuanShouLevel( rate );
			if ( level == 1 )																			//一级师德
			{
				str = "<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_1" ]+"</font>";                   //1级 一无所知
				return str;
			}
			else if ( level == 2 )															//二级师德
			{
				str = "<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_2" ]+"</font>";                   //2级 半生不熟
				return str;
			}
			else if ( level == 3 )															//三级师德
			{
				str = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_3" ]+"</font>";                   //3级 初窥门径
				return str;
			}
			else if ( level == 4 )															//四级师德
			{
				str = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_4" ]+"</font>";                   //4级 略有小成
				return str;
			}
			else if ( level == 5 )															//五级师德
			{
				str = "<font color='#0066cc'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_5" ]+"</font>";                   //5级 渐入佳境
				return str;
			}
			else if ( level == 6 )															//六级师德
			{
				str = "<font color='#0066cc'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_6" ]+"</font>";                   //6级 心领神会
				return str;
			}
			else if ( level == 7 )															//七级师德
			{
				str = "<font color='#ff00ff'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_7" ]+"</font>";                   //7级 豁然贯通
				return str;
			}
			else if ( level == 8 )															//八级师德
			{
				str = "<font color='#ff00ff'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_8" ]+"</font>";                   //8级 出类拔萃
				return str;
			}
			else if ( level == 9 )															//九级师德
			{
				str = "<font color='#ff9900'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_9" ]+"</font>";                   //9级 一代名师
				return str;
			}
			else if ( level == 10 )															//十级师德
			{
				str = "<font color='#ff9900'>"+GameCommonData.wordDic[ "mod_mas_dat_mas_get_10" ]+"</font>";                   //10级 震古烁今
				return str;
			}else
			{
				return str;
			}
		}
		
		//获得传授等级
		public static function getChuanShouLevel( num:int ):int
		{
			if ( num < 1000 ) 
			{
				return 1;
			}
			var level:int = 1;
			var dic:Dictionary = UIConstData.ExpDic;
			var aa:* = UIConstData.ExpDic[ 9001+i ];
			for ( var i:uint=1; i<10; i++ )
			{
				if ( num>=int( UIConstData.ExpDic[ 9000+i ] ) && num<int( UIConstData.ExpDic[ 9001+i ] ) )
				{
					level = i+1;
				}
//				else
//				{
//					level = 10;
//				}
			}
			if ( num >= int( UIConstData.ExpDic[ 9001+9 ] )  )
			{
				level = 10;
			}
			return level;
		}
		
		public static function setGrayFilter( obj:DisplayObject ):void
		{
			var mat:Array = [];
			mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // red
            mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // green
            mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // blue
            mat = mat.concat([0, 0, 0, 1, 0]);                                                                  // alpha
            var grayFilter:ColorMatrixFilter = new ColorMatrixFilter(mat);
            obj.filters = [ grayFilter ];
		}
		
		public static function delGrayFilter( obj:DisplayObject ):void
		{
			var filterArr:Array = obj.filters;
			for ( var i:int=filterArr.length-1; i>=0; i-- )
			{
				if ( filterArr[ i ] is ColorMatrixFilter )
				{
					filterArr.splice( i,1 );
//					break;
				}
			}
		}
		
		//加个黑色滤镜
		public static function addGlowFilter( obj:DisplayObject ):void
		{
			var glowFilter:GlowFilter = new GlowFilter();
			glowFilter.color = 0x000000;
			glowFilter.blurX = 2;
			glowFilter.blurY = 2;
			glowFilter.strength = 6;
			obj.filters = [ glowFilter ];
		}

	}
}