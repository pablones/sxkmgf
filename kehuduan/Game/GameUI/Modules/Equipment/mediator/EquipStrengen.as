package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.SetFrame;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.ItemInfo;
	import Net.ActionSend.EquipSend;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 *  装备的强化
	 * @author felix
	 * 
	 */	
	public class EquipStrengen extends Mediator
	{
		public static const NAME:String="EquipStrengen";
		private const DEFAULT_POS:Point=new Point(200,100);
	
		private var basePanel:PanelBase;
		private var dataProxy:DataProxy;
		/** 所需金钱*/
		private var moneyTextField:TextField;
		private var moneyContainer:Sprite;
		private var equipItem:UseItem;
		/** 失败加成数*/
		private var failScale:uint;
		/** 选择索引 */
		private var selectedRadioIndex:uint;
		/** 图像*/
		private var faceItem:FaceItem;
		
		/**
		 * 强化
		 * @param mediatorName
		 * @param viewComponent
		 * 
		 */		
		public function EquipStrengen(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get viewUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
					EventList.INITVIEW,
					EquipCommandList.SHOW_EQUIPSTRENGEN_UI,
					EquipCommandList.UPDATE_EQUIPSTRENGEN_UI,
					EquipCommandList.UPDATE_FAIL_SCALE,
					EquipCommandList.RECALL_EQUIPSTRENGEN,
					EquipCommandList.REFRESH_EQUIP,
					EquipCommandList.UPDATE_QH_EQUIP,
					EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE	//转场时关闭面板
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"EquipStrengen_1"});
					this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					this.basePanel=new PanelBase(this.viewUI,this.viewUI.width+8,this.viewUI.height+12);
					this.basePanel.SetTitleTxt("装备强化");
					this.basePanel.addEventListener(Event.CLOSE,onClosePanelHandler);
					(this.viewUI.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitHandler);
					(this.viewUI.btn_cancel as SimpleButton).addEventListener(MouseEvent.CLICK,onClosePanelHandler);
				
					(this.viewUI.shopItem_0.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
					(this.viewUI.shopItem_1.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
					(this.viewUI.shopItem_2.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
					
					
					this.viewUI.shopItem_0.txt_goodName.mouseEnabled=false;
			 		this.viewUI.shopItem_0.txt_goodPrice.mouseEnabled=false;
					this.viewUI.shopItem_1.txt_goodName.mouseEnabled=false;
			 		this.viewUI.shopItem_1.txt_goodPrice.mouseEnabled=false;
					this.viewUI.shopItem_2.txt_goodName.mouseEnabled=false;
			 		this.viewUI.shopItem_2.txt_goodPrice.mouseEnabled=false;
					
				
					this.moneyContainer=new Sprite();
					this.moneyTextField=new TextField();
					this.moneyTextField.filters=Font.Stroke();
					this.moneyTextField.defaultTextFormat=this.textFormat();
					this.moneyTextField.width=600;
					this.moneyTextField.autoSize=TextFieldAutoSize.LEFT;
					this.moneyTextField.wordWrap=false;
					this.moneyTextField.mouseEnabled=false;
					this.moneyTextField.selectable=false;
					
					this.moneyContainer.addChild(this.moneyTextField);
					this.viewUI.addChild(moneyContainer);
					moneyContainer.x=4;
					moneyContainer.y=272;
					this.initSet();
					break;
				case EquipCommandList.SHOW_EQUIPSTRENGEN_UI:
					EquipDataConst.getInstance().openPanel(3);
					this.showView();
					break;
				case EquipCommandList.UPDATE_QH_EQUIP:
					var obj:Object=notification.getBody();
					if(this.equipItem!=null){
						this.cancelLock(obj.useItem);
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"已经有装备了", color:0xffff00}); 
						return;
					}	
					var useItem:UseItem=obj.useItem;
					if(useItem!=null && EquipDataConst.getInstance().isEquip(useItem.Type)){
						this.addEquip(useItem);
					}else if(useItem!=null){
						this.cancelLock(useItem);
					}
					break;	
				//收到失败加成率	
				case EquipCommandList.UPDATE_FAIL_SCALE:
					var num:uint=uint(notification.getBody());
					if(num%10==1){
						this.failScale=Math.floor(num/10)/10;
						this.changeDes();
					}
					break;	
				//服端返回信息	
				case EquipCommandList.RECALL_EQUIPSTRENGEN:
					var type:uint=uint(notification.getBody()["type"]);
					if(type<1 || type>2)return;
					if(type==1){
						var equipLevel:uint=IntroConst.ItemInfo[this.equipItem.Id].level;
						 if(EquipDataConst.getInstance().isScaleClear(equipLevel)){
							this.failScale=0;
						 }
					}else if(type==2){
						this.failScale+=(this.selectedRadioIndex+1);
					}
					ItemInfo.isLevelUp = true;
					UiNetAction.GetItemInfo(this.equipItem.Id,GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);	
					this.changeDes();
					break;
				case EquipCommandList.REFRESH_EQUIP:
					if(this.dataProxy.EquipStrengenIsOpen){
						this.changeMoney();
						this.changeDes();
					}
					break;
				case EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE:		//转场时关闭面板
					if(dataProxy.EquipStrengenIsOpen) onClosePanelHandler(null);
					break;
			}
		}
		
		
		/**
		 * 初始化设置 
		 * 
		 */		
		protected function initSet():void{
			(this.viewUI.txt_lucklyScale as TextField).text="0";
			(this.viewUI.txt_strengLevel as TextField).text="+0";
			(this.viewUI.txt_success as TextField).text="0%";
			(this.viewUI.txt_clear as TextField).text="不降";
			for(var i:uint=0;i<4;i++){
				(this.viewUI["mc_radio"+i] as MovieClip).addEventListener(MouseEvent.CLICK,onRadioButtonClick);
			}
			this.setSelectedRadio(3);
			this.changeShop();
		}
		
		protected function onRadioButtonClick(e:MouseEvent):void{
			var nameStr:String=e.currentTarget.name;
			nameStr=nameStr.substring(nameStr.length-1,nameStr.length);
			this.selectedRadioIndex=uint(nameStr);
			this.setSelectedRadio(uint(nameStr));
			this.changeDes();
		}
		
		/**
		 * 设置选中radioButton  
		 * @param index ：选中索引
		 * 
		 */		
		protected function setSelectedRadio(index:uint):void{
			for(var i:uint=0;i<4;i++){
				if(index==i){
					(this.viewUI["mc_radio"+i]as MovieClip).gotoAndStop(2);
					this.selectedRadioIndex=index;
				}else{
					(this.viewUI["mc_radio"+i]as MovieClip).gotoAndStop(1);
				}
			}	
		}
		
		protected function onMouseOverHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.UseFrame(mc,"RedFrame");
		}
		
		protected function onMouseOutHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.RemoveFrame(mc.parent,"RedFrame");
		}
		
		protected function onCommitHandler(e:MouseEvent):void{
			if(equipItem==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先放入你的装备", color:0xffff00}); 
			}else{
				this.commitEquip();
			}
		}
		
		/**
		 * 提交升星
		 * 
		 */		
		private function commitEquip():void{
			var params:Array=[1,this.selectedRadioIndex+1,62,this.equipItem.Id,0];
			EquipSend.createMsgCompound(params);
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.font = "宋体";
			return tf;
		}
		
		/**
		 * 添加物品 
		 * @param useItem
		 * @param index
		 * 
		 */		
		protected function addEquip(useItem:UseItem):void{
			this.equipItem=useItem;
			var equipLevel:uint=IntroConst.ItemInfo[this.equipItem.Id].level;
			if(equipLevel==10){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你已经强化到最高等级了", color:0xffff00}); 
				this.cancelLock(useItem);
				return ;
			}
			faceItem=new FaceItem(String(useItem.Type),null);
			faceItem.addEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
			faceItem.addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			faceItem.name="Strengen_"+useItem.Id;
			faceItem.setEnable(true);
			faceItem.addEventListener(MouseEvent.CLICK,onContainerMouseClickHandler);
			var container:MovieClip=this.viewUI.getChildByName("QH_0") as MovieClip;
			this.viewUI.addChild(faceItem);
			faceItem.x=container.x;
			faceItem.y=container.y;
			this.requestFailScale(useItem);
			this.changeMoney();
			this.changeDes();
		}
		
		/**
		 * 改变所需银两 
		 * 
		 */		
		protected function changeMoney():void{
			this.moneyTextField.width=300;
			if(this.equipItem==null){
				this.moneyTextField.htmlText='<font color="#ffffff">需要花费：</font>';
				ShowMoney.ShowIcon(this.moneyContainer,moneyTextField,true);
			}else{	
				var equipLevel:uint=IntroConst.ItemInfo[this.equipItem.Id].level;
				this.moneyTextField.htmlText=EquipDataConst.getInstance().getStrengFeeByLevel(equipLevel);
				ShowMoney.ShowIcon(this.moneyContainer,this.moneyTextField,true);
			}
		}
		
		
		/**
		 * 改娈文字的描述
		 * 
		 */		
		protected function changeDes():void{
			if(this.equipItem==null){
				(this.viewUI.txt_lucklyScale as TextField).text="0";
				(this.viewUI.txt_strengLevel as TextField).text="+0";
				(this.viewUI.txt_success as TextField).text="0%";
				(this.viewUI.txt_clear as TextField).text="不降";
			}else{
				var equipLevel:uint=IntroConst.ItemInfo[this.equipItem.Id].level;
				(this.viewUI.txt_lucklyScale as TextField).text=String(this.failScale);
				(this.viewUI.txt_strengLevel as TextField).text="+"+equipLevel;
				var colorStr:String=GameCommonData.Player.Role.VIPColor;
				var desStr:String=EquipDataConst.getInstance().getSuccessByStoreLevel(equipLevel,this.selectedRadioIndex+1)+"%";
				if(GameCommonData.Player.Role.VIP==2){
					desStr=EquipDataConst.getInstance().getSuccessByStoreLevel(equipLevel,this.selectedRadioIndex+1)+'%<font color="'+colorStr+'">+1%<font>';
				}else if(GameCommonData.Player.Role.VIP==3){
					desStr=EquipDataConst.getInstance().getSuccessByStoreLevel(equipLevel,this.selectedRadioIndex+1)+'%<font color="'+colorStr+'">+2%<font>';
				}
				(this.viewUI.txt_success as TextField).htmlText=desStr;
				if(EquipDataConst.getInstance().isToReset(equipLevel)){
					if(equipLevel==5 || equipLevel==6){
						(this.viewUI.txt_clear as TextField).text="4";
					}else if(equipLevel==8 || equipLevel==9) {
						(this.viewUI.txt_clear as TextField).text="7";
					}
				}else{
					(this.viewUI.txt_clear as TextField).text="不降";
				}
			}
		}
		
		
		/**
		 * 点击一下装备，将装备取下 
		 * @param e
		 * 
		 */		
		protected function onContainerMouseClickHandler(e:MouseEvent):void{
			var displayObj:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.RemoveFrame(displayObj.parent,"RedFrame");
			displayObj.parent.removeChild(displayObj);
			this.cancelLock(this.equipItem);
			this.equipItem=null;
			this.changeMoney();
			this.changeDes();
		}
		
		/**
		 * 显示 
		 * 
		 */		
		protected function showView():void{
			this.initSet();
			if(this.dataProxy.EquipStrengenIsOpen)return;
			dataProxy.EquipStrengenIsOpen=true;
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
			this.basePanel.x=DEFAULT_POS.x;
			this.basePanel.y=DEFAULT_POS.y;
		}
	
	
		/**
		 * 
		 * 改变商店里的物品（根据当前页签） 
		 * 
		 */		
		private function changeShop():void{
			this.clearShop();	
			 var arr:Array=UIConstData.MarketGoodList[16] as Array;
			 var len:uint=arr.length;
			 for(var i:int=0;i<len;i++){
			 	var face:FaceItem=new FaceItem(arr[i].type);
			 	face.setEnable(true);
			 	face.name="goodQuickBuy_"+arr[i].type;
			 	if(this.viewUI["shopItem_"+i].mc_goodIcon.numChildren>1){
			 		this.viewUI["shopItem_"+i].mc_goodIcon.removeChildAt(1);
			 	}
			 	
			 	this.viewUI["shopItem_"+i].mc_goodIcon.addChild(face);
			 	this.viewUI["shopItem_"+i].txt_goodName.text=arr[i].Name;
			 	this.viewUI["shopItem_"+i].txt_goodPrice.text=arr[i].PriceIn;
			 	
			 }	 
		}
		
		
		private function clearShop():void{
			for(var i:uint=0;i<3;i++){
				if(this.viewUI["shopItem_"+i].mc_goodIcon.numChildren>1){
			 		this.viewUI["shopItem_"+i].mc_goodIcon.removeChildAt(1);
			 	}
			 	this.viewUI["shopItem_"+i].txt_goodName.text="暂无";
			 	this.viewUI["shopItem_"+i].txt_goodPrice.text="0";
			}
		}
		
		/**
		 *  购买商品 
		 * @param e
		 * 
		 */		
		private  function buyGoodClickHandler(e:MouseEvent):void{
			var arrShop:Array=UIConstData.MarketGoodList[16] as Array;
			var arr:Array=e.currentTarget.parent.name.split("_");
			if(arrShop.length>arr[1]){
				sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:arrShop[arr[1]].type});	
			} 
		}
		
		/**
		 * 关闭面板（应该解除背包里锁定的物品） 
		 * @param e
		 * 
		 */		
		protected function onClosePanelHandler(e:Event):void{
			if(GameCommonData.GameInstance.GameUI.contains(this.basePanel)){
				if(this.faceItem!=null && this.faceItem.parent!=null){
					this.faceItem.parent.removeChild(this.faceItem);
				}	
				this.faceItem=null;
				GameCommonData.GameInstance.GameUI.removeChild(basePanel);
			}
			this.dataProxy.EquipStrengenIsOpen=false;
			this.cancelLock(this.equipItem);
			this.equipItem=null;
			
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
		
		/**
		 * 取消背包中对该物品的锁定 
		 * @param u
		 * 
		 */		
		private function cancelLock(u:UseItem):void{
			if(u==null)return;
			u.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,u.Id);
		}
		
	}
}