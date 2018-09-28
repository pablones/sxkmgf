package GameUI.Modules.Task.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Equipment.ui.UIList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Task.View.VitListCell;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TaskVitMediator extends Mediator
	{
		public static const NAME:String="TaskVitMediator";
		protected var basePanel:PanelBase;
		protected var selectedIndex:uint;
		protected var taskId:uint;
		protected var vitList:UIList;
		
		
		public function TaskVitMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get view():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		
		public override function listNotificationInterests():Array{
			return [
				TaskCommandList.SHOW_VIT_PANEL	
			];
		}
		
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case TaskCommandList.SHOW_VIT_PANEL:
					if(this.basePanel==null){
						facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"VitPaneldf"});
						this.basePanel=new PanelBase(this.view,this.view.width,this.view.height);
						this.basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_task_med_taskv_han_1" ]);           //精力选择
						this.basePanel.addEventListener(Event.CLOSE,onPanelBaseClose);
						this.basePanel.x=200;
						this.basePanel.y=200;
						this.init();
					}
					this.taskId=uint(notification.getBody());	
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
					this.changeDes();					
					break;			
			}
		}
		
		
		/**
		 * 初始化操作 
		 * 
		 */		
		protected function init():void{
			(this.view.txt_vit as TextField).mouseEnabled=false;
			(this.view.txt_vit as TextField).text="";
			
			this.vitList=new UIList(176,50,-2);
			this.vitList.rendererClass=VitListCell;
			this.vitList.mouseChildren=false;
			this.vitList.mouseEnabled=false;
			this.view.addChild(this.vitList);
			this.vitList.x=125;
			this.vitList.y=75;	
			
			(this.view.mc_1 as MovieClip).gotoAndStop(2);	
			(this.view.mc_2 as MovieClip).gotoAndStop(1);	
			(this.view.mc_3 as MovieClip).gotoAndStop(1);	
			(this.view.mc_4 as MovieClip).gotoAndStop(1);
			
			(this.view.mc_1 as MovieClip).addEventListener(MouseEvent.CLICK,onRadioBoxClick);
			(this.view.mc_2 as MovieClip).addEventListener(MouseEvent.CLICK,onRadioBoxClick);
			(this.view.mc_3 as MovieClip).addEventListener(MouseEvent.CLICK,onRadioBoxClick);
			(this.view.mc_4 as MovieClip).addEventListener(MouseEvent.CLICK,onRadioBoxClick);
			
			(this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
			
		}
		
		protected function onCommitClick(e:MouseEvent):void{
			//todo提交按受任务
			sendNotification(TaskCommandList.RECALL_TASK,{taskID:this.taskId,type:241,petId:this.selectedIndex-1});
			facade.sendNotification(TaskCommandList.CLOSE_TASK_PANEL);
			this.onPanelBaseClose(null);
		}
		
		
		/**
		 * checkBox点击 
		 * @param e
		 * 
		 */		
		protected function onRadioBoxClick(e:MouseEvent):void{
			if(e.target===this.view.mc_1){
				this.setSelectedIndex(1);
			}else if(e.target===this.view.mc_2){
				this.setSelectedIndex(2);
			}else if(e.target===this.view.mc_3){
				this.setSelectedIndex(3);
			}else if(e.target===this.view.mc_4){
				this.setSelectedIndex(4);
			}
		}
		
		/**
		 *  
		 * @param index
		 * 
		 */		
		protected function setSelectedIndex(index:uint):void{
			var vit:uint=GameCommonData.Player.Role.Vit;
			if(index==2 && vit<15){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taskv_set_1" ], color:0xffff00});            //你当前的精力值不够
				return;
			}
			if(index==3 && vit<25){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taskv_set_1" ], color:0xffff00});             //你当前的精力值不够
				return;
			}
			if(index==4 && vit<35){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taskv_set_1" ], color:0xffff00});              //你当前的精力值不够
				return;
			}
				
			this.selectedIndex=index;
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[this.taskId];
			var expArr:Array=taskInfo.taskRandBaseExp.split("_");
			var moneyArr:Array=taskInfo.taskRandBaseMoney.split("_");
			var listArr:Array=[];
			if(this.selectedIndex>1){
				if(expArr.length==3){
					listArr.push(expArr[this.selectedIndex-2]);
				}
				if(moneyArr.length==3){
					listArr.push(moneyArr[this.selectedIndex-2]);
				}
			}
			
			this.vitList.dataPro=listArr;
			(this.view.mc_1 as MovieClip).gotoAndStop(1);	
			(this.view.mc_2 as MovieClip).gotoAndStop(1);	
			(this.view.mc_3 as MovieClip).gotoAndStop(1);	
			(this.view.mc_4 as MovieClip).gotoAndStop(1);	
			this.view["mc_"+index].gotoAndStop(2);
			this.changeDes();
		}
		
		/**
		 * 改变描述 
		 * 
		 */		
		protected function changeDes():void{
			(this.view.txt_vit as TextField).text=String(GameCommonData.Player.Role.Vit);	
		}
		
		
		 
		/**
		 * 关闭面板 
		 * @param e
		 * 
		 */		
		protected function onPanelBaseClose(e:Event):void{
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
		}
		
		
	}
}