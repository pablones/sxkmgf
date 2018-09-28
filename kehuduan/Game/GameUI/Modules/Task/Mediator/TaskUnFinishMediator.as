package GameUI.Modules.Task.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TaskUnFinishMediator extends Mediator
	{
		public static const NAME:String="TaskUnFinishMediator";
		
		private var basePanel:PanelBase;
		private var taskUnFinishDes:TaskText;
		private var taskInfo:TaskInfoStruct;
		
		
		public function TaskUnFinishMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get viewUi():MovieClip{
			return this.viewComponent as MovieClip;	
		}
		
		public override function listNotificationInterests():Array{
			return [
				EventList.ENTERMAPCOMPLETE, 
//				EventList.INITVIEW,
				EventList.SHOW_UNFINISH_TASK,
				EventList.CLOSE_NPC_ALL_PANEL
			];
		}
	
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.ENTERMAPCOMPLETE:
					sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"NPCChat"});
					this.viewUi.mouseEnabled=false;
					this.basePanel=new PanelBase(this.viewUi,this.viewUi.width+8,this.viewUi.height+12);
					this.basePanel.x=UIConstData.DefaultPos1.x;
					this.basePanel.y=UIConstData.DefaultPos1.y;
					this.basePanel.addEventListener(Event.CLOSE,onCloseBasePanelHandler);
					
				break;
				case EventList.SHOW_UNFINISH_TASK:
					taskInfo=GameCommonData.TaskInfoDic[notification.getBody()["taskId"]];
					if(taskInfo!=null){
						initData();
						GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
						this.basePanel.x=UIConstData.DefaultPos1.x;
						this.basePanel.y=UIConstData.DefaultPos1.y;
					}
				break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					this.onCloseBasePanelHandler(null);
				break;
			}
		}
		
		private function initData():void{
			if(this.taskUnFinishDes!=null && this.taskUnFinishDes.parent!=null){
				this.taskUnFinishDes.parent.removeChild(this.taskUnFinishDes);
			}
			this.taskUnFinishDes=new TaskText();
			this.taskUnFinishDes.x=20;
			this.taskUnFinishDes.y=20;
			this.viewUi.addChild(this.taskUnFinishDes);
			this.taskUnFinishDes.tfText=this.taskInfo.taskUnFinish;
			this.basePanel.SetTitleTxt(this.taskInfo.taskName);
		}
		
		/**
		 * 关闭 
		 * @param e
		 * 
		 */		
		private function onCloseBasePanelHandler(e:Event):void{
			if(this.basePanel.parent!=null){
				this.basePanel.parent.removeChild(this.basePanel);
			}
		}
		
	}
}