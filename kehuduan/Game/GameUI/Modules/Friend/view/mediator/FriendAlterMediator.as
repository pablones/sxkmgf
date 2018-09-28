package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class FriendAlterMediator extends Mediator
	{
		public static const NAME:String="FriendAlterMediator";
		protected var basePanel:PanelBase;
		protected var playerInfo:PlayerInfoStruct;
		protected var roleName:String="";
		
		public function FriendAlterMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get viewUi():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				FriendCommandList.SHOW_FRIEND_ALTER,
				FriendCommandList.CLOSE_FRIEND_ALTER
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case FriendCommandList.SHOW_FRIEND_ALTER:
				
				
					playerInfo=notification.getBody() as PlayerInfoStruct;
					if(this.roleName==playerInfo.roleName)return;
					if(this.viewUi==null){
						sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"mc_alterFriend"});
						(this.viewUi.btn_agree as SimpleButton).addEventListener(MouseEvent.CLICK,onAgreeHandler);
						(this.viewUi.btn_disagree as SimpleButton).addEventListener(MouseEvent.CLICK,onDisagreeHandler);
						(this.viewUi.btn_agreeAndAdd as SimpleButton).addEventListener(MouseEvent.CLICK,onAgreeAndAddHandler);
						this.basePanel=new PanelBase(this.viewUi,this.viewUi.width+7,this.viewUi.height+10);
						this.basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_fri_view_med_friendA_han_1" ]);//"好友申请"
						this.basePanel.addEventListener(Event.CLOSE,onBasePanelClose);
					}
					var str:String='<font color="#ff0000">'+playerInfo.roleName+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_fri_view_med_friendA_han_2" ]+'</font>';//申请加你为好友？
					(this.viewUi.txt_des as TextField).filters=Font.Stroke();
					(this.viewUi.txt_des as TextField).htmlText=str;
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
					this.basePanel.x=300;
					this.basePanel.y=200;
					var friendManager:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
					if(friendManager.isHasTheFriend(playerInfo.roleName,playerInfo.frendId)){
						(this.viewUi.btn_agreeAndAdd as SimpleButton).visible=false;
					}else{
						(this.viewUi.btn_agreeAndAdd as SimpleButton).visible=true;
					}
					break;
				case FriendCommandList.CLOSE_FRIEND_ALTER: 
					this.onBasePanelClose(null);
					break;		
			}
		}
		
		
		/**
		 * 同意 
		 * @param e
		 * 
		 */		
		protected function onAgreeHandler(e:MouseEvent):void{
			FriendSend.getInstance().recallApplyToFriend(this.playerInfo.frendId,1,this.playerInfo.friendGroupId,0,this.playerInfo.roleName);
			this.removeUI();
		}
		
		
		/**
		 * 拒绝  
		 * @param e
		 * 
		 */		
		protected function onDisagreeHandler(e:MouseEvent):void{
			FriendSend.getInstance().recallApplyToFriend(this.playerInfo.frendId,3,this.playerInfo.friendGroupId,1,this.playerInfo.roleName);
			this.removeUI();
		}
		
		/**
		 * 同意并加为好友 
		 * @param e
		 * 
		 */		
		protected function onAgreeAndAddHandler(e:MouseEvent):void{
			var friendManager:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
			var groupId:uint=friendManager.seachGroupID();
			if(groupId>=0){
				FriendSend.getInstance().recallApplyToFriend(this.playerInfo.frendId,2,this.playerInfo.friendGroupId,(groupId+1),this.playerInfo.roleName);
			}	
			this.removeUI();
		}
		
		/**
		 * 关闭 
		 * @param e
		 * 
		 */		
		protected function onBasePanelClose(e:Event):void{
			this.onDisagreeHandler(null);		
		}
		
		protected function removeUI():void{
			this.roleName="";
			if(this.basePanel.parent!=null){
				GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			}
		}
		
	}
}