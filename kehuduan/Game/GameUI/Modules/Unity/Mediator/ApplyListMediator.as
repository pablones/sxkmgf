package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.*;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.FriendSend;
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ApplyListMediator extends Mediator
	{
		public static const NAME:String = "ApplyListMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var currentPage:int;
		private var totalPage:int;
		private var amount:int;
		private var countClick:int = 0;
		private var idArray:Array = new Array();						//存储了一页玩家ID的数组
		private var playName:String;									//选中玩家的名称
		private var currentPageArray:Array = [];						//当前页数组
		public function ApplyListMediator()
		{
			super(NAME);
		}
		
		public function get applyUnityList():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			 		EventList.INITVIEW,
			       	UnityEvent.SHOWAPPLYLISTVIEW,
			       	UnityEvent.CLOSEAPPLYLISTVIEW,
			       	UnityEvent.UPDATEAPPLYDATA,
			       	UnityEvent.GETLISTPAGE,
			       	UnityEvent.APPLYUPDATA
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.APPLYUNITYLIST});
					panelBase = new PanelBase(applyUnityList, applyUnityList.width+8, applyUnityList.height+12);
					panelBase.name = "ApplyUnityList";
					panelBase.x = 50;//UIConstData.DefaultPos2.x - 600;
					panelBase.y = 58;//UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_alm_han_1" ] );  // 申请列表
					if(applyUnityList != null)
					{
						applyUnityList.mouseEnabled = false;
						applyUnityList.txtPage.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWAPPLYLISTVIEW:
					showApplyUnityList();
					mcselectinit();
					isbtnenable();
			    	currentPage = 1;
			    	totalPage   = 1;
			    	applyUnityList.txtPage.text = currentPage+"/"+totalPage;
			    	cleatTxt();												//发送请求前清空文本
					sendAction(210 , currentPage , 0);						//发送申请列表请求
		    	break;
		    	
		    	case UnityEvent.CLOSEAPPLYLISTVIEW:
		    		gcAll();
		    	break;
		    	
		    	case UnityEvent.UPDATEAPPLYDATA:							//获取一页数据
		    		var arrDate:Array = notification.getBody() as Array;
		    		updateData(arrDate);
		    		currentPageArray = arrDate;
		    	break;
		    	
		    	case UnityEvent.GETLISTPAGE:								//获取页码信息
		    		var arrpage:Array = notification.getBody() as Array;
		    		totalPage = arrpage[1];
		    		amount    = arrpage[0];
		    		addLis();
		    		isbtnenable();
		    		mcselectinit();
		    		applyUnityList.txtPage.text = currentPage+"/"+totalPage;
		    	break;
		    	
		    	case UnityEvent.APPLYUPDATA:
		    		var action:int = notification.getBody() as int;
		    		if(action == 216)
		    		{
		    			cleatTxt();
		    			sendAction(210 , currentPage , 0);												//发送申请列表请求
		    		}
		       break;
		       
			}
		}
		
		private function showApplyUnityList():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			panelBase.x = 50;
			panelBase.y = 58;
			UnityConstData.applyViewIsOpen = true;
		}
		
		private function addLis():void
		{
			 panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			 applyUnityList.btnFrontPage.addEventListener(MouseEvent.CLICK,FrontHandler);				//点击向左按钮
			 applyUnityList.btnBackPage.addEventListener(MouseEvent.CLICK,BackHandler);					//点击向右按钮
			 applyUnityList.btnAgree.addEventListener(MouseEvent.CLICK,agreeHandler);					//点击同意按钮
			 applyUnityList.btnRefuse.addEventListener(MouseEvent.CLICK,refuseHandler);					//点击拒绝按钮
			 for(var i:int = 0;i < amount;i++)
			 {
				applyUnityList["btnselect_" + i].addEventListener(MouseEvent.CLICK , click); 
				applyUnityList["mcselect_" + i].addEventListener(MouseEvent.DOUBLE_CLICK , mcdouble_Handler);
			 }
		}
		
		private function gcAll():void
		{
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			UnityConstData.applyViewIsOpen = false;
			playName = "";
			idArray = [];
			cleatTxt();
			amount = 0;
			currentPage = 0;
			totalPage   = 0;
		}
		
		private function panelCloseHandler(e:Event):void
		{
			facade.sendNotification(UnityEvent.CLOSEAPPLYLISTVIEW);
		}
		/** 点击向左按钮翻页 */
		private function FrontHandler(e:MouseEvent):void
		{
			if(currentPage > 1)
			{
				currentPage--;
				applyUnityList.txtPage.text = currentPage+"/"+totalPage;
				cleatTxt();																							//发送请求前清空文本                                                                          			
				sendAction(210 , currentPage , 0);																	//发送申请列表请求
			}
		}
		/** 点击向右按钮翻页 */
		private function BackHandler(e:MouseEvent):void
		{
//			isbtnenable();                                                                           			 //初始化前后按钮
			if(currentPage < totalPage)
			{
				currentPage++;
				applyUnityList.txtPage.text = currentPage+"/"+totalPage;
				cleatTxt();																							//发送请求前清空文本
				sendAction(210 , currentPage , 0);																	//发送申请列表请求
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
				double_Handler();																				//双击查看消息
			 }
		}
		/** 点击同意按钮*/
		private function agreeHandler(e:MouseEvent):void
		{
			if(GameCommonData.Player.Role.unityJob < 80)				//堂主以下权限不足
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00})  // 你的权限不足
				return;
			}
			if(UnityConstData.mainUnityDataObj.unityMenber >= UnityNumTopChange.change(int(UnityConstData.mainUnityDataObj.level)))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_2" ], color:0xffff00})  // 帮派人数已达到现等级的上线
				return;
			}
			for(var i:int = 0;i < this.currentPageArray.length; i++)
			{
				if(this.currentPageArray[i][6] == UnityConstData.playerId)
				{
					UnityConstData.updataArr = [];
					UnityConstData.updataArr = [this.currentPageArray[i] , 2]; 									//2为添加
				}
			}
			sendAction(211 , 0 , UnityConstData.playerId);
		}
		/** 点击拒绝按钮*/
		private function refuseHandler(e:MouseEvent):void
		{
			if(GameCommonData.Player.Role.unityJob < 80)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00})  // 你的权限不足
				return;
			}
			sendAction(212 , 0 , UnityConstData.playerId);
		}
		/** 选中按钮条初始化 */
		private function mcselectinit():void
		{
			for(var i:int = 0;i < 14;i++)
			{
				applyUnityList["mcselect_" + i].visible            = false;
				applyUnityList["btnselect_" + i].visible           = false;
				applyUnityList["btnselect_" + i].mouseEnabled      = true;	
				applyUnityList["mcselect_" + i].doubleClickEnabled = true; 
				applyUnityList["txtName_" + i].mouseEnabled        = false;
				applyUnityList["txtLevel_" + i].mouseEnabled       = false;
			}
			for(var n:int ; n < amount ;n++)
			{
				applyUnityList["btnselect_" + n].visible = true;
			}
			 applyUnityList.btnAgree.visible  = false;
			 applyUnityList.btnRefuse.visible = false;
			 
		}
		/** 判断是否为第一页和最后一页，如果是，按钮变黑 */
		private function isbtnenable():void
		{
			if(currentPage == 1)
				return;
			if(currentPage == totalPage)
				return;
		}
		/** 选中某个人物 */
		private function selectedUnity(btn:SimpleButton):void
		{
			var i:int = btn.name.split("_")[1];
			this.countClick = 0;       																	//单双击事件结束
			mcselectinit();																				//选中按钮初始化																
			applyUnityList["mcselect_"+i].visible = true;
			UnityConstData.playerId = idArray[i];														//选中的人物的ID号
			applyUnityList.btnAgree.visible  = true;													//选中人物后按钮才可用
			applyUnityList.btnRefuse.visible = true;
			playName = applyUnityList["txtName_"+i].text;												//把名称存储在playName变量中
			for(var n:int = 0;n < 12;n++)
			{
				applyUnityList["mcselect_"+n].visible == true ? applyUnityList["btnselect_"+n].mouseEnabled = false:applyUnityList["btnselect_"+n].mouseEnabled = true;			//选中的按钮不需要鼠标移上效果
			}
		}
		/** 双击mcselect调用double_Handler()函数*/
		private function mcdouble_Handler(e:MouseEvent):void
		{
			double_Handler();
		} 
		/** 双击查看消息 */
		private function double_Handler():void
		{
			FriendSend.getInstance().getFriendInfo(UnityConstData.playerId , playName);
		}
		/** 得到帮派数据，并排列*/
		private function updateData(arr:Array):void
		{	
			for(var i:int = 0; i < amount ; i++)                                   								 
			{
				applyUnityList["txtName_"+i].text  = arr[i][0];
				applyUnityList["txtLevel_"+i].text = arr[i][1];  
				idArray[i] = arr[i][6];  
			}                		
		
		}
		/** 发送请求*/
		private function sendAction(type:int , page:int , id:int):void
		{
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , type , page , id]
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求
		}
		/** 清空文本框,在关闭面板和更新面板时使用*/
		private function cleatTxt():void
		{
			for(var i:int = 0; i < amount ; i++)                                   								 
			{
				applyUnityList["txtName_"+i].text  = "";
				applyUnityList["txtLevel_"+i].text = "";  
				idArray[i] = 0;
			} 
		}
	}
}