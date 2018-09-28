package GameUI.Modules.Task.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Modules.Task.Commamd.RecallTaskCommand;
	import GameUI.Modules.Task.Commamd.ReceiveTaskCommand;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Commamd.TaskSpecificCommand;
	import GameUI.Modules.Task.Commamd.TreeEvent;
	import GameUI.Modules.Task.Commamd.UpdateLevelTaskCommand;
	import GameUI.Modules.Task.Commamd.UpdateMaskCommand;
	import GameUI.Modules.Task.Commamd.UpdateTaskProcess;
	import GameUI.Modules.Task.Model.TaskGroupStruct;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Task.View.AcceptTaskInfoPanel;
	import GameUI.Modules.Task.View.TaskGroupCellRenderer;
	import GameUI.Modules.Task.View.TaskInfoPanel;
	import GameUI.Modules.Task.View.TreeCellRenderer;
	import GameUI.Modules.Task.View.UITree;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.DisplayObject;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TaskMediator extends Mediator
	{
		public static const NAME:String = "TaskMediator";
		
		//2011-07-12 luchao add start
		private var loader:Loader;
		private var isNewTaskView:Boolean = true; 
		private var newTaskPage:MovieClip;
		//2011-07-12 luchao add end
		
		public var taskTree:UITree;
		public var accTaskTree:UITree;
		
		protected var scrollPanel:UIScrollPane;
		protected var accScrollPanel:UIScrollPane;
		/** 已经接任务细节 */
		protected var taskDetailPanel:TaskInfoPanel;
		/** 可接任务细节*/
		protected var acceptTaskPanel:AcceptTaskInfoPanel;
		
		protected var curID:uint;
		protected var isCurrentPage:Boolean=true;
		protected var taskTotal:int=0;     //任务数
		protected var isFirstCancel:Boolean=false;
		
		/** 是否是第一次接收任务*/
		protected var doneFirst:Boolean=false;
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		
		private var _fbIcon:DisplayObject;	//fb按钮
		private var _fbNeedTaskId:int;	//选择的可接任务id（fb成就系统用）
		public function TaskMediator()
		{
			super(NAME);
		}
		
		private function get taskView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function onRegister():void
		{
			if(this.isNewTaskView == true){
				initNewTask();
			}
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWTASKVIEW,
				EventList.CLOSETASKVIEW,
				TaskCommandList.UPDATETASKTREE,
				TaskCommandList.UPDATE_TASK_PROCESS_VIEW,
				TaskCommandList.UPDATE_TASK_TOTAL,
				TaskCommandList.ADD_ACCEPT_TASK,
				TaskCommandList.REMOVE_ACCEPT_TASK,
				TaskCommandList.SHOW_SELECTED_TASK,
				TaskCommandList.SET_SHOW_FOLLOW,
				TaskCommandList.UPDATE_ACCTASK_UITREE,
				EventList.CLOSE_NPC_ALL_PANEL,
				TaskCommandList.DEAL_AFTER_SEND_FB_FORM_TASKPANEL,
				TaskCommandList.SEND_FB_AWARD
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.registerCommand(TaskCommandList.SHOW_TASKSPECIFIC_COMMAND,TaskSpecificCommand);
					facade.registerCommand(TaskCommandList.UPDATE_MASK,UpdateMaskCommand);
					facade.registerCommand(TaskCommandList.RECEIVE_TASK,ReceiveTaskCommand);
					facade.registerCommand(TaskCommandList.RECALL_TASK,RecallTaskCommand);
					facade.registerCommand(TaskCommandList.UPDATE_TASK_PROCESS,UpdateTaskProcess);
					facade.registerCommand(TaskCommandList.UPDATE_LEVEL_TASK,UpdateLevelTaskCommand);
					facade.registerMediator(new TaskVitMediator());
				break;
				case EventList.SHOWTASKVIEW:
					showView();
					dataProxy.TaskIsOpen=true; 
					this.taskTree.width = 190;
					this.taskTree.y=8;
					this.scrollPanel.refresh();
					this.taskView.addChild(this.scrollPanel);
					this.scrollPanel.x = 12;
					this.scrollPanel.y = 26;
					
					this.accTaskTree.width = 190;
					this.accTaskTree.y=8;
					this.accScrollPanel.refresh();
					this.taskView.addChild(this.accScrollPanel);
					this.accScrollPanel.x = 12;
					this.accScrollPanel.y = 26;
					this.accScrollPanel.visible=false;
					this.setSelectedPage(0);
					if(notification.getBody()!=null){
						this.setSelectedPage(uint(notification.getBody()));
					}
					break;
				case EventList.CLOSETASKVIEW:
					panelCloseHandler(null);
					break;
				case TaskCommandList.UPDATETASKTREE:
					this.updateTaskTree(notification.getBody()["type"],notification.getBody()["id"]);
					break;
				case TaskCommandList.UPDATE_TASK_PROCESS_VIEW:
					var taskInfo:TaskInfoStruct=notification.getBody() as TaskInfoStruct;
					this.taskTree.refresh();
					 if(this.isCurrentPage && this.curID==taskInfo.id){
						this.taskDetailPanel.update(taskInfo.id);
					} 
					sendNotification(TaskCommandList.REFRESH_TASKFOLLOW);
					break;	
					
				case TaskCommandList.UPDATE_TASK_TOTAL:  
					(this.taskView.txt_currentTaskNum as TextField).text="任务数量：" + this.taskTotal+"/20";
					break;	
					
				case TaskCommandList.ADD_ACCEPT_TASK:
					if(!this.doneFirst){
				 		this.doneFirst=true;
				 		this.setAcc();    //初始可接任务表（如果是链将第一步加入）
				 	}
					this.clearTaskChainInAcc(uint(notification.getBody()));
					this.updateTaskTree(11,uint(notification.getBody()));
					break
				case TaskCommandList.REMOVE_ACCEPT_TASK:
					this.updateTaskTree(12,uint(notification.getBody()));		
					break;
				case TaskCommandList.SHOW_SELECTED_TASK:
					if(!dataProxy.TaskIsOpen){
						sendNotification(EventList.SHOWTASKVIEW);
					}
					this.taskTree.setSelected(uint(notification.getBody()));
					break;
				case TaskCommandList.SET_SHOW_FOLLOW:
					if(notification.getBody()!=null){
						this.setShowFollow(2);
					}else{
						this.setShowFollow(1);
					}	
					break;
				case TaskCommandList.UPDATE_ACCTASK_UITREE:
					this.refreshAccTree();
					break;	
				case EventList.CLOSE_NPC_ALL_PANEL:
					panelCloseHandler(null);
					break;
				case TaskCommandList.DEAL_AFTER_SEND_FB_FORM_TASKPANEL:	//fb账号绑定请求后返回
					removeFbIcon();
					addFbIcon();
					break;
			}
		}
		
		/**
		 *加载的资源初始化 
		 * 
		 */		
		private function init():void
		{
			this.taskView.mouseEnabled=false;
			panelBase = new PanelBase(taskView, taskView.width+8, taskView.height+12);
			panelBase.name = "TaskView";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_task_med_taski_han_1" ]);         //任 务
			(this.taskView.btn_cancelTask as SimpleButton).addEventListener(MouseEvent.CLICK,onCancelTaskHandler); 
			(this.taskView.btn_isFollowTask as SimpleButton).addEventListener(MouseEvent.CLICK,onIsFollowClick);
			(this.taskView.mc_isShowFollow as MovieClip).addEventListener(MouseEvent.CLICK,onShowFollowHandler);
			(this.taskView.mc_isShowFollow as MovieClip).gotoAndStop(2);
			(this.taskView.txt_isFollowTask as TextField).mouseEnabled=false;
			(this.taskView.mc_page1 as MovieClip).addEventListener(MouseEvent.CLICK,onPageClick);
			(this.taskView.mc_page2 as MovieClip).addEventListener(MouseEvent.CLICK,onPageClick);
			(this.taskView.mc_page1 as MovieClip).gotoAndStop(1);
			(this.taskView.mc_page2 as MovieClip).gotoAndStop(2);
				
			(this.taskView.btn_cancelTask as SimpleButton).visible=false;
			(this.taskView.btn_isFollowTask as SimpleButton).visible=false;	
				
			this.taskTree=new UITree();//当前任务
			this.taskTree.addEventListener(TreeEvent.CHANGE_SELECTED,onCurSelectedChange);
			this.scrollPanel=new UIScrollPane(taskTree);
			this.scrollPanel.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.scrollPanel.width = 210;
			this.scrollPanel.height = 362;

					
			this.accTaskTree=new UITree();//可选任务
			this.accTaskTree.addEventListener(TreeEvent.CHANGE_SELECTED,onAccSelectedChange);
			this.accScrollPanel=new UIScrollPane(accTaskTree);
			this.accScrollPanel.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.accScrollPanel.width = 210;
			this.accScrollPanel.height = 362;

					
			this.acceptTaskPanel=new AcceptTaskInfoPanel();//可选任务详细

			this.acceptTaskPanel.x = 225;
			this.acceptTaskPanel.y = 26;
					
			this.taskDetailPanel=new TaskInfoPanel();//当前任务详细
			this.taskView.addChild(this.taskDetailPanel);

			this.taskDetailPanel.x = 225;
			this.taskDetailPanel.y = 26;		
		}
		
		/**
		 *新任务界面加载 
		 * 
		 */
		private function initNewTask():void
		{
			this.loader = new Loader(); 
			this.loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoadComplete);
			this.loader.load(new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewTask.swf"));
		}
		
		private function onLoadComplete(e:Event):void 
		{
			var domain:ApplicationDomain = this.loader.contentLoaderInfo.applicationDomain;
			var newTaskView:MovieClip = new (domain.getDefinition("NewTaskView"))();
			this.setViewComponent(newTaskView);
			init();//加载资源后初始化
			loader.contentLoaderInfo.removeEventListener(Event.COMPLETE,onLoadComplete);
			loader.unload();
			loader = null;
		}
		
		private function refreshAccTree():void{
			this.accTaskTree.dataProvider=[];  //清除
			this.setAcc();                     //过滤
			//去掉已经接的
			for (var tid:* in GameCommonData.TaskInfoDic){
				var task:TaskInfoStruct=GameCommonData.TaskInfoDic[tid] as TaskInfoStruct;
				if(task.status!=0){
					this.clearTaskChainInAcc(task.id);
				}	
			}
			this.accTaskTree.refresh();
		}
		
		/**
		 * 
		 * @param id
		 * 
		 */
		protected function clearTaskChainInAcc(id:uint):void{
			var tId:uint=id;
				tId=Math.floor(tId/100)*100;
				for(var i:uint=0;i<100;i++){
					var t:uint=tId+i;
					if(GameCommonData.TaskInfoDic[t]!=null){
						var accTask:TaskInfoStruct=GameCommonData.TaskInfoDic[t] as TaskInfoStruct;
						var taskGroup:TaskGroupStruct=this.accTaskTree.searchGroupByDes(accTask.taskType,this.accTaskTree.dataProvider);
						if(taskGroup==null)continue;
						delete taskGroup.taskDic[accTask.id];
						this.acceptTaskPanel.removeInfo(accTask.id);
						sendNotification(TaskCommandList.ACCTASK_SYNC_REMOVE, accTask.id);
				}
			}
		}
		
		protected function onPageClick(e:MouseEvent):void{
			//当前任务
			if(e.currentTarget===this.taskView.mc_page1){
				this.setSelectedPage(0);
			//可接任务	
			}else{
				this.setSelectedPage(1);
			}
		}
		
		/**
		 * 设置选择当前页签并处理相关内容的显示  
		 * @param type
		 * 
		 */		
		protected function setSelectedPage(type:uint):void{
			if(type==0){
				this.isCurrentPage=true;
				this.taskView.mc_page1.gotoAndStop(1);
				this.taskView.mc_page2.gotoAndStop(2);
				this.scrollPanel.visible=true;
				this.accScrollPanel.visible=false;
				if(this.acceptTaskPanel.stage!=null){
					this.taskView.removeChild(this.acceptTaskPanel);
				}
				this.taskView.addChild(this.taskDetailPanel);
				this.scrollPanel.refresh();
				if(GameCommonData.wordVersion == 2)
				{
					removeFbIcon();
				}
			}else if(type==1){
				if(!this.doneFirst){  
			 		this.doneFirst=true;
			 		this.setAcc();
				}
				this.isCurrentPage=false;
				this.taskView.mc_page1.gotoAndStop(2);
				this.taskView.mc_page2.gotoAndStop(1);
				this.scrollPanel.visible=false;
				this.accScrollPanel.visible=true;
				(this.taskView.btn_cancelTask as SimpleButton).visible=false;
				(this.taskView.btn_isFollowTask as SimpleButton).visible=false;
				if(this.taskDetailPanel.stage!=null){
					this.taskView.removeChild(this.taskDetailPanel);
				}
				this.taskView.addChild(this.acceptTaskPanel);
				this.accScrollPanel.refresh();
				if(GameCommonData.wordVersion == 2)
				{
					addFbIcon();
				}
			}		
		}
		
		/**
		 *增加fb图标
		 * 
		 */		
		protected function addFbIcon():void{
			var selfInfoMed:SelfInfoMediator = facade.retrieveMediator(SelfInfoMediator.NAME) as SelfInfoMediator;
			if(!_fbIcon)
			{
				if(selfInfoMed.getFBBindedState())  //fb账号已经绑定
				{
					_fbIcon = selfInfoMed.getFBHasBindedIcon();
					_fbIcon.x = 60;
					_fbIcon.y = 300;
				}	
				else	//fb账号已经没有绑定
				{
					_fbIcon = selfInfoMed.getFBNoBindedIcon();
					_fbIcon.x = 190;
					_fbIcon.y = 300;
				}
				_fbIcon.addEventListener(MouseEvent.CLICK,onFbIconClick);
				taskView.addChild(_fbIcon);
			}
			else
			{
				taskView.addChildAt(_fbIcon,taskView.numChildren - 1);
			}
		}
		/**
		 *移除fb图标 
		 * 
		 */		
		protected function removeFbIcon():void{
			if(_fbIcon)
			{
				if(this.taskView.contains(_fbIcon))
				{
					this.taskView.removeChild(_fbIcon);
				}
				_fbIcon.removeEventListener(MouseEvent.CLICK,onFbIconClick);
				_fbIcon = null;
			}
		}
	
		protected function onFbIconClick(me:MouseEvent):void{
			/* {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"敬請期待", color:0xffff00});  // FB已經綁定
				return;
			} */
			var selfInfoMed:SelfInfoMediator = facade.retrieveMediator(SelfInfoMediator.NAME) as SelfInfoMediator;
			if(selfInfoMed.getFBBindedState())	//已经绑定fb
			{
				selfInfoMed.sendAwardFormTaskPanel(_fbNeedTaskId);
			}
			else	//没有绑定fb
			{
				selfInfoMed.isSendFbAward(0,10);
			}
		}
		protected function onCurSelectedChange(e:TreeEvent):void{
			
			this.setStatusBySelected(e.id);
			
			if(e.id==0){
				this.taskDetailPanel.clearContent();
			}else{
				this.taskDetailPanel.update(e.id);
			}
			this.curID=e.id;
		}
		
		
		protected function setStatusBySelected(id:uint):void{
			if(id==0){
				(this.taskView.btn_cancelTask as SimpleButton).visible=false;
				(this.taskView.btn_isFollowTask as SimpleButton).visible=false;
			}else{
				var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id] as TaskInfoStruct;
				if(this.isCurrentPage){
					(this.taskView.btn_cancelTask as SimpleButton).visible=true;
					(this.taskView.btn_isFollowTask as SimpleButton).visible=true;
				}
				if(taskInfo.isFollow==1){
					(this.taskView.txt_isFollowTask as TextField).text=GameCommonData.wordDic[ "mod_task_med_taskm_set_1" ];         //取消追踪
				}else{
					(this.taskView.txt_isFollowTask as TextField).text=GameCommonData.wordDic[ "mod_task_med_taskm_set_2" ];         //追踪任务
				}
			}
		}
		
		protected function onAccSelectedChange(e:TreeEvent):void{
			if(e.id==0){
				this.acceptTaskPanel.clearContent();
			}else{
				this.acceptTaskPanel.update(e.id);
				_fbNeedTaskId = e.id;
			}
		}
		
		protected function onIsFollowClick(e:MouseEvent):void{
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[this.curID] as TaskInfoStruct;
			if(taskInfo==null)return;
			var followText:String=(this.taskView.txt_isFollowTask as TextField).text;
			if(followText==GameCommonData.wordDic[ "mod_task_med_taskm_set_1" ]){                //取消追踪
				taskInfo.isFollow=0;
				sendNotification(TaskCommandList.REMOVE_TASK_FOLLOW,taskInfo);
				(this.taskView.txt_isFollowTask as TextField).text=GameCommonData.wordDic[ "mod_task_med_taskm_set_2" ];               //追踪任务
			}else{
				taskInfo.isFollow=1;
				sendNotification(TaskCommandList.ADD_TASK_FOLLOW,taskInfo);
				(this.taskView.txt_isFollowTask as TextField).text=GameCommonData.wordDic[ "mod_task_med_taskm_set_1" ];               //取消追踪
			}
			this.taskTree.refresh();
			
		}
		
		protected function onShowFollowHandler(e:MouseEvent):void{
			//显示
			if((e.target as MovieClip).currentFrame==2){
				(e.target as MovieClip).gotoAndStop(1);
				 sendNotification(TaskCommandList.HIDE_TASKFOLLOW_UI);
			//隐藏	
			}else if((e.target as MovieClip).currentFrame==1){
				(e.target as MovieClip).gotoAndStop(2);
				sendNotification(TaskCommandList.SHOW_TASKFOLLOW_UI);
			}
		}
		
		protected function setShowFollow(type:uint):void{
			if(type==1){
				(this.taskView.mc_isShowFollow as MovieClip).gotoAndStop(1);
				 sendNotification(TaskCommandList.HIDE_TASKFOLLOW_UI);
			}else if(type==2){
				(this.taskView.mc_isShowFollow as MovieClip).gotoAndStop(2);
				sendNotification(TaskCommandList.SHOW_TASKFOLLOW_UI);
			}
		}
		
		protected function onCancelTaskHandler(e:MouseEvent):void{
			if(!this.isCurrentPage)return;
			if(!this.isFirstCancel)this.isFirstCancel=true;
			if(this.curID==0)return;
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[this.curID];
			if(taskInfo==null)return;
//			sendNotification(TaskCommandList.REMOVE_TASK_FOLLOW,taskInfo);
			if((this.taskView.btn_cancelTask as SimpleButton).visible == true)
			{
				(this.taskView.btn_cancelTask as SimpleButton).visible = false;
				(this.taskView.btn_isFollowTask as SimpleButton).visible = false;
			}
			sendNotification(TaskCommandList.RECALL_TASK,{taskID:this.curID,type:242});
		}
			
		protected function onExpandClick(e:TreeEvent):void{
			this.scrollPanel.refresh();	
		}
		
		/**
		 * 刚登录时，设置初始化可接任务(去掉打上掩码的) 
		 * 
		 */		
		protected function setAcc():void{
			for (var tid:* in GameCommonData.TaskInfoDic){
				if(tid>10001000)continue;            //去掉跑环任务
				if(tid>3500 && tid<3600)continue;    //去掉师门任务
				var task:TaskInfoStruct=GameCommonData.TaskInfoDic[tid] as TaskInfoStruct;
					var low:uint
					if(task.type==1){
						if(task.maskIndex>31){
							low=GameCommonData.MaskHi & Math.pow(2,uint(task.maskIndex-32));
						}else{
							low=GameCommonData.MaskLow & Math.pow(2,uint(task.maskIndex));
						}
					}
					if(task.type==0){
						if(task.maskIndex>31){
							low=GameCommonData.DayMaskHi & Math.pow(2,uint(task.maskIndex-32));
						}else{
							low=GameCommonData.DayMaskLow & Math.pow(2,uint(task.maskIndex));
						}		
					}
					if(low==0 && GameCommonData.Player.Role.Level>=task.taskLevel){         //可接任务
						if(task.isChain==1){                                                //是链
							if(task.step==1){                                               //第一步加入可接列表
								sendNotification(TaskCommandList.ADD_ACCEPT_TASK,tid);
							}
						}else{                                                              //不是链
							 sendNotification(TaskCommandList.ADD_ACCEPT_TASK,tid);         //直接加入
						}		
					}	
			}
		}
		/**
		 *  
		 * @param type 1：接收一个任务， 2：取消一个任务 ，3：追踪一个任务 4：取消一个任务的追踪 5:完成一个任务
		 * 				11:添加一个可接任务   12：取消一个可接任务
		 * 
		 */		
		protected function updateTaskTree(type:uint,id:uint):void{
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id] as TaskInfoStruct;
			if(taskInfo==null)return;
			switch (type){
				case 1:
				 	if(!this.doneFirst){
				 		this.doneFirst=true;
				 		this.setAcc();    //初始可接任务表（如果是链将第一步加入）
				 	}
				 	this.taskTree.addTask(taskInfo);
					// this.taskTotal++;  因为服务器有时会发来奇怪的通知，导致 taskTotal 不可信，所以 taskTotal 要进行绝对统计，保证和列表同步。  -zhao 2011/7/5
					this.taskTotal = countTaskTotal();
					if(taskInfo.isFollow==1){
						sendNotification(TaskCommandList.ADD_TASK_FOLLOW,taskInfo);
					}
					this.clearTaskChainInAcc(taskInfo.id);  //每接一个就清掉该链中的任务
					sendNotification(TaskCommandList.UPDATE_TASK_TOTAL,this.taskTotal);
					this.taskTree.setSelected(taskInfo.id);
					break;
					
				case 2:
					this.taskTree.delTask(taskInfo.id);
					if(this.isFirstCancel){
						// this.taskTotal--;  因为服务器有时会发来奇怪的通知，导致 taskTotal 不可信，所以 taskTotal 要进行绝对统计，保证和列表同步。  -zhao 2011/7/5
						this.taskTotal = countTaskTotal();
					}
					sendNotification(TaskCommandList.UPDATE_TASK_TOTAL,this.taskTotal);
					sendNotification(TaskCommandList.REMOVE_TASK_FOLLOW,taskInfo); //让跟踪也取消该任务
					this.taskDetailPanel.removeInfo(taskInfo.id);
					break;
					
				case 3:
					taskInfo.isFollow=1;
					this.taskTree.refresh();
					sendNotification(TaskCommandList.ADD_TASK_FOLLOW,taskInfo);
					break;
					
				case 4:
					taskInfo.isFollow=0;
					this.taskTree.refresh();
					sendNotification(TaskCommandList.REMOVE_TASK_FOLLOW,taskInfo);
					break;
					
				case 5:	
					this.taskTree.delTask(taskInfo.id);		
					// this.taskTotal--;  因为服务器有时会发来奇怪的通知，导致 taskTotal 不可信，所以 taskTotal 要进行绝对统计，保证和列表同步。  -zhao 2011/7/5
					this.taskTotal = countTaskTotal();
					sendNotification(TaskCommandList.UPDATE_TASK_TOTAL,this.taskTotal);
					sendNotification(TaskCommandList.REMOVE_TASK_FOLLOW,taskInfo); //让跟踪也取消该任务	
					this.taskDetailPanel.removeInfo(taskInfo.id);
					break;		
								
				case 11:
					this.accTaskTree.addTask(taskInfo);
					this.accTaskTree.setSelected(taskInfo.id);
					sendNotification(TaskCommandList.ACCTASK_SYNC_ADD, taskInfo.id);
					break;	
					
				case 12:
					this.accTaskTree.delTask(id);
					this.acceptTaskPanel.removeInfo(id);
					sendNotification(TaskCommandList.ACCTASK_SYNC_REMOVE, id);
					break;					
			}
			
			if(this.isCurrentPage){
				this.scrollPanel.refresh();
			}else{
				this.accScrollPanel.refresh();
			}
		}
		
		protected function countTaskTotal():int
		{
			var count:int = 0;
			var l:int = this.taskTree.numChildren;
			
			for (var i:int = 0; i < l; i ++)
			{
				if (this.taskTree.getChildAt(i) is TreeCellRenderer)
				{
					++ count;
				}
			}
			
			return count;
		}
		
		
		/**
		 * 查找某个分组 
		 * @param str
		 * @param groups
		 * @return 
		 * 
		 */		
		public function searchGroupByDes(str:String,groups:Array):TaskGroupStruct{
			for each(var t:TaskGroupStruct in groups){
				if(t.des==str){
					return t;
				}
			}
			return null;
		}
		
		
		private function showView():void
		{
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
//			panelBase.x = UIConstData.DefaultPos2.x-30;
//			panelBase.y = UIConstData.DefaultPos2.y;
		}
		
		
		private function panelCloseHandler(event:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				dataProxy.TaskIsOpen = false;
			}
		}
	}
}