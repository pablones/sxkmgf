package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionProcessor.SynList;
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ApplyUnityMediator extends Mediator
	{
		public static const NAME:String = "ApplyUnityMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var currentPage:int;
		private var totalPage:int;
		private var countClick:int = 0;
		private var amount:int;
		private var idArray:Array = new Array();							//包含一页帮派ID的数组
		private var timer:Timer;
		private var rankStateList:Array = [];
		private var isAddLis:Boolean = false;                              //是否可以在开场加监听器
		private var getDataNum:int;										   //请求帮派数据的次数
		private var isclearRank:Boolean = false;								//是否清掉了缓存，排序用
		private var isclearPage:Boolean = false;								//是否清掉了缓存，翻页用
		private var rankIndex:int;											//哪一个排行
		private var setTimeSingle:Boolean = false;			//只能有一个计时器
		
		public function ApplyUnityMediator()
		{
			super(NAME);
		}
		
		private function get applyUnity():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			       EventList.INITVIEW,
			       EventList.SHOWUNITYVIEW,
			       EventList.CLOSEUNITYVIEW,
			       UnityEvent.GETAPPLYPAGE,
			       UnityEvent.UPDATEUNITYDATA,
			       UnityEvent.CLOSEAPPLY,
			       UnityEvent.TIMERPROGRESS,
			       UnityEvent.TIMERCOMPLETE,
			       UnityEvent.CLEARAPPLYUNITYARRAY
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.APPLYUNITYVIEW});			
					panelBase = new PanelBase(applyUnity, applyUnity.width+8, applyUnity.height+12);
					panelBase.name = "ApplyUnityView";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos2.x - 180;
					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_aum_han_1" ] ); // 申请加入帮派
					if(applyUnity != null)
					{
						applyUnity.mouseEnabled = false;
						applyUnity.txtPage.mouseEnabled = false;
					}	
				break;
				
				case EventList.SHOWUNITYVIEW:
					UnityConstData.iscreating = int(GameCommonData.Player.Role.unityJob-1) / 100;										//如果职业除以100得到0，则创建成功，不为0，则正在创建中
					if(GameCommonData.Player.Role.unityId == 0 || (UnityConstData.iscreating != 0 && GameCommonData.Player.Role.unityJob != 1100))	//帮派id为0或不是创建中的帮主且已响应了帮派
					{
						if(timer == null) timer = new Timer(3000 * 60 , 1);		
						dataProxy.UnityIsOpen = true;
						showApplyUnityView();
						textinit();              														//文本初始化
				   	 	mcselectinit();																	//选中按钮初始化
				   	 	clearTxt();
				    	currentPage = 1;
				    	totalPage   = 1;
				    	sendPage(207 , currentPage , 0);												//发送请求
				    	isbtnenable();																	//翻页按钮初始化
				    	applyUnity.txtPage.text = currentPage+"/"+totalPage;               	//初始化总页数
				    	if(isAddLis == true) addLis();
					}
				break;
				
				case EventList.CLOSEUNITYVIEW:
				    if(GameCommonData.Player.Role.unityId == 0 || (UnityConstData.iscreating != 0 && GameCommonData.Player.Role.unityJob != 1100))
				    {
				    	 gcAll();
				    }
				break;
				
				case UnityEvent.GETAPPLYPAGE:
					var arr:Array = new	Array();													//得到总页数
					arr = notification.getBody() as Array;
					totalPage = arr[0];
					amount    = arr[1];
					addLis();
				    textinit();              														//文本初始化
				    mcselectinit();																	//选中按钮初始化
				    isbtnenable();																	//翻页按钮初始化
				  	applyUnity.txtPage.text = currentPage+"/"+totalPage;               	//初始化总页数
				  	//排序正反状态
			    	for(var q:int = 0;q < 6;q++)
			    	{
			    		var rankState:Boolean = false;
				    	rankStateList[q] = rankState;
			    	}
				break;
				
				case UnityEvent.UPDATEUNITYDATA:
					var menberDataList:Array = notification.getBody() as Array;
					var menberArr:Array = menberDataList[0] as Array;
					this.getDataNum = menberDataList[1] as int;
					if(this.getDataNum == 1 && isclearRank == false && isclearPage == false)
					{
						updateData(menberArr);
					}
					if(this.getDataNum < this.totalPage)
					{
						sendPage(207 , getDataNum + 1  , 0);												//发送请求
					}
					UnityConstData.allUnityList.push(menberArr);
					var asd:Array = UnityConstData.allUnityList;
					if(this.getDataNum == this.totalPage )
					{
						if(isclearRank == true)									//重新排序
						{
							isclearRank = false;
							rank(this.rankIndex);								//更新固定排行
						}
						else if(isclearPage == true)
						{
							isclearPage = false;
							updateData(UnityConstData.allUnityList[currentPage - 1]);
						}
						setBtnEnable(true);
					}
				break;
				
				case UnityEvent.CLOSEAPPLY:
					if(panelBase)
					{
						if(GameCommonData.GameInstance.GameUI.contains(panelBase))
						gcAll();
					}
				break;
				
				case UnityEvent.TIMERPROGRESS:			//3分钟的间隔时间内的操作通知
		    		 UnityConstData.isRespondUnity = false;
		    		 applyUnity.btnApp.addEventListener(MouseEvent.CLICK , infoHandler);							//点击响应按钮弹出信息
		    	break;
		    	
		    	case UnityEvent.TIMERCOMPLETE:			//3分钟的间隔时间完成后的操作通知
		    		UnityConstData.isRespondUnity = true;
		    		applyUnity.btnApp.removeEventListener(MouseEvent.CLICK , infoHandler);						//点击响应按钮弹出信息
		    	break;
		    	
		    	case UnityEvent.CLEARAPPLYUNITYARRAY:															//清除申请帮派的缓存
		    		updataApply();
		    	break;
			}
		} 
		
		private function gcAll():void
		{
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			clearTxt();
			currentPage = 0;
			dataProxy.UnityIsOpen = false;                               						//申请入帮面板关闭
			this.rankIndex = 0;
			if(dataProxy.UnitInfoIsOpen == true) facade.sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
			UnityConstData.oneUnityId = 0;
			UnityConstData.isOpenNpcView = false;
			for(var n:int = 0;n < 4; n++)
			{
				applyUnity["btnSort_" + n].removeEventListener(MouseEvent.CLICK , sortHandler);
			}
			applyUnity.btnCreateUnity.removeEventListener(MouseEvent.CLICK , enterCreateHandler);
			applyUnity.btnApp.removeEventListener(MouseEvent.CLICK , applyUnityHandler);
			applyUnity.btnRespond.removeEventListener(MouseEvent.CLICK , respondHandler);				//点击响应按钮
			applyUnity.btnFrontPage.removeEventListener(MouseEvent.CLICK,FrontHandler);					//点击向左按钮
			applyUnity.btnBackPage.removeEventListener(MouseEvent.CLICK,BackHandler);					//点击向右按钮
			this.isAddLis = true;
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		
		private function addLis():void
		{
			applyUnity.btnCreateUnity.addEventListener(MouseEvent.CLICK , enterCreateHandler);
			applyUnity.btnApp.addEventListener(MouseEvent.CLICK , applyUnityHandler);
			applyUnity.btnRespond.addEventListener(MouseEvent.CLICK , respondHandler);				//点击响应按钮
			applyUnity.btnFrontPage.addEventListener(MouseEvent.CLICK,FrontHandler);				//点击向左按钮
			applyUnity.btnBackPage.addEventListener(MouseEvent.CLICK,BackHandler);					//点击向右按钮
			timer.addEventListener(TimerEvent.TIMER_COMPLETE , timerCompleteHandler);				//间隔时间结束
			for(var i:int = 0;i < amount ;i++)
			{
				applyUnity["btnselect_" + i].addEventListener(MouseEvent.CLICK, click); 
				applyUnity["mcselect_" + i].addEventListener(MouseEvent.DOUBLE_CLICK,mcdouble_Handler);
			}
			for(var n:int = 0;n < 4; n++)
			{
				applyUnity["btnSort_" + n].addEventListener(MouseEvent.CLICK , sortHandler);
			}
		}
		/** 显示面板 */
		private function showApplyUnityView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
		}
		/** 文本初始化 */
		private function textinit():void
		{
			for(var i:int = 0;i < 12;i++)
			{
				applyUnity["txtname_"+i].mouseEnabled  = false;
				applyUnity["txtman_"+i].mouseEnabled   = false;
				applyUnity["txtlevel_"+i].mouseEnabled = false;
				applyUnity["txtnum_"+i].mouseEnabled   = false;
			}
		}
		/** 点击进入创建帮派界面 */
		private function enterCreateHandler(e:MouseEvent):void
		{
			facade.sendNotification(EventList.CLOSEUNITYVIEW);
			facade.sendNotification(UnityEvent.SHOWCREATEUNITYVIEW);
			if(!dataProxy.BagIsOpen) {
				facade.sendNotification(EventList.SHOWBAG);
				dataProxy.BagIsOpen = true;
			}
		}
		/** 点击申请入帮 */
		private function applyUnityHandler(e:MouseEvent):void
		{
			if(UnityConstData.isRespondUnity == false)										//如果不能响应，申请，创建帮派
			{
				return;
			}
			if(GameCommonData.Player.Role.Level <15)						//如果等级小于15
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_aum_app_1" ], color:0xffff00});  // 你的等级小于15级，不能申请帮派
//				facade.sendNotification(UnityEvent.CLOSERESPONDUNITYVIEW);
				return;
			}
			sendPage(206 , 0 , UnityConstData.oneUnityId);													//发送申请的请求
			facade.sendNotification(UnityEvent.TIMERPROGRESS);												//点击一次后3分钟内不能响应，申请，创建帮派了
			timer.reset();
			timer.start();
		}
		/** 点击响应按钮 */
		private function respondHandler(e:MouseEvent):void
		{
			gcAll();																						//申请页面关闭
			facade.sendNotification(UnityEvent.SHOWRESPONDUNITYVIEW);										//打开响应面板
		}
		/** 点击向左按钮翻页 */
		private function FrontHandler(e:MouseEvent):void
		{
			if(currentPage > 1)
			{
				currentPage--;
				page();
			}
		}
		/** 点击向右按钮翻页 */
		private function BackHandler(e:MouseEvent):void
		{
			if(currentPage < totalPage)
			{
				currentPage++;
				page();
			}
		}
		/** 点击帮派条，有单击和双击两种事件 */
		private function click(e:MouseEvent):void
		{
			var btn:SimpleButton = e.target as SimpleButton;
			var id:int = 0;
			countClick++;
			if(countClick == 1)
			{
		 		id = setTimeout(selectedUnity , 200 , btn);
				e.stopPropagation();
			} 
			else if (countClick == 2) {
				countClick = 0;
				clearTimeout(id);
				e.stopPropagation();
				selectedUnity(btn);											
				double_Handler();																			//双击查看消息
			 }
		}
		/**  点击排序 */
		private function sortHandler(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			rank(i);
			
		}
		/** 选中按钮条初始化 */
		private function mcselectinit():void
		{
			for(var i:int = 0;i < 12;i++)
			{
				applyUnity["mcselect_"+i].visible       = false;
				applyUnity["btnselect_"+i].visible       = false;
				applyUnity["btnselect_"+i].mouseEnabled = true;	
				applyUnity["mcselect_"+i].doubleClickEnabled = true; 
			}
			for(var n:int = 0; n < amount; n++) {
				applyUnity["btnselect_"+n].visible = true;
			}
			
			applyUnity.btnApp.visible = false;																//申请按钮为灰
		}
		/** 判断是否为第一页和最后一页，如果是，按钮变黑 */
		private function isbtnenable():void
		{
			if(currentPage == 1)
				return;
			if(currentPage == totalPage)
				return;
		}
		/** 选中某个帮 */
		private function selectedUnity(btn:SimpleButton):void
		{
			var i:int = btn.name.split("_")[1];
			this.countClick = 0;       																	//单双击事件结束
			mcselectinit();																				//选中按钮初始化																
			applyUnity["mcselect_"+i].visible = true;
			for(var n:int = 0;n < 12;n++)
			{
				applyUnity["mcselect_"+n].visible == true ? applyUnity["btnselect_"+n].mouseEnabled = false:applyUnity["btnselect_"+n].mouseEnabled = true;			//选中的按钮不需要鼠标移上效果
			}
			UnityConstData.oneUnityId = idArray[i];							   												//得到选中的ID
			applyUnity.btnApp.visible = true; 																				//申请按钮为灰
		}
		/** 双击mcselect调用double_Handler()函数*/
		private function mcdouble_Handler(e:MouseEvent):void
		{
			double_Handler();
		} 
		/** 双击查看消息 */
		private function double_Handler():void
		{
			facade.sendNotification(UnityEvent.SHOWUNITYINFOVIEW ,UnityConstData.oneUnityId);	//将帮派ID发送出去
		}
		/** 得到帮派数据，并排列*/
		private function updateData(arr:Array):void
		{
			totalPage = UnityConstData.allUnityList.length == 0 ? totalPage : UnityConstData.allUnityList.length;//UnityConstData.allUnityList.length;
			clearTxt();
			amount = arr.length;
			mcselectinit();					//每次都要判断一下选择条的可用性
			for(var i:int;i < amount ;i++)
			{
				applyUnity["txtname_" + i].text  = arr[i]["1"];
				applyUnity["txtman_" + i].text   = arr[i]["2"];
				applyUnity["txtlevel_" + i].text = arr[i]["3"];
				applyUnity["txtnum_" + i].text   = arr[i]["4"];
				idArray[i] = arr[i]["0"];																	//将帮会id存储在idArray数组里面
			}
			
		}
		/** 发送请求可以申请的帮派方法 */
		private function sendPage(i:int , page:int , id:int):void
		{
			//如果有缓存 , 就不打扰服务器了
			if(UnityConstData.allUnityList[page-1] != null && i != 206 )
			{
				updateData(UnityConstData.allUnityList[currentPage - 1]);
				return;
			}
			
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 ,i, page , id];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求
			if(i != 206)  setBtnEnable(false);
			if(setTimeSingle == false)
			{
				setTimeout(updataApply , 1000 * 60 * 1);													//每隔3分钟更新一下缓存
				setTimeSingle = true;
			}
		}
		/** 清除缓存 */
		/** 得到帮派数据，并排列*/
		private function clearTxt():void
		{
			for(var i:int;i < amount ;i++)
			{
				applyUnity["txtname_" + i].text  = "";
				applyUnity["txtman_" + i].text   = "";
				applyUnity["txtlevel_" + i].text = "";
				applyUnity["txtnum_" + i].text   = "";
				idArray[i] = "";																	//将帮会id存储在idArray数组里面
			}
			
		}
		/** 时间间隔结束 */
		private function timerCompleteHandler(e:TimerEvent):void
		{
			facade.sendNotification(UnityEvent.TIMERCOMPLETE);
		}
		/** 点击申请按钮弹出不可用信息*/
		private function infoHandler(e:MouseEvent):void
		{
			 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_aum_inf_1" ], color:0xffff00});  // 两次操作需要间隔三分钟
		}
		private function updataApply():void
		{
			UnityConstData.allUnityList = [];
			SynList.isFirst = 0;
			setTimeSingle = false;
		}
		/** 排序方法 */
		private function rank(i:int):void
		{
			if(UnityConstData.allUnityList.length == 0) return;
			if(UnityConstData.allUnityList[0] == null)												
			{
				clear();
				rankIndex = i;
				sendPage(207 , 1 , 0);															//发送请求;
				isclearRank = true;																//设置状态，重新排序
				return;
			} 
			var a:Array = UnityConstData.allUnityList;
			if(rankStateList[i] == false)
			{
				rankStateList[i] = true;
				UnityConstData.allUnityList = UIUtils.ArraysortOn(UnityConstData.ApplySORTLIST[i] , UnityConstData.allUnityList , i + 1 , true , 12);
				var b:Array = UnityConstData.allUnityList;
			}
			else
			{
				rankStateList[i] = false;
				UnityConstData.allUnityList = UIUtils.ArraysortOn(UnityConstData.ApplySORTLIST[i] , UnityConstData.allUnityList , i + 1 , false , 12);
			}
			mcselectinit();																					//初始化按钮条
			clearTxt();
			updateData(UnityConstData.allUnityList[0]);
			this.currentPage = 1;
			applyUnity.txtPage.text = currentPage+"/"+totalPage;
		}
		/** 清除 */
		private function clear():void
		{
			currentPage = 1;
			this.totalPage = 1;
		}
		
		/** 翻页方法 */
		private function page():void
		{
			applyUnity.txtPage.text = currentPage+"/"+totalPage;
			clearTxt();
			if(UnityConstData.allUnityList[currentPage - 1] == null)												
			{
				clear();
				isclearPage = true;
				sendPage(207 , currentPage , 0);																		//发送请求;
				return;
			} 
			updateData(UnityConstData.allUnityList[currentPage - 1]);
		}
		private function setBtnEnable(isSee:Boolean):void
		{
			applyUnity.btnFrontPage.visible = isSee;
			applyUnity.btnBackPage.visible = isSee;
			//数据没来之前，按钮禁用
			for(var i:int = 0;i < amount;i++)
			{
				applyUnity["btnselect_"+i].visible       = isSee;
				applyUnity["btnselect_"+i].mouseEnabled  = isSee;	
			}
			for(var n:int = 0;n < 4 ; n++)
			{
				applyUnity["btnSort_" + n].mouseEnabled  = isSee;	
				applyUnity["mcselect_" + n].mouseEnabled  = isSee;	
			}
		}
	}
}