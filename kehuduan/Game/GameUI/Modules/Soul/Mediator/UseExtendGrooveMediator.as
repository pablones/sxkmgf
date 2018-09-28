package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
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
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄-开启扩展属性槽 
	 * @author lh
	 * 
	 */
	public class UseExtendGrooveMediator extends Mediator
	{
		
		public static const NAME:String = "UseExtendGrooveMediator";
		public static const INITMEDIATOR:String = "initUseExtendGrooveMediator";
		public static const SHOWVIEW:String = "showUseExtendGroovePanel";
		public static const DEAL_AFTER_SEND_USE_EXT_GROOVE:String = "dealAfterSendUseExtendGroove";
		
		public static var isUseExtendGrooveSend:Boolean;
		private var updataNum:int;
		public var panelBase:PanelBase;
		private var moneyAll:int;	//需要的金钱总数
		
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var quickBuy:QuickBuyComponent;//面板和快速购买
		public function UseExtendGrooveMediator(mediatorName:String=null, viewComponent:Object=null)
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
				DEAL_AFTER_SEND_USE_EXT_GROOVE,
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
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"useExtendGroove"});
					quickBuy = new QuickBuyComponent(this.mainView,591003,"UseExtendGroovePanel");
//					panelBase = new PanelBase(mainView, mainView.width - 13, mainView.height + 12 );
					panelBase = new PanelBase(quickBuy, quickBuy.width - 9, quickBuy.height + 12 );
					panelBase.name = "UseExtendGroovePanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_useExt_hand" ]);//"开启扩展属性槽"
					initView();
				break;
				case SHOWVIEW:
					updataNum = int(notification.getBody())+1;
					showView();
				break;
				case DEAL_AFTER_SEND_USE_EXT_GROOVE:
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
					if(int(notification.getBody()) == 591003)
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
					if(BagData.hasItemNum(591003) == 0)	//是否有天心箭
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_useExt_onMou_1" ], color:0xffff00});//"没有足够的天心箭"
						return;
					}
					
					if(!RolePropDatas.ItemList[15])
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_leaExt_onMouseC_2" ], color:0xffff00});//"您没有装备魂魄"
						return;
					}
					if(SoulProxy.getPlayTotalMoney() < moneyAll)
					 {
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
						return;
					 }
					 updataNum = getExtNum();
					isUseExtendGrooveSend = true;
					SoulProxy.getExtendInfo(updataNum);
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		
		private function dealAfterSend():void
		{
			if(isUseExtendGrooveSend)
			{
				isUseExtendGrooveSend = false;
				if(updataNum != getExtNum())
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_useExt_deal" ], color:0xffff00});//"开启了一个新槽"
					this.panelCloseHandler(null);
				}
			}
			(this.mainView.txt_explain as TextField).htmlText = this.getPercentTxt().eTxt;
		}
		private function showView():void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			moneyAll = SoulData.other.addAttributes;
			this.upDateNeedMoney(moneyAll);
			var infoObj:Object = getPercentTxt();
			(this.mainView.txt_percent as TextField).text = infoObj.pTxt+"%";
			(this.mainView.txt_explain as TextField).htmlText = infoObj.eTxt;
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
		
		private function getExtNum():int
		{
			var tag:int;
			for each(var obj:* in SoulMediator.soulVO.extProperties)
			{
				if(obj is SoulExtPropertyVO)
				{
					if((obj as SoulExtPropertyVO).state == 2)
					{
						tag = (obj as SoulExtPropertyVO).number;
						break;
					}
				}
			}
			return tag;
		}
		
		/**
		 * 获得开槽百分率 
		 * @return 
		 * 
		 */		
		private function getPercentTxt():Object
		{
			var percentTxt:String = "";
			var extArr:Array = SoulMediator.soulVO.extProperties;
			var tag:int = 0;
			for each(var obj:* in extArr)
			{
				if(obj == false)
				{
					continue;
				}
				if((obj as SoulExtPropertyVO).state < 2)
				{
					tag ++;
				}
			}
		
		     percentTxt = SoulData.getAttributesInfo(tag-1).addcombining;
			if(int(percentTxt) < 100)
			{
				(this.mainView.txt_percent as TextField).textColor = 0xFF0000;
			}
			
			var toolNum:int = BagData.hasItemNum(591003);
			
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+GameCommonData.wordDic[ "mod_soul_med_useExt_getP" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];//需要	个	天心箭	当前拥有	个
			return {pTxt:percentTxt,eTxt:explainTxt};
		}
		
		private function panelCloseHandler(e:Event):void
		{
			updataNum = 0;
			isUseExtendGrooveSend = false;
			dealEventListeners(false);
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
		}
	}
}