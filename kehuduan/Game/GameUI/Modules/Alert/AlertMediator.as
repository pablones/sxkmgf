package GameUI.Modules.Alert
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.countDown.CountDownEvent;
	import GameUI.View.Components.countDown.CountDownText;
	import GameUI.View.ShowMoney;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.utils.Timer;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	
	public class AlertMediator extends Mediator
	{
		public static const NAME:String = "AlertMediator";
		/**
		 *  
		 */		 
		private var timer:Timer;
		private var info:String = "";
		
		private var _h:int = 0;
		private var _autoSize:int = 2 ;
		private var align:String = "CENTER";
		
		private var title:String = GameCommonData.wordDic["often_used_warning" ];// "警 告";
		private var comfirmTxt:String = GameCommonData.wordDic["often_used_confim"];// "确 认";
		private var cancelTxt:String = GameCommonData.wordDic[ "often_used_cancel" ];// "取 消";
		private var comfirmFn:Function = null;
		private var cancelFn:Function = null;
		private var extendsFn:Function = null;
		private var btnBtn:int = 2;
		private var panelBase:PanelBase = null;
		private var alertView:MovieClip = null;
		private var comFirmPosX:Number = 0;
		private var comFirmPosY:Number = 0;
		private var cancelPosX:Number = 0;
		private var cancelPosY:Number = 0;
		private var backMask:Sprite = null;
		private var params:Object = null;
		private var params_cancel:Object = null;
		private var params_extendsFn:Object = null;
		private var doExtends:uint = 0;	//执行expend方法
		private var isShowClose:Boolean = true;
		private var worldMap:uint = 0;
		private var canDragPanel:Boolean = true;
		private var canOperate:Boolean = false;
		private var countDown:uint = 0;				//倒计时  倒计时结束后确定按钮才可见
		private var countDownText:CountDownText;	//倒计时组件
		
		private var isShow:Boolean = false;
		private var altertQuene:Array = new Array();
		
		private var timeout:int;
		private var timeoutToken:uint;
		
// 第一次进入游戏扩展窗口
		private var viewPanelBase:PanelBase = null;
		private var viewPanel:MovieClip		= null;
		private var viewBack:MovieClip  	= null;
		private var viewBtns:MovieClip  	= null; 
		
		private var comfirmFn_big:Function 	= null;
		private var cancelFn_big:Function 	= null;
		private var extendsFn_big:Function	= null;
		
		private var info_big:String = "";
		private var title_big:String = GameCommonData.wordDic[ "mod_ale_ale_Title_big" ];//"欢迎进入游戏世界"
		private var comfirmTxt_big:String = GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ] ;//"我知道了"
		private var cancelTxt_big:String = GameCommonData.wordDic[ "often_used_cancel" ];//"取 消"
		private var btnBtn_big:int = 2;
		private var comFirmPosX_big:Number = 0;
		private var comFirmPosY_big:Number = 0;
		private var cancelPosX_big:Number = 0;
		private var cancelPosY_big:Number = 0;
		private var params_big:Object = null;
		private var isShowClose_big:Boolean = true;
		private var isShow_Big:Boolean = false;
		private var width_param:uint = 300;
		private var canOp:int = 0;
		private var canDrag:int = 0;
		private var backMask_big:Sprite;
		
//------------------------------------------
		
//新手指导，图片帮助
		private var viewPanelBase_newer:PanelBase   = null;
		private var viewPanel_newer:MovieClip	= null;
		
		private var viewBack_newer:MovieClip  	= null;
		private var viewBtns_newer:MovieClip  	= null; 
		private var viewTask_newer:MovieClip 	= null;
		
		private var comfirmFn_newer:Function 	= null;
		private var cancelFn_newer:Function 	= null;
		private var extendsFn_newer:Function	= null;
		
		private var info_newer:String = "";
		private var title_newer:String =GameCommonData.wordDic[ "often_used_smallTip" ] ;//"小提示"
		private var comfirmTxt_newer:String = GameCommonData.wordDic[ "often_used_confim" ] ;//"确 定"
		private var cancelTxt_newer:String = GameCommonData.wordDic[ "often_used_cancel" ] ;//"取 消"
		private var btnBtn_newer:int = 2;
		private var comFirmPosX_newer:Number = 0;
		private var comFirmPosY_newer:Number = 0;
		private var cancelPosX_newer:Number = 0;
		private var cancelPosY_newer:Number = 0;
		private var params_newer:Object = null;
		private var isShowClose_newer:Boolean = true;
		private var isShow_newer:Boolean = false;
		private var taskStr_newer:String = "";
		private var canOp_newer:int = 0;
		private var backMask_newer:Sprite = null; 
		private var leading:int;
//---------------------------------------------------------------------
		private var dataProxy:DataProxy;
		
//---------------------------------------------------------------------
//美女 
		private static const IMAGE_GIRL_ADDR:String = "Resources/GameDLC/head_notice_1.swf";
//		private static const IMG_POS:Point  = new Point(280, 56);	//图片坐标
		private static const IMG_POS:Point  = new Point( 245, 10 );	//图片坐标
		private static const RECT_POS:Point = new Point(343, 312);	//框坐标
		private var _mcLoader:ImageItem = null;
		private var _imgGirl:Bitmap = null;
		private var _noticeView:MovieClip = null; 
//--------------------------------------------------
		
		
		public function AlertMediator()
		{
			super(NAME);
		}
		
		private function get alert():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
				
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOWALERT,
				EventList.DO_FIRST_TIP,					//第一次进入游戏提示
				NewerHelpEvent.ALERT_IMG_NEWER_HELP,	  	//新手指导系统，图片帮助
				EventList.STAGECHANGE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOWALERT:
					var alertObj:Object = notification.getBody();
					altertQuene.push(alertObj);
					if(!isShow)
					{
						showAlert(notification.getBody());
					}
					changeUI();
					break;
				case EventList.DO_FIRST_TIP:							//第一次进入游戏提示
					if(isShow_Big) {
						panelBigCloseHandler(null);
					}
					var info:Object = notification.getBody();
					showFirstInfo(info);
					changeUI();
					break;
				case NewerHelpEvent.ALERT_IMG_NEWER_HELP:				//新手指导，图片帮助
					if(isShow_newer) {
						panelNewerCloseHandler(null);
					}
					var infoNew:Object = notification.getBody();
					showNewerInfo(infoNew);
					changeUI();
					break;
				case EventList.STAGECHANGE:
					changeUI();
					break;
			}
		}
		
		private function changeUI():void
		{
//			if( GameCommonData.GameInstance.GameUI.stage.stageWidth >= GameConfigData.GameWidth )
//			{
				if( backMask && (GameCommonData.GameInstance.TooltipLayer.contains(backMask) || GameCommonData.GameInstance.GameUI.contains(backMask)) )
				{
					backMask.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				}
				if( panelBase && (GameCommonData.GameInstance.TooltipLayer.contains(panelBase) || GameCommonData.GameInstance.GameUI.contains(panelBase)) )
				{
					panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width) / 2;
				}
				if( backMask_big && GameCommonData.GameInstance.GameUI.contains(backMask_big) )
				{
					backMask_big.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				}
				if( viewBack && GameCommonData.GameInstance.GameUI.contains(viewBack) )
				{
					viewBack.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				}
				if( backMask_newer && GameCommonData.GameInstance.GameUI.contains(backMask_newer) )
				{
					backMask_newer.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				}
				if( _noticeView && GameCommonData.GameInstance.GameUI.contains(_noticeView) )
				{
					_noticeView.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				}
//			}
//			if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
//			{
				if( backMask && (GameCommonData.GameInstance.TooltipLayer.contains(backMask) || GameCommonData.GameInstance.GameUI.contains(backMask)) )
				{
					backMask.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}
				if( panelBase && (GameCommonData.GameInstance.TooltipLayer.contains(panelBase) || GameCommonData.GameInstance.GameUI.contains(panelBase)) )
				{
					panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height) / 2;
				}
				if( backMask_big && GameCommonData.GameInstance.GameUI.contains(backMask_big) )
				{
					backMask_big.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}
				if( viewBack && GameCommonData.GameInstance.GameUI.contains(viewBack) )
				{
					viewBack.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}
				if( backMask_newer && GameCommonData.GameInstance.GameUI.contains(backMask_newer) )
				{
					backMask_newer.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}
				if( _noticeView && GameCommonData.GameInstance.GameUI.contains(_noticeView) )
				{
					_noticeView.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}
//			}
		}
//--------------------------------------------------------------------------------------------------------------------------------------
//--美女
		private function loadGirl():void
		{
			_mcLoader = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + IMAGE_GIRL_ADDR, BulkLoader.TYPE_IMAGE ,"");
			_mcLoader.addEventListener(Event.COMPLETE, onPicComplete);
			_mcLoader.load();
		}
		
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
//			_imgGirl = e.target.content.content;
			_imgGirl = new Bitmap();
			_imgGirl.bitmapData = new ( ( e.target as ImageItem ).GetDefinitionByName( "LovelyGril" ) )( 347,382 ) as BitmapData;
			e.target.destroy();
			e.target.removeEventListener(Event.COMPLETE, onPicComplete);
			addGirlImg();
		}
		
		private function addGirlImg():void
		{
			if(_noticeView) {
				_noticeView.addChildAt(_imgGirl, 0);
				_imgGirl.x = IMG_POS.x;
				_imgGirl.y = IMG_POS.y;
			} if(viewBack) {
				viewBack.addChildAt(_imgGirl, 0);
				_imgGirl.x = IMG_POS.x;
				_imgGirl.y = IMG_POS.y;
			}
		}
//--------------------------------------------------------------------------------------------------------------------------------------
//--新手帮助 图片帮助	
//---------------------------------------------------------------------------------------------------------------------------------------		
		/** 显示图片帮助 */
		private function showNewerInfo(alertObj:Object):void
		{
			if(!alertObj) return;
			if(alertObj.taskStr)					taskStr_newer = alertObj.taskStr;
			if(alertObj.title)						title_newer = alertObj.title;
			if(alertObj.params)						params_newer = alertObj.params;
			if(alertObj.comfirmTxt)					comfirmTxt_newer = alertObj.comfirmTxt;
			if(alertObj.cancelTxt)					cancelTxt_newer = alertObj.cancelTxt;
			if(alertObj.isShowClose != undefined) 	isShowClose_newer = alertObj.isShowClose;	
			if(alertObj.canOp)						canOp_newer = alertObj.canOp;
			
			info_newer = alertObj.info;
			comfirmFn_newer = alertObj.comfrim as Function;
			cancelFn_newer  = alertObj.cancel as Function;
			extendsFn_newer = alertObj.extendsFn as Function;
			initNewerView();
		}
		
		/** 初始化 */
		private function initNewerView():void
		{
			if(canOp_newer == 1) {
				backMask_newer = new Sprite();
				backMask_newer.graphics.beginFill(0xffffff, 0);
				backMask_newer.graphics.drawRect(0,0,GameCommonData.GameInstance.GameUI.width, GameCommonData.GameInstance.GameUI.height);
				backMask_newer.graphics.endFill();
				GameCommonData.GameInstance.GameUI.addChild(backMask_newer);
			}
			
			_noticeView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AlertGirl");
			viewTask_newer = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NewerHelp_Task_"+taskStr_newer);
			viewBtns_newer = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AlertBtnLayer");
			_noticeView.mouseEnabled = false;
			_noticeView.mcPanel.mouseEnabled = false;
			viewTask_newer.mouseEnabled = false;
			viewBtns_newer.txtComfrim.mouseEnabled = false;
			viewBtns_newer.txtCancel.mouseEnabled = false;
			
			var taskX:int = 0;
			var taskY:int = 0;

			if(taskStr_newer == "2") {
				taskX = 20;
				taskY = -3;
			} else if(taskStr_newer == "15_1") {
				taskX = 50;
				taskY = -13;
			} else if(taskStr_newer == "15_2") {
				taskX = 8;
				taskY = -2;
			} else if(taskStr_newer == "16_1") {
				taskX = 8;
				taskY = -2;
			}
			
			var panelWidth:int  = _noticeView.mcPanel.width;
			var panelHeight:int = _noticeView.mcPanel.height;
			
			
			viewBtns_newer.x = (panelWidth  - viewBtns_newer.width) / 2  + _noticeView.mcPanel.x;
			viewBtns_newer.y = _noticeView.mcPanel.y + panelHeight - viewBtns_newer.height - 6;   	//(panelHeight - viewBtns_newer.height) / 2 + _noticeView.mcPanel.y;
			
			viewTask_newer.x = (panelWidth  - viewTask_newer.width) / 2  + _noticeView.mcPanel.x + taskX;
			viewTask_newer.y = (panelHeight - viewTask_newer.height) / 2 + _noticeView.mcPanel.y + taskY;
			
			_noticeView.addChild(viewBtns_newer);
			_noticeView.addChild(viewTask_newer);
			
			comFirmPosX_newer = viewBtns_newer.btnComfrim.x;
			comFirmPosY_newer = viewBtns_newer.btnComfrim.y;
			cancelPosX_newer  = viewBtns_newer.btnCancel.x;
			cancelPosY_newer  = viewBtns_newer.btnCancel.y;
			
			viewBtns_newer.btnComfrim.addEventListener(MouseEvent.CLICK, comFrimNewerHandler);
			
			viewBtns_newer.btnComfrim.x = comFirmPosX_newer;
			viewBtns_newer.btnComfrim.y = comFirmPosY_newer;
			viewBtns_newer.txtComfrim.x = comFirmPosX_newer+2;
			viewBtns_newer.txtComfrim.y = comFirmPosY_newer+2;
			viewBtns_newer.txtComfrim.text = comfirmTxt_newer;
			
			viewBtns_newer.btnCancel.x = cancelPosX_newer;
			viewBtns_newer.btnCancel.y = cancelPosY_newer;
			viewBtns_newer.txtCancel.x = cancelPosX_newer+2;
			viewBtns_newer.txtCancel.y = cancelPosY_newer+2;
			viewBtns_newer.txtCancel.text = cancelTxt_newer;

			if(cancelFn_newer != null)
			{
				viewBtns_newer.btnCancel.addEventListener(MouseEvent.CLICK, cancelNewerHandler);
			}
			else
			{
				viewBtns_newer.btnCancel.visible = false;
				viewBtns_newer.btnComfrim.x = (viewBtns_newer.width - viewBtns_newer.btnComfrim.width)/2;
				viewBtns_newer.txtCancel.visible = false;
				viewBtns_newer.txtComfrim.x = viewBtns_newer.btnComfrim.x+2;
				viewBtns_newer.txtComfrim.y = viewBtns_newer.btnComfrim.y+2;
			}
			
			GameCommonData.GameInstance.GameUI.addChild(_noticeView);
			
			if(_imgGirl) {
				_noticeView.addChildAt(_imgGirl, 0);
				_imgGirl.x = IMG_POS.x;
				_imgGirl.y = IMG_POS.y;
			} else if(_mcLoader == null) {
				loadGirl();
			}
			
			isShow_newer = true;
		}
		
		private function comFrimNewerHandler(e:Event):void
		{
			if(params_newer) {
				comfirmFn_newer(params_newer);
			} else if(comfirmFn_newer != null) {
				comfirmFn_newer();
			}
			panelNewerCloseHandler(null);
		}
		
		private function cancelNewerHandler(event:MouseEvent):void
		{
			cancelFn_newer();
			panelNewerCloseHandler(null);
		}
		
		private function panelNewerCloseHandler(event:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(_noticeView))
			{
				if(extendsFn_newer != null)
				{
					extendsFn_newer();	
				}
				GameCommonData.GameInstance.GameUI.removeChild(_noticeView);
				
				_noticeView = null;
				comfirmFn_newer = null;
				cancelFn_newer = null;
				params_newer = null;
				extendsFn_newer = null;
				isShowClose_newer = true;
				title_newer = GameCommonData.wordDic[ "often_used_smallTip" ];//"小提示"
				comfirmTxt_newer = GameCommonData.wordDic[ "often_used_confim" ];//"确 定"
				cancelTxt_newer = GameCommonData.wordDic[ "often_used_cancel" ];//"取 消"
				isShow_newer = false;
			}
			if(backMask_newer && GameCommonData.GameInstance.GameUI.contains(backMask_newer)) {
				GameCommonData.GameInstance.GameUI.removeChild(backMask_newer);
				backMask_newer = null;
			}
			canOp_newer = 0;
		}
		
//------------------------------------------------------------------------------------------------------------------
//第一次进入游戏

		/** 第一次进入游戏 */
		private function showFirstInfo(alertObj:Object):void
		{
			if(!alertObj) return;
			viewBack = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AlertGirl"); //AlertBackBig
			viewBack.mouseEnabled = false;
			viewBack.mcPanel.mouseEnabled = false;
			viewBtns = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AlertBtnLayer");
			info_big = alertObj.info;
			comfirmFn_big = alertObj.comfrim as Function;
			cancelFn_big = alertObj.cancel as Function;
			extendsFn_big = alertObj.extendsFn as Function;
			if(alertObj.title)						title_big = alertObj.title;
			if(alertObj.params)						params_big = alertObj.params;
			if(alertObj.comfirmTxt)					comfirmTxt_big = alertObj.comfirmTxt;
			if(alertObj.cancelTxt)					cancelTxt_big = alertObj.cancelTxt;
			if(alertObj.isShowClose != undefined) 	isShowClose_big = alertObj.isShowClose;	
			if(alertObj.canOp) 						canOp = alertObj.canOp; 
			if(alertObj.canDrag)					canDrag = alertObj.canDrag;
			if(alertObj.width)						width_param = alertObj.width;
			initFristView();
		}
		
		/** 第一次进入游戏 初始化 */
		private function initFristView():void
		{
			if(canOp == 1) {
				backMask_big = new Sprite();
				backMask_big.graphics.beginFill(0xffffff, 0);
				backMask_big.graphics.drawRect(0,0,GameCommonData.GameInstance.GameUI.width, GameCommonData.GameInstance.GameUI.height);
				backMask_big.graphics.endFill();
				GameCommonData.GameInstance.GameUI.addChild(backMask_big);
			}
			/////////////
//			viewPanel = new MovieClip();
			
			var tf:TaskText = new TaskText(352);  //width_param
			tf.tfText = info_big;
			var tfHeidht:uint = tf.height;
			var tfWidth:uint  = tf.width;
			
//			viewBack.height = tfHeidht + 20;
//			viewBack.width  = tfWidth + 20;
			
//			tf.x = 15;
//			tf.y = 10;
			
			var panelWidth:int  = viewBack.mcPanel.width;
			var panelHeight:int = viewBack.mcPanel.height;
			
			viewBtns.x = (panelWidth  - viewBtns.width) / 2  + viewBack.mcPanel.x;
			viewBtns.y = viewBack.mcPanel.y + panelHeight - viewBtns.height - 6;
			viewBtns.btnComfrim.width = 74;
			viewBtns.txtComfrim.width = 68;
			
			tf.x = (panelWidth  - tfWidth) / 2  + viewBack.mcPanel.x - 4;
			tf.y = (panelHeight - tfHeidht) / 2 + viewBack.mcPanel.y - 3;
			
//			tf.mouseEnabled = false;
			viewBack.addChild(tf);
			viewBack.addChild(viewBtns);
//			viewPanel.addChild(tf);
//			viewBtns.x = (viewBack.width - viewBtns.width) / 2;
//			viewBtns.y = viewBack.height + 3;
			 
//			viewPanelBase = new PanelBase(viewPanel, viewPanel.width+8, viewPanel.height+12);
//			viewPanelBase.name = "firstTaskAlert";
//			viewPanelBase.SetTitleTxt(title_big);
//			viewPanelBase.addEventListener(Event.CLOSE, panelBigCloseHandler);
//			viewPanelBase.x = (GameConfigData.GameWidth - viewPanelBase.width) / 2;
//			viewPanelBase.y = (GameConfigData.GameHeight - viewPanelBase.height) / 2;
			
			viewBtns.txtComfrim.mouseEnabled = false;
			viewBtns.txtCancel.mouseEnabled = false;
			comFirmPosX_big = viewBtns.btnComfrim.x;
			comFirmPosY_big = viewBtns.btnComfrim.y;
			cancelPosX_big  = viewBtns.btnCancel.x;
			cancelPosY_big  = viewBtns.btnCancel.y;
			
			viewBtns.btnComfrim.addEventListener(MouseEvent.CLICK, comFrimBigHandler);
			viewBtns.btnComfrim.x = comFirmPosX_big;
			viewBtns.btnComfrim.y = comFirmPosY_big;
			viewBtns.txtComfrim.x = comFirmPosX_big+2; 
			viewBtns.txtComfrim.y = comFirmPosY_big+2;
			viewBtns.txtComfrim.text = comfirmTxt_big;
			viewBtns.btnCancel.x = cancelPosX_big;
			viewBtns.btnCancel.y = cancelPosY_big;
			viewBtns.txtCancel.x = cancelPosX_big+2;
			viewBtns.txtCancel.y = cancelPosY_big+2;
			viewBtns.txtCancel.text = cancelTxt_big;

			if(cancelFn_big != null)
			{
				viewBtns.btnCancel.addEventListener(MouseEvent.CLICK, cancelBigHandler);
			}
			else
			{
				viewBtns.btnCancel.visible = false;
				viewBtns.btnComfrim.x = (viewBtns.width - viewBtns.btnComfrim.width)/2;
				viewBtns.txtCancel.visible = false;
				viewBtns.txtComfrim.x = viewBtns.btnComfrim.x+3;
				viewBtns.txtComfrim.y = viewBtns.btnComfrim.y+2;
			}
//			if(isShowClose == false)
//			{
//				viewPanelBase.disableClose();
//			}
            viewBack.name = "viewBack";
			GameCommonData.GameInstance.GameUI.addChild(viewBack);
			isShow_Big = true;
//			if(canDrag == 1) {
//				viewPanelBase.IsDrag = false;
//			}
			
			if(_imgGirl) {
				viewBack.addChildAt(_imgGirl, 0);
				_imgGirl.x = IMG_POS.x;
				_imgGirl.y = IMG_POS.y;
			} else if(_mcLoader == null) {
				loadGirl();
			}
		}
		
		private function comFrimBigHandler(e:Event):void
		{
			if(params_big) {
				comfirmFn_big(params_big);
			} else if(comfirmFn_big != null) {
				comfirmFn_big();
			}
			panelBigCloseHandler(null);
		}
		
		private function cancelBigHandler(event:MouseEvent):void
		{
			cancelFn_big();
			panelBigCloseHandler(null);
		}
		
		private function panelBigCloseHandler(event:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(viewBack))
			{
				if(extendsFn_big != null)
				{
					extendsFn_big();
					extendsFn_big = null;	
				}
//				viewPanelBase.removeEventListener(Event.CLOSE, comFrimBigHandler);
				GameCommonData.GameInstance.GameUI.removeChild(viewBack);
				viewBack = null;
//				viewPanelBase = null;
				comfirmFn_big = null;
				cancelFn_big = null;
				params_big = null;
				extendsFn_big = null;
				isShowClose_big = true;
				title_big = GameCommonData.wordDic[ "mod_ale_ale_panelBigCloseHandler_Title_big" ];//欢迎进入游戏世界
				comfirmTxt_big = GameCommonData.wordDic[ "often_used_confim" ];//"确 定"
				cancelTxt_big = GameCommonData.wordDic[ "often_used_cancel" ] ;//"取 消"
				isShow_Big = false;
				width_param = 300;
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 18);	// 通知新手指导
			}
			if(backMask_big && GameCommonData.GameInstance.GameUI.contains(backMask_big)) {
				GameCommonData.GameInstance.GameUI.removeChild(backMask_big);
				backMask_big = null;
			}
			canOp = 0;
			canDrag = 0;
		}
//===========================================================================================================================================
		
		private function showAlert(alertObj:Object):void
		{
			if(!alertObj) return;
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			isShow = true;
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.ALERTTEXTVIEW});
			info = alertObj.info;
			comfirmFn = alertObj.comfrim as Function;
			cancelFn = alertObj.cancel as Function;
			extendsFn = alertObj.extendsFn as Function;
			if(alertObj.title) 						title = alertObj.title;
			if(alertObj.params)						params = alertObj.params;
			if(alertObj.comfirmTxt)					comfirmTxt = alertObj.comfirmTxt;
			if(alertObj.cancelTxt)					cancelTxt = alertObj.cancelTxt;
			if(alertObj.isShowClose != undefined)	isShowClose = alertObj.isShowClose;	
			if(alertObj.doExtends) 					doExtends = alertObj.doExtends; 
			if(alertObj.params_cancel)				params_cancel = alertObj.params_cancel;
			if(alertObj.params_extendsFn)			params_extendsFn = alertObj.params_extendsFn;
			if(alertObj.worldMap)					worldMap = alertObj.worldMap;
			if(alertObj.canDrag) 					canDragPanel  = alertObj.canDrag;
			if(alertObj.countDown) 					countDown  = alertObj.countDown;
			if(alertObj.canOperate) 				canOperate = alertObj.canOperate;
			if(alertObj.leading) 				leading = alertObj.leading;
			if(alertObj.timeout)				timeout = alertObj.timeout;
			
			if( alertObj.height )
			{
				_h = alertObj.height;
			}else
			{
				_h = 59;
			}
			
			if( alertObj.autoSize )	
			{
				_autoSize = int( alertObj.autoSize);
			}else
			{
				_autoSize = 2;
			}
			UIConstData.KeyBoardCanUse = false;		// 禁用快捷键
			initView();
			if (timeout)
			{
				timeoutToken = setTimeout(function():void
				{
					if (panelBase)
					{
						panelCloseHandler(null);
					}
				}, timeout);
			}
		}
		
		private function initView():void
		{
			if(canOperate == false) {
				backMask = new Sprite();
				backMask.graphics.beginFill(0xffffff, 0);
				backMask.graphics.drawRect(0,0,GameCommonData.GameInstance.GameUI.width, GameCommonData.GameInstance.GameUI.height);
				backMask.graphics.endFill();
				if(worldMap == 1) {
					GameCommonData.GameInstance.TooltipLayer.addChild(backMask); 
				} else {
					GameCommonData.GameInstance.GameUI.addChild(backMask);
				}
			}
			
			if( _h !== 0 )
			{
				alert.AlertBackground.height = _h;
				alert.btnComfrim.y = _h + 4 - 1;
				alert.btnCancel.y = _h + 4 - 1;
				alert.txtComfrim.y = _h + 6 - 1;
				alert.txtCancel.y = _h + 6 - 1;
				alert.mcTextBack.txtInfo.height =  _h - 10;	
			}
			
			( alert.txtComfrim as TextField ).mouseEnabled = false;
			( alert.txtCancel as TextField ).mouseEnabled = false;
			
			if( _h > 59 )
			{
				var sp:Sprite = new Sprite();
					sp.graphics.lineStyle( 1, 0x4D3C13, 1);
					sp.graphics.beginFill( 0x000000 );
					sp.graphics.drawRect( 0, 0, 253, _h );
					sp.graphics.endFill();
				var tip1:MovieClip = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "tip1" );
					tip1.x = 0;
					tip1.y = 0;
					sp.addChild( tip1 );
				var tip2:MovieClip = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "tip2" );
					tip2.x = 253 - 11 + 1;
					tip2.y = 0;
					sp.addChild( tip2 );
				var tip3:MovieClip = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "tip3" );
					tip3.x = 0;
					tip3.y = _h - 12 + 0.5;
					sp.addChild( tip3 );
				var tip4:MovieClip = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "tip4" );
					tip4.x = 253 -11 + 1;
					tip4.y = _h - 12 + 0.5;
					sp.addChild( tip4 );
					alert.addChildAt( (alert.AlertBackground as MovieClip ), 0 );
					(alert.mcTextBack.txtInfo as TextField).x = 6;
					alert.removeChildAt( 0 );
					alert.addChildAt( sp , 1);
					sp.x = 0.4;
			}
			
				switch( _autoSize )
				{
					case 1:
						align = "LEFT";
					break;
					case 2:
						align = "CENTER";
					break;
					case 3:
						align = "RIGHT";
					break;
					default:
					break;
				}
			
			panelBase = new PanelBase(alert, alert.width+8, alert.height+16);
			panelBase.name = "Alert";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.SetTitleTxt(title);								
			panelBase.x = (GameConfigData.GameWidth - panelBase.width) / 2;
			panelBase.y = (GameConfigData.GameHeight - panelBase.height) / 2;
			if(worldMap == 1) {
				GameCommonData.GameInstance.TooltipLayer.addChild(panelBase);
			} else {
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
			}
			alert.mcTextBack.txtInfo.wordWrap = true;
			alert.mcTextBack.txtInfo.htmlText =	"<P ALIGN = " + "'" + align + "'" + ">" + info + "</P>";
			if ( leading > 0 )
			{
				var leadingFormat:TextFormat = new TextFormat();
				leadingFormat.leading = leading;
				alert.mcTextBack.txtInfo.setTextFormat( leadingFormat );
			}
			ShowMoney.ShowIcon(alert.mcTextBack, alert.mcTextBack.txtInfo, true);
			alert.txtComfrim.mouseEnabled = false;
			alert.txtCancel.mouseEnabled = false;
			comFirmPosX = alert.btnComfrim.x;
			comFirmPosY = alert.btnComfrim.y;
			cancelPosX  = alert.btnCancel.x;
			cancelPosY = alert.btnCancel.y;
			if(comfirmFn != null)
			{
				alert.btnComfrim.addEventListener(MouseEvent.CLICK, comFrimHandler);
				alert.btnComfrim.x = comFirmPosX;
				alert.btnComfrim.y = comFirmPosY;
				alert.txtComfrim.x = comFirmPosX+2;
				alert.txtComfrim.y = comFirmPosY+2;
				alert.txtComfrim.text = comfirmTxt;
				alert.btnCancel.x = cancelPosX;
				alert.btnCancel.y = cancelPosY;
				alert.txtCancel.x = cancelPosX+2;
				alert.txtCancel.y = cancelPosY+2;
				alert.txtCancel.text = cancelTxt;
			}
			if(cancelFn != null)
			{
				alert.btnCancel.addEventListener(MouseEvent.CLICK, cancelHandler);
			}
			else
			{
				alert.btnCancel.visible = false;
				alert.btnComfrim.x = (alert.width - alert.btnComfrim.width)/2;
				alert.txtCancel.visible = false;
				alert.txtComfrim.x = alert.btnComfrim.x+2;
				alert.txtComfrim.y = alert.btnComfrim.y+2;
			}
			alert.mcUseable.x = alert.btnComfrim.x;		//灰色按钮MC
			alert.mcUseable.y = alert.btnComfrim.y;
			if(countDown > 0) {			//如果是倒计时
				initCountDown();
			}
			if(isShowClose == false)
			{
				panelBase.disableClose();
			}
			if(canDragPanel == false) {
				panelBase.IsDrag = false;
			}
		}
		
		/** 初始化倒计时组件 */
		private function initCountDown():void
		{
			countDownText = new CountDownText(5);
			countDownText.x = alert.btnComfrim.x + 20;
			countDownText.y = alert.btnComfrim.y + 3;
			countDownText.addEventListener(CountDownEvent.TIME_OVER, countOverHandler);
			alert.addChild(countDownText);
			alert.btnComfrim.visible = false;
			alert.txtComfrim.visible = false;
			countDownText.start();
		}
		
		/** 倒计时结束 */
		private function countOverHandler(e:CountDownEvent):void
		{
			alert.btnComfrim.visible = true;
			alert.txtComfrim.visible = true;
			clearCountDown();
		}
		
		private function clearCountDown():void
		{
			if(countDownText) {			//移除倒计时组件
				countDownText.removeEventListener(CountDownEvent.TIME_OVER, countOverHandler);
				alert.removeChild(countDownText);
				countDownText.dispose();
				countDownText = null;
			}
		}
		
		private function timerHandler(event:TimerEvent):void
		{
			timer.removeEventListener(TimerEvent.TIMER, timerHandler);
			timer = null;
			sendNotification( NewerHelpEvent.SHOW_NEWER_HELP, 19 );
		}
		
		private function comFrimHandler(event:MouseEvent):void
		{
			if(params) {
				comfirmFn(params);
				try
				{
					if( params.mapId && params.tileX && params.tileY && params.taskId && params.npcId )
					{
						var obj:Object = NewerHelpData.changeId( 1002, -6 );
						if( params.mapId == obj._mapId && params.tileX == 86 && params.tileY == 98 && params.taskId == 306 )
						{
							timer = new Timer(500, 1);
							timer.addEventListener(TimerEvent.TIMER, timerHandler);
							timer.start();
						}
					}
				}
				catch ( e:Error )
				{
					
				}
			}
			else {
				comfirmFn();
			}
			if(extendsFn != null && doExtends == 0) {
				if(params_extendsFn) {
					extendsFn(params_extendsFn);
				} else {
					extendsFn();
				}
				extendsFn = null;
			} else {
				extendsFn = null;
			}
			panelCloseHandler(null);
		}
		
		private function cancelHandler(event:MouseEvent):void
		{
			if(params_cancel) {
				cancelFn(params_cancel);
			} else {
				cancelFn();
			}
			if(extendsFn != null && doExtends == 0)  {
				if(params_extendsFn) {
					extendsFn(params_extendsFn);
				} else {
					extendsFn();
				}
				extendsFn = null;
			} else {
				extendsFn = null;
			}
			panelCloseHandler(null);
		}
			
		private function panelCloseHandler(event:Event):void
		{
			clearTimeout(timeoutToken);
			if(countDownText) {			//清除倒计时组件
				clearCountDown();
			}
			if(worldMap == 1) {
				if(GameCommonData.GameInstance.TooltipLayer.contains(panelBase)) {
					if(extendsFn != null) 
					{
						if(params_extendsFn) {
							extendsFn(params_extendsFn);
						} else {
							extendsFn();
						}
						extendsFn = null;
					}
					isShow = false;
					panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
					GameCommonData.GameInstance.TooltipLayer.removeChild(panelBase);
					if(backMask && GameCommonData.GameInstance.TooltipLayer.contains(backMask)) {
						GameCommonData.GameInstance.TooltipLayer.removeChild(backMask);
					}
					altertQuene.shift();
					backMask = null;
					panelBase = null;
					comfirmFn = null;
					cancelFn = null;
					params = null;
					params_cancel = null;
					params_extendsFn = null;
					extendsFn = null;
					isShowClose = true;
					title = GameCommonData.wordDic[ "often_used_warning" ];//"警 告"
					comfirmTxt = GameCommonData.wordDic[ "often_used_confim" ];//"确 认"
					cancelTxt = GameCommonData.wordDic[ "often_used_cancel" ];//"取 消"
					this.viewComponent = null;
					doExtends = 0;
					worldMap = 0;
					countDown = 0;
					canDragPanel = true;
					canOperate = false;
				}
			}
			else if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				if(extendsFn != null) 
				{
					if(params_extendsFn) {
						extendsFn(params_extendsFn);
					} else {
						extendsFn();
					}
					extendsFn = null;
				}
				isShow = false;
				panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				if(backMask && GameCommonData.GameInstance.GameUI.contains(backMask)) {
					GameCommonData.GameInstance.GameUI.removeChild(backMask);
				}
				altertQuene.shift();
				backMask = null;
				panelBase = null;
				comfirmFn = null;
				cancelFn = null;
				params = null;
				params_cancel = null;
				params_extendsFn = null;
				extendsFn = null;
				isShowClose = true;
				title = GameCommonData.wordDic[ "often_used_warning" ];//"警 告"
				comfirmTxt = GameCommonData.wordDic[ "often_used_confim" ];//"确 认"
				cancelTxt = GameCommonData.wordDic[ "often_used_cancel" ];//"取 消"
				this.viewComponent = null;
				doExtends = 0;
				worldMap = 0;
				countDown = 0;
				canDragPanel = true;
				canOperate = false;
			}
			if(altertQuene.length > 0)
			{
				showAlert(altertQuene[0]);
				return;
			}
			if(dataProxy.TradeIsOpen || StallConstData.stallSelfId > 0) {	//交易或摆摊中
				return;
			} else {
				UIConstData.KeyBoardCanUse = true;		// 可用快捷键
			}
		}
		
	}
}