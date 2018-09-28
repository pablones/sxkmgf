package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.QuickBuyComponent;
	import GameUI.Modules.Soul.View.SoulComponents;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.*;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄-重洗扩展属性完成
	 * @author lh
	 * 
	 */
	public class RepeatExtendProMediator extends Mediator
	{
		public static const NAME:String = "RepeatExtendProMediator";
		public static const INITMEDIATOR:String = "initRepeatExtendProMediator";
		public static const SHOWVIEW:String = "initRepeatExtendProPanel";
		public static const DEAL_AFTER_SEND_REPEAT_EXTEND_PRO:String = "dealAfterSendRepeatExtendPro";
		
		public static var isRepeatExtendProSend:Boolean;
		private var tagName:String;	//重洗临时数据
		public var panelBase:PanelBase;
		private var moneyAll:int;	//需要的金钱总数
		private var tempExtVo:SoulExtPropertyVO;
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var yellowShape:Shape;
		private var redShape:Shape;
		private var quickBuy:QuickBuyComponent;//面板和快速购买
		public function RepeatExtendProMediator(mediatorName:String=null, viewComponent:Object=null)
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
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				DEAL_AFTER_SEND_REPEAT_EXTEND_PRO,
				EventList.UPDATEMONEY,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"repeatExtendPro"});
					quickBuy = new QuickBuyComponent(this.mainView,591004,"RepeatExtendProPanel");
//					panelBase = new PanelBase(quickBuy, quickBuy.width-22, quickBuy.height + 12 );
					panelBase = new PanelBase(quickBuy, quickBuy.width-21, quickBuy.height + 12 );
					panelBase.name = "RepeatExtendProPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_repExt_hand" ]);//"改变扩展属性"
					initView(); 
				break;
				case SHOWVIEW:
					showView();
				break;
				case DEAL_AFTER_SEND_REPEAT_EXTEND_PRO:
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
					if(int(notification.getBody()) == 591004)
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
			
			for(var i:int = 0; i < 10; i ++)
			{
				(this.mainView["txt_extendPro"+i] as TextField).mouseEnabled = false;
			}
			
			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 150;
			bindMoneyItem.y = 337;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 150;
			unBindMoneyItem.y = 359;
			needMoney = new MoneyItem();
			needMoney.x = 150;
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
		
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			moneyAll = SoulData.other.changeAttributes;
			this.upDateNeedMoney(moneyAll);
			initExtendPropertyView();
			yellowShape = new Shape();
			redShape = new Shape();
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
			var toolNum:int = BagData.hasItemNum(591004);
				
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+GameCommonData.wordDic[ "mod_soul_med_repExt_getT" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];//需要	个	铸魂石	当前拥有  个	
			return explainTxt;
		}
		
		private function dealAfterSend():void
		{
			if(isRepeatExtendProSend)
			{
				isRepeatExtendProSend = false;
				initExtendPropertyView();
				var extVo:SoulExtPropertyVO = SoulMediator.soulVO.extProperties[tagName.split("_")[2]];
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:tempExtVo.name+'('+tempExtVo.level+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+')'+GameCommonData.wordDic[ "mod_soul_med_repExt_deal" ]+extVo.name+'('+extVo.level+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+')', color:0xffff00});//级		已改变为		级
				tempExtVo = null;
				(this.mainView.txt_percent as TextField).text = "100%";
			}
			(this.mainView.txt_explain as TextField).htmlText = getTxtInfo();
		}
		
		private function initExtendPropertyView():void
		{
			var extArr:Array = SoulMediator.soulVO.extProperties;
			var soulcomp:SoulComponents = SoulComponents.getInstance();	//获取按钮组件
			gcExtContainer(); 
			if(SoulMediator.isEquiptSoul)
			{
				for(var i:int = 0; i < 10; i ++)
				{
					var extComp:DisplayObjectContainer = this.mainView["btn_extendPro"+i];	//对应的面板中的扩展属性按钮组件
					if(extArr[i] == false)
					{
						var notUseComp:SimpleButton = soulcomp.getNotLearn()
						notUseComp.name = "soulExt_notUse_" + i;
						notUseComp.mouseEnabled = false;

						extComp.addChild(notUseComp);
					}
					else
					{
						var extVo:SoulExtPropertyVO = extArr[i] as SoulExtPropertyVO;
						if(extVo.state == 0)
						{
							var hasLearnComp:SimpleButton = soulcomp.getHasLearn();
							hasLearnComp.name = "soulExt_hasLearn_" + i;
							setHasLearnTxt(extVo);
							/* (this.mainView["txt_extendPro" +i] as TextField).text = extVo.name+"("+extVo.level+"级)";
							(this.mainView["txt_extendPro" +i] as TextField).textColor = 0X00FF00; */
							hasLearnComp.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
							hasLearnComp.addEventListener(MouseEvent.CLICK,onMouseClick);
							extComp.addChild(hasLearnComp);
						
						}
						else if(extVo.state == 1)	//可学习
						{
							var canLearnComp:SimpleButton = soulcomp.getHasLearn();
							canLearnComp.name = "soulExt_canLearn_" + i;
							canLearnComp.mouseEnabled = false;
							extComp.addChild(canLearnComp);
						}
						else if(extVo.state == 2)	//可开槽
						{
							var notLearnComp:SimpleButton = soulcomp.getCanLearn();
							notLearnComp.name = "soulExt_canUseToLearn_" + i;
							notLearnComp.mouseEnabled = false;
							extComp.addChild(notLearnComp);
						}
					}
				}
			}
			else
			{
				for(var j:int = 0; j < 10; j ++)
				{
					var extComp2:DisplayObjectContainer = this.mainView["btn_extendPro"+j];	//对应的面板中的扩展属性按钮组件
					var notUseComp2:SimpleButton = soulcomp.getNotLearn()
					notUseComp2.name = "soulExt_notUse_" + j;
					extComp2.addChild(notUseComp2);
				}
			}
		}
		
		private function setHasLearnTxt(extVo:SoulExtPropertyVO):void
		{
			var color:String;
			
			if(extVo.level <= 1)
			{
				color = IntroConst.itemColors[0];
			} 
			else if(extVo.level <= 2)
			{
				color = IntroConst.itemColors[2];
			}
			else if(extVo.level <= 3)
			{
				color = IntroConst.itemColors[3];
			}
			else if(extVo.level <= 4)
			{
				color = IntroConst.itemColors[4];
			}
			else if(extVo.level <= 10)
			{
				color = IntroConst.itemColors[5];
			}
			(this.mainView["txt_extendPro" +extVo.number] as TextField).htmlText = '<font color="'+color+'">'+extVo.name+"("+extVo.level+GameCommonData.wordDic[ "often_used_level" ]+")"+'</font>';	//级
		}
		/**
		 *	扩展属性容器垃圾回收  
		 * 
		 */		
		private function gcExtContainer():void
		{
			for(var i:int = 0; i < 10; i ++)
			{
				var extContainer:DisplayObjectContainer = this.mainView["btn_extendPro"+i];
				var num:int = extContainer.numChildren - 1;
				while(num >= 0)
				{
					var dis:DisplayObject = extContainer.getChildAt(num);
					if(dis)
					{
						if(dis.name.split("_")[0] == "soul")
						{
							if(dis.hasEventListener(MouseEvent.CLICK))
							{
								dis.removeEventListener(MouseEvent.CLICK,onMouseClick);
							}
							if(dis.hasEventListener(MouseEvent.MOUSE_OVER))
							{
								dis.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
							}
							dis.parent.removeChild(dis);
							dis = null;
						}
					}
					num --;
				}
				(this.mainView["txt_extendPro" +i] as TextField).text = "";
			}	
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name.split("_")[1]) 
			{
				case "hasLearn":
					tagName = me.target.name;
					addYellowFrame(me.target as DisplayObject);
				break;
				case "sure":
					if(!tagName)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_repExt_onMouseC_1" ], color:0xffff00});//"请选择需要重洗的属性"
						return;
					}
					if(BagData.hasItemNum(591004) == 0)	//是否有铸魂石
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_repExt_onMouseC_2" ], color:0xffff00});//"没有足够的铸魂石"
						return;
					}
					if(SoulProxy.getPlayTotalMoney() < moneyAll)
					{
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
						return;
					}
					isRepeatExtendProSend = true;
					tempExtVo = SoulMediator.soulVO.extProperties[tagName.split("_")[2]];
					SoulProxy.getExtendRandom(int(tagName.split("_")[2])+1);
				break;
				case "cancel":
					this.panelCloseHandler(null);
				break;
			}
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
			
		
		private function panelCloseHandler(e:Event):void
		{
			tagName = null; 
			tempExtVo = null;
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