package GameUI.Modules.Unity.UnityUtils
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownEvent;
	import GameUI.Modules.Unity.Command.SendActionCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	
	public class UnityDoWork
	{
		// ["训练新兵","护送军粮","募捐资金","新田开发","木材砍伐","石料采集"]
		private static const WORKLIST:Array = [ GameCommonData.wordDic[ "mod_uni_uni_udw_list_1" ],GameCommonData.wordDic[ "mod_uni_uni_udw_list_2" ],GameCommonData.wordDic[ "mod_uni_uni_udw_list_3" ],
																		GameCommonData.wordDic[ "mod_uni_uni_udw_list_4" ],GameCommonData.wordDic[ "mod_uni_uni_udw_list_5" ],GameCommonData.wordDic[ "mod_uni_uni_udw_list_6" ] ];
		/** 打工得到的奖励类型 */
		// ["研究度" ,  "研究度" , "繁荣度" , "繁荣度" , "建设度" , "建设度"]
		public static const WORKAWARD:Array = [GameCommonData.wordDic[ "mod_uni_med_umm_han_1" ],  GameCommonData.wordDic[ "mod_uni_med_umm_han_1" ] , GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ] , 
																			GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ] , GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ] , GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ] ];
		 
		private static var time:Number;
		
		/** 打工成功后，得到的消息*/
		/**"你给军营的新兵讲解兵法理论，讲得众将士昏昏欲睡。" , "你和军营的新兵参加了实战训练，战斗中拔得头筹，军营的士气空前高涨。"
												,"雁门关杨虎将军委托你将珍贵的药草护送至幽州。途中遭到了马贼的袭击，虽顽强反抗，还是被马贼抢走了一些药草。" , "严殊委托你护送补给物资，途中成功击退偷袭的金国士兵，将物资成功护送至边关，获得了北宋王朝的奖赏。" 
												,"你在辽国皇城卖艺赚钱，结果被辽国禁兵驱赶。" , "你买了本江i湖异闻录碰碰运气，结果第一次就捉到了传说中的大盗黑玉堂。你开心的去领取了一大笔赏金。" 
												,"开垦新田过程中突然天降暴雪，种下的幼苗忍受不住寒冷被冻死，只好提前结束此次行程。" , "你辛勤的劳作，并洒下金克拉肥料，成功开垦了新的田地，增加了乡田的粮食产量。" 
												,"你刚砍下一块木头，就被淘气的幽魂抢走，你急得扔下手中的斧头去追杀幽魂。" , "你展现出色的棋艺让众多棋手折服，他们表示感激，为你砍伐了许多柴火。"
												,"漫天飞舞的萤梦令你昏昏欲睡，睡梦中你和漂亮的雾花婢女一起捉迷藏，醒来后空手而归。" ,"你大番称赞李诫的《营造法式》，李诫命手下工匠协助你采集石料，你满载而归。" */
		public static const WORKMGS:Array = [
												 GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_1" ] , GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_2" ]
												,GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_3" ] , GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_4" ]
												,GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_5" ], GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_6" ] 
												,GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_7" ] , GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_8" ]
												,GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_9" ] , GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_10" ]
												,GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_11" ] ,GameCommonData.wordDic[ "mod_uni_uni_udw_mgs_12" ]
												]
		
		public static var subType:int = 0;	/** 打工的分堂序列号 */
		
		public static function beginWork(type:int):void
		{
			subType = type;
			if(time > 0)
			{
				var date1:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
				if(Number(date1.getTime() - time) < 1000 * 60 * 15)		//更改回来
				{
					if(UnityConstData.isWorking == true)
					{
						GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_uni_udw_beg_1" ], color:0xffff00});  // 你正在打工中
						return;
					}
				}
			}
			GameCommonData.UIFacadeIntance.sendNotification(SendActionCommand.SENDACTION , {type:228 , data:subType});		//发送打工协议
		}
		/** 开始打工了 */
		public static function workGo(workType:int):void
		{
			UnityConstData.isWorking = true;
			var date:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
			time = date.getTime();
			UnityConstData.subWorkType = workType;
			// 你决定去  打工  确定
			GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWALERT, {comfrim:comfirm, cancel:null, isShowClose:false, info: GameCommonData.wordDic[ "mod_uni_uni_udw_wor_1" ] + WORKLIST[workType], title:UnityConstData.otherUnityArray[subType].name + GameCommonData.wordDic[ "mod_uni_uni_udw_wor_2" ], comfirmTxt:GameCommonData.wordDic[ "often_used_commit" ], cancelTxt:null});
			GameCommonData.UIFacadeIntance.sendNotification(TimeCountDownEvent.SHOWTIMECOUNTDOWN , {taskId:20900});			//调出打工计时器
		}
		/** 确定打工 */
		private static function comfirm():void{};
	}
}