package GameUI.Modules.Campaign.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Campaign.Mediator.UI.UICalendarAward;
	import GameUI.Modules.Campaign.Mediator.UI.UICalendarTitle;
	import GameUI.Modules.CopyLead.Data.CopyLeadData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CalendarMediator extends Mediator
	{
		public static const NAME:String = "CalendarMediator";
		
		private const GOODTYPE:uint = 610053;	//速达灵玉的id
		
		private var price:uint = 0;		//元宝价格 
		private var mainView:MovieClip;
		private var taskView:MovieClip;
		private var awardView:MovieClip;
		private var taskContent:Sprite;
		private var awardContent:Sprite;
		private var page_mc:MovieClip;
		private var fastFinish_mc:MovieClip;
		private var iScrollPane_Task:UIScrollPane;
		private var iScrollPane_Award:UIScrollPane;
		private var panelBase:PanelBase;		//快速完成外壳
		private var finishList:Array = [];		//完成列表，二进制
		private var currentFinishNum:int = 0;	//当前完成的个数
		public var isGetAward:Boolean = false;	//是否领取过奖励
//		public static var SIMBOL_TAG:Boolean;	//发送消息请求标记
		public function CalendarMediator()
		{
			super(NAME);
		}
		public override function listNotificationInterests():Array
		{
			return [
					CampaignData.CHANGEPAGE,
					CampaignData.CLOSECLAENDARVIEW,
					CampaignData.SHOWFASTFINISH,
					CampaignData.HANDLERDATA
					];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case CampaignData.CHANGEPAGE://切换页签
					if(CampaignData.currentPageNum == 0)
					{
						if(page_mc == null)		page_mc	= notification.getBody() as MovieClip;
						if(mainView == null)	mainView = CampaignData.calendarView_mc;
						if(taskView == null)	taskView = CampaignData.calendarTask_mc;
						if(awardView == null)	awardView = CampaignData.calendarAward_mc;
						page_mc.addChild(mainView);
						mainView.y = -18;
						CampaignData.calendarViewIsOpen = true;
						showView();
						
					}
				break;
					
				case CampaignData.CLOSECLAENDARVIEW:
					gcAll();
				break;
				case CampaignData.SHOWFASTFINISH:
					var index:int = notification.getBody() as int;
					if(fastFinish_mc == null) fastFinish_mc = CampaignData.calendarFastFinish_mc;
					controllerFinish(index);
				break;
				case CampaignData.HANDLERDATA:	//收到是否完成的数据
					var dataList:Array = notification.getBody() as Array;
					CampaignData.finishList = [dataList[0] , dataList[1]];		//服务器的数据
					if(CampaignData.calendarViewIsOpen == true) 
					{
						clear();
						showView();		//如果面板是打开的则更新界面
					}
				break;
			}
		}
		private function addLis():void
		{
			mainView.btnFinishAll.addEventListener(MouseEvent.CLICK , finishAllHandler);
			mainView.btnGetAll.addEventListener(MouseEvent.CLICK , getAllHandler);
		}
		private function showView():void
		{
			taskContent  = new Sprite();
			awardContent = new Sprite();
			initData();
			initTask();
			initAward();
			initPane();
			initInfo();
			addLis();
		}
		private function gcAll():void
		{
			if(mainView)
			{
				CampaignData.calendarViewIsOpen = false;
				if(mainView.contains(iScrollPane_Task) && (iScrollPane_Task != null)) mainView.removeChild(iScrollPane_Task);
				if(mainView.contains(iScrollPane_Award) && (iScrollPane_Award!= null)) mainView.removeChild(iScrollPane_Award);
				mainView.btnGetAll.removeEventListener(MouseEvent.CLICK , finishAllHandler);
				mainView.btnGetAll.removeEventListener(MouseEvent.CLICK , getAllHandler);
			} 
			mainView		  = null;
			iScrollPane_Task  = null;
			iScrollPane_Award = null;  
		}
		/** 清除界面 */
		private function clear():void
		{
			if(mainView)
			{
				if(mainView.contains(iScrollPane_Task) && (iScrollPane_Task != null)) mainView.removeChild(iScrollPane_Task);
				if(mainView.contains(iScrollPane_Award) && (iScrollPane_Award!= null)) mainView.removeChild(iScrollPane_Award);
				mainView.btnGetAll.removeEventListener(MouseEvent.CLICK , finishAllHandler);
				mainView.btnGetAll.removeEventListener(MouseEvent.CLICK , getAllHandler);
			} 
			iScrollPane_Task  = null;
			iScrollPane_Award = null;  
		}
		/** 数据初始化 */
		private function initData():void
		{
			this.finishList = getStateList(CampaignData.finishList[0]).concat(getStateList(CampaignData.finishList[1]));
			var len1:int = this.finishList.length;
			currentFinishNum = 0;
			isGetAward = this.finishList[0];
			/*for(var n:int = 1; n < len1; n++)
			{
				if(this.finishList[n] == 1) currentFinishNum++;
			}  */
			for(var n:int = 1; n < 13; n++)
			{
				if(this.finishList[n] == 1) currentFinishNum++;
			} 
		}
		/** 滚动条初始化 */
		private function initPane():void
		{
			iScrollPane_Task = new UIScrollPane(this.taskContent);
			iScrollPane_Task.x = 15;
			iScrollPane_Task.y = 28;
			iScrollPane_Task.width = taskContent.width + 18;
			iScrollPane_Task.height = 380;
			iScrollPane_Task.scrollPolicy = UIScrollPane.SCROLLBAR_ALWAYS;
			iScrollPane_Task.refresh();
			mainView.addChild(iScrollPane_Task);
			
			iScrollPane_Award  = new UIScrollPane(this.awardContent);
			iScrollPane_Award .x = 470;
			iScrollPane_Award .y = 55;
			iScrollPane_Award .width = awardContent.width - 5;
			iScrollPane_Award .height = 432;
			iScrollPane_Award .scrollPolicy = UIScrollPane.SCROLLBAR_ALWAYS;
			iScrollPane_Award .refresh();
			mainView.addChild(iScrollPane_Award);
			
		}
		/** 任务栏的排列 */
		private function initTask():void
		{
			var arr:Array = CampaignData.campaignTaskData;
			var len:int = arr.length;
			for(var i:int = 0; i < len; i++)
			{
				var task:UICalendarTitle = new UICalendarTitle(taskView);
				task.index = i+1;
				task.dataPro = arr[i];
				task.taskIsFinish = this.finishList[i+1];
				taskContent.addChild(task);
				task.x = 0;
				task.y = 5 + i * 95;
			}
		}	
		/** 奖励栏的排列 */
		private function initAward():void
		{
			var arr:Array = CampaignData.campaignAwardData;
			var len:int = arr.length;
			for(var i:int = 0; i < len; i++)
			{
				var award:UICalendarAward = new UICalendarAward(awardView);
				award.index = i+1;
				award.dataPro = arr[i];
				if(currentFinishNum == (i + 1))
				{
					award.taskIsFinish = true;
				}
				awardContent.addChild(award);
				award.x = 0;
				if(i > 0)	
				{
					var upaward:UICalendarAward = awardContent.getChildAt(i-1) as UICalendarAward;
					award.y = upaward.y + upaward.height;
				}
			}
		}
		/** 基本奖励信息 */
		private function initInfo():void
		{
			var exp:int = 0;
			var moneyY:int = 0;
			var moneyS:int = 0;
			var len:int = taskContent.numChildren;
			for(var i:int = 0 ; i < len; i++)
			{
				var task:UICalendarTitle = taskContent.getChildAt(i) as UICalendarTitle;
				if(task.taskIsFinish == true)
				{
					exp += task.taskExp;
					moneyY += task.taskMoneyY;
					moneyS += task.taskMoneyS;
				}
			}
			mainView.txtCurrentNum.mouseEnabled = false;
			mainView.txtGetExp.mouseEnabled = false;
			mainView.txtGetMoney_Y.mouseEnabled = false;
			mainView.txtGetMoney_S.mouseEnabled = false;
			mainView.txtGetAllAward.mouseEnabled = false;
			mainView.txtFinishAll.mouseEnabled	= false;
			
			mainView.txtCurrentNum.text = this.currentFinishNum;
			mainView.mcMoney_Y.gotoAndStop(2);
			
			if(exp > 0)
			{
				mainView.mcMoney_Y.visible = true;
				mainView.mcMoney_S.visible = true;
				mainView.txtGetExp.text = exp + GameCommonData.wordDic[ "often_used_exp" ];//经验
			}
			else 
			{
				mainView.txtGetExp.text = "";
				mainView.mcMoney_Y.visible = false;
				mainView.mcMoney_S.visible = false;
			}
			if(moneyY > 0)
			{
				mainView.mcMoney_Y.visible = true;
				mainView.txtGetMoney_Y.text = int(moneyY / 10000);
			}
			else 
			{
				mainView.txtGetMoney_Y.text = "";
				mainView.mcMoney_Y.visible = false;
			}
			if(moneyS > 0)
			{
				mainView.mcMoney_S.visible = true;
				mainView.txtGetMoney_S.text = int(moneyS / 10000);
			}
			else 
			{
				mainView.txtGetMoney_S.text = "";
				mainView.mcMoney_S.visible = false;
			}
			if(this.isGetAward) 	//0未领取 已领取
			{
				mainView.btnGetAll.visible		= false;
				mainView.btnFinishAll.visible      = false;
				mainView.txtGetAllAward.text = GameCommonData.wordDic[ "mod_cam_med_cal_initInfo_1" ];//"奖励已领取"
				mainView.txtFinishAll.text     = "";
			}
			else
			{
				mainView.btnGetAll.visible		= true;
				mainView.btnFinishAll.visible   = true;
				mainView.txtGetAllAward.text = GameCommonData.wordDic[ "mod_cam_med_cal_initInfo_2" ];//"领取全部奖励"
				mainView.txtFinishAll.text     = GameCommonData.wordDic[ "mod_cam_med_cal_initInfo_3" ];//"全部快速完成"
			}
		}
		/** 点击领取全部奖励 */
		private function getAllHandler(e:MouseEvent):void
		{
			if(currentFinishNum == 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cam_med_cal_getA_1" ], color:0xffff00});//"请完成任务后再领取奖励"
				return;
			}
			 sendNotification(EventList.DO_FIRST_TIP, {comfrim:confrimGetAll,cancel:cancel, info:GameCommonData.wordDic[ "mod_cam_med_cal_getA_2" ],title:GameCommonData.wordDic[ "often_used_smallTip" ],width:245,comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});
			 //"<font color='#ffffff'>该奖励每天只能领取一次，你确定要现在领取吗？<br><font color='#00FF00'>领取的经验不会超过人物当前可拥有经验的上限。</font></font>";		"提示"	 "确认"		"取消"
//			facade.sendNotification(EventList.SHOWALERT, {comfrim:confrimGetAll, cancel:cancel, isShowClose:false, info: "该奖励每天只能领取一次，你确定要现在领取吗？<font color='#00FF00'>领取的经验不会超过人物当前可拥有经验的上限。</font>", title:"提示", comfirmTxt:"确定", cancelTxt:"取消"});
		}
		/** 确定领取 */
		private function confrimGetAll():void
		{
			if(CampaignData.campaignAwardData[currentFinishNum-1][2])
			{
				var tempArr:Array = UIUtils.DeeplyCopy(CampaignData.campaignAwardData[currentFinishNum-1][2]) as Array;
				for(var i:int = 0; i < tempArr.length; i++)
				{
					tempArr[i] = tempArr[i].toString().substr(1);
				}
				if(BagData.canPushGroupBag(tempArr) == true) 	//查看背包栏的数量是否足够
				{
	//				 facade.sendNotification(HintEvents.RECEIVEINFO, {info:"背包已满，请清理背包", color:0xffff00});
					return;
				}
			}
//			SIMBOL_TAG = true;
			var data:Array=[0,0,0,0,0,0,307,0,0];
			var obj:Object={type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
		}
		private function cancel():void{};
		/** 点击快速完成全部的奖励 */
		private function finishAllHandler(e:MouseEvent):void
		{
			facade.sendNotification(CampaignData.SHOWFASTFINISH , 1000);		//1000代表全部领取
		}
		/** 请求是否完成数据 */
		private function sendAction():void
		{
//			SIMBOL_TAG = true; 
			var obj:Object = new Object();
			obj.type = 1010;
			obj.data = [0 , 0 , 0 , 0 , 0 , 0 , 284, 0 , 0];
			PlayerActionSend.PlayerAction(obj);
		}
		/** 得到状态数组,二进制*/
		private function getStateList(num:int):Array
		{
			var list:Array = [];
			var x:int;
			for(var i:int = 0 ; i < 32 ;i++)
			{
				list.push(int(num % 2));
				num = num / 2; 
				if(num == 0) break;
			}
			while(list.length < 32)
			{
				list[list.length] = 0;
			}
			return list;
		}
		/**
		 * ----- 以下是快速完成面板区域 ------------ 
		 * 
		 */
		private var finishIndex:int = 0;
		/** 控制快速完成面板 */
		private function controllerFinish(index:int):void
		{
			if(CampaignData.fastFinishIsOpen)
			{
				panelCloseHandler(null);
			}
			this.price = fastFinish_mc.txtPrice.text;
			finishIndex = index;
			CampaignData.fastFinishIsOpen = true;
			panelBase = new PanelBase(fastFinish_mc , fastFinish_mc.width + 8 , fastFinish_mc.height + 10);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = 300;
			panelBase.y = 200;
			(fastFinish_mc.txtNum as TextField).text = "1";
			(fastFinish_mc.txtNum as TextField).maxChars = 3;
			(fastFinish_mc.txtNum as TextField).restrict = "0-9";
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			fastFinish_mc.btnSub.addEventListener(MouseEvent.CLICK , subClickHandler);
			fastFinish_mc.btnAdd.addEventListener(MouseEvent.CLICK , addClickHandler);
			fastFinish_mc.btnBuy.addEventListener(MouseEvent.CLICK , buyClickHandler);
			fastFinish_mc.txtNum.addEventListener(MouseEvent.CLICK, focusinHandler);
			fastFinish_mc.txtNum.addEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			if(index < 1000)		//单个任务的快速完成
			{
				panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_cam_med_cal_con_1" ]);//"快速完成"
				fastFinish_mc.gotoAndStop(1);
				fastFinish_mc.btnSure.addEventListener(MouseEvent.CLICK , oneSureHandler);
			}
			else 					//全部快速完成
			{
				panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_cam_med_cal_con_2" ]);//"全部快速完成"
				fastFinish_mc.gotoAndStop(2);
				fastFinish_mc.txtFinishNum.text = CampaignData.campaignTaskData.length - this.currentFinishNum;		//未完成的个数
				fastFinish_mc.btnSure.addEventListener(MouseEvent.CLICK , allSureHandler);
			}
			
		}
		/** 关闭快速完成面板 */
		private function panelCloseHandler(e:Event):void
		{
			CampaignData.fastFinishIsOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			fastFinish_mc.btnSub.removeEventListener(MouseEvent.CLICK , subClickHandler);
			fastFinish_mc.btnAdd.removeEventListener(MouseEvent.CLICK , addClickHandler);
			fastFinish_mc.btnBuy.removeEventListener(MouseEvent.CLICK , buyClickHandler);
			fastFinish_mc.txtNum.removeEventListener(MouseEvent.CLICK, focusinHandler);
			fastFinish_mc.txtNum.removeEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			fastFinish_mc.btnSure.removeEventListener(MouseEvent.CLICK , oneSureHandler);
			fastFinish_mc.btnSure.removeEventListener(MouseEvent.CLICK , allSureHandler);
		}
		/** 输入文本获得光标事件 */
		private function focusinHandler(e:MouseEvent):void
		{
			(e.target as TextField).setSelection(0 , (e.target as TextField).length);	
		}
		/** 输入文本失去光标事件 */
		private function focusOutHandler(e:FocusEvent):void
		{
			if((e.target as TextField).text == "") (e.target as TextField).text = "1";
		}
		/** 点击减少按钮 */
		private function subClickHandler(e:MouseEvent):void
		{
			var num:int = int(fastFinish_mc.txtNum.text);
			if(num > 1) num--;
			fastFinish_mc.txtNum.text = String(num);
		}
		/** 点击增加按钮 */
		private function addClickHandler(e:MouseEvent):void
		{
			var num:int = int(fastFinish_mc.txtNum.text);
			if(num < 999) num++;
			fastFinish_mc.txtNum.text = String(num);
		}
		/** 点击购买按钮 */
		private function buyClickHandler(e:MouseEvent):void
		{
			var num:int = int(fastFinish_mc.txtNum.text);
			if(GameCommonData.Player.Role.UnBindRMB < (num * price))
			{
				 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cam_med_cal_buy_1" ], color:0xffff00});// "元宝不足"
				 return;
			}
			facade.sendNotification(EventList.SHOWALERT, {comfrim:confrimBuy, cancel:cancel, isShowClose:false, info: GameCommonData.wordDic[ "mod_cam_med_cal_buy_2" ]+ int(this.price * num) +GameCommonData.wordDic[ "mod_cam_med_cal_buy_3" ]+ num+GameCommonData.wordDic[ "mod_cam_med_cal_buy_4" ], title:GameCommonData.wordDic[ "often_used_tip" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});
			//"你将花费"		 "元宝购买"		"个速达灵玉，确定购买吗？"  	提示		确认  取消
		}
		/** 确定购买 */
		private function confrimBuy():void
		{
			var num:int = int(fastFinish_mc.txtNum.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:this.GOODTYPE , count:num});
		}
		/** 点击确定完成一个 */
		private function oneSureHandler(e:MouseEvent):void
		{
//			SIMBOL_TAG = true;
			var data:Array=[0,0,0,0,0,finishIndex,308,0,0];
			var obj:Object={type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
			if(CampaignData.fastFinishIsOpen)
			{
				panelCloseHandler(null);
			}
		}
		/** 点击确定完成所有 */
		private function allSureHandler(e:MouseEvent):void
		{
			if(CampaignData.campaignTaskData.length - this.currentFinishNum == 0)		//全部完成
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_cam_med_cal_all" ], color:0xffff00});//"你今天的任务已全部完成"
				return;
			}
//			SIMBOL_TAG = true;
			var data:Array=[0,0,0,0,0,0,308,0,0];
			var obj:Object={type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
			if(CampaignData.fastFinishIsOpen)
			{
				panelCloseHandler(null);
			}
		}
	}
}/***
    
*/
