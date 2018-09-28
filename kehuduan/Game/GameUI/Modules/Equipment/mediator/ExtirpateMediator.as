package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Equipment.ui.EnableItem;
	import GameUI.Modules.Equipment.ui.NumberItem;
	import GameUI.Modules.Equipment.ui.StilettoCell;
	import GameUI.Modules.Equipment.ui.UIDataGrid;
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
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

	public class ExtirpateMediator extends Mediator
	{
		public static const NAME:String="ExtirpateMediator";
		
		protected var buyItem:NumberItem;
		protected var helpItem:EnableItem;
		protected var useItem:UseItem;
		protected var equipData:Object;
		protected var helpItemData:Object;
		protected var helpItenEnable:Boolean=true;
		
		protected var selectedObj:Object;
		
		protected var isBuyFlag:Boolean=false;
		/** 孔列表 */
		protected var stilettoList:UIDataGrid;
		/** 孔数据 */	
		protected var stilettoItems:Array=[[]];
		
		
		public function ExtirpateMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.view.addEventListener(Event.ADDED_TO_STAGE,onAddToStage);
			this.view.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveToStage);
		}
		
		protected function onAddToStage(e:Event):void{
			this.init();
		}
		
		protected function onRemoveToStage(e:Event):void{
			this.clearAll();
		}
		
		
		protected function init():void{
			(this.view.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyItemClick);
			(this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
			(this.view.btn_commit as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).mouseEnabled=false;
			(this.view.btn_buyHelpItem as SimpleButton).visible=false;
			(this.view.txt_buyHelpItem as TextField).visible=false;
			(this.view.txt_inputNum as TextField).restrict="0-9";
			(this.view.txt_inputNum as TextField).multiline=false;
			(this.view.txt_inputNum as TextField).mouseWheelEnabled=false;
			(this.view.txt_inputNum as TextField).maxChars=4;
			UIUtils.addFocusLis(this.view.txt_inputNum);
//			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesByLevel(12));
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getFeeDesMoney(12));
			this.helpItemData=null;
			this.helpItenEnable=true;
			this.selectedObj=null;
			
			if(this.helpItem!=null){
				if(this.view.contains(this.helpItem)){
					this.view.removeChild(this.helpItem);
				}
			}else{
				this.helpItem=new EnableItem("610019","icon");
				this.helpItem.x=142;
				this.helpItem.y=74;
				this.helpItem.mouseEnabled=true;
				this.helpItem.name="goodQuickBuy_610019";
				this.helpItem.setEnable(this.helpItenEnable);
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
			
			if(this.buyItem==null){
				this.buyItem=new NumberItem("610019","icon");
				this.view.addChild(this.buyItem);
				this.buyItem.x=258;
				this.buyItem.y=33;
				this.buyItem.mouseEnabled=true;
				this.buyItem.name="goodQuickBuy_610019";
			}
			
		}
		
		
		protected function onCommitClick(e:MouseEvent):void{
			if(this.equipData==null || this.useItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
				return;
			}
			
			if(this.helpItemData==null || this.helpItenEnable){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_ext_onC_1" ], color:0xffff00});//"没有摘取宝符"
				return;
			}
			
			if(this.isBagHasEmpty()==false){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_ext_onC_2" ], color:0xffff00});//"背包已经满了"
				return;
			}
			
			if(this.selectedObj==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_ext_onC_3" ], color:0xffff00});//"没有选择你要摘取的宝石"
				return ;	
			}	
			var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(money<5000){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_ext_onC_4" ], color:0xffff00});//"你的银两不足"
				return;
			}
					
			var param:Array=[1,this.helpItemData.id,70,this.equipData.id,this.selectedObj.index];
			EquipSend.createMsgCompound(param);
		}
		
		
		
		protected function onGridCellClick(e:GridCellEvent):void{
			for each(var item:* in this.stilettoItems){
				for each(var obj:* in item){
					if(obj.type==6){
						obj.type=7;
					}
					else if(obj.type==61){
						obj.type=71;
					}
				}
			}
			e.data.type=6;
			if(e.data.isFourthStiletto){	//第四个孔
				e.data.type=61;
			}
			this.selectedObj=e.data;
			this.stilettoList.refresh();
		}
		
		protected function onBuyItemClick(e:MouseEvent):void{
			var arr:Array=UIConstData.MarketGoodList[15] as Array;
			var num:uint=uint(this.view.txt_inputNum.text);
			if(num == 0){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyI_2" ], color:0xffff00});//"请输入有效的购买数"
				return;
			}
			var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+arr[0].PriceIn*num+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+num+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+arr[0].Name+'</font>';
			//花费	购买		个
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuy, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
		}
		
		protected function onSureToBuy():void{
			
			var num:uint=uint(this.view.txt_inputNum.text);
			this.isBuyFlag=true;
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:"610019",count:num});
		}
		
		
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		
		public override function listNotificationInterests():Array{
			return [
				EquipCommandList.ADD_EXTIRPATE_EQUIP,
				EquipCommandList.ADD_EXTIRPATE_ITEM,
				EquipCommandList.BUY_STRENGENHELP_ITEM,
				EquipCommandList.RECALL_EQUIPSTILETTO,
				EquipCommandList.REFRESH_EQUIP,
				EquipCommandList.DROP_EQUIP_ITEM
			];
		}
		
		/**
		 * 查看一下是否有宝石可以摘取
		 * 
		 */		
		protected function isHasStone(obj:Object):Boolean{
			var arr:Array=obj.stoneList as Array;
			for each(var o:* in arr){
				if(o>300000 && o<500000){
					return true;
				}
			}
			return false;
		}
		
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EquipCommandList.ADD_EXTIRPATE_EQUIP:
					if(this.isHasStone(notification.getBody())){
						this.equipData=notification.getBody();
						this.addEquip();
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_ext_han" ], color:0xffff00});//"没有宝石可以进行摘取"
						sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
					}
					break;
				case EquipCommandList.ADD_EXTIRPATE_ITEM:
					if(this.equipData==null){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
						return; 
					}
				
					
					if(this.helpItenEnable && this.helpItemData==null){
						this.helpItenEnable=false;
						this.helpItemData=notification.getBody();
						EquipDataConst.getInstance().lockItems[this.helpItemData.id]=true;
						this.changeDes();
					}
					break;	
				case EquipCommandList.BUY_STRENGENHELP_ITEM:
					if(this.view.stage==null)return;
					if(notification.getBody() && notification.getBody()==1){
						this.isBuyFlag=false;
					}else{
						if(this.isBuyFlag){
							this.isBuyFlag=false;
							this.lockItem(610019);
							sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
							this.helpItenEnable=false;
							this.changeDes();
						}
					}
					break;	
				case EquipCommandList.RECALL_EQUIPSTILETTO:
					if(notification.getBody()["type"]==71){
						sendNotification(EquipCommandList.ADD_SPECIAL_SHOW,new Point(165,56));
						ItemInfo.isLevelUp=true;
						UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
					}else if(notification.getBody()["type"]==72){
						this.helpItenEnable=true;
						this.changeDes();
					}
					break;
				case EquipCommandList.REFRESH_EQUIP:
					if(this.view.stage==null)return;
					this.equipData = IntroConst.ItemInfo[equipData.id];
					this.selectedObj=null;
					this.helpItenEnable=true;
					this.helpItemData=null;
					//todo继续上摘取符
					this.lockItem(610019);
					this.changeDes();
					break;
				//装备掉落	
				case EquipCommandList.DROP_EQUIP_ITEM:
					if(this.view.stage==null || this.equipData==null)return;
					var itemId:int = notification.getBody().id;
					if(this.equipData.id==itemId){
						this.clearAll();
					}
					else if(this.helpItemData && this.helpItemData.id==itemId){
						if(!lockItem(610019) && this.helpItem){
							/* if(this.view.contains(this.helpItem)){
								if(this.helpItem.hasEventListener(MouseEvent.CLICK)){
									this.helpItem.removeEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
								}
								this.view.removeChild(this.helpItem);
							} */
							this.helpItenEnable=true;
							this.helpItem.setEnable(this.helpItenEnable);
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
					EquipDataConst.getInstance().lockItems[arr[i].id]=true;
					this.helpItemData=arr[i];
					this.helpItenEnable=false;
					isHaveType = true;
					break;
				}
			}
			return isHaveType;
		}
		
		protected function addEquip():void{
			if(useItem!=null && this.view.contains(this.useItem)){
				this.view.removeChild(this.useItem);
				this.useItem=null;
				this.helpItenEnable=true;
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
			this.selectedObj=null;
			//todo自动上符	
			this.lockItem(610019);	
			this.changeDes();
		}
		
		protected function onEquipMouseClick(e:MouseEvent):void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;
				this.equipData=null;
				this.helpItemData=null;
				sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
				EquipDataConst.getInstance().lockItems=new Dictionary();
			}
			this.changeDes();
		}
		
		
		protected function clearAll():void{
			this.stilettoList.dataPro=[[]];
			this.stilettoList.removeEventListener(GridCellEvent.GRIDCELL_CLICK,onGridCellClick);
			UIUtils.removeFocusLis(this.view.txt_inputNum);
			this.stilettoList=null;
			this.helpItemData=null;
			this.helpItenEnable=true;
			this.equipData=null;
			
			if(this.useItem && this.view.contains(this.useItem)){
				this.view.removeChild(this.useItem);
				
			}
			this.useItem=null;
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");
		
		}
		
		protected function changeDes():void{
			if(this.useItem==null){
				(this.view.btn_buyHelpItem as SimpleButton).visible=false;
				(this.view.txt_buyHelpItem as TextField).visible=false;
				(this.view.btn_buyHelpItem as SimpleButton).removeEventListener(MouseEvent.CLICK,onBuyHelpItemHandler);
				if(this.view.contains(this.helpItem)){
					if(this.helpItem.hasEventListener(MouseEvent.CLICK)){
						this.helpItem.removeEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
					}
					this.view.removeChild(this.helpItem);
				}
				if(this.view.contains(this.stilettoList)){
					this.view.removeChild(this.stilettoList);
				}
				(this.view.btn_commit as SimpleButton).visible=false;
			}else{
				(this.view.btn_buyHelpItem as SimpleButton).visible=true;
				(this.view.btn_commit as SimpleButton).visible=true;
				(this.view.txt_buyHelpItem as TextField).visible=true;
				(this.view.btn_buyHelpItem as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyHelpItemHandler);
				this.view.addChild(this.helpItem);
				this.helpItem.setEnable(this.helpItenEnable);
				
				var arr:Array=IntroConst.ItemInfo[this.equipData.id].stoneList as Array;
				var flag:Boolean=false;
				this.stilettoItems=[[]];		
				if(arr[0]===99999){
					
				}else if(arr[0]==88888){
					(this.stilettoItems[0] as Array).push({type:5});
				}else{
					(this.stilettoItems[0] as Array).push({type:6,stoneType:arr[0],index:1});
					this.selectedObj={type:6,stoneType:arr[0],index:1};
					flag=true;
				}
				
				if(arr[1]===99999){
					
				}else if(arr[1]==88888){
					(this.stilettoItems[0] as Array).push({type:5});
				}else{
					if(flag){
						(this.stilettoItems[0] as Array).push({type:7,stoneType:arr[1],index:2});	
					}else{
						(this.stilettoItems[0] as Array).push({type:6,stoneType:arr[1],index:2});
						this.selectedObj={type:6,stoneType:arr[1],index:2};
						flag=true;
					}
				}
				
				if(arr[2]===99999){
					
				}else if(arr[2]==88888){
					(this.stilettoItems[0] as Array).push({type:5});
				}else{
					if(flag){
						(this.stilettoItems[0] as Array).push({type:7,stoneType:arr[2],index:3});
					}else{
						(this.stilettoItems[0] as Array).push({type:6,stoneType:arr[2],index:3});
						this.selectedObj={type:6,stoneType:arr[2],index:3};
						flag=true;
					}	
				}
				if(EquipDataConst.isFourthStilettoOpen)	//第四个孔，（type*10）+1，type为0时，type==10
				{
					var fourthData:Array = [];
					if(arr[3]===99999){
					
					}else if(arr[3]==88888){
						fourthData.push({type:51});
					}else{
						if(flag){
							fourthData.push({type:71,stoneType:arr[3],index:4});
						}else{
							fourthData.push({type:61,stoneType:arr[3],index:4});
							this.selectedObj={type:61,stoneType:arr[3],index:4};
							flag=true;
						}	
					}
					this.stilettoItems.push(fourthData);
				}
				
				this.view.addChild(this.stilettoList);
				this.stilettoList.dataPro=this.stilettoItems;
				
				if(this.helpItenEnable==false){
					this.helpItem.addEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
				}else{
					this.helpItem.removeEventListener(MouseEvent.CLICK,onMouseHelpItemClick);
				}
			} 
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			
		}
		
		protected function onMouseHelpItemClick(e:MouseEvent):void{
			
		}
		
		protected function onBuyHelpItemHandler(e:MouseEvent):void{
			var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">50</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">1</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'个<font color="#00ff00">'+GameCommonData.wordDic[ "mod_equ_med_ext_onBuyH" ]+'</font>';
			//花费		购买		摘取宝符
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
		}
		
		protected function onSureToBuyHelp():void
		{
			this.isBuyFlag=true;
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:"610019",count:1});

		}
		
		/**
		 *
		 *  判断背材料栏是不是有空格子
		 * @return 
		 * 
		 */		
		protected function isBagHasEmpty():Boolean{
			for(var i:uint=0;i<BagData.BagNum[1];i++){
				if(BagData.AllItems[1][i]==null){
					return true;
				}
			}
			return false;	
		}
		
		
	}
}
