package GameUI.Modules.ToolTip.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Modules.ToolTip.Mediator.UI.BallAndTicketToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.BusinessItemToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.EquipToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.ExpToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.FormulaToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.IToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.MasterItemTooltip;
	import GameUI.Modules.ToolTip.Mediator.UI.SetBloodToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.SetCompensateStorageItemToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.SetItemToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.ShopGoodToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.SkillToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.SoulSkillToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.SoulTooltip;
	import GameUI.Modules.ToolTip.Mediator.UI.TextToolTip;
	import GameUI.Proxy.DataProxy;
	
	import Net.ActionProcessor.ItemInfo;
	
	import OopsEngine.Role.GameRole;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ItemToolTipMediator extends Mediator
	{
		public static const NAME:String = "ItemToolTipMediator";
		
		private var itemToolTip:Sprite = null;
		private var parallelEquip_1:Sprite = null;
		private var parallelToolTip_1:IToolTip = null;
		private var parallelEquip_2:Sprite = null;
		private var parallelToolTip_2:IToolTip = null;
		private var setItemToolTip:IToolTip = null;
		private var isParallel:Boolean = false;
		private var isExp:Boolean = false;
		private var hasDetectYPos:Boolean = false;
		
		private var curDataType:uint = 0;	//当前悬浮框的data.type  用于位置变动changePos时对坐标的控制
		
		public function ItemToolTipMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOWITEMTOOLTIP,
				EventList.REMOVEITEMTOOLTIP,
				EventList.MOVEITEMTOOLTIP,
				IntroConst.SHOWPARALLEL,
				EventList.SHOW_NPC_SHOP_TOOLTIP
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				
				case EventList.SHOWITEMTOOLTIP:
				{
					removeToolTip();
//					removeParallelEquip();
					if(!UIConstData.ToolTipShow) return;	//ToolTip开关，防止服务器数据返回时鼠标已经不在图标上了。
					itemToolTip = new Sprite();
					itemToolTip.mouseEnabled = false;
					itemToolTip.mouseChildren = false;
					var data:Object = notification.getBody();					
					/**
					 * 根据Type 显示不同的ToolTip
					 * 
					 */
					curDataType = data.type;	//--------------------------当前悬浮框的data.type  用于位置变动changePos时对坐标的控制
					
					//药品type
					if(data.type > 300000 && data.type < 400000)
					{
						var typeMul:uint = data.type / 1000;
						if(typeMul == 301 || typeMul == 311 || typeMul == 321) {			//大血大蓝
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data, false);
							setValues();
						} else if(typeMul == 351) {
							isExp = false;
							hasDetectYPos = false;
							if(data.type == 351001)	//元宝票
							{
								setItemToolTip = new BallAndTicketToolTip(itemToolTip, data.data, false);
							}
							else
							{
								setItemToolTip = new SetItemToolTip(itemToolTip, data.data, false);
							}
							setValues();
						} else {
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data);
							setValues();
						}
					}
					//宝石type
					else if(data.type >= 400000 && data.type < 500000)
					{
						isExp = false;
						hasDetectYPos = false;
						setItemToolTip = new EquipToolTip(itemToolTip, data.data, false);
						setValues();	
					}
					//礼包
					else if(data.type >= 500000 && data.type < 600000)
					{
						isExp = false;
						hasDetectYPos = false; 
						if ( data.type>=506000 && data.type<=506005 )
						{
//							if ( data.type == 506000 )
//							{
//								UiNetAction.GetItemInfo( data.data.id, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name );
//							}
							setItemToolTip = new MasterItemTooltip( itemToolTip,data.data );
							setValues();	
						}
						else if ( data.type>521000 && data.type<523999 )		//配方
						{
							setItemToolTip = new FormulaToolTip(itemToolTip, data.data);
							setValues();	
						}
						else
						{
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data);	
							setValues();	
						}
					}
					//打孔宝石  跑商商品、银票
					else if(data.type >= 600000 && data.type < 700000)
					{
						var typeB:uint = data.type / 1000;
						if(typeB == 626) {			//跑商商品
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new BusinessItemToolTip(itemToolTip, data.data);
							setValues();
						} else {
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data);
							setValues();
						}
					}
					//血条
					else if(data.type == 999991)
					{	
						isExp = false;
						hasDetectYPos = false;
						var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;				 
						if(data.index != undefined)
						{
							var info:GameRole = dataProxy.roleDatas[data.index]	as GameRole;
						}
						setItemToolTip = new SetBloodToolTip(itemToolTip, data.role, info, dataProxy.isSelectSelf);
						setValues();
					}
					//所有的文字信息，在IntroConst里面有文字信息
					else if(data.type == 999992)
					{	
						hasDetectYPos = true;
						if(data.change != undefined)
						{
							isExp = data.change;
						}
						else
						{
							isExp = false;
						}
						setItemToolTip = new TextToolTip(itemToolTip, data.data);
						setValues();
					}
					//经验条
					else if(data.type == 999993)
					{
						isExp = true;
						hasDetectYPos = false;
						setItemToolTip = new ExpToolTip(itemToolTip);	
						setValues();
					}
					//技能
					else if(data.type == 999994)
					{
						isExp = false;
						hasDetectYPos = false;
						setItemToolTip = new SkillToolTip(itemToolTip, data.data, data.isLearn);
						setValues();
					} 
					else if (data.type == 999995) // 音乐播放器列表项目
					{
						isExp = false;
						hasDetectYPos = false;
						setItemToolTip = new TextToolTip(itemToolTip, data.data.data);
						(setItemToolTip as TextToolTip).fixedWidth = 175;
						setValues();
						var offset:Point = (data.data.anchor as DisplayObject).localToGlobal(new Point(0, 0)); 
						itemToolTip.x = offset.x - itemToolTip.width - 3;
						itemToolTip.y = offset.y;
					}
					else if ( data.type == 33338888 )
					{
						isExp = false;
						hasDetectYPos = false;
						setItemToolTip = new SoulSkillToolTip( itemToolTip, data.data );
						setValues();
					}
					else if ( data.type >= 250000 && data.type <= 300000 )
					{
						isExp = false;
						isParallel = false;
						hasDetectYPos = false;
						setItemToolTip = new SoulTooltip( itemToolTip, data.data );
						setValues();
					}
					//装备
					else if(data.type < 250000)
					{
						if(!UIConstData.ToolTipShow) return;
						isExp = false;
						isParallel = false;
						hasDetectYPos = false;
						setItemToolTip = new EquipToolTip(itemToolTip, data.data, data.isEquip);
						itemToolTip.name = "itemToolTip";
						setValues();
						if(data.isEquip)
						{
							return;
						}
						for(var i:int = 0; i<RolePropDatas.ItemList.length; i++)
						{
							if(RolePropDatas.ItemList[i] == undefined) continue;
							
							var typeDataItem:int  = data.data.type/10000;
							var typeEquipItem:int = RolePropDatas.ItemList[i].type/10000;
							var idEquipItem:int   = RolePropDatas.ItemList[i].id;
							
							if(typeDataItem == typeEquipItem)
							{
								isParallel = true;
								if(IntroConst.ItemInfo[idEquipItem] == undefined)
								{
//									ItemInfo.isParallel = true;
									ItemInfo.queryIdList[idEquipItem] = idEquipItem;
									UiNetAction.GetItemInfo(idEquipItem, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
								}
								else
								{
									removeParallelEquip();
									if(needShowTowParallel(typeDataItem)) {						//需要显示两个悬浮框
										var introArr:Array = canShowTwoParallel(typeDataItem);	//双悬浮框装备数据
										if(introArr.length == 2) {								//数据已齐全(=2个)可以显示两个悬浮框
											showTowParallel(introArr);
											setParallelValues();
											break;
										}
									} else {
										if(!UIConstData.ToolTipShow) return;
										parallelEquip_1 = new Sprite();
										parallelEquip_1.name = "parallelEquip_1";
										parallelEquip_1.mouseEnabled = false;
										parallelEquip_1.mouseChildren = false;
										parallelToolTip_1 = new EquipToolTip(parallelEquip_1, IntroConst.ItemInfo[idEquipItem], true, false, null, true);
										parallelToolTip_1.Show();
										GameCommonData.GameInstance.TooltipLayer.addChild(parallelEquip_1);
										if(typeDataItem != 20) {	//非坐骑
											if(setItemToolTip is EquipToolTip) {
												var scoreArr:Array = [];
												scoreArr.push((parallelToolTip_1 as EquipToolTip).getScore);
												(setItemToolTip as EquipToolTip).compareScore(scoreArr);
											}
										}
										setParallelValues();
									}
								}
								if(typeDataItem == 21 || typeDataItem == 22) {		//戒指&饰品需要判断两个
									continue;
								} else {
									break;
								}
							}
						}
					}
					else if(data.type > 10000000 && data.type <= 11000000)	//补偿仓库悬浮框
					{
						isExp = false;
						hasDetectYPos = false;
						data.type -= 10000000;
						if(data.data && data.data.type)
						{
							data.data.type -= 10000000;
						}
						setItemToolTip = new SetCompensateStorageItemToolTip(itemToolTip, data.data);
						setValues();
					}
			    }
				break;
				case EventList.SHOW_NPC_SHOP_TOOLTIP:												//显示NPC商店·
				{
					removeToolTip();
					itemToolTip = new Sprite();
					itemToolTip.mouseEnabled = false;
					itemToolTip.mouseChildren = false;
					data = notification.getBody();					
					/**
					 * 根据Type 显示不同的ToolTip
					 * 
					 */
					 
					 curDataType = data.type;	//--------------------------当前悬浮框的data.type  用于位置变动changePos时对坐标的控制
					 
					//药品type
					if(data.type > 300000 && data.type < 400000) 
					{
						typeMul = data.type / 1000;
						if(typeMul == 301 || typeMul == 311 || typeMul == 321) {			//大血大蓝
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data, false);
							setValues();
						} else {
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data);
							setValues();
						}
					}
					//宝石type
					else if(data.type >= 400000 && data.type < 500000)
					{
						isExp = false;
						hasDetectYPos = false;
						setItemToolTip = new EquipToolTip(itemToolTip, data.data, false);
						setValues();	
					}
					//礼包
					else if(data.type >= 500000 && data.type < 600000)
					{
						isExp = false;
						hasDetectYPos = false; 
						if ( data.type>=506000 && data.type<=506005 )
						{
							setItemToolTip = new MasterItemTooltip( itemToolTip,data.data );
							setValues();	
						}
						else if ( data.type>521000 && data.type<523999 )		//配方
						{
							setItemToolTip = new FormulaToolTip(itemToolTip, data.data);
							setValues();	
						}
						else
						{
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data);	
							setValues();	
						}
					}
					//打孔宝石
					else if(data.type >= 600000 && data.type < 700000)
					{
						var typeNPCGOOD:uint = data.type / 1000;
						if(typeNPCGOOD == 626) {			//跑商商品
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new BusinessItemToolTip(itemToolTip, data.data);
							setValues();
						} else {
							isExp = false;
							hasDetectYPos = false;
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data);
							setValues();	
						}
					}
					//装备
					else if(data.type < 300000)
					{
						isExp = false;
						isParallel = false;
						hasDetectYPos = false;
						setItemToolTip = new ShopGoodToolTip(itemToolTip, data.data, data.isEquip);
						itemToolTip.name = "itemToolTip";
						setValues();
						if(data.isEquip)
						{
							return;
						}
						for(i = 0; i<RolePropDatas.ItemList.length; i++)
						{
							if(RolePropDatas.ItemList[i] == undefined) continue;
							
							var typeDataItemShop:int  = data.data.type/10000;
							var typeEquipItemShop:int = RolePropDatas.ItemList[i].type/10000;
							var idEquipItemShop:int   = RolePropDatas.ItemList[i].id;
							
							if(typeDataItemShop == typeEquipItemShop)
							{
								isParallel = true;
								if(IntroConst.ItemInfo[idEquipItemShop] == undefined)
								{
//									ItemInfo.isParallel = true;
									ItemInfo.queryIdList[idEquipItemShop] = idEquipItemShop;
									UiNetAction.GetItemInfo(idEquipItemShop, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
								}
								else
								{
									removeParallelEquip();
									if(needShowTowParallel(typeDataItemShop)) {								//需要显示两个悬浮框
										var introArrShop:Array = canShowTwoParallel(typeDataItemShop);		//双悬浮框装备数据
										if(introArrShop.length == 2) {										//数据已齐全(=2个)可以显示两个悬浮框
											showTowParallel(introArrShop);
											setParallelValues();
											break;
										}
									} else {
										if(!UIConstData.ToolTipShow) return;
										parallelEquip_1 = new Sprite();
										parallelEquip_1.name = "parallelEquip_1";
										parallelEquip_1.mouseEnabled = false;
										parallelEquip_1.mouseChildren = false;
										parallelToolTip_1 = new EquipToolTip(parallelEquip_1, IntroConst.ItemInfo[idEquipItemShop], true, false, null, true);
										parallelToolTip_1.Show();
										GameCommonData.GameInstance.TooltipLayer.addChild(parallelEquip_1);
										setParallelValues();
									}
								}
								if(typeDataItemShop == 21 || typeDataItemShop == 22) {		//戒指&饰品需要判断两个
									continue;
								} else {
									break;
								}
							}
							
						}
					}
				}
				break;
				case EventList.REMOVEITEMTOOLTIP:
					UIConstData.ToolTipShow = false;		//9.11.2010
					removeToolTip();
					curDataType = 0;
				break;
				case EventList.MOVEITEMTOOLTIP:
					if(!itemToolTip) return;
					if(isExp)
					{
						itemToolTip.x = GameCommonData.GameInstance.TooltipLayer.mouseX 
						itemToolTip.y = GameCommonData.GameInstance.TooltipLayer.mouseY - itemToolTip.height;
					}
//					else
//					{
//						itemToolTip.x = GameCommonData.GameInstance.TooltipLayer.mouseX + 15;	// + 25;
//						itemToolTip.y = GameCommonData.GameInstance.TooltipLayer.mouseY + 15;	// + 25;
//					}
//					if(isParallel)
//					{
//						if(parallelEquip_1)
//						{
//							parallelEquip_1.x = itemToolTip.x + itemToolTip.width + 10;
//							parallelEquip_1.y = itemToolTip.y;
//						}
//						if(parallelEquip_2)
//						{
//							parallelEquip_2.x = parallelEquip_2.x + parallelEquip_2.width + 10;
//							parallelEquip_2.y = itemToolTip.y;
//						}
//					}
					changePos();
				break;
				case IntroConst.SHOWPARALLEL:
					removeParallelEquip();
					if(!setItemToolTip) return;
					var typeEq:int = int(setItemToolTip.GetType()) / 10000;
					if(needShowTowParallel(typeEq)) {
						var eqData:Array = canShowTwoParallel(typeEq);
						if(eqData.length == 2) {
							showTowParallel(eqData);
						}
					} else {
						var dataToCompare:Object = notification.getBody();
						parallelEquip_1 = new Sprite();
						parallelEquip_1.mouseEnabled = false;
						parallelEquip_1.mouseChildren = false;
						parallelToolTip_1 = new EquipToolTip(parallelEquip_1, dataToCompare, true, false, null, true);
						parallelEquip_1.name = "parallelEquip_1";
						parallelToolTip_1.Show();
						GameCommonData.GameInstance.TooltipLayer.addChild(parallelEquip_1);
						if(int(dataToCompare.type/10000) != 20) {	//非坐骑
							if(setItemToolTip is EquipToolTip) {
								var scoreArrData:Array = [];
								scoreArrData.push((parallelToolTip_1 as EquipToolTip).getScore);
								(setItemToolTip as EquipToolTip).compareScore(scoreArrData);
							}
						}
					}
					setParallelValues();
				break;
			}
		}
		
		private function setValues():void
		{
			setItemToolTip.Show();				
			GameCommonData.GameInstance.TooltipLayer.addChild(itemToolTip);
			if(isExp)
			{
				itemToolTip.x = GameCommonData.GameInstance.TooltipLayer.mouseX;
				itemToolTip.y = GameCommonData.GameInstance.TooltipLayer.mouseY - itemToolTip.height;
			}
			else
			{
				itemToolTip.x = GameCommonData.GameInstance.TooltipLayer.mouseX + 15;	// + 25;
				itemToolTip.y = GameCommonData.GameInstance.TooltipLayer.mouseY + 15;	// + 25;
				changePos();
			}
		}
		
		private function setParallelValues():void
		{
			if(parallelEquip_2 && parallelEquip_1 && itemToolTip) {
				if(itemToolTip.x > 1000 - itemToolTip.width)
				{
					itemToolTip.x = itemToolTip.x - setItemToolTip.BackWidth();
				}
				if(itemToolTip.y > 580 - itemToolTip.height)
				{
					itemToolTip.y = itemToolTip.y - itemToolTip.height;
				}
				
				parallelEquip_1.x = itemToolTip.x + itemToolTip.width-5;
				parallelEquip_2.x = parallelEquip_1.x + parallelEquip_1.width-25;
				
				if(parallelEquip_2.x > 1100 - parallelEquip_2.width)
				{
					parallelEquip_1.x = itemToolTip.x - parallelEquip_1.width;
					parallelEquip_2.x = parallelEquip_1.x - parallelEquip_2.width;
				}
				if(itemToolTip.y < 0)
				{
					itemToolTip.y = 0;
				}
				parallelEquip_1.y = itemToolTip.y;
				parallelEquip_2.y = itemToolTip.y;
				
				if(parallelEquip_2.y > 580 - parallelEquip_2.height)
				{
					parallelEquip_1.y = 0;
					parallelEquip_2.y = 0;
				}
			}
			else if(parallelEquip_1 && itemToolTip)
			{
				if(itemToolTip.x > 1000 - itemToolTip.width)
				{
					itemToolTip.x = itemToolTip.x - setItemToolTip.BackWidth();
				}
				if(itemToolTip.y > 580 - itemToolTip.height)
				{
					itemToolTip.y = itemToolTip.y - itemToolTip.height;
				}
				parallelEquip_1.x = itemToolTip.x + itemToolTip.width;
				if(parallelEquip_1.x > 1000 - parallelEquip_1.width)
				{
					parallelEquip_1.x = itemToolTip.x - parallelEquip_1.width;
				}
				if(itemToolTip.y < 0)
				{
					itemToolTip.y = 0;
				}
				parallelEquip_1.y = itemToolTip.y;
				if(parallelEquip_1.y > 580 - parallelEquip_1.height)
				{
					parallelEquip_1.y = 0;
				}
			}
		}
		
		private function changePos():void
		{
			if(itemToolTip.x > 1000 - itemToolTip.width + GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth)
			{
				if(curDataType > 999990) {		//用于位置变动changePos时对坐标的控制
					itemToolTip.x = itemToolTip.x - setItemToolTip.BackWidth(); 
				} else {
					itemToolTip.x = itemToolTip.x - setItemToolTip.BackWidth() - 40; 
				}
			}
//			if(hasDetectYPos)
//			{
			if(itemToolTip.y > 580 - itemToolTip.height + GameCommonData.GameInstance.TooltipLayer.stage.stageHeight - GameConfigData.GameHeight)
			{
				itemToolTip.y = itemToolTip.y - itemToolTip.height - 25; 
			}
			if(itemToolTip.y < 0)
			{
				itemToolTip.y = 0;
			}
//			}
			if(isParallel)
			{
				if(parallelEquip_2) 
				{
					if(parallelEquip_2.x > 1100 - parallelEquip_2.width)
					{
						parallelEquip_1.x = itemToolTip.x - parallelEquip_1.width-5;
						parallelEquip_2.x = parallelEquip_1.x - parallelEquip_2.width-25;
					}
					//
					if(parallelEquip_2.y > 580 - parallelEquip_2.height)
					{
						parallelEquip_1.y = parallelEquip_1.y - parallelEquip_1.height - 25;
						parallelEquip_2.y = parallelEquip_1.y;
					}
					if(parallelEquip_2.y < 0)
					{
						parallelEquip_1.y = 0;
						parallelEquip_2.y = 0;
					}
					//
				}
				else if(parallelEquip_1)
				{
					if(parallelEquip_1.x > 1000 - parallelEquip_1.width)
					{
						parallelEquip_1.x = itemToolTip.x - parallelEquip_1.width;
					}
					//
					if(parallelEquip_1.y > 580 - parallelEquip_1.height)
					{
						parallelEquip_1.y = parallelEquip_1.y - parallelEquip_1.height - 25;
					} 
					if(parallelEquip_1.y < 0)
					{
						parallelEquip_1.y = 0;
					}
					//
				}
			}
		}
		
		private function removeToolTip():void
		{
			if(itemToolTip)
			{
				if(GameCommonData.GameInstance.TooltipLayer.contains(itemToolTip))
				{
					GameCommonData.GameInstance.TooltipLayer.removeChild(itemToolTip);
					setItemToolTip.Remove();
					setItemToolTip = null;
					itemToolTip = null;
				}
			}
			removeParallelEquip();
		}
		
		private function removeParallelEquip():void
		{
			if(parallelEquip_1)
			{
				if(GameCommonData.GameInstance.TooltipLayer.contains(parallelEquip_1))
				{
					GameCommonData.GameInstance.TooltipLayer.removeChild(parallelEquip_1);
					parallelToolTip_1.Remove();
					parallelToolTip_1 = null;
					parallelEquip_1 = null;
				}
			}
			if(parallelEquip_2)
			{
				if(GameCommonData.GameInstance.TooltipLayer.contains(parallelEquip_2))
				{
					GameCommonData.GameInstance.TooltipLayer.removeChild(parallelEquip_2);
					parallelToolTip_2.Remove();
					parallelToolTip_2 = null;
					parallelEquip_2 = null;
				}
			}
		}
		
		/** 
		 * 是否需要显示两个装备悬浮框
		 * @param type 当前正在显示的悬浮框物品数据type/10000
		 * @return Boolean 是否需要显示双悬浮框 
		 */
		private function needShowTowParallel(type:int):Boolean
		{
			var res:Boolean = false;
			var typeEu:int  = 0;
			var count:uint  = 0;
			for(var i:int = 0; i < RolePropDatas.ItemList.length; i++) {
				if(RolePropDatas.ItemList[i] == undefined) {
					continue;
				} 
				typeEu = RolePropDatas.ItemList[i].type / 10000;
				if(type == typeEu && (type == 21 || type == 22)) {
					count++;
				}
			}
			if(count == 2) {
				res = true;
			}
			return res;
		}
		
		/** 
		 * 是否可以显示两个装备悬浮框，两个悬浮框数据都有的时候才可以显示
		 * @param type 当前正在显示的悬浮框物品数据type/10000
		 * @return Boolean 是否可以显示双悬浮框
		 */
		private function canShowTwoParallel(type:int):Array
		{
			var res:Array = [];
			var typeEu:int  = 0;
			for(var i:int = 0; i < RolePropDatas.ItemList.length; i++) {
				if(RolePropDatas.ItemList[i] == undefined) {
					continue;
				}
				typeEu = RolePropDatas.ItemList[i].type / 10000;
				if(type == typeEu && (type == 21 || type == 22)) {
					var data:Object = IntroConst.ItemInfo[RolePropDatas.ItemList[i].id];
					if(data) {
						res.push(data);
					}
				}
			}
			return res;
		}
		
		/** 显示双装备悬浮框 */
		private function showTowParallel(data:Array):void
		{
			parallelEquip_1 = new Sprite();
			parallelEquip_1.name = "parallelEquip_1";
			parallelEquip_1.mouseEnabled = false;
			parallelEquip_1.mouseChildren = false;
			parallelToolTip_1 = new EquipToolTip(parallelEquip_1, data[0], true, false, null, true);
			parallelToolTip_1.Show();
			
			parallelEquip_2 = new Sprite();
			parallelEquip_2.name = "parallelEquip_2";
			parallelEquip_2.mouseEnabled = false;
			parallelEquip_2.mouseChildren = false;
			parallelToolTip_2 = new EquipToolTip(parallelEquip_2, data[1], true, false, null, true);
			parallelToolTip_2.Show();
			
			GameCommonData.GameInstance.TooltipLayer.addChild(parallelEquip_1);
			GameCommonData.GameInstance.TooltipLayer.addChild(parallelEquip_2);
			
			if(setItemToolTip is EquipToolTip) {
				var scoreArr:Array = [];
				scoreArr.push((parallelToolTip_1 as EquipToolTip).getScore);
				scoreArr.push((parallelToolTip_2 as EquipToolTip).getScore);
				(setItemToolTip as EquipToolTip).compareScore(scoreArr);
			}
		}
		
		
	}
}