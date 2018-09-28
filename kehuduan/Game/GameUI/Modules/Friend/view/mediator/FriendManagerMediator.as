package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.command.ReceiveMsgCommand;
	import GameUI.Modules.Friend.command.SelectElementCommand;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Friend.view.ui.UINewBlackListContainer;
	import GameUI.Modules.Friend.view.ui.UINewEnemyContainer;
	import GameUI.Modules.Friend.view.ui.UINewFriendContainer;
	import GameUI.Modules.Friend.view.ui.UINewSysMsgContainer;
	import GameUI.Modules.Friend.view.ui.UINewTempContainer;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.SystemMessage.Data.SysMessageData;
	import GameUI.Modules.SystemMessage.Data.SysMessageEvent;
	import GameUI.Modules.SystemMessage.Memento.MessageMemento;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	
	import Net.ActionSend.FriendSend;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.system.System;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class FriendManagerMediator extends Mediator
	{
		
		public static const NAME:String="FriendManagerMediator";
		
		/** 好友管理弹出的默认位置 */
		public static const FRIENDDEFAULTPOS:Point=new Point(UIConstData.DefaultPos2.x, UIConstData.DefaultPos2.y);
		/** 好友管理面板 */	
		protected var panelBase:PanelBase;
		/** 公共数据管理 */
		protected var dataProxy:DataProxy;
		/** 好友列表容器 */
		protected var fContainer:UINewFriendContainer;
		/** 仇人列表容器 */
		protected var enemyContainer:UINewEnemyContainer;
		/** 黑名单列表容器 */
		protected var blackListContainer:UINewBlackListContainer;
		/** 临时好友列表容器 */
		protected var tempContainer:UINewTempContainer;
		
		protected var sysMsgContainer:UINewSysMsgContainer;
		
		/** 好友列表 */
		public var dataList:Array=[[],[],[],[],[],[]]; 
		/** 仇人列表 */
		protected var enemyList:Array=[];
		/** 黑名单列表 */
		protected var blackList:Array = [];
		/** 临时好友列表 */
		protected var tempList:Array = [];
		
		protected var flagShow:Boolean=false;
		
		protected var roleInfo:PlayerInfoStruct;
		
		private var confirm:Function = deleteFriend;
		
		private var isLockFeelStatus:Boolean=false;
		
		private var isNewFriendView:Boolean = true;
		
		private var loader:Loader;
		
		private var currentMc:String;
		
		public function FriendManagerMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get friendManager():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function onRegister():void
		{
			if(this.isNewFriendView == true){
				loadNewFrienfView();
			}
		}
		
		/**
		 * 监听列表 
		 * @return 
		 * 
		 */		
		override public function listNotificationInterests():Array{
			return [EventList.INITVIEW,
					FriendCommandList.SHOWFRIEND,
					FriendCommandList.HIDEFRIEND,
					FriendCommandList.UPDATE_FRIEND_LIST,
					FriendCommandList.CHANGE_GROUP,
					FriendCommandList.RESET_FRIEND_LIST,
					FriendCommandList.INVATE_TO_FRIEND,
					FriendCommandList.DELETE_FRIEND_SUCCESS,
					FriendCommandList.EDIT_FEEL_SUCCESS,
					FriendCommandList.CHANGE_FRIEND_FEEL,
					FriendCommandList.CHANGE_FRIEND_ONLINE,
					FriendCommandList.ADD_TEMP_FRIEND,
					FriendCommandList.ADD_TO_FRIEND,
					FriendCommandList.FRIEND_TEAM_UPDATE,
					EventList.UPDATE_FEEL_STATUS,
					FriendCommandList.FRIEND_INFO_CLEAR,
					FriendCommandList.CHANGE_FRIEND_FACE
					];
		}
		
		override public function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW:
					this.facade.registerMediator(new PlayerInfoMediator(PlayerInfoMediator.NAME));
					this.facade.registerCommand(CommandList.FRIEND_CHAT_MESSAGE,ReceiveMsgCommand);
					this.facade.registerCommand(PlayerInfoComList.SELECT_ELEMENT,SelectElementCommand);
					this.facade.registerMediator(new FriendAlterMediator());
				   	this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					break;
				case FriendCommandList.SHOWFRIEND:
					var str:String = notification.getBody() as String;
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = UIConstData.DefaultPos2.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						panelBase.y = UIConstData.DefaultPos2.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}else{
						panelBase.x = UIConstData.DefaultPos2.x;
						panelBase.y = UIConstData.DefaultPos2.y;
					}
					if(str == SysMessageEvent.SHOWMESSAGEVIEW){
						GameCommonData.GameInstance.GameUI.addChild(panelBase);
						this.mcChange(5);
						dataProxy.FriendsIsOpen = true;						
					}
					if(dataProxy.FriendsIsOpen == false && SysMessageData.messageListIsOpen == false){
					GameCommonData.GameInstance.GameUI.addChild(panelBase);
					//initView();
					this.mcChange(1);
					dataProxy.FriendsIsOpen = true;
					}
					break;
				case FriendCommandList.HIDEFRIEND:
					this.panelCloseHandler(null);
					break;
				//更新好友列表	(等同于添加好友);
				case FriendCommandList.UPDATE_FRIEND_LIST :
					 var reloInfo:PlayerInfoStruct=notification.getBody() as PlayerInfoStruct;		
					 if(reloInfo.friendGroupId==7){//仇人
					 	enemyList.push({name1:reloInfo.roleName,name2:reloInfo.feel,roleInfo:reloInfo});
						this.enemyContainer.fListDataPro = this.enemyList;
					 }else if(reloInfo.friendGroupId <= 4){
						(dataList[reloInfo.friendGroupId-1] as Array).push({name1:reloInfo.roleName,name2:reloInfo.feel,roleInfo:reloInfo});
						this.fContainer.fListDataPro=this.dataList;
					 }else if(reloInfo.friendGroupId == 6){
					 	(dataList[reloInfo.friendGroupId-1] as Array).push({name1:reloInfo.roleName,name2:reloInfo.feel,roleInfo:reloInfo});
					 	this.blackListContainer.fListDataPro = (dataList[reloInfo.friendGroupId-1] as Array);
					 }else if(reloInfo.friendGroupId == 5){
					 	(dataList[reloInfo.friendGroupId-1] as Array).push({name1:reloInfo.roleName,name2:reloInfo.feel,roleInfo:reloInfo});
					 	this.tempContainer.fListDataPro = (dataList[reloInfo.friendGroupId-1] as Array);
					 }
					break;
				//更改好友分组	
				case FriendCommandList.CHANGE_GROUP:
					var temp:Object={};
					for (var i:uint=0;i<this.dataList.length;i++){
						for(var j:uint=0;j<this.dataList[i].length;j++){
							if((this.dataList[i][j].roleInfo as PlayerInfoStruct).frendId==notification.getBody()["friendId"]){
								temp=dataList[i][j];
								(dataList[i] as Array).splice(j,1);
								break;
							}
						}
					}
					var newGroupId:uint=notification.getBody()["newGroupID"];
					(temp["roleInfo"] as PlayerInfoStruct).friendGroupId=newGroupId+1;
					if((temp["roleInfo"] as PlayerInfoStruct).friendGroupId==7){
						this.enemyList.push(temp);
						this.enemyContainer.fListDataPro =enemyList;
					}
					if((temp["roleInfo"] as PlayerInfoStruct).friendGroupId <= 4)
					{
						(dataList[newGroupId] as Array).push(temp);
						this.fContainer.fListDataPro=dataList;
						this.tempContainer.fListDataPro = (dataList[4] as Array);
						this.blackListContainer.fListDataPro = (dataList[5] as Array);							
					}
					if((temp["roleInfo"] as PlayerInfoStruct).friendGroupId == 5){
						(dataList[newGroupId] as Array).push(temp);
						this.fContainer.fListDataPro=dataList;	
						this.tempContainer.fListDataPro = (dataList[newGroupId] as Array);
						this.blackListContainer.fListDataPro = (dataList[newGroupId + 1] as Array);
					}
					if((temp["roleInfo"] as PlayerInfoStruct).friendGroupId == 6){
						(dataList[newGroupId] as Array).push(temp);
						this.fContainer.fListDataPro=dataList;
						this.tempContainer.fListDataPro = (dataList[newGroupId - 1] as Array);
						this.blackListContainer.fListDataPro = (dataList[newGroupId] as Array);
					}
					updateCurrentPage();
					break;	
				case FriendCommandList.RESET_FRIEND_LIST:
					 dataList=[[],[],[],[],[],[]];
					 enemyList=[];  
					break;	
				case FriendCommandList.INVATE_TO_FRIEND:
					var temp11:Object=notification.getBody();
					this.roleInfo=notification.getBody() as PlayerInfoStruct;
					sendNotification(FriendCommandList.SHOW_FRIEND_ALTER,this.roleInfo);
					break;	
				//删除好友（删除仇人）	
				case FriendCommandList.DELETE_FRIEND_SUCCESS:
					var role:PlayerInfoStruct=notification.getBody() as PlayerInfoStruct
					if(role.friendGroupId==7){
						for(var n:uint=0;n<this.enemyList.length;n++){
							if((this.enemyList[n].roleInfo as PlayerInfoStruct).frendId==role.frendId){
								this.enemyList.splice(n,1);
								this.enemyContainer.fListDataPro = this.enemyList;
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_6" ]+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_7" ]+'</font><font color="#ff0000">['+role.roleName+']</font>', color:0xffff00});//你成功的删除了仇人
								return;
							}
						}
					}
					var groupCount:int;
					for (var k:uint=0;k<this.dataList.length;k++){
						for(var l:uint=0;l<this.dataList[k].length;l++){
							if((this.dataList[k][l].roleInfo as PlayerInfoStruct).frendId==role.frendId){
								temp=dataList[k][l];
								(dataList[k] as Array).splice(l,1);
								groupCount = k + 1;
								break;
							}
						}
					}
					if(groupCount <= 4){
						this.fContainer.fListDataPro=dataList;
					}
					if(groupCount == 5){
						this.tempContainer.fListDataPro=dataList[groupCount - 1] as Array;
					}
					if(groupCount == 6){
						this.blackListContainer.fListDataPro = dataList[groupCount - 1] as Array;
					}
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_6" ]+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_9" ]+'</font><font color="#ff0000">['+role.roleName+']</font>', color:0xffff00});//你成功的删除了好友
					break;
				//修改心情成功	
				case FriendCommandList.EDIT_FEEL_SUCCESS:
					friendManager.txt_InputIdea.text=GameCommonData.Player.Role.Feel;
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_8" ], color:0xffff00});//"你成功的修改了自己的心情"  
					break;
				//修改好友（仇人）的心情	
				case FriendCommandList.CHANGE_FRIEND_FEEL:
					var changeRoleInfo:PlayerInfoStruct=notification.getBody() as PlayerInfoStruct;
					var enemyObjFeel:Object=this.searchEnemyById(changeRoleInfo.frendId);
					if(enemyObjFeel!=null){
						enemyObjFeel.roleInfo.feel=changeRoleInfo.feel;
						enemyObjFeel.name2=changeRoleInfo.feel;
						this.enemyContainer.fListDataPro = this.enemyList;
						return;
					}
					
					var obj:Object=this.searchFriend(this.dataList,changeRoleInfo.frendId);
					if(obj!=null){
						this.dataList[obj.i][obj.j].name2=changeRoleInfo.feel;
						(this.dataList[obj.i][obj.j].roleInfo as PlayerInfoStruct).feel=changeRoleInfo.feel;	
					}
					if(obj.i <= 3){
						this.fContainer.fListDataPro=dataList;
					}
					if(obj.i == 4){
						this.tempContainer.fListDataPro=dataList[obj.i];
					}
					if(obj.i == 5){
						this.blackListContainer.fListDataPro=dataList[obj.i];
					}
					break;
				//修改好头像		
				case FriendCommandList.CHANGE_FRIEND_FACE:
					var changeFaceRoleInfo:PlayerInfoStruct=notification.getBody() as PlayerInfoStruct;
					var enemyObjFace:Object=this.searchEnemyById(changeFaceRoleInfo.frendId);
					if(enemyObjFace!=null){
						enemyObjFace.roleInfo.face=changeFaceRoleInfo.face
						this.enemyContainer.fListDataPro = this.enemyList;
						return;
					}
					var objFace:Object=this.searchFriend(this.dataList,changeFaceRoleInfo.frendId);
					if(objFace!=null){
						(this.dataList[objFace.i][objFace.j].roleInfo as PlayerInfoStruct).face=changeFaceRoleInfo.face;	
					}
					if(objFace.i <= 3){
						this.fContainer.fListDataPro=dataList;
					}
					if(objFace.i == 4){
						this.tempContainer.fListDataPro=dataList[objFace.i];
					}
					if(objFace.i == 5){
						this.blackListContainer.fListDataPro=dataList[objFace.i];
					}
					break;	
				//修改好友(仇人)上下线	
				case FriendCommandList.CHANGE_FRIEND_ONLINE:
					var onlineRoleInfo:PlayerInfoStruct=notification.getBody() as PlayerInfoStruct;
					var enemyObj:Object=this.searchEnemyById(onlineRoleInfo.frendId);
					if(enemyObj!=null){
						enemyObj.roleInfo.isOnline=onlineRoleInfo.isOnline;
						this.enemyContainer.fListDataPro = this.enemyList;
						if(onlineRoleInfo.isOnline){
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_7" ]+'</font><font color="#ff0000">['+onlineRoleInfo.roleName+']</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_10" ]+'</font>', color:0xffff00});//仇人 	上线了
						}else{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_7" ]+'</font><font color="#ff0000">['+onlineRoleInfo.roleName+']</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_11" ]+'</font>', color:0xffff00});//仇人		下线了
						}
						return;
					}
					
					var objOline:Object=this.searchFriend(this.dataList,onlineRoleInfo.frendId);
					if(objOline!=null){
						this.dataList[objOline.i][objOline.j].roleInfo.isOnline=onlineRoleInfo.isOnline;
					}
					if(objOline.i <= 3){
						this.fContainer.fListDataPro=dataList;
					}
				    if(objOline.i == 4){
						this.tempContainer.fListDataPro=dataList[objOline.i];
					}
					if(objOline.i == 5){
						this.blackListContainer.fListDataPro=dataList[objOline.i];
					}
					if(onlineRoleInfo.isOnline){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_9" ]+'</font><font color="#ff0000">['+onlineRoleInfo.roleName+']</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_10" ]+'</font>', color:0xffff00});//好友		上线了   	
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_9" ]+'</font><font color="#ff0000">['+onlineRoleInfo.roleName+']</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_11" ]+'</font>', color:0xffff00});//好友		下线了
					}
					break;
				//添加临时好友	
				case FriendCommandList.ADD_TEMP_FRIEND:
					var roleInfo1:PlayerInfoStruct=notification.getBody() as PlayerInfoStruct; 
					var resultObj:Object=this.searchFriend(this.dataList,0,0,roleInfo1.roleName);
					//好友列表中没有此人（包括临时列表）
					if(resultObj==null){
						FriendSend.getInstance().addFriendAction(roleInfo1.frendId,5,roleInfo1.roleName);
					}else if(resultObj.i==4){
						//ToDO已经有好友了
					}					
					break;	
				//添加好友		
				case FriendCommandList.ADD_TO_FRIEND:
					var id:uint=notification.getBody()["id"];
					var name:String=notification.getBody()["name"];
					if(!this.checkHasTheFriend(name,id)){
						var groupId:uint=this.seachGroupID();
						if(groupId>=0){
							FriendSend.getInstance().addFriendAction(id,(groupId +1),name);
						}	
					}
					break;	
				case FriendCommandList.FRIEND_TEAM_UPDATE:
					var o:Object=notification.getBody();
					var resultObject:Object=this.searchFriend(this.dataList,0,0,this.fContainer.menu.roleInfo.roleName);
					if(resultObject==null)return;
					(this.dataList[resultObject.i][resultObject.j].roleInfo as PlayerInfoStruct).idTeam=o.idTeam;
					(this.dataList[resultObject.i][resultObject.j].roleInfo as PlayerInfoStruct).idTeam=o.idTeamleader;
					this.fContainer.showMenu(this.dataList[resultObject.i][resultObject.j].roleInfo);
					break;	
				case EventList.UPDATE_FEEL_STATUS:
					var status:uint=uint(notification.getBody());
					this.isLockFeelStatus=false;
					if(status==0){
						(friendManager.txt_show as TextField).text=GameCommonData.wordDic[ "mod_des_med_des_initT_2" ];//"隐藏";
					}else{
						(friendManager.txt_show as TextField).text=GameCommonData.wordDic[ "mod_des_med_des_initT_1" ];//"显示";
					}
					break;
				//清空好友列表	（清空仇人列表）	
				case FriendCommandList.FRIEND_INFO_CLEAR:
					dataList=[[],[],[],[],[],[]]; 
					this.enemyList=[];
					this.fContainer.fListDataPro=dataList;
					break;					
			}
		}
		
		protected function loadNewFrienfView():void
		{
			//this.loader = new Loader();
			//this.loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoadComplete);
			//this.loader.load(new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf?" + Math.random()));
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf",onLoadComplete);
		}
		
		protected function onLoadComplete():void
		{
			var friendSwf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf");	
			//var domain:ApplicationDomain = this.loader.contentLoaderInfo.applicationDomain;
			//var mainFriendView:MovieClip = new (domain.getDefinition("mainFriendView"))();
			var mainFriendView:MovieClip = new (friendSwf.loaderInfo.applicationDomain.getDefinition("mainFriendView"));
			this.setViewComponent(mainFriendView);
			init();
			//loader.contentLoaderInfo.removeEventListener(Event.COMPLETE,onLoadComplete);
			//loader.unload();
			//loader = null;			
		}
		
		protected function init():void
		{
			this.friendManager.mouseEnabled=false;
			this.panelBase=new PanelBase(this.friendManager,this.friendManager.width+8,this.friendManager.height+12);
			this.panelBase.addEventListener(Event.CLOSE,panelCloseHandler);
			
			panelBase.name="Friend";
			friendManager.name = "mc_page1";
			friendManager.gotoAndStop(1);
			friendManager.mc_page1.gotoAndStop(1);
			friendManager.mc_page2.gotoAndStop(2);
			friendManager.mc_page3.gotoAndStop(2);
			friendManager.mc_page4.gotoAndStop(2);
			friendManager.mc_page5.gotoAndStop(2);
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_1" ]);//"好 友"
			//(friendManager.txt_InputIdea as TextField).addEventListener(Event.CHANGE,onTextInputHandler);
			//UIUtils.addFocusLis(friendManager.txt_InputIdea);	//焦点管理
					
			var tf:TextFormat = new TextFormat();
			tf.align = TextFormatAlign.CENTER;
			friendManager.txt_InputIdea.mouseEnabled=false;
			(friendManager.txt_InputIdea as TextField).defaultTextFormat=tf;
//			this.friendManager.txt_editAndOk.text=GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_5" ];//"修改";	
										
			this.fContainer = new UINewFriendContainer(friendManager);
			this.fContainer.addEventListener(MenuEvent.Cell_Click,onMenuItemCellClick);
			this.fContainer.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onCellDoubleClick);
			
			this.enemyContainer = new UINewEnemyContainer(friendManager);
			this.enemyContainer.addEventListener(MenuEvent.Cell_Click,onEnemyItemCellClick);
			this.enemyContainer.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onCellDoubleClick);
			
			this.blackListContainer = new UINewBlackListContainer(friendManager);
			this.blackListContainer.addEventListener(MenuEvent.Cell_Click,onMenuItemCellClick);
			this.blackListContainer.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onCellDoubleClick);			
			
			this.tempContainer = new UINewTempContainer(friendManager);
			this.tempContainer.addEventListener(MenuEvent.Cell_Click,onMenuItemCellClick);
			this.tempContainer.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onCellDoubleClick);				
			
			this.sysMsgContainer = new UINewSysMsgContainer(friendManager);
						
			//this.friendManager.addChild(this.fContainer);
			
		}
		
		/**
		 * 页签点击 
		 * @param e
		 * 
		 */		
		private function onMcChange(e:MouseEvent):void
		{
			//this.gc();
			switch(e.target.name)
			{
				case "mc_page1":
					if(this.friendManager.name != "mc_page1")
					{
						this.gc();
						mcChange(1);
					}
					break;
				case "mc_page2":
					if(this.friendManager.name != "mc_page2")
					{
						this.gc();
						mcChange(2);
					}
					break;
				case "mc_page3":
					if(this.friendManager.name != "mc_page3")
					{
						this.gc();
						mcChange(3);
					}
					break;
				case "mc_page4":
					if(this.friendManager.name != "mc_page4")
					{
						this.gc();
						mcChange(4);
					}
					break;
				case "mc_page5":
					if(this.friendManager.name != "mc_page5")
					{
						this.gc();
						mcChange(5);
					}
					break;																
			}
		}
		
		private function gc():void
		{
			if(this.friendManager.name == "mc_page1")
			{
				delLis();
				if(this.fContainer.contains(this.fContainer.shape))
				{
					this.fContainer.removeChild(this.fContainer.shape);	
				}			
				this.friendManager.removeChild(this.fContainer);
			}
			if(this.friendManager.name == "mc_page2")
			{
				delLis();
				if(this.tempContainer.contains(this.tempContainer.shape))
				{
					this.tempContainer.removeChild(this.tempContainer.shape);	
				}					
				this.friendManager.removeChild(this.tempContainer);
			}
			if(this.friendManager.name == "mc_page4")
			{
				delLis();
				if(this.enemyContainer.contains(this.enemyContainer.shape))
				{
					this.enemyContainer.removeChild(this.enemyContainer.shape);	
				}				
				this.friendManager.removeChild(this.enemyContainer);
			}
			if(this.friendManager.name == "mc_page3")
			{
				delLis();
				if(this.blackListContainer.contains(this.blackListContainer.shape))
				{
					this.blackListContainer.removeChild(this.blackListContainer.shape);	
				}				
				this.friendManager.removeChild(this.blackListContainer);
			}
			if(this.friendManager.name == "mc_page5")
			{
				delLis();
				if(this.sysMsgContainer.contains(this.sysMsgContainer.shape))
				{
					this.sysMsgContainer.removeChild(this.sysMsgContainer.shape);	
				}				
				this.friendManager.removeChild(this.sysMsgContainer);
			}							
		}
		
		/**
		 * 切换页面 
		 * @param page
		 * 
		 */		
		private function mcChange(page:int):void
		{
			friendManager.name = "mc_page" + page.toString();
			friendManager.gotoAndStop(page);
			if(page == 1){
				initView();
				addLis();
				this.friendManager.addChild(this.fContainer);
				this.fContainer.setPage();
				friendManager.mc_page1.gotoAndStop(1);
			}else {
				friendManager.mc_page1.gotoAndStop(2);
			}
			if(page == 2){
				initView();
				addLis();
				this.friendManager.addChild(this.tempContainer);
				this.tempContainer.setPage();
				friendManager.mc_page2.gotoAndStop(1);
			} else {
				friendManager.mc_page2.gotoAndStop(2);
			}
			if(page == 3){
				initView();
				addLis();
				this.blackListContainer.setPage();
				this.friendManager.addChild(this.blackListContainer);
				friendManager.mc_page3.gotoAndStop(1);
			} else {
				friendManager.mc_page3.gotoAndStop(2);
			}
			if(page == 4){
				initView();
				addLis();
				this.enemyContainer.setPage();			
				this.friendManager.addChild(this.enemyContainer);
				friendManager.mc_page4.gotoAndStop(1);
			} else {
				friendManager.mc_page4.gotoAndStop(2);
			}
			if(page == 5){			
				initView();
				addLis();
				this.sysMsgContainer.fListDataPro = SysMessageData.messageList;
				this.sysMsgContainer.setPage();
				this.friendManager.addChild(this.sysMsgContainer);
				friendManager.mc_page5.gotoAndStop(1);
			} else {
				friendManager.mc_page5.gotoAndStop(2);
			}			
		}

		/**
		 *更新当前页面的页数 
		 * 
		 */
		private function updateCurrentPage():void
		{
			switch(this.friendManager.name){
				case "mc_page1":
					this.fContainer.setPage();
					break;
				case "mc_page2":
					this.tempContainer.setPage();
					break;					
				case "mc_page3":
					this.blackListContainer.setPage();
					break;
				case "mc_page4":
					this.enemyContainer.setPage();
					break;
				case "mc_page5":
					this.sysMsgContainer.setPage();
					break;																
			}
		}
		
		private function addLis():void
		{
			//if(this.friendManager.name == "mc_page1"){
				UIUtils.addFocusLis(friendManager.txt_InputIdea);
				(this.friendManager.txt_InputIdea as TextField).addEventListener(Event.CHANGE,onTextInputHandler);				
				(this.friendManager.btn_Edit as SimpleButton).addEventListener(MouseEvent.CLICK,onEditIdeaHandler);
				(this.friendManager.btn_Show as SimpleButton).addEventListener(MouseEvent.CLICK,onShowIdeaHandler);
			//}
			if(this.friendManager.name == "mc_page5"){
				(this.friendManager.btnSysMegOpen as SimpleButton).addEventListener(MouseEvent.CLICK , openHandler);
				(this.friendManager.btnSysMegClear as SimpleButton).addEventListener(MouseEvent.CLICK , clearHandler);
			}
			(this.friendManager.btn_left as SimpleButton).addEventListener(MouseEvent.CLICK,onPageLeft);
			(this.friendManager.btn_right as SimpleButton).addEventListener(MouseEvent.CLICK,onPageRight);
			(this.friendManager.mc_page1 as MovieClip).addEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page2 as MovieClip).addEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page3 as MovieClip).addEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page4 as MovieClip).addEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page5 as MovieClip).addEventListener(MouseEvent.CLICK,onMcChange);				
		}
		
		private function delLis():void
		{
			//if(this.friendManager.name == "mc_page1"){
				UIUtils.removeFocusLis(friendManager.txt_InputIdea);
				(this.friendManager.txt_InputIdea as TextField).removeEventListener(Event.CHANGE,onTextInputHandler);				
				(this.friendManager.btn_Edit as SimpleButton).removeEventListener(MouseEvent.CLICK,onEditIdeaHandler);
				(this.friendManager.btn_Show as SimpleButton).removeEventListener(MouseEvent.CLICK,onShowIdeaHandler);
			//}
			if(this.friendManager.name == "mc_page5"){
				(this.friendManager.btnSysMegOpen as SimpleButton).removeEventListener(MouseEvent.CLICK , openHandler);
				(this.friendManager.btnSysMegClear as SimpleButton).removeEventListener(MouseEvent.CLICK , clearHandler);
			}			
			(this.friendManager.btn_left as SimpleButton).removeEventListener(MouseEvent.CLICK,onPageLeft);
			(this.friendManager.btn_right as SimpleButton).removeEventListener(MouseEvent.CLICK,onPageRight);
			(this.friendManager.mc_page1 as MovieClip).removeEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page2 as MovieClip).removeEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page3 as MovieClip).removeEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page4 as MovieClip).removeEventListener(MouseEvent.CLICK,onMcChange);
			(this.friendManager.mc_page5 as MovieClip).removeEventListener(MouseEvent.CLICK,onMcChange);			
		}
		
		/**
		 * 向上翻页 
		 * @param e
		 * 
		 */		
		private function onPageLeft(e:MouseEvent):void
		{
			if(this.friendManager.name == "mc_page1")
			{
				this.fContainer.pageUp();
			}
			if(this.friendManager.name == "mc_page2")
			{
				this.tempContainer.pageUp();
			}
			if(this.friendManager.name == "mc_page3")
			{
				this.blackListContainer.pageUp();
			}
			if(this.friendManager.name == "mc_page4")
			{
				this.enemyContainer.pageUp();
			}
			if(this.friendManager.name == "mc_page5")
			{
				this.sysMsgContainer.pageUp();
			}	
		}
		
		/**
		 * 向下翻页 
		 * @param e
		 * 
		 */		
		private function onPageRight(e:MouseEvent):void
		{
			if(this.friendManager.name == "mc_page1")
			{
				this.fContainer.pageDown();
			}
			if(this.friendManager.name == "mc_page2")
			{
				this.tempContainer.pageDown();
			}
			if(this.friendManager.name == "mc_page3")
			{
				this.blackListContainer.pageDown();
			}
			if(this.friendManager.name == "mc_page4")
			{
				this.enemyContainer.pageDown();
			}
			if(this.friendManager.name == "mc_page5")
			{
				this.sysMsgContainer.pageDown();
			}				
		}
		
		/** 单击信息条目(委托) */
		public function clickMegItem(item:MessageMemento):void
		{
			SysMessageData.selectItemIndex = item.index;
		}
				
		/** 打开信息条目  (委托)*/
		public function openMegItem():void
		{
			var length:int = SysMessageData.messageList.length;
			if(length == 0){return;}
			//查看消息内容
			if(this.friendManager.name == "mc_page5" && SysMessageData.messageList[SysMessageData.selectItemIndex -1] != null)
			{
				facade.sendNotification(SysMessageEvent.SHOWMESSAGECONTENTVIEW , SysMessageData.messageList[SysMessageData.selectItemIndex -1]);	//弹出消息面板
				SysMessageData.messageList[SysMessageData.selectItemIndex -1].state = GameCommonData.wordDic[ "mod_sysmsg_med_scm_2" ];    //"已读"
				this.sysMsgContainer.fListDataPro = SysMessageData.messageList;
			}
		}
		
		/** 点击打开按钮 */
		private function openHandler(e:MouseEvent):void
		{
			if(SysMessageData.selectItemIndex == 0) return;
			openMegItem();
		}
		
		/** 点击清空按钮 */
		private function clearHandler(e:MouseEvent):void
		{
			SysMessageData.selectItemIndex = 0;
			SysMessageData.messageList = [];
			this.sysMsgContainer.fListDataPro =  SysMessageData.messageList;
			if(this.sysMsgContainer.contains(this.sysMsgContainer.shape))
			{
				this.sysMsgContainer.removeChild(this.sysMsgContainer.shape);	
			}				
			//scrollInit();
		}		
		/**
		 * 查看哪个好友分组有空位 
		 * @return  -1:好友已经达到人数上限
		 * 
		 */		
		public function seachGroupID():int{
			for(var i:uint=0;i<4;i++){
				if((dataList[i] as Array).length<10)return i;
			}
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_sea" ], color:0xffff00});//"好友人数已经达到上限" 
			return -1;
		}
		
		/**
		 * 获取玩家好友总数 
		 * @return 
		 * 
		 */		
		public function getFriendNum():uint{
			var n:uint=0;
			n=this.dataList[0].length+this.dataList[1].length+this.dataList[2].length+this.dataList[3].length;
			return n;	
		}
		
		/**
		 *  
		 * @param id :好友ID 
		 * @param name ：好友Name
		 * @return 
		 * 
		 */		
		public function checkHasTheFriend(name:String,id:int=-1):Boolean{
			if(id==0)id=-1;
			var arr:Array=this.dataList;
			for (var k:uint=0;k<arr.length;k++){
					for(var l:uint=0;l<arr[k].length;l++){
						if((arr[k][l].roleInfo as PlayerInfoStruct).roleName==name ||  (arr[k][l].roleInfo as PlayerInfoStruct).frendId==id){
							if(k==4){
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ff0000">'+name+'</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_che_1" ]+'</font>', color:0xffff00});//已经在临时好友列表中
							}else{
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ff0000">'+name+'</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_che_2" ]+'</font>', color:0xffff00});//已经在好友列表中
							}	
							return true;
					}
				}
			}
			return false;
		}
		
		
		/**
		 * 检查好友列表中是否有该好友 
		 * @param name ：要查询的好友名称
		 * @param id：要查询好友的ID
		 * @return true:有此好友  false:无此好友
		 * 
		 */		
		public function isHasTheFriend(name:String,id:uint):Boolean{
			var arr:Array=this.dataList;
			for (var k:uint=0;k<4;k++){
					for(var l:uint=0;l<arr[k].length;l++){
						if((arr[k][l].roleInfo as PlayerInfoStruct).roleName==name ||  (arr[k][l].roleInfo as PlayerInfoStruct).frendId==id){
							return true;
					}
				}
			}
			return false;
		}
		
		/**
		 * 双击好友 
		 * @param e
		 *  
		 */		
		protected function onCellDoubleClick(e:MenuEvent):void{
			if(e.roleInfo.friendGroupId==5)return;
			if(e.roleInfo.friendGroupId==6){
				FriendSend.getInstance().getFriendInfo(e.roleInfo.frendId,e.roleInfo.roleName);
			}else{
				this.sendNotification(FriendCommandList.SHOW_SEND_MSG,e.roleInfo);
			}
		}
		
		/**
		 * 心情编辑 
		 * @param e
		 * 
		 */			
		protected function onTextInputHandler(e:Event):void{
			var str:String=(friendManager.txt_InputIdea as TextField).text;
			(friendManager.txt_InputIdea as TextField).text=UIUtils.getTextByCharLength(str,15);	
		}
		
		/**
		 * 从数组中搜索指定ID的好友 type:1 根据ID找，0:name找
		 * @param arr
		 * @return 
		 * 
		 */		
		public function searchFriend(arr:Array,id:int,type:uint=1,name:String=""):Object{
		
				for (var k:uint=0;k<arr.length;k++){
					for(var l:uint=0;l<arr[k].length;l++){
						if(type==1){
							if((arr[k][l].roleInfo as PlayerInfoStruct).frendId==id){
								return {i:k,j:l}
							}	
						}else{
							if((arr[k][l].roleInfo as PlayerInfoStruct).roleName==name){
								return {i:k,j:l}
							}
						}
					}
				}
			return null;
		}
		
		/**
		 * 根据ID查找仇人 
		 * @param id
		 * @return 
		 * 
		 */		
		public function searchEnemyById(id:uint):Object{
			for(var n:uint=0;n<this.enemyList.length;n++){
				if((this.enemyList[n].roleInfo as PlayerInfoStruct).frendId==id){
					return this.enemyList[n];
				}
			}
			return null;
		}
		
		/**
		 * 
		 * @param name
		 * @return 
		 * 
		 */		
		public function searchEnemyByName(name:String):Object{
			for(var n:uint=0;n<this.enemyList.length;n++){
				if((this.enemyList[n].roleInfo as PlayerInfoStruct).roleName==name){
					return this.enemyList[n];
				}
			}
			return null;
		}
		
		
		/**
		 *  根据人物的名称去查找人物(包括仇人) 
		 * @return 
		 * 
		 */		
		public function searchPersonByName(name:String):PlayerInfoStruct{
			var obj:Object=this.searchFriend(this.dataList,0,0,name);
			if(obj!=null){
				return this.dataList[obj.i][obj.j].roleInfo as PlayerInfoStruct;
			}else{
				return this.searchEnemyByName(name) as PlayerInfoStruct; 
			}	
		}	
		
		/**
		 * 根据名字判该人是不是在线。。。 
		 * @param name
		 * @return 
		 * 
		 */		
		public function isPersonOnline(name:String):Boolean{
			var player:PlayerInfoStruct=this.searchPersonByName(name);
			if(player!=null)
			{
				if(player.isOnline != 0){
					return true;
				}
			}
			return false; 
		}
		
		
		/**
		 * 确定添加此人为好友 
		 * 
		 */		
		protected function comfirm():void{
			sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:roleInfo.frendId,name:roleInfo.roleName});
		}
		/**
		 * 取消添加此人为好友 
		 * 
		 */		
		protected function cancel():void{
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:'<font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_can_1" ]+'</font><font color="#ff0000">['+roleInfo.roleName+']</font><font color="#ffff00">'+GameCommonData.wordDic[ "mod_fri_view_med_friendM_can_2" ]+'</font>', color:0xffff00});
			//你拒绝添加		为好友
		}
		
		protected function onEnemyItemCellClick(e:MenuEvent):void{
			 if(e.cell.data["type"]==null)return;
			 var sendAction:FriendSend=FriendSend.getInstance();
		 	switch (e.cell.data["type"] as String){
				 case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_1" ]://"查看资料"
					FriendSend.getInstance().getFriendInfo(e.roleInfo.frendId,e.roleInfo.roleName);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]://"发送消息"
					this.sendNotification(FriendCommandList.SHOW_SEND_MSG,e.roleInfo);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_3" ]://"设为私聊"
					facade.sendNotification(ChatEvents.QUICKCHAT,e.roleInfo.roleName); 
					break;	
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ]://"复制名字"
					System.setClipboard(e.roleInfo.roleName);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_5" ]://"删除仇人"
					this.deleteId=e.roleInfo.frendId;
					this.deleteName=e.roleInfo.roleName;
					facade.sendNotification(EventList.SHOWALERT,{comfrim:confirm,cancel:new Function(),info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_6" ]});//"是否确认删除该仇人？"
					break;		
			 }
		}
		
		/**
		 * 选择点击选项 
		 * @param e
		 * 
		 */		
		protected function onMenuItemCellClick(e:MenuEvent):void{
			 if(e.cell.data["type"]==null)return;
			var sendAction:FriendSend=new FriendSend();
			switch (e.cell.data["type"] as String){
				case GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]://"查看资料" 
					FriendSend.getInstance().getFriendInfo(e.roleInfo.frendId,e.roleInfo.roleName);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]://"发送消息"
					this.sendNotification(FriendCommandList.SHOW_SEND_MSG,e.roleInfo);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"1"://好友分组
					if((dataList[0] as Array).length>10){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"1"+GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_1" ], color:0xffff00});//"好友分组1人数已经满，请选择其它分组！"
						return;
					}
					sendAction.changeGroup(e.roleInfo,1);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"2"://好友分组
					if((dataList[1] as Array).length>10){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"2"+GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_1" ], color:0xffff00});//"好友分组2人数已经满，请选择其它分组！"
						return;
					}
					sendAction.changeGroup(e.roleInfo,2);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"3"://好友分组
					if((dataList[2] as Array).length>10){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"3"+GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_1" ], color:0xffff00});//"好友分组3人数已经满，请选择其它分组！"
						return;
					}
					sendAction.changeGroup(e.roleInfo,3);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"4"://好友分组
					if((dataList[3] as Array).length>10){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"4"+GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_1" ], color:0xffff00});//"好友分组4人数已经满，请选择其它分组！"
						return;
					}
					sendAction.changeGroup(e.roleInfo,4);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ]://"黑名单"
					if((dataList[5] as Array).length>20){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_2" ], color:0xffff00});//"黑名单人数已经达到上限"
						return;
					}
					sendAction.changeGroup(e.roleInfo,6);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ]://"删除好友"
					this.deleteId=e.roleInfo.frendId;
					this.deleteName=e.roleInfo.roleName;
					facade.sendNotification(EventList.SHOWALERT,{comfrim:confirm,cancel:new Function(),info:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_9" ]});//"是否确认删除该好友？"
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_4" ]://"添加好友"
					FriendSend.getInstance().changeGroup(e.roleInfo,this.seachGroupID()+1);
					break;	
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ]://"设为私聊"
					facade.sendNotification(ChatEvents.QUICKCHAT,e.roleInfo.roleName); 
					break;	
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_6" ]://"复制名字"
					System.setClipboard(e.roleInfo.roleName);
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ]://"邀请入队"
					sendNotification(EventList.INVITETEAM, {id:e.roleInfo.frendId});
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ]://"申请入队"
					sendNotification(EventList.APPLYTEAM, {id:e.roleInfo.frendId});
					break;		
			} 
		}
		
		
		private var deleteId:uint;
		private var deleteName:String;
		protected function deleteFriend():void{
			FriendSend.getInstance().deleteFriendAction(this.deleteId,this.deleteName);
		}
		
		/**
		 * 初始化 
		 * 
		 */		
		protected function initView():void{
			
			//addLis();
			friendManager.txt_editAndOk.mouseEnabled=false;
			friendManager.txt_InputIdea.type=TextFieldType.DYNAMIC;
			(this.friendManager.txt_InputIdea as TextField).text=GameCommonData.Player.Role.Feel as String;
			this.friendManager.txt_editAndOk.text=GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_5" ];//"修改";
			(this.friendManager.txt_InputIdea as TextField).height = 16;
			//this.friendManager.addChild(this.fContainer);
			(friendManager.txt_show as TextField).mouseEnabled=false;
		}
		
		/**
		 * 修改心情 
		 * @param e
		 * 
		 */			
		protected function onEditIdeaHandler(e:MouseEvent):void{
			var str:String=this.friendManager.txt_editAndOk.text;
			if(str==GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_5" ]){//"修改"
				this.friendManager.txt_editAndOk.text=GameCommonData.wordDic[ "often_used_commit" ];//"确定"
				friendManager.txt_InputIdea.type=TextFieldType.INPUT;
				friendManager.txt_InputIdea.selectable=true;
				friendManager.txt_InputIdea.maxChars=16;
				(friendManager.txt_InputIdea as TextField).scrollH=0;
				(friendManager.txt_InputIdea as TextField).multiline=false;
				friendManager.txt_InputIdea.mouseEnabled=true;
				//friendManager.txt_InputIdea.border=true;
				//friendManager.txt_InputIdea.borderColor=0xffffff;
				//(friendManager.txt_InputIdea as TextField).background=true;
				//(friendManager.txt_InputIdea as TextField).backgroundColor=0x663300;
				(friendManager.txt_InputIdea as TextField).autoSize=TextFieldAutoSize.CENTER;
				(friendManager.txt_InputIdea as TextField).addEventListener(KeyboardEvent.KEY_DOWN,onKeyDownHandler);
				friendManager.stage.focus=friendManager.txt_InputIdea;
				var len:int=friendManager.txt_InputIdea.length;
				(friendManager.txt_InputIdea as TextField).setSelection(len,len);
				return;
			}
			if(str==GameCommonData.wordDic[ "often_used_commit" ]){//"确定"
				(friendManager.txt_InputIdea as TextField).removeEventListener(KeyboardEvent.KEY_DOWN,onKeyDownHandler);
				this.friendManager.txt_editAndOk.text=GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_5" ];//"修改";
				friendManager.txt_InputIdea.type=TextFieldType.DYNAMIC;
				friendManager.txt_InputIdea.mouseEnabled=false;
				(friendManager.txt_InputIdea as TextField).background=false;
			(friendManager.txt_InputIdea as TextField).autoSize=TextFieldAutoSize.CENTER;
				friendManager.txt_InputIdea.border=false;
				if(GameCommonData.Player.Role.Feel!=friendManager.txt_InputIdea.text){
					FriendSend.getInstance().editFeel(GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name,friendManager.txt_InputIdea.text);
					(this.friendManager.txt_InputIdea as TextField).text="";
				}
				return;
			}
		}
		
		protected function onKeyDownHandler(e:KeyboardEvent):void{
			if(e.keyCode==13){
				onEditIdeaHandler(null);
			}
		}
		
		/**
		 * 显示心情
		 * @param e
		 * 
		 */		
		protected function onShowIdeaHandler(e:MouseEvent):void{
			if(this.isLockFeelStatus){
				return;
			}
			
			var str:String=(friendManager.txt_show as TextField).text;
			
			var status:uint=0;
			if(str==GameCommonData.wordDic[ "mod_des_med_des_initT_1" ])//"显示"
			{
				//todo向服务端发送
				status=0;
			}
			else
			{
				 status=1;
			}
			FriendSend.getInstance().sendOnsynFeel(status);
			this.isLockFeelStatus=true;
		}
		
		/**
		 * 对页签的点击处理 
		 * 
		 */		
		protected function onClick(e:MouseEvent):void{
			if(e.target===friendManager.mc_CR){
				this.friendManager.mc_CR.gotoAndStop(1);
				this.friendManager.mc_Friend.gotoAndStop(2);
				if(this.fContainer.parent!=null){
					this.friendManager.removeChild(this.fContainer);
				}
				this.friendManager.addChild(this.enemyContainer); 
			}else{
				this.friendManager.mc_CR.gotoAndStop(2);
				this.friendManager.mc_Friend.gotoAndStop(1);
				if(this.enemyContainer.parent!=null){
					this.friendManager.removeChild(this.enemyContainer);
				}
				this.friendManager.addChild(this.fContainer);
			}
		}
		
		/**
		 * 关闭好友管理面板 处理关闭流程
		 * @param e
		 * 
		 */		
		protected function panelCloseHandler(e:Event):void{
			gc();
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			dataProxy.FriendsIsOpen=false;

			//if(this.friendManager.name == "mc_page1"){
				var str:String=this.friendManager.txt_editAndOk.text;
				if(str==GameCommonData.wordDic[ "often_used_commit" ]){//"确定"
					this.onEditIdeaHandler(null);
				}
			//}
		}
		
		public function get enemyDataList():Array
		{
			return this.enemyList;
		}
		
	}
}