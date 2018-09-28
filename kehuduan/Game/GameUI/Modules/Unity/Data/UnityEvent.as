package GameUI.Modules.Unity.Data
{
	public class UnityEvent
	{
		public static const CREATEUNITYGRID:String = "CREATEUNITYGRID";			//创建格子消息
		public static const GETAPPLYPAGE:String = "GETTOTALPAGE";				//得到申请面板的总页数消息
		public static const GETTOTALPAGE_RESPOND:String = "GETTOTALPAGE_RESPOND";//得到响应面板总页数消息
		public static const GETMENBERLIST:String = "GETMENBERLIST";				//得到主角在帮派中的职位，权限
		public static const CREATEUNITY:String = "CREATEUNITY";					//得到创建帮派成功与否的消息
		public static const UPDATEUNITYDATA:String = "UPDATEUNITYDATA";   		//得到要申请的帮派的数据
		public static const UPDATEAPPLYDATA:String = "UPDATEAPPLYDATA";			//得到申请帮派的人物数据
		public static const UPDATERESPONDDATA:String = "UPDATERESPONDDATA";		//得到响应门派的数据
		public static const UPDATEORDAINDATA:String = "UPDATEORDAINDATA:";		//得到任命面板的数据
		public static const UPDATECREATINGDATA:String = "UPDATECREATINGDATA:";	//得到帮派创建中面板的数据(响应的人数)
		public static const UPDATENUM:String = "UPDATENUM";						//更新响应帮派人数
		public static const GETPAGEINFO:String = "GETPAGEINFO";					//得到帮派界面的页面信息
		public static const GETINFO:String = "GETINFO";							//得到帮派信息
		public static const GETLISTPAGE:String = "GETLISTPAGE";					//得到申请列表的页码
		public static const UPTATAINFODATE:String = "UPTATAINFODATE";			//得到申请帮派的信息
		public static const GETNOTICE:String = "GETNOTICE";						//得到响应面板所需的通知
		public static const UNITYUPDATA:String = "UNITYUPDATA";					//更新帮派界面
		public static const APPLYUPDATA:String = "APPLYUPDATA"; 				//更新申请列表界面
		public static const UPDATANOTICE:String = "UPDATANOTICE";				//修改公告后立刻更新主界面的公告
		public static const CLOSEUNTIY:String = "CLOSEUNTIY";					//被踢出帮会后，关闭主界面，没有限制的
		public static const CLOSEAPPLY:String = "CLOSEAPPLY";					//被加入帮会后，关闭申请界面
		public static const TIMERPROGRESS:String = "TIMERPROGRESS";				//3分钟的间隔时间内的操作通知
		public static const TIMERCOMPLETE:String = "TIMERCOMPLETE";				//3分钟的间隔时间完成后的操作通知
		public static const RESPONDCHANGEJOP:String = "ESPONDCHANGEJOP";		//响应成功后发送广播
//		public static const UPDATAAPPLY:String = "UPDATAAPPLY";					//刷新申请帮派面板
		public static const ADDMENBER:String = "ADDMENBER";						//获得加入成员的数据信息
		public static const INITUNITYVIEW:String = "INITUNITYVIEW";				//帮派分页初始化
		public static const GETUNITYOTHERDATA:String = "GETUNITYOTHERDATA";		//得到帮派分堂数据
		public static const CLEARAPPLYUNITYARRAY:String ="CLEARAPPLYUNITYARRAY";//清除申请帮派的缓存事件
		public static const BAGTOCONTRIBUTE:String = "BAGTOCONTRIBUTE";			//背包发给捐献的物品事件
		public static const MENBERUPDOWNLINE:String = "MENBERUPDOWNLINE";		//成员上下线时间
		public static const CLEARALL:String = "CLEARALL";						//清除所有的缓存
		public static const CHANGEHIRENUM:String = "CHANGEHIRENUM";				//修改雇佣人数成功
		public static const UPDATEOTHERDATA:String = "UPDATEOTHERDATA";			//更新分堂的显示数据
		public static const UPDATEMENBERDATA:String = "UPDATEMENBERDATA";		//更新帮派成员的显示数据
		public static const SKILLSTUDIED:String = "SKILLSTUDIED";				//分堂技能研究成功返回数据
		public static const SUBDOWORK:String = "SUBDOWORK";						//分堂打工返回操作是否陈宫
		public static const WORKFINISH:String = "WORKFINISH";					//分堂打工完成事件
		public static const CONTRIBUTEFINISH:String = "GETMONEY";				//捐献得到的金钱
		public static const SHOWMYJOP:String = "SHOWMYJOP";						//显示我的职位
		
		
		
		/** 显示申请帮派的人物数据 */
		public static const SHOWAPPLYLISTVIEW:String = "SHOWAPPLYLISTVIEW";	
		/** 关闭申请帮派的人物数据 */
		public static const CLOSEAPPLYLISTVIEW:String = "CLOSEAPPLYLISTVIEW";	
		/** 显示创建帮派  */
		public static const SHOWCREATEUNITYVIEW:String = "ShowCreateUnityView";
		/** 关闭创建帮派  */
		public static const CLOSECREATEUNITYVIEW:String = "CloseCreateUnityView";
		/** 打开帮派信息  */
		public static const SHOWUNITYINFOVIEW:String = "SHOWUNITYINFOVIEW";
		/** 关闭帮派信息  */
		public static const CLOSEUNITYINFOVIEW:String = "CLOSEUNITYINFOVIEW";
		/** 打开响应帮派  */
		public static const SHOWRESPONDUNITYVIEW:String = "SHOWRESPONDUNITYVIEW";
		/** 关闭响应帮派  */
		public static const CLOSERESPONDUNITYVIEW:String = "CLOSERESPONDUNITYVIEW";
		/** 打开任命面板  */
		public static const SHOWORDAINVIEW:String = "SHOWORDAINVIEW";
		/** 关闭任命面板  */
		public static const CLOSEORDAINVIEW:String = "CLOSEORDAINVIEW";
		/** 打开修改通知面板  */
		public static const SHOWPERFECTVIEW:String = "SHOWPERFECTVIEW";
		/** 关闭修改通知面板  */
		public static const CLOSEPERFECTVIEW:String = "CLOSEPERFECTVIEW";
		/** 打开捐献面板  */
		public static const SHOWCONTRIBUTEVIEW:String = "SHOWCONTRIBUTEVIEW";
		/** 关闭捐献面板  */
		public static const CLOSECONTRIBUTETVIEW:String = "CLOSECONTRIBUTETVIEW";
		/** 打开分页面板 */
		public static const SHOWUNITYPAGEVIEW:String = "SHOWUNITYPAGEVIEW";
		/** 关闭分页面板 */
		public static const CLOSEUNITYPAGEVIEW:String = "CLOSEUNITYPAGEVIEW";
		/** 打开雇佣面板 */
		public static const SHOWHIREVIEW:String = "SHOWHIREVIEW";
		/** 关闭雇佣面板 */
		public static const CLOSEHIREVIEW:String = "CLOSEHIREVIEW";
		
	}
}