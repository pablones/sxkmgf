package GameUI.Modules.PlayerInfo.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Designation.Data.DesignationProxy;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.PlayerInfo.Command.ProcessPlayerDetailCommand;
	import GameUI.Modules.PlayerInfo.UI.EquipmentItem;
	import GameUI.Modules.RoleProperty.Prxoy.ItemUnit;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import OopsEngine.Role.GameRole;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	import org.puremvc.as3.multicore.patterns.observer.Notification;

	public class PlayerDetailInfoMediator extends Mediator
	{
		public static const NAME:String="PlayerDetailInfoMediator";
		public static const DEFAULT_POSTION:Point=new Point(100,100);
		
		protected var panelBase:PanelBase;
		public static var Role:GameRole;
		protected var cells:Array;
		private var dataProxy:DataProxy = null;
		
		public function PlayerDetailInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get detailInfoUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		override public function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case PlayerInfoComList.INIT_PLAYERINFO_UI :
					facade.registerCommand(PlayerInfoComList.SHOW_PLAYER_DETAILINFO,ProcessPlayerDetailCommand);
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"RoleDetailInfo"});
					this.detailInfoUI.mouseEnabled=false;
					this.panelBase=new PanelBase(this.detailInfoUI,this.detailInfoUI.width+8,this.detailInfoUI.height+12);
					this.panelBase.addEventListener(Event.CLOSE,onPanelBaseCloseHandler);
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					break;
				case PlayerInfoComList.SHOW_PLAYER_DETAILINFO_UI:
					if(!dataProxy) dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					removeAll();
					this.panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pla_med_pla_han_1" ] );   //人物资料
					GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
					this.panelBase.x=DEFAULT_POSTION.x;
					this.panelBase.y=DEFAULT_POSTION.y;
					initSet(notification as Notification);
					initData(notification as Notification);
					break;			
			}
		}
		
		protected function removeAll():void{
			for each(var item:EquipmentItem in cells){
				if(item.parent!=null){
					item.parent.removeChild(item);
					item=null;
				}
			}
			this.cells=[];
		}
		
		override public function listNotificationInterests():Array{
			return [PlayerInfoComList.INIT_PLAYERINFO_UI,
			PlayerInfoComList.SHOW_PLAYER_DETAILINFO_UI
			];
		}
		
		protected function initSet(notification:Notification):void{
			(this.detailInfoUI.txt_battleLevel as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_level as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_mainJob as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_subJob as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_faction as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_title as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_feel as TextField).mouseEnabled=false;
			(this.detailInfoUI.txt_mate as TextField).mouseEnabled=false;
			(this.detailInfoUI.btn_setPersonChat as SimpleButton).addEventListener(MouseEvent.CLICK,onMousePersonChatHandler);
			(this.detailInfoUI.btn_addToFriend as SimpleButton).addEventListener(MouseEvent.CLICK,onMouseAddToFriendHandler);
			
		}
		
		protected function onMousePersonChatHandler(e:MouseEvent):void{
			facade.sendNotification(ChatEvents.QUICKCHAT,Role.Name);
		}
		
		protected function onMouseAddToFriendHandler(e:MouseEvent):void{
			facade.sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:Role.Id,name:Role.Name});
		}
		
		protected function initData(notification:Notification):void{
//			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var obj:Object=notification.getBody()
			var equips:Array=obj["equipments"] as Array;
			for(var j:int = 0; j < dataProxy.equipments.length; j++) {	//清除缓存
				if(dataProxy.equipments[j] == undefined) continue;
				var id:uint = dataProxy.equipments[j].id;
				if(IntroConst.ItemInfo[id])
					delete IntroConst.ItemInfo[id];
			}
			dataProxy.equipments=[];
			for(var i:int = 1; i<=15; i++)
			{
				var itemUnit:ItemUnit = new ItemUnit();
				itemUnit.Grid = detailInfoUI["Equ_"+i];
				itemUnit.Item = null;
				itemUnit.Index  = i-1;
				itemUnit.IsUsed = false;
				if(equips[i-1].equipTypeID!=0 && equips[i-1].equipRealID!=0){
					var equipItem:EquipmentItem=new EquipmentItem(equips[i-1].equipTypeID,null, equips[i-1].color);
					equipItem.type=equips[i-1].equipTypeID;
					equipItem.equipId=equips[i-1].equipRealID;
					this.detailInfoUI.addChild(equipItem);
					equipItem.x=itemUnit.Grid.x+2;
					equipItem.y=itemUnit.Grid.y+2;
					var ownerId:uint = (GameCommonData.TargetAnimal && GameCommonData.TargetAnimal.Role) ? GameCommonData.TargetAnimal.Role.Id : 0;
					dataProxy.equipments[i-1]={id:equips[i-1].equipRealID, type:equips[i-1].equipTypeID, ownerId:ownerId};
					this.cells.push(equipItem);
				}	
			}
			(this.detailInfoUI.mc_Sex as MovieClip).gotoAndStop(obj.usSex+1);
			(this.detailInfoUI.txt_battleLevel as TextField).text=UIUtils.GetScoreStr(obj.usWarSroce);                          
			(this.detailInfoUI.txt_level as TextField).text=+obj.usUserlev+GameCommonData.wordDic[ "often_used_level" ];   //  级 
			(this.detailInfoUI.txt_mainJob as TextField).text=dataProxy.RolesListDic[obj.usFirPro] +" "+obj.usFirProLev+GameCommonData.wordDic[ "often_used_level" ];  // 级
			if(obj.usSecPro==0){
				(this.detailInfoUI.txt_subJob as TextField).text=dataProxy.RolesListDic[obj.usSecPro];
			}else{
				(this.detailInfoUI.txt_subJob as TextField).text=dataProxy.RolesListDic[obj.usSecPro] +" "+obj.usSecProLev +GameCommonData.wordDic[ "often_used_level" ];  //级
			}
				
			(this.detailInfoUI.txt_faction as TextField).text=obj.szSyn==null ? GameCommonData.wordDic[ "often_used_none" ] : obj.szSyn;  // 无
			var des:DesignationProxy=facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy;
			(this.detailInfoUI.txt_title as TextField).text=des.getDesignationName(int(obj.sztitle));
			(this.detailInfoUI.txt_feel as TextField).text=obj.szFeel==null ? "" : obj.szFeel;
			(this.detailInfoUI.txt_mate as TextField).text=obj.szWife==null ? GameCommonData.wordDic[ "often_used_none" ]:obj.szWife;		// 无
		}
		
		protected function onPanelBaseCloseHandler(e:Event):void{
			removeAll();
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
		}
		
	}
}