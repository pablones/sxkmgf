package GameUI.Modules.PlayerInfo.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.PlayerInfo.Command.TeamEvent;
	import GameUI.Modules.PlayerInfo.UI.TeamList;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	import GameUI.Proxy.DataProxy;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TeamListInfoMediator extends Mediator
	{
		public static const NAME:String="TeamListInfoMediator";
		public static const DEFAULT_POS:Point=new Point(5,120);
		
		protected var tl:TeamList;
		
		public function TeamListInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get TeamListInfoUI():TeamList{
			return tl;
		}
		
		public override function listNotificationInterests():Array{
			return [PlayerInfoComList.INIT_PLAYERINFO_UI,
			PlayerInfoComList.UPDATE_TEAM,
			EventList.ENTERMAPCOMPLETE,
			PlayerInfoComList.UPDATE_TEAM_UI,
			PlayerInfoComList.SHOW_TEAM_UI,
			PlayerInfoComList.HIDE_TEAM_UI,
			EventList.MEMBER_ONLINE_STATUS_TEAM
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case PlayerInfoComList.INIT_PLAYERINFO_UI:
					
					this.tl=new TeamList();
					this.tl.addEventListener(TeamEvent.CELL_CLICK,onCellClickHandler);
					break;
				case PlayerInfoComList.UPDATE_TEAM:
					initData();
					break;	
					
				case EventList.ENTERMAPCOMPLETE:
					GameCommonData.GameInstance.GameUI.addChild(this.tl);
					this.tl.x=DEFAULT_POS.x;
					this.tl.y=DEFAULT_POS.y;
					initSet();
					initData(); 
					break;	
				case PlayerInfoComList.UPDATE_TEAM_UI:
					this.upDateTeam(notification.getBody()["id"]);
					break;	
				case PlayerInfoComList.SHOW_TEAM_UI:
					if(this.tl!=null){
						this.tl.visible=true;
					}
					break;
				case PlayerInfoComList.HIDE_TEAM_UI:	
					if(this.tl!=null){
						this.tl.visible=false;
					}	
					break;
				case EventList.MEMBER_ONLINE_STATUS_TEAM:
					this.tl.updateStatus(notification.getBody()["id"],notification.getBody()["state"]);
					break;	
				case PlayerInfoComList.MSG_LEAVE_TEAM:
					TeamDataProxy.teamDataDic=new Dictionary(true);
					break;
			}
		}
		
		/** 更新指定ID的队友属性  */
		protected function upDateTeam(id:uint):void{
			var index:int=this.indexOf(this.tl.dataPro,id);
			if(index>=0){
				if(GameCommonData.SameSecnePlayerList[id]==null)return;
				this.tl.dataPro[index]=(GameCommonData.SameSecnePlayerList[id] as GameElementAnimal).Role;
				this.tl.upDate(this.tl.dataPro);
			}
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			dataProxy.roleDatas=this.tl.dataPro;	
		}
		
		protected function onCellClickHandler(e:TeamEvent):void{
			switch (e.flagStr){
				case GameCommonData.wordDic[ "mod_pla_med_tea_onc_1" ]:   // 离开队伍
					facade.sendNotification(EventList.LEAVETEAMCOMMON);
					break;
				case GameCommonData.wordDic[ "mod_pla_med_tea_onc_2" ]:  // 移出队伍
					facade.sendNotification(EventList.KICKOUTTEAMCOMMON, {id:e.role.Id}); 
					break;
				case GameCommonData.wordDic[ "mod_pla_med_tea_onc_3" ]:  // 提升为队长
					facade.sendNotification(EventList.CHANGELEADERTEAMCOMMON,{id:e.role.Id});
					break;
				case GameCommonData.wordDic[ "mod_pla_med_tea_onc_4" ]:  // 加为好友
					sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:e.role.Id,name:e.role.Name});
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_3" ]:  // 设为私聊
					facade.sendNotification(ChatEvents.QUICKCHAT,e.role.Name);
					break;
				case GameCommonData.wordDic[ "often_used_trade" ]:   // 交易
					sendNotification(EventList.APPLYTRADE, {id:e.role.Id});
					break;	
			}
		}
		
		/**
		 * 初始化设置 
		 * 
		 */		
		protected function initSet():void{
			
		}
		
		/**
		 * 初始化数据 
		 * 
		 */		
		protected function initData():void{
			
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var oldDataPro:Array=this.tl.dataPro;
			var newDataPro:Array=[];
			var roleData:Array=[];
			
			var statusDic:Dictionary=new Dictionary();
			var count:uint=0;
			for (var id:* in GameCommonData.TeamPlayerList){
				if(uint(id)==GameCommonData.Player.Role.Id)continue;
				count++;
				statusDic[id]=GameCommonData.TeamPlayerList[id]["state"];
				var dataObj:Object=GameCommonData.TeamPlayerList[id];
				var element:GameElementAnimal=GameCommonData.SameSecnePlayerList[id] as GameElementAnimal;
				if(element==null){
					var role:GameRole;
					if(this.indexOf(oldDataPro,id)!=-1){
						if(dataObj.isCaption==true){
							role=oldDataPro[this.indexOf(oldDataPro,id)];
							role.idTeam=GameCommonData.Player.Role.idTeam;
							role.idTeamLeader=id;
							newDataPro.unshift(role);			
						}else{
							role=oldDataPro[this.indexOf(oldDataPro,id)];
							role.idTeam=GameCommonData.Player.Role.idTeam;
							role.idTeamLeader=0;
							newDataPro.push(role);
						}
					}else{
						var obj:Object=this.searchRole(id);
						role=new GameRole();
						role.Id=obj.id;
						role.Name=obj.szName;
						role.Face=obj.dwLookFace;
						role.Level=obj.usLev;
						role.MainJob.Job=obj.usPro;
						role.MainJob.Level=obj.usProLev;
						if(dataObj.isCaption==true){
							role.idTeam=GameCommonData.Player.Role.idTeam;
							role.idTeamLeader=obj.id;
							newDataPro.unshift(role);		
						}else{
							role.idTeam=GameCommonData.Player.Role.idTeam;
							role.idTeamLeader=0;
							newDataPro.push(role);
						}
					}
				}else{
					if(dataObj.isCaption==true){
						newDataPro.unshift(element.Role);						
					}else{
						newDataPro.push(element.Role);
					}	
				}
			}
			
			if(count>0){
				sendNotification(PlayerInfoComList.SHOW_EXPANDTEAM_ICON);
			}else{
				sendNotification(PlayerInfoComList.HIDE_EXPANDTEAM_ICON);
			}
			
			
			this.tl.dataPro=newDataPro; 
			dataProxy.roleDatas=newDataPro;	
			facade.sendNotification(EventList.ITEMREMOVED,{flag:"Role"});	
			for(var i:* in statusDic){
				this.tl.updateStatus(i,statusDic[i]);
			}
		}

		protected function searchRole(id:uint):Object{
			var dataProxy:TeamDataProxy=facade.retrieveProxy(TeamDataProxy.NAME) as TeamDataProxy;
			var arr:Array=dataProxy.teamMemberList;
			for each(var obj:Object in arr){
				if(obj.id==id){
					return obj;
				}			
			}
			return null;
		}
		
		/**
		 * 寻找元素的下标 ,没有找到返回-1
		 * @param arr
		 * @param id
		 * @return 
		 * 
		 */		
		protected function indexOf(arr:Array,id:uint):int{
			var len:uint=arr.length;
			for(var i:int=0;i<len;i++){
				var role:GameRole=arr[i] as GameRole;
				if(role.Id==id)return i;
			}
			return -1;
		}
	}
}