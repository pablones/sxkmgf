package Net.ActionProcessor
{
	import GameUI.Modules.Rank.Data.RankConstData;
	import GameUI.Modules.Rank.Data.RankEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class TopList extends GameAction
	{
		public static const MSGTOPLIST_WORLDLEVEL:int 	= 0;		// 玩家等级
		public static const MSGTOPLIST_WORLDFIRJOB:int 	= 1;		// 主职业
		public static const MSGTOPLIST_WORLDSECJOB:int 	= 2;		// 副职业
		public static const MSGTOPLIST_WORLDMONEY:int   = 3;		// 富豪
		public static const MSGTOPLIST_WORLDPHYATT:int  = 4;		// 外功
		public static const MSGTOPLIST_WORLDMAGATT:int  = 5;		// 内功
		public static const MSGTOPLIST_WORLDMAXLIFE:int = 6;		// 血量
		public static const MSGTOPLIST_WORLDCRIT:int 	= 7;		// 暴击
		public static const MSGTOPLIST_WORLDHITRATE:int = 8;		// 命中
		public static const MSGTOPLIST_WORLDSCORE:int   = 9;		// 评分
		public static const MSGTOPLIST_QUERYTOTAL:int   = 20;		// 查询
//		public static var usAmount:int;
//		private var arr_h:Array   = new Array();                    // 装有一行的排行数据的数组
		private var arr_all:Array = [];                    // 装有这一页全部的排行数据
		public function TopList(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		public override function Processor(bytes:ByteArray):void
		{
			arr_all = [];
			bytes.position = 4;
			var topInfo:Object 		= new Object();
			topInfo.usAction 		= bytes.readUnsignedShort();
			topInfo.usPage 			= bytes.readUnsignedShort();
			topInfo.usAmount 		= bytes.readUnsignedShort();
			topInfo.myRank          = bytes.readUnsignedShort();
			if(topInfo.usAmount > 20)
			{
				return;			
			}
			for(var i:uint = 0 ; i < topInfo.usAmount ; i ++)
			{
				topInfo.data1   = bytes.readUnsignedInt();
				topInfo.data2   = bytes.readUnsignedInt();
				topInfo.szName1 = bytes.readMultiByte(16,GameCommonData.CODE);
				topInfo.szName2 = bytes.readMultiByte(16,GameCommonData.CODE);  	          
				
				switch(topInfo.usAction)
				{
					case  0:
				       arr_all.push([i+1 , topInfo.szName1 , topInfo.szName2 , topInfo.data1]);
				    break;
				    
				    case 1:	
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1], topInfo.data2]);
				    break;
				    
				    case 2:
				      var home:String = GameCommonData.RolesListDic[topInfo.data1];
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1], topInfo.data2]);
				    break;
				    
				    case 3:
				      arr_all.push([int(i+1) , topInfo.szName1 ,topInfo.szName2 , topInfo.data1]);
				    break;
				    
				    case 4:
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1] , topInfo.data2]);
				    break;
				    
				    case 5:
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1], topInfo.data2]);
				    break;
				    
				    case 6:
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1], topInfo.data2]);
				    break;
				    
				    case 7:
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1],  topInfo.data2]);
				    break;
				    
				    case 8:
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1], topInfo.data2]);
				    break;
				    
				    case 9:
				      arr_all.push([int(i+1) , topInfo.szName1 , GameCommonData.RolesListDic[topInfo.data1],  topInfo.data2]);
				    break;
				    
				    case 10:
				      arr_all.push([int(i+1) , topInfo.szName1 , topInfo.data1,  topInfo.data2]);
				    break;
				    
				    case 11:
				      arr_all.push([int(i+1) , topInfo.szName1 , topInfo.data1 , topInfo.data2]);
				    break;
				    
				    case 12:
				      arr_all.push([int(i+1) , topInfo.szName1 , topInfo.szName2,  topInfo.data1]);
				    break;
				    
				      
				}
			}
			RankConstData.total_h   = topInfo.usAmount;
//			RankConstData.totalPage = int((topInfo.usAmount-1) / 20 ) + 1;                          		//總頁數算法
			facade.sendNotification(RankEvent.UPDATERANKDATA , arr_all);                                  	//发送页面数据排行
			facade.sendNotification(RankEvent.MYRANK,topInfo.myRank);                                                     	//发送我的排行
			arr_all = [];
//			trace("totalPage=",RankMediator.totalPage);
//			usAmount = topInfo.usAmount;

		}
	}
}