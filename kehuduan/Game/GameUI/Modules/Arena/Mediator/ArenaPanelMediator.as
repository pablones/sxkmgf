package GameUI.Modules.Arena.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Arena.Command.ArenaPanelCommandList;
	import GameUI.Modules.Arena.Data.ArenaScore;
	import GameUI.Modules.Arena.View.ArenaScoreItemRenderer;
	import GameUI.Modules.Task.Mediator.TaskFollowMediator;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.UIKit.components.PagingDataList;
	import GameUI.View.UIKit.data.ArrayList;
	import GameUI.View.UIKit.data.SortOptions;
	
	import Net.ActionSend.WarGameSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ArenaPanelMediator extends Mediator
	{
		public static const NAME:String = "ArenaPanelMediator";
		public const DEFAULT_POS:Point = new Point(220, 30);
		public const SMALL_PANEL_DEFAULT_POS:Point = new Point(853, 204);
		public const SMALL_PANEL_FOLDED_POS: Point = new Point(853, 204);
		
		protected var dataProxy:DataProxy;
		protected var panelView:MovieClip;
		protected var basePanel:PanelBase;
		protected var list:PagingDataList;
		protected var smallPanel:MovieClip;
		
		public function get panelSmall():MovieClip
		{
			return smallPanel;
		}
		
		public function get panelBig():PanelBase
		{
			return basePanel;
		}
		
		public function ArenaPanelMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				ArenaPanelCommandList.ARENAPANEL_SHOW,
				ArenaPanelCommandList.ARENAPANEL_UPDATE,
				ArenaPanelCommandList.ARENASMALLPANEL_SHOW,
				ArenaPanelCommandList.ARENASMALLPANEL_HIDE,
				ArenaPanelCommandList.ARENASMALLPANEL_UPDATE,
				ArenaPanelCommandList.ARENASMALLPANEL_CALL_UPDATE
			];
		}
		
		protected var initialized:Boolean = false;
		protected var updatePending:Boolean = false;
		
		override public function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case ArenaPanelCommandList.ARENAPANEL_SHOW:
					if (notification.getBody() != null)
					{
						updatePending = notification.getBody().update as Boolean;
					}
					else
					{
						updatePending = true;
					}
					if (!initialized)
					{
						ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ArenaPanel.swf", onPanelLoadComplete);
						dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
						break;
					}
					show(updatePending);
					break;
				case ArenaPanelCommandList.ARENAPANEL_UPDATE:
					update();
					break;
				case ArenaPanelCommandList.ARENASMALLPANEL_SHOW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if (initialized)
					{
						onSmallPanelLoadComplete();
					}
					else
					{
						ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ArenaPanel.swf", onSmallPanelLoadComplete);
					}
					break;
				case ArenaPanelCommandList.ARENASMALLPANEL_HIDE:
					hideSmallPanel();
					break;
				case ArenaPanelCommandList.ARENASMALLPANEL_UPDATE:
					if (smallPanel)
					{
						smallPanel.full.txtCamp1Score.text = ArenaScore.camp1Score;
						smallPanel.full.txtCamp2Score.text = ArenaScore.camp2Score;
						smallPanel.full.txtCamp3Score.text = ArenaScore.camp3Score;
					}
					break;
				case ArenaPanelCommandList.ARENASMALLPANEL_CALL_UPDATE:
					callUpdate();
					break;
			}
		}
		
		protected function onPanelLoadComplete():void
		{
			var swf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ArenaPanel.swf");
			panelView = new (swf.loaderInfo.applicationDomain.getDefinition("ArenaPanel"));
			this.setViewComponent(panelView);
			
			this.basePanel = new PanelBase(panelView, panelView.width + 8, panelView.height + 12);
			
			this.basePanel.addEventListener(Event.CLOSE, basePanel_closeHandler);
			
			this.basePanel.SetTitleTxt(GameCommonData.wordDic["mod_are_med_are_onp_1"]); // 将星排行榜
			
			(panelView.tabTotal as MovieClip).gotoAndStop(2);
			
			(panelView.tabTotal as MovieClip).useHandCursor = (panelView.tabTotal as MovieClip).buttonMode =
			(panelView.tabCamp1 as MovieClip).useHandCursor = (panelView.tabCamp1 as MovieClip).buttonMode =
			(panelView.tabCamp2 as MovieClip).useHandCursor = (panelView.tabCamp2 as MovieClip).buttonMode =
			(panelView.tabCamp3 as MovieClip).useHandCursor = (panelView.tabCamp3 as MovieClip).buttonMode = true;
			
			(panelView.tabTotal as MovieClip).addEventListener(MouseEvent.CLICK, tab_clickHandler);
			(panelView.tabCamp1 as MovieClip).addEventListener(MouseEvent.CLICK, tab_clickHandler);
			(panelView.tabCamp2 as MovieClip).addEventListener(MouseEvent.CLICK, tab_clickHandler);
			(panelView.tabCamp3 as MovieClip).addEventListener(MouseEvent.CLICK, tab_clickHandler);
			
			panelView.tabCamp1.stop();
			panelView.tabCamp2.stop();
			panelView.tabCamp3.stop();
			
			currentTab = (panelView.tabTotal as MovieClip);
			
			list = new PagingDataList();
			list.itemRenderer = ArenaScoreItemRenderer;
			list.pageSize = 12;
			list.addEventListener("pageTotalChanged", list_pageTotalChangedHandler);
			list.x = 9;
			list.y = 138;
			panelView.addChild(list);
			
			(panelView.btnPrevPage as SimpleButton).addEventListener(MouseEvent.CLICK, pageButton_clickHandler);
			(panelView.btnNextPage as SimpleButton).addEventListener(MouseEvent.CLICK, pageButton_clickHandler);
			
			initialized = true;
			show(updatePending);
		}
		
		protected function onSmallPanelLoadComplete():void
		{
			var swf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ArenaPanel.swf");
			smallPanel = new (swf.loaderInfo.applicationDomain.getDefinition("ArenaSmallPanel"));
			
			smallPanel.small.visible = false;
			smallPanel.full.mcMove.stop();
			smallPanel.full.mcMove.addEventListener(MouseEvent.MOUSE_DOWN, onDragMcMouseDown);
			smallPanel.full.btnFold.addEventListener(MouseEvent.CLICK, onFold);
			smallPanel.small.btnUnfold.addEventListener(MouseEvent.CLICK, onUnfold);
			smallPanel.full.btnDetail.addEventListener(MouseEvent.CLICK, onDetail);
						
			showSmallPanel();
		}
		
		protected function onDragMcMouseDown(e:MouseEvent):void{
			dragBoundMinX = 0;
			dragBoundMaxX = GameCommonData.GameInstance.GameUI.stage.stageWidth - this.smallPanel.full.width; //GameCommonData.GameInstance.GameUI.width;
			dragBoundMinY = 0;
			dragBoundMaxY = GameCommonData.GameInstance.GameUI.stage.stageHeight - this.smallPanel.full.height; //GameCommonData.GameInstance.GameUI.height;
					
			this.anchorPoint = new Point(this.smallPanel.mouseX, this.smallPanel.mouseY);
			this.smallPanel.stage.addEventListener(MouseEvent.MOUSE_MOVE, dragUIHandler);
			this.smallPanel.stage.addEventListener(MouseEvent.MOUSE_UP, stopDragUIHandler);
			this.smallPanel.full.mcMove.gotoAndStop(2);
		}
		
		protected var anchorPoint:Point;
		protected var dragBoundMinX:Number;
		protected var dragBoundMaxX:Number;
		protected var dragBoundMinY:Number;
		protected var dragBoundMaxY:Number;
		
		protected function dragUIHandler(event:MouseEvent):void
		{
			var actualX:Number = event.stageX - anchorPoint.x;
			var actualY:Number = event.stageY - anchorPoint.y;
			
			actualX = Math.max(dragBoundMinX, actualX);
			actualX = Math.min(dragBoundMaxX, actualX);
			
			actualY = Math.max(dragBoundMinY, actualY);
			actualY = Math.min(dragBoundMaxY, actualY); 
			
			this.smallPanel.x = actualX;
			this.smallPanel.y = actualY;
		}
		
		protected function stopDragUIHandler(event:MouseEvent):void
		{
			this.smallPanel.stage.removeEventListener(MouseEvent.MOUSE_MOVE, dragUIHandler);
			this.smallPanel.stage.removeEventListener(MouseEvent.MOUSE_UP, stopDragUIHandler);
			this.smallPanel.full.mcMove.gotoAndStop(1);
		}
		
		protected function onFold(event:MouseEvent):void
		{
			smallPanel.full.visible = false;
			smallPanel.small.visible = true;
			smallPanel.x = SMALL_PANEL_FOLDED_POS.x;
			smallPanel.y = SMALL_PANEL_FOLDED_POS.y;
			sendNotification(EventList.STAGECHANGE);
		}
		
		protected function onUnfold(event:MouseEvent):void
		{
			smallPanel.full.visible = true;
			smallPanel.small.visible = false;
			sendNotification(EventList.STAGECHANGE);
		}
		
		protected function onDetail(event:MouseEvent):void
		{
			sendNotification(ArenaPanelCommandList.ARENAPANEL_SHOW, {update: true});
		}	
		
		protected var currentTab:MovieClip;
		
		protected function tab_clickHandler(event:MouseEvent):void
		{
			if (event.currentTarget == currentTab) return;
			
			if (event.currentTarget == panelView.tabTotal)
			{
				list.data = listData;
			}			
			else
			{
				switch (event.currentTarget)
				{
					case panelView.tabCamp1:
						list.data = getListByCamp(1);
						break;
					case panelView.tabCamp2:
						list.data = getListByCamp(2);
						break;
					case panelView.tabCamp3:
						list.data = getListByCamp(3);
						break;
				}
			}
			currentTab.gotoAndStop(1);
			(event.currentTarget as MovieClip).gotoAndStop(2);
			currentTab = (event.currentTarget as MovieClip);
		}
		
		protected function pageButton_clickHandler(event:MouseEvent):void
		{
			if (event.currentTarget == (panelView.btnPrevPage))
			{
				list.currentPage --;
				resetPageNums();
			}
			else
			{
				list.currentPage ++;
				resetPageNums();
			}
		}
		
		protected function list_pageTotalChangedHandler(event:Event):void
		{
			resetPageNums();
		}
		
		protected function resetPageNums():void
		{
			(panelView.txtPageNum as TextField).text = GameCommonData.wordDic["mod_are_med_are_res_1"] // 第
																				+ (list.currentPage + 1) + "/" + Math.max(1, list.pageTotal)
																				+ GameCommonData.wordDic["mod_are_med_are_res_2"]; // 页
		}
		
		protected function basePanel_closeHandler(event:Event):void
		{
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			dataProxy.arenaPanelIsOpen = false;
			clearTimeout(intervalKey);
		}
		
		protected function show(update:Boolean):void
		{
			dataProxy.arenaPanelIsOpen = true;
			if (update)
			{
				var timeSpan:Number = new Date().getTime() - lastUpdate;
				if (timeSpan > 5000)
				{
					callUpdate();
				}
				else
				{
					clearTimeout(intervalKey);
					intervalKey = setTimeout(callUpdate, 5000 - timeSpan);
				}
			}
			if( GameCommonData.fullScreen == 2 )
			{
				this.basePanel.x = DEFAULT_POS.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - basePanel.width)/2;
				this.basePanel.y = DEFAULT_POS.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - basePanel.height)/2;
			}
			else
			{
				this.basePanel.x = DEFAULT_POS.x;
				this.basePanel.y = DEFAULT_POS.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
		}
		
		protected function showSmallPanel():void
		{
			if ((facade.retrieveMediator( TaskFollowMediator.NAME ) as TaskFollowMediator).taskFollowUI != null)
			{
				(facade.retrieveMediator( TaskFollowMediator.NAME ) as TaskFollowMediator).taskFollowUI.visible = false;
			}
			if( GameCommonData.fullScreen == 2 )
			{
				smallPanel.x = SMALL_PANEL_DEFAULT_POS.x + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
			}
			else
			{
				smallPanel.x = SMALL_PANEL_DEFAULT_POS.x;
			}
			smallPanel.y = SMALL_PANEL_DEFAULT_POS.y;
			GameCommonData.GameInstance.GameUI.addChild(this.smallPanel);
			
			dataProxy.arenaSmallPanelIsOpen = true;
		}
		
		protected function hideSmallPanel():void
		{
			if ((facade.retrieveMediator( TaskFollowMediator.NAME ) as TaskFollowMediator).taskFollowUI != null)
			{
				(facade.retrieveMediator( TaskFollowMediator.NAME ) as TaskFollowMediator).taskFollowUI.visible = true;
			}
			
			if (this.smallPanel && GameCommonData.GameInstance.GameUI.contains(this.smallPanel))
			{
				GameCommonData.GameInstance.GameUI.removeChild(this.smallPanel);	
			}
			
			if (dataProxy)
				dataProxy.arenaSmallPanelIsOpen = false;
		}
		
		protected var listData:ArrayList;
		protected var listCampData:ArrayList;
		
		protected var lastUpdate:Number = 0;
		
		protected function callUpdate():void
		{
//			trace(">>> call update");
			lastUpdate = new Date().getTime();
			WarGameSend.sendWarGameAction({action:3, pageIndex:0, memID: GameCommonData.Player.Role.Id});
		}
		
		protected function update():void
		{
//			trace(">>> client side update");
			if (panelView)
			{
				(panelView.txtCamp1Score as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_7"] + GameCommonData.wordDic["mod_are_med_are_upd_10"] + String(ArenaScore.camp1Score);
				(panelView.txtCamp2Score as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_8"] + GameCommonData.wordDic["mod_are_med_are_upd_10"] + String(ArenaScore.camp2Score);
				(panelView.txtCamp3Score as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_9"] + GameCommonData.wordDic["mod_are_med_are_upd_10"] + String(ArenaScore.camp3Score);
				(panelView.txtMyCamp as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_1"];
				(panelView.txtMyScore as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_2"] + String(GameCommonData.Player.Role.arenaScore);	
				(panelView.txtCurrentScore as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_3"] + String(ArenaScore.myScore + ArenaScore.myAwardScore);
				(panelView.txtKill as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_4"] + String(ArenaScore.myKill);
				(panelView.txtRank as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_5"] + String(getMyCampRank());
				
				switch (ArenaScore.myCamp)
				{
					case 1:
						(panelView.txtMyCamp as TextField).appendText(GameCommonData.wordDic["mod_are_med_are_upd_7"]);
						(panelView.txtMyCamp as TextField).textColor = 0xFF32CC;
						break;
					case 2:
						(panelView.txtMyCamp as TextField).appendText(GameCommonData.wordDic["mod_are_med_are_upd_8"]);
						(panelView.txtMyCamp as TextField).textColor = 0x00CBFF;
						break;
					case 3:
						(panelView.txtMyCamp as TextField).appendText(GameCommonData.wordDic["mod_are_med_are_upd_9"]);
						(panelView.txtMyCamp as TextField).textColor = 0xE08E1F;
						break;
				}
			
				
				listData = ArenaScore.listFull.clone();
				sort(listData);
				ArenaScore.myRank = listData.getItemAt(listData.query("id", GameCommonData.Player.Role.Id)[0]).rank;
				(panelView.txtGrandRank as TextField).text = GameCommonData.wordDic["mod_are_med_are_upd_6"] + String(ArenaScore.myRank);
			}
			
			if (list)
			{
				switch (currentTab)
				{
					case panelView.tabTotal:
						list.data = listData;
						break;
					case panelView.tabCamp1:
						list.data = getListByCamp(1);
						break;
					case panelView.tabCamp2:
						list.data = getListByCamp(2);
						break;
					case panelView.tabCamp3:
						list.data = getListByCamp(3);
						break;
				}
			}
			
			ArenaScore.listFull.removeAll();
			
			if (smallPanel)
			{
				smallPanel.full.txtCamp1Score.text = ArenaScore.camp1Score;
				smallPanel.full.txtCamp2Score.text = ArenaScore.camp2Score;
				smallPanel.full.txtCamp3Score.text = ArenaScore.camp3Score;
			}
			
			clearTimeout(intervalKey);
			if (updatePending)
				intervalKey = setTimeout(callUpdate, 5000);
		}
		
		protected function sort(data:ArrayList):void
		{
			function compareFunc(a:Object, b:Object):int
			{
				var a1:Number = a.currentScore + a.awardScore;
				var b1:Number = b.currentScore + b.awardScore;
				
				if (a1 > b1)
				{ 
					return 1;
				}
				else if (a1 < b1)
				{
					return -1;
				}
				else
				{
					return 0;
				}
			} 
			
			listData.startSort(new SortOptions(null, Array.DESCENDING, compareFunc));
			var l:int = listData.length;
			for (var i:int = 0; i < l; i ++)
			{
				listData.getItemAt(i).rank = (i + 1);
			}
		}
		
		protected var intervalKey:uint;
		
		protected function getMyCampRank():int
		{
			var l:int = ArenaScore.listFull.length;
			var a:Array = [];
			
			for (var i:int = 0; i < l; i ++)
			{
				var o:Object = ArenaScore.listFull.getItemAt(i);
				if (o.camp == ArenaScore.myCamp)
					a.push(o.currentScore);
			}
			
			a.sort(Array.NUMERIC | Array.DESCENDING);
			
			return a.indexOf(ArenaScore.myScore) + 1;
		}
		
		protected function getListByCamp(camp:int):ArrayList
		{
			if (listData == null) return null;
			
			var result:ArrayList = new ArrayList();
			
			var l:int = listData.length;
			
			for (var i:int = 0; i < l; i ++)
			{
				var o:Object = listData.getItemAt(i);
				if (o.camp == camp)
					result.addItem(o);
			}
			
			return result;
		}
	}
}