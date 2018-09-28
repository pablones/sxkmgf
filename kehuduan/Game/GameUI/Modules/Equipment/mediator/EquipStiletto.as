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
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;


	/**
	 * 打孔:对还没有打孔的装备进行打孔
	 * 镶嵌:（必须先放最左边的空位置）
	 * 摘取(有选择摘取某一颗宝石功能)
	 * @author felix
	 * 
	 */	
	public class EquipStiletto extends Mediator
	{
		public static const NAME:String="EquipStiletto";
		private var dataProxy:DataProxy;
		private var basePanel:PanelBase;
		private const DEFAULT_POS:Point=new Point(200,100);
		private var pageNum:uint;
		
		private var dataDic:Dictionary=new Dictionary();  //保存显示对象数据
		private var gridDic:Dictionary=new Dictionary();  //格子数据
		
		private var desTextField:TextField;
		private var moneyTextField:TextField;
		private var moneyContainer:Sprite;
		
		private var stoneArr:Array=[];
		private var selectedIndex:uint=0;
		private var selectedPageNum:uint=0;
		
		public function EquipStiletto(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			dataDic[0]=[null,null,null,null,null,0,0,"打  孔","打孔道具"];
			dataDic[1]=[null,null,null,null,null,0,0,"镶  嵌","镶嵌宝符"];
			dataDic[2]=[null,null,null,null,null,0,0,"摘  取","摘取宝符"];
			
			gridDic[0]=new Array(9);
			gridDic[1]=new Array(9);
			gridDic[2]=new Array(9);
		}
		
		public function get viewUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
			
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				EquipCommandList.SHOW_EQUIPSTILETTO_UI,
				EquipCommandList.UPDATE_EQUIPSTILETTO_UI,
				EquipCommandList.RECALL_EQUIPSTILETTO,
				EquipCommandList.REFRESH_EQUIP,
				EquipCommandList.CLOSE_EQUIP_STILETTO_PANEL
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch(notification.getName()){
				case EventList.INITVIEW:
					this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"EquipStiletto"});
					this.viewUI.mouseEnabled=false;
					basePanel=new PanelBase(this.viewUI,this.viewUI.width+8,this.viewUI.height+12);
					basePanel.addEventListener(Event.CLOSE,onClosePanelHandler);
									
					(this.viewUI.shopItem_0.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
					(this.viewUI.shopItem_1.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
					(this.viewUI.shopItem_2.btn_buy as SimpleButton).addEventListener(MouseEvent.CLICK,buyGoodClickHandler);
					
					this.viewUI.shopItem_0.txt_goodName.mouseEnabled=false;
			 		this.viewUI.shopItem_0.txt_goodPrice.mouseEnabled=false;
					this.viewUI.shopItem_1.txt_goodName.mouseEnabled=false;
			 		this.viewUI.shopItem_1.txt_goodPrice.mouseEnabled=false;
					this.viewUI.shopItem_2.txt_goodName.mouseEnabled=false;
			 		this.viewUI.shopItem_2.txt_goodPrice.mouseEnabled=false;
					
					initSet();
					break;
				case EquipCommandList.SHOW_EQUIPSTILETTO_UI:
					if(notification.getBody()!=null){
						this.selectedPageNum=uint(notification.getBody());
						this.setSelectedPage(selectedPageNum);
					}else{
						this.setSelectedPage(0);
					}
					EquipDataConst.getInstance().openPanel(4);
					this.showView();
					break;
					
				case EquipCommandList.UPDATE_EQUIPSTILETTO_UI:
					var useItem:UseItem=notification.getBody()["useItem"] as UseItem;
					var index:uint=uint(notification.getBody()["index"]);
					if(gridDic[pageNum][index]==null){
						this.addEquip(useItem,index);
					}else{
						useItem.IsLock=false;
						sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
					}
					break;
				case EquipCommandList.RECALL_EQUIPSTILETTO:
					switch(notification.getBody()["type"]){
						case 24:
							ItemInfo.isLevelUp=true;
							UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);	
							break;
						case 25:
							
							break;	
						case 9:
							ItemInfo.isLevelUp=true;
							UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
							break;
						case 10:
							break;	
						case 71:
							ItemInfo.isLevelUp=true;
							UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
							break;
						case 72:
							ItemInfo.isLevelUp=true;
							UiNetAction.GetItemInfo(notification.getBody()["eId"],GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
							break;				
					}
					break;
				case EquipCommandList.REFRESH_EQUIP:
					this.refresh();
					break;	
				case EquipCommandList.CLOSE_EQUIP_STILETTO_PANEL:
					this.onClosePanelHandler(null);
					break;	
			}
		}
		
		/**
		 * 刷新一下显示 ,主要是根据装备的显示
		 * 
		 */		
		protected function refresh():void{
			
			this.clearAllDes();
			for(var j:uint=0;j<2;j++){
				if(dataDic[pageNum][j]!=null){
					this.viewUI.addChild(dataDic[pageNum][j]);
				}
			}
			for(var i:uint=2;i<=4;i++){
				this.dataDic[pageNum][i]=null;
				if(pageNum==1 && this.gridDic[pageNum][i]!=null && this.gridDic[pageNum][i] is UseItem){
					this.cancelLock(this.gridDic[pageNum][i]);	
				}
				this.gridDic[pageNum][i]=null;
			}
			var useItem:UseItem=gridDic[pageNum][0] as UseItem;
			if(useItem!=null){
				this.addHoleIcon(useItem);
			}
			
			this.changeDes();
		}
		
		/**
		 * 向格子中加物品 
		 * @param useItem
		 * @param index
		 * 
		 */		
		protected function addEquip(useItem:UseItem,index:uint):void{
			if(useItem.Type==144000){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"不能对此装备进行操作", color:0xffff00});
				useItem.IsLock=false;
				sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);	
				return;	
			}
			if(index==0){
				
				if(!EquipDataConst.getInstance().isEquip(useItem.Type)){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你放入的物品不是装备", color:0xffff00});
					useItem.IsLock=false;
					sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
					return;
				}else{
					var obj:Object=IntroConst.ItemInfo[useItem.Id];;
					var stoneArr:Array=obj.stoneList;
					if(!EquipDataConst.getInstance().hasEmptyBore(useItem) && pageNum==0){
						if(stoneArr[0]==0 || stoneArr[1]==0 || stoneArr[2]==0){
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该装备不能打孔", color:0xffff00});
						}else{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该装备的孔已经打完", color:0xffff00});
						}
						useItem.IsLock=false;
						sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
						return;
					}
					if(pageNum==1){
						if(stoneArr[0]==0 || stoneArr[1]==0 || stoneArr[2]==0){
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该装备不能镶嵌", color:0xffff00});
							useItem.IsLock=false;
							sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
							return;
						}
					}
					if(pageNum==2){
						if(stoneArr[0]==0 || stoneArr[1]==0 || stoneArr[2]==0){
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该装备没有宝石可摘取", color:0xffff00});
							useItem.IsLock=false;
							sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
							return;
						}
					}
				}
				this.addIcon(useItem,index);
				this.addHoleIcon(useItem);
			}else{
				if(this.gridDic[pageNum][0]==null){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先放入你的装备", color:0xffff00});
					useItem.IsLock=false;
					sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
					return;
				}else{
					if(index==1){
						var eItem:UseItem=this.gridDic[pageNum][0] as UseItem;
						if(EquipDataConst.getInstance().isRightStore(pageNum,useItem.Type,eItem.Type)){
							this.addIcon(useItem,index);
						}else{//类型不对
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你放入的宝石类型不对或已经有同类型的宝石了", color:0xffff00}); 
							useItem.IsLock=false;
							sendNotification(EventList.BAGITEMUNLOCK,useItem.Id);
							return;
						}
					}else{						
						//只有pageNum==1才能放入与取下物品    与  pageNum==2可以选择物品
						if(pageNum==0){
							this.cancelLock(useItem);
						}else if(pageNum==1){
							this.addStoneInPage2(useItem,index);
						}else if(pageNum==2){
							this.cancelLock(useItem);
						}		
					}
				}	
			}
			this.changeDes();
		}
		
		/**
		 * 放入镶嵌宝石 （必须先放最左边的空位置） 
		 * @param useItem
		 * @param index
		 * 
		 */		
		private function addStoneInPage2(useItem:UseItem,index:uint):void{
			if(this.gridDic[pageNum][index]!=null ){   //已经有宝石了
				this.cancelLock(useItem);		
			}else{
				if(index!=this.getEmptyIndex()){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先将宝石放入最左边的孔", color:0xffff00}); 
					this.cancelLock(useItem);
					return;
				}
				if(this.isAddInPage2()){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"一次只能镶嵌一颗宝石", color:0xffff00}); 
					this.cancelLock(useItem); 
					return ;
				}
				var e:UseItem=this.gridDic[pageNum][0] as UseItem;
				if(EquipDataConst.getInstance().isRightS(e,useItem.Type)){
					this.addStone(useItem,index);
					this.gridDic[pageNum][index]=useItem;								
				}else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"放入的道具类型不对", color:0xffff00}); 
					this.cancelLock(useItem);                                     //宝石类型不对
				}
			}	
		}
		
		private function isAddInPage2():Boolean{
			
			for(var i:uint=2;i<5;i++){
				if(this.gridDic[pageNum][i] is UseItem){
					return true;
				}
			}
			return false;
		}
		
		private function getEmptyIndex():uint{
			for(var i:uint=2;i<5;i++){
				if(this.gridDic[pageNum][i]==null){
					return i;
				}
			}
			return 0;
		}
		
		
		protected function addStone(u:UseItem,index:uint):void{
			var face:FaceItem=new FaceItem(String(u.Type));
			face.name="stone_"+index;
			face.setEnable(true);
			face.addEventListener(MouseEvent.CLICK,onStoneClickHandler);
			this.viewUI.addChild(face);
			face.x=9+49*(index-2)+7;
			face.y=76+7;
			this.dataDic[pageNum][index][1]=face;
		}
		
		protected function onStoneClickHandler(e:MouseEvent):void{
			var str:String=e.currentTarget.name;
			str=str.substr(str.length-1,str.length);
			var index:uint=uint(str);
			this.viewUI.removeChild(e.currentTarget as DisplayObject);
			(this.gridDic[pageNum][index] as UseItem).IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,(this.gridDic[pageNum][index] as UseItem).Id);
			this.gridDic[pageNum][index]=null;
			this.dataDic[pageNum][index][1]=null;
			this.changeDes();
		}
		
		/**
		 * 添加孔图标 
		 * @param u
		 * 
		 */			
		protected function addHoleIcon(u:UseItem):void{
			var arr:Array=IntroConst.ItemInfo[u.Id].stoneList as Array;
			var len:uint=arr.length;
			var faceItem:FaceItem;
			this.stoneArr=[];
			var flag:Boolean=false;
			for(var i:uint=0;i<len;i++){
				if(arr[i]==99999){
					
				}else if(arr[i]==88888){
					faceItem=new FaceItem("800001");
					faceItem.setEnable(true);
					this.viewUI.addChild(faceItem)
					faceItem.x=9+49*i;
					faceItem.y=76;
					faceItem.name="DK_"+(i+2);
					this.dataDic[pageNum][i+2]=[faceItem];
				}else{
					faceItem=new FaceItem("800001");
					this.viewUI.addChild(faceItem)
					faceItem.x=9+49*i;
					faceItem.y=76;
					
					//加红色的框和ToolTip  
					var stone:FaceItem=new FaceItem(arr[i]);
					this.viewUI.addChild(stone);
					stone.x=9+49*i+7;
					stone.y=76+7;
					this.dataDic[pageNum][i+2]=[faceItem,stone];
					
					if(pageNum==1){  //打上标记  已经有宝石了
						gridDic[pageNum][i+2]=1;
					}
					if(pageNum==2){
						stone.addEventListener(MouseEvent.CLICK,onStoneMouseClick);
						stone.setEnable(true);
						if(!flag){
							EquipDataConst.getInstance().setStoneSelected(stone);
//							SetFrame.UseFrame(stone,"RedFrame");
							this.selectedIndex=i;
							flag=true;
						}
						this.stoneArr[i]=stone;
					}
				}				
			}
		}
		
		/**
		 * 改变选择颜色 
		 * @param e
		 * 
		 */		
		protected function onStoneMouseClick(e:MouseEvent):void{
			this.clearAllYellowFrame();
			var mc:DisplayObjectContainer=e.currentTarget as DisplayObjectContainer;
			var index:int=this.stoneArr.indexOf(mc);
			this.selectedIndex=index;
			EquipDataConst.getInstance().setStoneSelected(mc);
//			SetFrame.UseFrame(mc,"RedFrame");
		}
		/**
		 * 修改为红色框 
		 * 
		 */		
		protected function clearAllYellowFrame():void{
			for each(var s:DisplayObjectContainer in this.stoneArr){
				if(s.parent!=null){
					EquipDataConst.getInstance().removeStoneSelected(s);
//					SetFrame.RemoveFrame(s.parent,"RedFrame");
				}
			}
		}
			
		/**
		 * 添加图标 
		 * @param useItem
		 * @param index
		 * 
		 */		
		private function addIcon(useItem:UseItem,index:uint):void{
			var sprite:Sprite=this.viewUI.getChildByName("DK_"+index) as Sprite;
			var faceItem:FaceItem=new FaceItem(String(useItem.Type));
			faceItem.name="EDK_"+useItem.Id+"_"+index;
			faceItem.setEnable(true);
			faceItem.addEventListener(MouseEvent.MOUSE_OVER,onMouseOverHandler);
			faceItem.addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			faceItem.addEventListener(MouseEvent.CLICK,onGridMouseClick);
			this.dataDic[pageNum][index]=faceItem;
			this.gridDic[pageNum][index]=useItem;
			this.viewUI.addChild(faceItem);
			faceItem.x=sprite.x;
			faceItem.y=sprite.y;
		}
		
		
		/**
		 * 点击装备或各种符 
		 * @param e
		 * 
		 */		
		private function onGridMouseClick(e:MouseEvent):void{
			
			var sName:String=e.currentTarget.name;
			var	index:uint=uint(sName.substring(sName.length-1,sName.length));
			var item:UseItem=this.gridDic[pageNum][index] as UseItem;
			item.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,item.Id);
			this.viewUI.removeChild(this.dataDic[pageNum][index]);
			this.dataDic[pageNum][index]=null;
			this.gridDic[pageNum][index]=null;
			
			if(pageNum==2 && index==0){
				this.clearAllYellowFrame();
				this.stoneArr=[];
			}
			this.refresh();
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.font = "宋体";
			return tf;
		}
		
		/**
		 * 初始化设置 
		 * 
		 */		
		protected function initSet():void{
			
			this.moneyContainer=new Sprite();
			this.desTextField=new TextField();
			this.viewUI.addChild(desTextField);
			this.desTextField.x=19;
			this.desTextField.y=179;
			this.desTextField.autoSize=TextFieldAutoSize.LEFT;
			this.desTextField.mouseEnabled=false;
			this.desTextField.width=120;
			this.desTextField.wordWrap=true;
			this.desTextField.defaultTextFormat=textFormat();
			this.desTextField.multiline=true;
			
			this.moneyTextField=new TextField();
			this.moneyContainer.addChild(this.moneyTextField);
			this.viewUI.addChild(this.moneyContainer);
			this.moneyContainer.x=18;
			this.moneyContainer.y=292;
			this.moneyTextField.autoSize=TextFieldAutoSize.LEFT;
			this.moneyTextField.mouseEnabled=false;
			this.moneyTextField.width=200;
			this.moneyTextField.wordWrap=false;
			this.moneyTextField.defaultTextFormat=textFormat();
			
			this.clearAllPage();
			(this.viewUI.txt_commit as TextField).text=dataDic[pageNum][7];
			(this.viewUI.txt_commit as TextField).mouseEnabled=false;
			(this.viewUI.txt_needGood as TextField).mouseEnabled=false;
			(this.viewUI.page_0 as MovieClip).gotoAndStop(1);
			(this.viewUI.page_0 as MovieClip).addEventListener(MouseEvent.CLICK,onPageClickHandler);
			(this.viewUI.page_1 as MovieClip).addEventListener(MouseEvent.CLICK,onPageClickHandler);
			(this.viewUI.page_2 as MovieClip).addEventListener(MouseEvent.CLICK,onPageClickHandler);
			(this.viewUI.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitHandler);
			for(var i:uint=0;i<2;i++){
				var mc:MovieClip=this.viewUI.getChildByName("DK_"+i) as MovieClip;
				mc.addEventListener(MouseEvent.ROLL_OVER,onMouseOverHandler);
				mc.addEventListener(MouseEvent.ROLL_OUT,onMouseOutHandler);
			}
			this.changeDes();
		}
		
		/**
		 *  提交事件处理
		 * @param e
		 * 
		 */		
		protected function onCommitHandler(e:MouseEvent):void{
			var item0:UseItem=this.gridDic[pageNum][0] as UseItem;
			var item1:UseItem=this.gridDic[pageNum][1] as UseItem;
			var item2:UseItem=this.gridDic[pageNum][2] as UseItem;
			
			if(item0==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先放入你的装备", color:0xffff00}); 
				return;
			}else if(item1==null){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"放入的"+dataDic[pageNum][8]+"不能为空", color:0xffff00}); 
				return;
			}
			var param:Array;
			if(pageNum==0){
				param=[1,1,23,item0.Id,item1.Id];
				EquipSend.createMsgCompound(param);
				item1.IsLock=false;
				sendNotification(EventList.BAGITEMUNLOCK,item1.Id);
				this.viewUI.removeChild(dataDic[pageNum][1]);
				dataDic[pageNum][1]=null;
				this.gridDic[pageNum][1]=null;
			}else if(pageNum==1){	
				this.sendPage2Msg(item0,item1);
			}else if(pageNum==2){
				this.sendPage3Msg(item0,item1);
			}			
		}
		
		/**
		 *  
		 * @param item0 ：装备
		 * @param item1 ：镶嵌符
		 * 
		 */			
		private function sendPage2Msg(item0:UseItem,item1:UseItem):void{
			
			var item2:UseItem;
			for(var i:uint=2;i<5;i++){
				if(this.gridDic[pageNum][i] is UseItem){
					item2=this.gridDic[pageNum][i] as UseItem;  //宝石
					break;
				}
			}
			if(item2==null)return;
			if(item2.IsBind==1 && item0.IsBind!=1){
				sendNotification(EventList.SHOWALERT,{comfrim:sendPage2MsgData,cancel:new Function(),info:"镶嵌上绑定宝石后装备将自动变为绑定状态",params:{item0:item0,item1:item1}});
			}else{
				this.sendPage2MsgData({item0:item0,item1:item1});
			}
			
		}
		
		/**
		 *  发送镶嵌宝石命令
		 * @param obj
		 * 
		 */		
		private function sendPage2MsgData(obj:Object):void{
			var item0:UseItem=obj.item0;
			var item1:UseItem=obj.item1;
			var item2:UseItem;
			for(var i:uint=2;i<5;i++){
				if(this.gridDic[pageNum][i] is UseItem){
					item2=this.gridDic[pageNum][i] as UseItem;  //宝石
					break;
				}
			}
			
			var param:Array=[1,item1.Id,6,item0.Id,item2.Id];
			EquipSend.createMsgCompound(param);
				
			item1.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,item1.Id);
			this.viewUI.removeChild(dataDic[pageNum][1]);
			dataDic[pageNum][1]=null;
			this.gridDic[pageNum][1]=null;
				
			item2.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,item2.Id);
			if(dataDic[pageNum][i][1]!=null){
				this.viewUI.removeChild(dataDic[pageNum][i][1]);
			}
			
			dataDic[pageNum][i][1]=null;
			this.gridDic[pageNum][i]=null;
		}
		
		
		/**
		 * 取出消息发送 
		 * @param item0
		 * @param item1
		 * 
		 */		
		private function sendPage3Msg(item0:UseItem,item1:UseItem):void{
			
			//去掉取出符
			item1.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,item1.Id);
			this.viewUI.removeChild(dataDic[pageNum][1]);
			dataDic[pageNum][1]=null;
			this.gridDic[pageNum][1]=null;
		
			var param:Array=[1,item1.Id,70,item0.Id,(this.selectedIndex+1)];
			EquipSend.createMsgCompound(param);
			this.selectedIndex=0;
		}
			
		/**
		 * 鼠标移入事件格子处理 
		 * 
		 */		
		protected function onMouseOverHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.UseFrame(mc,"RedFrame");
		}
		
		/**
		 * 鼠标移出事件格子处理 
		 * 
		 */		
		protected function onMouseOutHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			SetFrame.RemoveFrame(mc.parent,"RedFrame");
		}
		
		/**
		 * 显示 
		 * 
		 */		
		protected function showView():void{
			dataProxy.EquipStilettoIsOpen=true;
			this.basePanel.x=DEFAULT_POS.x;
			this.basePanel.y=DEFAULT_POS.y;
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
			
		}
		
		/**
		 * 清空所有页签 
		 * 
		 */		
		protected function clearAllPage():void{
			(this.viewUI.page_0 as MovieClip).gotoAndStop(2);
			(this.viewUI.page_1 as MovieClip).gotoAndStop(2);
			(this.viewUI.page_2 as MovieClip).gotoAndStop(2);	
		}
		
		
		/**
		 * 设置当前要显示的面板 
		 * @param index: 0:打孔 1：镶嵌 2：摘取
		 * 
		 */		
		private function setSelectedPage(index:uint):void{
			this.clearAllPage();
			this.pageNum=index;
			this.viewUI["page_"+index].gotoAndStop(1);
			this.changeDes();
			this.changeShop();
		}
		
		
		/**
		 * 改变页签 
		 * @param e
		 * 
		 */		
		protected function onPageClickHandler(e:MouseEvent):void{
			this.clearAllPage();
			var s:String=e.currentTarget.name;
			this.pageNum=uint(s.substr(s.length-1,s.length));
			(e.currentTarget as MovieClip).gotoAndStop(1);
			this.changeDes();
			this.changeShop();
		}
		
		/**
		 * 动态改变显示内容 
		 * 
		 */		
		protected function changeDes():void{
			this.clearAllDes();
			(this.viewUI.txt_commit as TextField).text=dataDic[pageNum][7];
			(this.viewUI.txt_needGood as TextField).text=dataDic[pageNum][8];
			this.desTextField.htmlText=EquipDataConst.getInstance().getDesByLevel(10+pageNum);
			this.moneyTextField.htmlText=EquipDataConst.getInstance().getFeeDesByLevel(10+pageNum);
			ShowMoney.ShowIcon(this.moneyContainer,moneyTextField,true);
			var arr:Array=dataDic[pageNum] as Array;
			for(var i:uint=0;i<5;i++){
				if(arr[i]==null)continue;
				if(i==0 || i==1){
					this.viewUI.addChild(arr[i]);
				}else{
					for each(var s:DisplayObject in arr[i]){
						if(s==null)continue;
						this.viewUI.addChild(s);
					}
				}
			}
			
			if(pageNum==2){
				var dispalyObj:DisplayObject=this.stoneArr[this.selectedIndex] as DisplayObject;
				if(dispalyObj!=null){
					EquipDataConst.getInstance().setStoneSelected(dispalyObj as DisplayObjectContainer);
//					SetFrame.UseFrame(dispalyObj,"RedFrame");0
				}	
			}
			if(pageNum==0){   //打孔界面
				var eItem:UseItem=this.gridDic[pageNum][0] as UseItem;
				var str:String=EquipDataConst.getInstance().getDesByLevel(10+pageNum);
				if(eItem==null){
					str=str.replace("$",0);
				}else{
					var obj:Object=IntroConst.ItemInfo[eItem.Id];
					if(obj!=null){
						var array:Array=obj.stoneList as Array;
						for(var j:uint=0;j<3;j++){
							if(array[j]==99999){
								break;
							}
						}
						if(j==0){
							str=str.replace("$","100");
							this.moneyTextField.htmlText=EquipDataConst.getInstance().getFeeDesByLevel(10+pageNum);
						}else if(j==1){
							str=str.replace("$","50");
							this.moneyTextField.htmlText=EquipDataConst.getInstance().getFeeDesByLevel(13);
						}else if(j==2){
							str=str.replace("$","25");
							this.moneyTextField.htmlText=EquipDataConst.getInstance().getFeeDesByLevel(14);
						}else{
							str=str.replace("$","0");
						}
					}
				}
				this.desTextField.htmlText=str;
				
			}
		} 
		
		/**
		 * 
		 * 改变商站里的物品（根据当前页签） 
		 * 
		 */		
		private function changeShop():void{
			 this.clearShop();
			 var arr:Array=UIConstData.MarketGoodList[13+pageNum] as Array;
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
		 * 清除所有的描述
		 * 
		 */		
		protected function clearAllDes():void{
			this.desTextField.htmlText='<font color="#fffe65">请先放入你的装备</font>';
			this.moneyTextField.htmlText='<font color="#fffe65"碎银：</font>';
			ShowMoney.ShowIcon(this.moneyContainer,this.moneyTextField,true);
			this.clearAllYellowFrame();
			for(var j:uint=0;j<3;j++){
				var arr:Array=this.dataDic[j] as Array;
				for(var i:uint=0;i<5;i++){
					if(arr[i]!=null){
						if(i==0 || i==1){
							if(this.viewUI.contains(arr[i]))this.viewUI.removeChild(arr[i]);
						}else{
							for each(var s:DisplayObject in arr[i]){
								if(s!=null && this.viewUI.contains(s)){
									this.viewUI.removeChild(s);
								}
							}
						}		
					}
				}
			}
		}
		
		/**
		 *  购买商品 
		 * @param e
		 * 
		 */		
		private  function buyGoodClickHandler(e:MouseEvent):void{
			var arrShop:Array=UIConstData.MarketGoodList[13+pageNum] as Array;
			var arr:Array=e.currentTarget.parent.name.split("_");
			if(arrShop.length>arr[1]){
				sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:arrShop[arr[1]].type});	
			}
		}
		
		
		
		/**
		 * 关闭事件处理 
		 * @param e
		 * 
		 */		
		protected function onClosePanelHandler(e:Event):void{
			
			dataProxy.EquipStilettoIsOpen=false;
			EquipDataConst.getInstance().closePanel();
			this.clearAllDes();
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
				
			dataDic[0]=[null,null,null,null,null,0,0,"打  孔","打孔道具"];
			dataDic[1]=[null,null,null,null,null,0,0,"镶  嵌","镶嵌宝符"];
			dataDic[2]=[null,null,null,null,null,0,0,"摘  取","摘取宝符"];
			
			for each(var arr:Array in this.gridDic){
				for each(var u:* in arr){
					if(u is UseItem){
						this.cancelLock(u);
						
					}
				}
			}
			gridDic[0]=new Array(9);
			gridDic[1]=new Array(9);
			gridDic[2]=new Array(9);
		}
		
		/**
		 * 取消背包中对该物品的锁定 
		 * @param u
		 * 
		 */		
		private function cancelLock(u:UseItem):void{
			u.IsLock=false;
			sendNotification(EventList.BAGITEMUNLOCK,u.Id);
		}
	}
}