package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.FacePanelMediator;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Friend.command.FriendActionList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Chat;
	import Net.Protocol;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldType;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SendMsgMediator extends Mediator
	{
		public static const NAME:String="SendMsgMediator";
		public static const FRIENDINFODEFAULTPOS:Point=new Point(300,100);
		
		protected var dataProxy:DataProxy;
		/** 内容显示面板 */
		protected var panelBase:PanelBase;
		
		protected var roleInfo:PlayerInfoStruct;
		
		public function SendMsgMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		/**
		 * 发送消息组件 
		 * 
		 */		
		public function get sendMsgPanel():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		override public function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case FriendCommandList.SHOW_SEND_MSG:
					this.roleInfo=notification.getBody() as PlayerInfoStruct;
					this.initView(notification.getBody() as PlayerInfoStruct);
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					sendMsgPanel.stage.focus = sendMsgPanel.txt_sendMsg;
					UIConstData.FocusIsUsing = true;
					break;
				//获得表情	
				case FriendCommandList.GET_FACE_NAME:
					this.sendMsgPanel.stage.focus=this.sendMsgPanel.txt_sendMsg;
					(this.sendMsgPanel.txt_sendMsg as TextField).text+='\\'+notification.getBody();
					this.onTextChangeHandler(null);
					var len:uint=(this.sendMsgPanel.txt_sendMsg as TextField).length;
					(this.sendMsgPanel.txt_sendMsg as TextField).setSelection(len,len);
					break;	
				case FriendCommandList.SELECTED_FONT_COLOR:
					if(!this.sendMsgPanel) return;
					this.sendMsgPanel.stage.focus=this.sendMsgPanel.txt_sendMsg;
					(this.sendMsgPanel.txt_sendMsg as TextField).textColor=uint(notification.getBody());
					var len1:uint=(this.sendMsgPanel.txt_sendMsg as TextField).length;
					(this.sendMsgPanel.txt_sendMsg as TextField).setSelection(len1,len1);
					break;
				case EventList.UNITY_SEND_MSG:
					this.roleInfo=new PlayerInfoStruct();
					this.roleInfo.roleName=notification.getBody() as String;
					this.initView(roleInfo);
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					sendMsgPanel.stage.focus = sendMsgPanel.txt_sendMsg;
					UIConstData.FocusIsUsing = true;
					break;
			}
		}
		
		override public function listNotificationInterests():Array{
//			 return [FriendCommandList.SHOW_SEND_MSG,
//					FriendCommandList.GET_FACE_NAME,
//					FriendCommandList.SELECTED_FONT_COLOR,
//					EventList.UNITY_SEND_MSG
					]; 
		}
		
		/**
		 *   初始化好友信息显示组件，将组件加载到内存中 
		 * 
		 */		
		protected function initView(info:PlayerInfoStruct):void{
			
			this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			if(this.dataProxy.FriendSendMsgIsOpen){
				this.onClosePanelBase(null);
			}
			this.dataProxy.FriendSendMsgIsOpen=true;
			facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"SendMsgPanel"});
			this.panelBase=new PanelBase(this.sendMsgPanel,this.sendMsgPanel.width-18,this.sendMsgPanel.height+12);
			this.panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_fri_view_med_rec_initV" ]);//"发送信件"
			this.panelBase.x=FRIENDINFODEFAULTPOS.x;
			this.panelBase.y=FRIENDINFODEFAULTPOS.y;
			this.panelBase.addEventListener(Event.CLOSE,onClosePanelBase);
			initSet(info);
			initData(info);
		}
		
		/**
		 * 初始功能的设定 
		 * 
		 */		
		protected function initSet(info:PlayerInfoStruct):void{
			(this.sendMsgPanel.txt_userName as TextField).mouseEnabled=false;
			(this.sendMsgPanel.txt_sendMsg as TextField).type=TextFieldType.INPUT;
			(this.sendMsgPanel.txt_sendMsg as TextField).addEventListener(Event.CHANGE,onTextChangeHandler);
		 	(this.sendMsgPanel.btn_fontColor as SimpleButton).addEventListener(MouseEvent.CLICK,onSelectFontColorHandler);
			(this.sendMsgPanel.btn_face as SimpleButton).addEventListener(MouseEvent.CLICK,onSelectFaceHandler);
			(this.sendMsgPanel.btn_recall as SimpleButton).addEventListener(MouseEvent.CLICK,onReCallHandler);
			(this.sendMsgPanel.btn_close as SimpleButton).addEventListener(MouseEvent.CLICK,onCloseHandler);
			UIUtils.addFocusLis(sendMsgPanel.txt_sendMsg);
		}
		
		/**
		 * 检测字符输入长度 
		 * @param e
		 * 
		 */		
		protected function onTextChangeHandler(e:Event):void{
			var msg:String=(this.sendMsgPanel.txt_sendMsg as TextField).text;
			(this.sendMsgPanel.txt_sendMsg as TextField).text=UIUtils.getTextByCharLength(msg,127);
		}
		
		 
		/**
		 * 关闭面板处理
		 * @param e
		 * 
		 */		
		protected function onCloseHandler(e:MouseEvent):void{
			this.onClosePanelBase(null);
		}
		/**
		 * 回复 
		 * 
		 */		
		protected function onReCallHandler(e:MouseEvent):void{
			if((this.sendMsgPanel.txt_sendMsg as TextField).text==""){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_rec_onR" ]+'</font>', color:0xffff00});//发送内容不能为空
				return;
			}
				
			var obj:Object={};
			obj.type=Protocol.PLAYER_CHAT;
			obj.data=getParam();
			Chat.SendChat(obj);
			this.onClosePanelBase(null);
		}
		
		protected function getParam():Array{
		 	var data:Array=[];
			data.push(GameCommonData.Player.Role.Name);   //发信人姓名
			data.push(roleInfo.roleName);                 //收信人姓名
			data.push(new Date().time);						//发信时间
			data.push((this.sendMsgPanel.txt_sendMsg as TextField).text);  //聊天内容
			var style:String=(this.sendMsgPanel.txt_sendMsg as TextField).textColor as String;
			var n:Number=(this.sendMsgPanel.txt_sendMsg as TextField).textColor;
			data.push(n.toString(16)); 									//文本样式
			data.push(GameCommonData.Player.Role.Id);
			data.push(FriendActionList.CHAT_FLAG);
			data.push(0); 
			data.push(0);
			data.push(GameCommonData.Player.Role.Face);
			data.push(GameCommonData.Player.Role.Face);
			return data;
		}
		
		/**
		 * 打开颜色选择框 
		 * @param e
		 * 
		 */		
		protected function onSelectFontColorHandler(e:MouseEvent):void{
			if(!ChatData.ColorIsOpen){
				facade.registerMediator(new SelectFontColorMediator());
				facade.sendNotification(FriendCommandList.SHOW_FONT_COLOR,{x:this.panelBase.x+58,y:this.panelBase.y+160});
			}else{
				facade.sendNotification(FriendCommandList.HIDE_FONT_COLOR);
			}
				
		}
		
		/**
		 * 点击表情 
		 * @param e
		 * 
		 */		
		protected function onSelectFaceHandler(e:MouseEvent):void{
			if(!UIConstData.SelectFaceIsOpen){
				facade.registerMediator(new FacePanelMediator());
				facade.sendNotification(EventList.SHOWFACEVIEW,{x:this.panelBase.x+8,y:this.panelBase.y+128,type:"friend"});
			}else{
				facade.sendNotification(EventList.HIDESELECTFACE);
			}
		}
		
		/**
		 * 初始化显示数据 
		 * 
		 */		
		protected function initData(info:PlayerInfoStruct):void{
			(this.sendMsgPanel.txt_userName as TextField).text=info.roleName;
			(this.sendMsgPanel.txt_sendMsg as TextField).text="";
		}
		
		/**
		 * 关闭 
		 * 
		 */		
		protected function onClosePanelBase(e:Event):void{
			
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			this.dataProxy.FriendSendMsgIsOpen=false;
			UIConstData.FocusIsUsing = false;
			this.panelBase.removeEventListener(Event.CLOSE,onClosePanelBase);
			(this.sendMsgPanel.btn_fontColor as SimpleButton).removeEventListener(MouseEvent.CLICK,onSelectFontColorHandler);
			(this.sendMsgPanel.btn_face as SimpleButton).removeEventListener(MouseEvent.CLICK,onSelectFaceHandler);
			UIUtils.removeFocusLis(sendMsgPanel.txt_sendMsg);
		}
		
	}
}