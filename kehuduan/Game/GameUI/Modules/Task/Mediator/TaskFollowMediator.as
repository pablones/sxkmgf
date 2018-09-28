package GameUI.Modules.Task.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Task.View.TaskAccPanel;
	import GameUI.Modules.Task.View.TaskFollowPanel;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TaskFollowMediator extends Mediator
	{
		public static const NAME:String="TaskFollowMediator";
		public const DEFAULT_POS:Point=new Point(780,208);//(794,211); (764,185);(733,185);
		public const ALPHA:Array=[0,0.2,0.4,0.6,1];
		public const BG_HEIGHT:Array=[163,235,308];
		public const FOLDED_POS:Point = new Point(779,210);//(779,192);
		
		protected var currentAlphaIndex:uint=4;
		protected var currentHeightIndex:uint=0;
		protected var taskFollowPanel:TaskFollowPanel;
		protected var taskAccPanel:TaskFollowPanel;
		protected var dragFlag:Boolean=false;
		protected var dataProxy:DataProxy = null;
		
		
		public function TaskFollowMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get taskFollowUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		protected var showViewPending:Boolean = false;
		
		
		public override function handleNotification(notification:INotification):void{
			var taskInfo:TaskInfoStruct=null;
			var dic:Dictionary=null;
			var arr:Array;
			var index:int;
			switch (notification.getName()){
				case EventList.INITVIEW :
					// 大框架 taskFollowUI 的加载过程以异步形式转移到最后了，onPanelLoadComplete。  -zhao 
					ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/TaskFollowAccPanel.swf", onPanelLoadComplete);
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					this.taskFollowPanel=new TaskFollowPanel(BG_HEIGHT[currentHeightIndex]);
					taskFollowPanel.x=2;
					taskFollowPanel.y=21;  // 17;
					this.taskAccPanel = new TaskAccPanel(BG_HEIGHT[currentHeightIndex]);
					taskAccPanel.x = 2;
					taskAccPanel.y = 21; 
					break;
				case TaskCommandList.SHOW_TASKFOLLOW_UI :
					this.showView();  
					sendNotification( EventList.CHANGE_UI, 1 );
					sendNotification(TaskCommandList.SELECT_TASKFOLLOW_PAGE);
					break;
				case TaskCommandList.HIDE_TASKFOLLOW_UI:
					this.closeView();
					if (this.taskFollowUI.contains(this.taskFollowPanel))
						this.taskFollowUI.removeChild(this.taskFollowPanel);
					if (this.taskFollowUI.contains(this.taskAccPanel))
						this.taskFollowUI.removeChild(this.taskAccPanel);
					sendNotification( EventList.CHANGE_UI, 1 );
					break;
				case TaskCommandList.SELECT_TASKFOLLOW_PAGE:
					if (!this.taskFollowUI.contains(this.taskFollowPanel))
						this.taskFollowUI.addChild(this.taskFollowPanel);
					if (this.taskFollowUI.contains(this.taskAccPanel))
						this.taskFollowUI.removeChild(this.taskAccPanel);
					this.taskFollowUI.select(this.taskFollowUI.full.mc_taskFollow);
					this.taskFollowUI.deselect(this.taskFollowUI.full.mc_taskAcc);
					dataProxy.TaskFollowUIIsFolded = false;
					dataProxy.TaskFollowIsOpen = true;
					dataProxy.TaskAccIsOpen = false;
					break;
				case TaskCommandList.SELECT_TASKACC_PAGE:
					if (!this.taskFollowUI.contains(this.taskAccPanel))
						this.taskFollowUI.addChild(taskAccPanel);
					if (this.taskFollowUI.contains(this.taskFollowPanel))
						this.taskFollowUI.removeChild(this.taskFollowPanel);
					this.taskFollowUI.deselect(this.taskFollowUI.full.mc_taskFollow);
					this.taskFollowUI.select(this.taskFollowUI.full.mc_taskAcc);
					dataProxy.TaskFollowUIIsFolded = false;
					dataProxy.TaskFollowIsOpen = false;
					dataProxy.TaskAccIsOpen = true;
					break;
				case TaskCommandList.ADD_TASK_FOLLOW:
					taskInfo=notification.getBody() as TaskInfoStruct;
					dic=this.taskFollowPanel.dataDic;
					arr=dic[taskInfo.taskType];
					if(arr==null){
						arr=new Array();
					}else{
						if(arr.indexOf(taskInfo)!=-1){
							return;
						}
					}
					arr.push(taskInfo);
					dic[taskInfo.taskType]=arr;
					this.taskFollowPanel.dataDic=dic;
					break;
				case TaskCommandList.REMOVE_TASK_FOLLOW:
					taskInfo=notification.getBody() as TaskInfoStruct;
					dic=this.taskFollowPanel.dataDic;
					arr=dic[taskInfo.taskType];
					if(arr==null)return;
					index=arr.indexOf(taskInfo);
					if(index==-1)return;
					arr.splice(index,1);
					this.taskFollowPanel.dataDic=dic;
					break;	
				case TaskCommandList.REFRESH_TASKFOLLOW:
					this.taskFollowPanel.refresh();
					break;	
				case TaskCommandList.ACCTASK_SYNC_ADD:
					taskInfo = GameCommonData.TaskInfoDic[notification.getBody()] as TaskInfoStruct;
					dic = this.taskAccPanel.dataDic;
					arr = dic[taskInfo.taskType];
					if (arr == null)
					{
						arr = new Array();
					}
					else if (arr.indexOf(taskInfo) != -1)
					{
						return;
					}
					arr.push(taskInfo);
					dic[taskInfo.taskType] = arr;
					this.taskAccPanel.dataDic = dic;
					
					break;
				case TaskCommandList.ACCTASK_SYNC_REMOVE:
					taskInfo = GameCommonData.TaskInfoDic[notification.getBody()] as TaskInfoStruct;
					dic = this.taskAccPanel.dataDic;
					arr = dic[taskInfo.taskType];
					if (arr == null) return;
					index = arr.indexOf(taskInfo);
					if (index == -1) return;
					arr.splice(index, 1);
					this.taskAccPanel.dataDic = dic;
					break;
				case EventList.ENTERMAPCOMPLETE :
					if (!this.taskFollowUI)
					{
						showViewPending = true;
					}
					else
					{
						this.showView();
					}
					break;
				case TaskCommandList.UPDATE_TASK_TOTAL:
					// TODO: 和策划讨论后决定是否恢复 taskTotal 显示		--zhao
					//(this.taskFollowUI.txt_taskTotal as TextField).text=GameCommonData.wordDic[ "mod_task_med_taskf_han_1" ]+notification.getBody()+"/20";     //任务追踪
					break;
				case TaskCommandList.SET_TASKFOLLOW_DRAG:
					this.dragFlag=notification.getBody() as Boolean; //=true禁止拖动，=false可拖动
					if(this.dragFlag){
						if(!dataProxy.TaskFollowUIIsFolded)
						{
							sendNotification(TaskCommandList.SHOW_TASKFOLLOW_UI);
						}
//						if(!dataProxy.TaskFollowIsOpen){
//							sendNotification(TaskCommandList.SET_SHOW_FOLLOW,2);
//						}
						this.sendNotification( EventList.CHANGE_UI, 1 );
					}
					break;
			}
		}
		
		/**
		 * 返回任务ID在追踪面板上的位置，如果没有该ID的任务就返回null; 
		 * @param taskId ：任务ID号
		 * @return 
		 * 
		 */		
		public function getTaskRectangle(taskId:uint):Rectangle{
			return this.taskFollowPanel.getTaskRectangle(String(taskId));
		}
		
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				TaskCommandList.SHOW_TASKFOLLOW_UI,
				TaskCommandList.HIDE_TASKFOLLOW_UI,
				TaskCommandList.SELECT_TASKFOLLOW_PAGE,
				TaskCommandList.SELECT_TASKACC_PAGE,
				TaskCommandList.ADD_TASK_FOLLOW,
				TaskCommandList.REMOVE_TASK_FOLLOW,
				TaskCommandList.REFRESH_TASKFOLLOW,
				EventList.ENTERMAPCOMPLETE,
				TaskCommandList.UPDATE_TASK_TOTAL,
				TaskCommandList.SET_TASKFOLLOW_DRAG,
				TaskCommandList.ACCTASK_SYNC_ADD,
				TaskCommandList.ACCTASK_SYNC_REMOVE
			];
		}
		
		protected function showView():void{
			taskFollowUI.mouseEnabled = false;
			if (!GameCommonData.GameInstance.GameUI.contains(this.taskFollowUI))
				GameCommonData.GameInstance.GameUI.addChild(this.taskFollowUI);
			this.taskFollowUI.full.visible = true;
			this.taskFollowUI.small.visible = false;
			dataProxy.TaskFollowIsOpen = true;
			dataProxy.TaskAccIsOpen = false;
			dataProxy.TaskFollowUIIsFolded = false;
			
			this.taskFollowUI.x = DEFAULT_POS.x;
			this.taskFollowUI.y = DEFAULT_POS.y;
			
			sendNotification(EventList.STAGECHANGE);
			
			// 从折叠模式展开时可能超出屏幕。这里调整面板位置。  --zhao
//			var actualX:Number = this.taskFollowUI.x;
//			var actualY:Number = this.taskFollowUI.y;
//			
//			actualX = Math.max(0, actualX);
//			actualX = Math.min(1000 - this.taskFollowUI.full.width, actualX);
//			
//			actualY = Math.max(0, actualY);
//			actualY = Math.min(580 - this.taskFollowUI.full.height, actualY); 
//			
//			this.taskFollowUI.x = actualX;
//			this.taskFollowUI.y = actualY;
		}
		
		protected function initSet():void{
			var drag:MovieClip = this.taskFollowUI.full.mc_drag as MovieClip;
			drag.addEventListener(MouseEvent.MOUSE_DOWN, onDragMcMouseDownHandler);
			
			var bg:MovieClip = this.taskFollowUI.full.mc_bg as MovieClip;
			bg.height = BG_HEIGHT[currentHeightIndex];
			bg.mouseEnabled = false;
			
			var fold:SimpleButton = this.taskFollowUI.full.btn_fold as SimpleButton;
			fold.addEventListener(MouseEvent.CLICK, onFoldHandler);
			
			var unfold:SimpleButton = this.taskFollowUI.small.btn_unfold as SimpleButton;
			unfold.addEventListener(MouseEvent.CLICK, onUnfoldHandler);
			
//			var dragBig:MovieClip = this.taskFollowUI.small.mc_dragBig as MovieClip;
//			dragBig.addEventListener(MouseEvent.MOUSE_DOWN, onDragMcMouseDownHandler);
			
			var taskFollow:MovieClip = this.taskFollowUI.full.mc_taskFollow as MovieClip;
			taskFollow.addEventListener(MouseEvent.CLICK, onSelectTaskFollowHandler);
			
			var taskAcc:MovieClip = this.taskFollowUI.full.mc_taskAcc as MovieClip;
			taskAcc.addEventListener(MouseEvent.CLICK, onSelectTaskAccHandler);
			
			this.taskFollowUI.full.mouseEnabled = false;
			this.taskFollowUI.small.mouseEnabled = false;
		}
		
		
		protected function onDragMcMouseDownHandler(e:MouseEvent):void{
			if(this.dragFlag){
				return;
			}
			// TODO: 没找到哪里能取到正确的舞台大小    --zhao
//			if (e.currentTarget == this.taskFollowUI.full.mc_drag)
//			{
				dragBoundMinX = 0;
				dragBoundMaxX = GameCommonData.GameInstance.GameUI.stage.stageWidth - this.taskFollowUI.full.width; //GameCommonData.GameInstance.GameUI.width;
				dragBoundMinY = 0;
				dragBoundMaxY = GameCommonData.GameInstance.GameUI.stage.stageHeight - this.taskFollowUI.full.height; //GameCommonData.GameInstance.GameUI.height;	
//			}
//			else if (e.currentTarget == this.taskFollowUI.small.mc_dragBig)
//			{
//				dragBoundMinX = this.taskFollowUI.small.width - this.taskFollowUI.full.width;
//				dragBoundMaxX = 1000 - this.taskFollowUI.full.width;
//				dragBoundMinY = 0;
//				dragBoundMaxY = 580 - this.taskFollowUI.small.height;
//			}
			this.anchorPoint = new Point(this.taskFollowUI.mouseX, this.taskFollowUI.mouseY);
			this.taskFollowUI.stage.addEventListener(MouseEvent.MOUSE_MOVE, dragUIHandler);
			this.taskFollowUI.stage.addEventListener(MouseEvent.MOUSE_UP, stopDragUIHandler);
			this.taskFollowUI.full.mc_drag.gotoAndStop(2);
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
			
			this.taskFollowUI.x = actualX;
			this.taskFollowUI.y = actualY;
		}
		
		protected function stopDragUIHandler(event:MouseEvent):void
		{
			this.taskFollowUI.stage.removeEventListener(MouseEvent.MOUSE_MOVE, dragUIHandler);
			this.taskFollowUI.stage.removeEventListener(MouseEvent.MOUSE_UP, stopDragUIHandler);
			this.taskFollowUI.full.mc_drag.gotoAndStop(1);
		}
		
		protected function onFoldHandler(e:MouseEvent):void{
			sendNotification(TaskCommandList.SET_SHOW_FOLLOW);
		}
		
		protected function onUnfoldHandler(e:MouseEvent):void
		{
			sendNotification(TaskCommandList.SET_SHOW_FOLLOW, 2);
		}
		
		protected function onSelectTaskFollowHandler(event:MouseEvent):void
		{
			sendNotification(TaskCommandList.SELECT_TASKFOLLOW_PAGE);
		}
		
		protected function onSelectTaskAccHandler(event:MouseEvent):void
		{
			sendNotification(TaskCommandList.SELECT_TASKACC_PAGE);
		}
		
		protected function closeView():void{
			if(this.taskFollowUI && GameCommonData.GameInstance.GameUI.contains(this.taskFollowUI)) {
				this.taskFollowUI.small.visible = true;
				this.taskFollowUI.full.visible = false;
				this.taskFollowUI.x = FOLDED_POS.x;
				this.taskFollowUI.y = FOLDED_POS.y;
				dataProxy.TaskFollowIsOpen = false;
				dataProxy.TaskAccIsOpen = false;
				dataProxy.TaskFollowUIIsFolded = true;
			}
		}
		
//		protected function onExpandHandler(e:MouseEvent):void{
//			currentHeightIndex++;
//			if(currentHeightIndex>=BG_HEIGHT.length)currentHeightIndex=0;
//			(this.taskFollowUI.full.mc_bg as MovieClip).height=BG_HEIGHT[currentHeightIndex];
//			this.taskFollowPanel.maxHeight=BG_HEIGHT[currentHeightIndex];
//		}
//		
//		protected function onAlphaHandler(e:MouseEvent):void{
//			currentAlphaIndex++;
//			if(currentAlphaIndex>=ALPHA.length)currentAlphaIndex=0;
//			(this.taskFollowUI.full.mc_bg as MovieClip).alpha=ALPHA[currentAlphaIndex];
//		}
		
		protected function onPanelLoadComplete():void
		{
			var panelSwf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/TaskFollowAccPanel.swf");
			var panelMc:MovieClip = new (panelSwf.loaderInfo.applicationDomain.getDefinition("TaskFollowAcc"));
			this.setViewComponent(panelMc);
			this.initSet();
			
			this.taskFollowUI.x=DEFAULT_POS.x;
			this.taskFollowUI.y=DEFAULT_POS.y;
			
			if (showViewPending)
			{
				showViewPending = false;
				this.showView();
			}
			
//			this.taskFollowUI.stage.addEventListener(MouseEvent.CLICK, function(event:MouseEvent):void{trace(event.target);});
			this.taskFollowUI.full.mc_drag.stop();
			sendNotification(TaskCommandList.SELECT_TASKFOLLOW_PAGE);
		}	
	}
}