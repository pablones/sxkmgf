package GameUI.Modules.Meridians.model
{
	public class MeridiansEvent
	{
		
		
		public static const UPDATA_MERIDIANS_DATA:String = "UPDATA_MERIDIANS_DATA";							//更新经脉数据

		public static const INIT_MERIDIANS_MAIN:String = "INIT_MERIDIANS_MAIN";								//初始化经脉主界面
		public static const SHOW_MERIDIANS_MAIN:String = "SHOW_MERIDIANS_MAIN";								//显示经脉主界面
		public static const CLOSE_MERIDIANS_MAIN:String = "CLOSE_MERIDIANS_MAIN";							//关闭经脉主界面
		public static const INIT_MERIDIANS_STRENG:String = "INIT_MERIDIANS_STRENG";							//初始化经脉强化界面
		public static const SHOW_MERIDIANS_STRENG:String = "SHOW_MERIDIANS_STRENG";							//显示经脉强化界面
		public static const CLOSE_MERIDIANS_STRENG:String = "CLOSE_MERIDIANS_STRENG";						//关闭经脉强化界面
		public static const INIT_MERIDIANS_LEARN_LIST:String = "INIT_MERIDIANS_LEARN_LIST";					//初始化经脉修炼队列界面
		public static const SHOW_MERIDIANS_LEARN_LIST:String = "SHOW_MERIDIANS_LEARN_LIST";					//显示经脉修炼队列界面
		public static const CLOSE_MERIDIANS_LEARN_LIST:String = "CLOSE_MERIDIANS_LEARN_LIST";				//关闭经脉修炼队列界面
		public static const INIT_MERIDIANS_LEARN_PROMPT:String = "INIT_MERIDIANS_LEARN_PROMPT";				//初始化经脉修炼提示界面
		public static const SHOW_MERIDIANS_LEARN_PROMPT:String = "SHOW_MERIDIANS_LEARN_PROMPT";				//显示经脉修炼提示界面		
		public static const CLOSE_MERIDIANS_LEARN_PROMPT:String = "CLOSE_MERIDIANS_LEARN_PROMPT";				//关闭经脉修炼提示界面
		public static const INIT_ADDZHENYUAN:String = "INIT_ADDZHENYUAN";									//初始化补充真元界面
		public static const SHOW_ADDZHENYUAN:String = "SHOW_ADDZHENYUAN";									//显示补充真元界面
		public static const CLOSE_ADDZHENYUAN:String = "CLOSE_ADDZHENYUAN";									//关闭补充真元界面
		
		
		public static const QUERYMERIDIANSINFO:String = "actionQueryJinmeiInfo";								//获取自己的经脉信息
		public static const COMPLETE_MERIDIANS_UPGRADE:String = "COMPLETE_MERIDIANS_UPGRADE";					//某经脉修炼完成 Type(int)
		public static const AUTO_MERIDIANS_LEARN:String = "AUTO_MERIDIANS_LEARN";								//某经脉自动修炼  Type(int) Time(int)
		
		public static const RESULT_MERIDIANS_OPERATE:String = "RESULT_MERIDIANS_OPERATE";						//某经脉的操作结果 3个int
		
		
		/********* 经脉操作结果 ***********/
		//第三个数代表类型，第四个数代表时间（开始修炼需要），第五个数代表操作结果
		public static const RESULT_ADDWAITQUEUE_FAIL:String = "RESULT_ADDWAITQUEUE_FAIL";						//加入等待队列失败
		public static const RESULT_ADDWAITQUEUE_SUC:String = "RESULT_ADDWAITQUEUE_SUC";							//加入等待队列成功
		
		public static const RESULT_STARTLEARN_FAIL:String = "RESULT_STARTLEARN_FAIL";							//开始修炼失败
		public static const RESULT_STARTLEARN_SUC:String = "RESULT_STARTLEARN_SUC";							//开始修炼成功
		
		public static const RESULT_UPLEV_FAIL:String = "RESULT_UPLEV_FAIL";									//升级失败  加速后的返回结果
		public static const RESULT_UPLEV_SUC:String = "RESULT_UPLEV_SUC";									//升级成功  完全加速 直接升级完成
		public static const RESULT_UPLEV_PART:String = "RESULT_UPLEV_PART";							 		//经脉加速
		
		public static const RESULT_STRENGTHJINMEI_FAIL:String = "RESULT_STRENGTHJINMEI_FAIL";				//强化失败
		public static const RESULT_STRENGTHJINMEI_SUC:String = "RESULT_STRENGTHJINMEI_SUC";					//强化成功
		
		public static const RESULT_MOVEWAITQUEUE_FAIL:String = "RESULT_MOVEWAITQUEUE_FAIL";					//移除队列失败
		public static const RESULT_MOVEWAITQUEUE_SUC:String = "RESULT_MOVEWAITQUEUE_SUC";					//移除队列成功

		public static const INIT_LRARN_NUM:String = "INIT_LRARN_NUM";										//获取最大允许修炼数目 /***和最大允许等待数目***/
		
		public static const GET_ARCHEAUS_SUC:String = "GET_ARCHEAUS_SUC";							//领取真元成功
		public static const GET_ARCHEAUS_FAIL:String = "GET_ARCHEAUS_FAIL";							//领取真元失败
		
		public static const UPDATA_ARCHEAUS:String = "UPDATA_ARCHEAUS";								//更新真元值
		
		public static const UPDATA_STRENGTH_AND_MAIN:String = "UPDATA_STRENGTH_AND_MAIN";			//关联经脉主界面和经脉强化界面
		
		public static const UPDATA_STRENGTH_PROTECT:String = "UPDATA_STRENGTH_PROTECT";				//更新强化丹和保护丹的数量变化
		
		public static const UPDATA_ARCHEAUS_DAN:String = "UPDATA_ARCHEAUS_DAN";					//更新真元丹数量
	}
}