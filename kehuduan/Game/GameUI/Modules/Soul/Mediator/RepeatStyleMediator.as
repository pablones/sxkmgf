package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.QuickBuyComponent;
	import GameUI.Modules.Soul.View.SoulComponents;
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
	 * 魂魄-重洗属相
	 * @author lh
	 * 
	 */
	public class RepeatStyleMediator extends Mediator
	{
		public static const NAME:String = "RepeatStyleMediator";
		public static const INITMEDIATOR:String = "initRepeatStyleMediator";
		public static const SHOWVIEW:String = "showRepeatStylePanel";
		public static const DEAL_AFTER_SEND_REPEAT_STYLE:String = "dealAfterSendRepeatStyle";		
		
		public static var isRepeatStyleSend:Boolean;
		private var skillArrow1:MovieClip;	//选项1
		private var skillArrow2:MovieClip;	//选项2
		private var skillArrow3:MovieClip;	//选项3
		
		public var panelBase:PanelBase;	//当前选择的技能编号
		private var choseSyleNum:int;	//当前选择的技能编号
		private var tempSkillMc:MovieClip;
		private var moneyAll:int;	//需要的金钱总数
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var quickBuy:QuickBuyComponent;//面板和快速购买
		public function RepeatStyleMediator(mediatorName:String=null, viewComponent:Object=null)
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
				DEAL_AFTER_SEND_REPEAT_STYLE,
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
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"repeatStyle"});
					quickBuy = new QuickBuyComponent(this.mainView,591005,"RepeatStylePanel");
//					panelBase = new PanelBase(mainView, mainView.width-10, mainView.height + 12 );
					panelBase = new PanelBase(quickBuy, quickBuy.width-12, quickBuy.height + 12 );
					panelBase.name = "RepeatStylePanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_repStyle_hand" ]);//"重洗魂魄属相"
					initView();
				break;
				case SHOWVIEW:
					showView();
				break;
				case DEAL_AFTER_SEND_REPEAT_STYLE:
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
				break;	case EventList.ONSYNC_BAG_QUICKBAR:
					if(int(notification.getBody()) == 591005)
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
			var soulComp:SoulComponents = SoulComponents.getInstance();
			skillArrow1 = soulComp.getSkillArrow();
			skillArrow1.name = "skillArrow1";
			skillArrow1.x = 24;
			skillArrow1.y = 107;
			this.mainView.addChild(skillArrow1);
			
			skillArrow2 = soulComp.getSkillArrow();
			skillArrow2.name = "skillArrow2";
			skillArrow2.x = 78;
			skillArrow2.y = 107;
			this.mainView.addChild(skillArrow2);
			
			skillArrow3 = soulComp.getSkillArrow();
			skillArrow3.name = "skillArrow3"; 
			skillArrow3.x = 127;
			skillArrow3.y = 107;
			this.mainView.addChild(skillArrow3);
			
			(this.mainView.txt_Style1 as TextField).mouseEnabled = false;
			(this.mainView.txt_Style2 as TextField).mouseEnabled = false;
			(this.mainView.txt_Style3 as TextField).mouseEnabled = false;
			(this.mainView.txt_Style3 as TextField).mouseEnabled = false;
			
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
			
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			dealEventListeners(true);
			updateView();
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		private function dealEventListeners(isAdd:Boolean):void
		{
			if(isAdd)
			{
				skillArrow1.addEventListener(MouseEvent.CLICK,onMouseClick);
				skillArrow2.addEventListener(MouseEvent.CLICK,onMouseClick);
				skillArrow3.addEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.btn_sure.addEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.btn_cancel.addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				skillArrow1.removeEventListener(MouseEvent.CLICK,onMouseClick);
				skillArrow2.removeEventListener(MouseEvent.CLICK,onMouseClick);
				skillArrow3.removeEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.removeEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		
		private function updateView():void
		{	
			moneyAll = SoulData.other.pcgold;
			this.upDateNeedMoney(moneyAll);
			
			if(SoulMediator.soulVO.style == 1)
			{
				initStyleTxt(1);
			}
			else if(SoulMediator.soulVO.style == 2)
			{
				initStyleTxt(2);
			}
			else if(SoulMediator.soulVO.style == 3)
			{
				initStyleTxt(3);
			}
			else if(SoulMediator.soulVO.style == 4)
			{
				initStyleTxt(4);
			}
			
			(this.mainView.txt_percent as TextField).text = "100%";
			(this.mainView.txt_explain as TextField).htmlText = getPercentTxt();
			
		}
		
		private function dealAfterSend():void
		{
			if(isRepeatStyleSend)
			{
				isRepeatStyleSend = false;
				tempSkillMc = null;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_repStyle_deal" ], color:0xffff00});//"改变魂魄属相成功"
				this.panelCloseHandler(null);
				
			}
			(this.mainView.txt_explain as TextField).htmlText = getPercentTxt();
		}
		
		private function getPercentTxt():String
		{
			var toolNum:int = BagData.hasItemNum(591005);//魂魄易相丹
			
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+GameCommonData.wordDic[ "mod_soul_med_repStyle_getP" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];//需要		个	魂魄易相丹	当前拥有	个
			return explainTxt;	
		}
		private function onMouseClick(me:MouseEvent):void
		{
			if(tempSkillMc)
			{
				if(tempSkillMc.currentFrame == 2)
				{
					tempSkillMc.gotoAndStop(1);
				}
			}
			switch((me.target as DisplayObject).name)
			{
				case "skillArrow1":
					skillArrow1.gotoAndStop(2);
					tempSkillMc = skillArrow1;
					judgeCurrentSkill();
				break;
				case "skillArrow2":
					skillArrow2.gotoAndStop(2);
					tempSkillMc = skillArrow2;
					judgeCurrentSkill();
				break;
				case "skillArrow3":
					skillArrow3.gotoAndStop(2);
					tempSkillMc = skillArrow3;
					judgeCurrentSkill();
				break;
				case "btn_sure":
					onSureSubmit();
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;	
			}
		}
		
		private function onSureSubmit():void
		{
			if(!tempSkillMc)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_repStyle_onSure_1" ], color:0xffff00});//"请选择要重洗后的属性"
				return;
			}
			if(BagData.hasItemNum(591005) == 0)	//是否有足够润魂石
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_repStyle_onSure_2" ], color:0xffff00});//"您的背包中没有魂魄易相丹"
				return;
			}
			if(SoulProxy.getPlayTotalMoney() < moneyAll)
			 {
			 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
				return;
			 }
			isRepeatStyleSend = true;
			SoulProxy.getRepeatStyle(choseSyleNum);
		}
		private function initStyleTxt(tag:int):void
		{
			if(tag == 1)
			{
				(this.mainView.txt_Style1 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_1" ];//"地";
				(this.mainView.txt_Style2 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_2" ];//"水";
				(this.mainView.txt_Style3 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_3" ];//"火";
				(this.mainView.txt_Style4 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_4" ];//"风";
			}
			else if(tag == 2)
			{
				(this.mainView.txt_Style1 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_2" ];//"水";
				(this.mainView.txt_Style2 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_3" ];//"火";
				(this.mainView.txt_Style3 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_4" ];//"风";
				(this.mainView.txt_Style4 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_1" ];//"地";
			}
			else if(tag == 3)
			{
				(this.mainView.txt_Style1 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_3" ];//"火";
				(this.mainView.txt_Style2 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_4" ];//"风";
				(this.mainView.txt_Style3 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_1" ];//"地";
				(this.mainView.txt_Style4 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_2" ];//"水";
			}
			else if(tag == 4)
			{
				(this.mainView.txt_Style1 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_4" ];//"风";
				(this.mainView.txt_Style2 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_1" ];//"地";
				(this.mainView.txt_Style3 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_2" ];//"水";
				(this.mainView.txt_Style4 as TextField).text = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_3" ];//"火";
			}
		}
		
		
		private function judgeCurrentSkill():void
		{
			if(SoulMediator.soulVO.style == 1)
			{
				if(tempSkillMc)
				{
					if(tempSkillMc.name == "skillArrow1")
					{
						choseSyleNum = 2;
					}
					else if(tempSkillMc.name == "skillArrow2")
					{
						choseSyleNum = 3;
					}
					else if(tempSkillMc.name == "skillArrow3")
					{
						choseSyleNum = 4;
					}
				}
			}
			else if(SoulMediator.soulVO.style == 2)
			{
				if(tempSkillMc)
				{
					if(tempSkillMc.name == "skillArrow1")
					{
						choseSyleNum = 3;
					}
					else if(tempSkillMc.name == "skillArrow2")
					{
						choseSyleNum = 4;
					}
					else if(tempSkillMc.name == "skillArrow3")
					{
						choseSyleNum = 1;
					}
				}
			}
			else if(SoulMediator.soulVO.style == 3)
			{
				if(tempSkillMc)
				{
					if(tempSkillMc.name == "skillArrow1")
					{
						choseSyleNum = 4;
					}
					else if(tempSkillMc.name == "skillArrow2")
					{
						choseSyleNum = 1;
					}
					else if(tempSkillMc.name == "skillArrow3")
					{
						choseSyleNum = 2;
					}
				}
			}
			else if(SoulMediator.soulVO.style == 4)
			{
				if(tempSkillMc)
				{
					if(tempSkillMc.name == "skillArrow1")
					{
						choseSyleNum = 1;
					}
					else if(tempSkillMc.name == "skillArrow2")
					{
						choseSyleNum = 2;
					}
					else if(tempSkillMc.name == "skillArrow3")
					{
						choseSyleNum = 3;
					}
				}
			}
		}
		
		private function panelCloseHandler(e:Event):void
		{
			if(tempSkillMc)
			{
				tempSkillMc = null;
			}
			choseSyleNum = 0;
			isRepeatStyleSend = false;
			if(this.panelBase.parent)
			{
				dealEventListeners(false);
				this.panelBase.parent.removeChild(this.panelBase);
			}
//			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
		}
	}
}