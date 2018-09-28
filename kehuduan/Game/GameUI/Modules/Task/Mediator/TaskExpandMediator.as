package GameUI.Modules.Task.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TaskExpandMediator extends Mediator
	{
		
		public static const NAME:String="TaskExpandMediator";
		protected var basePanel:PanelBase;
		/**
		 * 捐出的装备的图标 
		 */		
		protected var useItem:UseItem;
		protected var dataProxy:DataProxy;
		/**
		 * 任务ID号 
		 */		
		protected var id:uint;     
		
		public function TaskExpandMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		/**
		 *  
		 * @return 
		 * 
		 */		
		public function get view():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		
		
		public override function listNotificationInterests():Array{
			return [
				TaskCommandList.SHOW_TASKEXPAND_PANEL,
				TaskCommandList.RETURN_EQUIP
				];
		}
		
		
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case TaskCommandList.SHOW_TASKEXPAND_PANEL:
					this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					
					if(!this.basePanel){
						this.init();
						this.addEventListeners();
					}
					id=int(notification.getBody());
					var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id];
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
					this.dataProxy.taskEquipReturnIsOpen=true;
					break;
				case TaskCommandList.RETURN_EQUIP:
					selectEquip(notification.getBody()); 
					break;
			}
		}
		
		
		/**
		 * 初始化 
		 * 
		 */		
		protected function init():void{
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"TaskEquipReturn"});
			this.basePanel=new PanelBase(this.view,this.view.width-5,this.view.height+12);
			this.basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_task_med_taske_init_1" ]);    //装备捐献
			this.basePanel.addEventListener(Event.CLOSE,onClosePanel);
			this.basePanel.x=300;
			this.basePanel.y=50; 
		}
		
		
		/**
		 * 事件监听
		 **/ 
		protected function addEventListeners():void
		{
			this.view.mc_contributeCell.addEventListener(MouseEvent.CLICK,clickHandler);
			this.view.btn_sure.addEventListener(MouseEvent.CLICK,clickHandler);
			this.view.btn_no.addEventListener(MouseEvent.CLICK,clickHandler);
		}
		 
		protected function clickHandler(me:MouseEvent):void{
			var name:String=(me.currentTarget as DisplayObject).name;
			switch(name){
				case "mc_contributeCell":
					if(this.useItem!=null && this.view.mc_contributeCell.contains(this.useItem)){
						sendNotification(EventList.BAGITEMUNLOCK, this.useItem.Id);
						this.view.mc_contributeCell.removeChild(this.useItem);
						this.useItem=null;
					}
					break;
				case "btn_sure":
					sureHander();
					break;
				case "btn_no":
					notSureHandler();
					break;
			}
		}
		
		/**
         *
         * @author lh
         *
         *  从背包选择装备到回收面板
         * @data: 2011-1-17
		 */
		protected function selectEquip(data:Object):void{
			if(this.useItem!=null && this.view.mc_contributeCell.contains(this.useItem)){
				sendNotification(EventList.BAGITEMUNLOCK, this.useItem.Id);
				this.view.mc_contributeCell.removeChild(this.useItem);
				this.useItem=null;
			}
			var color:uint = IntroConst.ItemInfo[data.id].color;
			if(color>1){
				if(UIConstData.getItem(data.type).Level+20>=GameCommonData.Player.Role.Level){
					
					BagData.SelectedItem.Item.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true; 
					this.useItem=new UseItem(0,data.type,null);
					this.useItem.Id=data.id;
					useItem.x=2;
					useItem.y=2;
					(this.view.mc_contributeCell as DisplayObjectContainer).addChild(useItem);
					this.showExplain(color);
				}else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taske_sel_1" ], color:0xffff00});     //最低可提交小于人物等级20级的装备
				}
			}else{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taske_sel_2" ], color:0xffff00});     //捐出的装备必须是绿色或品质好于绿色的装备
			} 
		}
		
		
		/**
         * @author lh
         * 
         *  回收说明
         * @data: 2011-1-17
		 */
		 protected function showExplain(color:uint):void{
		 	var str:String;
		 	if(color==2){
		 		str='<font color="#00ff00">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_1" ]+'</font><font color="#e2cca5">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_2" ]+'<font color="#ffff00">1--1.5</font>'+GameCommonData.wordDic[ "mod_task_med_taske_sho_3" ]+'</font>';  //绿色      装备，获得      倍奖励。
		 	}else if(color==3){
		 		str='<font color="#0099ff">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_4" ]+'</font><font color="#e2cca5">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_2" ]+'<font color="#ffff00">1.5--2</font>'+GameCommonData.wordDic[ "mod_task_med_taske_sho_3" ]+'</font>';  //蓝色      装备，获得      倍奖励。
		 	}else if(color==4){
		 		str='<font color="#ff00ff">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_5" ]+'</font><font color="#e2cca5">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_2" ]+'<font color="#ffff00">2--2.5</font>'+GameCommonData.wordDic[ "mod_task_med_taske_sho_3" ]+'</font>';  //紫色      装备，获得      倍奖励。
		 	}else if(color==5){
		 		str='<font color="#ff9900">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_6" ]+'</font><font color="#e2cca5">'+GameCommonData.wordDic[ "mod_task_med_taske_sho_2" ]+'<font color="#ffff00">2.5--3</font>'+GameCommonData.wordDic[ "mod_task_med_taske_sho_3" ]+'</font>';  //橙色      装备，获得      倍奖励。
		 	}
		 	(this.view.txt_explain as TextField).htmlText=str;
		 }
		
		
		/**
         * @author lh
         *  确定回收(提交要回收的装备。)
         * @date: 2011-1-17
		 */
		protected function sureHander():void{
			if(this.useItem){
				sendNotification(TaskCommandList.RECALL_TASK,{taskID:id,type:243,equipId:this.useItem.Id});
				sendNotification(TaskCommandList.CLOSE_TASK_PANEL);
				this.onClosePanel(null);
			}else{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_task_med_taske_sur_1" ], color:0xffff00});     //请放入你要捐出的装备
			}
			
		}
		
		
		/**
         * @author lh
         *
         *  取消回收
         * @date: 2011-1-17
		 */
		private var specialShowMc:MovieClip; //特效
		protected function notSureHandler():void{
			sendNotification(TaskCommandList.RECALL_TASK,{taskID:id,type:243});
			sendNotification(TaskCommandList.CLOSE_TASK_PANEL);
			this.onClosePanel(null);
		}
		
		/**
         * @author lh
         *
         *  获取装备回收特效
         * @date: 2011-1-21
         **/
        protected function getSpecialShow():void
        {
        	if(!specialShowMc){
	        	ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/EquipSpecial.swf",onLoabdComplete);
        	}
        	else{
        		onLoabdComplete();
        	}
        }
        
        /**
         * @author lh
         *
         * 播放特效
         * @date: 2011-1-21
         **/
        protected function onLoabdComplete():void
        {
        	if(!specialShowMc){
        		specialShowMc=ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/EquipSpecial.swf");
        	}
        	specialShowMc.x = 200;
        	specialShowMc.y = 200;
        	specialShowMc.gotoAndPlay(1);
        	this.view.addChild(specialShowMc);
        }
		
		
		/**
		 * 关闲面板 
		 * @param e
		 *  
		 */		
		protected function onClosePanel(e:Event):void{
			this.dataProxy.taskEquipReturnIsOpen=false;
			if(this.useItem!=null && this.view.mc_contributeCell.contains(this.useItem)){
				sendNotification(EventList.BAGITEMUNLOCK, this.useItem.Id);
				this.view.mc_contributeCell.removeChild(this.useItem);
				this.useItem=null;
			}
			specialShowMc = null;
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
		}
		
		
	}
}