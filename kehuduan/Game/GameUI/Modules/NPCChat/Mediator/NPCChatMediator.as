package GameUI.Modules.NPCChat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.NPCChat.Command.NPCChatComList;
	import GameUI.Modules.NPCChat.Command.ReceiveNPCMsgCommand;
	import GameUI.Modules.NPCChat.Command.SelectedNPCCommand;
	import GameUI.Modules.NPCChat.Command.SendNPCMsgCommand;
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.Modules.NPCChat.Proxy.PipeDataProxy;
	import GameUI.Modules.NPCChat.View.LinkCell;
	import GameUI.Modules.NPCChat.View.NPCDialogPanel;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NPCChatMediator extends Mediator
	{
		public static const NAME:String="NPCChatMediator";
		public static const DEFAULTPOS:Point=new Point(80,58);
		private var finishMC:MovieClip;
		private var linkCell:LinkCell;
		public static var NPCChatIsOpen:Boolean = false;				/** 该面板是否打开 */
		
		protected var panelBase:PanelBase;
		protected var conentUI:NPCDialogPanel;
		protected var container:UIScrollPane;
		protected var pipeDataProxy:PipeDataProxy;
		
		public function NPCChatMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get chatUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				NPCChatComList.SHOW_NPC_CHAT,
				NPCChatComList.HIDE_NPC_CHAT,
				EventList.CLOSE_NPC_ALL_PANEL,
				NPCChatComList.SET_NPCTALK_ISMOVE,
				NPCChatComList.SHOW_SIMULATE_NPC_CHAT
			];	
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW:
					sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"NPCChat"});
					this.chatUI.mouseEnabled=false;
					facade.registerCommand(NPCChatComList.RECEIVE_NPC_MSG,ReceiveNPCMsgCommand);
					facade.registerCommand(NPCChatComList.SEND_NPC_MSG,SendNPCMsgCommand);
					facade.registerCommand(EventList.SELECTED_NPC_ELEMENT,SelectedNPCCommand);
					facade.registerProxy(new PipeDataProxy());
					this.panelBase=new PanelBase(this.chatUI,this.chatUI.width+9,this.chatUI.height+36);
					
					this.panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_npcc_med_npccm_hn_1" ]);              //"对 话"	
					this.panelBase.name = "NPCPanelBase";	
					this.conentUI=new NPCDialogPanel();
					this.container=new UIScrollPane(this.conentUI);
					this.container.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
					this.container.width=223;
					this.container.height=272;
					this.conentUI.onLinkClick=this.onLinkClick;
					this.container.refresh();
					
					finishMC = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("finishTask");
					finishMC.x = 184;
					finishMC.y = 295;
					finishMC.txt_accept.mouseEnabled=false;
					break;
				case NPCChatComList.SHOW_NPC_CHAT:
					NPCChatMediator.NPCChatIsOpen = true;
					this.pipeDataProxy=facade.retrieveProxy(PipeDataProxy.NAME) as PipeDataProxy;
					if( GameCommonData.fullScreen == 2 )
					{
						this.panelBase.x=DEFAULTPOS.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					    this.panelBase.y=DEFAULTPOS.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;	
					}else{
						this.panelBase.x=DEFAULTPOS.x;
						this.panelBase.y=DEFAULTPOS.y;
					}
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					this.panelBase.addEventListener(Event.CLOSE,onPanelCloseHandler);
					if(GameCommonData.SameSecnePlayerList!=null)
					{
						var npc:GameElementAnimal=GameCommonData.SameSecnePlayerList[GameCommonData.talkNpcID];
						if(npc!=null){
							this.panelBase.SetTitleTxt(npc.Role.Name);
						}else{
							this.panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_npcc_med_npccm_hn_1" ]);            //"对 话"
						}
						var des:String=this.pipeDataProxy.desText;
						var dataPro:Array=this.pipeDataProxy.linkArr;
						this.pipeDataProxy.reset();
						this.conentUI.desStr=des;
						this.conentUI.dataProvider=dataPro;
						this.container.refresh();
						this.chatUI.addChild(this.container);
						this.container.x=19;
						this.container.y=9;
					}
					
					if( DialogConstData.finishTask && DialogConstData.finishTask.length > 0 )
					{
						finishMC.txt_accept.text = GameCommonData.wordDic[ "mod_task_med_taski_han_3" ];         //完成任务
						if( !chatUI.contains( finishMC ) ) chatUI.addChild( finishMC );
						finishMC.btn_Accept.addEventListener( MouseEvent.CLICK, onClick );
					}else if( DialogConstData.unAcceptTask && DialogConstData.unAcceptTask.length > 0 ){
						finishMC.txt_accept.text = GameCommonData.wordDic[ "mod_task_med_taski_han_2" ];         //接受任务
						if( !chatUI.contains( finishMC ) ) chatUI.addChild( finishMC );
						finishMC.btn_Accept.addEventListener( MouseEvent.CLICK, onClick );
					}else{
						if( chatUI.contains( finishMC ) ) chatUI.removeChild( finishMC );
						if( finishMC.btn_Accept.hasEventListener( MouseEvent.CLICK ) ) finishMC.btn_Accept.removeEventListener( MouseEvent.CLICK, onClick );
					}
					
					//通知新手引导系统
					if(NewerHelpData.id && NewerHelpData.obj && NewerHelpData.newerHelpIsOpen)
					{
					    sendNotification(NewerHelpEvent.SELECT_NPC_NOTICE_NEWER_HELP, {id:NewerHelpData.id, newerData:NewerHelpData.obj}); 
					    NewerHelpData.id = 0;
					    NewerHelpData.obj = null;
					}
				 	break;
				case NPCChatComList.HIDE_NPC_CHAT:
					onPanelCloseHandler(null);
					break;	
				case EventList.CLOSE_NPC_ALL_PANEL:
					onPanelCloseHandler(null);
					break;	
				case NPCChatComList.SET_NPCTALK_ISMOVE:
					var setType:uint=uint(notification.getBody());
					if(setType==0){
						this.panelBase.IsDrag=true;
					}else if(setType==1){
						this.panelBase.IsDrag=false;
					}
					break;	
				case NPCChatComList.SHOW_SIMULATE_NPC_CHAT:	
					if(NPCChatMediator.NPCChatIsOpen)
					{
						onPanelCloseHandler(null);
						return;
					}
					NPCChatMediator.NPCChatIsOpen = true;
					var obj:Object=notification.getBody();
					this.panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_npcc_med_npccm_hn_2" ]);                       //"装备强化"
					var simulateDes:String=obj["simulateDes"];
					var simulateDataPro:Array=obj["simulateDataPro"];
					this.conentUI.desStr=simulateDes;
					this.conentUI.dataProvider=simulateDataPro;
					this.container.refresh();
					this.chatUI.addChild(this.container);
					this.container.x=19;
					this.container.y=9;	
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					this.panelBase.addEventListener(Event.CLOSE,onPanelCloseHandler);
					break;
			}
		}
		
		private function onClick( e:MouseEvent ):void
		{
			if( DialogConstData.finishTask && DialogConstData.finishTask.length > 0 )
			{
				linkCell = DialogConstData.finishTask.shift() as LinkCell;
			}else{
				linkCell = DialogConstData.unAcceptTask.shift() as LinkCell;
			}
			if( linkCell )  linkCell.onClickBtn();
		}
		
		protected function onPanelCloseHandler(e:Event):void{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase)){
				NPCChatMediator.NPCChatIsOpen = false;
				GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
				this.panelBase.IsDrag=true;
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLOSE_NPCCHAT_NOTICE_NEWER_HELP);
			}
		}
		
		/**
		 * 点击NPC链接处理方法 
		 * @param str
		 * 
		 */		
		protected function onLinkClick(str:String):void{
			switch (str){
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_hn_2" ]:             //"装备强化"
					sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI);
					break;
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_olk_1" ]:	         //"装备升星"
					sendNotification(EquipCommandList.SHOW_ADDSTAR_EQUIP);
					break;
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_olk_2" ]:   //"装备打孔"
					sendNotification(EquipCommandList.SHOW_EQUIPSTILETTO_UI,0);
					break;	
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_olk_3" ]:   //"宝石合成"
					sendNotification(EventList.STONE_COMPOSE_UI);
					break;
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_olk_4" ]:	    //"宝石镶嵌"
					sendNotification(EquipCommandList.SHOW_EQUIPSTILETTO_UI,1);
					break;
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_olk_5" ]:    //"宝石取出"
					sendNotification(EquipCommandList.SHOW_EQUIPSTILETTO_UI,2);
					break;	
				case GameCommonData.wordDic[ "mod_npcc_med_npccm_olk_6" ]:    //"装备玩法说明"
					sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 25);                                    
					break;
				default:
					sendNotification(NPCChatComList.SEND_NPC_MSG,str);
					break;
			}
			this.onCloseHandler(null);
		}
		
		
		protected function onCloseHandler(e:MouseEvent):void{
			this.onPanelCloseHandler(null);
			
		}
	}
}