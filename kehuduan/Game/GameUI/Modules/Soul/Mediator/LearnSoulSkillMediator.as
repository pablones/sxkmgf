package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 * 魂魄-学习魂魄技能
	 * @author lh 
	 * 
	 */
	public class LearnSoulSkillMediator extends Mediator
	{
		public static const NAME:String = "LearnSoulSkillMediator";
		public static const INITMEDIATOR:String = "initLearnSoulSkillMediator";
		public static const SHOWVIEW:String = "showLearnSoulSkillPanel";
		public static const DEAL_AFTER_SEND_LEARN_SKILL:String = "dealAfterSendLearnSoulSkill";		
		
		public static var isLearnSoulSkillSend:Boolean;
		private var tempNum:int;	//选择学习的技能框编号
		private var panelBase:PanelBase;
		private var moneyAll:int;	//需要的金钱总数
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		public function LearnSoulSkillMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		private function get mainView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				INITMEDIATOR,
				SHOWVIEW,
				DEAL_AFTER_SEND_LEARN_SKILL,
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				EventList.UPDATEMONEY
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"learnSoulSkill"});
					panelBase = new PanelBase(mainView, mainView.width-10, mainView.height + 12 );
					panelBase.name = "LearnSoulSkillPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_leaSouSki_hand" ]);//"领悟魂魄技能"
					initView();
				break;
				case DEAL_AFTER_SEND_LEARN_SKILL:
					dealAfterSend();
				break;
				case SHOWVIEW:
					tempNum = notification.getBody() + 1;
					showView();
				break;
				case SoulProxy.CLOSE_ALL_SOUL_PANEL:
					if(GameCommonData.GameInstance.GameUI.contains(this.mainView))
					{
						panelCloseHandler(null);
					}
				break;
				case EventList.UPDATEMONEY:
					upDataMoney();
				break;
			}
		}
		
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false;
			(this.mainView.txt_explain as TextField).mouseEnabled = false;
			
			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 20;
			bindMoneyItem.y = 238;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 20;
			unBindMoneyItem.y = 260;
			needMoney = new MoneyItem();
			needMoney.x = 20;
			needMoney.y = 216;
			
			mainView.addChild( bindMoneyItem );
			mainView.addChild( unBindMoneyItem );
			mainView.addChild( needMoney );
			upDataMoney();
			upDateNeedMoney( 0 );
		}
		
		private function upDataMoney():void
		{
			this.bindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]) );
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "btn_sure":
					 if(SoulProxy.getPlayTotalMoney() < moneyAll)
					 {
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
						return;
					 }
					isLearnSoulSkillSend = true;
					SoulProxy.getSkillLearn(tempNum);
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			if(tempNum == 1)
			{
				moneyAll = SoulData.other.firstSkill;
			}
			else if(tempNum == 2)
			{
				moneyAll = SoulData.other.secondSkill;
			}
			else if(tempNum == 3)
			{
				moneyAll = SoulData.other.thirdSkill;
			}
			this.upDateNeedMoney(moneyAll);
			(this.mainView.txt_percent as TextField).text = "100%";
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		private function dealEventListeners(isAdd:Boolean):void
		{
			if(isAdd)
			{
				(this.mainView.btn_sure as DisplayObject).addEventListener(MouseEvent.CLICK,onMouseClick);
				(this.mainView.btn_cancel as DisplayObject).addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				(this.mainView.btn_sure as DisplayObject).removeEventListener(MouseEvent.CLICK,onMouseClick);
				(this.mainView.btn_cancel as DisplayObject).removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		
		private function dealAfterSend():void
		{
			tempNum = 0; 
			isLearnSoulSkillSend = false;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_leaSouSki_deal" ], color:0xffff00});//"技能学习成功"
			this.panelCloseHandler(null);

		}
		
		private function panelCloseHandler(e:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				tempNum = 0;
				isLearnSoulSkillSend = false;
				dealEventListeners(false);
				GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			}
		}
	}
}