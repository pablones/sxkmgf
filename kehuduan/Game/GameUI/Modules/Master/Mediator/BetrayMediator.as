package GameUI.Modules.Master.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class BetrayMediator extends Mediator
	{
		public static const NAME:String = "BetrayMediator";
		private var prenticeArr:Array;
		
		private var panelBase:PanelBase;
		private var currentId:uint;
		private var currentName:String;
		
		public function BetrayMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get betrayView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [EventList.INITVIEW,
						MasterData.BETRAY_MASTER,
						MasterData.SHOW_BETRAY
						]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					initUI();
					break;
					
				case MasterData.BETRAY_MASTER:
					MasterData.isRequestBetray = true;
					requestData();
					break;
					
				case MasterData.SHOW_BETRAY:
					MasterData.isRequestBetray = false;
					showBetray( notification.getBody() );
					break;
			}
		}
		
		private function requestData():void
		{
			RequestTutor.requestTutorAction(78,1);												//78为请求列表 
		}
		
		private function initUI():void
		{
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"BanishPrentice"});
			panelBase = new PanelBase(betrayView, betrayView.width+8, betrayView.height+12);
			panelBase.name = "betrayView";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos2.x - 220;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt("逐出师门");      //逐出师门
		}
		
		private function showBetray(obj:Object):void
		{
			if ( obj.list[0].relation == 1 )											//自己是徒弟
			{
				var masterName:String = obj.list[0].name;                                                                                  //强制解除师徒关系需要缴纳N金钱并且扣除所有的体力和精力。你确定要解除与                                                                                     的师徒关系吗？                                                                提 示
				facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:prenticeBetray, cancel:cancelClose, info:"<font color = '#ffffff'>"+GameCommonData.wordDic[ "mod_mas_med_bet_sho_1" ]+"</font><font color = '#ffff00'>" +masterName+ "</font><font color = '#ffffff'>"+GameCommonData.wordDic[ "mod_mas_med_bet_sho_2" ]+"</font>", title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],canOp:1});
				return;
			}
			else if ( obj.list[0].relation == 2 )									//自己是师傅
			{
				this.prenticeArr = [];
				var len:uint = obj.list.length;
				for ( var i:uint=0; i<len; i++ )
				{
					if ( obj.list[i].relation == 2 )
					{
						prenticeArr.push( obj.list[i] );
					}
				}
				checkPrentice(prenticeArr);
			}
		}
		
		
		private function checkPrentice(arr:Array):void
		{
			if ( arr.length == 1 )
			{
				var prenticeName:String = arr[0].name;
				currentId = arr[0].idTarget;                                                                                                               //强制解除师徒关系需要缴纳N金钱并且扣除所有的体力和精力。你确定要解除与                                                                                      的师徒关系吗？                                                                 提 示
				facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:masterBetray, params:currentId,cancel:cancelClose, info:"<font color = '#ffffff'>"+GameCommonData.wordDic[ "mod_mas_med_bet_sho_1" ]+"</font><font color = '#ffff00'>" +prenticeName+ "</font><font color = '#ffffff'>"+GameCommonData.wordDic[ "mod_mas_med_bet_sho_2" ]+"</font>", title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],canOp:1});
				return;
			}
			else if ( arr.length == 2 )
			{
				if ( !GameCommonData.GameInstance.GameUI.contains(panelBase) )
				{
					GameCommonData.GameInstance.GameUI.addChild(panelBase);
					showBetrayView();
				}
			}
		}
		
		private function showBetrayView():void
		{
			( betrayView.name1_txt as TextField ).text = prenticeArr[0].name;
			( betrayView.name2_txt as TextField ).text = prenticeArr[1].name;
			( betrayView.circle_0 as MovieClip ).gotoAndStop(2);
			( betrayView.circle_1 as MovieClip ).gotoAndStop(1);
			currentId = prenticeArr[0].idTarget;
			
			( betrayView.commit_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,onCommit);
			( betrayView.cancel_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,panelCloseHandler);
			( betrayView.circle_0 as MovieClip ).addEventListener(MouseEvent.CLICK,clickCircle0);
			( betrayView.circle_1 as MovieClip ).addEventListener(MouseEvent.CLICK,clickCircle1);
		}
		
		private function clickCircle0(evt:MouseEvent):void
		{
			( betrayView.circle_0 as MovieClip ).gotoAndStop(2);
			( betrayView.circle_1 as MovieClip ).gotoAndStop(1);
			currentId = prenticeArr[0].idTarget;
			currentName = prenticeArr[0].name;
		}
		
		private function clickCircle1(evt:MouseEvent):void
		{
			( betrayView.circle_0 as MovieClip ).gotoAndStop(1);
			( betrayView.circle_1 as MovieClip ).gotoAndStop(2);
			currentId = prenticeArr[1].idTarget;
			currentName = prenticeArr[1].name;
		}
		
		private function onCommit(evt:MouseEvent):void
		{
			facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:masterBetray, params:currentId,cancel:cancelClose, info:"<font color = '#ffffff'>强制解除师徒关系需要缴纳N金钱并且扣除所有的体力和精力。你确定要解除与</font><font color = '#ffff00'>" +currentName+ "</font><font color = '#ffffff'>的师徒关系吗？</font>", title:"提 示",canOp:1});
		}
		
		// 徒弟单方解除师徒关系
		private function prenticeBetray():void
		{
			sendInfo(77);
		}
		
		//师傅单方解除师徒关系
		private function masterBetray(tID:uint):void
		{
			sendInfo(76,tID);
		}
		
		private function sendInfo(action:uint,tID:uint=0):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(tID);									//这里发徒弟id
			parm.push(action);							   //76师傅删徒弟  77徒弟删师傅
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		private function cancelClose():void
		{
			
		}
		
		
		private function panelCloseHandler(evt:Event):void
		{
			if ( GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
		}
		
	}
}