
package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.UI.FaceText;
	import GameUI.Modules.Friend.command.FriendActionList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.proxy.MessageWordProxy;
	import GameUI.Modules.Friend.model.vo.MessageStruct;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionSend.FriendSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ReceiveMsgMediator extends Mediator
	{
		public static const NAME:String="ReceiveMsgMediator";
		public static const FRIENDINFODEFAULTPOS:Point=new Point(300,100);
		
		protected var leaveWord:MessageWordProxy;
		
		protected var msg:MessageStruct;
		protected var dataProxy:DataProxy;
		protected var panelBase:PanelBase;
		
		public function ReceiveMsgMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		/**
		 * 组件UI 
		 * @return 
		 * 
		 */		
		public function get receiveMsgPanel():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		override public function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW :
					 this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					 leaveWord=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"ReceiveMsgPanel"});
					this.receiveMsgPanel.mouseEnabled=false;
					this.panelBase=new PanelBase(this.receiveMsgPanel,this.receiveMsgPanel.width-28,this.receiveMsgPanel.height+12);
					this.panelBase.addEventListener(Event.CLOSE,panelCloseHandler);
					panelBase.name="receiveMsg";
					panelBase.x=FRIENDINFODEFAULTPOS.x;
					panelBase.y=FRIENDINFODEFAULTPOS.y;
					panelBase.SetTitleTxt(wordDic[ "mod_fri_view_med_rec_han" ]);//"接收信件"
					this.initSet();
					break;
				case FriendCommandList.SHOW_RECEIVE_MSG :
					if(this.dataProxy.FriendReveiveMsgIsOpen){
						this.panelCloseHandler(null);
					}
					var leavelWordPanel:MessageWordProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					dataProxy.FriendReveiveMsgIsOpen=true;
					this.initData(leavelWordPanel.popMsg());
					break;
				case FriendCommandList.READED_MESSAGE:
					(this.receiveMsgPanel.btn_nextMsg as SimpleButton).visible=false;
					break;
				case FriendCommandList.FRIEND_MESSAGE:
					(this.receiveMsgPanel.btn_nextMsg as SimpleButton).visible=true;
					break;		
			}
		}
		
		override public function listNotificationInterests():Array{
			return [EventList.INITVIEW,
			FriendCommandList.SHOW_RECEIVE_MSG,
			FriendCommandList.READED_MESSAGE,
			FriendCommandList.FRIEND_MESSAGE
			];
		}
		
		protected function initSet():void{
			
			(this.receiveMsgPanel.txt_timeFirst as TextField).mouseEnabled=false;
			(this.receiveMsgPanel.txt_timeSecond as TextField).mouseEnabled=false;
			(this.receiveMsgPanel.txt_msgType as TextField).mouseEnabled=false;
			
			(this.receiveMsgPanel.btn_close as SimpleButton).visible=true; 
			(this.receiveMsgPanel.btn_queryInfo as SimpleButton).visible=true;
			(this.receiveMsgPanel.btn_queryInfo as SimpleButton).addEventListener(MouseEvent.CLICK,onQueryInfoHandler);
			(this.receiveMsgPanel.btn_act as SimpleButton).visible=true;
			(this.receiveMsgPanel.btn_nextMsg as SimpleButton).addEventListener(MouseEvent.CLICK,onNextMsgHandler);
			(this.receiveMsgPanel.btn_addFriend as SimpleButton).addEventListener(MouseEvent.CLICK,onAddFriendHandler);
			(this.receiveMsgPanel.btn_recall as SimpleButton).addEventListener(MouseEvent.CLICK,onRecallHandler);
			(this.receiveMsgPanel.btn_close as SimpleButton).addEventListener(MouseEvent.CLICK,onCloseHandler);
		}
		
		/**
		 * 查询详细信息 
		 * @param e
		 * 
		 */		
		protected function onQueryInfoHandler(e:MouseEvent):void{
			if(msg.action==FriendActionList.SYSTEM_MSG){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:wordDic[ "mod_fri_view_med_rec_onQ" ], color:0xffff00});//"系统信息不能查看资料" 
			}else{
				FriendSend.getInstance().getFriendInfo(0,msg.sendPersonName);
			}
		}
		
		protected function onCloseHandler(e:MouseEvent):void{
			this.panelCloseHandler(null);
		}
		
		/**
		 * 
		 * 添加为好友
		 * @param e
		 * 
		 */		
		protected function onAddFriendHandler(e:MouseEvent):void{
			sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:-1,name:msg.sendPersonName});
			(this.receiveMsgPanel.btn_addFriend as SimpleButton).visible=false;
		}
		
		protected function onRecallHandler(e:MouseEvent):void{
			var info:PlayerInfoStruct=new PlayerInfoStruct();
			info.roleName=msg.sendPersonName;
			facade.sendNotification(FriendCommandList.SHOW_SEND_MSG,info);
			this.panelCloseHandler(null);
		}
		
		protected function onNextMsgHandler(e:MouseEvent):void{
			leaveWord=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
			this.msg=leaveWord.popMsg() as MessageStruct;
			this.initData(msg);
		}
	
		protected function initData(value:Object=null):void{
			
			var faceText:FaceText=new FaceText();
			msg=value as MessageStruct;
			var sendTime:Date=new Date(msg.sendTime);
			var str:String=sendTime.getFullYear()+wordDic[ "mod_fri_view_med_rec_initD_1" ]+(sendTime.getMonth()+1)+wordDic[ "mod_fri_view_med_rec_initD_2" ]+sendTime.getDate()+wordDic[ "mod_fri_view_med_rec_initD_3" ];
			//"年"		"月"		"日"
			
			(this.receiveMsgPanel.txt_timeFirst as TextField).text=str
			str=sendTime.getHours()+":"+sendTime.getMinutes()+":"+sendTime.getSeconds();
			(this.receiveMsgPanel.txt_timeSecond as TextField).text=str;
			(this.receiveMsgPanel.txt_msgType as TextField).mouseEnabled=false;
			if(msg.action==FriendActionList.SYSTEM_MSG){
				(this.receiveMsgPanel.txt_msgType as TextField).text=wordDic[ "mod_fri_view_med_rec_initD_4" ];//"系统消息";
			}else if(msg.action==FriendActionList.LEAVE_WORD){
				(this.receiveMsgPanel.txt_msgType as TextField).text=wordDic[ "mod_fri_view_med_rec_initD_5" ];//"玩家留言";
			}else if(msg.action==FriendActionList.CHAT_FLAG){
				(this.receiveMsgPanel.txt_msgType as TextField).text=wordDic[ "mod_fri_view_med_rec_initD_6" ];//"玩家消息";
			}
			(this.receiveMsgPanel.txt_userName as TextField).text=msg.sendPersonName;
			faceText.SetHtmlText('<font color="#'+msg.style+'">'+msg.msg+'</font>',"",false,300);
			faceText.width=300;
			while((this.receiveMsgPanel.mc_receiveMsg as MovieClip).numChildren>1){
				(this.receiveMsgPanel.mc_receiveMsg as MovieClip).removeChildAt(1);
			}
			(this.receiveMsgPanel.mc_receiveMsg as MovieClip).addChild(faceText);
			(this.receiveMsgPanel.btn_act as SimpleButton).mouseEnabled=false;    //举报
			if(msg.isFriend==0 || msg.action==FriendActionList.SYSTEM_MSG){
				(this.receiveMsgPanel.btn_addFriend as SimpleButton).visible=false;
			}else if(msg.isFriend==1){
				(this.receiveMsgPanel.btn_addFriend as SimpleButton).visible=true;
			}
			this.setFace(msg.face);
		}
		
		protected function panelCloseHandler(e:Event):void{
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			dataProxy.FriendReveiveMsgIsOpen=false;
		}
		
		protected function setFace(faceType:uint):void{
			var face:FaceItem=new FaceItem(String(faceType),null,"face", 1.0);
			face.offsetPoint = new Point(0, 0);
			face.x = 9.45;
			face.y = 7.95;
			var mc:MovieClip=this.receiveMsgPanel.mc_headImg as MovieClip;
			while(mc.numChildren>0){
				mc.removeChildAt(0);
			}
			mc.addChild(face);
		}	
	}
}