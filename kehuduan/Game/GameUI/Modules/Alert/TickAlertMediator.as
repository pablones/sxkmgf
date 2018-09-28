package GameUI.Modules.Alert
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	//有打勾的提示框
	public class TickAlertMediator extends Mediator
	{
		public static const NAME:String = "TickAlertMediator"; 

		private var info:String = "";
		private var title:String = GameCommonData.wordDic[ "often_used_warning" ];
		private var comfirmTxt:String = GameCommonData.wordDic[ "often_used_confim" ];
		private var cancelTxt:String = GameCommonData.wordDic[ "often_used_cancel" ];
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
		
		private var isShow:Boolean = false;
		private var altertQuene:Array = new Array();
		
		private var remember_txt:TextField;
		private var tick_mc:MovieClip;
		
		private var clickTickFn:Function;
		
		private var dataProxy:DataProxy;
				
		public function TickAlertMediator()
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
				EventList.SHOW_TICK_ALERT,
				EventList.STAGECHANGE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOW_TICK_ALERT:
					var alertObj:Object = notification.getBody();
					altertQuene.push(alertObj);
					if(!isShow)
					{
						showAlert( notification.getBody() );
					}
					break;
				case EventList.STAGECHANGE:
					if( panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase) )
					{
						panelBase.x = (GameCommonData.GameInstance.MainStage.stageWidth - panelBase.width) / 2;
						panelBase.y = (GameCommonData.GameInstance.MainStage.stageHeight - panelBase.height) / 2;
					}
					break;
			}
		}
		
		private function showAlert(alertObj:Object):void
		{
			if(!alertObj) return;
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			isShow = true;
			facade.sendNotification(EventList.GETRESOURCE, { type:UIConfigData.MOVIECLIP, mediator:this, name:"TickAlertUI" } );
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
			if(alertObj.canOperate) 				canOperate = alertObj.canOperate;
			if ( alertObj.clickTickFn )              clickTickFn = alertObj.clickTickFn;
			UIConstData.KeyBoardCanUse = false;		// 禁用快捷键
			initView();
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
			panelBase = new PanelBase(alert, alert.width+8, alert.height+16);
			panelBase.name = "TickAlert";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.SetTitleTxt(title);								
			panelBase.x = (GameCommonData.GameInstance.MainStage.stageWidth - panelBase.width) / 2;
			panelBase.y = (GameCommonData.GameInstance.MainStage.stageHeight - panelBase.height) / 2;
			if(worldMap == 1) {
				GameCommonData.GameInstance.TooltipLayer.addChild(panelBase);
			} else {
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
			}
			alert.mcTextBack.txtInfo.htmlText = info;
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

			if(isShowClose == false)
			{
				panelBase.disableClose();
			}
			if(canDragPanel == false) {
				panelBase.IsDrag = false;
			}
			
			initBottom();
//			( alert.tickArrow_mc ).buttonMode = true;
//			( alert.tickArrow_mc ).addEventListener( MouseEvent.CLICK, clickArrowHandler ); 
		}
		
		private function initBottom():void
		{
			if ( !remember_txt )
			{
				remember_txt = new TextField;
				remember_txt.x = 98;
				remember_txt.y = 43;
				remember_txt.mouseEnabled = false;
				alert.addChildAt( remember_txt,alert.numChildren-1 );
				remember_txt.htmlText = "<font color = '#ffffff'>" + GameCommonData.wordDic[ "mod_ale_tick_initb_1" ] + "</font>";
			}
			if ( !tick_mc )
			{
				tick_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillArrow");
				tick_mc.x = 75;
				tick_mc.y = 45;
				alert.addChildAt( tick_mc,alert.numChildren-1 );
				tick_mc.gotoAndStop( 1 );
				tick_mc.buttonMode = true;
			}
			tick_mc.addEventListener( MouseEvent.CLICK, clickArrowHandler ); 
		}
		
		private function clickArrowHandler( evt:MouseEvent ):void
		{
			if ( ( evt.target ).currentFrame == 1 )
			{
				( evt.target ).gotoAndStop( 2 );
//				if ( clickTickFn != null )
//				{
//					clickTickFn( true );
//				}
			}
			else
			{
				( evt.target ).gotoAndStop( 1 );
//				clickTickFn( false ); 
			}
		}
		
		private function comFrimHandler(event:MouseEvent):void
		{
			if(params) {
				comfirmFn(params);
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
			
			if ( tick_mc && clickTickFn != null )
			{
				if ( tick_mc.currentFrame == 1 )
				{
					clickTickFn( false );
				}
				else
				{
					clickTickFn( true );
				}
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
					title = GameCommonData.wordDic[ "often_used_warning" ];
					comfirmTxt = GameCommonData.wordDic[ "often_used_confim" ];
					cancelTxt = GameCommonData.wordDic[ "often_used_cancel" ];
					this.viewComponent = null;
					doExtends = 0;
					worldMap = 0;
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
				title = GameCommonData.wordDic[ "often_used_warning" ];
				comfirmTxt = GameCommonData.wordDic[ "often_used_confim" ];
				cancelTxt = GameCommonData.wordDic[ "often_used_cancel" ];
				this.viewComponent = null;
				doExtends = 0;
				worldMap = 0;
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
			tick_mc.removeEventListener( MouseEvent.CLICK, clickArrowHandler ); 
			remember_txt = null;
			tick_mc = null;
		}
		
	}
}