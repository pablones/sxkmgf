package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class RespondUnityMediator extends Mediator
	{
		public static const NAME:String = "RespondUnityMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var currentPage:int;
		private var totalPage:int;
		private var Amount:int;
		private var dataArr:Array = new Array();								//包含操作号,帮派ID的数组
		private var idArr:Array = new Array();									//从服务器获取到一页的帮派id数组
		private var unityObj:Object = new Object();								//储存当前选中帮派的信息对象,id，帮派名，通告
		private var H:int;														//选中的行数
		private var timer:Timer;
		public function RespondUnityMediator()
		{
			super(NAME);
		}
		
		public function get respondUnityView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			 		EventList.INITVIEW,
			       	UnityEvent.SHOWRESPONDUNITYVIEW,
			       	UnityEvent.CLOSERESPONDUNITYVIEW,
			       	UnityEvent.UPDATERESPONDDATA,
			       	UnityEvent.GETTOTALPAGE_RESPOND,
			       	UnityEvent.UPDATENUM,
			       	UnityEvent.GETNOTICE,
			       	UnityEvent.TIMERPROGRESS,
			       	UnityEvent.TIMERCOMPLETE,
			       	UnityEvent.RESPONDCHANGEJOP
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.RESPONDUNITYVIEW});
					panelBase = new PanelBase(respondUnityView, respondUnityView.width+8, respondUnityView.height+12);
					panelBase.name = "RespondView"; 
					panelBase.x = UIConstData.DefaultPos2.x - 180;
					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_res_han_1" ] );  // 响应帮派
					if(respondUnityView != null)
					{
						respondUnityView.mouseEnabled = false;
						respondUnityView.txtName.mouseEnabled = false;
						respondUnityView.txtNotice.mouseEnabled = false;
						respondUnityView.txtPage.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWRESPONDUNITYVIEW:
					if(GameCommonData.Player.Role.unityJob == 1100)										//如果创建中的帮主点击了响应面板
					{
						facade.sendNotification(EventList.SHOWUNITYVIEW);								//打开创建中的面板（创建中帮主专属）
						return;
					}
					UnityConstData.respondViewIsOpen = true;
					currentPage = 1;
				    totalPage   = 1;
					showRespondUnityView();
					respondUnityView.txtPage.text   = currentPage+"/"+totalPage;
					mcselectinit();
					btnEnable();						//按钮不可用
					clearTxt();
					sendPage();							//发送请求当前页面数据
					sendNotice(GameCommonData.Player.Role.unityId)					//发送请求当前响应的帮派
		    	break;
		    	
		    	case UnityEvent.CLOSERESPONDUNITYVIEW:
		    		gcAll();
		    	break;
		    	
		    	case UnityEvent.UPDATERESPONDDATA:		//得到数据并排列
		    		var array:Array = notification.getBody() as Array;
		    		clearTxt();							//得到数据前清空一下
		    		updateData(array);
		    	break;
		    	
		    	case UnityEvent.GETTOTALPAGE_RESPOND:	//得到总页数和每一页的总个数
		    		var arrayPage:Array = notification.getBody() as Array;
		    		totalPage = arrayPage[0];
		    		Amount = arrayPage[1];
		    		respondUnityView.txtPage.text = currentPage+"/"+totalPage;
		    		mcselectinit();
					txtinit();
//					isbtnenable();						//按钮变灰
					timer = new Timer(3000 * 60 , 1);														//定义闹钟
			    	addLis();
		    	break;
		    	
		    	case UnityEvent.UPDATENUM:				//更新响应帮派的人数， 响应成功
		    		unityObj.num = notification.getBody();
		    		respondUnityView["txtNum_" + this.H].text = unityObj.num + "/6";
		    		unityObj.notice = respondUnityView.txtNotice.text;						//储存响应的通告
		    		
//		    		if(respondUnityView["txtNum_" + this.H].text == 6)
//		    		{
//		    			clearTxt();
//		    			sendPage();						//人数到达6后更新当前页面的数据
//		    			gcAll();
//		    		}
		    	break;
		    	
		    	case UnityEvent.GETNOTICE:				//得到详细信息
		    		var currentArr:Array = notification.getBody() as Array;
		    		if(UnityConstData.isSelectUnity)
		    		{
		    			if(currentArr[0] == null)
		    			{
		    				respondUnityView.txtNotice.text = "";
		    			}
		    			else
		    			{
		    				respondUnityView.txtNotice.text = currentArr[0];
		    			}
		    			if(currentArr[2] >= 6)
		    			{
		    				clearTxt();
		    				respondUnityView.txtName.text = "";
		    				sendPage();		
		    			}
		    		}
		    		else
		    		{
		    			respondUnityView.txtName.text   = currentArr[1];
		    		}
		    	break;
		    	
		    	case UnityEvent.TIMERPROGRESS:			//3分钟的间隔时间内的操作通知
		    		 UnityConstData.isRespondUnity = false;
		    		 respondUnityView.btnRespond.addEventListener(MouseEvent.CLICK , infoHandler);							//点击响应按钮弹出信息
		    	break;
		    	
		    	case UnityEvent.TIMERCOMPLETE:			//3分钟的间隔时间完成后的操作通知
		    		UnityConstData.isRespondUnity = true;
		    		respondUnityView.btnRespond.removeEventListener(MouseEvent.CLICK , infoHandler);						//点击响应按钮弹出信息
		    	break;
		    	//响应达到6人后，发送一个广播 ， 响应该帮派的人都要入帮
		    	case UnityEvent.RESPONDCHANGEJOP:
		    		if(GameCommonData.Player.Role.unityJob > 100)
		    		{
		    			GameCommonData.Player.Role.unityJob -= 1000;
		    		}
	    			facade.sendNotification(UnityEvent.CLEARAPPLYUNITYARRAY);
	    			if(GameCommonData.Player.Role.unityJob == 100)	facade.sendNotification(EventList.HASUINTY);		//如果是帮主，就发起频道
		    		respondUnityView.txtName.text = ""; 
		    	break;
			}
		}

		private function showRespondUnityView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
		}
		
		private function addLis():void
		{
			 panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			 /** 数据翻页选中侦听器*/
			 respondUnityView.btnFrontPage.addEventListener(MouseEvent.CLICK,FrontHandler);					//点击向左按钮
			 respondUnityView.btnBackPage.addEventListener(MouseEvent.CLICK,BackHandler);					//点击向右按钮
			 respondUnityView.btnRespond.addEventListener(MouseEvent.CLICK,RespondHandler);					//点击响应按钮
//			 timer.addEventListener(TimerEvent.TIMER , timerHandler);										//时间间隔操作
			 timer.addEventListener(TimerEvent.TIMER_COMPLETE , timerCompleteHandler);						//间隔时间结束
			 for(var i:int = 0;i < Amount;i++)
			 {
				respondUnityView["btnselect_" + i].addEventListener(MouseEvent.CLICK, selectHandler); 
			 }
		}
		
		private function gcAll():void
		{
			UnityConstData.respondViewIsOpen = false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);	
			}
			respondUnityView.txtNotice.text = "";
			currentPage = 0;
			clearTxt();
			UnityConstData.isSelectUnity = false;
			timer = null; 
			UnityConstData.isOpenNpcView = false;
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 选中按钮条初始化 */
		private function mcselectinit():void
		{
			for(var i:int = 0;i < 10;i++)
			{
				respondUnityView["mcselect_"+i].visible       	= false;
				respondUnityView["btnselect_"+i].visible 		= false;
				respondUnityView["btnselect_"+i].mouseEnabled 	= true;	
				respondUnityView["mcselect_"+i].doubleClickEnabled = true; 
			}
			for(var n:int = 0;n < Amount ;n++)
			{
				respondUnityView["btnselect_"+n].visible = true;
			} 
		}
		/** 按钮不可用 */
		private function btnEnable():void
		{
			respondUnityView.btnRespond.visible = false;
		}
		/** 动态文本初始化，鼠标不可用*/
		private function txtinit():void
		{
			for(var i:int;i < 10;i++)
			{
				respondUnityView["txtUnity_" + i].mouseEnabled	= false;
				respondUnityView["txtNum_" + i].mouseEnabled 	= false;
				respondUnityView["txtBoss_" + i].mouseEnabled 	= false;
			}
		}
		/** 点击向左按钮翻页 */
		private function FrontHandler(e:MouseEvent):void
		{
			if(currentPage > 1)
			{
				currentPage--;
				respondUnityView.txtPage.text = currentPage+"/"+totalPage;
//				isbtnenable();                                                                           		 	//初始化前后按钮
				mcselectinit();																						//初始化按钮条
				clearTxt();
				sendPage();																							//发送请求
			}
		}
		/** 点击向右按钮翻页 */
		private function BackHandler(e:MouseEvent):void
		{
			if(currentPage < totalPage)
			{
				currentPage++;
				respondUnityView.txtPage.text = currentPage+"/"+totalPage;
//				isbtnenable();                                                                           		 	//初始化前后按钮
				mcselectinit();																						//初始化按钮条
				clearTxt();
				sendPage();																							//发送请求
			}
		}
		/** 点击人物条 */
		private function selectHandler(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			selectedUnity(i);
			unityObj.id = idArr[i];											//获取id号
			unityObj.name = respondUnityView["txtUnity_"+i].text;			//存储帮派名
			sendNotice(unityObj.id)
			this.H = i;														//行数为i
			UnityConstData.isSelectUnity = true;							//选中了帮派
		}
		/** 点击响应按钮 */
		private function RespondHandler(e:MouseEvent):void
		{
			if(UnityConstData.isRespondUnity == false)										//如果不能响应，申请，创建帮派
			{
				return;
			}
			if(GameCommonData.Player.Role.Level <15)						//如果等级小于15
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_res_res_1" ], color:0xffff00});  // 你的等级小于15级，不能响应帮派
				facade.sendNotification(UnityEvent.CLOSERESPONDUNITYVIEW);
				return;
			}
			dataArr[0] = 0;				
			dataArr[1] = 0;
			dataArr[2] = 205;												//响应的操作号
			dataArr[3] = 0;													//默认为0
			dataArr[4] = unityObj.id;
			UnityConstData.unityObj.type = 1107;
			UnityConstData.unityObj.data = dataArr;
			if(UnityConstData.respondUnity == null)							//如果是第一次响应
			{
				if(respondUnityView.txtName.text == unityObj.name)			//响应已响应的帮派
				{
					 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_res_res_2" ], color:0xffff00}); // 你已经响应了该帮派
					 return;
				}
				clearTxt();
				UnityActionSend.SendSynAction(UnityConstData.unityObj);		//发送响应请求
				UnityConstData.respondUnity = unityObj.name;				//将已响应的帮派储存在公共变量里
				if(respondUnityView["txtNum_" + this.H].text >= 6)			//响应达标
	    		{
	    			respondUnityView.txtNotice.text = "";						//清除公告
	    			facade.sendNotification(UnityEvent.CLOSERESPONDUNITYVIEW);	//关闭面板
	    		}
	    		clearTxt();
				sendPage();													//发送请求这一页的数据
			}
			else															//如果是修改响应
			{
				if(respondUnityView.txtName.text == unityObj.name)			//响应已响应的帮派
				{
					 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_res_res_2" ], color:0xffff00});  //你已经响应了该帮派
				}
				else														//响应其它的帮派
				{
					clearTxt();
					UnityActionSend.SendSynAction(UnityConstData.unityObj);	//发送响应请求
					if(respondUnityView["txtNum_" + this.H].text >= 6)		//响应达标
		    		{
		    			respondUnityView.txtNotice.text = "";						//清除公告
		    			facade.sendNotification(UnityEvent.CLOSERESPONDUNITYVIEW);	//关闭面板
		    		}
		    		clearTxt();
					sendPage();												//发送请求这一页的数据
					UnityConstData.respondUnity = unityObj.name;
				}
			}
			facade.sendNotification(UnityEvent.TIMERPROGRESS);				//点击一次后3分钟内不能响应，申请，创建帮派了
			timer.reset();
			timer.start();
			respondUnityView.txtName.text = unityObj.name;					//当前响应的帮派文本
		}
		/** 判断是否为第一页和最后一页，如果是，按钮变黑,响应按钮变灰的条件 */
		//不需要了
//		private function isbtnenable():void
//		{
//			if(currentPage == 1)
//				respondUnityView.btnFrontPage.visible = false;
//			if(currentPage == totalPage)
//				respondUnityView.btnBackPage.visible = false;
//			if(currentPage != 1)
//				respondUnityView.btnFrontPage.visible = true;
//			if(currentPage != totalPage)
//				respondUnityView.btnBackPage.visible = true;
//			if(GameCommonData.Player.Role.unityId == 0)
//			{
//				respondUnityView.btnRespond.visible = true;
//			}
//			else respondUnityView.btnRespond.visible = false;
//		}
		/** 选中某个帮派 */
		private function selectedUnity(i:int):void
		{   																	
			mcselectinit();																						//选中按钮初始化	
			respondUnityView.btnRespond.visible = true;															//响应按钮可用															
			respondUnityView["mcselect_"+i].visible = true;
			for(var n:int = 0;n < 10;n++)
			{
				respondUnityView["mcselect_"+n].visible == true ? respondUnityView["btnselect_"+n].mouseEnabled = false:respondUnityView["btnselect_"+n].mouseEnabled = true;			//选中的按钮不需要鼠标移上效果
			}
		}
		/** 发送请求方法 */
		private function sendPage():void
		{
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , 204 , currentPage , 0]
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送申请响应请求
		}
		/** 发送请求可以查看到通知方法 */
		private function sendNotice(id:int):void
		{
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , 208 , 0 , id];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送请求
		}
		/** 数据排列 */
		private function updateData(array:Array):void
		{
			for(var i:int;i < Amount;i++)
			{
				respondUnityView["txtUnity_"+i].text = array[i][0];
				respondUnityView["txtNum_"+i].text   = array[i][1] + "/6";
				respondUnityView["txtBoss_"+i].text   = array[i][3];
				idArr[i] = array[i][2];
				if(respondUnityView["txtNum_" + i].text == 6)
					respondUnityView["txtNum_" + i].text == "";
			}
		}
		/** 清楚界面txt*/
		private function clearTxt():void
		{
			for(var i:int = 0; i < 10; i++)
			{
				respondUnityView["txtUnity_"+i].text = "";
				respondUnityView["txtNum_"+i].text   = "";
				respondUnityView["txtBoss_"+i].text  = ""; 
				if(idArr[i]) idArr[i] = "";
			}
			respondUnityView.txtNotice.text = "";
		}
		/** 时间间隔结束 */
		private function timerCompleteHandler(e:TimerEvent):void
		{
			facade.sendNotification(UnityEvent.TIMERCOMPLETE);
		}
		/** 点击响应按钮弹出不可用信息*/
		private function infoHandler(e:MouseEvent):void
		{
			 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_aum_inf_1" ], color:0xffff00});  //两次间隔需要三分钟
		}
	}
}