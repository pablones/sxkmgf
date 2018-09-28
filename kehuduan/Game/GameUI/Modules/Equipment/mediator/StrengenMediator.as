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

	public class StrengenMediator extends Mediator
	{
		public static var NAME:String="StrengenMediator";
		protected var strengenList:UIList;
		protected var strengenNumList:UIList;
		/** 选择强化符数量数据  */
		protected var strengenNumData:Object={Name:"4",type:4};
		/** 装备图标  */
		protected var useItem:UseItem;
		/** 装备数据  */
		protected var equipData:Object;
		/** 强化符图标 */
		protected var strengenUseItem:UseItem;
		/** 选择强化符数据*/
		protected var strengenUseItemData:Object; 
		/** 强化等级数据显示 */
		protected var strengenLevel:NumberItem;
		/** 失败加成数  */
		private var failScale:uint;
		/** 辅助宝符图标  */
		protected var helpItem:EnableItem;
		/** 辅助宝符有没有上标记（true:未上 ，flase:已使用）  */
		protected var helpData:Boolean=true;
		/** 是否点击了购买 */
		protected var isBuyFlag:Boolean;
		/**装备当前的强化等级*/
		protected var currentLevel:uint;
		/**  服务端返回信息*/
		protected var isServerReturn:Boolean;
		public function StrengenMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.view.addEventListener(Event.ADDED_TO_STAGE,onAddtoStage);
			this.view.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStage);
		}
		
		protected function onAddtoStage(e:Event):void{
			this.init();
		}
		
		protected function onRemoveFromStage(e:Event):void{
			this.clearAll();	
		}
		
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		protected function init():void{
		
			if(this.strengenList==null){
				this.strengenList=new UIList(80,20);
				this.strengenList.rendererClass=ListCell;
				var arr:Array=UIConstData.MarketGoodList[16] as Array;	
				this.strengenList.dataPro=arr;
				this.strengenList.addEventListener(ListCellEvent.LISTCELL_CLICK,onListClick);
			}
			if(this.strengenNumList==null){
				this.strengenNumList=new UIList(40,20); 
				this.strengenNumList.rendererClass=ListCell;
				this.strengenNumList.addEventListener(ListCellEvent.LISTCELL_CLICK,onListClick);
			}
			(this.view.txt_4 as TextField).restrict="0-9";
			(this.view.txt_3 as TextField).restrict="0-9";
			(this.view.txt_4 as TextField).multiline=false;
			(this.view.txt_3 as TextField).multiline=false;
			(this.view.txt_4 as TextField).mouseWheelEnabled=false;
			(this.view.txt_3 as TextField).mouseWheelEnabled=false;
			(this.view.txt_4 as TextField).maxChars=4;
			(this.view.txt_3 as TextField).maxChars=4;
			UIUtils.addFocusLis(this.view.txt_4);
			UIUtils.addFocusLis(this.view.txt_3);
			(this.viewComponent.view.btn_1 as SimpleButton).addEventListener(MouseEvent.MOUSE_DOWN,onMouseClickHandler);
			(this.viewComponent.view.btn_2 as SimpleButton).addEventListener(MouseEvent.CLICK,onMouseClickHandler);
			(this.view.btn_buyItem as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyItemHandler);
			(this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitHandler);
			(this.view.btn_buyHelpItem as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyHelpItemClick);
			(this.view.txt_vipSucess as TextField).mouseEnabled=false;
			(this.view.txt_luckly as TextField).mouseEnabled=false;
			(this.view.txt_des as TextField).mouseEnabled=false;
			(this.view.btn_commit as SimpleButton).visible=false;
			(this.view.txt_vipSucess as TextField).mouseEnabled=false;
			(this.view.txt_luckly as TextField).mouseEnabled=false;
			(this.view.txt_success as TextField).mouseEnabled=false;
			if(this.helpItem==null){
				this.helpItem=new EnableItem("610048","Icon");
				this.helpItem.name="goodQuickBuy_610048";
				this.helpItem.addEventListener(MouseEvent.CLICK,onMouseEnableItemClick);
			}
			if(this.useItem==null){
				this.view.txt_success.text="0%";
				if(GameCommonData.Player.Role.VIP==1){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +0%";//VIP加成
				}else if(GameCommonData.Player.Role.VIP==2){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +1%";//VIP加成
				}else if(GameCommonData.Player.Role.VIP==3){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +2%";//VIP加成
				}else{
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_2" ]+" +0%";//非VIP
				}
				this.view.txt_luckly.text=GameCommonData.wordDic[ "mod_equ_med_add_init_3" ]+" 0";//幸运值
				this.view.txt_des.text=GameCommonData.wordDic[ "mod_equ_med_add_init_4" ];//"请放入装备"		
				this.showStrengenLevel("0");
				(view.txt_1 as TextField).mouseEnabled=false;
				(view.txt_2 as TextField).mouseEnabled=false;
				(this.view.mc_1 as MovieClip).visible=false;
				(this.view.txt_3 as TextField).visible=false;
				(this.view.txt_buy as TextField).visible=false;
				(this.view.txt_buy as TextField).mouseEnabled=false;
				(this.view.btn_buyHelpItem as SimpleButton).visible=false;
				(this.view.btn_buyHelpItem as SimpleButton).mouseEnabled=false;
				if(this.view.contains(this.helpItem))this.view.removeChild(this.helpItem);
				this.helpData=true;
				EquipDataConst.getInstance().lockItems=new Dictionary();
				this.isServerReturn=false;
				this.currentLevel=0;
			}
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");
		}
		
		protected function onMouseEnableItemClick(e:MouseEvent):void{
			if(this.helpData==false){
				this.helpData=true;
				EquipDataConst.getInstance().lockItems=new Dictionary();
				this.changeDes();
			}
		}
		
		protected function onBuyHelpItemClick(e:MouseEvent):void{
			/* var num:uint=uint(this.view.txt_3.text);
			if(num == 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请输入有效的购买数", color:0xffff00});
				return;
			}
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:"您是否确定购买数量为"+num+"的天运宝符? 此物品的单价为",title:"提 示"}); */
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyH" ], color:0xffff00});//"暂无此物品出售"
		}
		
		/* protected function onSureToBuyHelp():void
		{
			this.isBuyFlag=true;
			var num:uint=uint(this.view.txt_3.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:610048,count:num});
		} */
		
		/**
		 * buy item... 
		 * @param e
		 * 
		 */		
		protected function onBuyItemHandler(e:MouseEvent):void{
			if(this.strengenUseItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_str_onB" ], color:0xffff00});//"请选择要购买的物品"
			}else{
				var num:uint=uint(this.view.txt_4.text);
				if(num == 0)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyI_2" ], color:0xffff00});//"请输入有效的购买数"
					return;
				}
				var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+strengenUseItemData.PriceIn*num+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+num+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+strengenUseItemData.Name+'</font>';
				//花费		购买		个
				facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuy, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
			}
		}
		
		protected function onSureToBuy():void{
			var num:uint=uint(this.view.txt_4.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:strengenUseItemData.type,count:num});
		}
		
		
		protected function onListClick(e:ListCellEvent):void{
			if(e.target===this.strengenList){
				this.viewComponent.view.txt_1.text=e.data["Name"];
				if(this.strengenUseItem!=null){
					this.viewComponent.view.removeChild(this.strengenUseItem);
				}
				this.strengenUseItemData=e.data;
				this.strengenUseItem=new UseItem(0,e.data["type"],null);
				this.viewComponent.addChild(this.strengenUseItem);
				this.strengenUseItem.name="goodQuickBuy_"+e.data.type;
				this.strengenUseItem.mouseEnabled=true;
				this.strengenUseItem.x=258;
				this.strengenUseItem.y=33;
				this.viewComponent.view.addChild(this.strengenUseItem);
			}else if(e.target===this.strengenNumList){
				this.strengenNumData=e.data;
				this.viewComponent.view.txt_2.text=e.data["Name"];
				this.changeDes();
			}
		}
		
		/**
		 * 提交强化 
		 * @param e
		 * 
		 */		
		protected function onCommitHandler(e:MouseEvent):void{
			
			if(this.useItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_1" ], color:0xffff00});//"请先放入你强化的装备"
				return;
			}
			var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(money<EquipDataConst.getInstance().getStrengFee(this.equipData.level)){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//"你的银两不足"
				return;
			}
				
			var obj:Object=IntroConst.ItemInfo[this.equipData.id];
			if(obj.isBind==0 && this.isHasBindItem(obj.level)){
				facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:new Function(), info:GameCommonData.wordDic[ "mod_equ_med_str_onC_1" ],title:GameCommonData.wordDic[ "often_used_tip" ]});//"使用绑定的强化符，强化后装备将自动绑定"		"提 示"
			}else{
				if(!this.isServerReturn){
					this.isServerReturn=true;
					if(this.currentLevel==8 || this.currentLevel==9){
						facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToHighCommit, cancel:cancel, info:GameCommonData.wordDic[ "mod_equ_med_str_onC_2" ],title:GameCommonData.wordDic[ "often_used_tip" ]});//"如强化失败，强化等级将降为7级，是否继续强化操作"		"提 示"
					}else{
						beSureToHighCommit();
					}
						
				}else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_5" ], color:0xffff00});//"点击过快请稍后再试"
				}	
			}
		}
		
		protected function cancel():void{
			this.isServerReturn=false;
		}
		
		/**
		 * 提交 
		 * 
		 */		
		protected function beSureToHighCommit():void{
			var params:Array=[1,this.strengenNumData.type,62,this.equipData.id,0];
			if(this.helpData==false){
				facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToUseItem, cancel:cancelHelp, info:GameCommonData.wordDic[ "mod_equ_med_str_beS" ],title:GameCommonData.wordDic[ "often_used_tip" ],params:params});//"是否确定要使用天运宝符进行强化"	"提 示"
			}else{
				EquipSend.createMsgCompound(params,0);
			}
		}
		
		protected function cancelHelp():void
		{
			var params:Array=[1,this.strengenNumData.type,62,this.equipData.id,0];
			EquipSend.createMsgCompound(params,0);
		}
		
		/**
		 * 确定使用天运宝符进行强化 
		 * @param obj
		 * 
		 */		
		protected function beSureToUseItem(obj:Object):void{
			var params:Array=obj as Array;
			EquipSend.createMsgCompound(params,1);
		}
		
		/**
		 * 确定要提交强化 
		 * 
		 */		
		protected function beSureToCommit():void{
			if(!this.isServerReturn){
				this.isServerReturn=true;
				if(this.currentLevel==8 || this.currentLevel==9){
					facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToHighCommit, cancel:cancel, info:GameCommonData.wordDic[ "mod_equ_med_str_beSureToC" ],title:GameCommonData.wordDic[ "often_used_tip" ]});
					//"如强化失败，强化等级将降为7级，是否继续强化操作"		"提 示"
				}else{
					this.beSureToHighCommit();
				}
			}else{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_5" ], color:0xffff00});//"点击过快请稍后再试"
			}
		}
		/**
		 * 判断等级后提交强化
		 **/
		protected function beSureToCommitByLevel():void{
			currentLevel=IntroConst.ItemInfo[this.equipData.id].level;
			var params:Array=[1,this.strengenNumData.type,62,this.equipData.id,0];
			if(this.helpData==false){
				EquipSend.createMsgCompound(params,1);
			}else{
				EquipSend.createMsgCompound(params,0);
			}
		}
		
		/**
		 * 判断是否有相应的绑定的符 
		 * @param type
		 * 
		 */		
		protected function isHasBindItem(type:uint):Boolean{
			var itemType:uint=0;
			if(type>=0 && type<=2){
				itemType=610020;
			}else if(type>=3 && type<=5){
				itemType=610021;
			}else if(type>=6 && type<=9){
				itemType=610022;	
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
		
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				EquipCommandList.ADD_STRENGEN_EQUIP,
				EquipCommandList.REMOVE_STRENGEN_EQUIP,
				EquipCommandList.REFRESH_EQUIP,
				EquipCommandList.RECALL_EQUIPSTRENGEN,
				EquipCommandList.UPDATE_FAIL_SCALE,
				EquipCommandList.BUY_STRENGENHELP_ITEM,
				EquipCommandList.ADD_STRENGEN_ITEM,
				EquipCommandList.DROP_EQUIP_ITEM,
				EquipCommandList.SERVER_STRENGTH_UI
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			if(this.view.stage==null)return;
			switch(notification.getName()){
				case EquipCommandList.ADD_STRENGEN_EQUIP:
					this.equipData=notification.getBody();
					this.addEquip();
					break;
				case EquipCommandList.REMOVE_STRENGEN_EQUIP:
					
					break;	
				case EquipCommandList.REFRESH_EQUIP:
					this.changeDes();
					break;	
				//获得失败加成幸运值	
				case EquipCommandList.UPDATE_FAIL_SCALE:
					var num:uint=uint(notification.getBody());
					if(num%10==1){
						this.failScale=Math.floor(num/10)/10;
						this.changeDes();
					}
					break;	
				//回复强化后的信息（type:1为成功  2：为强化失败）
				case EquipCommandList.RECALL_EQUIPSTRENGEN:
					var type:uint=uint(notification.getBody()["type"]);
					if(type<1 || type>2)return;
					this.currentLevel=uint(notification.getBody()["level"]);
					if(type==1){
						sendNotification(EquipCommandList.ADD_SPECIAL_SHOW,new Point(166,84));
						 if(EquipDataConst.getInstance().isScaleClear(currentLevel-1)){
							this.failScale=0;
						 }
					}else if(type==2){
						this.failScale+=(this.strengenNumData.type);
					}
					ItemInfo.isLevelUp = true;
					UiNetAction.GetItemInfo(this.equipData.id,GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
					if(this.checkHelpItem()==false){
						this.helpData=true;
					}						
					this.changeDes();
					break;	
				//购买符返回消息 obj==1.证明购买失败	
				case EquipCommandList.BUY_STRENGENHELP_ITEM:
					if(notification.getBody() && notification.getBody()==1){
						this.isBuyFlag=false;
					}else{
						if(this.isBuyFlag){
							this.isBuyFlag=false;
							this.lockItem(610048);
							sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
							this.helpData=false;
							this.changeDes();
						}
					}
					break;	
					
				case EquipCommandList.ADD_STRENGEN_ITEM:
					var objHelpItem:Object=notification.getBody();
					if(this.helpData==true){
						if(EquipDataConst.getInstance().lockItems[objHelpItem.id]){
							var n:uint=EquipDataConst.getInstance().lockItems[objHelpItem.id].usedNum;
							n++;
							EquipDataConst.getInstance().lockItems[objHelpItem.id].usedNum=n;
						}else{
							EquipDataConst.getInstance().lockItems[objHelpItem.id]={usedNum:1};
						}
						sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
						this.helpData=false;
						this.changeDes();
					}else{
						
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_str_han" ], color:0xffff00});//"已经有相应的辅助宝符"
					}
					break;	
					
				case EquipCommandList.DROP_EQUIP_ITEM:	
					if(this.view.stage==null || this.equipData==null)return;
					if(this.equipData.id==notification.getBody().id){
						this.clearAll();
					}	
					break;
				case EquipCommandList.SERVER_STRENGTH_UI:
					this.isServerReturn=false;
					break;	
			}
		}
		
		/**
		 *  判断背包中是否有辅助符 
		 * @return 
		 * 
		 */		
		protected function checkHelpItem():Boolean{
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			for (var i:uint=0;i<len;i++){
				if(arr[i] && arr[i].type==610048){
					return true;
				}
			}
			return false;
		}
		
		/**
		 * 寻找某个物品。然后进行锁定（如果是叠加就减少他的数量） 
		 * @param type
		 * 
		 */		
		protected function lockItem(type:uint):void{
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			for (var i:uint=0;i<len;i++){
				if(arr[i] && arr[i].type==type){
					if(EquipDataConst.getInstance().lockItems[arr[i].id]){
						var n:uint=EquipDataConst.getInstance().lockItems[arr[i].id].usedNum;
						n++;
						EquipDataConst.getInstance().lockItems[arr[i].id].usedNum=n;
					}else{
						EquipDataConst.getInstance().lockItems[arr[i].id]={usedNum:1};
					}
					this.helpData=false;
					return;
				}
			}
		}
		
		
		/**
		 * 上装备 
		 * 
		 */		
		protected function addEquip():void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;
			}
			this.useItem=new UseItem(0,this.equipData.type,null);
			this.viewComponent.view.addChild(useItem);
			this.useItem.Id=this.equipData.id;
			this.useItem.x=145;
			this.useItem.y=57;
			this.useItem.mouseEnabled=true;
			this.useItem.name="bagQuickKey_"+this.equipData.id;
			this.useItem.addEventListener(MouseEvent.CLICK,onEquipMouseClick);
			this.showStrengenLevel(this.equipData.level);
			this.requestFailScale(this.useItem);
			this.currentLevel=this.equipData.level;
			this.isServerReturn=false;
			//todo;自动上辅助宝符
			this.autoUpItem();
				
		}
		
		
		/**
		 * 自动上辅助宝符 
		 * 
		 */
		protected function autoUpItem():void{
			if(this.checkHelpItem() && this.helpData==true){
				this.lockItem(610048);
			}
		}
		
		/**
		 * 点击装备图标 
		 * @param e
		 * 
		 */		
		protected function onEquipMouseClick(e:MouseEvent):void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;	
				this.equipData=null;
				sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
			}
			this.changeDes();
		}
		
		
		protected function onMouseClickHandler(e:MouseEvent):void{
			if(e.target===this.viewComponent.view.btn_1){	
				this.strengenList.x=242;
				this.strengenList.y=141;
				
				if(this.strengenList.parent!=null){
					this.strengenList.parent.removeChild(this.strengenList);
				}else{
					this.sendNotification(EquipCommandList.SHOW_COMBOX_LIST,this.strengenList);
				} 
			}else if(e.target===this.viewComponent.view.btn_2){
				this.strengenNumList.dataPro=[{Name:"1",type:1},{Name:"2",type:2},{Name:"3",type:3},{Name:"4",type:4}];
				this.strengenNumList.x=148;
				this.strengenNumList.y=260;
				if(this.strengenNumList.parent!=null){
					this.strengenNumList.parent.removeChild(this.strengenNumList);
				}else{
					this.sendNotification(EquipCommandList.SHOW_COMBOX_LIST,this.strengenNumList);
				} 
			}
			e.stopImmediatePropagation();			
		}
		
		
		protected function showStrengenLevel(level:String):void{
			
			if(this.strengenLevel!=null){
				this.view.removeChild(this.strengenLevel);
			}
			if(level=="clear"){
				this.strengenLevel=null;
				return;
			}
			this.strengenLevel=new NumberItem(level);
			this.view.addChild(this.strengenLevel);
			this.strengenLevel.x=128;
			this.strengenLevel.y=5;
			
		}
		
		
		/**
		 * 改变描述（成功率，VIP加成，失败加成率，失败后降与不降，初始化描述） 
		 * 
		 */		
		protected function changeDes():void{
			//todo没有装备
			if(this.helpItem==null)this.helpItem=new EnableItem("610048","Icon");
			
			if(this.useItem==null){
				this.view.txt_success.text="0%";
				if(GameCommonData.Player.Role.VIP==1){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +0%";//VIP加成
				}else if(GameCommonData.Player.Role.VIP==2){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +1%";//VIP加成
				}else if(GameCommonData.Player.Role.VIP==3){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +2%";//VIP加成
				}else{
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_2" ]+" +0%";//非VIP
				}
				this.view.txt_luckly.text=GameCommonData.wordDic[ "mod_equ_med_add_init_3" ]+" 0";//幸运值
				this.view.txt_des.text=GameCommonData.wordDic[ "mod_equ_med_add_init_4" ];//"请放入装备"	
				this.showStrengenLevel("clear");
				(this.view.mc_1 as MovieClip).visible=false;
				(this.view.txt_3 as TextField).visible=false;
				(this.view.txt_buy as TextField).visible=false;
				(this.view.btn_buyHelpItem as SimpleButton).visible=false;
				(this.view.btn_buyHelpItem as SimpleButton).mouseEnabled=false;
				if(this.view.contains(this.helpItem))this.view.removeChild(this.helpItem);
				this.helpData=true;
				(this.view.btn_commit as SimpleButton).visible=false;
				EquipDataConst.getInstance().lockItems=new Dictionary();
				sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");
			}else{
				(this.view.btn_commit as SimpleButton).visible=true;
				this.equipData=IntroConst.ItemInfo[this.equipData.id];
				if(this.helpData==false){
					var number:uint=Math.min(uint(EquipDataConst.getInstance().getSuccessByStoreLevel(this.equipData.level,this.strengenNumData.type)+10),100);
					this.view.txt_success.text=number+"%";
				}else{
					var num:Number=EquipDataConst.getInstance().getSuccessByStoreLevel(this.equipData.level,this.strengenNumData.type);
					if(num<1){
						this.view.txt_success.text=num.toFixed(1)+"%";
					}else{
						this.view.txt_success.text=num+"%";
					}
				}
				if(GameCommonData.Player.Role.VIP==1){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +0%";//VIP加成
				}else if(GameCommonData.Player.Role.VIP==2){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +1%";//VIP加成
				}else if(GameCommonData.Player.Role.VIP==3){
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +2%";//VIP加成
				}else{
					this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_2" ]+" +0%";//非VIP
				}
				this.view.txt_luckly.text=GameCommonData.wordDic[ "mod_equ_med_add_init_3" ]+" "+this.failScale;//幸运值 
				this.showStrengenLevel(IntroConst.ItemInfo[this.equipData.id].level);	
				(this.view.mc_1 as MovieClip).visible=true;
				(this.view.txt_3 as TextField).visible=true;
				(this.view.txt_buy as TextField).visible=true;
				(this.view.btn_buyHelpItem as SimpleButton).visible=true;
				(this.view.btn_buyHelpItem as SimpleButton).mouseEnabled=true;
				this.view.addChild(this.helpItem);
				this.helpItem.x=145;
				this.helpItem.y=119;
				this.helpItem.setEnable(this.helpData);	
				var equipLevel:uint=IntroConst.ItemInfo[this.equipData.id].level;
				if(EquipDataConst.getInstance().isToReset(equipLevel)){
					if(equipLevel==5 || equipLevel==6){
						(this.view.txt_des as TextField).htmlText='<font color="#ffff00">'+GameCommonData.wordDic[ "mod_equ_med_str_cha_1" ]+'</font><font color="#00ff00">+4</font>';//失败强化等级降为
					}else if(equipLevel==8 || equipLevel==9) {
						(this.view.txt_des as TextField).htmlText='<font color="#ffff00">'+GameCommonData.wordDic[ "mod_equ_med_str_cha_1" ]+'</font><font color="#00ff00">+7</font>';//失败强化等级降为
					}
				}else{
					(this.view.txt_des as TextField).htmlText='<font color="#ffff00">'+GameCommonData.wordDic[ "mod_equ_med_str_cha_1" ]+'</font><font color="#00ff00">'+GameCommonData.wordDic[ "mod_equ_med_add_cha_6" ]+'</font>';//失败强化等级	不降
				}
				this.checkBuyItem();   //改变购买的物品  
//				sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getStrengFeeByLevel(this.equipData.level));				
				sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,EquipDataConst.getInstance().getStrengFee(this.equipData.level));				
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			
		}
		
		
		/**
		 * 自动寻找适合装备等级的符 
		 * 
		 */		
		protected function checkBuyItem():void{
			var equipLevel:uint=IntroConst.ItemInfo[this.equipData.id].level;
			if(equipLevel<=2){
				//todo1级强化符
				strengenList.setSelectedIndex(0);
			}else if(equipLevel>=3 && equipLevel<=5){
				//todo2级强化符
				strengenList.setSelectedIndex(1);
			}else{
				//todo3级强化符
				strengenList.setSelectedIndex(2);
			}
			
			
		}
		
		/**
		 * 所有都重新清除,得新设置（如：切换面板）
		 * 
		 */		
		public function clearAll():void{
			if(this.useItem!=null){
				this.view.removeChild(this.useItem);
				this.useItem.removeEventListener(MouseEvent.CLICK,onEquipMouseClick);
				this.useItem=null;	
			}
			
			UIUtils.removeFocusLis(this.view.txt_4);
			UIUtils.removeFocusLis(this.view.txt_3);
			this.equipData=null;
			this.view.txt_success.text="0%";
			if(GameCommonData.Player.Role.VIP==1){
				this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +0%";//VIP加成
			}else if(GameCommonData.Player.Role.VIP==2){
				this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +1%";//VIP加成
			}else if(GameCommonData.Player.Role.VIP==3){
				this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_1" ]+" +2%";//VIP加成
			}else{
				this.view.txt_vipSucess.text=GameCommonData.wordDic[ "mod_equ_med_add_init_2" ]+" +0%";//非VIP
			}
			this.view.txt_luckly.text=GameCommonData.wordDic[ "mod_equ_med_add_init_3" ]+" 0";//幸运值
			this.view.txt_des.text=GameCommonData.wordDic[ "mod_equ_med_add_init_4" ];//"请放入装备";		
			this.showStrengenLevel("clear");
			(this.view.mc_1 as MovieClip).visible=false;
			(this.view.txt_3 as TextField).visible=false;
			(this.view.txt_buy as TextField).visible=false;
			(this.view.txt_buy as TextField).mouseEnabled=false;
			(this.view.btn_buyHelpItem as SimpleButton).visible=false;
			(this.view.btn_buyHelpItem as SimpleButton).mouseEnabled=false;
			(this.view.btn_commit as SimpleButton).visible=false;
			if(this.view.contains(this.helpItem))this.view.removeChild(this.helpItem);
			this.helpData=true;
			EquipDataConst.getInstance().lockItems=new Dictionary();
		}
		
		/**
		 * 申请失败加成数 
		 * @param u :装备数  
		 * 
		 */			
		protected function requestFailScale(u:UseItem):void{
			var params:Array=[1,1,68,u.Id,1];
			EquipSend.createMsgCompound(params);
		}
		
		
		
	}
}
