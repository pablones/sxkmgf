package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.SoulComponents;
	import GameUI.Modules.ToolTip.Const.IntroConst;
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
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄-升级扩展属性
	 * @author lh
	 * 
	 */
	public class ImproveExtendProMediator extends Mediator
	{
		public static const NAME:String = "ImproveExtendProMediator";
		public static const INITMEDIATOR:String = "initImproveExtendProMediator";
		public static const SHOWVIEW:String = "showImproveExtendPanel";
		public static const DEAL_AFTER_SEND_IMPROVE_EXT_PRO:String = "dealAfterSendImproveExtendPro";
		
		public static var isImproveExtendSend:Boolean;
		public var panelBase:PanelBase;
		private var tempString:String;	//临时选的的扩展属性按钮名
		private var moneyAll:int;	//需要的金钱总数
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var yellowShape:Shape;
		private var redShape:Shape;
		
		public function ImproveExtendProMediator(mediatorName:String=null, viewComponent:Object=null)
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
				DEAL_AFTER_SEND_IMPROVE_EXT_PRO,
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				EventList.UPDATEMONEY
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"improveExtendPro"});
					panelBase = new PanelBase(mainView, mainView.width-22, mainView.height + 12 );
					panelBase.name = "ImproveExtendPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_impExt_hand" ]);	//"升级扩展属性"
					initView();
				break;
				case SHOWVIEW:
					showView(int(notification.getBody()));
				break;
				case DEAL_AFTER_SEND_IMPROVE_EXT_PRO:
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
		
		/**
		 *初始化面板 
		 * 
		 */		
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false; 
			(this.mainView.txt_explain as TextField).mouseEnabled = false; 
			
			for(var i:int = 0; i < 10; i++)
			{
				(this.mainView["txt_extendPro"+i] as TextField).mouseEnabled = false;
			}
			
			bindMoneyItem = new MoneyItem();
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
		
		private function showView(improveNum:int):void
		{
			sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.panelCloseHandler(null);
				return;
			}
			this.initExtendPropertyView();
			yellowShape = new Shape();
			redShape = new Shape();
			initBtnState(improveNum);		//初始按钮状态
			/* var percent:int = SoulData.getAttributesInfo(SoulMediator.soulVO.extProperties[improveNum].level).addcombining;
			(this.mainView.txt_percent as TextField).text = percent.toString()+"%"; */
			(this.mainView.txt_percent as TextField).text = "100%";
			(this.mainView.txt_explain as TextField).htmlText = getTxtInfo(SoulMediator.soulVO.extProperties[improveNum].level);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			dealEventListeners(true);
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
		/**
		 * 初始化条目按钮状态 
		 * @param improveNum
		 * 
		 */		
		private function initBtnState(improveNum:int):void
		{
			var extContainer:DisplayObjectContainer = this.mainView["btn_extendPro"+improveNum];
			var num:int = extContainer.numChildren - 1;
			while(num >= 0)
			{
				var dis:DisplayObject = extContainer.getChildAt(num);
				if(dis)
				{
					if(dis.name.split("_")[1] == "hasLearn")
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
			var toolNum:int = BagData.hasItemNum(591100+num);
			
			var explainTxt:String = GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_1" ]+'<font color="#00FF00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+num+GameCommonData.wordDic[ "mod_soul_med_comRun_showView" ]+'</font><br>'+GameCommonData.wordDic[ "mod_soul_med_comRun_getPre_2" ]+'<font color="#00FF00">'+ toolNum + '</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ];	//需要	个	级润魂石	当前拥有	个
			return explainTxt;
		}
		
		private function dealAfterSend():void
		{
			this.initExtendPropertyView();
			isImproveExtendSend = false;
			facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "mod_soul_med_impExt_deal" ], color:0xffff00});		//"扩展等级提升成功"
			if(tempString)
			{
				var extVo:SoulExtPropertyVO = SoulMediator.soulVO.extProperties[tempString.split("_")[2]];
				moneyAll = SoulData.getAttributesInfo(extVo.level - 1).upGold;
				upDateNeedMoney(moneyAll);
				(this.mainView.txt_explain as TextField).htmlText = getTxtInfo(extVo.level);
				
			}
			
			
		}
		
		/**
		 *初始化界面 
		 * 
		 */		
		private function initExtendPropertyView():void
		{
			var extArr:Array = SoulMediator.soulVO.extProperties;
			
			gcExtContainer();
			var soulcomp:SoulComponents = SoulComponents.getInstance();	//获取按钮组件
			if(SoulMediator.isEquiptSoul)
			{
				for(var i:int = 0; i < 10; i ++)
				{
					var extComp:DisplayObjectContainer = this.mainView["btn_extendPro"+i];	//对应的面板中的扩展属性按钮组件
					if(extArr[i] == false)
					{
						var notUseComp:SimpleButton = soulcomp.getNotLearn()
						notUseComp.name = "soulExt_notUse_" + i;
						extComp.addChild(notUseComp);
					}
					else
					{
						var extVo:SoulExtPropertyVO = extArr[i] as SoulExtPropertyVO;
						if(extVo.state == 0) //已使用
						{
							var hasLearnComp:SimpleButton = soulcomp.getHasLearn();
							hasLearnComp.name = "soulExt_hasLearn_" + i;
							setHasLearnTxt(extVo);
							/* (this.mainView["txt_extendPro" +i] as TextField).text = extVo.name+"("+extVo.level+"级)";
							(this.mainView["txt_extendPro" +i] as TextField).textColor = 0XFFFFFF; */
							hasLearnComp.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
							hasLearnComp.addEventListener(MouseEvent.CLICK,onMouseClick);
							extComp.addChild(hasLearnComp);
						}
						else if(extVo.state == 1)	//可学习
						{
							var canLearnComp:SimpleButton = soulcomp.getHasLearn();
							canLearnComp.name = "soulExt_canLearn_" + i;
							extComp.addChild(canLearnComp);
						}
						else if(extVo.state == 2)	//可开槽
						{
							var notLearnComp:SimpleButton = soulcomp.getCanLearn();
							notLearnComp.name = "soulExt_canUseToLearn_" + i;
							extComp.addChild(notLearnComp);
						}
					}
					
				}
			}
			else
			{
				for(var j:int = 0; j < 10; j ++)
				{
					var extComp2:DisplayObjectContainer = this.mainView["btn_extendPro"+i];	//对应的面板中的扩展属性按钮组件
					(this.mainView["txt_extendPro"+j] as TextField).text = "";
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
			(this.mainView["txt_extendPro" +extVo.number] as TextField).htmlText = '<font color="'+color+'">'+extVo.name+"("+extVo.level+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+")"+'</font>';	//级
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
			var data:Array = (me.target.name as String).split("_");
			switch(data[1])	
			{
				case "compose":
					sendNotification(ComposeRunStoneMediator.SHOWVIEW);
				break;
				case "hasLearn":
					tempString = me.target.name;
					addYellowFrame(me.target as DisplayObject);
					var extVo:SoulExtPropertyVO = SoulMediator.soulVO.extProperties[int(data[2])]; 
					if(extVo.level == 10) return;
					moneyAll = SoulData.getAttributesInfo(extVo.level - 1).upGold;
					upDateNeedMoney(moneyAll);
					(this.mainView.txt_explain as TextField).htmlText = getTxtInfo(extVo.level);
				break;
				case "sure":
					if(tempString)
					{
						var extVo2:SoulExtPropertyVO = SoulMediator.soulVO.extProperties[tempString.split("_")[2]];
						if(extVo2.level == 10)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "mod_soul_med_impExt_onMouseC_1" ], color:0xffff00});	//"属性值已满，无需升级"
							return;
						}
						if(BagData.isHasItem(591100+extVo2.level) == 0)//是否有对应的润魂石
						{
							facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "mod_soul_med_impExt_onMouseC_2" ], color:0xffff00});//"您的背包中没有对应的润魂石"
							return;
						}
						if(SoulProxy.getPlayTotalMoney() < moneyAll)
						 {
						 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//"没有足够银两"
							return;
						 }
						isImproveExtendSend = true;
						var level:int = (SoulMediator.soulVO.extProperties[tempString.split("_")[2]] as SoulExtPropertyVO).level; 
						SoulProxy.getUpdateExtend(int(tempString.split("_")[2]) + 1);
						
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "mod_soul_med_impExt_onMouseC_3" ], color:0xffff00});//"请先选择要升级的属性"
						return;
					}
					
				break;
				case "cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		private function panelCloseHandler(e:Event):void
		{
			tempString = null;
			isImproveExtendSend = false;
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