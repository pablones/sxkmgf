package GameUI.Modules.GmTools.Mediator
{
	import Controller.TerraceController;
	
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.GmTools.Data.GmToolsEvent;
	import GameUI.Modules.GmTools.Data.GmtoolsData;
	import GameUI.Modules.GmTools.Utils.LoadSwfTool;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import OopsFramework.Net.*;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.utils.clearInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class GmToolsMediator extends Mediator
	{
		public static const NAME:String = "GmToolsMediator";
		
		private var loadswfTool:LoadSwfTool;
		private var gmToolsView:MovieClip;
		private var panelBase:PanelBase;
		private var iScrollPane:UIScrollPane;
		private var intervalId:uint;
		private var scrollList:Sprite;
		public function GmToolsMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
						GmToolsEvent.LOADGMTOOLSVIEW,
						GmToolsEvent.SHOWGMTOOLSVIEW,
						GmToolsEvent.CLOSEGMTOOLSVIEW
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case GmToolsEvent.LOADGMTOOLSVIEW:
					if(gmToolsView == null) 
					{
						loadswfTool = new LoadSwfTool(GameConfigData.GmToolsSWF , this);
						loadswfTool.sendShow = sendShow;
					}
					else facade.sendNotification(GmToolsEvent.SHOWGMTOOLSVIEW);
				break;
				
				case GmToolsEvent.SHOWGMTOOLSVIEW:
					if(gmToolsView == null)  gmToolsView = notification.getBody() as MovieClip;
					if(GmtoolsData.gmToolsViewIsOpen == false)
					{
						GmtoolsData.gmToolsViewIsOpen = true;
						showView();
					}
					else gcAll();
				break;
				
				case GmToolsEvent.CLOSEGMTOOLSVIEW:
					gcAll();
				break;
				
			}
		}
		/** 加载swf成功后发送事件 */
		private function sendShow(view:DisplayObject):void
		{
			facade.sendNotification(GmToolsEvent.SHOWGMTOOLSVIEW , view);
		}
		private function showView():void
		{
			initView();
			addLis();
			gmToolsView.showSelfView(this);
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			
		}
		private function initView():void
		{
			panelBase = new PanelBase(gmToolsView, gmToolsView.width - 3, gmToolsView.height + 12);
			panelBase.name = "GmToolsMediator";
			panelBase.x = UIConstData.DefaultPos2.x - 180;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_gmt_med_gmt_ini" ]+"GM");//联系
		}
		private function addLis():void
		{
			 UIUtils.addFocusLis(gmToolsView.txtContent);
			 UIUtils.addFocusLis(gmToolsView.txtTilt);
			 panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			 gmToolsView.txtContent.addEventListener(FocusEvent.FOCUS_IN , contentFocusIn);
			 gmToolsView.txtContent.addEventListener(FocusEvent.FOCUS_OUT , contentFocusOut);
			 gmToolsView.btnClose.addEventListener(MouseEvent.CLICK , closeClickHandler);
			 gmToolsView.btnSubmit.addEventListener(MouseEvent.CLICK , submitClickHandler);
		}
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		public function gcAll():void
		{
			GmtoolsData.gmToolsViewIsOpen = false;
			if(gmToolsView)  gmToolsView.gcAll();
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			gmToolsView.txtContent.removeEventListener(FocusEvent.FOCUS_IN , contentFocusIn);
			gmToolsView.txtContent.removeEventListener(FocusEvent.FOCUS_OUT , contentFocusOut);
			gmToolsView.btnClose.removeEventListener(MouseEvent.CLICK , closeClickHandler);
			gmToolsView.btnSubmit.removeEventListener(MouseEvent.CLICK , submitClickHandler);
		}
		public function postData(index:int , title:String , content:String):void
		{
			if(title == "") 	
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_gmt_med_gmt_gcA_1" ], color:0xffff00});//"标题不能为空"
				return;
			}
			else if(content == "")
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_gmt_med_gmt_gcA_2" ], color:0xffff00});//"内容不能为空"
				return;
			}
			else if(GmtoolsData.isPosting == true)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_gmt_med_gmt_gcA_3" ], color:0xffff00});//"数据提交中"
				return;
			} 
			var date:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
			var nowTime:uint = date.getTime();
			if((nowTime - GmtoolsData.submitTime) < 1000 * 60 * 10)			//两次操作没超过10分钟
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_gmt_med_gmt_gcA_4" ], color:0xffff00});//"两次操作时间需要超过10分钟"
				date = null;
				return;
			}
			date = null;
			var baseHttp:BaseHttp = new BaseHttp(GameConfigData.GmPhpPath);
			baseHttp.RequestComplete = requestComplete;
			baseHttp.AddUrlVariables("username"	, GameCommonData.Player.Role.Name);
			baseHttp.AddUrlVariables("type" 	, index);
			baseHttp.AddUrlVariables("title" 	, title);
			baseHttp.AddUrlVariables("content" 	, content);
			baseHttp.Submit();
			GmtoolsData.isPosting = true;			//提交数据中
			intervalId = setTimeout(timeOut , 1000 * 40);		//40秒以后操作超时
		}
		
		/** 提交表单后返回的数据*/
		private function requestComplete(data:String):void
		{
			if(data == "OK")
			{
				var date:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
				GmtoolsData.submitTime = date.getTime();
				clearInterval(intervalId);    		//清除操作超时的计时器
				GmtoolsData.isPosting = false;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_gmt_med_gmt_req" ], color:0xffff00});//"提交成功"
				facade.sendNotification(GmToolsEvent.CLOSEGMTOOLSVIEW);
				date = null;
			}
		}
		/** 提交超时，请再次操作 */
		private function timeOut():void
		{
			GmtoolsData.isPosting = false;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_gmt_med_gmt_tim" ], color:0xffff00});//"操作超时，请重新提交"
		}
		/** 充值 */
		public function payMoney():void
		{
			facade.sendNotification(TerraceController.NAME , "pay");
		}
		 /** 点击取消按钮 */
		private function closeClickHandler(e:MouseEvent):void
		{
			this.gcAll();
		}
		/** 点击提交按钮 */
		private function submitClickHandler(e:MouseEvent):void
		{
			this.postData(gmToolsView.selectType , gmToolsView.txtTilt.text , gmToolsView.txtContent.text);
		}
		private function contentFocusIn(e:FocusEvent):void
		{
			UIConstData.FocusIsUsing = true;
		}
		private function contentFocusOut(e:FocusEvent):void
		{
			UIConstData.FocusIsUsing = false;
		}
	}
}