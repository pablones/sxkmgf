package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.SoulComponents;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄-升级魂魄技能
	 * @author lh
	 * 
	 */
	public class ImproveSkillMediator extends Mediator
	{
		public static const NAME:String = "ImproveSkillMediator";
		public static const INITMEDIATOR:String = "initImproveSkillMediator";
		public static const SHOWVIEW:String = "showImproveSkillPanel";
		public static const DEAL_AFTER_SEND_IMPROVE_SKILL:String = "dealAfterSendImproveSkill";		
		
		public static var isImproveSkillSend:Boolean;
		private var tempObj:Object;
		public var panelBase:PanelBase;
		private var moneyAll:int;	//需要的金钱总数
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var yellowShape:Shape;
		private var redShape:Shape;
		public function ImproveSkillMediator(mediatorName:String=null, viewComponent:Object=null)
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
				DEAL_AFTER_SEND_IMPROVE_SKILL,
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				EventList.UPDATEMONEY
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"improveSkill"});
					panelBase = new PanelBase(mainView, mainView.width - 12, mainView.height + 12 );
					panelBase.name = "ImproveSkillPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_impSki_hand" ]);//"升级魂魄技能"
					initView();
				break;
				case SHOWVIEW:
					showView(int(notification.getBody()));
				break;
				case DEAL_AFTER_SEND_IMPROVE_SKILL:
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
			}
		}
		
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false;
			(this.mainView.txt_explain as TextField).mouseEnabled = false;
			
			for(var i:int = 0; i < 3; i++)
			{
				(this.mainView["txt_improve"+i] as TextField).mouseEnabled = false;
			}

			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 75;
			bindMoneyItem.y = 337;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 75;
			unBindMoneyItem.y = 359;
			needMoney = new MoneyItem();
			needMoney.x = 75;
			needMoney.y = 315;
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
		private function showView(improveNum:int):void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			this.initSkillList();
			yellowShape = new Shape();
			redShape = new Shape();
			initGridState(improveNum);
			(this.mainView.txt_percent as TextField).text = "100%";
			var skillVo:SoulSkillVO = SoulMediator.soulVO.soulSkills[improveNum];
			if(skillVo.level == 10) return;
			moneyAll = SoulData.skill[skillVo.level].gold;
			this.upDateNeedMoney(moneyAll);
			(this.mainView.txt_explain as TextField).htmlText = getTxtInfo(skillVo.level);
			dealEventListeners(true);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		private function dealEventListeners(isAdd:Boolean):void
		{
			if(isAdd)
			{
				this.mainView.btn_compose.addEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.btn_sure.addEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.btn_cancel.addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				this.mainView.btn_compose.removeEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.btn_sure.removeEventListener(MouseEvent.CLICK,onMouseClick);
				this.mainView.btn_cancel.removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		
		private function initGridState(improveNum:int):void
		{
			var skillDis:DisplayObjectContainer = this.mainView["mc_improve"+improveNum];
			var num:int = skillDis.numChildren - 1;
			while(num >= 0)
			{
				var dis:DisplayObject = skillDis.getChildAt(num);
				if(dis)
				{
					if(dis.name.split("_")[1] == "hasSkill")
					{
						if(dis.hasEventListener(MouseEvent.CLICK))
						{
							dis.dispatchEvent(new MouseEvent(MouseEvent.CLICK));
							break;
						}
					}
				}
				num --;
			}
		}
		
		private function getTxtInfo(num:int):String
		{
			var toolNum:int = BagData.hasItemNum(591200+num); //通灵玉
			if(num == 0)
			{
				toolNum = 0;
			}
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+num+GameCommonData.wordDic[ "mod_soul_med_comTong_showView" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];//需要	个	级通灵玉	当前拥有	个
			return explainTxt;
		}
		
		private function initSkillList():void
		{
			var skills:Array = SoulMediator.soulVO.soulSkills;
			var soulCom:SoulComponents = SoulComponents.getInstance();
			gcSkillContainer();
			for(var i:int = 0; i < 3; i++)
			{
				var skillDis:DisplayObjectContainer = this.mainView["mc_improve"+i];
				if(skills[i] == false)	//不可学习
				{
					var btnNotLearn:SimpleButton = soulCom.getNotLearn();
					btnNotLearn.name = "soul_noSkill_" + i;
					btnNotLearn.mouseEnabled = false;
					skillDis.addChild(btnNotLearn);
				}
				else if(skills[i] is SoulSkillVO)	//已有技能
				{
					var skillVo:SoulSkillVO = skills[i];
					if(skillVo.state == 1)
					{
						var btncanLearn:SimpleButton = soulCom.getNotLearn();
						btncanLearn.name = "soul_canSkill_" + i;
						skillDis.addChild(btncanLearn);
					}
					else if(skillVo.state == 0)
					{
						var btnhasLearn:SimpleButton = soulCom.getHasLearn();
						btnhasLearn.name = "soul_hasSkill_" + i;
						btnhasLearn.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
						btnhasLearn.addEventListener(MouseEvent.CLICK,onMouseClick);
						skillDis.addChild(btnhasLearn);
						
						(this.mainView["txt_improve"+i] as TextField).textColor = 0xFFFFFF;
						(this.mainView["txt_improve"+i] as TextField).text = skillVo.name+"("+skillVo.level+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+")";//级
						(this.mainView["txt_improve"+i] as TextField).autoSize = TextFieldAutoSize.CENTER;
					}
				}
			}
		}
		
		private function gcSkillContainer():void
		{
			for(var j:int = 0; j < 3; j ++)
			{
				var skillContainer:DisplayObjectContainer = this.mainView["mc_improve"+j];	//技能格子
				var num:int = skillContainer.numChildren - 1;
				while(num >= 0)
				{
					var dis:DisplayObject = skillContainer.getChildAt(num)
					if(num)
					{
						if(dis.name.split("_")[0] == "soul")
						{
							if(dis.hasEventListener(MouseEvent.CLICK))
							{
								dis.removeEventListener(MouseEvent.CLICK,onMouseClick);
							}
							dis.parent.removeChild(dis);
							dis = null;
						}
					}
					num --;
				}
			}
		}
		
		private function dealAfterSend():void
		{
			
			redShape.graphics.clear();
			if(redShape.parent)
			{
				redShape.parent.removeChild(redShape);
			}
			if(tempObj)
			{
				(this.mainView.txt_explain as TextField).htmlText = getTxtInfo(int(tempObj.level)+1);
				moneyAll = SoulData.skill[int(tempObj.level)+1].gold;
				this.upDateNeedMoney(moneyAll);
			}
			isImproveSkillSend = false;
//			tempObj = null;
			initSkillList();
		}
		
		private function onMouseOver(me:MouseEvent):void
		{
			var dis:DisplayObject = me.target as DisplayObject;
			if(dis.parent.parent.contains(yellowShape))
			{
				if(dis.name.split("_")[2] == yellowShape.name.split("_")[1])
				{
					return;
				}
			}
			redShape.graphics.clear();	
			redShape.graphics.lineStyle(1,0xFF0000);
			redShape.graphics.drawRect(dis.x,dis.y,dis.width,dis.height);
			redShape.graphics.endFill();
			dis.parent.addChild(redShape);
			dis.addEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
		}
		
		
		private function addYellowFrame(dis:DisplayObject):void
		{
			if(redShape.parent)
			{
				redShape.parent.removeChild(redShape);
			}
			yellowShape.name = "yellow_"+dis.name.split("_")[2];
			yellowShape.graphics.clear();
			yellowShape.graphics.lineStyle(1,0xFFFF00);
			yellowShape.graphics.drawRect(dis.parent.x,dis.parent.y,dis.parent.width,dis.parent.height);
			yellowShape.graphics.endFill();
			if(dis.parent.parent)
			{
				dis.parent.parent.addChild(yellowShape);
			}
		}
		private function onMouseOut(me:MouseEvent):void
		{
			(me.target as DisplayObject).removeEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
			if(redShape.parent)
			{
				redShape.parent.removeChild(redShape);
			}
		}
		
		private function onMouseClick(me:MouseEvent):void 
		{
			var data:Array = me.target.name.split("_");
			switch(data[1])	
			{
				case "compose":
					sendNotification(ComposeTongStroneMediator.SHOWVIEW);
				break;
				case "sure":
					onSureSubmit();
				break;
				case "hasSkill":
					addYellowFrame(me.target as DisplayObject);
					tempObj = {};
					tempObj.level = (SoulMediator.soulVO.soulSkills[int(data[2])] as SoulSkillVO).level;
					tempObj.num = int(data[2]) + 1;
					(this.mainView.txt_explain as TextField).htmlText = getTxtInfo(tempObj.level);
					var skillVo:SoulSkillVO = SoulMediator.soulVO.soulSkills[int(data[2])];
					if(skillVo.level == 10) return;
					moneyAll = SoulData.skill[skillVo.level].gold;
					this.upDateNeedMoney(moneyAll);
				break;
				case "cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		private function onSureSubmit():void
		{
			if(!tempObj) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_impSki_onS_1" ], color:0xffff00});//"请选择要升级的技能"
				return;
			}
			if(tempObj.level == 10)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_impSki_onS_2" ], color:0xffff00});//"等级已满，无需升级"
				return;
			}
			if(BagData.hasItemNum(591200+tempObj.level) == 0) //通灵玉
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_impSki_onS_3" ], color:0xffff00});	//"您没有对应等级的通灵玉"
				return;
			}
			if(SoulProxy.getPlayTotalMoney() < moneyAll)
			 {
			 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
				return;
			 }
			isImproveSkillSend = true;
			SoulProxy.getSkillUpdate(int(tempObj.num));
		}
		
		private function panelCloseHandler(e:Event):void
		{
			isImproveSkillSend = false;
			tempObj = null;
			yellowShape.graphics.clear();
			if(yellowShape.parent)
			{
				yellowShape.parent.removeChild(yellowShape);
			}
			yellowShape = null;
			redShape.graphics.clear();
			if(redShape.parent)
			{
				redShape.parent.removeChild(redShape);
			}
			redShape = null;
			dealEventListeners(false);
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
		}
	}
}