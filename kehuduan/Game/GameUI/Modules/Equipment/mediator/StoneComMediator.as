package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
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
	import GameUI.UIUtils;
	
	import Net.ActionSend.EquipSend;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StoneComMediator extends Mediator
	{
		public static const NAME:String="StoneCompose";
		
		protected var buyItem:NumberItem;
		protected var selectedItemData:Object;
		protected var itemList:UIList;
		
		protected var helpItem:EnableItem;
		protected var helpItemData:Object;
		protected var helpItemEnable:Boolean=true;
		
		protected var stoneArr:Array=[false,false,false,false,false];
		protected var cells:Array=[];
		protected var stoneType:uint;
		
		/** 购买标记 */
		protected var isBuyFlag:Boolean;
		
		public function StoneComMediator(mediatorName:String=null, viewComponent:Object=null)
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
			if(this.buyItem && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
				this.buyItem=null;
			}
			if(this.itemList==null){
				this.itemList=new UIList(80,18,6);
				this.itemList.rendererClass=ListCell;
				this.itemList.addEventListener(ListCellEvent.LISTCELL_CLICK,onListClick);
			}
			this.stoneArr=[false,false,false,false,false];
		
			this.selectedItemData=null;
			this.view.txt_1.text=GameCommonData.wordDic[ "mod_equ_med_sto_ini" ];//"选择宝石"
			(this.view.txt_1 as TextField).mouseEnabled=false;
			this.view.txt_success.text="0%";
			this.view.txt_success.mouseEnabled=false;
			(this.view.txt_inputNum as TextField).restrict="0-9";
			(this.view.txt_inputNum as TextField).maxChars=4; 
			UIUtils.addFocusLis(this.view.txt_inputNum);
			this.stoneType=0;
			if(this.helpItem && this.view.contains(this.helpItem)){
				this.view.removeChild(this.helpItem);
			}
			this.helpItemData=null;
		    this.helpItemEnable=true;
		    
			(this.view.btn_1 as SimpleButton).addEventListener(MouseEvent.MOUSE_DOWN,onBuyItemClick);
			(this.view.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyClick);
			(this.view.btn_buyHelpItem as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyHelpItemClick);
			(this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
			(this.view.btn_commit as SimpleButton).visible=false;
			
			EquipDataConst.getInstance().lockItems=new Dictionary();
//			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"50\\ss");		
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"5000");		
		}
		
		
		protected function onListClick(e:ListCellEvent):void{
			this.view.txt_1.text=e.data["Name"];
			if(this.buyItem!=null && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			
			this.buyItem=new NumberItem(e.data.type,"icon");
			this.buyItem.name="goodQuickBuy_"+e.data.type;
			this.view.addChild(this.buyItem);
			this.buyItem.x=256;
			this.buyItem.y=33;
			this.selectedItemData=e.data;
		}
		
		
		protected function onBuyItemClick(e:MouseEvent):void{
			e.stopPropagation();
			var arr:Array=UIConstData.MarketGoodList[12] as Array;
			this.itemList.dataPro=arr;
			this.itemList.x=242;
			this.itemList.y=141;
			if(this.itemList.parent!=null){
				this.itemList.parent.removeChild(this.itemList);
			}else{
				sendNotification(EquipCommandList.SHOW_COMBOX_LIST,this.itemList);
			}
		}
		
		protected function onBuyClick(e:MouseEvent):void{
			if(this.buyItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sto_onBuyI" ], color:0xffff00});//"请选择你要购买的宝石"
			}else{
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
		
		protected function onSureToBuy():void
		{
			var num:uint=uint(this.view.txt_inputNum.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:this.selectedItemData.type,count:num});	
		}
		
		/**
		 * 购买宝石合成符(快速购买) 
		 * @param e
		 * 
		 */		
		protected function onBuyHelpItemClick(e:MouseEvent):void{
			if(this.stoneType==0){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sto_onBuyH_1" ], color:0xffff00});//"请先放入你要合成的宝石"
			}else{
				this.isBuyFlag=true;
				if(this.helpItemData){
					var priceObj:Object = {}
					if(helpItemData.type == 610016){
						priceObj.price = 10;
						priceObj.name = GameCommonData.wordDic[ "mod_equ_med_sto_onBuyH_2" ];//"低级宝石合成符"
					}
					else if(helpItemData.type == 610017){
						priceObj.price = 98;
						priceObj.name = GameCommonData.wordDic[ "mod_equ_med_sto_onBuyH_3" ];//"高级宝石合成符"
					}
					priceObj.type = helpItemData.type;
					var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+priceObj.price+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+1+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+priceObj.name+'</font>';
					//花费		购买		个
					facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ],params:priceObj});//"提 示"
				
				}else{
					var type:String=this.helpItem.name.split("_")[1];
					var priceObj2:Object = {};
					if(type == "610016"){
						priceObj2.price = 10;
						priceObj2.name = GameCommonData.wordDic[ "mod_equ_med_sto_onBuyH_2" ];//"低级宝石合成符"
					}
					else if(type == "610017"){
						priceObj2.price = 98;
						priceObj2.name = GameCommonData.wordDic[ "mod_equ_med_sto_onBuyH_3" ];//"高级宝石合成符"
					}
					priceObj2.type = type;
					var strInfo2:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+priceObj2.price+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+1+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+priceObj2.name+'</font>';
					//花费		购买		个
					facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuyHelp, cancel:new Function(), info:strInfo2,title:GameCommonData.wordDic[ "often_used_tip" ],params:priceObj2});//"提 示"

				}
				
			} 
						
		}
		
		protected function onSureToBuyHelp(obj:Object):void{
			this.isBuyFlag=true;
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:obj.type,count:1});
		}
		
		/**
		 * 提交  
		 * @param e
		 * 
		 */		
		protected function onCommitClick(e:MouseEvent):void{
			
			var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(money<5000){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ], color:0xffff00});//"你的银两不足"
				return;
			}
			if(stoneType%10 == 0){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sto_onCom" ], color:0xffff00});//"物品已经是最高等级，不用再合成"
				return;
			}
			if(BagData.bagIsFull(stoneType+1)){
				return;
			}
			if(this.isHasBindItem()){
				facade.sendNotification(EventList.SHOWALERT, {comfrim:beCommit, cancel:new Function(), info:GameCommonData.wordDic[ "mod_equ_med_sto_onC" ],title:GameCommonData.wordDic[ "often_used_tip" ]});//"使用绑定的道具，合成后宝石将自动变为绑定"		"提 示"
			}else{
				this.beCommit();
			}	
		}
	
		/**
		 * 确定提交 
		 * 
		 */		
		protected function beCommit():void{
			if(this.helpItemData){
				this.beSureToCommit();
			}else{
				facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:new Function(), info:GameCommonData.wordDic[ "mod_equ_med_sto_beC" ],title:GameCommonData.wordDic[ "often_used_tip" ]});//"你没有使用宝石合成符，合成有可能失败，是否继续？"		"提 示"
			}
		}
		
		/**
		 * 确定提交时要执行的提交代码 
		 * 
		 */		
		protected function beSureToCommit():void{
			var num:uint=5-this.checkEmptyNum();
			var data:Array=[];
			data.push(num);
			for each(var o:* in this.stoneArr){
				if(o!=false){
					data.push(o.id);
				}
			}
			data.push(73);
			if(this.helpItemData){
				data.push(this.helpItemData.id);
			}else{
				data.push(0);
			}
			data.push(num);
			EquipSend.createMsgCompound(data);
						
			this.stoneType=0;
			this.stoneArr=[false,false,false,false,false];
			EquipDataConst.getInstance().lockItems=new Dictionary();
			this.removeAllItems();
			if(this.helpItem && this.view.contains(this.helpItem)){
				this.view.removeChild(this.helpItem);
			}
			this.helpItem=null;
			this.helpItemData=null;
			this.helpItemEnable=true;
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			(this.view.btn_commit as SimpleButton).visible=false;
		}
	
		/**
		 *  查看一下是否含有绑定的道具进行宝石合成
		 * @return true:有使用绑定道具  false:无
		 * 
		 */		
		protected function isHasBindItem():Boolean{
			for each(var o:* in this.stoneArr){
				if(o!=false){
					if(o.isBind==1)return true
				}
			}
			if(this.helpItemData && this.helpItemData.isBind==1)return true;
			return false;
		}
		
	
	
	
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EquipCommandList.ADD_STONECOMPOSE_STONE,
				EquipCommandList.ADD_STONECOMPOSE_ITEM,
				EquipCommandList.BUY_STRENGENHELP_ITEM
			];
		}
		
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EquipCommandList.ADD_STONECOMPOSE_STONE:
					var obj:Object=notification.getBody();
					//还没有放任何宝石，可以放入任何宝石
					if(this.stoneType==0){
						var num:uint=Math.min(obj.amount,5);
						this.addStone(obj,num);
						this.stoneType=obj.type;
					}else{
						//同类型宝石（检查是否有空的位置）
						if(this.stoneType==obj.type){
							//todo	
							if(this.checkEmptyNum()>0){
								var stoneNum:uint=Math.min(this.checkEmptyNum(),obj.amount);
								if(EquipDataConst.getInstance().lockItems[obj.id]){
									stoneNum=Math.min(this.checkEmptyNum(),uint(obj.amount-EquipDataConst.getInstance().lockItems[obj.id].usedNum));
								}
								this.addStone(obj,stoneNum);
							}else{
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_sto_han" ], color:0xffff00});//"已经没有位置可以放置宝石"
								return;
							}
						}else{
							//todo更换宝石
							for each(var o1:* in this.stoneArr){
								if(o1!=false){
									delete EquipDataConst.getInstance().lockItems[o1.id];
								}
							}
							this.stoneArr=[false,false,false,false,false];
							var changeNum:uint=Math.min(obj.amount,5);
							this.addStone(obj,changeNum);
							this.stoneType=obj.type;	
						}
					}
					if(this.helpItemData){
						delete EquipDataConst.getInstance().lockItems[this.helpItemData.id];
					}
					if(this.helpItem && this.view.contains(this.helpItem)){
						this.view.removeChild(this.helpItem);
					}
					this.helpItem=null;
					this.helpItemData=null; 
					this.helpItemEnable=true;
					this.changeDes(); 
					break;
				case EquipCommandList.ADD_STONECOMPOSE_ITEM:
				
					if(this.helpItem==null)return;
					if(this.stoneType==0)return;
					
					if(this.helpItemData==null && this.helpItemEnable){
						var objStone:Object=notification.getBody();
						var level:uint=this.stoneType%10;
						if(level>=4 && objStone.type==610016)return;
						
						this.helpItemData=notification.getBody();
						if(this.view.contains(this.helpItem)){
							this.view.removeChild(this.helpItem);
						}
						this.helpItem=new EnableItem(this.helpItemData.type,"icon");
						this.helpItem.addEventListener(MouseEvent.CLICK,onHelpItemMouseClick);
						this.view.addChild(this.helpItem);
						this.helpItem.x=this.view.mc_5.x+3;
						this.helpItem.y=this.view.mc_5.y+3;
						this.helpItem.name="goodQuickBuy_"+this.helpItemData.type;
						this.helpItemEnable=false;
						this.helpItem.setEnable(this.helpItemEnable);
						if(EquipDataConst.getInstance().lockItems[helpItemData.id]){
							var n:uint=EquipDataConst.getInstance().lockItems[helpItemData.id].usedNum;
							n++;
							EquipDataConst.getInstance().lockItems[helpItemData.id].usedNum=n;
						}else{
							EquipDataConst.getInstance().lockItems[helpItemData.id]={usedNum:1};
						}
						this.changeScale();	
						sendNotification(EquipCommandList.REFRESH_HELP_ITEM);	
					}
					break;	
					//购买符返回消息 obj==1.证明购买失败	
				case EquipCommandList.BUY_STRENGENHELP_ITEM:
					if(this.view.parent==null)return;
					if(this.helpItemData){
						this.isBuyFlag=false;
						return;
					}
					if(notification.getBody() && notification.getBody()==1){
						this.isBuyFlag=false;
					}else{
						
						if(this.isBuyFlag){
							this.isBuyFlag=false;
							this.autoUpItem();
							this.changeDes();
						}
					}
					break;
			}
		}
		
		protected function checkEmptyNum():uint{
			var count:uint=0;
			for each(var o:* in this.stoneArr){
				if(o==false){
					count++;
				}
			}
			return count;
		}
		
		
		protected function removeAllItems():void{
			for each(var dis:DisplayObject in this.cells){
				if(this.view.contains(dis)){
					this.view.removeChild(dis);
				}
				dis=null;
			}
			this.cells=[];
		}
		
		
		/**
		 * 判断是否有空位置 
		 * @return 
		 * 
		 */		
		protected function isEmpty():Boolean{
			for each(var o:* in this.stoneArr){
				if(o==false){
					return true;
				}
			}
			return false;
		}
		
		/**
		 * 改变描述 
		 * 
		 */		
		protected function changeDes():void{
			this.removeAllItems();
			this.createStones();	
			if(this.checkIsEmpty()){
				this.stoneType=0;
				if(this.helpItem && this.view.contains(this.helpItem)){
					this.view.removeChild(this.helpItem);
				}
				if(this.helpItemData){
					delete EquipDataConst.getInstance().lockItems[this.helpItemData.id];
					this.helpItemData=null;
					this.helpItem=null;
				}
			}
			if(this.helpItemData==null){
				this.autoUpItem();	
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			var num:uint=5-this.checkEmptyNum();
			if(num<3){
				this.view.txt_success.text="0%";
				(this.view.btn_commit as SimpleButton).visible=false;
			}else{
				var scale:uint=0;
				if(num==3){
					scale=25;
				}else if(num==4){
					scale=50;
				}else if(num==5){
					scale=75;
				}
				if(this.helpItemData){
					scale+=25;
				}
				this.view.txt_success.text=scale+"%";
				(this.view.btn_commit as SimpleButton).visible=true;
			}
		}
		
		/**
		 * 添加宝石合成符后从新统计成功率 
		 * 
		 */		
		protected function changeScale():void{
			var num:uint=5-this.checkEmptyNum();
			if(num<3){
				this.view.txt_success.text="0%";
				(this.view.btn_commit as SimpleButton).visible=false;
			}else{
				var scale:uint=0;
				if(num==3){
					scale=25;
				}else if(num==4){
					scale=50;
				}else if(num==5){
					scale=75;
				}
				if(this.helpItemData){
					scale+=25;
				}
				this.view.txt_success.text=scale+"%";
				(this.view.btn_commit as SimpleButton).visible=true;
			}
		}
		
		/**
		 * 检查一下是否已经没有任何宝石放入了 
		 * true:空
		 * false:有宝石 
		 */		
		protected function checkIsEmpty():Boolean{
			for each(var o:* in this.stoneArr){
				if(o!=false){
					return false;
				}
			}
			return true;
		}
		
		/**
		 * 自动上符(当有宝石时)
		 * 
		 */		
		protected function autoUpItem():void{
			if(this.helpItem && this.view.contains(this.helpItem)){
				this.view.removeChild(this.helpItem);
				this.helpItem=null;
			}
			var level:uint=this.stoneType%10;
			var arr:Array=BagData.AllItems[1] as Array;
			if(level>=4 || level==0 && this.stoneType!=0){
				//todo用高级合成符
				for each(var item:* in arr){
					if(item==null)continue;
					if(item.type==610017){
						//lock;
						if(EquipDataConst.getInstance().lockItems[item.id]){
							var n:uint=EquipDataConst.getInstance().lockItems[item.id].usedNum;
							n++;
							EquipDataConst.getInstance().lockItems[item.id].usedNum=n;
						}else{
							EquipDataConst.getInstance().lockItems[item.id]={usedNum:1};
						}
						this.helpItemEnable=false;
						this.helpItemData=item;
						break;
					}
				}
				var dis:DisplayObject=this.view.getChildByName("goodQuickBuy_610017");
				if(dis){
					this.view.removeChild(dis);
				}
				
				this.helpItem=new EnableItem("610017","icon");
				this.helpItem.mouseEnabled=true;
				this.helpItem.addEventListener(MouseEvent.CLICK,onHelpItemMouseClick);
				this.helpItem.name="goodQuickBuy_610017";
				this.helpItem.setEnable(this.helpItemEnable);
				this.view.addChild(this.helpItem);
				this.helpItem.x=this.view.mc_5.x+3;
				this.helpItem.y=this.view.mc_5.y+3;
				
				
			}else if(level>0){
				//todo用低级合成符
				
				for each(var item1:* in arr){
					if(item1==null)continue;
					if(item1.type==610016){
						//lock;
						if(EquipDataConst.getInstance().lockItems[item1.id]){
							var n1:uint=EquipDataConst.getInstance().lockItems[item1.id].usedNum;
							n1++;
							EquipDataConst.getInstance().lockItems[item1.id].usedNum=n1;
						}else{
							EquipDataConst.getInstance().lockItems[item1.id]={usedNum:1};
						}
						this.helpItemEnable=false;
						this.helpItemData=item1;
						break;
					}
				}
				var dis1:DisplayObject=this.view.getChildByName("goodQuickBuy_610016");
				if(dis1){
					this.view.removeChild(dis1);
				}
					
				
				this.helpItem=new EnableItem("610016","icon");
				this.helpItem.mouseEnabled=true;
				this.helpItem.addEventListener(MouseEvent.CLICK,onHelpItemMouseClick);
				this.helpItem.name="goodQuickBuy_610016";
				this.helpItem.setEnable(this.helpItemEnable);
				this.view.addChild(this.helpItem);
				this.helpItem.x=this.view.mc_5.x+3;
				this.helpItem.y=this.view.mc_5.y+3;
			}
			
		}
		
		protected function onHelpItemMouseClick(e:MouseEvent):void{
			if(this.helpItemData){
				delete EquipDataConst.getInstance().lockItems[this.helpItemData.id];
				var num:uint=5-this.checkEmptyNum();
				if(num<3){
					this.view.txt_success.text="0%";
					(this.view.btn_commit as SimpleButton).visible=false;
				}else{
					var scale:uint=0;
					if(num==3){
						scale=25;
					}else if(num==4){
						scale=50;
					}else if(num==5){
						scale=75;
					}
				}
				this.helpItemData=null;
				if(this.helpItemData){
					scale+=25;
				}
				this.view.txt_success.text=scale+"%";
				this.helpItemEnable=true;
				this.helpItem.setEnable(this.helpItemEnable);
				sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
			}
		}
		
		
		/**
		 *  刷新加宝石显示渲染
		 * 
		 */		
		protected function createStones():void{
			for(var i:uint=0;i<5;i++){
				if(this.stoneArr[i]!=false){
					var cell:NumberItem=new NumberItem(this.stoneArr[i].type,"icon");
					cell.mouseEnabled=true;
					cell.addEventListener(MouseEvent.CLICK,onCellMouseClick);
					cell.name="bagQuickKey_"+this.stoneArr[i].id;
					this.view.addChild(cell);
					cell.x=(this.view["mc_"+i] as MovieClip).x+3;
					cell.y=(this.view["mc_"+i] as MovieClip).y+3;
					this.cells[i]=cell;			                   
				}
			}
		}
		
		
		
		protected function onCellMouseClick(e:MouseEvent):void{
			var index:uint=this.cells.indexOf(e.target);
			var n:uint=EquipDataConst.getInstance().lockItems[this.stoneArr[index].id].usedNum;
			n--;
			EquipDataConst.getInstance().lockItems[this.stoneArr[index].id].usedNum=n;
			this.stoneArr[index]=false;
			if(this.checkEmptyNum()==5){
				this.stoneType=0;
			}
			this.changeDes();
		}
		
		
		/**
		 * 向其中添加num数量的宝石 
		 * @param obj
		 * @param num
		 * 
		 */			
		protected function addStone(obj:Object,num:uint):void{
			var m:uint=num;
			for(var i:uint=0;i<5;i++){
				if(this.stoneArr[i]==false && m>0){
					this.stoneArr[i]=obj;
					m--;
					if(EquipDataConst.getInstance().lockItems[obj.id]){
						var n:uint=EquipDataConst.getInstance().lockItems[obj.id].usedNum;
						n++;
						EquipDataConst.getInstance().lockItems[obj.id].usedNum=n;
					}else{
						EquipDataConst.getInstance().lockItems[obj.id]={usedNum:1};
					}
				}
			}
		}
		
		
		/**
		 * 清除复位 
		 * 
		 */		
		protected function clearAll():void{
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");	
			if(this.buyItem && this.view.contains(this.buyItem)){
				this.view.removeChild(this.buyItem);
			}
			UIUtils.removeFocusLis(this.view.txt_inputNum);
			this.selectedItemData=null;
			this.buyItem=null;
			this.stoneType=0;
			this.stoneArr=[false,false,false,false,false];
			this.helpItemData=null;
			EquipDataConst.getInstance().lockItems=new Dictionary();
			this.removeAllItems();
			(this.view.btn_commit as SimpleButton).visible=false;
		}
		
	}
}
