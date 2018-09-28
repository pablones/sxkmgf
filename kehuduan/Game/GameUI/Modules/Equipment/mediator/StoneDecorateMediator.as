package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Equipment.ui.EnableItem;
	import GameUI.Modules.Equipment.ui.ListCell;
	import GameUI.Modules.Equipment.ui.NumberItem;
	import GameUI.Modules.Equipment.ui.UIList;
	import GameUI.Modules.Equipment.ui.event.ListCellEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.UIUtils;
	
	import Net.ActionSend.EquipSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StoneDecorateMediator extends Mediator
	{
		public static const NAME:String = "StoneDecorateMediator";
		/** 购买宝石列表*/
		private var itemList:UIList;
		/** 购买图标 */
		private var buyItem:NumberItem;
		/** 购买物品数据*/
		private var selectedItemData:Object;
		private var helpItem:EnableItem;
		private var helpItemData:Object;
		/**宝石图标 */
		private var stoneItem:NumberItem;
		private var stoneData:Object;		//雕琢的宝石信息
		private var isSendBuy:Boolean;	//是否发送了购买辅助道具信息
		public function StoneDecorateMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.view.addEventListener(Event.ADDED_TO_STAGE,onAddToStage);
			this.view.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStage);
		}
		
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		private function onRemoveFromStage(e:Event):void{
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,0);	
			if(stoneData)
			{
				onStoneItemClick(null);
			}
			UIUtils.removeFocusLis(this.view.txt_inputNum);
			EquipDataConst.getInstance().lockItems=new Dictionary();
			(this.view.btn_commit as SimpleButton).visible=false;
		}
		
		override public function listNotificationInterests():Array{
			return [
				EquipCommandList.ADD_DECORATE_ITEM,
				EquipCommandList.STONE_DECORATE_SEND,
				EventList.ONSYNC_BAG_QUICKBAR
			];
		}
		
		override public function handleNotification(note:INotification):void{
			switch(note.getName())
			{
				case EquipCommandList.ADD_DECORATE_ITEM:
					
					if(note.getBody().type == 610056)	//雕琢宝符
					{
						if(!stoneData)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equip_med_stonec_hand" ], color:0xffff00});//请放入要雕琢的宝石
							return;
						}
						else
						{
							addHelpItem();
						}
					}
					else
					{
						addStone(note.getBody());
					}
				break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if(isSendBuy)
					{
						dealAfterBuyHelpItem();
					}
				break;
				case EquipCommandList.STONE_DECORATE_SEND:
					dealAfterSend(note.getBody());
				break;
				
			}
		}
		
		private function onAddToStage(e:Event):void{
			(this.view.btn_1 as SimpleButton).addEventListener(MouseEvent.MOUSE_DOWN,onBuyListBtnClick);
			(this.view.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyItemClick);
		    (this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
			(this.view.btn_buyHelpItem as SimpleButton).addEventListener(MouseEvent.CLICK,onQuickButItem);
		    (this.view.btn_commit as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).mouseEnabled=false;
			this.helpItemData=null;
			(this.view.txt_1 as TextField).mouseEnabled=false;
			(this.view.txt_inputNum as TextField).restrict="0-9";
			(this.view.txt_inputNum as TextField).multiline=false;
			(this.view.txt_inputNum as TextField).mouseWheelEnabled=false;
			(this.view.txt_inputNum as TextField).maxChars=4;
			UIUtils.addFocusLis(this.view.txt_inputNum);
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,0);
			
			this.view.txt_1.text=GameCommonData.wordDic[ "mod_equ_med_enc_ini" ];//"选择道具"
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			this.selectedItemData=null;
			
			if(this.itemList==null){
				this.itemList=new UIList(80,20);
				this.itemList.rendererClass=ListCell;
				this.itemList.addEventListener(ListCellEvent.LISTCELL_CLICK,onListCellClick);
			}
			
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			EquipDataConst.getInstance().lockItems=new Dictionary();
			this.stoneData=null;
		}
		
		/**
		 * 添加宝石 
		 * @param obj
		 * 
		 */		
		private function addStone(obj:Object):void
		{
			if(stoneData)
			{
				onStoneItemClick(null);
			}
			stoneData = obj;
			stoneItem = new NumberItem(obj.type,"icon");
			stoneItem.addEventListener(MouseEvent.CLICK,onStoneItemClick);
			this.stoneItem.name="goodQuickBuy_"+obj.type;
			stoneItem.x = this.view.mc_container.x+3;
			stoneItem.y = this.view.mc_container.y+3;
			this.view.addChild(stoneItem);
			
			if(this.stoneData!=null)
			{
				EquipDataConst.getInstance().lockItems[stoneData.id]=true;
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			var moneyNeeded:int = EquipDataConst.getInstance().decorateMoneyNeeded[obj.type%10];
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,moneyNeeded);
			(this.view.btn_commit as SimpleButton).visible=true;
			addHelpItem();
		}
		
		
		private function onStoneItemClick(me:MouseEvent):void
		{
			stoneItem.removeEventListener(MouseEvent.CLICK,onStoneItemClick);
			this.view.removeChild(stoneItem);
			EquipDataConst.getInstance().lockItems[stoneData.id]=false;
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			(this.view.btn_commit as SimpleButton).visible=false;
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,0);
			if(helpItem)
			{
				onMcToolClick(null);
			}
			stoneItem = null;
			stoneData = null;
		}
		
		/**
		 * 添加雕琢宝符 
		 * 
		 */		
		private function addHelpItem():void
		{
			var helpData:Object = EquipDataConst.getInstance().getDecorateItem();
			if(helpItem)
			{
				onMcToolClick(null);
			}
			helpItemData = helpData;
			helpItem = new EnableItem("610056","icon");	
			helpItem.setEnable(true);
			if(helpData)
			{
				helpItem.setEnable(false);
				helpItem.addEventListener(MouseEvent.CLICK,onMcToolClick);
				EquipDataConst.getInstance().lockItems[helpData.id]=true;
			}
			helpItem.name = "goodQuickBuy_610056";
			helpItem.x = this.view.mc_tool.x+3;
			helpItem.y = this.view.mc_tool.y+3;
			this.view.addChild(helpItem);
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
		}
		private function onMcToolClick(me:MouseEvent):void
		{
			if(helpItem)
			{
				if(helpItem.hasEventListener(MouseEvent.CLICK))
				{
					helpItem.removeEventListener(MouseEvent.CLICK,onMcToolClick);
				}
				if(this.view.contains(helpItem))
				{
					this.view.removeChild(helpItem);
				}
				if(helpItemData)
				{
					EquipDataConst.getInstance().lockItems[helpItemData.id]=false;
				}
				sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
				helpItem = null;
				helpItemData = null;
			}
		}
		
		private function onBuyListBtnClick(e:MouseEvent):void{
			e.stopPropagation();
			var arr:Array=UIConstData.MarketGoodList[31] as Array;
			this.itemList.dataPro=arr;
			this.itemList.x=242;
			this.itemList.y=141;
			if(this.itemList.parent!=null){
				this.itemList.parent.removeChild(this.itemList);
			}else{
				sendNotification(EquipCommandList.SHOW_COMBOX_LIST,this.itemList);
			}
		}
		
		private function onBuyItemClick(e:MouseEvent):void{
			if(this.buyItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onG_1" ], color:0xffff00});//"请先选择你要购买的道具"
			}else{
				var num:uint=uint(this.view.txt_inputNum.text);
				if(num == 0){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onG_2" ], color:0xffff00});//"请输入有效的购买数"
					return;
				}
				var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+selectedItemData.PriceIn*num+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+num+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+selectedItemData.Name+'</font>';
				//	花费		购买		个
				facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuy, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
			}
		}
		
		private function onSureToBuy():void{
			if(selectedItemData.type == 610056)
			{
				isSendBuy = true;
			}
			var num:uint=uint(this.view.txt_inputNum.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:this.selectedItemData.type,count:num});	
		}
		
		/**
		 * 快速购买 
		 * @param e
		 * 
		 */		
		private function onQuickButItem(me:MouseEvent):void
		{
			var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">98</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+1+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+GameCommonData.wordDic[ "mod_equip_med_stonec_onQ" ]+'</font>';//"雕琢符"
					//花费		购买		个
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
		}		
		
		private function onSureToBuyHelp():void
		{
			isSendBuy = true;
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:610056,count:1});
		}
		/**
		 * 提交镶嵌 
		 * @param e
		 * 
		 */		
		private function onCommitClick(e:MouseEvent):void{
			if(this.helpItemData==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equip_med_stonec_onC" ], color:0xffff00});//"没有雕琢符"
				return;
			}
			
			var moneyNeeded:int;
			if(stoneData)
			{
				moneyNeeded = EquipDataConst.getInstance().decorateMoneyNeeded[stoneData.type%10];
			}
			else
			{
				return;
			}
			var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(money<moneyNeeded){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//你的银两不足
				return;
			}
			
			 var param:Array=[1,this.helpItemData.id,79,this.stoneData.id,0];
			EquipSend.createMsgCompound(param);
			 
				
		}
		private function onListCellClick(e:ListCellEvent):void{
			this.view.txt_1.text=e.data["Name"];
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			this.buyItem=new NumberItem(e.data.type,"icon");
			this.buyItem.mouseEnabled=true;
			this.buyItem.name="goodQuickBuy_"+e.data.type;
			this.view.addChild(this.buyItem);
			this.buyItem.x=258;
			this.buyItem.y=33;
			this.selectedItemData=e.data;
		}
		/**
		 *购买辅助道具后处理 
		 * 
		 */		
		private function dealAfterBuyHelpItem():void
		{
			isSendBuy = false;
			if(!stoneData)
			{
				return;
			}
			addHelpItem();
		}
		/**
		 *雕琢成功后处理 
		 * @param obj
		 * 
		 */		
		private function dealAfterSend(obj:Object):void
		{
			if(obj.type == 1)	//成功
			{
				onStoneItemClick(null);
				var id:int = obj.idItem;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equip_med_stonec_deal_1" ], color:0xffff00});//雕琢成功
			}
			else if(obj.type == 2)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equip_med_stonec_deal_2" ], color:0xffff00});//雕琢失败
			}
		}
	}
}