package GameUI.Modules.Task.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.NewerHelp.Mediator.NewerHelpUIMediaror;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Mediator.PetChooseTaskMediator;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Task.Model.TaskProxy;
	import GameUI.Modules.Task.View.TaskPanel;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 *  接收与提交任务
	 * @author felix
	 * 
	 */	
	public class TaskInfoMediator extends Mediator
	{
		public static const NAME:String="TaskInfoMediator"; 
		public static const DEFAULT_POS:Point=new Point(80,58);
		protected var panelBase:PanelBase;
		
		protected var contentPanel:TaskPanel;
		protected var id:uint;
		
		public function TaskInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		
		public function get taskInfoView():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				TaskCommandList.SHOW_TASKINFO_UI,
				TaskCommandList.HIDE_TASKINFO_UI,
				TaskCommandList.CLOSE_TASK_PANEL,
				EventList.CLOSE_NPC_ALL_PANEL,
				TaskCommandList.SET_TASKINFO_DRAG
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW :
					facade.registerMediator(new TaskUnFinishMediator());
					facade.registerMediator(new TaskExpandMediator());
					sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP, mediator:this, name:"NPCTask"});
					this.taskInfoView.mouseEnabled=false;
					this.panelBase=new PanelBase(this.taskInfoView,this.taskInfoView.width+8,this.taskInfoView.height+12);
					this.panelBase.addEventListener(Event.CLOSE,onPanelClose);
					this.panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_task_med_taski_han_1" ]);      //任 务
//					this.panelBase.x=DEFAULT_POS.x;
//					this.panelBase.y=DEFAULT_POS.y;
					var accept:SimpleButton=this.taskInfoView.btn_Accept as SimpleButton;
					accept.addEventListener(MouseEvent.CLICK,onAcceptHandler);
					break;
				case TaskCommandList.SHOW_TASKINFO_UI:
					if(this.contentPanel!=null && this.taskInfoView.contains(this.contentPanel)){
						this.taskInfoView.removeChild(this.contentPanel);
					}
					this.id=notification.getBody()["id"];
					var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[notification.getBody()["id"]];
					if(taskInfo==null)return;	
					this.panelBase.SetTitleTxt(taskInfo.taskName);
					if(taskInfo.status==0){
						if(taskInfo.id==301){ 
							var btn:SimpleButton=this.taskInfoView.btn_Accept as SimpleButton;
							var point:Point = this.taskInfoView.localToGlobal(new Point(btn.x, btn.y));
							NewerHelpData.point = point;
							sendNotification(NewerHelpEvent.OPEN_TASK_ACCEPT_NOTICE_NEWER_HELP);
						}
						
						(this.taskInfoView.txt_accept as TextField).text=GameCommonData.wordDic[ "mod_task_med_taski_han_2" ];      //接受任务
						(this.taskInfoView.txt_accept as TextField).mouseEnabled=false;
					}else{
						if(TaskProxy.getInstance().taskAnimalDic[id]!=null){
							if(PetPropConstData.petChooseTaskIsOpen) sendNotification(PetEvent.CLOSE_PET_CHOICE_PANEL_SINGLE);
							facade.registerMediator(new PetChooseTaskMediator());
							sendNotification(PetEvent.OPEN_PET_CHOICE_PANEL_SINGLE,TaskProxy.getInstance().taskAnimalDic[id]);
						}
						(this.taskInfoView.txt_accept as TextField).text=GameCommonData.wordDic[ "mod_task_med_taski_han_3" ];         //完成任务
						(this.taskInfoView.txt_accept as TextField).mouseEnabled=false;
					}
					
					this.contentPanel=new TaskPanel(notification.getBody()["id"]);
					this.taskInfoView.addChild(contentPanel);
					this.contentPanel.x=12;
					this.contentPanel.y=10;
					if( GameCommonData.fullScreen == 2 )
					{
						this.panelBase.x=DEFAULT_POS.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					    this.panelBase.y=DEFAULT_POS.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;	
					}else{
						this.panelBase.x=DEFAULT_POS.x;
						this.panelBase.y=DEFAULT_POS.y;
					}
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
//					this.panelBase.x=DEFAULT_POS.x;
//					this.panelBase.y=DEFAULT_POS.y;
					break;
				case TaskCommandList.CLOSE_TASK_PANEL:
					onPanelClose(null);
					break;	
				case EventList.CLOSE_NPC_ALL_PANEL:
					onPanelClose(null);
					break;	
				case TaskCommandList.SET_TASKINFO_DRAG:           //NPC任务禁用拖动
					if(panelBase)
						panelBase.IsDrag = false;
					break;
			}
		}
		
		protected function onPanelClose(e:Event):void{
			if(PetPropConstData.petChooseTaskIsOpen){
				sendNotification(PetEvent.CLOSE_PET_CHOICE_PANEL_SINGLE);
			}
			if(this.id==301){
				sendNotification(NewerHelpEvent.CLOSE_TASK_SHOW_NOTICE_NEWER_HELP);
			}
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase)){
				GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			}
		}
		
		protected function onAcceptHandler(e:MouseEvent):void{
			var str:String=(this.taskInfoView.txt_accept as TextField).text
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id];
			if(str==GameCommonData.wordDic[ "mod_task_med_taski_han_2" ]){        //接受任务
				if(taskInfo && (taskInfo.taskExpandType & 2)>0){
					sendNotification(TaskCommandList.SHOW_VIT_PANEL,id);
					return;
				}
				sendNotification(TaskCommandList.RECALL_TASK,{taskID:id,type:241});
				if(SelfInfoMediator.isFBOpen)
				{
					if(this.id == 4401)	 //显示台服fb绑定任务接受
					{
						if(!SelfInfoMediator.isShowFBNewerHelp || SelfInfoMediator.isShowFBNewerHelp == 2)
						{
							SelfInfoMediator.isShowFBNewerHelp = 1;
							(facade.retrieveMediator(NewerHelpUIMediaror.NAME) as NewerHelpUIMediaror).knownForFbBinded();
						}
					} 
				}
			}
			if(str==GameCommonData.wordDic[ "mod_task_med_taski_han_3" ]){        //完成任务
				if(TaskProxy.getInstance().taskAnimalDic[id]){
					if(PetPropConstData.PetIdSelectedChooseTask>0) {
						var chooseId:uint=PetPropConstData.PetIdSelectedChooseTask;
						sendNotification(PetEvent.CLOSE_PET_CHOICE_PANEL_SINGLE);
						sendNotification(TaskCommandList.RECALL_TASK,{taskID:id,type:243,petId:chooseId});
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taski_han_4" ], color:0xffff00});         //请先选择宠物
						return;
					}
				}else{
					if(taskInfo && (taskInfo.taskExpandType & 4)>0){
						sendNotification(TaskCommandList.SHOW_TASKEXPAND_PANEL,id);
						return ;	
					}
					sendNotification(TaskCommandList.RECALL_TASK,{taskID:id,type:243});
				}
				if(contentPanel.selectedGoodType!=-1){
					var obj:Object={type:1010};
					var data:Array=[0,262,0,0,0,contentPanel.selectedGoodType,0,0,"0"];
					obj.data=data;
					PlayerActionSend.PlayerAction(obj);
				}
				//点击“完成”任务按钮时，通知新手指导
				if(NewerHelpData.newerHelpIsOpen)//新手指导开启
					sendNotification(NewerHelpEvent.TASK_COMPLETE_NOTICE_NEWER_HELP,{id:id});
					//台服facebook成就
					if(GameCommonData.wordVersion == 2)
					{
						judgeIsFbAward(id);
					}
			}
			this.onPanelClose(null);
		}
		
		/**
		 * fb成就 
		 * @param taskId 任務id
		 * 
		 */		
		protected function judgeIsFbAward(taskId:int):void{
			var type:int;
			if(taskId == 302)	//第一把武器
			{
				type = 4; 
			}
			else if(taskId == 303)	//第一次戰鬥
			{
				type = 5;
			}
			else if(taskId == 305)	//第一隻寵物
			{
				type = 6;
			}
			else if(taskId == 308)	//第一次使用藥物
			{
				type = 7;
			}
			else if(taskId == 102)	//第一次加入門派
			{
				type = 8;
			}
			else if(taskId == 311)	//一代名俠
			{
				type = 9;
			}
			if(type > 0)
			{
				sendNotification(TaskCommandList.SEND_FB_AWARD,type);
			}
		}
		
		protected function isInAnimalDic():Boolean{
			return false;
		}
		
		/**
		 * 关闭 
		 * @param e
		 * 
		 */		
		protected function onClickHanler(e:MouseEvent):void{
			this.onPanelClose(null);
		}
		
	}
}