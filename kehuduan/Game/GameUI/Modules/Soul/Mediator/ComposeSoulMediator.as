package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject; 
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	/**
	 * 魂魄—合成魂魄
	 * @author lh
	 * 
	 */
	public class ComposeSoulMediator extends Mediator
	{
		public static const NAME:String = "ComposeSoulMediator";
		public static const INITMEDIATOR:String = "initComposeSoulMediator";
		public static const SHOWVIEW:String = "showComposeSoulPanel";
		public static const ADD_COMPOSE_ITEM_BY_CLICK:String = "addComposeItemByClick";
		public static const DEAL_AFTER_SEND_COMPSE_SOUL:String = "dealAfterSendComposeSoul";		
		
		public static var isComposeSoulSend:Boolean;
		private var gridMian:DisplayObjectContainer;
		private var gridSecond:DisplayObjectContainer;
		private var useItemMian:UseItem;
		private var useItemSecond:UseItem;
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var redShape:Shape;
		public static var isComposeSoulMediatorOpen:Boolean;
		
		private var panelBase:PanelBase;
		private var moneyAll:int;	//需要的金钱总数
		public function ComposeSoulMediator(mediatorName:String=null, viewComponent:Object=null)
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
				ADD_COMPOSE_ITEM_BY_CLICK,
				SoulProxy.CLOSE_ALL_SOUL_PANEL,
				DEAL_AFTER_SEND_COMPSE_SOUL,
				EventList.UPDATEMONEY
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case INITMEDIATOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"composeSoul"});
					panelBase = new PanelBase(mainView, mainView.width-12, mainView.height + 12 );
					panelBase.name = "ComposeSoulPanel";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x+200;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_soul_med_comSoul_hand" ]);	//"合成魂魄"
					initView();
				break;
				case SHOWVIEW:
					showView();
				break;
				case ADD_COMPOSE_ITEM_BY_CLICK:	//添加魂魄到合成界面
					addItemToCompose(notification.getBody());
				break;
				case SoulProxy.CLOSE_ALL_SOUL_PANEL:
					if(GameCommonData.GameInstance.GameUI.contains(this.mainView))
					{
						panelCloseHandler(null);
					}
				break;
				case DEAL_AFTER_SEND_COMPSE_SOUL:
					dealAfterSend();
				break;
				case EventList.UPDATEMONEY:
					upDataMoney();
				break;
				
			}
		}
		
		private function initView():void
		{
			(this.mainView.txt_percent as TextField).mouseEnabled = false;
			(this.mainView.txt_percent as TextField).text = "100%";
			(this.mainView.txt_mianLevel as TextField).mouseEnabled = false;
			(this.mainView.txt_secondLevel as TextField).mouseEnabled = false;
			gridMian = this.mainView.composeSoul_main;
			gridSecond = this.mainView.composeSoul_second;	
			
			
			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 75;
			bindMoneyItem.y = 279;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 75;
			unBindMoneyItem.y = 301;
			needMoney = new MoneyItem();
			needMoney.x = 75;
			needMoney.y = 257;
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
				GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
				return;
			}
			dealEventListeners(true);
			redShape = new Shape();
			upDateNeedMoney( 0 );
			isComposeSoulMediatorOpen = true;
			sendNotification(NewerHelpEvent.OPEN_BAG_NOTICE_NEWER_HELP);
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
		}
		
		private function dealEventListeners(isAdd:Boolean):void
		{
			if(isAdd)
			{
				(this.mainView.btn_sure as DisplayObject).addEventListener(MouseEvent.CLICK,onComposeEvent); 
				(this.mainView.btn_cancel as DisplayObject).addEventListener(MouseEvent.CLICK,onComposeEvent);
				gridMian.addEventListener(MouseEvent.MOUSE_UP,onMouseUp);
				gridSecond.addEventListener(MouseEvent.MOUSE_UP,onMouseUp);
				gridMian.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
				gridSecond.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
			}
			else
			{
				(this.mainView.btn_sure as DisplayObject).removeEventListener(MouseEvent.CLICK,onComposeEvent); 
				(this.mainView.btn_cancel as DisplayObject).removeEventListener(MouseEvent.CLICK,onComposeEvent);
				gridMian.removeEventListener(MouseEvent.MOUSE_UP,onMouseUp);
				gridSecond.removeEventListener(MouseEvent.MOUSE_UP,onMouseUp);
				gridMian.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
				gridSecond.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
				
			}
		}
		
		private function dealAfterSend():void
		{
			isComposeSoulSend = false;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_deal" ], color:0xffff00});	//"你的魂魄合成等级提升1"
			dealAfterComposeSoul();
		}

		private function addItemToCompose(data:Object):void
		{
			if(!SoulData.getSoulDetailById(data.id))
			{
				return;
			}
			if(!useItemMian)
			{
				(this.mainView.txt_mianLevel as TextField).text = SoulData.getSoulDetailById(data.id).composeLevel.toString();
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true;
				useItemMian = new UseItem(0,data.type,null);
				useItemMian.Id = data.id;
				useItemMian.x=2;
				useItemMian.y=2;

				this.gridMian.addChild(useItemMian);
				gridMian.addEventListener(MouseEvent.CLICK,onMouseClick);
				return;
			}
			if(!useItemSecond)
			{
				var obj:Object =  SoulData.getSoulDetailById(useItemMian.Id);
				if(SoulData.getSoulDetailById(useItemMian.Id).composeLevel != SoulData.getSoulDetailById(data.id).composeLevel)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_addItem" ], color:0xffff00});	//"无法合成，主副魂魄的等级不同"
					return;
				}
				
				BagData.SelectedItem.Item.IsLock = true;
				BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true;
				useItemSecond = new UseItem(0,data.type,null);
				useItemSecond.Id = data.id;
				useItemSecond.x=2;
				useItemSecond.y=2;
				(this.mainView.txt_secondLevel as TextField).text = SoulData.getSoulDetailById(data.id).composeLevel.toString();

				this.gridSecond.addChild(useItemSecond);
				gridSecond.addEventListener(MouseEvent.CLICK,onMouseClick);
				moneyAll = SoulData.compound[SoulData.getSoulDetailById(data.id).composeLevel - 1].gold;
				upDateNeedMoney(moneyAll);
			}
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			var dis:DisplayObject = me.target as DisplayObject;
			if(dis === gridMian)
			{
				if(useItemMian)
				{
					sendNotification(EventList.BAGITEMUNLOCK, this.useItemMian.Id);
					this.useItemMian.parent.removeChild(useItemMian);
					useItemMian.removeEventListener(MouseEvent.CLICK,onMouseClick);
					useItemMian = null;
					(this.mainView.txt_mianLevel as TextField).text = "0";
					upDateNeedMoney(0);
				}
				
				if(useItemSecond)
				{
					sendNotification(EventList.BAGITEMUNLOCK, this.useItemSecond.Id);
					this.useItemSecond.parent.removeChild(useItemSecond);
					useItemSecond.removeEventListener(MouseEvent.CLICK,onMouseClick);
					(this.mainView.txt_secondLevel as TextField).text = "0";
					useItemSecond = null;
				}
			}
			else if(dis === gridSecond)
			{
				if(useItemSecond)
				{
					sendNotification(EventList.BAGITEMUNLOCK, this.useItemSecond.Id);
					this.useItemSecond.parent.removeChild(useItemSecond);
					useItemSecond.removeEventListener(MouseEvent.CLICK,onMouseClick);
					(this.mainView.txt_secondLevel as TextField).text = "0";
					useItemSecond = null;
					upDateNeedMoney(0);
				}
			}
		}
		
		private function onMouseUp(me:MouseEvent):void
		{
			if(!BagData.SelectedItem)	//未选择物品
			{
				return;
			}
			if(BagData.SelectedItem.Item.IsLock) //物品已锁
			{
				return;
			}
			if(useItemMian)
			{
				if(useItemMian.Id == BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index].id)
				{
					return;
				}
			}
			
			if(int(BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index].type/10) != int(SoulData.soulType/10))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_onMouseUp_1" ], color:0xffff00});		//"您选择的物品不是魂魄"
				return;  
			} 
			var dis:DisplayObject = me.target as DisplayObject;
			if(dis.name == "composeSoul_main")
			{
				if(useItemMian)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_onMouseUp_2" ], color:0xffff00});	//"已有主魂魄，请放入副魂魄"
					return;
				}
				if(!useItemMian)
				{
					var data:Object = BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index];
					(this.mainView.txt_mianLevel as TextField).text = SoulData.getSoulDetailById(data.id).composeLevel.toString();
					BagData.SelectedItem.Item.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true;
					useItemMian = new UseItem(0,data.type,null);
					useItemMian.Id = data.id;
					useItemMian.x=2;
					useItemMian.y=2;
	
					this.gridMian.addChild(useItemMian);
					gridMian.addEventListener(MouseEvent.CLICK,onMouseClick);
					return;
					
				}
			}
			else if(dis.name == "composeSoul_second")
			{
				if(!useItemMian)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_1" ], color:0xffff00});	//"请先放入主魂魄"
					return;
				}
				if(!useItemSecond)
				{
					var data2:Object = BagData.AllItems[BagData.SelectIndex][BagData.SelectedItem.Index];
					if(SoulData.getSoulDetailById(useItemMian.Id).composeLevel != SoulData.getSoulDetailById(data2.id).composeLevel)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_addItem" ], color:0xffff00});	//"无法合成，主副魂魄的等级不同"
						return;
					}
					BagData.SelectedItem.Item.IsLock = true;
					BagData.AllLocks[BagData.SelectIndex][BagData.SelectedItem.Index] = true;
					useItemSecond = new UseItem(0,data2.type,null);
					useItemSecond.Id = data2.id;
					useItemSecond.x=2;
					useItemSecond.y=2;
					(this.mainView.txt_secondLevel as TextField).text = SoulData.getSoulDetailById(data2.id).composeLevel.toString();
	
					this.gridSecond.addChild(useItemSecond);
					gridSecond.addEventListener(MouseEvent.CLICK,onMouseClick);
					moneyAll = SoulData.compound[SoulData.getSoulDetailById(data2.id).composeLevel - 1].gold;
					upDateNeedMoney(moneyAll);
				}
			}
		}
		private function onMouseOver(me:MouseEvent):void
		{
			var dis:DisplayObject = me.target as DisplayObject;
			redShape.graphics.clear();	
			redShape.graphics.lineStyle(1,0xFF0000);
			redShape.graphics.drawRect(dis.x,dis.y,dis.width,dis.height);
			redShape.graphics.endFill();
			dis.parent.addChild(redShape);
			dis.addEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
		}
		
		private function onMouseOut(me:MouseEvent):void
		{
			(me.target as DisplayObject).removeEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
			if(redShape.parent)
			{
				redShape.parent.removeChild(redShape);
			}
		}
		
		private function onComposeEvent(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "btn_sure":
					if(!useItemMian)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_1" ], color:0xffff00});	//"请放入主魂魄" 
						return;    
					}
					if(!useItemSecond)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_2" ], color:0xffff00});	//"请放入副魂魄"
						return;     
					}
					 var money:int = SoulProxy.getPlayTotalMoney();
					 if(SoulProxy.getPlayTotalMoney() < moneyAll)
					{
					 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_comRun_onMouseC_4" ], color:0xffff00});//没有足够的天心箭 //"没有足够银两"
						return;
					}
					var secondData:SoulVO = SoulData.SoulDetailInfos[useItemSecond.Id];
					var soulObj:Object = IntroConst.ItemInfo[ secondData.id ];
					var kongNum:int;			//孔数
					if ( soulObj )
					{
						var kongArr:Array = soulObj.stoneList;
						for each(var i:uint in kongArr)
						{
							if(i!=99999 && i!=0)
							{
								kongNum++;
							}
						}
					}
					var showStr:String = "";
					if(kongNum > 0)
					{
						if(secondData.composeLevel == 2)
						{
							showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_4" ];//'合成等级从2提升到3时，魂魄会变为绑定不可摧毁的状态。注意：你的副魂魄不是原始的魂魄，合成后会<font color="#FF0000">消失</font>。是否确认要合成？';
							sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ], height: 77});	//"提 示"  "确 定"  "取 消"
						}
						else
						{
							showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_5" ];//'注意：你的副魂魄不是原始的魂魄，合成后会<font color="#FF0000">消失</font>。是否确认要合成？';
							sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});	//"提 示"  "确 定"  "取 消"
						}
						return;
					}
					if(secondData.level == 1 && secondData.growPercent == 500)
					{
						if((secondData.extProperties[1] is SoulExtPropertyVO) && (secondData.extProperties[1] as SoulExtPropertyVO).state < 2)
						{
							if(secondData.composeLevel == 2)
							{
								showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_4" ];//'合成等级从2提升到3时，魂魄会变为绑定不可摧毁的状态。注意：你的副魂魄不是原始的魂魄，合成后会<font color="#FF0000">消失</font>。是否确认要合成？';
								sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ],height: 77});//"提 示"		"确 定"  "取 消"	  
							}
							else
							{
								showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_5" ];//'注意：你的副魂魄不是原始的魂魄，合成后会<font color="#FF0000">消失</font>。是否确认要合成？';
								sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]	});//"提 示"		"确 定"  "取 消"	  
							}
						}
						else
						{
							if(secondData.composeLevel == 2)
							{
								showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_6" ];//'合成等级从2提升到3时，魂魄会变为绑定不可摧毁的状态。是否确认要合成？';
								sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});  //"提 示"		"确 定"  "取 消"
							}
							else
							{
								beSureToCommit();
							}
						}
					}
					else
					{
						if(secondData.composeLevel == 2)
						{
							showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_4" ];//'合成等级从2提升到3时，魂魄会变为绑定不可摧毁的状态。注意：你的副魂魄不是原始的魂魄，合成后会<font color="#FF0000">消失</font>。是否确认要合成？';
							sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ],height: 77});//"提 示"		"确 定"  "取 消"
						}
						else
						{
							showStr = GameCommonData.wordDic[ "mod_soul_med_comSoul_onCom_5" ];//'注意：你的副魂魄不是原始的魂魄，合成后会<font color="#FF0000">消失</font>。是否确认要合成？';
							sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:beCancel, info:showStr ,title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});//"提 示"		"确 定"  "取 消"
						}
					}
				break;
				case "btn_cancel":
					this.panelCloseHandler(null);
				break;
			}
		}
		
		private function beCancel():void
		{
			if(useItemMian)
			{
				sendNotification(EventList.BAGITEMUNLOCK, this.useItemMian.Id);
				this.useItemMian.parent.removeChild(useItemMian);
				useItemMian.removeEventListener(MouseEvent.CLICK,onMouseClick);
				useItemMian = null;
				(this.mainView.txt_mianLevel as TextField).text = "0";
			}
			if(useItemSecond)
			{
				sendNotification(EventList.BAGITEMUNLOCK, this.useItemSecond.Id);
				this.useItemSecond.parent.removeChild(useItemSecond);
				useItemSecond.removeEventListener(MouseEvent.CLICK,onMouseClick);
				useItemSecond = null;
				(this.mainView.txt_secondLevel as TextField).text = "0";
			}
		}
		private function beSureToCommit():void
		{
			isComposeSoulSend = true;
			SoulProxy.getComposeSoulInfo(useItemMian.Id,useItemSecond.Id);
		}
		
		private function dealAfterComposeSoul():void
		{
			if(useItemMian)
			{
				sendNotification(EventList.BAGITEMUNLOCK, this.useItemMian.Id);
				this.useItemMian.parent.removeChild(useItemMian);
				useItemMian.removeEventListener(MouseEvent.CLICK,onMouseClick);
				useItemMian = null;
				(this.mainView.txt_mianLevel as TextField).text = "0";
				upDateNeedMoney(0);
			}
			if(useItemSecond)
			{
				sendNotification(EventList.BAGITEMUNLOCK, this.useItemSecond.Id);
				this.useItemSecond.parent.removeChild(useItemSecond);
				useItemSecond.removeEventListener(MouseEvent.CLICK,onMouseClick);
				useItemSecond = null;
				(this.mainView.txt_secondLevel as TextField).text = "0";
			}
			isComposeSoulSend = false;
				
		}
		
		private function panelCloseHandler(e:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				isComposeSoulMediatorOpen = false;
				dealEventListeners(false);
				if(useItemMian)
				{
					sendNotification(EventList.BAGITEMUNLOCK, this.useItemMian.Id);
					this.useItemMian.parent.removeChild(useItemMian);
					useItemMian.removeEventListener(MouseEvent.CLICK,onMouseClick);
					useItemMian = null;
					(this.mainView.txt_mianLevel as TextField).text = "0";
				}
				if(useItemSecond)
				{
					sendNotification(EventList.BAGITEMUNLOCK, this.useItemSecond.Id);
					this.useItemSecond.parent.removeChild(useItemSecond);
					useItemSecond.removeEventListener(MouseEvent.CLICK,onMouseClick);
					useItemSecond = null;
					(this.mainView.txt_secondLevel as TextField).text = "0";
				}
				isComposeSoulSend = false;
				redShape.graphics.clear();
				if(redShape.parent)
				{
					redShape.parent.removeChild(redShape);
				}
				redShape = null;
				GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			}
		}
	}
}