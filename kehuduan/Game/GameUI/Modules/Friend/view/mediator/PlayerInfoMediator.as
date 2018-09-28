package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.ProConversion;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Designation.Data.DesignationProxy;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.vo.RoleLaborStruct;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PlayerInfoMediator extends Mediator
	{
		public static const NAME:String="PlayerInfoMediator";
		public static const FRIENDINFODEFAULTPOS:Point=new Point(400,120);
		
		protected var dataProxy:DataProxy;
		/** 内容显示面板 */
		protected var panelBase:PanelBase; 
		
		protected var roleInfo:RoleLaborStruct;
		
		protected var _isEnemy:Boolean=false;
		
		public function PlayerInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(mediatorName, viewComponent);
		}
		
		public function get friendInfoUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array{
			return [FriendCommandList.SHOW_FRIENDINFO];
		}
		
		override public function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case FriendCommandList.SHOW_FRIENDINFO:
					if(dataProxy!=null && dataProxy.FriendInfoIsOpen){
						this.onClosePanelBase(null);
					}
					this.initView(notification);
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					break;
			}
		}
		
		/**
		 * 关闭 
		 * 
		 */		
		protected function onClosePanelBase(e:Event):void{
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			this.dataProxy.FriendInfoIsOpen=false;
			this.panelBase.removeEventListener(Event.CLOSE,onClosePanelBase);
			var mc:MovieClip=(this.friendInfoUI.mc_headImg as MovieClip);
			while(mc.numChildren>0){
				mc.removeChildAt(0);
			}
				
		}
		
		/**
		 *   初始化好友信息显示组件，将组件加载到内存中 
		 * 
		 */		
		protected function initView(notification:INotification):void{
			
			this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			this.dataProxy.FriendInfoIsOpen=true;
			facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"FriendInfo"});
			this.friendInfoUI.mouseEnabled=false;
			this.panelBase=new PanelBase(this.friendInfoUI,this.friendInfoUI.width+8,this.friendInfoUI.height+12);
			this.panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_fri_view_med_pla_initV" ]);//"玩家信息"
			this.panelBase.x=FRIENDINFODEFAULTPOS.x;
			this.panelBase.y=FRIENDINFODEFAULTPOS.y;
			this.panelBase.addEventListener(Event.CLOSE,onClosePanelBase);
			
			initSet(notification);
			initData(notification);
		}
		
		/**
		 * 初始功能的设定 
		 * 
		 */		
		protected function initSet(notification:INotification):void{
			
			roleInfo=notification.getBody() as RoleLaborStruct;
			
			(this.friendInfoUI.txt_addFriend as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_applyTeam as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_invateTeam as TextField).mouseEnabled=false;
			
			if(this.isEnemy(roleInfo)){
				this._isEnemy=true;
				(this.friendInfoUI.txt_addFriend as TextField).visible=false;
				(this.friendInfoUI.txt_applyTeam as TextField).visible=false;
				(this.friendInfoUI.txt_invateTeam as TextField).visible=false;
				
				(this.friendInfoUI.btn_inviteToTeam as SimpleButton).visible=false;
				(this.friendInfoUI.btn_applyToTeam as SimpleButton).visible=false;
				(this.friendInfoUI.btn_addFriend as SimpleButton).visible=false;
				
				(this.friendInfoUI.btn_inviteToTeam_bg).visible=false;
				(this.friendInfoUI.btn_applyToTeam_bg).visible=false;
				(this.friendInfoUI.btn_addFriend_bg).visible=false;		
			}else{
				this._isEnemy=false;
				(this.friendInfoUI.txt_addFriend as TextField).visible=true;
				(this.friendInfoUI.txt_applyTeam as TextField).visible=true;
				(this.friendInfoUI.txt_invateTeam as TextField).visible=true;
				
				(this.friendInfoUI.btn_inviteToTeam as SimpleButton).visible=true;
				(this.friendInfoUI.btn_applyToTeam as SimpleButton).visible=true;
				(this.friendInfoUI.btn_addFriend as SimpleButton).visible=true;
				
				(this.friendInfoUI.btn_inviteToTeam_bg).visible=true;
				(this.friendInfoUI.btn_applyToTeam_bg).visible=true;
				(this.friendInfoUI.btn_addFriend_bg).visible=true;
			}
			
			(this.friendInfoUI.txt_userName as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_mainProfession as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_avocation as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_relation as TextField).mouseEnabled=false;
			
			(this.friendInfoUI.txt_gradeOne as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_gradeTwo as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_gradeThree as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_friendDeep as TextField).mouseEnabled=false;
			
			(this.friendInfoUI.txt_faction as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_breast as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_nickName as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_userMate as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_userPosition as TextField).mouseEnabled=false;
			(this.friendInfoUI.txt_userTeam as TextField).mouseEnabled=false;
			(this.friendInfoUI.btn_close as SimpleButton).addEventListener(MouseEvent.CLICK,onCloseHandler);
			(this.friendInfoUI.btn_secretChat as SimpleButton).addEventListener(MouseEvent.CLICK,onSecretChatHandler);	
			(this.friendInfoUI.btn_inviteToTeam as SimpleButton).addEventListener(MouseEvent.CLICK,onInviteToTeamHandler);
			(this.friendInfoUI.btn_applyToTeam as SimpleButton).addEventListener(MouseEvent.CLICK,onApplyToTeamHandler);
			(this.friendInfoUI.btn_addFriend as SimpleButton).addEventListener(MouseEvent.CLICK,onAddFriendHandler);
			
			
			
		}
		
		protected function onSecretChatHandler(e:MouseEvent):void{
			facade.sendNotification(ChatEvents.QUICKCHAT, roleInfo.roleName); 
		}
		
		/**
		 * 邀请入队
		 * @param e
		 * 
		 */		
		protected function onInviteToTeamHandler(e:MouseEvent):void{
			sendNotification(EventList.INVITETEAM, {id:roleInfo.friendID});
		}
		
		/**
		 * 申请入队 
		 * @param e
		 * 
		 */		
		protected function onApplyToTeamHandler(e:MouseEvent):void{
			sendNotification(EventList.APPLYTEAM, {id:roleInfo.friendID});
		}
		
		protected function onAddFriendHandler(e:MouseEvent):void{
			sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:-1,name:roleInfo.roleName});
			(this.friendInfoUI.btn_addFriend as SimpleButton).visible=false;
		}
	
		protected function onCloseHandler(e:MouseEvent):void{
			this.onClosePanelBase(null);
		}
		
		/**
		 * 判断是否是仇人 
		 * @param info
		 * 
		 */			
		protected function isEnemy(info:RoleLaborStruct):Boolean{
			var friendMediator:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
			if(friendMediator.searchEnemyByName(info.roleName)){
				return true;
			}
			return false;
		}
		
		/**
		 * 初始化显示数据 
		 * 
		 */		
		protected function initData(notification:INotification):void{
			
		
			var friendMediator:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
			var queryObj:Object=friendMediator.searchFriend(friendMediator.dataList,0,0,roleInfo.roleName);
			
			
			
			//不是好友可以添加好友
			if(queryObj==null || queryObj.i==4){
				(this.friendInfoUI.btn_addFriend as SimpleButton).visible=true;
			}
			
			//有队伍,自己没有队伍
			if(roleInfo.team!=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ]){//"无"
				if(GameCommonData.Player.Role.idTeam==0){
					(this.friendInfoUI.btn_applyToTeam as SimpleButton).visible=true;
					(this.friendInfoUI.btn_inviteToTeam as SimpleButton).visible=false;
				}else{
					(this.friendInfoUI.btn_applyToTeam as SimpleButton).visible=false;
					(this.friendInfoUI.btn_inviteToTeam as SimpleButton).visible=false;
				}
			}else{
				(this.friendInfoUI.btn_applyToTeam as SimpleButton).visible=false;
				(this.friendInfoUI.btn_inviteToTeam as SimpleButton).visible=true;
			}
					
			var head:FaceItem=new FaceItem(String(roleInfo.face),null,"face", 1.0);
			head.offsetPoint=new Point(0,0);
			head.x = 7.25;
			head.y = 7.5;
			(this.friendInfoUI.mc_headImg as MovieClip).addChild(head);
			
			(this.friendInfoUI.txt_userName as TextField).text=roleInfo.roleName;
			(this.friendInfoUI.txt_mainProfession as TextField).text=ProConversion.getInstance().RolesListDic[roleInfo.profession];
			(this.friendInfoUI.txt_avocation as TextField).text=(roleInfo.subWork==0 ? GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ] : ProConversion.getInstance().RolesListDic[roleInfo.subWork]);//"无"
//			(this.friendInfoUI.txt_relation as TextField).text=roleInfo.relation==0 ? GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ] : String(roleInfo.relation);//"无"
			(this.friendInfoUI.txt_relation as TextField).text = GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ]; //无
			
			(this.friendInfoUI.txt_gradeOne as TextField).text=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_2" ]+roleInfo.level;//"级别："
			(this.friendInfoUI.txt_gradeTwo as TextField).text=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_2" ]+roleInfo.professionLevel;//"级别："
			var subStr:String;
			if((this.friendInfoUI.txt_avocation as TextField).text==GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ]){//"无"
				subStr=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_2" ]+GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ];//"级别："	"无"
			}else{
				subStr=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_2" ]+roleInfo.subWorkLevel;//"级别："
			}
			(this.friendInfoUI.txt_gradeThree as TextField).text=subStr;
			(this.friendInfoUI.txt_friendDeep as TextField).text=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_3" ]+roleInfo.friendShip;//"友好度："
			(this.friendInfoUI.txt_faction as TextField).text=roleInfo.faction==null ? GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_4" ] :String(roleInfo.faction);//"无帮派"
			(this.friendInfoUI.txt_breast as TextField).text=roleInfo.feel==null ? GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ] :String(roleInfo.feel);//"无"
			var designation:DesignationProxy=this.facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy;
			(this.friendInfoUI.txt_nickName as TextField).text=designation.getDesignationName(int(roleInfo.title));
			var mateStr:String;
			if(roleInfo.mate=="无" || roleInfo.mate==null || roleInfo.mate=="拸")
			{
				if(GameCommonData.wordVersion == 2)
				{
					mateStr = "無";
				}
				else
				{
					mateStr = GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ];
				}
			}
			else
			{
				mateStr = roleInfo.mate;
			}
			(this.friendInfoUI.txt_userMate as TextField).text=mateStr;
//			(this.friendInfoUI.txt_userMate as TextField).text=roleInfo.mate==null ? mateStr:String(roleInfo.mate);//"无"
			if(roleInfo.mapId==GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_5" ]){//"未知"
				(this.friendInfoUI.txt_userPosition as TextField).text=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_5" ];//"未知"
			}else{
				var arr:Array=roleInfo.mapId.split("_");
				if(arr.length==2){
					(this.friendInfoUI.txt_userPosition as TextField).text=arr[0]+"  "+SmallConstData.getInstance().mapItemDic[arr[1]].name
				}else{
					(this.friendInfoUI.txt_userPosition as TextField).text=GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_5" ];//"未知"
				}
				
			}
			var teamArr:Array=roleInfo.team.split("_");
			if(teamArr.length==2){
				(this.friendInfoUI.txt_userTeam as TextField).text=teamArr[0];
			}else{
				(this.friendInfoUI.txt_userTeam as TextField).text=String(roleInfo.team);
			}
			
			(this.friendInfoUI.btn_close as SimpleButton).mouseEnabled=true;
			
			if(this._isEnemy){
					
				(this.friendInfoUI.btn_inviteToTeam as SimpleButton).visible=false;
				(this.friendInfoUI.btn_applyToTeam as SimpleButton).visible=false;
				(this.friendInfoUI.btn_addFriend as SimpleButton).visible=false;
				
				(this.friendInfoUI.btn_inviteToTeam_bg).visible=false;
				(this.friendInfoUI.btn_applyToTeam_bg).visible=false;
				(this.friendInfoUI.btn_addFriend_bg).visible=false;		
			}
		}
	}
}