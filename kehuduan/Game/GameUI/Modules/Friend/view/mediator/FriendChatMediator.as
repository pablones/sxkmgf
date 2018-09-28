package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.ProConversion;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.FacePanelMediator;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Friend.command.FriendActionList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.proxy.MessageWordProxy;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Friend.model.vo.RoleLaborStruct;
	import GameUI.Modules.Friend.view.ui.ChatInfoPanel;
	import GameUI.Modules.Friend.view.ui.ChatList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.Meridians.Components.DisplayObjectGlow;
	import GameUI.Modules.SystemMessage.Data.SysMessageEvent;
	import GameUI.Modules.Team.Datas.TeamEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionSend.Chat;
	import Net.ActionSend.FriendSend;
	import Net.Protocol;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.ui.Keyboard;
	import flash.utils.Dictionary;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class FriendChatMediator extends Mediator
	{
		
		public static const NAME:String="FriendChatMediator";
		
		/**
		 *  聊天数据列表   
		 *  chatDic[Id]=[{fontColor:  ,des: },{},{}]; 
		 */		
		 
		
		protected var dataProxy:DataProxy;
		protected var basePanel:PanelBase;
		protected var msgProxy:MessageWordProxy;
		protected var friendManager:FriendManagerMediator;
		
		protected var chatingList:ChatList;
		protected var chatHistoryList:ChatList;
		protected var chatInfoView:ChatInfoPanel;
		protected var chatHistoryView:MovieClip;
		protected var sendLeaveMsgTime:Number=0;
		
		public var msgIconPanel:Sprite; 
		protected var btnMinimizeMsgWindow:SimpleButton;
		protected var chattingFriendList:Array=[];
		protected var chattingFriendIconList:Array = [];
		
		protected var glows:Dictionary = new Dictionary(true);
		
		public  var roleInfo:PlayerInfoStruct;
		
		public function FriendChatMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get view():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				EventList.ENTERMAPCOMPLETE,
				FriendCommandList.GET_FACE_NAME,
				FriendCommandList.SELECTED_FONT_COLOR,
				FriendCommandList.SHOW_RECEIVE_MSG,
				FriendCommandList.SHOW_SEND_MSG,
				FriendCommandList.ADD_MSG_CHAT,
				FriendCommandList.REVEIVE_CHAT_INFO ,
				FriendCommandList.MINIMIZE_MSG_WINDOW,
				FriendCommandList.FRIEND_MESSAGE
			];
		}
		
		
		public override function handleNotification(notification:INotification):void{
			var id:uint;
			var role:PlayerInfoStruct;
			var arr:Array;
			var obj:Object;
			switch (notification.getName()){
				case EventList.INITVIEW:
					sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"FriendChatPanel"});
					
					this.chatInfoView=new ChatInfoPanel();
					this.chatHistoryView=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("FriendChatHistoryPanel");
					dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					msgProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
					this.basePanel=new PanelBase(this.view,this.view.width,this.view.height+10);
					this.basePanel.x=UIConstData.DefaultPos1.x;
					this.basePanel.y=UIConstData.DefaultPos1.y;
					this.basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_1" ]);//"好友聊天"
					(this.view.txt_chatInput as TextField).text="";
					this.addEventList();
				
					this.chatingList=new ChatList(318,219);
					
					this.chatingList.x=16;
					this.chatingList.y=59;
					
					this.chatHistoryList=new ChatList(245,376);
					
					this.chatHistoryList.x=10;
					this.chatHistoryList.y=16;
					this.chatHistoryView.addChild(this.chatHistoryList);
					
					this.view.addChild(this.chatingList);
					this.view.addChild(this.chatInfoView);
					this.chatInfoView.x=338;
					this.chatHistoryView.x=338;
					
					// 1. 在主界面上添加一个区域 Sprite，放置所有聊天图标
					msgIconPanel = new Sprite();
					msgIconPanel.x = 660;
					msgIconPanel.y = 472;
					GameCommonData.GameInstance.GameUI.addChild(msgIconPanel);
					
					ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/FriendMsgIcons.swf",
						function():void
						{
							// 2. 在 basePanel 右上角添加一个“最小化”按钮
							var resSwf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/FriendMsgIcons.swf");
							btnMinimizeMsgWindow = new (resSwf.loaderInfo.applicationDomain.getDefinition("MinimizeButton"))();
							btnMinimizeMsgWindow.addEventListener(MouseEvent.CLICK, minimizeChatWindowHandler);
							adjustMinimizeBtnPos();
							basePanel.addChild(btnMinimizeMsgWindow);
						});
					
					break;
				case EventList.ENTERMAPCOMPLETE:
					break;	
				//获得表情	
				case FriendCommandList.GET_FACE_NAME:
					this.basePanel.stage.focus=this.view.txt_chatInput;
					var des:String=(this.view.txt_chatInput as TextField).text;
					des+='\\'+notification.getBody();
					(this.view.txt_chatInput as TextField).text=des;
					this.onTextChangeHandler(null);
					var len:uint=(this.view.txt_chatInput as TextField).length;
					(this.view.txt_chatInput as TextField).setSelection(len,len);
					break;	
				case FriendCommandList.SELECTED_FONT_COLOR:
					if(!this.basePanel) return;
					this.basePanel.stage.focus=this.view.txt_chatInput;
					(this.view.txt_chatInput as TextField).textColor=uint(notification.getBody());
					var len1:uint=(this.view.txt_chatInput as TextField).length;
					(this.view.txt_chatInput as TextField).setSelection(len1,len1);
					break;	
				//收消息	
				case FriendCommandList.SHOW_RECEIVE_MSG:                                //显示接收到的数据
					msgProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
					id=msgProxy.popMsgId();
					if(id==110){
						facade.sendNotification(SysMessageEvent.SHOWMESSAGEVIEW);
						return;
					}
					
					if (this.roleInfo)
					{
						sendNotification(FriendCommandList.MINIMIZE_MSG_WINDOW);
					}
					
					this.roleInfo=new PlayerInfoStruct();
					this.roleInfo.frendId=id;		
					arr=msgProxy.getMsgs(id);
					if(arr!=null){
						obj=arr[arr.length-1];
						this.roleInfo.roleName=obj.sendPersonName;
						this.roleInfo.face=obj.face;
						this.roleInfo.feel=obj.feel;
						this.initData(arr.concat());
					}
					this.showChatInfo();
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
					
					if(this.view.contains(this.chatHistoryView)){
						this.view.removeChild(this.chatHistoryView);
						this.view.addChild(this.chatInfoView);
					}
					this.basePanel.SetTitleTxt(this.roleInfo.roleName);
					this.basePanel.setWidth(475);
					dataProxy.FriendReveiveMsgIsOpen=true;
					this.chatingList.scrollBottom();
					UIConstData.FocusIsUsing = true;
					this.adjustMinimizeBtnPos();
					this.removeMsgIcon(getMsgIcon(this.roleInfo.frendId));
					break;	
				//发消息	
				case FriendCommandList.SHOW_SEND_MSG:
					if (this.roleInfo)
					{
						sendNotification(FriendCommandList.MINIMIZE_MSG_WINDOW);
					}
				
					this.roleInfo=notification.getBody() as PlayerInfoStruct;
					this.showChatInfo();
					this.initData((msgProxy.getMsgs(this.roleInfo.frendId) as Array).concat());					
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
					if(this.view.contains(this.chatHistoryView)){
						this.view.removeChild(this.chatHistoryView);
						this.view.addChild(this.chatInfoView);
					}
					this.basePanel.setWidth(475);
					this.basePanel.SetTitleTxt(this.roleInfo.roleName);
					view.stage.focus = this.view.txt_chatInput;
					UIConstData.FocusIsUsing = true;
					dataProxy.FriendReveiveMsgIsOpen=true;
					this.chatingList.scrollBottom();
					this.adjustMinimizeBtnPos();
					this.removeMsgIcon(getMsgIcon(this.roleInfo.frendId));
					break;
				//添加一条收到的消息		
				case FriendCommandList.ADD_MSG_CHAT:
					msgProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
					if(this.roleInfo.frendId!=uint(notification.getBody()))return;
					var tempArr:Array=msgProxy.getMsgs(this.roleInfo.frendId);
					this.chatingList.addChatCell(tempArr[tempArr.length-1]);
					var a:Array=msgProxy.getMsgs(this.roleInfo.frendId);
					break;
				//收到好友的聊天信息		
				case FriendCommandList.REVEIVE_CHAT_INFO:
					var roleDetail:RoleLaborStruct=notification.getBody() as RoleLaborStruct;
					if(this.roleInfo && (roleDetail.friendID==this.roleInfo.frendId)){
						this.chatInfoView.friendFace=String(roleDetail.face);
						this.chatInfoView.friendFeel=roleDetail.feel;
						this.chatInfoView.friendJob=ProConversion.getInstance().RolesListDic[roleDetail.profession];
						this.chatInfoView.friendLevel=roleDetail.level+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级"
						if(roleDetail.mapId!=GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_3" ]){//"未知"
							var mapArr:Array=roleDetail.mapId.split("_");
							this.chatInfoView.friendLine=mapArr[0]
							this.chatInfoView.friendMap=SmallConstData.getInstance().mapItemDic[mapArr[1]].name;
						}else{
							this.chatInfoView.friendMap=GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_3" ];//"未知";
							this.chatInfoView.friendLine=GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_3" ];//"未知"; 
						}
					}
					this.chatingList.scrollBottom();
					break;		
				// 最小化消息窗口
				case FriendCommandList.MINIMIZE_MSG_WINDOW:
					ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/FriendMsgIcons.swf",
					function ():void
					{
						if (roleInfo)
							addMsgIcon(roleInfo);
						
						panelCloseHandler(null);
					});
					
					dataProxy.FriendReveiveMsgIsOpen=false;
					break;
				// 收到朋友消息要高亮相应的消息图标，如果没图标，创建一个
				case FriendCommandList.FRIEND_MESSAGE:
					msgProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
					id=msgProxy.popMsgId();
					if(id==110){
						break;
					}
					
					if (chattingFriendList.indexOf(id) == -1)
					{
						role=new PlayerInfoStruct();
						role.frendId=id;		
						arr=msgProxy.getMsgs(id);
						if(arr!=null){
							obj=arr[arr.length-1];
							role.roleName=obj.sendPersonName;
							role.face=obj.face;
							role.feel=obj.feel;
						}
						glows[this.addMsgIcon(role)].lightOn();
					}
					else
					{
						glows[getMsgIcon(id)].lightOn();
					}
					break;
			}	
		}
		
		/**
		 * 显示聊友资料
		 * 
		 */		
		protected function showChatInfo():void{
			FriendSend.getInstance().getChatInfo(this.roleInfo.frendId,this.roleInfo.roleName);
			this.chatInfoView.face=String(GameCommonData.Player.Role.Face);
			this.chatInfoView.feel=GameCommonData.Player.Role.Feel; 
			this.chatInfoView.line=GameConfigData.GameSocketName.replace("s",GameCommonData.wordDic[ "mod_fri_view_med_show_1"] )+GameCommonData.wordDic[ "mod_fri_view_med_friendC_sho" ];//"线";
			this.chatInfoView.level=GameCommonData.Player.Role.Level+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级";
			this.chatInfoView.map=GameCommonData.GameInstance.GameScene.GetGameScene.MapName;
			this.chatInfoView.job=ProConversion.getInstance().RolesListDic[GameCommonData.Player.Role.CurrentJobID];	
		}
		
		/**
		 * 调整最小化按钮的位置 
		 * 
		 */		
		protected function adjustMinimizeBtnPos():void
		{
			if (!btnMinimizeMsgWindow) return;
			if (this.basePanel.contains(btnMinimizeMsgWindow))
				this.basePanel.removeChild(btnMinimizeMsgWindow);
			btnMinimizeMsgWindow.x = this.basePanel.width - 48;
			btnMinimizeMsgWindow.y = 2;
			this.basePanel.addChild(btnMinimizeMsgWindow);
		}
		
		
		/**
		 * 添加一个消息图标 
		 * 
		 */		
		protected function addMsgIcon(role:PlayerInfoStruct):MovieClip
		{
			var index:int = chattingFriendList.indexOf(role.frendId);
			var iconMC:MovieClip;
			if (index == -1)
			{
				var fmiMC:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/FriendMsgIcons.swf");
				iconMC = new (fmiMC.loaderInfo.applicationDomain.getDefinition('MsgIcon'));
//				msgIconPanel.addChildAt(iconMC, 0);
				chattingFriendList.unshift(role.frendId);
				chattingFriendIconList.unshift(iconMC);
				arrangeMsgIcons();
				iconMC.data = role;
				
				var senderTF:TextField = iconMC.txtSender as TextField;
				senderTF.autoSize = TextFieldAutoSize.LEFT;
				var t:String = role.roleName; 
				senderTF.text = t;
				while (senderTF.width > 60)
				{
					t = t.slice(0, -1);
					senderTF.text = t + "...";
					// 用 while 就怕特殊字体崩掉...  -zhao
				}
				
				var face:FaceItem = new FaceItem(String(role.face), null, "face", (20/50), new Point(2.0, 2.5));
				iconMC.addChild(face);
				iconMC.addEventListener(MouseEvent.CLICK, friendMsgIconClickHandler);
			}
			else
			{
				iconMC = msgIconPanel.getChildAt(index) as MovieClip;
				chattingFriendIconList.splice(chattingFriendIconList.indexOf(iconMC), 1).unshift(iconMC);
			}
			
			if (!glows[iconMC])
			{
				var glow:DisplayObjectGlow = new DisplayObjectGlow(iconMC, 0x00ff00, 1, 18, 1.5);
				glows[iconMC] = glow;
			}
			
			return iconMC;
		}
		
		/**
		 * 根据图标实例或角色信息删除一个消息图标
		 * @param icon
		 * 
		 */		
		protected function removeMsgIcon(icon:MovieClip):void
		{
			if (icon) // 根据图标实例删除
			{
				if (msgIconPanel.contains(icon))
				{
					msgIconPanel.removeChild(icon);
					icon.removeEventListener(MouseEvent.CLICK, friendMsgIconClickHandler);
					
					chattingFriendList.splice(chattingFriendList.indexOf(icon.data.frendId), 1);
					chattingFriendIconList.splice(chattingFriendIconList.indexOf(icon), 1);
				}
				arrangeMsgIcons();
				glows[icon] = null;
				delete glows[icon];
				icon = null;
			}
		}
		
		/**
		 * 根据角色 ID 反查消息图标，如果图标不存在则返回 null。 
		 * 
		 */		
		protected function getMsgIcon(roleId:uint):MovieClip
		{
			var iconCount:int = msgIconPanel.numChildren;
			for (var i:int = 0; i < iconCount; i ++)
			{
				var icon:MovieClip = msgIconPanel.getChildAt(i) as MovieClip;
				if (icon.data.frendId == roleId)
				
				{
					return icon;
				}
			}
			return null;
		}
		
		
		/**
		 * 调整聊天小图标的位置
		 * 
		 */		
		protected function arrangeMsgIcons():void
		{
			var cx:Number = 0;
			var cy:Number = 0;
			var i:int;
			for (i = 0; i < msgIconPanel.numChildren; i++)
			{
				var d:DisplayObject = msgIconPanel.getChildAt(i);
				var index:int = chattingFriendIconList.indexOf(d);
				
				if (index == -1 || index >= 6)
				{
					msgIconPanel.removeChild(d);
				}
			}
			for (i = 0; i < 6; i++)
			{
				var icon:DisplayObject = chattingFriendIconList[i] as DisplayObject;
				if (!icon) continue;
				icon.x = cx;
				icon.y = cy;
				if (!msgIconPanel.contains(icon))
				{
					msgIconPanel.addChild(icon);
				}
				if (cy == 0)
				{
					cy = 28;
				}
				else
				{
					cx += 88;
					cy = 0;
				}
			}
		}
		
		
		/**
		 * 最小化按钮 
		 * 
		 */		
		protected function minimizeChatWindowHandler(event:MouseEvent):void
		{
			sendNotification(FriendCommandList.MINIMIZE_MSG_WINDOW);
		}
		
		
//		protected var lastClick:Number = 0;
//		protected var menuTimeout:Number;
//		protected var menuPos:Point;
		/**
		 * 单击：恢复窗口 
		 * 
		 */		
		protected function friendMsgIconClickHandler(event:MouseEvent):void
		{
//			var now:Number = new Date().getTime();
//			
//			if (now - lastClick < 200)
//			{
//				clearTimeout(menuTimeout);
//				
				restoreChatWindow(event.currentTarget as MovieClip);
//			}
//			else
//			{
//				var icon:MovieClip = event.currentTarget as MovieClip;
//				menuPos = new Point(icon.stage.mouseX, icon.stage.mouseY);
//				menuTimeout = setTimeout(function():void
//				{
//					popUpMenu(icon);
//				}, 200);
//			}
//			
//			lastClick = now;
		}
		
		protected function restoreChatWindow(icon:MovieClip):void
		{
			if (this.roleInfo)
			{
				sendNotification(FriendCommandList.MINIMIZE_MSG_WINDOW);
			}
			
			removeMsgIcon(icon);
			
			sendNotification(FriendCommandList.SHOW_SEND_MSG, icon.data);
		}
		
		
//		protected var menu:MenuItem;
//		protected var isShowingMenu:Boolean = false;
//		protected var menuIcon:MovieClip;
//		
//		protected function popUpMenu(icon:MovieClip):void
//		{
//			menuIcon = icon;
//			
//			if (!menu)
//			{
//				buildMenu();
//			}
//			
//			var m:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("MENU");
//			if (m)
//			{
//				GameCommonData.GameInstance.GameUI.removeChild(m);
//			}
//			
//			menu.x = menuPos.x;
//			menu.y = menuPos.y;
//			GameCommonData.GameInstance.GameUI.addChild(menu);
//			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.CLICK, stageClickHandler);
//		}
//		
//		protected function buildMenu():void
//		{
//			menu = new MenuItem();
//			var player:PlayerInfoStruct = menuIcon.data as PlayerInfoStruct;
//										
//			var menuData:Array = [{cellText: GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ], data: {id: 1}}];
//			if (player.idTeam == 0)
//			{
//				menuData.push({cellText: GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ], data: {id: 2}});
//			}
//			else if (player.idTeam > 0 && GameCommonData.Player.Role.idTeam == 0)
//			{
//				menuData.push({cellText: GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ], data: {id: 3}});
//			}
//			menuData.push(	{cellText: GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ], data: {id: 4}},
//										{cellText: GameCommonData.wordDic[ "mod_fri_view_med_friendC_bui_1" ], data: {id: 5}});
//										
//			menu.dataPro = menuData;
//			
//			menu.addEventListener(MenuEvent.Cell_Click, menuCellClickHandler);
//		}
//		
//		protected function menuCellClickHandler(event:MenuEvent):void
//		{
////			trace(">>> cellClick: " + event.cell.data.id);
//			var player:PlayerInfoStruct = menuIcon.data as PlayerInfoStruct;
//			
//			switch (event.cell.data.id)
//			{
//				case 1: // 发送消息
//					restoreChatWindow(menuIcon);
//					break;
//				case 2: // 邀请入队
//					sendNotification(EventList.INVITETEAM, {id:player.frendId});
//					break;
//				case 3: // 申请入队
//					sendNotification(EventList.APPLYTEAM, {id:player.frendId});
//					break;
//				case 4: // 查看资料
//					FriendSend.getInstance().getFriendInfo(player.frendId, player.roleName);
//					break;
//				case 5: // 结束对话
//					removeMsgIcon(menuIcon);
//					break;
//			}
//		}
//		
//		protected function stageClickHandler(event:MouseEvent):void
//		{
////			trace(">>> stageClick");
//			if (GameCommonData.GameInstance.GameUI.contains(menu))
//			{
//				GameCommonData.GameInstance.GameUI.removeChild(menu);
//				GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.CLICK, stageClickHandler);
//			}
//		}
		
		protected function getParam():Array{
		 	var data:Array=[];
			data.push(GameCommonData.Player.Role.Name);   //发信人姓名
			data.push(roleInfo.roleName);                 //收信人姓名
			data.push(new Date().time);					  //发信时间
			data.push((this.view.txt_chatInput as TextField).text);  //聊天内容
			var style:String=(this.view.txt_chatInput as TextField).textColor as String;
			var n:Number=(this.view.txt_chatInput as TextField).textColor;
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
		 * 初始化聊天数据 
		 * 
		 */		
		protected function initData(arr:Array):void{
			this.chatingList.dataPro=arr;
		}
		
		protected function onOpenMsgHistory(e:MouseEvent):void{
			if(this.view.contains(this.chatHistoryView)){
				this.view.removeChild(this.chatHistoryView);
				this.view.addChild(this.chatInfoView);
				this.basePanel.setWidth(475);
				this.adjustMinimizeBtnPos();
			}else{
				this.view.removeChild(this.chatInfoView);
				this.view.addChild(this.chatHistoryView);
				this.chatHistoryList.dataPro=(msgProxy.getMsgs(this.roleInfo.frendId) as Array).concat();
				this.basePanel.setWidth(615);
				this.chatHistoryList.scrollBottom();
				this.adjustMinimizeBtnPos();
			}
		}
		
		
		
		/**
		 * 添加事件监听器 
		 * 
		 */		
		protected function addEventList():void{
			this.basePanel.addEventListener(Event.CLOSE,panelCloseHandler);
			(this.view.btn_msgHistory as SimpleButton).addEventListener(MouseEvent.CLICK,onOpenMsgHistory);
			(this.view.btn_send as SimpleButton).addEventListener(MouseEvent.CLICK,onSendMsgClick);
			(this.view.btn_face as SimpleButton).addEventListener(MouseEvent.CLICK,onSelectFaceHandler);
			(this.view.btn_color as SimpleButton).addEventListener(MouseEvent.CLICK,onSelectFontColorHandler);
			(this.view.txt_chatInput as TextField).addEventListener(Event.CHANGE,onTextChangeHandler);
			(this.view.txt_chatInput as TextField).addEventListener(KeyboardEvent.KEY_UP,onTextKeyUp);
			(this.view.btn_queryInfo as SimpleButton).addEventListener(MouseEvent.CLICK,onQueryInfoHandler);
			(this.view.btn_team as SimpleButton).addEventListener(MouseEvent.CLICK,onTeamHandler);
			(this.view.btn_close as SimpleButton).addEventListener(MouseEvent.CLICK,panelCloseHandler);
			UIUtils.addFocusLis(this.view.txt_chatInput);
		}
		
		
		/**
		 * 查询好友资料
		 * @param e
		 * 
		 */		
		protected function onQueryInfoHandler(e:MouseEvent):void{
			FriendSend.getInstance().getFriendInfo(this.roleInfo.frendId,this.roleInfo.roleName);
		}
		
		
		/**
		 * 组队
		 * @param e
		 * 
		 */		
		protected function onTeamHandler(e:MouseEvent):void{
			sendNotification(TeamEvent.SUPER_MAKE_TEAM,this.roleInfo.frendId);
		}
		
		
		/**
		 * 按ctrl+Enter键发送消息 
		 * @param e
		 * 
		 */		
		protected function onTextKeyUp(e:KeyboardEvent):void{
			
			if(e.ctrlKey && e.keyCode==Keyboard.ENTER){
				this.onSendMsgClick(null);
				this.basePanel.stage.focus=this.view.txt_chatInput;
			}
		}
		
	
		/**
		 * 发送消息 
		 * @param e
		 * 
		 */		
		protected function onSendMsgClick(e:MouseEvent):void{
			if(this.view.txt_chatInput.text==null || this.view.txt_chatInput.text==""){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendC_onS_1" ]+'</font>', color:0xffff00});//发送内容不能为空
				return;
			}
			friendManager=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
			msgProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
			var desArr:Array=msgProxy.getMsgs(this.roleInfo.frendId);
			var des:String=this.view.txt_chatInput.text;
			var style:String=(this.view.txt_chatInput as TextField).textColor as String;
			var n:Number=(this.view.txt_chatInput as TextField).textColor; 
			var color:String=n.toString(16);
			
			var obj:Object={};
			obj.type=Protocol.PLAYER_CHAT;
			obj.data=getParam();
			if(!friendManager.isPersonOnline(this.roleInfo.roleName)){
				if(getTimer()-this.sendLeaveMsgTime>5000){
					desArr.push('<font color="#00ff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendC_onS_2" ]+'</font>&nbsp;&nbsp;&nbsp;<font color="#ffffff">'+(new Date()).toLocaleTimeString()+'</font><br>&nbsp;&nbsp;<font color="#'+color+'">'+des+'</font>')//我
					this.chatingList.addChatCell('<font color="#00ff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendC_onS_2" ]+'</font>&nbsp;&nbsp;&nbsp;<font color="#ffffff">'+(new Date()).toLocaleTimeString()+'</font><br>&nbsp;&nbsp;<font color="#'+color+'">'+des+'</font>');//我
					Chat.SendChat(obj);
					this.view.txt_chatInput.text="";
					this.sendLeaveMsgTime=getTimer();
				}else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendC_onS_3" ]+'</font>', color:0xffff00});//两次留言信息的时间间隙是5秒
				}
			}else{
				desArr.push('<font color="#00ff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendC_onS_2" ]+'</font>&nbsp;&nbsp;&nbsp;<font color="#ffffff">'+(new Date()).toLocaleTimeString()+'</font><br>&nbsp;&nbsp;<font color="#'+color+'">'+des+'</font>')//我
				this.chatingList.addChatCell('<font color="#00ff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendC_onS_2" ]+'</font>&nbsp;&nbsp;&nbsp;<font color="#ffffff">'+(new Date()).toLocaleTimeString()+'</font><br>&nbsp;&nbsp;<font color="#'+color+'">'+des+'</font>');//我
				Chat.SendChat(obj);
				this.view.txt_chatInput.text="";
			}
		}
		
		/**
		 * 打开颜色选择框 
		 * @param e
		 * 
		 */		
		protected function onSelectFontColorHandler(e:MouseEvent):void{
			if(!ChatData.ColorIsOpen){
				facade.registerMediator(new SelectFontColorMediator());
				facade.sendNotification(FriendCommandList.SHOW_FONT_COLOR,{x:this.basePanel.x+58-21,y:this.basePanel.y+160+71});
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
				facade.sendNotification(EventList.SHOWFACEVIEW,{x:this.basePanel.x+8+60,y:this.basePanel.y+128+85,type:"friend"});
			}else{
				facade.sendNotification(EventList.HIDESELECTFACE);
			}
		}
		
		/**
		 * 检测字符输入长度 
		 * @param e
		 * 
		 */		
		protected function onTextChangeHandler(e:Event):void{
			var msg:String=(this.view.txt_chatInput as TextField).text;
			(this.view.txt_chatInput as TextField).text=UIUtils.getTextByCharLength(msg,127);
		}
		
		/**
		 * 关闭 
		 * @param e
		 * 
		 */		
		protected function panelCloseHandler(e:Event):void{
			if(this.view.contains(this.chatHistoryView)){
				this.view.removeChild(this.chatHistoryView);
				this.view.addChild(this.chatInfoView);
				this.basePanel.setWidth(475);
				
			}
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			dataProxy.FriendReveiveMsgIsOpen=false;
			UIConstData.FocusIsUsing = false;
			
			this.roleInfo = null;
			(this.view.txt_chatInput as TextField).text="";
		}
	}
}