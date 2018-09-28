package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Equipment.ui.EnableItem;
	import GameUI.Modules.Equipment.ui.ListCell;
	import GameUI.Modules.Equipment.ui.NumberItem;
	import GameUI.Modules.Equipment.ui.StilettoCell;
	import GameUI.Modules.Equipment.ui.UIDataGrid;
	import GameUI.Modules.Equipment.ui.UIList;
	import GameUI.Modules.Equipment.ui.event.ListCellEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.ItemInfo;
	import Net.ActionSend.EquipSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 *  装备打孔
	 * @author felix
	 * 
	 */	
	public class StilettoMediator extends Mediator
	{
		public static var NAME:String="StilettoMediator"; 
		/** 购买打孔针的列表*/
		protected var itemList:UIList;
		/** 选择购买打孔的数据 */
		protected var selectedItemData:Object;
		
		protected var buyItem:NumberItem;
		/** 打孔针图标*/
		protected var helpItem:EnableItem;
		/** 打孔符是否可用*/
		protected var helpItemEnable:Boolean=true;
		/** 打孔针数据*/
		protected var helpItemData:Object;
		
		protected var helpItemBagData:Object;
		
		/** 装备数据 */
		protected var equipData:Object;
		/** 装备图标  */
		protected var useItem:UseItem;
		/** 购买标记*/	 
		protected var isBuyFlag:Boolean;
		/** 孔列表 */
		protected var stilettoList:UIDataGrid;
		/** 孔数据 */
		protected var stilettoItems:Array=[[]];
		
		
			
		public function StilettoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.view.addEventListener(Event.ADDED_TO_STAGE,onAddToStage);
			this.view.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStage);
		}
		
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		
		protected function onAddToStage(e:Event):void{
			this.init();
		}
		
		protected function onRemoveFromStage(e:Event):void{
			this.clearAll();
		}
		
		
		/**
		 * 初始化，面板显示时的初始化 
		 * 
		 */		
		protected function init():void{
			if(this.itemList==null){
				this.itemList=new UIList(80,20);
				this.itemList.rendererClass=ListCell;
				this.itemList.addEventListener(ListCellEvent.LISTCELL_CLICK,onListClick);
				var arr:Array=UIConstData.MarketGoodList[13] as Array;
				this.itemList.dataPro=arr;
			}
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			
			if(this.stilettoList==null){
				if(EquipDataConst.isFourthStilettoOpen){
					this.stilettoList=new UIDataGrid(194,115);					
				}else{
					this.stilettoList=new UIDataGrid(194,80);
				}
				this.stilettoList.hPadding=0;
				this.stilettoList.vPadding=0;
				this.stilettoList.rendererClass=StilettoCell;
				this.stilettoList.x=74;
				this.stilettoList.y=117;                            
				
			}else if(this.view.contains(this.stilettoList)){
				this.view.removeChild(this.stilettoList);
			}
			(this.view.txt_inputNum as TextField).restrict="0-9";
			(this.view.txt_inputNum as TextField).multiline=false;
			(this.view.txt_inputNum as TextField).mouseWheelEnabled=false;
			(this.view.txt_inputNum as TextField).maxChars=3;
			UIUtils.addFocusLis(this.view.txt_inputNum);
			this.view.txt_1.text=GameCommonData.wordDic[ "mod_equ_med_sti_ini" ];//"选择道具"
			(this.view.txt_1 as TextField).mouseEnabled=false;
			this.selectedItemData=null;
			this.equipData=null;
			this.helpItemBagData=null;
			this.view.txt_success.text="0%";
			this.view.txt_success.mouseEnabled=false;
			//不显示所需的金钱数
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");
			(this.view.btn_1 as SimpleButton).addEventListener(MouseEvent.MOUSE_DOWN,onBuyItemClick);
			(this.view.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyClick);
			(this.view.btn_buyHelpItem as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyHelpItemClick);
			(this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
			(this.view.btn_commit as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).mouseEnabled=false;
			(this.view.btn_buyHelpItem as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).visible=false;			
		}
		
		
		protected function onCommitClick(e:MouseEvent):void{
			if(this.useItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_1" ], color:0xffff00});//"请先放入你强化的装备"
				return;
			}
			
			
			this.equipData=IntroConst.ItemInfo[this.equipData.id];
			var tag:Boolean;
			for each(var num:int in equipData.stoneList){
				if(num == 99999){
					tag = true;
					break;
				}
			}
			if(!tag){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_hun_onC_1" ], color:0xffff00});//"此装备已经没有位置可以开孔"
				return;
			}
			
			
			if(this.helpItemBagData==null || this.helpItemEnable==true){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_hun_onC_2" ], color:0xffff00});//"没有放入相对应的打孔符"
				return;
			}
			
			var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(this.view.txt_success.text=="100%"){
				if(EquipDataConst.isFourthStilettoOpen){	//第四孔开放
					if(equipData.stoneList[0] != 99999 && equipData.stoneList[3] == 99999 && money < 200000){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//"你的银两不足"
						return
					}
				}
				if(money<30000){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//"你的银两不足"
					return;
				}
			}else if(this.view.txt_success.text=="50%"){
				if(money<60000){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//"你的银两不足"
					return;
				}
				
			}else if(this.view.txt_success.text=="25%"){
				if(money<90000){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//"你的银两不足"
					return;
				}
			}
			if(equipData.isBind==0 && helpItemBagData.isBind==1){ 
				facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:new Function(), info:GameCommonData.wordDic[ "mod_equ_med_sti_onC_3" ],title:GameCommonData.wordDic[ "often_used_tip" ]});//"使用绑定的打孔符，打孔后装备将自动绑定"   "提 示"
				return;
			}
//			if(EquipDataConst.isFourthStilettoOpen){
//				if(equipData.stoneList[2] != 99999 && equipData.stoneList[3] == 99999){
//					this.helpItemBagData.id = 15171;
//				}
//			}
			var param:Array=[1,1,23,this.equipData.id,this.helpItemBagData.id];
			EquipSend.createMsgCompound(param);	
					
		}
		
		/**
		 * 确定使用绑定的打孔符
		 *               
		 */	
		protected function beSureToCommit():void{
			var param:Array=[1,1,23,this.equipData.id,this.helpItemBagData.id];
			EquipSend.createMsgCompound(param);	
		}
		 
		
		/**
		 * 判断是否有相应的绑定的符 
		 * @param type
		 *               
		 */		
		protected function isHasBindItem(type:uint):Boolean{
			var itemType:uint=0;
			if(type>=0 && type<=2){
				itemType=610012;
			}else if(type>=3 && type<=5){
				itemType=610013;
			}else if(type>=6 && type<=9){
				itemType=610014;	
			}
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			for(var i:uint=0;i<len;i++){
				var obj:Object=arr[i];
				if(obj==null)continue;
				if(obj.type==itemType && obj.isBind==1){
					return true;
				}
			}
			return false;
		}
		
		protected function onBuyItemClick(e:MouseEvent):void{
			e.stopPropagation();
			
			this.itemList.x=242;
			this.itemList.y=141;
			if(this.itemList.parent!=null){
				this.itemList.parent.removeChild(this.itemList);
			}else{
				sendNotification(EquipCommandList.SHOW_COMBOX_LIST,this.itemList);
			}
		}
		
		/**
		 * 选择购买打孔符 
		 * @param e
		 * 
		 */		
		protected function onBuyClick(e:MouseEvent):void{
			if(this.buyItem==null || selectedItemData==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sti_onBuyC" ], color:0xffff00});//"请选择你要购买的打孔符"
			}else{
				if(selectedItemData.type == 610057){   //虚空破碎针
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyH" ], color:0xffff00});//"暂无此物品出售"
					return;
				}
				var num:uint=uint(this.view.txt_inputNum.text);
				if(num == 0){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyI_2" ], color:0xffff00});//"请输入有效的购买数"
					return;
				}
				var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+selectedItemData.PriceIn*num+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+num+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+selectedItemData.Name+'</font>';
				//花费		购买		个
				facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuy, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
			}
		}
		
		protected function onSureToBuy():void{
			var num:uint=uint(this.view.txt_inputNum.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:selectedItemData.type,count:num});
		}
		
		/** 
		 * 快速购买打孔符 
		 * @param e
		 * 
		 */		
		protected function onBuyHelpItemClick(e:MouseEvent):void{
			if(this.helpItemEnable==false){
				var showStr:String = GameCommonData.wordDic[ "mod_equ_med_sti_onBuyH" ];
				if(this.helpItemData.type == 610057){	//虚空破碎针
					showStr = GameCommonData.wordDic[ "mod_equ_med_add_onBuyH" ];
				}
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:showStr, color:0xffff00});//
				return;
			}
			if(this.helpItemData!=null){
				var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+helpItemData.PriceIn*1+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+1+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+helpItemData.Name+'</font>';
				//花费		购买		个
				if(this.helpItemData.type == 610057){	//虚空破碎针
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyH" ], color:0xffff00});//"暂无此物品出售"
					return;
				}
				facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
			}
		}
		protected function onSureToBuyHelp():void{
			this.isBuyFlag=true;
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:this.helpItemData.type,count:1});			
		}
		
		
		protected function onListClick(e:ListCellEvent):void{
			this.view.txt_1.text=e.data["Name"];
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			this.buyItem=new NumberItem(e.data.type,"icon");
			this.view.addChild(this.buyItem);
			this.buyItem.x=258;
			this.buyItem.y=33;
			this.buyItem.mouseEnabled=true;
			this.buyItem.name="goodQuickBuy_"+e.data.type;
			this.selectedItemData=e.data;
		}
		
		
		public override function listNotificationInterests():Array{
			return [
				EquipCommandList.ADD_STILETTO_EQUIP,
				EquipCommandList.ADD_STILETTO_ITEM,
				EquipCommandList.BUY_STRENGENHELP_ITEM,
				EquipCommandList.RECALL_EQUIPSTILETTO,
				EquipCommandList.REFRESH_EQUIP,
				EquipCommandList.DROP_EQUIP_ITEM
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			if(this.view.stage==null)return;
			switch (notification.getName()){
				case EquipCommandList.ADD_STILETTO_EQUIP:
					if(this.isEmpty(notification.getBody())){
						this.equipData=notification.getBody();
						this.addEquip();
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sti_han_1" ], color:0xffff00});//"已经没有位置可以打孔"
						sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
					}
					break;
						
				case EquipCommandList.ADD_STILETTO_ITEM:
					if(this.useItem==null){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
						return;
					}else{
						if(this.helpItemEnable==false){
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sti_han_2" ], color:0xffff00});//"已经有相应的打孔符了"
							return;	
						}
						this.helpItemBagData=notification.getBody();
						EquipDataConst.getInstance().lockItems[this.helpItemBagData.id]=true;
						this.helpItemEnable=false;
						this.changeDes();
					}	
					break;	
				//购买符返回消息 obj==1.证明购买失败			
				case EquipCommandList.BUY_STRENGENHELP_ITEM:
					if(this.view.stage==null)return;
					if(notification.getBody() && notification.getBody()==1){
						this.isBuyFlag=false;
					}else{
						if(this.isBuyFlag){
							this.isBuyFlag=false;
						}
						if(this.helpItemData==null)return;
						this.lockItem(this.helpItemData.type);
						sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
						this.changeDes();
					}
					break;	
				/** 打孔后的返回信息  24：成功  25：失败*/	
				case EquipCommandList.RECALL_EQUIPSTILETTO:
					if(notification.getBody()["type"]==24){
						sendNotification(EquipCommandList.ADD_SPECIAL_SHOW,new Point(164,56));
						ItemInfo.isLevelUp=true;
						UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
					}else if(notification.getBody()["type"]==25){
						this.helpItemEnable=true;
						this.helpItemBagData=null;
						this.changeDes();
					}
					break;
				case EquipCommandList.REFRESH_EQUIP:
					 if(this.view.stage==null)return;
					 this.helpItemBagData=null;
					 this.helpItemEnable=true;
					 equipData = IntroConst.ItemInfo[equipData.id]
					 if(EquipDataConst.isFourthStilettoOpen){
						 /** *第四个孔要跟换辅助道具 */
						 if(equipData.stoneList[2] != 99999 && equipData.stoneList[3] == 99999){
						  	this.helpItemData=UIConstData.MarketGoodList[13][12];
						 }
				     }
					this.changeDes();
					 EquipDataConst.getInstance().lockItems=new Dictionary();
					break;	
				case EquipCommandList.DROP_EQUIP_ITEM:
					if(this.view.stage==null || this.equipData==null)return;
					var itemId:int = notification.getBody().id;
					if(this.equipData.id==itemId){
						this.clearAll();
					}
					else if(this.helpItemBagData && this.helpItemBagData.id==itemId){
						if(!this.lockItem(this.helpItemBagData.type) && this.helpItem){
							this.helpItemEnable=true;
							this.helpItem.setEnable(this.helpItemEnable);
							this.helpItem.mouseEnabled=false;
						}
					}
					break;		
			}
		}
		
		protected function lockItem(type:uint):Boolean{
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			var isHaveType:Boolean;
			for (var i:uint=0;i<len;i++){
				if(arr[i] && arr[i].type==type){
					EquipDataConst.getInstance().lockItems=new Dictionary();
					EquipDataConst.getInstance().lockItems[arr[i].id]=true;
					this.helpItemBagData=arr[i];
					this.helpItemEnable=false;
					isHaveType = true;
					break;
				}
			}
			return isHaveType;
		}
		
		/**
		 *  
		 * @param obj
		 * @return 
		 * 
		 */		
		protected function isEmpty(obj:Object):Boolean{
			var arr:Array=obj.stoneList;
			for each(var o:Object in arr){
				if(o==99999){
					return true;
				}
			}
			return false;		
		}
		
		
		protected function addEquip():void{
			
			
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;
				this.helpItemBagData=null;
				this.helpItemEnable=true;
				EquipDataConst.getInstance().lockItems=new Dictionary();
			}
			this.useItem=new UseItem(0,this.equipData.type,null);
			this.view.addChild(this.useItem);
			this.useItem.Id=this.equipData.id; 
			this.useItem.x=142;
			this.useItem.y=27;
			this.useItem.mouseEnabled=true;
			this.useItem.name="bagQuickKey_"+this.equipData.id;
			this.useItem.addEventListener(MouseEvent.CLICK,onEquipMouseClick);
			var limitLevel:uint=Math.max(UIConstData.getItem(this.equipData.type).Level,1);;
			var n:uint=Math.floor((limitLevel-1)/10);
			if(EquipDataConst.isFourthStilettoOpen)
			{
				if(equipData.stoneList[2] != 99999 && equipData.stoneList[3] == 99999)
				{
					n = 12;
				}
			}
			if(n != 12)
			{
				this.itemList.setSelectedIndex(n);
			}
			this.helpItemData=UIConstData.MarketGoodList[13][n];
			this.changeDes();
		}
		
		protected function onEquipMouseClick(e:MouseEvent):void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;
				this.equipData=null;
				this.helpItemBagData=null;
				sendNotification(EquipCommandList.RETURN_EQUIP_INFO);          //解除锁定状态
				EquipDataConst.getInstance().lockItems=new Dictionary();
			}
			this.changeDes();
		}
		
		
		/**
		 * 刷新显示  
		 * 
		 */		
		protected function changeDes():void{
			if(this.helpItem!=null && this.view.contains(this.helpItem)){
				this.view.removeChild(this.helpItem);
				this.helpItem=null;
			}
			if(this.useItem==null){
				this.view.txt_success.text="0%";
				helpItemBagData=null;
				if(this.view.contains(this.stilettoList)){
					this.view.removeChild(this.stilettoList);
				}
				(this.view.btn_commit as SimpleButton).visible=false;
				(this.view.btn_buyHelpItem as SimpleButton).visible=false;
				(this.view.txt_buyHelpItem as TextField).visible=false;
				
				sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");
				
			}else{
				(this.view.btn_commit as SimpleButton).visible=true;
				var arr:Array=IntroConst.ItemInfo[this.equipData.id].stoneList as Array;
				this.stilettoItems = [[]];
				var count:int=0;		
				if(arr[0]===99999){
					(this.stilettoItems[0] as Array).push({type:4});
				}else if(arr[0]==88888){
					count++;
					(this.stilettoItems[0] as Array).push({type:5});
				}else{
					count++;
					(this.stilettoItems[0] as Array).push({type:3,stoneType:arr[0]});
				}
				if(arr[1]===99999){
					(this.stilettoItems[0] as Array).push({type:4});
				}else if(arr[1]==88888){
					count++;
					(this.stilettoItems[0] as Array).push({type:5});
				}else{
					count++;
					(this.stilettoItems[0] as Array).push({type:3,stoneType:arr[1]});	
				}
				if(arr[2]===99999){
					(this.stilettoItems[0] as Array).push({type:4});
				}else if(arr[2]==88888){
					count++;
					(this.stilettoItems[0] as Array).push({type:5});
				}else{
					count++;
					(this.stilettoItems[0] as Array).push({type:3,stoneType:arr[2]});
				}
				if(EquipDataConst.isFourthStilettoOpen)//第四个孔，（type*10）+1，type为0时，type==10
				{
					var fourthData:Array = [];
					if(arr[3]===99999){
						fourthData.push({type:41});
					}else if(arr[3]==88888){
						count++;
						fourthData.push({type:51});
					}else{
						count++;
						fourthData.push({type:31,stoneType:arr[3]});
					}
					this.stilettoItems.push(fourthData);
				}
				this.view.addChild(this.stilettoList);
				this.stilettoList.dataPro=this.stilettoItems;
				 
				if(count==0){
//					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesByLevel(10));
					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(10));
					this.view.txt_success.text="100%";
				}else if(count==1){
//					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesByLevel(13));
					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(13));
					this.view.txt_success.text="50%";
				}else if(count==2){
//					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesByLevel(14));
					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(14));
					this.view.txt_success.text="25%";
				}
				else if(count==3){
					sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(15));
					this.view.txt_success.text="100%";
				}
				

				if(this.helpItemEnable || this.helpItemBagData==null){
					if(this.checkHelpItem(this.helpItemData.type)){
						this.helpItemEnable=false;
					}
					this.helpItem=new EnableItem(this.helpItemData.type,"icon");
					this.helpItem.name="goodQuickBuy_"+this.helpItemData.type;
					this.helpItem.x=142;
					this.helpItem.y=74;
					this.helpItem.addEventListener(MouseEvent.CLICK,onHelpItemMouseClick);
					this.view.addChild(this.helpItem);
					this.helpItem.setEnable(this.helpItemEnable);
				}else{
					this.helpItem=new EnableItem(this.helpItemBagData.type,"icon");
					this.helpItem.name="goodQuickBuy_"+this.helpItemBagData.type;
					this.helpItem.x=142;
					this.helpItem.y=74;
					this.helpItem.addEventListener(MouseEvent.CLICK,onHelpItemMouseClick);
					this.view.addChild(this.helpItem);
					this.helpItem.setEnable(this.helpItemEnable);
				}
				(this.view.btn_buyHelpItem as SimpleButton).visible=true;
				(this.view.txt_buyHelpItem as TextField).visible=true;
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
		}
		
		
		
		/**
		 * 检查背包中是否有相对应的打孔符(如果有直接上并返回true,否则返回False) 
		 * 
		 */		
		protected function checkHelpItem(type:uint):Boolean{
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			for(var i:uint=0;i<len;i++){
				if(arr[i] && arr[i].type==type){
					EquipDataConst.getInstance().lockItems[arr[i].id]=true;
					this.helpItemBagData=arr[i];
					return true;
				}
			}
			return false;
		}
		
		protected function onHelpItemMouseClick(e:MouseEvent):void{
			if(this.helpItemEnable==false){
				this.helpItemEnable=true;
				EquipDataConst.getInstance().lockItems=new Dictionary();
				this.changeDes();
			}
		}
		
		
		/**
		 * 切换面板（清除所有功能） 
		 * 
		 */		
		protected function clearAll():void{
			
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			UIUtils.removeFocusLis(this.view.txt_inputNum);
			this.view.txt_1.text=GameCommonData.wordDic[ "mod_equ_med_enc_ini" ];//"选择道具"
			this.selectedItemData=null;
			
			if(this.useItem!=null && this.view.contains(this.useItem)){
				this.view.removeChild(this.useItem);
			}
			this.useItem=null;
			if(this.helpItem!=null && this.view.contains(this.helpItem)){
				this.view.removeChild(this.helpItem);
			}
			this.helpItem=null;
			this.helpItemData=null;
			
			this.equipData=null;
			this.isBuyFlag=false;
			this.view.txt_success.text="0%";
			this.view.txt_success.mouseEnabled=false;		
		}
	}
}
