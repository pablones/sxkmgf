package GameUI.Modules.RoleProperty.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Mediator.MasStuMainMediator;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Mediator.UI.OtherView;
	import GameUI.Modules.RoleProperty.Net.NetAction;
	import GameUI.UIConfigData;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class OtherMediator extends Mediator
	{

		public static const NAME:String = "OtherMediator";
		public static const TYPE:int = 4;
		private var parentView:MovieClip = null;
		private var otherView:OtherView;
		
		public function OtherMediator(parent:MovieClip)
		{
			parentView = parent;
			super(NAME);
		}
		
		private function get OtherPane():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				RoleEvents.INITROLEVIEW,
				RoleEvents.SHOWPROPELEMENT,
				RoleEvents.MEDIATORGC,
				RoleEvents.UPDATE_OTHER_INFO
			]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case RoleEvents.INITROLEVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.OTHERPANE});
					this.OtherPane.mouseEnabled=false;
					otherView = new OtherView( OtherPane );
				break;
				case RoleEvents.SHOWPROPELEMENT:
					if(notification.getBody() as int != TYPE) return;
					parentView.addChildAt(OtherPane, 4);
					initUI();
					requestData();
				break;
				case RoleEvents.MEDIATORGC:
					gc();
				break;
				case RoleEvents.UPDATE_OTHER_INFO:
					if ( otherView )
					{
						otherView.upDateView( notification.getBody() );
					}
				break;
			}
		}
		
		private function initUI():void
		{
			otherView.initUI();
//			( OtherPane.master_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,showMaster);
//			( OtherPane.mate_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,showMate);
//			( OtherPane.brother_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,showBrother);
			
			//暂时先干掉
			( OtherPane.master_btn as SimpleButton ).visible = false;
			( OtherPane.mate_btn as SimpleButton ).visible = false;
			( OtherPane.brother_btn as SimpleButton ).visible = false;
		}
		
		private function requestData():void
		{
			if ( RolePropDatas.lastRequestOtherTime == 0 )
			{
				NetAction.RequestOtherInfo();				//发playerAction 请求att
				RolePropDatas.lastRequestOtherTime = new Date().getTime();	
			}
			else
			{
				var newTime:Number = new Date().getTime();
				if ( (newTime-RolePropDatas.lastRequestOtherTime)<15000 )
				{
					return;
				}
				else
				{
					NetAction.RequestOtherInfo();
					RolePropDatas.lastRequestOtherTime = newTime;
				}
			}
		}
		
		private function showMaster(evt:MouseEvent):void
		{
//			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_om_1" ], color:0xffff00});     //"暂未开放" 
			var masterMed:MasStuMainMediator = facade.retrieveMediator( MasStuMainMediator.NAME ) as MasStuMainMediator;
			masterMed.masterCurPage = 1;
			sendNotification( MasterData.CLICK_MASTER_NPC );
		}
		
		private function showMate(evt:MouseEvent):void
		{
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_om_1" ], color:0xffff00});      //"暂未开放"
		}
		
		private function showBrother(evt:MouseEvent):void
		{
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_om_1" ], color:0xffff00});     //"暂未开放"
		}
		
		private function gc():void
		{
			( OtherPane.master_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,showMaster);
			( OtherPane.mate_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,showMate);
			( OtherPane.brother_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,showBrother);
		}
	}
}