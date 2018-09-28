package GameUI.Modules.Equipment.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.Equipment.ui.AddStarPanel;
	import GameUI.Modules.Equipment.ui.EnchasePanel;
	import GameUI.Modules.Equipment.ui.EquipCell;
	import GameUI.Modules.Equipment.ui.ExtirpatePanel;
	import GameUI.Modules.Equipment.ui.HunYunPanel;
	import GameUI.Modules.Equipment.ui.ItemCell;
	import GameUI.Modules.Equipment.ui.StilettoPanel;
	import GameUI.Modules.Equipment.ui.StoneComposePanel;
	import GameUI.Modules.Equipment.ui.StoneDecoratePanel;
	import GameUI.Modules.Equipment.ui.StrengenPanel;
	import GameUI.Modules.Equipment.ui.UIDataGrid;
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
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
	 *  装备玩法
	 * @author felix
	 * 
	 */	
	public class EquipmentMediator extends Mediator
	{
		
		public static var NAME:String="EquipmentMediator";
		
		//////////////////////////////////////////////////////
		//装备筛选数据
		protected var allEquips:Array=[[]];
		protected var whiteEquips:Array=[[]];
		protected var greenEquips:Array=[[]];
		protected var buleEquips:Array=[[]];
		protected var purpleEquips:Array=[[]];
		protected var orangeEquips:Array=[[]];
		//--------------------------------------------------
		
		////////////////////////////////////////////////////
		//道具筛选数据
		protected var strengenItems:Array=[[]];
		protected var addStarItems:Array=[[]];
		protected var stilettoItems:Array=[[]];
		protected var enchaseItems:Array=[[]];
		protected var extirpateItems:Array=[[]];
		protected var hunYunItems:Array=[[]];
		protected var stoneComposeItems:Array=[[]];
		protected var stoneDecorateItems:Array=[[]];
		
		//--------------------------------------------------
		
		///////////////////////////////////////////////////////////////////
		//金钱
		protected var needMoney:MoneyItem;
		protected var moneyBind:MoneyItem;
		protected var moneyRmb:MoneyItem; 	
		//-------------------------------------------------------------------
		protected var dataProxy:DataProxy;
		
		protected var basePanel:PanelBase;
		/** 装备格子  */
		protected var equipDataGrid:UIDataGrid;
		/** 道具格子  */
		protected var itemDataGrid:UIDataGrid;
		/** 强化面板  */
		protected var strengenPanel:StrengenPanel;
		/** 升星面板  */
		protected var addStarPanel:AddStarPanel;	
		/** 装备打孔面板   */
		protected var stilettoPanel:StilettoPanel;	
		/** 装备镶嵌界面   */
		protected var enchagePanel:EnchasePanel;
		/** 宝石摘取界面   */
		protected var extirpatePanel:ExtirpatePanel;
		/** 魂印界面  */
		protected var hunYunPanel:HunYunPanel;
		/** 宝石合成界面 */
		protected var stoneComposePanel:StoneComposePanel;
		/** 宝石雕琢界面 */
		protected var stoneDecoratePanel:StoneDecoratePanel;
		/** 装备颜色选择界面 */
		protected var equipColorSelectedIndex:uint=0;
		/** 页面选择索引  */
		protected var pageSelectedIndex:int=-1;
		/** 当前显示的面板 */
		protected var currentPanel:DisplayObject;
		
		protected var doFirst:Boolean;
		
		
		public function EquipmentMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get view():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				EventList.ENTERMAPCOMPLETE,
				EquipCommandList.SHOW_COMBOX_LIST,
				EquipCommandList.SHOW_EQUIPSTRENGEN_UI,
				EquipCommandList.RETURN_EQUIP_INFO,
				EquipCommandList.REFRESH_HELP_ITEM,
				EquipCommandList.REFRESH_EQUIP,
				EventList.UPDATEMONEY,
				EquipCommandList.UPDATE_NEEDMONEY_EQUIP,
				EquipCommandList.ADD_EQUIP_ITEM,
				EquipCommandList.DROP_EQUIP_ITEM,
				EquipCommandList.CLOSE_EQUIP_PANEL,
				EquipCommandList.ADD_SPECIAL_SHOW
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"EquipPanel"});
					this.basePanel=new PanelBase(this.view,this.view.width+8,this.view.height+9);
					
					this.basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_equ_med_equ_han_1" ]);//"装 备"
					this.basePanel.addEventListener(Event.CLOSE,onPanelCloseHandler);
					
					this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
										
					(this.view.txt_itemDes as TextField).mouseEnabled = false;
					
					(this.view.btn_all as SimpleButton).addEventListener(MouseEvent.CLICK,onBtnColorClick);
					(this.view.btn_white as SimpleButton).addEventListener(MouseEvent.CLICK,onBtnColorClick);
					(this.view.btn_green as SimpleButton).addEventListener(MouseEvent.CLICK,onBtnColorClick);
					(this.view.btn_bule as SimpleButton).addEventListener(MouseEvent.CLICK,onBtnColorClick);
					(this.view.btn_purplo as SimpleButton).addEventListener(MouseEvent.CLICK,onBtnColorClick);
					(this.view.btn_orange as SimpleButton).addEventListener(MouseEvent.CLICK,onBtnColorClick);
					
					(this.view.page_1 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_2 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_3 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_4 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_5 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_6 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_7 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);
					(this.view.page_8 as MovieClip).addEventListener(MouseEvent.CLICK,onPageMouseClick);	//宝石雕琢
					
					
					this.equipDataGrid=new UIDataGrid(325,166);
					this.equipDataGrid.addEventListener(GridCellEvent.GRIDCELL_CLICK,onEquipGridCellClick);
					this.equipDataGrid.rendererClass=EquipCell;
					this.equipDataGrid.x=328;
					this.equipDataGrid.y=74;
					this.view.addChild(this.equipDataGrid);

										
					this.itemDataGrid=new UIDataGrid(324,166);
					this.itemDataGrid.addEventListener(GridCellEvent.GRIDCELL_CLICK,onItemGridCellClick);
					this.itemDataGrid.rendererClass=ItemCell;
					this.itemDataGrid.x=330;
					this.itemDataGrid.y=263;
					this.view.addChild(this.itemDataGrid);

					this.strengenPanel=new StrengenPanel();
					facade.registerMediator(new StrengenMediator(null,this.strengenPanel));
					this.strengenPanel.x=7;
					this.strengenPanel.y=-7;
					
					this.addStarPanel=new AddStarPanel();
					facade.registerMediator(new AddStarMediator(null,this.addStarPanel));
					this.addStarPanel.x=7;
					this.addStarPanel.y=-7;
					
					this.stilettoPanel=new StilettoPanel();
					facade.registerMediator(new StilettoMediator(null,this.stilettoPanel));
					this.stilettoPanel.x=7;
					this.stilettoPanel.y=-7;
					
					this.enchagePanel=new EnchasePanel();
					facade.registerMediator(new EnchaseMediator(null,this.enchagePanel));
					this.enchagePanel.x=7;
					this.enchagePanel.y=-7;
					
					this.extirpatePanel=new ExtirpatePanel();
					facade.registerMediator(new ExtirpateMediator(null,this.extirpatePanel));
					this.extirpatePanel.x=7;
					this.extirpatePanel.y=-7;
					
					this.hunYunPanel=new HunYunPanel();
					facade.registerMediator(new HunYunMediator(null,this.hunYunPanel));
					this.hunYunPanel.x=7;
					this.hunYunPanel.y=-7;
					
					this.stoneComposePanel=new StoneComposePanel();
					facade.registerMediator(new StoneComMediator(null,this.stoneComposePanel));
					this.stoneComposePanel.x=7;
					this.stoneComposePanel.y=-7; 
					
					this.stoneDecoratePanel=new StoneDecoratePanel();
					facade.registerMediator(new StoneDecorateMediator(null,this.stoneDecoratePanel));
					this.stoneDecoratePanel.x=7;
					this.stoneDecoratePanel.y=-7; 
					
					this.needMoney=new MoneyItem();
					this.needMoney.x=150;
					this.needMoney.y=368;
					this.view.addChild(this.needMoney);
					this.moneyBind =new MoneyItem();
					this.moneyBind.x=150;
					this.moneyBind.y=390;
					this.view.addChild(this.moneyBind);
					this.moneyRmb  =new MoneyItem();
					this.moneyRmb.x=150;
					this.moneyRmb.y=413;
					this.view.addChild(this.moneyRmb);
					GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_DOWN,onStageClick);					
					break;
				case EventList.ENTERMAPCOMPLETE:
					
					break;
				
				//打开装备强化界面	
				case EquipCommandList.SHOW_EQUIPSTRENGEN_UI:


					if(dataProxy.equipPanelIsOpen){
						onPanelCloseHandler(null);
						return;
					}
					if(this.dataProxy.TradeIsOpen){
						//todo交易中
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_equ_han_2" ], color:0xffff00});//"交易中不能对装备进行操作"
						return;
					}
					if(this.dataProxy.StallIsOpen){
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_equ_han_3" ], color:0xffff00});//"摆摊中不能对装备进行操作"
						return;
					}
					if(dataProxy.DepotIsOpen) {//关掉仓库
						sendNotification(EventList.CLOSEDEPOTVIEW);
					}
					if(dataProxy.NPCShopIsOpen) {//关掉NPC商店
						sendNotification(EventList.CLOSENPCSHOPVIEW);
					}
					
					var n:uint=uint(notification.getBody());
					this.dataProxy.equipPanelIsOpen=true;
					this.setSelectedPage(n);
					this.setColorBtn();
					if( GameCommonData.fullScreen == 2 )
					{
						this.basePanel.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - basePanel.width)/2;;
						this.basePanel.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - basePanel.height)/2;;
					}else{
						this.basePanel.x = 150;
						this.basePanel.y = 50;
					}
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);	 
					break;
				//收到装备的详细信息（与背包同步，清除锁定装备）	
				case EquipCommandList.RETURN_EQUIP_INFO:
					if(this.view.stage==null)return;
					this.checkEquip();
					this.refresh();
					break;
				//显示ComboxList(因为要显示在最上层)				
				case EquipCommandList.SHOW_COMBOX_LIST:
					var list:DisplayObject=notification.getBody() as DisplayObject;
					var displayObj:DisplayObject=this.view.getChildByName("LIST");
					if(displayObj!=null){
						this.view.removeChild(displayObj);
					}else{
						this.view.addChild(list);
					}
					break;	
				//刷新装备信息	
				case EquipCommandList.REFRESH_HELP_ITEM:
					this.checkItems();
					this.refreshItems();
					break;
				//刷新装备信息	
				case EquipCommandList.REFRESH_EQUIP:
					this.equipDataGrid.refresh();
					break;
				case EventList.UPDATEMONEY:															//更新钱
					switch (notification.getBody().target){
						case "mcBind":
							this.moneyBind.update(notification.getBody().money);
							break;
						case "mcUnBind":
							this.moneyRmb.update(notification.getBody().money);
							break;
						default:			
							break;
					}
					break;	
				case EquipCommandList.UPDATE_NEEDMONEY_EQUIP:
//					this.needMoney.update(String(notification.getBody()));
					this.needMoney.update(UIUtils.getMoneyStandInfo( int(notification.getBody()), ["\\se","\\ss","\\sc"]));
					break;
					
				//增加装备 
				case EquipCommandList.ADD_EQUIP_ITEM:
					if(this.view.parent==null)return;
					var addObj:Object=notification.getBody();
					if(IntroConst.ItemInfo[addObj.id]==null){
						UiNetAction.GetItemInfo(addObj.id, GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
					}else{
						sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
					}
					break;
				//装备掉落	
				case EquipCommandList.DROP_EQUIP_ITEM:
					if(this.view.parent==null)return;
					sendNotification(EquipCommandList.RETURN_EQUIP_INFO);
					break;
				case EquipCommandList.CLOSE_EQUIP_PANEL:
					this.onPanelCloseHandler(null);
					break;		
				case EquipCommandList.ADD_SPECIAL_SHOW:
					addSpecialShow(notification.getBody() as Point);
					break;
					
			}		
		}
		
		
		protected function setSelectedPage(value:int):void{
			if(this.pageSelectedIndex==value)return;
			
			(this.view.page_1 as MovieClip).gotoAndStop(2);
			(this.view.page_2 as MovieClip).gotoAndStop(2);
			(this.view.page_3 as MovieClip).gotoAndStop(2);
			(this.view.page_4 as MovieClip).gotoAndStop(2);
			(this.view.page_5 as MovieClip).gotoAndStop(2);
			(this.view.page_6 as MovieClip).gotoAndStop(2);
			(this.view.page_7 as MovieClip).gotoAndStop(2);
			(this.view.page_8 as MovieClip).gotoAndStop(2);
			if(value==0){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_1" ];//"强化材料"
				(this.view.page_1 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=0){
					this.pageSelectedIndex=0;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.strengenPanel;	
					this.view.bgContainer.addChild(this.strengenPanel);
				}
			}else if(value==1){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_2" ];//"升星材料"
				(this.view.page_2 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=1){
					this.pageSelectedIndex=1;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.addStarPanel;	
					this.view.bgContainer.addChild(this.addStarPanel);
				}
			}else if(value==2){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_3" ];//"合成材料"
				(this.view.page_3 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=2){
					this.pageSelectedIndex=2;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.stoneComposePanel;
					this.view.bgContainer.addChild(this.stoneComposePanel);
				}
			}else if(value==3){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_4" ];//"打孔材料"
				(this.view.page_4 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=3){
					this.pageSelectedIndex=3;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.stilettoPanel;	
					this.view.bgContainer.addChild(this.stilettoPanel);
				}
			}else if(value==4){
//				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_5" ];/"镶嵌材料"
				(this.view.page_5 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=4){
					this.pageSelectedIndex=4;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.enchagePanel;	
					this.view.bgContainer.addChild(this.enchagePanel);
				}
			}else if(value==5){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_6" ];//"摘取材料"
				(this.view.page_6 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=5){
					this.pageSelectedIndex=5;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.extirpatePanel;
					this.view.bgContainer.addChild(this.extirpatePanel);
				}
			}else if(value==6){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equ_med_equ_setS_7" ];//"魂印材料"
				(this.view.page_7 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=6){
					this.pageSelectedIndex=6;
					if(this.currentPanel!=null){
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel=this.hunYunPanel;
					this.view.bgContainer.addChild(this.hunYunPanel);
				}
			}else if(value==7){
				this.view.txt_itemDes.text = GameCommonData.wordDic[ "mod_equip_med_equip_setS" ];//"雕琢材料";
				(this.viewComponent.page_8 as MovieClip).gotoAndStop(1);
				if(this.pageSelectedIndex!=7){
					this.pageSelectedIndex=7;
					if(this.currentPanel!=null)
					{
						this.view.bgContainer.removeChild(this.currentPanel);
					}
					this.currentPanel = this.stoneDecoratePanel;
					this.view.bgContainer.addChild(this.stoneDecoratePanel);
				}
			}
			
			this.checkEquip();
			this.refresh();
			EquipDataConst.getInstance().lockItems=new Dictionary();
			this.checkItems();
			this.refreshItems();
			this.changeDes();				
		}
		
		
		protected function onItemGridCellClick(e:GridCellEvent):void{
			
			if(this.pageSelectedIndex==0){
				if(e.data.detailData.type==610048){
					sendNotification(EquipCommandList.ADD_STRENGEN_ITEM,e.data.detailData);
				}
			}else if(this.pageSelectedIndex==1){
				if(e.data.detailData.type==610048){
					sendNotification(EquipCommandList.ADD_STAR_ITEM,e.data.detailData);
				}
			}else if(this.pageSelectedIndex==2){
				if(e.data.detailData.type==610016 || e.data.detailData.type==610017){
					sendNotification(EquipCommandList.ADD_STONECOMPOSE_ITEM,e.data.detailData);
				}else{
					sendNotification(EquipCommandList.ADD_STONECOMPOSE_STONE,e.data.detailData);
				}
			}else if(this.pageSelectedIndex==3){
				this.sendNotification(EquipCommandList.ADD_STILETTO_ITEM,e.data.detailData);
			}else if(this.pageSelectedIndex==4){
				if(e.data.detailData.type==610018){
					sendNotification(EquipCommandList.ADD_ENCHASE_ITEM,e.data.detailData);
				}else{
					sendNotification(EquipCommandList.ADD_ENCHASE_STONE,e.data.detailData);
				}
			}else if(this.pageSelectedIndex==5){
				if(e.data.detailData.type==610019){
					sendNotification(EquipCommandList.ADD_EXTIRPATE_ITEM,e.data.detailData);
				}
			}else if(this.pageSelectedIndex==7){ 	//宝石雕琢
				sendNotification(EquipCommandList.ADD_DECORATE_ITEM,e.data.detailData);
			}
			
		}
		
		
		protected function onEquipGridCellClick(e:GridCellEvent):void{
			if(this.pageSelectedIndex==2 ){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_equ_onE" ], color:0xffff00});//"宝石合成将不能使用装备"
				return;
			} 
			if(this.pageSelectedIndex==7)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equip_med_equip_onE" ], color:0xffff00});//"宝石雕琢将不能使用装备"
				return;
			}                     
			
			var obj:Object=e.data;
			this.checkEquip();
			var resultObj:Object=this.getInfoById(allEquips,obj.id);
			if(resultObj!=null)resultObj.isLock=true;
			resultObj=this.getInfoById(whiteEquips,obj.id);
			if(resultObj!=null)resultObj.isLock=true;
			resultObj=this.getInfoById(greenEquips,obj.id);
			if(resultObj!=null)resultObj.isLock=true;
			resultObj=this.getInfoById(buleEquips,obj.id);
			if(resultObj!=null)resultObj.isLock=true;
			resultObj=this.getInfoById(purpleEquips,obj.id);
			if(resultObj!=null)resultObj.isLock=true;
			resultObj=this.getInfoById(orangeEquips,obj.id);
			if(resultObj!=null)resultObj.isLock=true;
			this.refresh();	
			if(this.pageSelectedIndex==0){
				sendNotification(EquipCommandList.ADD_STRENGEN_EQUIP,obj.detailData); 
			}else if(this.pageSelectedIndex==1){
				sendNotification(EquipCommandList.ADD_ADDSTAR_EQUIP,obj.detailData);
			}else if(this.pageSelectedIndex==2){
				//不上装备 -
			}else if(this.pageSelectedIndex==3){
				sendNotification(EquipCommandList.ADD_STILETTO_EQUIP,obj.detailData);
			}else if(this.pageSelectedIndex==4){
				sendNotification(EquipCommandList.ADD_ENCHASE_EQUIP,obj.detailData); 
			}else if(this.pageSelectedIndex==5){
				sendNotification(EquipCommandList.ADD_EXTIRPATE_EQUIP,obj.detailData);
			}else if(this.pageSelectedIndex==6){
				sendNotification(EquipCommandList.ADD_HUNYUN_EQUIP,obj.detailData);
			}		
		}
		
		
		protected function refresh():void{
			if(this.equipColorSelectedIndex==0){
				this.equipDataGrid.dataPro=allEquips;
			}else if(this.equipColorSelectedIndex==1){
				this.equipDataGrid.dataPro=whiteEquips;
			}else if(this.equipColorSelectedIndex==2){
				this.equipDataGrid.dataPro=greenEquips;
			}else if(this.equipColorSelectedIndex==3){
				this.equipDataGrid.dataPro=buleEquips;
			}else if(this.equipColorSelectedIndex==4){
				this.equipDataGrid.dataPro=purpleEquips;
			}else if(this.equipColorSelectedIndex==5){
				this.equipDataGrid.dataPro=orangeEquips;
			}
		}
		
		
		protected function refreshItems():void{
			if(this.pageSelectedIndex==0){
				this.itemDataGrid.dataPro=this.strengenItems;
			}else if(this.pageSelectedIndex==1){
				this.itemDataGrid.dataPro=this.addStarItems;
			}else if(this.pageSelectedIndex==2){
				this.itemDataGrid.dataPro=this.stoneComposeItems;
			}else if(this.pageSelectedIndex==3){
				this.itemDataGrid.dataPro=this.stilettoItems;
			}else if(this.pageSelectedIndex==4){
				this.itemDataGrid.dataPro=this.enchaseItems;
			}else if(this.pageSelectedIndex==5){
				this.itemDataGrid.dataPro=this.extirpateItems;				
			}else if(pageSelectedIndex==6){
				this.itemDataGrid.dataPro=this.hunYunItems;
			}else if(pageSelectedIndex==7){
				this.itemDataGrid.dataPro=this.stoneDecorateItems;
			}
		}
		
		
		
		protected function getInfoById(arr:Array,id:uint):Object{
			var len1:uint=arr.length;
			if(len1==0)return null;
			for each(var a:Array in arr){
				for each(var obj:* in a){
					if(obj.id==id)return obj;
				}
			}
			return null;
		}
		
		
		/**
		 * 点击颜色按钮事件处理 
		 * @param e
		 * 
		 */		
		protected function onBtnColorClick(e:MouseEvent):void{
			(this.view.btn_all as SimpleButton).visible=true;
			(this.view.btn_white as SimpleButton).visible=true;
			(this.view.btn_green as SimpleButton).visible=true;
			(this.view.btn_bule as SimpleButton).visible=true;
			(this.view.btn_purplo as SimpleButton).visible=true;
			(this.view.btn_orange as SimpleButton).visible=true;
			
			
			if(e.target===this.view.btn_all){
				this.equipColorSelectedIndex=0;
				this.equipDataGrid.dataPro=allEquips;
				(this.view.btn_all as SimpleButton).visible=false;
			}else if(e.target===this.view.btn_white){
				this.equipColorSelectedIndex=1;
				this.equipDataGrid.dataPro=whiteEquips;
				(this.view.btn_white as SimpleButton).visible=false;
			}else if(e.target===this.view.btn_green){
				this.equipColorSelectedIndex=2;
				this.equipDataGrid.dataPro=greenEquips;
				(this.view.btn_green as SimpleButton).visible=false;
			}else if(e.target===this.view.btn_bule){
				this.equipColorSelectedIndex=3;
				this.equipDataGrid.dataPro=buleEquips;
				(this.view.btn_bule as SimpleButton).visible=false;
			}else if(e.target===this.view.btn_purplo){
				this.equipColorSelectedIndex=4;
				this.equipDataGrid.dataPro=purpleEquips;
				(this.view.btn_purplo as SimpleButton).visible=false;
			}else if(e.target===this.view.btn_orange){
				this.equipColorSelectedIndex=5;
				this.equipDataGrid.dataPro=orangeEquips;
				(this.view.btn_orange as SimpleButton).visible=false;
			}
		}
		
		/**
		 * 设置颜色装备选项 
		 * @param type
		 * 
		 */		
		protected function setColorBtn(type:uint=0):void{
			(this.view.btn_all as SimpleButton).visible=true;
			(this.view.btn_white as SimpleButton).visible=true;
			(this.view.btn_green as SimpleButton).visible=true;
			(this.view.btn_bule as SimpleButton).visible=true;
			(this.view.btn_purplo as SimpleButton).visible=true;
			(this.view.btn_orange as SimpleButton).visible=true;
			
			this.checkEquip();
			this.equipColorSelectedIndex=0;
			this.equipDataGrid.dataPro=allEquips;
			(this.view.btn_all as SimpleButton).visible=false;7
		}
		
		
		/**
		 * 点击页面选择按钮 
		 * @param e
		 * 
		 */		
		protected function onPageMouseClick(e:MouseEvent):void{
			
			
			if(e.target===this.view.page_1){
				this.setSelectedPage(0);
			}else if(e.target===this.view.page_2){
				this.setSelectedPage(1);
			}else if(e.target===this.view.page_3){
				this.setSelectedPage(2);
			}else if(e.target===this.view.page_4){
				this.setSelectedPage(3);
			}else if(e.target===this.view.page_5){
				this.setSelectedPage(4);
			}else if(e.target===this.view.page_6){
				this.setSelectedPage(5);
			}else if(e.target===this.view.page_7){
				this.setSelectedPage(6);
			}else if(e.target===this.view.page_8){
				this.setSelectedPage(7);
			}
				
		}
		
		/** 
		 *  筛选所有的装备
		 * 
		 */		
		protected function checkEquip():void{
			this.allEquips=[[]];    //全部
			this.whiteEquips=[[]];
			this.greenEquips=[[]];
			this.buleEquips=[[]];
			this.purpleEquips=[[]];
			this.orangeEquips=[[]];
			var arr:Array=BagData.AllItems[0] as Array;
			arr=arr.concat(RolePropDatas.ItemList);
			var count:uint=0;
			var countWhite:uint=0;
			var countGreen:uint=0;
			var countBule:uint=0;
			var countPurplo:uint=0;
			var countOrange:uint=0;
			 if(this.doFirst){
				if(this.pageSelectedIndex==0){
					arr=this.sortStrengen(arr);
				}else if(this.pageSelectedIndex==1){
					arr=this.sortAddStar(arr);
				}else if(this.pageSelectedIndex==3){
					arr=this.sortStilletto(arr);
				}else if(this.pageSelectedIndex==4){
					arr=this.sortEnchase(arr);
				}else if(this.pageSelectedIndex==5){
					arr=this.sortExtirpate(arr);	
				}else if(this.pageSelectedIndex==6){  //魂印排序
					arr=this.sortHunYun(arr);
				}else if(this.pageSelectedIndex==7){	//宝石雕琢排序
					arr=this.sortEnchase(arr);
				}
			} 
			var len:uint=arr.length;
			for(var i:uint=0;i<len;i++){
				var obj:Object=arr[i];
				if(obj==null)continue;
				if ( this.pageSelectedIndex==3 || this.pageSelectedIndex==4 || this.pageSelectedIndex == 0 || this.pageSelectedIndex==5 )
				{
					if(!EquipDataConst.getInstance().isStilleEquip(obj.type))
					{
						continue;
					}
				}
				else
				{
					if(!EquipDataConst.getInstance().isEquip(obj.type))
					{
						continue;
					}
				}
				if(IntroConst.ItemInfo[obj.id]==null){
					if(!doFirst){
						UiNetAction.GetItemInfo(obj.id, GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
						if ( obj.type >=250000 && obj.type < 300000 )
						{
							SoulProxy.getSoulDetailInfoFromBag( obj.id );
						}
					}
				}else{
					if(allEquips[Math.floor(count/2)]==null)allEquips[Math.floor(count/2)]=[];
					allEquips[Math.floor(count/2)][count%2]={detailData:IntroConst.ItemInfo[obj.id],id:obj.id,isLock:false,page:this.pageSelectedIndex};
					count++;
					if(IntroConst.ItemInfo[obj.id].color==0 || IntroConst.ItemInfo[obj.id].color==1){
						if(whiteEquips[Math.floor(countWhite/2)]==null)whiteEquips[Math.floor(countWhite/2)]=[];
						whiteEquips[Math.floor(countWhite/2)][countWhite%2]={detailData:IntroConst.ItemInfo[obj.id],id:obj.id,isLock:false,page:this.pageSelectedIndex};
						countWhite++;
					}else if(IntroConst.ItemInfo[obj.id].color==2){
						if(greenEquips[Math.floor(countGreen/2)]==null)greenEquips[Math.floor(countGreen/2)]=[];
						greenEquips[Math.floor(countGreen/2)][countGreen%2]={detailData:IntroConst.ItemInfo[obj.id],id:obj.id,isLock:false,page:this.pageSelectedIndex};
						countGreen++;
					}else if(IntroConst.ItemInfo[obj.id].color==3){
						if(buleEquips[Math.floor(countBule/2)]==null)buleEquips[Math.floor(countBule/2)]=[];
						buleEquips[Math.floor(countBule/2)][countBule%2]={detailData:IntroConst.ItemInfo[obj.id],id:obj.id,isLock:false,page:this.pageSelectedIndex};
						countBule++;
					}else if(IntroConst.ItemInfo[obj.id].color==4){
						if(purpleEquips[Math.floor(countPurplo/2)]==null)purpleEquips[Math.floor(countPurplo/2)]=[];
						purpleEquips[Math.floor(countPurplo/2)][countPurplo%2]={detailData:IntroConst.ItemInfo[obj.id],id:obj.id,isLock:false,page:this.pageSelectedIndex};
						countPurplo++;
					}else if(IntroConst.ItemInfo[obj.id].color==5){
						if(orangeEquips[Math.floor(countOrange/2)]==null)orangeEquips[Math.floor(countOrange/2)]=[];
						orangeEquips[Math.floor(countOrange/2)][countOrange%2]={detailData:IntroConst.ItemInfo[obj.id],id:obj.id,isLock:false,page:this.pageSelectedIndex};
						countOrange++;
					}
				}
			}
			this.doFirst=true;							
		}
		
		
		/**
		 * 装备强化排序 
		 * @param value
		 * @return 
		 * 
		 */		
		protected function sortStrengen(value:Array):Array{
			var source:Array=value;
			var target:Array=[];
		 	var len:int=source.length;
		 	
		 	for(var index:int=0;index<len;index++){
		 		var obj:Object=source[index];
				if(obj==null)continue;
				if(!EquipDataConst.getInstance().isEquip(obj.type))continue;
				if(IntroConst.ItemInfo[obj.id]==null)continue;
				var targetObj:Object=IntroConst.ItemInfo[obj.id];
				target.push(targetObj);
		 	}
		 	var len2:int = target.length;
		 	for(var i:int=0;i<len2;i++){
		 		var tempData:Object=target[i];
		 		for(var j:int=i;j>0;j--){
		 			if(tempData.level > target[j-1].level){
		 				target[j]=target[j-1];
		 				target[j-1]=tempData;
		 			}
		 		}
		 	}
			return target;
		}
		
		
		/**
		 * 升星排序
		 * @param value
		 * @return 
		 * 
		 */		
		protected function sortAddStar(value:Array):Array{
			var source:Array=value;
			var target:Array=[];
			var len:int=source.length;
			for(var index:int=0;index<len;index++){
		 		var obj:Object=source[index];
				if(obj==null)continue;
				if(!EquipDataConst.getInstance().isEquip(obj.type))continue;
				if(IntroConst.ItemInfo[obj.id]==null)continue;
				var targetObj:Object=IntroConst.ItemInfo[obj.id];
				target.push(targetObj);
		 	}
			var len2:int=target.length;
			for(var i:int=0;i<len2;i++){
				var num:int=i;
				for(var j:int=num+1;j<len2;j++){
					if(target[num].star < target[j].star){
						num=j;
					}
				}
				var tempData:Object=target[num];
				target[num]=target[i];
				target[i]=tempData;
			}
			return target;
		}
		
		/**
		 *宝石摘取排序 
		 * @param value
		 * @return 
		 * 
		 */		
		 protected function sortExtirpate(value:Array):Array{
		 	var source:Array=value;
		 	var target:Array=[];
		 	var len:int=source.length;
		 	var tempArr:Array=[];
		 	for(var index:int=0;index<len;index++){
		 		var obj:Object=source[index];
		 		if(obj==null)continue;
		 		if(!EquipDataConst.getInstance().isStilleEquip(obj.type))continue; 
		 		if(IntroConst.ItemInfo[obj.id]==null)continue;
		 		var targetObj:Object=IntroConst.ItemInfo[obj.id];
		 		var count:int=0;
		 		for each(var data:uint in targetObj.stoneList){
		 			if(data!=99999 && data!=0 && data!=88888){
		 				count++;
		 			}
		 		}
		 		tempArr.push({number:count,data:obj});
		 	}
		 	tempArr.sortOn("number",Array.DESCENDING);
		 	var len2:int=tempArr.length;
		 	for(var z:int=0;z<len2;z++){
		 		target.push(tempArr[z].data);
		 	}
		 	return target;
		 }
		 
		/**
		 * 打孔排序 
		 * @param value
		 * @return 
		 * 
		 */		
		protected function sortStilletto(value:Array):Array{
			var source:Array=value;
			var target:Array=[];
			var tempArr0:Array=[];
			var tempArr1:Array=[];
			var tempArr2:Array=[]; 
			var tempArr3:Array=[];
			var len:uint=source.length;
			for(var i:uint=0;i<len;i++){
				var obj:Object=source[i];
				if(obj==null)continue;
//				if(!EquipDataConst.getInstance().isEquip(obj.type))continue;
				if(!EquipDataConst.getInstance().isStilleEquip(obj.type))continue;
				if(IntroConst.ItemInfo[obj.id]==null)continue;
				var targetObj:Object=IntroConst.ItemInfo[obj.id];
				var count:int=0;
				for each(var num:uint in targetObj.stoneList){
					if(num != 99999 && num != 0){
						count ++;
					}
				}
				if(count == 0){
					tempArr0.push(obj)
				}
				else if(count==1){
					tempArr1.push(obj);
				}
				else if(count==2){
					tempArr2.push(obj);
				}
				else if(count==3){
					tempArr3.push(obj);
				}
			}
			target=tempArr2.concat(tempArr1).concat(tempArr0).concat(tempArr3);
			return target;
		}
		
		/**
		 *镶嵌排序 
		 * @param value
		 * @return 
		 * 
		 */		
		protected function sortEnchase(value:Array):Array{
			var source:Array=value;
			var target:Array=[];
			var len:int=source.length;
			var arr3_4:Array=[];
			var arr2_4:Array=[];
			var arr1_4:Array=[];
			var arr0_4:Array=[];
			var arr2_3:Array=[];
			var arr1_3:Array=[];
			var arr0_3:Array=[];
			var arr1_2:Array=[];
			var arr0_2:Array=[];
			var arr0_1:Array=[];
			var arr1_1:Array=[];
			var arr2_2:Array=[];
			var arr3_3:Array=[];
			var arr4_4:Array=[];
			var arr0_0:Array=[];
			for(var i:int=0;i<len;i++){
				var obj:Object=source[i];
				if(obj==null)continue;
//				if(!EquipDataConst.getInstance().isEquip(obj.type))continue;
				if(!EquipDataConst.getInstance().isStilleEquip(obj.type))continue;
				if(IntroConst.ItemInfo[obj.id]==null)continue;
				var targetObj:Object=IntroConst.ItemInfo[obj.id];
				var noUseNum:int=0;
				var useNum:int=0;
				for each(var data:uint in targetObj.stoneList){
					if(data!=99999 && data!=0){
						noUseNum++;
						if(data!=88888){
							useNum++;
						}  
					}
				}
				if(useNum==4 && noUseNum==4){
					arr4_4.push(obj);
				}
				if(useNum==3 && noUseNum==4)
				{
					arr3_4.push(obj);
				}
				else if(useNum==2 && noUseNum==4){
					arr2_4.push(obj);
				}
				else if(useNum==1 && noUseNum==4){
					arr1_4.push(obj);
				}
				else if(useNum==0 && noUseNum==4){
					arr0_4.push(obj);
				}
				else if(useNum==2 && noUseNum==3){
					arr2_3.push(obj);
				}
				else if(useNum==1 && noUseNum==3){
					arr1_3.push(obj);
				}
				else if(useNum==0 && noUseNum==3){
					arr0_3.push(obj);
				}
				else if(useNum==1 && noUseNum==2){
					arr1_2.push(obj);
				}
				else if(useNum==0 && noUseNum==2){
					arr0_2.push(obj);
				}
				else if(useNum==0 && noUseNum==1){
					arr0_1.push(obj);
				}
				else if(useNum==1 && noUseNum==1){
					arr1_1.push(obj);
				}
				else if(useNum==2 && noUseNum==2){
					arr2_2.push(obj);
				}
				else if(useNum==3 && noUseNum==3){
					arr3_3.push(obj);
				}else if(useNum==0 && noUseNum==0){
					arr0_0.push(obj);
				}
			}	
			target = arr4_4.concat(arr3_4).concat(arr2_4).concat(arr1_4).concat(arr0_4);
			target = target.concat(arr2_3).concat(arr1_3);
			target=target.concat(arr0_3);
			target=target.concat(arr1_2);
			target=target.concat(arr0_2);
			target=target.concat(arr0_1);
			target=target.concat(arr1_1);
			target=target.concat(arr2_2);
			target=target.concat(arr3_3);
			target=target.concat(arr0_0);
		
			return target;
		}
		
		/**
		 * 魂印排序  
		 * @param value
		 * @return 
		 * 
		 */		
		protected function sortHunYun(value:Array):Array{
			var source:Array=value;
			var target:Array=[];
			var len:uint=source.length;
			for(var i:uint=0;i<len;i++){
				var obj:Object=source[i];
				if(obj==null)continue;
				if(!EquipDataConst.getInstance().isEquip(obj.type))continue;
				if(IntroConst.ItemInfo[obj.id]==null)continue;
				if(IntroConst.ItemInfo[obj.id].isBind==2){
					target.push(obj);
				}else{
					target.unshift(obj);
				}		
			}
			
			return target;
		}
		/**
		 *	雕琢排序 
		 * @param value
		 * @return 
		 * 
		 */		 		
		 protected function sortDecorate(value:Array):Array{
			return this.sortEnchase(value);
		} 
		
		/**
		 * 挑选辅助道具物品 
		 * 
		 */		
		protected function checkItems():void{	
			
			
			var arr:Array=BagData.AllItems[1] as Array;
			var len:uint=arr.length;
			if(this.pageSelectedIndex==0){
				var strengenCount:uint=0;
				this.strengenItems=[[]];
				for (var i:uint=0;i<len;i++){
					if(i>=BagData.BagNum[1])continue;
					if(arr[i]==null){
						if(strengenItems[Math.floor(strengenCount/8)]==null)strengenItems[Math.floor(strengenCount/8)]=[];
						strengenItems[Math.floor(strengenCount/8)][strengenCount%8]=1;
						strengenCount++;
					}else{
						if(EquipDataConst.getInstance().isStrengenItem(arr[i].type)){
							if(strengenItems[Math.floor(strengenCount/8)]==null)strengenItems[Math.floor(strengenCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[i].id]){
								strengenItems[Math.floor(strengenCount/8)][strengenCount%8]={detailData:arr[i],id:arr[i].id,isLock:false,usedNum:EquipDataConst.getInstance().lockItems[arr[i].id].usedNum};
							}else{
								strengenItems[Math.floor(strengenCount/8)][strengenCount%8]={detailData:arr[i],id:arr[i].id,isLock:false};
							}
							strengenCount++;
						}
					}
				}		
			}else if(this.pageSelectedIndex==1){
				var addstarCount:uint=0;
				this.addStarItems=[[]];
				for (var j:uint=0;j<len;j++){
					if(j>=BagData.BagNum[1])continue;
					if(arr[j]==null){
						if(addStarItems[Math.floor(addstarCount/8)]==null)addStarItems[Math.floor(addstarCount/8)]=[];
						addStarItems[Math.floor(addstarCount/8)][addstarCount%8]=1;
						addstarCount++;
					}else{
						if(EquipDataConst.getInstance().isAddStarItem(arr[j].type)){
							if(addStarItems[Math.floor(addstarCount/8)]==null)addStarItems[Math.floor(addstarCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[j].id]){
								addStarItems[Math.floor(addstarCount/8)][addstarCount%8]={detailData:arr[j],id:arr[j].id,usedNum:EquipDataConst.getInstance().lockItems[arr[j].id].usedNum};
							}else{
								addStarItems[Math.floor(addstarCount/8)][addstarCount%8]={detailData:arr[j],id:arr[j].id,isLock:false};
							}
							addstarCount++;
						}
					}
				}		
			}else if(this.pageSelectedIndex==2){
				
				var stoneCompose:uint=0;
				this.stoneComposeItems=[[]];
				for (var k:uint=0;k<len;k++){
					if(k>=BagData.BagNum[1])continue;
					if(arr[k]==null){
						if(stoneComposeItems[Math.floor(stoneCompose/8)]==null)stoneComposeItems[Math.floor(stoneCompose/8)]=[];
						stoneComposeItems[Math.floor(stoneCompose/8)][stoneCompose%8]=1;
						stoneCompose++;
					}else{
						if(EquipDataConst.getInstance().isStoneComposeItem(arr[k].type)){
							if(stoneComposeItems[Math.floor(stoneCompose/8)]==null)stoneComposeItems[Math.floor(stoneCompose/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[k].id]){
								stoneComposeItems[Math.floor(stoneCompose/8)][stoneCompose%8]={detailData:arr[k],id:arr[k].id,isLock:false,usedNum:EquipDataConst.getInstance().lockItems[arr[k].id].usedNum};
							}else{
								stoneComposeItems[Math.floor(stoneCompose/8)][stoneCompose%8]={detailData:arr[k],id:arr[k].id,isLock:false};
							}
							stoneCompose++;
						}
					}
				}		
			
			}else if(this.pageSelectedIndex==3){
				var stilettoCount:uint=0;
				this.stilettoItems=[[]];
				for (var l:uint=0;l<len;l++){
					if(l>=BagData.BagNum[1])continue;
					if(arr[l]==null){
						if(stilettoItems[Math.floor(stilettoCount/8)]==null)stilettoItems[Math.floor(stilettoCount/8)]=[];
						stilettoItems[Math.floor(stilettoCount/8)][stilettoCount%8]=1;
						stilettoCount++;
					}else{
						if(EquipDataConst.getInstance().isStilettoItem(arr[l].type)){
							if(stilettoItems[Math.floor(stilettoCount/8)]==null)stilettoItems[Math.floor(stilettoCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[l].id]==true){
								stilettoItems[Math.floor(stilettoCount/8)][stilettoCount%8]={detailData:arr[l],id:arr[l].id,isLock:true};
							}else{
								stilettoItems[Math.floor(stilettoCount/8)][stilettoCount%8]={detailData:arr[l],id:arr[l].id,isLock:false};
							}
							stilettoCount++;
						}
					}
				}	
				
			}else if(this.pageSelectedIndex==4){
				var enchaseCount:uint=0;
				this.enchaseItems=[[]];
				for (var m:uint=0;m<len;m++){
					if(m>=BagData.BagNum[1])continue;
					if(arr[m]==null){
						if(enchaseItems[Math.floor(enchaseCount/8)]==null)enchaseItems[Math.floor(enchaseCount/8)]=[];
						enchaseItems[Math.floor(enchaseCount/8)][enchaseCount%8]=1;
						enchaseCount++;
					}else{
						if(EquipDataConst.getInstance().isEnchaseItem(arr[m].type)){
							if(enchaseItems[Math.floor(enchaseCount/8)]==null)enchaseItems[Math.floor(enchaseCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[m].id]==true){
								enchaseItems[Math.floor(enchaseCount/8)][enchaseCount%8]={detailData:arr[m],id:arr[i].id,isLock:true};
							}else{
								enchaseItems[Math.floor(enchaseCount/8)][enchaseCount%8]={detailData:arr[m],id:arr[m].id,isLock:false};
							}
							enchaseCount++;
						}
					}
				}
				
			}else if(this.pageSelectedIndex==5){
				var extirpateCount:uint=0;
				this.extirpateItems=[[]];
				for (var n:uint=0;n<len;n++){
					if(n>=BagData.BagNum[1])continue;
					if(arr[n]==null){
						if(extirpateItems[Math.floor(extirpateCount/8)]==null)extirpateItems[Math.floor(extirpateCount/8)]=[];
						extirpateItems[Math.floor(extirpateCount/8)][extirpateCount%8]=1;
						extirpateCount++;
					}else{
						if(EquipDataConst.getInstance().isExtirpateItem(arr[n].type)){
							if(extirpateItems[Math.floor(extirpateCount/8)]==null)extirpateItems[Math.floor(extirpateCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[n].id]==true){
								extirpateItems[Math.floor(extirpateCount/8)][extirpateCount%8]={detailData:arr[n],id:arr[n].id,isLock:true};
							}else{
								extirpateItems[Math.floor(extirpateCount/8)][extirpateCount%8]={detailData:arr[n],id:arr[i].id,isLock:false};
							}
							extirpateCount++;
						}
					}
				}
			}else if(this.pageSelectedIndex==6){
				var hunYunCount:uint=0;
				this.hunYunItems=[[]];
				for (var h:uint=0;h<len;h++){
					if(h>=BagData.BagNum[1])continue;
					if(arr[h]==null){
						if(hunYunItems[Math.floor(hunYunCount/8)]==null)hunYunItems[Math.floor(hunYunCount/8)]=[];
						hunYunItems[Math.floor(hunYunCount/8)][hunYunCount%8]=1;
						hunYunCount++;
					}else{
						if(EquipDataConst.getInstance().isHunYunItem(arr[h].type)){
							if(hunYunItems[Math.floor(hunYunCount/8)]==null)hunYunItems[Math.floor(hunYunCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[h].id]==true){
								hunYunItems[Math.floor(hunYunCount/8)][hunYunCount%8]={detailData:arr[h],id:arr[h].id,isLock:true};
							}else{
								hunYunItems[Math.floor(hunYunCount/8)][hunYunCount%8]={detailData:arr[h],id:arr[h].id,isLock:false};
							}
							hunYunCount++;
						}
					}
				}
			}
			else if(this.pageSelectedIndex == 7)	//宝石雕琢
			{
				var dCount:int = 0;
				this.stoneDecorateItems=[[]];
				for(var tag:int = 0; tag < len; tag++){
					if(tag>BagData.BagNum[1])continue;
					if(!arr[tag]){
						if(stoneDecorateItems[Math.floor(dCount/8)]==null)stoneDecorateItems[Math.floor(dCount/8)]=[];
						stoneDecorateItems[Math.floor(dCount/8)][dCount%8]=1;
						dCount++;
					}else{
						if(EquipDataConst.getInstance().isDecorateItem(arr[tag].type)){
							if(stoneDecorateItems[Math.floor(dCount/8)]==null)stoneDecorateItems[Math.floor(dCount/8)]=[];
							if(EquipDataConst.getInstance().lockItems[arr[tag].id]==true){
								stoneDecorateItems[Math.floor(dCount/8)][dCount%8]={detailData:arr[tag],id:arr[tag].id,isLock:true};
							}else{
								stoneDecorateItems[Math.floor(dCount/8)][dCount%8]={detailData:arr[tag],id:arr[tag].id,isLock:false};
							}
							dCount++;
						}
					}
				}
			}	
		}
		
		/**
		 * 
		 * @param e
		 * 
		 */		
		protected function onStageClick(e:MouseEvent):void{
			var displayObj:DisplayObject=this.view.getChildByName("LIST");
			if(displayObj!=null){
				this.view.removeChild(displayObj);
			}
		}
		
		protected function onPanelCloseHandler(e:Event):void{
			this.equipColorSelectedIndex=0;
			this.dataProxy.equipPanelIsOpen=false;
			_point=null;
			_specialMc=null;
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
		}
		
		/**
		 * 改变描述（成功率，VIP加成，失败加成率，失败后降与不降，初始化描述） 
		 * 
		 */		
		protected function changeDes():void{
			this.view.txt_des.htmlText=EquipDataConst.getInstance().pageDic[this.pageSelectedIndex];
			(this.view.txt_des as TextField).mouseEnabled=false;			
			this.moneyBind.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.moneyRmb.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		/**
		 * 装备属性改变成功后的播放特效
		 **/ 	
		private var _point:Point;
		private var _specialMc:MovieClip;
		protected function addSpecialShow(point:Point):void{
			this._point=point.clone();
			if(!_specialMc){
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/EquipSpecial.swf",onLoabdComplete);
			}
			else{
				onLoabdComplete();
			}
		}
		/**
		 * 特效swf加载完成
		 **/ 		
		protected function onLoabdComplete():void{
			if(!_specialMc){
				_specialMc=ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/EquipSpecial.swf");
			}
			_specialMc.x=_point.x; 
			_specialMc.y=_point.y;
			_specialMc.gotoAndPlay(1);
			this.view.addChildAt(_specialMc,this.view.numChildren-1);
//			setTimeout(specialDispose,160,_specialMc);
		}
		/**
		 * 移除特效
		 **/ 
		/* protected function specialDispose(mc:MovieClip):void
		{
			if(this.view.contains(mc)){
				mc.stop();
				this.view.removeChild(mc);
			}
		} */
		
	}
}
