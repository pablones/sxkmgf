package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.QuickBuyComponent;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.*;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize; 
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄-重洗魂魄技能
	 * @author lh
	 * 
	 */
	public class RepeatSkillMediator extends Mediator
	{
		public static const NAME:String = "RepeatSkillMediator";
		public static const INITMEDIATOR:String = "initRepeatSkillMediator";
		public static const SHOWVIEW:String = "showRepeatSkillPanel";
		public static const DEAL_AFTER_SEND_REPEAT_SKILL:String = "dealAfterSendRepeatSkill";
		
		public static var isRepeatSkillSend:Boolean;
		public var panelBase:PanelBase;
		private var moneyAll:int;	//需要的金钱总数
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var quickBuy:QuickBuyComponent;//面板和快速购买
		public function RepeatSkillMediator(mediatorName:String=null, viewComponent:Object=null)
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
				DEAL_AFTER_SEND_REPEAT_SKILL,
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				EventList.UPDATEMONEY,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"repeatSkill"});
					quickBuy = new QuickBuyComponent(this.mainView,591001,"RepeatSkillPanel");
//					panelBase = new PanelBase(mainView, mainView.width-9, mainView.height + 12 );
					panelBase = new PanelBase(quickBuy, quickBuy.width-12, quickBuy.height + 12 );
					panelBase.name = "RepeatSkillPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_repSki_hand" ]);//"重置魂魄技能"
					initView();
				break;
				case SHOWVIEW:
					showView();
				break;
				case DEAL_AFTER_SEND_REPEAT_SKILL:
					dealAfterSend();
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
				case EventList.ONSYNC_BAG_QUICKBAR:
					if(int(notification.getBody()) == 591001)
					{
						if(GameCommonData.GameInstance.GameUI.contains(this.mainView))
						{
							dealAfterSend();
						}
					}
				break;
			}
		}
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false;
			(this.mainView.txt_explain as TextField).mouseEnabled = false;
			for(var i:int = 0; i < 3; i++)
			{
				(this.mainView["txt_skill"+i] as TextField).autoSize = TextFieldAutoSize.CENTER;
			}
			
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
					 if(BagData.hasItemNum(591001) == 0)
					 {
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_repSki_onMouseC_1" ], color:0xffff00});//"没有足够的洗魂符"
					 	return;
					 }
					 if(SoulProxy.getPlayTotalMoney() < moneyAll)
					 {
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
						return;
					 }
					 isRepeatSkillSend = true;
					 SoulProxy.getSkillReapeat();
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		private function dealAfterSend():void
		{
			if(isRepeatSkillSend)
			{
				isRepeatSkillSend = false;
				initSkillView();
			}
			(this.mainView.txt_explain as TextField).htmlText = getTxtInfo();
		}
		
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			initSkillView();
			moneyAll = SoulData.other.scgold;
			this.upDateNeedMoney(moneyAll);
			(this.mainView.txt_percent as TextField).text = "100%";
			(this.mainView.txt_explain as TextField).htmlText = getTxtInfo();
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
		
		private function getTxtInfo():String
		{
			var toolNum:int = BagData.hasItemNum(591001);//洗魂符
				
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+GameCommonData.wordDic[ "mod_soul_med_repSki_getT" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];	//需要	个	技能重置符	当前拥有	个
			return explainTxt;
		}
		
		private function initSkillView():void
		{
			(this.mainView.txt_skill0 as TextField).mouseEnabled = false;
			(this.mainView.txt_skill1 as TextField).mouseEnabled = false;
			(this.mainView.txt_skill2 as TextField).mouseEnabled = false;
			
			var skills:Array = SoulMediator.soulVO.soulSkills
			for(var i:int = 0; i < skills.length; i++)
			{
				if(skills[i] == false)
				{
					(this.mainView["txt_skill"+i] as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repSki_initSki_1" ];//"暂无技能";
				}
				else if(skills[i] is SoulSkillVO)
				{
					var sVo:SoulSkillVO = skills[i];
					if(sVo.state == 1)
					{
						(this.mainView["txt_skill"+i] as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repSki_initSki_2" ];//"可学习新技能";
					}
					else if(sVo.state == 0)
					{
						(this.mainView["txt_skill"+i] as TextField).text = sVo.name+"("+GameCommonData.wordDic[ "mod_soul_med_repSki_initSki_3" ]+sVo.level+")";//等级
					}
					
				} 
			}
		}
		
		
		private function panelCloseHandler(e:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				isRepeatSkillSend = false;
				dealEventListeners(false);
				GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			}
		}
	}
}