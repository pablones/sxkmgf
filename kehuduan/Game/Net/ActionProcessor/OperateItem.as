package Net.ActionProcessor
{
	import Controller.PlayerSkinsController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.SoundList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Command.SetCdData;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessEvent;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.MouseCursor.RepeatRequest;
	import GameUI.Sound.SoundManager;
	import GameUI.UIUtils;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class OperateItem extends GameAction
	{
		/** 物品消息类型  */
		public static const VIEWITEMINFO:uint = 65; 		//请求物品详细信息（一般用于聊天里面的物品）
		public static const DROP:uint =  3; 				//丢掉物品
		public static const USE:uint =  4; 					//使用物品
		public static const REQUEST_SPLITITEM:uint = 7 		//请求拆分
		public static const REQUEST_COMBINEITEM:uint = 8 	//请求合并
		public static const GETINFO:uint = 101     			//请求自己物品的详细信息
		public static const ADD:uint = 100     				//物品的添加和更新
		public static const DEAL:uint = 102    				//整理
		public static const SYNCHRO_UNLOCK:uint	= 47  		//物品解锁
		public static const TRADEPACKIDX:uint	= 43		//请求互换两个物品的位置
		public static const CHGPACKIDX:uint	= 44			//请求改变物品的package_index属性在同一背包中
		public static const EQUIP:uint = 5;					//使用物品
		public static const UNEQUIP:uint = 6;				//取下装备
		public static const USESCUESS:uint = 57;			//物品装备使用成功
		public static const SYNCNUM:uint = 25;				//同步物品数量
		//从身上删除物品   8.12 
		public static const DROP_FROM_BODY:uint = 18;		//从身上删除物品(物品过期)
		//
		
		public static var IsSelfThrow:Boolean = false;
		public static var IsOrder:Boolean = false;
		public static var CompleteTrade:Boolean = false;
		public static var IsPick:Boolean = false;
		
		////////////////////////
		//NPC商店
		public static const BUYNPCITEM:uint = 1; 			//购买NPC商店商品
		public static const SINGLEREPARE:uint = 14; 		//NPC商店单修
		public static const MULTIREPARE:uint = 15; 			//NPC商店群修  （身上所有装备）
		public static const SALEGOOD:uint = 2; 				//NPC商店出售物品
		//////////////////////////
		
		//摆摊
		public static var IsStallItem:Boolean = false;		//是否是摆摊物品  position=200
		public static const GETSTALLITEMS:uint = 21;		//查询摊位物品
		public static const ADDSTALLITEM:uint  = 22;		//添加物品	
		public static const DELSTALLITEM:uint  = 23;		//删除商品
		public static const BUYSTALLITEM:uint  = 24;		//购买摆摊商品 
		public static const MODIFYPRICE:uint   = 103;		//修改商品单价
		public static const CLEARSTALLMSG:uint = 104;		//清除所有留言
		
		public static const PET_ADD_STALL:uint = 107;				//加宠物
		public static const PET_DEL_STALL:uint = 108;				//减宠物
		public static const PET_BUY_STALL:uint = 109;				//买宠物
		public static const PET_MODIFY_PRICE_STALL:uint = 110;		//宠物改价格
		//////////////////////////
		
		//宠物
		public static const GET_PET_INFO:uint = 105     	//请求宠物详细信息
		public static const GETPETLIST_OFPLAYER:uint  = 106;//查询某人的宠物列表
		/////////////////////////
		
		private var weaponModelType:uint = 0;
		
		public function OperateItem()
		{
			super();
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			
			bytes.position  = 4;
			var obj:Object  = null;
			var action:uint = bytes.readUnsignedShort();		//action
			var count:uint  = bytes.readUnsignedShort();		//操作多少个物品
			var item:uint   = bytes.readUnsignedInt();			//		
			var nData4:uint = bytes.readUnsignedInt();			//使用物品
			var nData5:uint = bytes.readUnsignedShort();		//0, 丢弃X
			var nData6:uint = bytes.readUnsignedShort();		//0, 丢弃Y
			var name:String = bytes.readMultiByte(16, GameCommonData.CODE);			//0, 丢弃Y
			var i:int = 0;
//			Logger.Print(this, "OperateItem action =" + action);
			
			switch(action)
			{
				case ADD:
				{
					if(item==1){
						RepeatRequest.getInstance().bagItemCount=count;
					}else if(item==2){
						RepeatRequest.getInstance().bagItemCount+=count;
					}
					
					for(i = 0 ; i < count; i ++)
					{
						obj 		  = new Object();
						obj.id		  = bytes.readUnsignedInt();				//id
						obj.type 	  = bytes.readUnsignedInt();				//类型   300001
						obj.amount    = bytes.readUnsignedInt();				//数量(持久)				(8.12 bytes.readUnsignedShort();)
						obj.maxAmount = bytes.readUnsignedInt();				//最大数量（最大持久）	(8.12 bytes.readUnsignedShort();)
						obj.position  = bytes.readUnsignedByte();				//位置
						obj.isBind 	  = bytes.readUnsignedByte();				//是否绑定
						obj.index 	  = bytes.readUnsignedShort() - 1;			//背包内部位置	
						obj.price	  = bytes.readUnsignedInt();				//单价		
						obj.color	  = bytes.readUnsignedInt();				//颜色 （0-5）	
						
						//武魂的图片特殊处理
//						if ( obj.type>=250000 && obj.type<300000  )
//						{
//							var wuHunObj:Object = UIConstData.getItem( obj.type );
//							if ( wuHunObj )
//							{
//								if ( String(wuHunObj.img).split( "_" ).length == 1 )
//								{
//									wuHunObj.img = ( wuHunObj.img.toString() + "_" +  obj.color.toString() );
//								}
//							}
//						}
						
						var typeAdd:uint = obj.type / 1000;
						if(typeAdd == 301 || typeAdd == 311 || typeAdd == 321) {	//大血大蓝	(客户端添加剩余血量)
							obj.noUse = obj.amount;			//剩余血魔
							obj.maxUse = obj.maxAmount;		//血魔总量
							obj.amount = 1;
							obj.maxAmount = 1; 
						}
						
						if(typeAdd==351){   //元宝票，经验票之类的    
							obj.amountMoney=obj.amount;
							obj.amount=1;
						}
						if(obj.type == 626100) {  //跑商银票
							obj.amountMoney=obj.amount;
							obj.amount=1;
							UIConstData.IS_BUSINESSING = true;
							PlayerSkinsController.UnMount();
						}
						
						if(obj.type == 612000 || obj.type == 612001)
						{
							facade.sendNotification(MeridiansEvent.UPDATA_STRENGTH_PROTECT); 
						}
						
						if(obj.type == 381001)
						{
							facade.sendNotification(MeridiansEvent.UPDATA_ARCHEAUS_DAN);
						}
						
						if(obj.type == 630014)
						{
							facade.sendNotification( PrepaidUIData.UPDATE_ADDXIAOYAO, obj.amount );
						}
						if(obj.type == 610059)
						{
							facade.sendNotification( CastSpiritData.UPDATE_CASTSPIRIT_NUMBER, obj.amount );
						}
						var cdObj:Object=SetCdData.searchCdDataByType(QuickBarData.getInstance().getCdType(obj.type));
						
						if(cdObj!=null){
							trace(QuickBarData.getInstance().getCdType(obj.type));
							trace("对新加入的物品进行CD");
							SetCdData.setData(obj.position-47,obj.index,cdObj.cdtimer,obj.type,cdObj.count,cdObj.cdType);
						}
//						trace("obj.position = ", obj.position); 
//						trace("obj.index = ", obj.index); 
//						trace("===背包========================================");
//						trace("action:", action);
//						trace("id:", obj.id);
//						trace("type:", obj.type);
//						trace("amount:", obj.amount);
//						trace("maxAmount:", obj.maxAmount);
//						trace("position:", obj.position);
//						trace("isBind:", obj.isBind);
//						trace("index:", obj.index);
//						trace("==========================================="); 
								
						if(obj.position>=47&&obj.position<=49)
						{
//							if(!BagData.isHasItemById(obj.id)) {
//								sendNotification(BagEvents.SHOW_MSG_GETED_ITEM, {type:obj.type, amount:obj.amount});
//							} 
							BagData.AllItems[obj.position-47][obj.index] = obj;
							facade.sendNotification(NPCBusinessEvent.UPDATE_MONEY_LAST_NPCBUSINESS);	//更新跑商银子
							if(obj.position==47){
								facade.sendNotification(EquipCommandList.ADD_EQUIP_ITEM,obj);
							}
						}
						else if(obj.position>=0&&obj.position<=46)
						{
							RolePropDatas.ItemList[obj.position - 1] = obj;
						}
						else if(obj.position == 200) //(200表示摆摊物品)
						{	
							IsStallItem = true;
							StallConstData.goodList[obj.index] = obj;
						}
					}
					if(IsStallItem) 
					{
						sendNotification(EventList.STALLITEM);
						for(var g:int = 0; g<BagData.GridUnitList.length; g++)
						{
							BagData.GridUnitList[g].Grid.mouseEnabled = true;
						}
						IsStallItem = false;
					}
					
					if(obj==null)return;
					if(IntroConst.ItemInfo[obj.id]) delete IntroConst.ItemInfo[obj.id];	//清除缓存
					facade.sendNotification(EventList.UPDATEBAG);
					sendNotification(EventList.ONSYNC_BAG_QUICKBAR,obj.type);
				}
				break;
//				case CLEARDETAIL:			7.29注释掉
//					var itemID:uint = bytes.readUnsignedInt();				//id
//					IntroConst.ItemInfo[itemID] = undefined;
//				break;
				case DROP:
				{
//					trace("------------------DROP------------------------------------------");
					var dropObj:Object = new Object();
					dropObj.id 		   = bytes.readUnsignedInt();
					dropObj.type 	   = bytes.readUnsignedInt();	
					dropObj.amount 	   = bytes.readUnsignedInt();					
					dropObj.maxAmount  = bytes.readUnsignedInt();
					dropObj.position   = bytes.readUnsignedByte();
					dropObj.isBind     = bytes.readUnsignedByte();
					dropObj.index 	   = bytes.readUnsignedShort();
					dropObj.price	  = bytes.readUnsignedInt();
					dropObj.color	  = bytes.readUnsignedInt();				//颜色 （0-5）	
					
					
					facade.sendNotification(BagEvents.DROPITEM, dropObj);
					
					delete IntroConst.ItemInfo[dropObj.id];
					for(i = 0; i<BagData.AllItems.length; i++)
					{
						for(var m:int = 0; m<BagData.AllItems[i].length; m++)
						{
							if(BagData.AllItems[i][m] == undefined) continue;
							if(BagData.AllItems[i][m].id == dropObj.id)
							{
								
								if(BagData.AllItems[i][m].type==626100){
									UIConstData.IS_BUSINESSING = false;
								}
								
								BagData.AllItems[i][m] = undefined;	
								BagData.AllLocks[i][m] = false;
								BagData.SelectedItem 	 = null;
								facade.sendNotification(BagEvents.SHOWBTN, false);
								facade.sendNotification(EventList.ONSYNC_BAG_QUICKBAR,dropObj.type);	 	
								facade.sendNotification(EquipCommandList.DROP_EQUIP_ITEM,dropObj);					
								facade.sendNotification(NPCBusinessEvent.UPDATE_MONEY_LAST_NPCBUSINESS);	//更新跑商银子
								return;
							}
						}
					}
				}
				break;
				case DROP_FROM_BODY:
					var dropBodyObj:Object = new Object();
					dropBodyObj.id 		   = bytes.readUnsignedInt();
					dropBodyObj.type 	   = bytes.readUnsignedInt();	
					dropBodyObj.amount 	   = bytes.readUnsignedInt();					
					dropBodyObj.maxAmount  = bytes.readUnsignedInt();
					dropBodyObj.position   = bytes.readUnsignedByte();
					dropBodyObj.isBind     = bytes.readUnsignedByte();
					dropBodyObj.index 	   = bytes.readUnsignedShort();
					dropBodyObj.price	   = bytes.readUnsignedInt();
					dropBodyObj.color	  = bytes.readUnsignedInt();				//颜色 （0-5）	
					
					for(i = 0; i < RolePropDatas.ItemList.length; i++) {
						if(RolePropDatas.ItemList[i] && (RolePropDatas.ItemList[i].id == dropBodyObj.id)) {
							//更新人物属性UI
							facade.sendNotification(RoleEvents.UPDATEOUTFIT);
							//调用游戏中的换装备（取下装备）
							PlayerSkinsController.SetAccouter(RolePropDatas.ItemList[i].type, GameCommonData.Player, false);
							RolePropDatas.ItemList[i] = undefined;
							break;
						}
					}
				break;
				case SYNCHRO_UNLOCK:
				{
					obj		      = new Object();
					obj.id		  = bytes.readUnsignedInt();
					obj.type 	  = bytes.readUnsignedInt();	
					obj.amount    = bytes.readUnsignedInt();					
					obj.maxAmount = bytes.readUnsignedInt();
					obj.position  = bytes.readUnsignedByte();
					obj.isBind 	  = bytes.readUnsignedByte();
					obj.index 	  = bytes.readUnsignedShort();
					obj.price 	  = bytes.readUnsignedInt();
					obj.color	  = bytes.readUnsignedInt();				//颜色 （0-5）	
								
					for(i= 0; i<BagData.AllItems.length; i++)
					{
						for(var n:int = 0; n<BagData.AllItems[i].length; n++)
						{
							if(BagData.AllItems[i][n] == undefined) continue;
							if(obj.id == BagData.AllItems[i][n].id)
							{
								BagData.AllLocks[i][n] = false;
								if(BagData.GridUnitList[n] && BagData.GridUnitList[n].Item && BagData.GridUnitList[n].Item.Id == obj.id) 
									BagData.GridUnitList[n].Item.IsLock = false;
								break;
							}
						}
					}
					BagData.lockBagGridUnit(true);		//背包格子mouseEnabled = true
					BagData.lockBtnCleanAndPage(true);	//背包整理、拆分、摆摊、丢弃、使用、页签按钮mouseEnabled = true
				}
				break;
				
				//将背包物品装备到玩家身上服务端返回信息
				case EQUIP:
					var eqId:uint = bytes.readUnsignedInt();
					var nData8:uint = bytes.readUnsignedInt();		
					var nData9:uint = bytes.readUnsignedInt();					
					var nData10:uint = bytes.readUnsignedInt();
					var nData11:uint = bytes.readUnsignedByte();
					var nData12:uint = bytes.readUnsignedByte();
					var nData13:uint = bytes.readUnsignedShort();
					var nData14:uint = bytes.readUnsignedInt();
					bytes.readUnsignedInt();				//颜色 （0-5）	
						
					var nPosition:uint = nData5 / 1000;								//装备位置
					var nPositionBag:uint = nData5 % 1000;							//背包类型
					var nIndexBag:uint = nData6;									//背包内位置

					var tempObj:Object=UIUtils.DeeplyCopy(RolePropDatas.ItemList[nPosition-1]);
				    
				    for(i = 0; i<BagData.AllItems.length; i++)
					{
						var isFind:Boolean = false;
						for(var z:int = 0; z<BagData.AllItems[i].length; z++)
						{
							if(BagData.AllItems[i][z] == undefined) continue;
							if(BagData.AllItems[i][z].id == eqId)
							{
								RolePropDatas.ItemList[nPosition-1] = UIUtils.DeeplyCopy(BagData.AllItems[i][z]);
								BagData.AllItems[i][z] = undefined;	
								BagData.AllLocks[i][z] = false;
								if(BagData.GridUnitList[z] && BagData.GridUnitList[z].Item) {
									BagData.GridUnitList[z].Item = null;
									BagData.GridUnitList[z].IsUsed = false;
								}
								isFind = true;
								break;
							}
						}
						if(isFind) break;
					}
					
					RolePropDatas.ItemList[nPosition-1].position = nPosition;
					BagData.SelectedItem = null;
					facade.sendNotification(BagEvents.SHOWBTN, false);
					if(nPositionBag != 0)
					{
						BagData.AllItems[nPositionBag-47][nIndexBag-1] = tempObj;
						BagData.AllItems[nPositionBag-47][nIndexBag-1].index = nIndexBag-1;
						facade.sendNotification(EventList.UPDATEBAG);
					}
					//调用游戏中的换装备（穿上装备） 
					weaponModelType = UIConstData.getItem(RolePropDatas.ItemList[nPosition-1].type).modelType;
					PlayerSkinsController.SetAccouter(weaponModelType,GameCommonData.Player,true);
					if(nPosition == 16) // 魂魄位置
					{
						SoulMediator.isEquiptSoul = true;
						SoulProxy.getSoulDetailInfo();
					}
					SoundManager.PlaySound(SoundList.EQUIPED);
					BagData.lockBagGridUnit(true);		//背包格子mouseEnabled = true
					BagData.lockBtnCleanAndPage(true);	//背包整理、拆分、摆摊、丢弃、使用、页签按钮mouseEnabled = true
				break;
				case UNEQUIP:
					var unEquipId:uint = bytes.readUnsignedInt();
					var unEquipType:uint = bytes.readUnsignedInt();	
					var unEquipAmount:uint = bytes.readUnsignedInt();					
					var unEquipMaxAmount:uint = bytes.readUnsignedInt();
					var unEquipPosition:uint = bytes.readUnsignedByte();
					var unEquipIsBind:uint = bytes.readUnsignedByte();
					var unEquipIndex:uint = bytes.readUnsignedShort();
					var unEquipPrice:uint = bytes.readUnsignedInt();
					bytes.readUnsignedInt();				//颜色 （0-5）	
					
					var position:uint = nData5 / 1000;	
					var positionBag:uint = nData5 % 1000;
					var indexBag:uint = nData6;
					BagData.AllItems[positionBag-47][indexBag-1] = UIUtils.DeeplyCopy(RolePropDatas.ItemList[position-1]);
					BagData.AllItems[positionBag-47][indexBag-1].position = positionBag;
					BagData.AllItems[positionBag-47][indexBag-1].index = indexBag-1;
					
					//调用游戏中的换装备（取下装备）
					weaponModelType = UIConstData.getItem(RolePropDatas.ItemList[position-1].type).modelType;
					PlayerSkinsController.SetAccouter(weaponModelType,GameCommonData.Player,false);
					
					RolePropDatas.ItemList[position-1] = undefined;	
					if(position == 16) //取下魂魄  刷新页面
					{
						SoulMediator.isEquiptSoul = false;
						facade.sendNotification(SoulProxy.SHOW_AFTER_GET_INFO);
					}
					facade.sendNotification(EventList.UPDATEBAG);
					facade.sendNotification(RoleEvents.UPDATEOUTFIT);	
				break;
				case USESCUESS:
					obj		      = new Object();
					obj.id		  = bytes.readUnsignedInt();
					obj.type 	  = bytes.readUnsignedInt();	
					obj.amount    = bytes.readUnsignedInt();					
					obj.maxAmount = bytes.readUnsignedInt();
//					obj.position  = bytes.readUnsignedByte();
//					obj.isBind 	  = bytes.readUnsignedByte();
//					obj.index 	  = bytes.readUnsignedShort();
//					obj.price 	  = bytes.readUnsignedInt();
//					obj.color	  = bytes.readUnsignedInt();				//颜色 （0-5）	
//
//					var typeMul:uint = obj.type / 1000;
//					if(typeMul == 301 || typeMul == 311 || typeMul == 321) {	//大血大蓝	(客户端添加剩余血量)
//						for(i= 0; i < BagData.AllItems.length; i++) {
//							var find:Boolean = false;
//							for(var k:int = 0; k < BagData.AllItems[i].length; k++) {
//								if(BagData.AllItems[i][k] == undefined) continue;
//								if(obj.id == BagData.AllItems[i][k].id) {
//									BagData.AllItems[i][k].noUse = obj.amount;
//									BagData.AllItems[i][k].maxUse = obj.maxAmount;
//									break;
//								}
//							}
//							if(find) break;
//						}
//					} 
					
					facade.sendNotification(EventList.UPDATEBAG);
					facade.sendNotification(RoleEvents.UPDATEOUTFIT);
				break;
				case SYNCNUM:
					var syncNum:Object = new Object();
					syncNum.id 		 	= bytes.readUnsignedInt();
					syncNum.type 	    = bytes.readUnsignedInt();	
					syncNum.amount 		= bytes.readUnsignedInt();		
					syncNum.maxAmount   = bytes.readUnsignedInt();
					syncNum.position    = bytes.readUnsignedByte();		//200则表示摆摊物品
					syncNum.isBind      = bytes.readUnsignedByte();		//绑定物品不能摆摊  0-绑定，1-未绑定
					syncNum.index 	 	= bytes.readUnsignedShort()-1;	//在格子数组中的位置，服务器从1开始，这里-1得到的是在数组中的下标
					syncNum.price	  	= bytes.readUnsignedInt();		//单价
					syncNum.color	  	= bytes.readUnsignedInt();		//颜色 （0-5）
					
					if(syncNum.type == 381001)
					{
						facade.sendNotification(MeridiansEvent.UPDATA_ARCHEAUS_DAN);
					}
					if(syncNum.type == 630014)
					{
						facade.sendNotification( PrepaidUIData.UPDATE_ADDXIAOYAO, syncNum.amount );
					}
//					if(syncNum.type == 610059)
//					{
//						facade.sendNotification( CastSpiritData.UPDATE_CASTSPIRIT_NUMBER, syncNum.amount );
//					}
					for(i= 0; i<BagData.AllItems.length; i++)
					{
						for(var j:int = 0; j<BagData.AllItems[i].length; j++)
						{
							if(BagData.AllItems[i][j] == undefined) continue;
							if(syncNum.id == BagData.AllItems[i][j].id)
							{
								BagData.AllItems[i][j].amount = syncNum.amount;
								if(BagData.AllItems[i][j].type == 626100) {  //跑商银票
									BagData.AllItems[i][j].amountMoney = syncNum.amount;
									BagData.AllItems[i][j].amount = 1;
								} else if(String(BagData.AllItems[i][j].type).indexOf("626") == 0){
									BagData.AllItems[i][j].price = syncNum.price;
								}
								if(IntroConst.ItemInfo[syncNum.id] != undefined)
								{
									IntroConst.ItemInfo[syncNum.id].amount = syncNum.amount;
									if(IntroConst.ItemInfo[syncNum.id].type == 626100) {  	//跑商银票
										IntroConst.ItemInfo[syncNum.id].amountMoney = syncNum.amount;
										IntroConst.ItemInfo[syncNum.id].amount = 1;
									} else if(String(IntroConst.ItemInfo[syncNum.id].type).indexOf("626") == 0) {
										BagData.AllItems[i][j].price = syncNum.price;
									}
								}
								facade.sendNotification(BagEvents.UPDATEITEMNUM);
								facade.sendNotification(NPCBusinessEvent.UPDATE_MONEY_LAST_NPCBUSINESS);	//更新跑商银子
								break;
							}
						}
					}
					sendNotification(EventList.ONSYNC_BAG_QUICKBAR,syncNum.type);
				break;
				
				///////////////////////////////////////////////
				//摆摊
				case DELSTALLITEM:
//					trace("23-删除商品");
					var stallItem:Object = new Object();
					stallItem.id 		 = bytes.readUnsignedInt();
					stallItem.type 	     = bytes.readUnsignedInt();	
					stallItem.amount 	 = bytes.readUnsignedInt();		
					stallItem.maxAmount  = bytes.readUnsignedInt();
					stallItem.position   = bytes.readUnsignedByte();	//200则表示摆摊物品
					stallItem.isBind     = bytes.readUnsignedByte();	//绑定物品不能摆摊  0-绑定，1-未绑定
					stallItem.index 	 = bytes.readUnsignedShort()-1;	//在格子数组中的位置，服务器从1开始，这里-1得到的是在数组中的下标
					stallItem.price	  	 = bytes.readUnsignedInt();		//单价
					stallItem.color	  	 = bytes.readUnsignedInt();		//颜色 （0-5）
					
					StallConstData.goodList[stallItem.index] = null;
					sendNotification(StallEvents.DELSTALLITEM, stallItem.id);
				break;
				case GETSTALLITEMS:
//					trace("21-查询摊位物品");
					sendNotification(StallEvents.SHOWSOMESTALL);
				break;
				case CLEARSTALLMSG:
//					trace("104-清除所有留言");
					StallConstData.stallMsg = [];
					sendNotification(StallEvents.UPDATESTALLMSG);
				break;
				case PET_ADD_STALL:					//添加宠物成功
					var idAdd:uint = bytes.readUnsignedInt();
					delete StallConstData.petListChoice[idAdd];
					GameCommonData.Player.Role.PetSnapList[idAdd].IsLock = true;
					StallConstData.petListSaleSelfIdArr.push(idAdd);
					sendNotification(StallEvents.UPDATE_PET_LIST_STALL);
				break;
				case PET_DEL_STALL:					//删除宠物成功
					var idDel:uint = bytes.readUnsignedInt();
					GameCommonData.Player.Role.PetSnapList[idDel].IsLock = false;
					StallConstData.petListChoice[idDel] = UIUtils.DeeplyCopy(GameCommonData.Player.Role.PetSnapList[idDel]);
					var idIndexPet:int = StallConstData.petListSaleSelfIdArr.indexOf(idDel);
					if(idIndexPet >= 0) {
						StallConstData.petListSaleSelfIdArr.splice(idIndexPet, 1);
					}
					sendNotification(StallEvents.UPDATE_PET_LIST_STALL);
				break;
				default:
				break;
			}
		}
		
	}
}