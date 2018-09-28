package GameUI.Modules.VipHeadIcon.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.VipHeadIcon.data.VipHeadIconData;
	import GameUI.Modules.VipHeadIcon.view.ui.HeadContainer;
	import GameUI.Modules.VipHeadIcon.view.ui.VhiLoadTool;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 * vip头像选择
	 * */
	public class VipHeadIconMediator extends Mediator
	{
		public static const NAME:String = "VipHeadIconMediator"; 
		private var vhiLoader:VhiLoadTool;//swf文件加载类
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase;
		private var _gridClass:Class;//头像小框的类
		public var dataNum:int = -1;//选择的头像的id号
		private var headContainer:HeadContainer;//放置九个方格的容器 
		private var itemId:int = 630021;//美容卷type号
		private var sexId:int = 0;//当前是男头像还是女头像，1为男，2为女
		private var currentMalePage:int = 1;//当前的男性头像显示页
		private var currentFemalePage:int = 1;//当前的男性头像显示页
		private var maxPage:int = 1;
		private var currentUsePage:int = 1;
		
		public function VipHeadIconMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME);
		}
		public function get vipHeadIconView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		public override function listNotificationInterests():Array
		{
			return [
				VipHeadIconData.LOADINT_VIPHEADICON_VIEW				
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case VipHeadIconData.LOADINT_VIPHEADICON_VIEW://初始化并弹出面板
					if(!dataProxy)
					{
						dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					}
					if(!dataProxy.vipHeadIconPanelIsOpen)
					{
						if(!vipHeadIconView)
						{
							vhiLoader = new VhiLoadTool(GameConfigData.VipHeadIconSWF);
							vhiLoader.sendShow = sendShowView;
						}
						else{
							initView();
						}
					}
					else{
						gc();
					}
					break;
				default:
					break;
			}
		}
		
		private function sendShowView(view:MovieClip,gridClass:Class):void
		{
			this.setViewComponent(view);
			_gridClass = gridClass;
			panelBase = new PanelBase(vipHeadIconView,vipHeadIconView.width + 8,vipHeadIconView.height-17);
			panelBase.name = NAME;
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos1.x+200;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_1" ]);                      //"头 像"
			initView();
		}
		/**初始化面板*/
		private function initView():void
		{
			dataProxy.vipHeadIconPanelIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			addEventListeners();
			dataNum = -1;
			(vipHeadIconView.txt_sex as TextField).mouseEnabled = false;
			showPage();
			sexId = GameCommonData.Player.Role.Sex;
			if(sexId == 0)
			{
				(vipHeadIconView.txt_sex as TextField).text = GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_2" ];          //"男性头像"
			}
			else{
				(vipHeadIconView.txt_sex as TextField).text = GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_3" ];      //"女性头像"
			}
			(vipHeadIconView.txt_explain as TextField).htmlText = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_4" ]+"[<font color='#2652BC'>"+GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_5" ]+"</font>]1"+GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_6" ] + 
																							//"每次更换头像会扣除"                                                                 "易容丹"                                                               "个，"
					GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_7" ]+"7"+GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_8" ]+"</font>";
					//"头像使用期限为"        "天，多次购买同个头像使用时间累加"
			(vipHeadIconView.txt_explain as TextField).selectable	= false;
			initGrid(sexId,1);
			
		}
		/**初始化格子*/
		private function initGrid(tempSexId:int = 0,tempPageIndex:int = 1):void
		{
			if(headContainer)
			{
				headContainer.gc();
				headContainer = null;
			}
			headContainer = new HeadContainer(this,_gridClass);
			headContainer.initGrid(tempSexId,tempPageIndex);
			headContainer.x = 45;
			headContainer.y = 72;
			vipHeadIconView.addChild(headContainer);
		}
		private function addEventListeners():void
		{
			
			vipHeadIconView.btn_close.addEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_sure.addEventListener(MouseEvent.CLICK,btnClickHandler);
			
			vipHeadIconView.btn_leftSexChange.addEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_rightSexChange.addEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_leftPageChange.addEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_rightPageChange.addEventListener(MouseEvent.CLICK,btnClickHandler);
		}
		
		private function removeEventListeners():void
		{
			
			vipHeadIconView.btn_close.removeEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_sure.removeEventListener(MouseEvent.CLICK,btnClickHandler);
			
			vipHeadIconView.btn_leftSexChange.removeEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_rightSexChange.removeEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_leftPageChange.removeEventListener(MouseEvent.CLICK,btnClickHandler);
			vipHeadIconView.btn_rightPageChange.removeEventListener(MouseEvent.CLICK,btnClickHandler);
		}
		private function btnClickHandler(me:MouseEvent):void
		{
			switch((me.target as DisplayObject).name)
			{
				case "btn_sure":
					isSendMessage();
					break;
				case "btn_close":
					gc();
					break;
				case "btn_leftSexChange":
					changeSex();
					break;
				case "btn_rightSexChange":
					changeSex();
					break;
				case "btn_leftPageChange":
					changePage("left");
					break;
				case "btn_rightPageChange":
					changePage("right");
					break;
			}
		}
		/**更换头像性别*/
		private function changeSex():void
		{
			dataNum = -1;
			if(0 == sexId)
			{
				sexId = 1;
				initGrid(sexId,currentFemalePage);
				showPage(currentFemalePage);
				(vipHeadIconView.txt_sex as TextField).text = GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_3" ];            //"女性头像"
			}
			else{
				sexId = 0;
				initGrid(sexId,currentMalePage);
				showPage(currentMalePage);
				(vipHeadIconView.txt_sex as TextField).text = GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_2" ];          //"男性头像"
			}
		}
		/**更换当前的性别的头像页*/
		private function changePage(taget:String):void
		{
			dataNum = -1;
			if(sexId == 0)
			{
				if(taget == "left")
				{
					if(currentMalePage == 1)
					{
						currentMalePage = maxPage;
					} 
					else {
						currentMalePage --;
					}
				}
				if(taget == "right")
				{
					if(currentMalePage == maxPage)
					{
						currentMalePage = 1;
					}
					else{
						currentMalePage ++;
					}
				}
				showPage(currentMalePage);
				initGrid(sexId,currentMalePage);
			}
			else if(sexId == 1)
			{
				if(taget == "left")
				{
					if(currentFemalePage == 1)
					{
						currentFemalePage = maxPage;
					} 
					else {
						currentFemalePage --;
					}
				}
				if(taget == "right")
				{
					if(currentFemalePage == maxPage)
					{
						currentFemalePage = 1;
					}
					else{
						currentFemalePage ++;
					}
				}	
				initGrid(sexId,currentFemalePage);
				showPage(currentFemalePage);
			}
		}
		/**页面显示文本框*/
		private function showPage(currentPage:int = 1):void
		{
			(vipHeadIconView.txt_page as TextField).text = currentPage+"/"+maxPage;
		}
		private function isSendMessage():void
		{
			if(dataNum < 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_9" ], color:0xffff00});        //"您还没有选择头像，请选择头像"
				return;
			}
			  if(BagData.isHasItem(itemId))
			 {
				if(GameCommonData.Player.Role.Sex == sexId)
				{
					facade.sendNotification(EventList.SHOWALERT, {comfrim:comfirmUse, cancel:canceUse, info:GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ism_1" ]+"[<font color='#2652BC'>"+GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ssv_5" ]+"</font>]"+GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ism_2" ],title:GameCommonData.wordDic[ "often_used_tip" ]});
																										//"确定更换？本次操作将扣除"                                                                 "易容丹"                                                              "一个？"        "提 示"                        
				}
				else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ism_3" ], color:0xffff00});      //"头像与您当前的性别不符"
				}
			}
			else{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_viphi_view_med_vhim_ism_4" ], color:0xffff00});  //"您的背包中，没有易容丹，请先购买易容丹"
			}   
		}
		private function comfirmUse():void
		{
			var palyerId:int = GameCommonData.Player.Role.Id;
			var handId:int = this.dataNum;
			
			var obj:Object={type:1010};
					obj.data = [0,palyerId,0,0,handId,0,35,0,0];
					PlayerActionSend.PlayerAction(obj);
		}
		private function canceUse():void
		{
		}
		private function panelCloseHandler(e:Event):void
		{
			gc();
		}
			
		private function gc():void
		{
			dataProxy.vipHeadIconPanelIsOpen = false;
			removeEventListeners();
			vhiLoader = null;
			vipHeadIconView.removeChild(headContainer);
			headContainer.gc();
			headContainer = null;
			this.setViewComponent(null);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
		}
	}
}