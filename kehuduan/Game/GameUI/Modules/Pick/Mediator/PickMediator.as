package GameUI.Modules.Pick.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.BagUtils;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pick.PickData.PickConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.Zippo;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.clearInterval;
	import flash.utils.clearTimeout;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PickMediator extends Mediator
	{
		public static const NAME:String = "PickMediator";
		
		private var panelBase:PanelBase = null;
		private var totalPage:uint = 0;
		private var id:uint = 0;
		/** 物品列表  */
		public var itemList:Array = null;
		public var currentPage:int = 0;
		public var preNum:uint = 4;
		
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		
		//判断三个背包是否满     by陈明
		private var normalBagIsFull:Boolean;
		private var stuffBagIsFull:Boolean;
		private var taskBagIsFull:Boolean;
		
		private var dataProxy:DataProxy = null;
		//自动挂机捡包参数
		private var autoPackageArr:Array;
		private var timeId:uint;
		private var pickTimeId:uint;
		
		public function PickMediator()
		{
			super(NAME);
		}
		
		private function get pickView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
	
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWPICK,
				EventList.DELETEPACKAGE,
				EventList.ALL_PICK_BAG,
				EventList.CLOSE_PICK_PANEL,
				PickConst.AUTO_PICK_PACKAGE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.PICKVIEW});
					panelBase = new PanelBase(pickView, pickView.width+8, pickView.height+12);
					panelBase.name = "BagItemPick";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pic_med_pic_han_1" ] );   //物品拾取 
					redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFramePick"); 
					redFrame.name = "redFramePick";
					yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFramePick"); 
					yellowFrame.name = "yellowFramePick";
				break;
				case EventList.SHOWPICK:
     				id = notification.getBody().id as uint;
     				if(!GameCommonData.PackageList[id])
     				{
     					return;
     				}
					if(!GameCommonData.PackageList[id].isPicked)
					{ 
						itemList = notification.getBody().items as Array;
						initView();
					}
				break;
				case EventList.DELETEPACKAGE:
					if(id == notification.getBody() as uint)
					{
						//panelCloseHandler(null);
						gcAll();
					}
				break;
				case EventList.ALL_PICK_BAG:
//					itemList = notification.getBody().items as Array;
//					id = notification.getBody().id as uint;
					if(dataProxy.pickBagIsOpen)
					{
						PickConst.isKeyBorPick = true;
						allPickHandler();
					}
					break;
				case EventList.CLOSE_PICK_PANEL:
					panelCloseHandler(null);
					break;
				case PickConst.AUTO_PICK_PACKAGE:
					itemList = [];
					autoPickHandler();
					break;
			}
		}
		
		private function initView():void
		{
			normalBagIsFull = false;
			stuffBagIsFull = false;
			taskBagIsFull = false;
			
			dataProxy.pickBagIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			panelBase.x = (GameConfigData.GameWidth - panelBase.width) / 2 + 150;
			panelBase.y = (GameConfigData.GameHeight - panelBase.height) / 2;
			
			pickView.btnPrv.addEventListener(MouseEvent.CLICK, prvHandler);
			pickView.btnBack.addEventListener(MouseEvent.CLICK, backHandler);
			pickView.btnAllPick.addEventListener(MouseEvent.CLICK, allPickHandlerBtn);			
			currentPage = 0;
			if( itemList && itemList.length > 0 )
			{
				for(var i:uint=0; i<preNum; i++)
				{
					pickView["mcItem_"+i].addEventListener(MouseEvent.CLICK, selectItemHandler);
					pickView["mcItem_"+i].addEventListener(MouseEvent.ROLL_OVER, ItemRollOver);
					pickView["mcItem_"+i].addEventListener(MouseEvent.ROLL_OUT, ItemRollOut);
				}
			}
			viewCurrentPage();
		}
		
		private function viewCurrentPage():void
		{
			removeFace();
			var len:uint = 0;
			len = itemList.length;
			totalPage = ((len-1)/preNum)>>0;
			for(var i:uint = this.currentPage*this.preNum; i<(this.currentPage + 1) * this.preNum; i++)
			{
				if(itemList[i])
				{
					pickView["mcItem_"+(i % preNum)].visible = true;
					pickView["mcItem_"+(i % preNum)].mouseEnabled = true;
					pickView["mcItem_"+(i % preNum)].mouseChildren = true;
					var faceItem:FaceItem = new FaceItem(itemList[i], pickView["mcItem_"+(i % preNum)]);
					faceItem.name = "faceItem";
					pickView["mcItem_"+(i % preNum)].addChild(faceItem);
					pickView["mcItem_"+(i % preNum)].txtName.text = UIConstData.getItem(itemList[i]).Name; 
					faceItem.x = 2;
					faceItem.y = 2;
				}
				else
				{
					pickView["mcItem_"+(i % preNum)].txtName.text = "";
					pickView["mcItem_"+(i % preNum)].visible = false;
					pickView["mcItem_"+(i % preNum)].mouseEnabled = false;
					pickView["mcItem_"+(i % preNum)].mouseChildren = false;
				}
			}
			pickView.txtPage.text = (currentPage+1) + "/" + (totalPage+1);
		}
		
		private function backHandler(event:MouseEvent):void {
			currentPage++;
			if(currentPage > totalPage) {
				currentPage = totalPage;
			} else {
				viewCurrentPage();
			}			
		}
		
		private function prvHandler(event:MouseEvent):void {
			currentPage--;
			if(currentPage < 0) {
				currentPage = 0;
			} else {
				viewCurrentPage();
			}
		}
		
		private function selectItemHandler(event:MouseEvent):void
		{
			var index:int = currentPage * preNum + int(event.target.name.split("_")[1]);
			
			/**修改时间 2010.8.25   by 陈明				*/
			if(bagIsFull(itemList[index]))
			{
				return;
			}
			
			OperateItem.IsPick = true;
			Zippo.SendPickItem(id, itemList[index]);
			var tmpArr:Array = itemList.splice(index,1);
			var total:int = ((itemList.length-1)/preNum)>>0;
			currentPage = 0;
			viewCurrentPage();
			testIsEmpty();
		}
		
		/**
			 * 新版本判断背包是否满，by陈明
		 * */
		 private function bagIsFull(type:uint):Boolean
		 {
		 	var type2:String = String(type).slice(0,2);
		 	
		 	//判断宠物背包是否已满
		 	if(String(type).slice(0,1) == "7")
		 	{
		 		var petNum:int;   //宠物背包容量
  				for(var key:Object in GameCommonData.Player.Role.PetSnapList) 
  				{
					if(GameCommonData.Player.Role.PetSnapList[key].IsLock == false) 
					{
	   					petNum++;
					}
				}
				if(petNum+1 >PetPropConstData.petBagNum)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_1" ], color:0xffff00});
					return true;
				}
				return false;
		 	}

		 	if(type2=="41" || type2=="42" || type2=="61" || type2=="63" )
		 	{
		 		if(BagUtils.TestBagIsFull(1) == BagData.BagNum[1])
		 		{
		 			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_2" ], color:0xffff00});
					return true;
		 		}
		 	}
		    else if(type2=="62" )
		 	{
		 		if(BagUtils.TestBagIsFull(2) == BagData.BagNum[2])
		 		{
		 			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_3" ], color:0xffff00});
					return true;
		 		}
		 	}
		 	else
		 	{
		 		if(BagUtils.TestBagIsFull(0) == BagData.BagNum[0])
		 		{
		 			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_4" ], color:0xffff00});
					return true;
		 		}
		 	}
		 	return false;
		 }
		 
		 //判断物品是否可以叠加
		 private function canPlus(type:uint,selectIndex:uint):Boolean
		 {
		 	var bagList:Array = BagData.AllItems[selectIndex];
		 	for(var i:uint = 0; i < bagList.length; i++)
		 	{
		 		if(bagList[i] == undefined) continue;
		 		if(type == bagList[i].type && type > 300000)
		 		{
		 			if(bagList[i].amount < bagList[i].maxAmount)
		 			{
		 				return true;
		 			}
		 		}
		 	}
		 	return false;
		 }
		
		private function ItemRollOver(event:MouseEvent):void
		{
			var targetMc:MovieClip = event.currentTarget as MovieClip;
			redFrame.x = targetMc.x;
			redFrame.y = targetMc.y;
			pickView.addChild(redFrame);
		}
		
		private function removeFace():void
		{
			for(var i:int = 0; i<preNum; i++)
			{
				if(pickView["mcItem_"+i].getChildByName("faceItem"))
				{
					pickView["mcItem_"+i].removeChild(pickView["mcItem_"+i].getChildByName("faceItem"));
				}
			}
		}
		
		private function ItemRollOut(event:MouseEvent):void
		{
			if(pickView.getChildByName("redFramePick"))
			{
				if(pickView.contains(pickView.getChildByName("redFramePick")))
				{
					pickView.removeChild(pickView.getChildByName("redFramePick"));
				}
			}
		}
		
		private function allPickHandlerBtn(event:MouseEvent):void
		{
			PickConst.isKeyBorPick = true;
			allPickHandler();
		}
			
		/**
		 * by陈明 修改时间 2010.08.26
		 * */
		private function allPickHandler():void
		{
			//如果同类型的物品背包已经满，第二个同类型的就不发送
			if (itemList.length>0)
			{
				if ( PickConst.isKeyBorPick )
				{
					if (isAllPickBagFull())
					{
						return;
					}
				}
				
				OperateItem.IsPick = true;

				if (GameCommonData.PackageList[id])
				{
					GameCommonData.PackageList[id].isPicked = true;
				}
				Zippo.SendPickItem(id, 999999);
				setTimeout(removePackage,3000,id);
				itemList = new Array();
				viewCurrentPage();
			}
			testIsEmpty();			
		}
		
		private function isAllPickBagFull():Boolean
		{
			var len:uint = itemList.length;
			var aType:Array = itemList.slice(0,len);
			var normal_num:uint = 0;
			var stuff_num:uint = 0;
			var task_num:uint = 0;
			
			for(var i:uint = 0; i <itemList.length; i++)
			{
				var newType:String = String(itemList[i]).slice(0,2);
				
				//判断宠物背包是否已满
		 		if(String(itemList[i]).slice(0,1) == "7")
		 		{
		 			var petNum:uint = 0;   //宠物背包容量
		 			
  					for(var key:Object in GameCommonData.Player.Role.PetSnapList) 
  					{
						if(GameCommonData.Player.Role.PetSnapList[key].IsLock == false) 
						{
	   						petNum++;
						}
					}
					if(petNum+1 >PetPropConstData.petBagNum)
					{
						if (PickConst.isKeyBorPick)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_1" ], color:0xffff00});
						}
						return true;
					}
//					return false;
					continue;
		 		}
		 		
				if(newType=="41" || newType=="42" || newType=="61" || newType=="63" )
				{
					if(BagUtils.TestBagIsFull(1) +stuff_num < BagData.BagNum[1])
					{
						stuff_num ++;
					}else
					{
						if (PickConst.isKeyBorPick)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_2" ], color:0xffff00});
						}
						return true;
					}
				}
				else if(newType=="62" )
				{
					if(BagUtils.TestBagIsFull(2) +task_num < BagData.BagNum[2])
					{
						task_num++;
					}else
					{
						if (PickConst.isKeyBorPick)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_3" ], color:0xffff00});
						}
						return true;
					}
				}
				else
				{
					if(BagUtils.TestBagIsFull(0) +normal_num < BagData.BagNum[0])
					{
						normal_num ++;
					}else
					{
						if (PickConst.isKeyBorPick)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_4" ], color:0xffff00});
						}
						return true;
					}
				}
			}
			return false;
		}
		
		//如果3秒没收到删包消息，则自行了断
		private function removePackage():void
		{
			UIFacade.UIFacadeInstance.DeletePackage(arguments[0]);
		}
		
		private function testIsEmpty():void
		{
			if(this.itemList.length == 0)
			{
				panelCloseHandler(null);
			}	
		}
		
		private function removeYellowFrame():void
		{
			if(redFrame.contains(redFrame.getChildByName("yellowFramePick")))
			{
				pickView.removeChild(redFrame.getChildByName("yellowFramePick"));
			}
		}
		
		/** 收到删除命令 */
		private function gcAll():void
		{
			dataProxy.pickBagIsOpen = false;
			PickConst.isKeyBorPick = false;
			if(panelBase)
			{
				if(GameCommonData.GameInstance.GameUI.contains(panelBase))
				{
					GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				}
			}
			if(pickView)
			{
				pickView.btnPrv.removeEventListener(MouseEvent.CLICK, prvHandler);
				pickView.btnBack.removeEventListener(MouseEvent.CLICK, backHandler);
				pickView.btnAllPick.removeEventListener(MouseEvent.CLICK, allPickHandler);
				for(var i:uint=0; i<preNum; i++)
				{
					pickView["mcItem_"+i].removeEventListener(MouseEvent.CLICK, selectItemHandler);
					pickView["mcItem_"+i].removeEventListener(MouseEvent.ROLL_OVER, ItemRollOver);
					pickView["mcItem_"+i].removeEventListener(MouseEvent.ROLL_OUT, ItemRollOut);
				}
				itemList =[];
			}
			
			if(GameCommonData.PackageList[id])
			{
				GameCommonData.PackageList[id].isPicked = true;
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			dataProxy.pickBagIsOpen = false;
			PickConst.isKeyBorPick = false;
			if(panelBase)
			{
				if(GameCommonData.GameInstance.GameUI.contains(panelBase))
				{
					GameCommonData.GameInstance.GameUI.removeChild(panelBase);
					pickView.btnPrv.removeEventListener(MouseEvent.CLICK, prvHandler);
					pickView.btnBack.removeEventListener(MouseEvent.CLICK, backHandler);
					pickView.btnAllPick.removeEventListener(MouseEvent.CLICK, allPickHandler);
					for(var i:uint=0; i<preNum; i++)
					{
						pickView["mcItem_"+i].removeEventListener(MouseEvent.CLICK, selectItemHandler);
						pickView["mcItem_"+i].removeEventListener(MouseEvent.ROLL_OVER, ItemRollOver);
						pickView["mcItem_"+i].removeEventListener(MouseEvent.ROLL_OUT, ItemRollOut);
					}
				}
			}	
//			if(itemList.length == 0)
//			{
//				trace("Pick allPickHandler 不捡");
//				Zippo.SendPickItem(id, 999998); 
//			}
		}
		
		//挂机捡包
		private function autoPickHandler():void
		{
			autoPackageArr = [];
			itemList = [];
			for(var keyPackage:Object in GameCommonData.PackageList)
			{
				var obj:Object = new Object();
				obj.items = GameCommonData.PackageList[keyPackage].Data;
				obj.id = GameCommonData.PackageList[keyPackage].ID;
				autoPackageArr.push(obj);
			}
			timeId = setInterval(autoPick,650);
		}
		
		private function autoPick():void
		{
			if(autoPackageArr.length == 0)
			{
				clearInterval(timeId);
				return;
			}
			
			var obj:Object = autoPackageArr.shift();
			itemList = obj.items;
			id = obj.id;
			if (itemList.length>0)
			{
				if(isAllPickBagFull())
				{
					clearInterval(timeId);
					return;
				}
			}
			
//			initView();
			pickTimeId = setTimeout(pickEnd,500);
		}
		
		private function pickEnd():void
		{
//			if(dataProxy.pickBagIsOpen)
//			{
				allPickHandler();
//			}
			clearTimeout(pickTimeId);
		}
	}
}