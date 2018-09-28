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
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
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

	public class EnchaseMediator extends Mediator
	{
		public static var NAME:String="ENCHASEMEDIATOR";
		
		/** 购买镶嵌列表*/
		protected var itemList:UIList;
		/** 购买物品数据*/
		protected var selectedItemData:Object;
		/** 购买图标 */
		protected var buyItem:NumberItem;
		
		protected var helpItem:EnableItem;
		protected var helpItemData:Object;
		protected var helpItemEnable:Boolean=true;		
		/** 装备数据 */
		protected var equipData:Object;
		/** 装备图标 */
		protected var useItem:UseItem;
		/** 购买标记 */
		protected var isBuyFlag:Boolean;
		/** 孔列表 */
		protected var stilettoList:UIDataGrid;
		/** 孔数据 */
		protected var stilettoItems:Array=[[]];
		
		public function EnchaseMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.view.addEventListener(Event.ADDED_TO_STAGE,onAddToStage);
			this.view.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStage);
		}
		
		protected function onAddToStage(e:Event):void{
			this.init();
		}
		
		protected function onRemoveFromStage(e:Event):void{
			this.clearAll();
		}
		
		
		protected function init():void{
			(this.view.btn_1 as SimpleButton).addEventListener(MouseEvent.MOUSE_DOWN,onBuyListBtnClick);
			(this.view.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyItemClick);
		    (this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
		    (this.view.btn_commit as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).mouseEnabled=false;
			(this.view.btn_buyHelpItem as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).visible=false;
			this.helpItemData=null;
			this.helpItemEnable=true;
			(this.view.txt_1 as TextField).mouseEnabled=false;
			(this.view.txt_inputNum as TextField).restrict="0-9";
			(this.view.txt_inputNum as TextField).multiline=false;
			(this.view.txt_inputNum as TextField).mouseWheelEnabled=false;
			(this.view.txt_inputNum as TextField).maxChars=4;
			UIUtils.addFocusLis(this.view.txt_inputNum);
//			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesByLevel(11));
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(11));
			
			this.view.txt_1.text=GameCommonData.wordDic[ "mod_equ_med_enc_ini" ];//"选择道具"
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			this.selectedItemData=null;
			
			if(this.itemList==null){
				this.itemList=new UIList(80,18,6);
				this.itemList.rendererClass=ListCell;
				this.itemList.addEventListener(ListCellEvent.LISTCELL_CLICK,onListCellClick);
			}
			
			if(this.helpItem!=null){
				if(this.view.contains(this.helpItem)){
					this.view.removeChild(this.helpItem);
				}	
			}else{
				this.helpItem=new EnableItem("610018","icon");
				this.helpItem.name="goodQuickBuy_610018";
				this.helpItem.x=142;
				this.helpItem.y=74;
				this.helpItem.setEnable(this.helpItemEnable);
			
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
				this.stilettoList.x=76;
				this.stilettoList.y=117;
				this.stilettoList.addEventListener(GridCellEvent.GRIDCELL_CLICK,onGridCellClick);
			}else if(this.view.contains(this.stilettoList)){
				this.view.removeChild(this.stilettoList);
			}	
			this.stoneData=null;						
		}
		
		/**
		 * 提交镶嵌 
		 * @param e
		 * 
		 */		
		protected function onCommitClick(e:MouseEvent):void{
			if(this.helpItemData==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_1" ], color:0xffff00});//"没有镶嵌宝符"
				return;
			}
			if(this.useItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
				return;
			}
			if(!this.isEmpty(this.equipData)){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_han_1" ], color:0xffff00});//"当前没有可以镶嵌宝石的孔"
				return;
			}
			if(this.stoneData==null){
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_3" ], color:0xffff00});//"请先放入你的定石"
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先放入你的宝石", color:0xffff00});//"请先放入你的定石"
				return;
			}
			var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(money<5000){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//你的银两不足
				return;
			}
			
			var param:Array=[1,this.helpItemData.id,6,this.equipData.id,this.stoneData.id];
			EquipSend.createMsgCompound(param);
			
				
		}
		
		/**
		 * 点击孔中的宝石 
		 * @param e
		 * 
		 */		
		protected function onGridCellClick(e:GridCellEvent):void{
			if(this.stoneData!=null){
				EquipDataConst.getInstance().lockItems[stoneData.id]=false;
			}
			this.stoneData=null;
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			this.changeDes();
		}
		
		protected function onBuyItemClick(e:MouseEvent):void{
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
		
		protected function onSureToBuy():void{
			var num:uint=uint(this.view.txt_inputNum.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:this.selectedItemData.type,count:num});	
		}
		
		
		
		protected function onListCellClick(e:ListCellEvent):void{
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
		
		protected function onBuyListBtnClick(e:MouseEvent):void{
			e.stopPropagation();
			var arr:Array=UIConstData.MarketGoodList[14] as Array;
			this.itemList.dataPro=arr;
			this.itemList.x=242;
			this.itemList.y=141;
			if(this.itemList.parent!=null){
				this.itemList.parent.removeChild(this.itemList);
			}else{
				sendNotification(EquipCommandList.SHOW_COMBOX_LIST,this.itemList);
			}
		}
		
		
		/**
		 * 切换面板需要全部清除的东西 
		 * 
		 */		
		protected function clearAll():void{
			(this.view.btn_commit as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).mouseEnabled=false;
			(this.view.btn_buyHelpItem as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).visible=false;
			UIUtils.removeFocusLis(this.view.txt_inputNum);
			this.helpItemData=null;
			this.helpItemEnable=true;
			this.stoneData=null;
			this.equipData=null;
			this.isBuyFlag=false;
				
//			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesByLevel(11));
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(11));
			
			
			if(this.helpItem!=null){
				if(this.view.contains(this.helpItem)){
					this.view.removeChild(this.helpItem);
					this.helpItem=null;
				}	
			}
			
			if(this.useItem && this.useItem.contains(this.useItem)){
				this.view.removeChild(this.useItem);
				this.useItem=null;
			}

			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
				this.buyItem=null;
			}
			if(this.view.contains(this.stilettoList)){
				this.view.removeChild(this.stilettoList);
			}							
		}
		
		
		
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		
		public override function listNotificationInterests():Array{
			return [EquipCommandList.ADD_ENCHASE_EQUIP,
					EquipCommandList.BUY_STRENGENHELP_ITEM,
					EquipCommandList.ADD_ENCHASE_STONE,
					EquipCommandList.ADD_ENCHASE_ITEM,
					EquipCommandList.RECALL_EQUIPSTILETTO,
					EquipCommandList.REFRESH_EQUIP,
					EquipCommandList.DROP_EQUIP_ITEM
			];
		}
		
		/**
		 * 是否还有空位置进行开孔 
		 * @param obj
		 * 
		 */		
		protected function isEmpty(obj:Object):Boolean{
			var arr:Array=obj.stoneList as Array;
			for each(var o:* in arr){
				if(o==88888){
					return true;
				}
			}
			return false;
		}
		
		/**
		 * 挑选宝石列表 
		 * 
		 */		
		protected function checkStoneList():void{
			
		}
		
		
		public override function handleNotification(notification:INotification):void{
			if(this.view.stage==null)return;
			switch (notification.getName()){
				case EquipCommandList.ADD_ENCHASE_EQUIP:
					if(this.isEmpty(notification.getBody())){
						this.equipData=notification.getBody();
						this.stoneData=null;
						this.addEquip();
						
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_han_1" ], color:0xffff00});//"当前没有可以镶嵌宝石的孔"
						sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
					}
					break; 
				case EquipCommandList.BUY_STRENGENHELP_ITEM:
					if(this.view.stage==null)return;
					if(notification.getBody() && notification.getBody()==1){
						this.isBuyFlag=false;
					}else{
						if(this.isBuyFlag){
							this.isBuyFlag=false;
							this.lockItem(610018);
							
							if(this.stoneData!=null){
								EquipDataConst.getInstance().lockItems[stoneData.id]=false;
								this.stoneData=null;
							}
							sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
							this.helpItemEnable=false;
							this.changeDes();
						}	
					}
					break;
				//添加宝石
				case EquipCommandList.ADD_ENCHASE_STONE:
					if(this.useItem==null){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
						return;
					}else{
						//todo 检查一下宝石是否符合
						if(this.stoneData!=null){
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_han_2" ], color:0xffff00});//"一次只能镶嵌一颗宝石"
							return;
						}
						this.addStone(notification.getBody());
					}
					
					break;
				//添加宝符	
				case EquipCommandList.ADD_ENCHASE_ITEM:
					if(this.useItem==null){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
						return;
					}
					if(this.helpItemEnable){
						this.helpItemEnable=false;
						this.helpItemData=notification.getBody();
						EquipDataConst.getInstance().lockItems[this.helpItemData.id]=true;
						sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
						this.view.addChild(this.helpItem);
						this.helpItem.addEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
						this.helpItem.setEnable(this.helpItemEnable);
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_han_3" ], color:0xffff00});//"已经有镶嵌宝符了"
					}
					break;	
				case EquipCommandList.RECALL_EQUIPSTILETTO:
					if(notification.getBody()["type"]==9){ //镶嵌成功
						sendNotification(EquipCommandList.ADD_SPECIAL_SHOW,new Point(165,56));
						ItemInfo.isLevelUp=true;
						UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
						EquipDataConst.getInstance().lockItems=new Dictionary();
						
					}else if(notification.getBody()["type"]==10){  //镶嵌失败
						this.helpItemEnable=true;
						this.helpItemData=null;
						this.changeDes();
					}
					break;	
				case EquipCommandList.REFRESH_EQUIP:
					if(this.view.stage==null)return;
					equipData = IntroConst.ItemInfo[equipData.id];
					this.stoneData=null;
					this.helpItemData=null;
					this.helpItemEnable=true;
					
					if(this.autoUpItem()){
						this.helpItemEnable=false;
					}
					
					this.changeDes();
					break;	
				case EquipCommandList.DROP_EQUIP_ITEM:
					if(this.view.stage==null || this.equipData==null)return;
					var itemId:int = notification.getBody().id;
					if(this.equipData.id==itemId){
						this.clearAll();
					}
					else if(this.helpItemData && this.helpItemData.id==itemId){
						if(!lockItem(610018) && this.helpItem){
							this.helpItemEnable=true;
							this.helpItem.setEnable(this.helpItemEnable);
							this.helpItem.mouseEnabled=false;
						}
					}
					break;	
			}
		}
		
		protected var stoneData:Object;
		
		
		/**
		 * 添加宝石 
		 * @param obj
		 * 
		 */		
		protected function addStone(obj:Object):void{
			var isFourthStiletto:Boolean;
			var stoneArr:Array = equipData.stoneList;
			if(EquipDataConst.isFourthStilettoOpen){
				if(stoneArr[0] != 88888 && stoneArr[1] != 88888 && stoneArr[2] != 88888 && stoneArr[3] == 88888){
					isFourthStiletto = true;
				}
			}
			if(EquipDataConst.getInstance().isRightS(this.useItem,obj.type,isFourthStiletto)){
				this.stoneData=obj;
				end:for each(var arr:* in this.stilettoItems){
					for each(var item:* in arr){
						if(item.type==1 || item.type==11){
							item.stoneType=obj.type;
							item.type=2;
							if(item.isFourthStiletto)
							{
								item.type = 21;  
							}
							if(this.stoneData!=null){
								EquipDataConst.getInstance().lockItems[stoneData.id]=false;
							}
							EquipDataConst.getInstance().lockItems[obj.id]=true;
							sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
							break end;
						}
					}	
				}	
				this.stilettoList.refresh();
			}else{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_addS" ], color:0xffff00});//"你放入的宝石类型不符"
			}
			
		}
		
		protected function lockItem(type:uint):Boolean{
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			var isHaveType:Boolean;
			for (var i:uint=0;i<len;i++){
				if(arr[i] && arr[i].type==type){
					EquipDataConst.getInstance().lockItems[arr[i].id]=true;
					this.helpItemData=arr[i];
					isHaveType = true;
					break;
				}
			}
			return isHaveType;
		}
		
		
		protected function addEquip():void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;
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
			if(this.helpItemEnable){
				this.autoUpItem();			
			}
			this.changeDes();
		}
		
		/**
		 *  点击装备取下装备
		 * @param e
		 * 
		 */		
		protected function onEquipMouseClick(e:MouseEvent):void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;
				this.equipData=null;
				this.helpItemData=null;
				sendNotification(EquipCommandList.RETURN_EQUIP_INFO);  //刷新装备信息
				EquipDataConst.getInstance().lockItems=new Dictionary();
			}
			this.changeDes();
		
		}
		
		 
		/**
		 * 改变显示  
		 * 
		 */		
		protected function changeDes():void{
			//没有装备
			if(this.useItem==null){
				this.helpItemEnable=true;
				this.helpItemData=null;
				(this.view.btn_buyHelpItem as SimpleButton).visible=false;
				(this.view.txt_buyHelpItem as TextField).visible=false;
				(this.view.btn_buyHelpItem as SimpleButton).removeEventListener(MouseEvent.CLICK,onBuyHelpItemHandler);
				if(this.helpItem && this.view.contains(this.helpItem)){
					this.view.removeChild(this.helpItem);
				}
				
				if(this.view.contains(this.stilettoList)){
					this.view.removeChild(this.stilettoList);
				}
				(this.view.btn_commit as SimpleButton).visible=false;
				
  			//有装备	
			}else{
				(this.view.btn_buyHelpItem as SimpleButton).visible=true;
				(this.view.txt_buyHelpItem as TextField).visible=true;
				(this.view.btn_buyHelpItem as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyHelpItemHandler);
				(this.view.btn_commit as SimpleButton).visible=true;
				this.view.addChild(this.helpItem);
				this.helpItem.setEnable(this.helpItemEnable);
				
				var arr:Array=IntroConst.ItemInfo[this.equipData.id].stoneList as Array;
				this.stilettoItems=[[]];		
				if(arr[0]===99999){
					(this.stilettoItems[0] as Array).push({type:0});
				}else if(arr[0]==88888){
					(this.stilettoItems[0] as Array).push({type:1});
				}else{
					(this.stilettoItems[0] as Array).push({type:3,stoneType:arr[0]});
				}
				if(arr[1]===99999){
					(this.stilettoItems[0] as Array).push({type:0});
				}else if(arr[1]==88888){
					(this.stilettoItems[0] as Array).push({type:1});
				}else{
					(this.stilettoItems[0] as Array).push({type:3,stoneType:arr[1]});	
				}
				if(arr[2]===99999){
					(this.stilettoItems[0] as Array).push({type:0});
				}else if(arr[2]==88888){
					(this.stilettoItems[0] as Array).push({type:1});
				}else{
					(this.stilettoItems[0] as Array).push({type:3,stoneType:arr[2]});
				}
				if(EquipDataConst.isFourthStilettoOpen)//第四个孔，（type*10）+1，type为0时，type==10
				{
					var fourthData:Array = [];
					if(arr[3]===99999){
						fourthData.push({type:10});
					}else if(arr[3]==88888){
						fourthData.push({type:11});
					}else{
						fourthData.push({type:31,stoneType:arr[3]});
					}
					this.stilettoItems.push(fourthData);
				}
				this.view.addChild(this.stilettoList);
				
				if(this.stoneData){
					for each(var item:* in this.stilettoItems){
						for each(var o:* in item){
							if(o.type==1){
								o.stoneType=stoneData.type;
								o.type=2;
								if(this.stoneData!=null){
									EquipDataConst.getInstance().lockItems[stoneData.id]=false;
								}
								EquipDataConst.getInstance().lockItems[stoneData.id]=true;
								break;
							}
						}	
					}	
				}
				this.stilettoList.dataPro=this.stilettoItems;
				if(this.helpItemEnable==false){
					this.helpItem.addEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
				}else{
					this.helpItem.removeEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
				}	
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);	
		}
		
		
		/**
		 * 自动上镶嵌宝符
		 *  
		 * 
		 */		
		protected function autoUpItem():Boolean{
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			for (var i:uint=0;i<len;i++){
				if(arr[i] && arr[i].type==610018){
					EquipDataConst.getInstance().lockItems[arr[i].id]=true;
					this.helpItemData=arr[i];
					this.helpItemEnable=false;
					return true;
				}
			}
			return false;
		}
		
		
		/**
		 * 点击辅助符，取消使用镶嵌符 
		 * @param e
		 * 
		 */		
		protected function onMouseHelpItemClick(e:MouseEvent):void{
			this.helpItemEnable=true;
			EquipDataConst.getInstance().lockItems[this.helpItemData.id]=false;
			if(this.stoneData!=null){
				EquipDataConst.getInstance().lockItems[stoneData.id]=false;
				this.stoneData=null;
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			this.helpItemData=null;
			this.changeDes();
		}
		
		
		protected function onBuyHelpItemHandler(e:MouseEvent):void{
			//todo发出购买信息
			var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">25</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">1</font>个<font color="#00ff00">'+GameCommonData.wordDic[ "mod_equ_med_enc_onB" ]+'</font>';
			//花费		购买		镶嵌宝符
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
		}	
		
		protected function onSureToBuyHelp():void
		{
			this.isBuyFlag=true;
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:"610018",count:1});
		}	
	}
}
