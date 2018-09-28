package GameUI.Modules.Answer.MasterAnswer
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Answer.Const.AnswerConst;
	import GameUI.Modules.Answer.Data.AnswerProp;
	import GameUI.Modules.Answer.MasterAnswer.view.MasterAnswerView;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.View.BaseUI.AutoPanelBase;
	import GameUI.View.Components.TimeoutComponent;
	
	import flash.events.Event;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MasterAnswerMediator extends Mediator
	{
		public static const NAME:String = "MasterAnswerMediator";
		private var isInit:Boolean = false;
		private var masterAnswerView:MasterAnswerView;
		private var panelBase:AutoPanelBase;
		
		private var allAnswers:Array;				//题库
		private var currentAnswerItem:AnswerProp;
		private var currentSerial:int;
		private var currentRelation:int;
		private var currentReadSerial:int;
		
		private var timeoutCom:TimeoutComponent;
		
		public function MasterAnswerMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							AnswerConst.OPEN_MASTER_ANSWER_PANEL,
							AnswerConst.CLOSE_MASTER_ANSWER_PANEL
						];	
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case AnswerConst.OPEN_MASTER_ANSWER_PANEL:
					if ( isInit )
					{
						currentSerial = int ( notification.getBody().serial ) % allAnswers.length + 1;
						currentRelation = notification.getBody().relation;
						currentReadSerial = notification.getBody().readSerial;
						openPanel();
						
					}
					else
					{
						initView( notification.getBody() );
//						showAnswer(2);			//显示题目
					}
				break;
				case AnswerConst.CLOSE_MASTER_ANSWER_PANEL:
					panelCloseHandler( null );
				break;
			}
		}
		
		private function initView( obj:Object ):void
		{
			masterAnswerView = new MasterAnswerView();
			panelBase = new AutoPanelBase( masterAnswerView,masterAnswerView.width+8,masterAnswerView.height+14 );
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y - 20;
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_ans_mas_mas_ini_1" ] );        //师徒答题
			isInit = true;
			
			timeoutCom = new TimeoutComponent();
			timeoutCom.x = 230;
			timeoutCom.y = 264;
			timeoutCom.timeDoneHandler = timeDoneHandler;
//			masterAnswerView.addChildAt( timeoutCom,masterAnswerView.numChildren-1 );
			masterAnswerView.addChild( timeoutCom );
			
			currentAnswerItem = new AnswerProp();
			allAnswers = GameCommonData.GameInstance.Content.Load( GameConfigData.Other_XML_SWF ).GetDisplayObject()["allAnswers"] as Array; 
//			openPanel();
			sendNotification( AnswerConst.OPEN_MASTER_ANSWER_PANEL,obj );
		}
		
		private function openPanel():void
		{
			masterAnswerView.listening();
//			currentSerial = 2;						//当前题目
			
			//异常处理
//			if ( !allAnswers[ currentSerial - 1 ] )
//			{
//				currentSerial = Math.random() * ( allAnswers.length - 1 ) + 1;
//			}
			
			currentAnswerItem.relation = currentRelation;
			currentAnswerItem.serial = currentReadSerial;
			currentAnswerItem.question = allAnswers[ currentSerial - 1 ][ 1 ];
			currentAnswerItem.answers = allAnswers[ currentSerial - 1 ].slice( -3 );
			
			masterAnswerView.answerItem = currentAnswerItem;
			
			if ( !GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				panelBase.addEventListener( Event.CLOSE, panelCloseHandler );
			}
			timeoutCom.start( 30 );
		}
		
		//倒计时用完之后
		public function timeDoneHandler():void
		{
			panelCloseHandler( null );
			sendNotification( HintEvents.RECEIVEINFO,{ info:GameCommonData.wordDic[ "mod_ans_mas_mas_tim_1" ], color:0xffff00 } ); //答案不一致，答题失败
		}
		
		private function panelCloseHandler( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				masterAnswerView.gc();
				timeoutCom.stop();
				panelBase.removeEventListener( Event.CLOSE, panelCloseHandler );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
		}
		
	}
}