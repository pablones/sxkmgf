package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Equipment.ui.NumberItem;
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

	public class HunYunMediator extends Mediator
	{
		public static const NAME:String="HunYunMediator";
		
		protected var equipData:Object;
		protected var useItem:UseItem;
		protected var buyItem:NumberItem;
		
		protected var icon1:NumberItem;
		protected var icon2:NumberItem;
		protected var icon3:NumberItem;
		
		public function HunYunMediator(mediatorName:String=null, viewComponent:Object=null)
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
		
		public function init():void{
			if(this.icon1==null ){
				this.icon1=new NumberItem("610047","icon");
				this.icon1.name="goodQuickBuy_610047";
			}else if(this.view.contains(this.icon1)){
				this.view.removeChild(this.icon1);
			}
			
			if(this.icon2==null){
				this.icon2=new NumberItem("610047","icon");
				this.icon2.name="goodQuickBuy_610047";
			}else if(this.view.contains(this.icon2)){
				this.view.removeChild(this.icon2);
			}
			
			if(this.icon3==null){
				this.icon3=new NumberItem("610047","icon");
				this.icon3.name="goodQuickBuy_610047";
			}else if(this.view.contains(this.icon3)){
				this.view.removeChild(this.icon3);
			}
			if(this.buyItem==null){
				this.buyItem=new NumberItem("610047","icon");
				this.buyItem.x=258;
				this.buyItem.y=33;
				this.buyItem.name="goodQuickBuy_610047";
				this.view.addChild(this.buyItem);
			}
			(this.view.txt_1 as TextField).mouseEnabled=false;
			(this.view.txt_2 as TextField).mouseEnabled=false;
			(this.view.txt_3 as TextField).mouseEnabled=false;
			
			this.showIcon(3);
			(this.view.txt_inputNum as TextField).restrict="0-9";
			(this.view.txt_inputNum as TextField).multiline=false;
			(this.view.txt_inputNum as TextField).mouseWheelEnabled=false;
			(this.view.txt_inputNum as TextField).maxChars=4;
			UIUtils.addFocusLis(this.view.txt_inputNum);
			(this.view.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,onBuyItemClick);
			(this.view.btn_commit as SimpleButton).visible=false;
					
//			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"10\\se");
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"100000");
			(this.view.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClick);
		}
		
		/**
		 * 购买魂印宝珠 
		 * @param e
		 * 
		 */		
		protected function onBuyItemClick(e:MouseEvent):void{			
			////////////////////////////////////////////////////
			//	魂印宝符商城数据
			var obj:Object=UIConstData.MarketGoodList[30][0];
			///////////////////////////////////////////////	
			var num:uint=uint(this.view.txt_inputNum.text);
			if(num == 0){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyI_2" ], color:0xffff00});//"请输入有效的购买数"
				return;
			}
			var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+obj.PriceIn*num+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+num+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+obj.Name+'</font>';
			//花费		购买		个
			facade.sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuy, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"			
		}
		
		
		protected function onSureToBuy():void{
		 	var num:uint=uint(this.view.txt_inputNum.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:"610047",count:num}); 
		 }
		
		/**
		 * 提交魂印内容 
		 * @param e
		 * 
		 */		
		protected function onCommitClick(e:MouseEvent):void{
			
			 if(this.useItem==null || this.equipData==null){
			 	facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onC_2" ], color:0xffff00});//"请先放入你的装备"
			 	return;
			 }
			 
			 var money:uint=GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
			if(money<100000){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_ext_onC_4" ], color:0xffff00});//"你的银两不足"
				return;
			}
			
			if(this.equipData.isBind==0){
				var _id:uint = this.equipData.id;
				facade.sendNotification(EventList.SHOWALERT, {comfrim:beSureToCommit, cancel:new Function(), info:GameCommonData.wordDic[ "mod_equ_med_hun_han_1" ],title:GameCommonData.wordDic[ "often_used_tip" ],params:_id});
				//"魂印后，装备将自动绑定"		"提 示"
			}else{
				var param:Array=[1,0,76,this.equipData.id,0];
				EquipSend.createMsgCompound(param); 
				updateView();
			}
		}
		
		/**
		 * 确定魂印
		 * */
		protected function beSureToCommit(id:Object):void
		{
			var param:Array=[1,0,76,id,0];
			EquipSend.createMsgCompound(param);
			updateView();
		}
		/**
		 * 跟新界面 
		 * 
		 */		
		private function updateView():void
		{
			if(this.useItem && this.view.contains(this.useItem)){
				this.view.removeChild(this.useItem);
			}
			this.equipData=null;
			this.useItem=null;
			sendNotification(EquipCommandList.RETURN_EQUIP_INFO); //刷新一下装备
			sendNotification(EquipCommandList.REFRESH_EQUIP);
			EquipDataConst.getInstance().lockItems=new Dictionary();
			this.changeDes();
		}
		public function get view():MovieClip{
			return this.viewComponent.view as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EquipCommandList.ADD_HUNYUN_EQUIP,
				EquipCommandList.ADD_HUNYUN_ITEM,
				EquipCommandList.RECALL_HUNYUN_EQUIP,
				EquipCommandList.CHANGE_HUNYUN_ITEM,
				EquipCommandList.DROP_EQUIP_ITEM
			];
		}
		
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EquipCommandList.ADD_HUNYUN_EQUIP:
					var hunYunObj:Object=IntroConst.ItemInfo[notification.getBody().id];
					if(hunYunObj.isBind!=2){
						if(this.useItem && this.view.contains(this.useItem)){
							this.view.removeChild(this.useItem);
							this.useItem=null;
							this.equipData=null;
						}
						
						this.equipData=notification.getBody();
						this.useItem=new UseItem(0,this.equipData.type,null);
						this.view.addChild(this.useItem);
						this.useItem.Id=this.equipData.id;
						this.useItem.x=141;
						this.useItem.y=53; 
						this.useItem.mouseEnabled=true;
						this.useItem.name="bagQuickKey_"+this.equipData.id;
						this.useItem.addEventListener(MouseEvent.CLICK,onEquipMouseClick);
						this.changeDes();
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "med_lost_9" ], color:0xffff00});//"该装备已经魂印过了"
						sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
					}
					break;	
				case EquipCommandList.RECALL_HUNYUN_EQUIP:
					if(notification.getBody().type==1){
						sendNotification(EquipCommandList.ADD_SPECIAL_SHOW,new Point(164,76));
						ItemInfo.isLevelUp=true;
						UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
						if(this.useItem && this.view.contains(this.useItem)){
							this.view.removeChild(this.useItem);
						}
						this.equipData=null;
						this.useItem=null;
						sendNotification(EquipCommandList.RETURN_EQUIP_INFO); //刷新一下装备
						EquipDataConst.getInstance().lockItems=new Dictionary();
						this.changeDes();
					}else{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_hun_han_2" ], color:0xffff00});//"魂印失败"
					}			
					break;
				case EquipCommandList.CHANGE_HUNYUN_ITEM:
					EquipDataConst.getInstance().lockItems=new Dictionary();
					this.changeDes();
					break;
				//装备掉落	
				case EquipCommandList.DROP_EQUIP_ITEM:
					if(this.view.stage==null || this.equipData==null)return;
					if(this.equipData.id==notification.getBody().id){
						this.clearAll();
					}
					break;	
						
			}
		}
		
		protected function onEquipMouseClick(e:MouseEvent):void{
			this.view.removeChild(this.useItem);
			this.useItem=null;
			this.equipData=null;
			sendNotification(EquipCommandList.RETURN_EQUIP_INFO);  //刷新一下装备
			EquipDataConst.getInstance().lockItems=new Dictionary();
			this.changeDes();
			
		}
		
		
		/**
		 * 改变描述显示  功能：自动上宝符
		 * 
		 */			
		protected function changeDes():void{
			this.clearIcon();
			if(this.useItem==null){
				this.showIcon(0);
				(this.view.btn_commit as SimpleButton).visible=false;
			}else{						
				//紫色
				if(this.equipData.color==4){
					this.showIcon(2);
					this.requestItem(2);
				//橙色	
				}else if(this.equipData.color==5){
					this.showIcon(3);
					this.requestItem(3);
				}else{
					this.showIcon(1);	
					this.requestItem(1);
				}
				
			}
			sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
		}	
			
		public function clearIcon():void{
			if(this.view.contains(this.icon1))this.view.removeChild(this.icon1);
			if(this.view.contains(this.icon2))this.view.removeChild(this.icon2);
			if(this.view.contains(this.icon3))this.view.removeChild(this.icon3);
			(this.view.btn_commit as SimpleButton).visible=false;
		}
		
		
		protected function showIcon(num:uint):void{
			if(num==0){
				(this.view.mc_1 as MovieClip).visible=true;
				(this.view.txt_1 as TextField).visible=true;
				(this.view.mc_2 as MovieClip).visible=true;
				(this.view.txt_2 as TextField).visible=true;
				(this.view.mc_3 as MovieClip).visible=true;
				(this.view.txt_3 as TextField).visible=true;
			}else if(num==1){
				(this.view.mc_1 as MovieClip).visible=false;
				(this.view.txt_1 as TextField).visible=false;
				(this.view.mc_2 as MovieClip).visible=true;
				(this.view.txt_2 as TextField).visible=true;
				(this.view.mc_3 as MovieClip).visible=false;
				(this.view.txt_3 as TextField).visible=false;
			}else if(num==2){
				(this.view.mc_1 as MovieClip).visible=true;
				(this.view.txt_1 as TextField).visible=true;
				(this.view.mc_2 as MovieClip).visible=false;
				(this.view.txt_2 as TextField).visible=false;
				(this.view.mc_3 as MovieClip).visible=true;
				(this.view.txt_3 as TextField).visible=true;
			}else if(num==3){
				(this.view.mc_1 as MovieClip).visible=true;
				(this.view.txt_1 as TextField).visible=true;
				(this.view.mc_2 as MovieClip).visible=true;
				(this.view.txt_2 as TextField).visible=true;
				(this.view.mc_3 as MovieClip).visible=true;
				(this.view.txt_3 as TextField).visible=true; 
			}	
		}
		
		/**
		 * 申请魂印宝珠.从背包中申请宝珠，如果够，就直接显示。如果不够。有几个显示几个 
		 * 
		 */		
		protected function requestItem(num:uint):void{
			var resultCount:uint;
			if(num==1){
				if(this.getItem(1)==1){
					this.view.addChild(this.icon2);
					this.icon2.x=(this.view.mc_2 as MovieClip).x+3;
					this.icon2.y=(this.view.mc_2 as MovieClip).y+3;
					(this.view.btn_commit as SimpleButton).visible=true;
				}
					
			}else if(num==2){
				resultCount=this.getItem(2);
				if(resultCount==1){
					this.view.addChild(this.icon1);
					this.icon1.x=(this.view.mc_1 as MovieClip).x+2;
					this.icon1.y=(this.view.mc_1 as MovieClip).y+2;
				}else if(resultCount==2){
					this.view.addChild(this.icon1);
					this.icon1.x=(this.view.mc_1 as MovieClip).x+2;
					this.icon1.y=(this.view.mc_1 as MovieClip).y+2;
					
					this.view.addChild(this.icon3);
					this.icon3.x=(this.view.mc_3 as MovieClip).x+2;
					this.icon3.y=(this.view.mc_3 as MovieClip).y+2;
					(this.view.btn_commit as SimpleButton).visible=true;
				}
				
			}else if(num==3){
				resultCount=this.getItem(3);
				if(resultCount==1){
					this.view.addChild(this.icon2);
					this.icon2.x=(this.view.mc_2 as MovieClip).x+2;
					this.icon2.y=(this.view.mc_2 as MovieClip).y+2;
				}else if(resultCount==2){
					this.view.addChild(this.icon1);
					this.icon1.x=(this.view.mc_1 as MovieClip).x+2;
					this.icon1.y=(this.view.mc_1 as MovieClip).y+2;
					
					this.view.addChild(this.icon2);
					this.icon2.x=(this.view.mc_2 as MovieClip).x+2;
					this.icon2.y=(this.view.mc_2 as MovieClip).y+2;
				}else if(resultCount==3){
					this.view.addChild(this.icon1);
					this.icon1.x=(this.view.mc_1 as MovieClip).x+2;
					this.icon1.y=(this.view.mc_1 as MovieClip).y+2;
					
					this.view.addChild(this.icon2);
					this.icon2.x=(this.view.mc_2 as MovieClip).x+2;
					this.icon2.y=(this.view.mc_2 as MovieClip).y+2;
					
					this.view.addChild(this.icon3);
					this.icon3.x=(this.view.mc_3 as MovieClip).x+2;
					this.icon3.y=(this.view.mc_3 as MovieClip).y+2;		
					(this.view.btn_commit as SimpleButton).visible=true;			
				}	
			}
		}
		
		protected function getItem(num:uint):uint{
			var arr:Array=BagData.AllItems[1] as Array;
			var count:int=0;
			if(num==1){
				for each(var obj:Object in arr){
					if(obj!=null && obj.type==610047){
						//todo锁定
						EquipDataConst.getInstance().lockItems[obj.id]=true;
						return 1;
					}
				}	
			}else if(num==2){
				for each(var obj2:Object in arr){
					if(obj2!=null && obj2.type==610047){
						if(obj2.amount>=2){
							EquipDataConst.getInstance().lockItems[obj2.id]=true;
							return 2;
						}else if(obj2.amount==1){
							count++;
							EquipDataConst.getInstance().lockItems[obj2.id]=true;
							if(count>=2)return 2;
						}
					}
				}
			}else if(num==3){
				for each(var obj3:Object in arr){
					if(obj3!=null && obj3.type==610047){
						if(obj3.amount>=3){
							EquipDataConst.getInstance().lockItems[obj3.id]=true;
							return 3;
						}else if(obj3.amount==2){
							count+=2;
							EquipDataConst.getInstance().lockItems[obj3.id]=true;
							if(count>=3)return 3;
						}else if(obj3.amount==1){
							count++;
							EquipDataConst.getInstance().lockItems[obj3.id]=true;
							if(count>=3)return 3;
						}
					}
				}
					
			}
			return count;
		}
		
		
		
		public function clearAll():void{
			sendNotification(EquipCommandList.UPDATE_NEEDMONEY_EQUIP,"");
			UIUtils.removeFocusLis(this.view.txt_inputNum);
			(this.view.btn_commit as SimpleButton).visible=false;
			
			if(this.useItem && this.view.contains(this.useItem)){
				this.view.removeChild(this.useItem);
			}
			this.showIcon(3);
		}
		
	}
}