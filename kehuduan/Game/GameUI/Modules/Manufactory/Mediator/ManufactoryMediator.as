package GameUI.Modules.Manufactory.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.BagUtils;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Manufactory.Command.CheckCommitBtnCommand;
	import GameUI.Modules.Manufactory.Command.ClickManuSkirtCellCommand;
	import GameUI.Modules.Manufactory.Command.RefreshManuDataCommand;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.Proxy.ManufatoryProxy;
	import GameUI.Modules.Manufactory.View.AppendMaterialCell;
	import GameUI.Modules.Manufactory.View.EquipCell;
	import GameUI.Modules.Manufactory.View.ManufactoryResource;
	import GameUI.Modules.Manufactory.View.ManufatoryView;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ManufactoryMediator extends Mediator
	{
		public static const NAME:String = "ManufactoryMediator";
		private var panelBase:PanelBase;
		private var manufatoryRes:ManufactoryResource;
		private var dataProxy:DataProxy;
		private var manufatoryProxy:ManufatoryProxy;
		private var manufatoryView:ManufatoryView;
		private var aCurDataObj:Array;									//当前选择的数据数组
		private var aCurType:Array;										//当前物品种类
		private var aCurLevel:Array;										//当前物品等级
		private var aCurApend:Array;										//当前选择的附加材料数组
		private var eBgShape:Shape = new Shape();
		
		//需要发送给服务器的数据
//		private var selectEquipType:uint;
//		private var selectBaseTypeArr:Array;
		//三个打造技能的type
		private var stockSkillType:uint = 6004;
		private var leatherSkillType:uint = 6005;
		private var refinementSkillType:uint = 6006;
		
		private var maskShape:Shape = new Shape();
		
		public function ManufactoryMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get mainView():ManufatoryView
		{
			return this.viewComponent as ManufatoryView;
		}
		
		public override function listNotificationInterests():Array
		{
			return [ ManufactoryData.INIT_MANUFACTORY_UI,
					 ManufactoryData.SHOW_MANUFACTORY_UI,
					 ManufactoryData.CLOSE_MANUFACTORY_UI,
					 ManufactoryData.CLICK_MANU_SKIRT_CELL,
					 ManufactoryData.SELECT_EQUIP_MANUFA,
					 ManufactoryData.SELECT_APPEND_CELL,
					 ManufactoryData.CHANGE_SUCESS_RATE,
					 ManufactoryData.MANUFACTORY_SUCEED,
					 ManufactoryData.RESOURCE_FORM_TOOLTIP
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case ManufactoryData.INIT_MANUFACTORY_UI:
					if ( manufatoryRes == null )
					{
						initResource();	
					}
				break;
				
				case ManufactoryData.SHOW_MANUFACTORY_UI:
//					dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
					initProxy();
					if ( notification.getBody().view )
					{
						if ( !manufatoryView )
						{
							this.manufatoryView = new ManufatoryView( notification.getBody().view );
							this.setViewComponent( manufatoryView );	
						}
					}
					if ( !dataProxy.EquipManufatoryIsOpen )
					{
						initData();
						showUI();		
					}
					else
					{
						panelClose(null);
					}
				break;
				
				case ManufactoryData.CLOSE_MANUFACTORY_UI:
					initProxy();
					if ( dataProxy.EquipManufatoryIsOpen )
					{
						panelClose(null);	
					}
				break;
				
				case ManufactoryData.CLICK_MANU_SKIRT_CELL:
					clickSkirtCellHandler(notification.getBody());
				break;
				
				case ManufactoryData.SELECT_EQUIP_MANUFA: 
					selectEquipHandler(notification.getBody());
				break;
				
				case ManufactoryData.SELECT_APPEND_CELL:
					selectAppendHandler(notification.getBody());
				break;
				
				case ManufactoryData.CHANGE_SUCESS_RATE:
					changSucRate(notification.getBody().rate);
				break;
				
				case ManufactoryData.MANUFACTORY_SUCEED:
					sucessHandler();
				break;
				
				case ManufactoryData.RESOURCE_FORM_TOOLTIP:
					if ( !manufatoryView )
					{
						this.manufatoryView = new ManufatoryView( notification.getBody().view );
						this.setViewComponent( manufatoryView );	
					}
				break;
			}
		}
		
		private function initResource():void
		{	
//			facade.registerProxy( new ManufatoryProxy() );
			manufatoryRes = new ManufactoryResource();
		}
		
		private function initData():void
		{
			facade.registerCommand( ClickManuSkirtCellCommand.NAME,ClickManuSkirtCellCommand );
			facade.registerCommand( RefreshManuDataCommand.NAME,RefreshManuDataCommand );
			facade.registerCommand( CheckCommitBtnCommand.NAME,CheckCommitBtnCommand );
			
			ManufactoryData.ManufatoryCount = 1;
			( mainView.main_mc.count_txt as TextField ).text = ManufactoryData.ManufatoryCount.toString();
			facade.sendNotification( RefreshManuDataCommand.NAME );
		}
		
		private function showUI():void
		{
			if ( panelBase == null )
			{
				panelBase = new PanelBase(mainView.main_mc, mainView.main_mc.width+8, mainView.main_mc.height+15);
//				panelBase.x = 222;
//				panelBase.x = 185;
//				panelBase.y = 50;
				panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_man_med_man_initD" ]);//"生  产"
			}
			
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			ManufactoryData.selectAppendType = 0;
			ManufactoryData.selectScenoType = 0;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			dataProxy.EquipManufatoryIsOpen = true;
			panelBase.addEventListener(Event.CLOSE, panelClose);
			
			setBtnState();
			setListeners();
			mainView.listenSkirt();
			( mainView.main_mc.count_txt as TextField ).textColor = 0x000000;
			
			eBgShape.graphics.clear();
			eBgShape.graphics.beginFill( 0x878581 );
			eBgShape.graphics.drawRect(0,0,298,20);
			eBgShape.graphics.endFill();
		}
		
		private function setListeners():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( mainView.main_mc[ "titleBtn_"+i ] as MovieClip ).addEventListener( MouseEvent.CLICK,clickBtnHandler );	
			}
			
			( mainView.main_mc.commit_txt as TextField ).mouseEnabled = false;
			( mainView.main_mc.commit_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,commitManuHandler );
		}
		
		//单击按钮开始打造
		private function commitManuHandler(evt:MouseEvent):void
		{
			//采集原材料时不能制造
			if ( SkillData.isLifeSeeking )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_man_med_man_com_1" ], color:0xffff00});//"采集原料中，不能生产"
				return;
			}
			if ( GameCommonData.Player.IsAutomatism )
			{
				showHint( GameCommonData.wordDic[ "mod_man_med_man_com_2" ] );//"挂机时不能生产"
				return;
			}
			//判断技能等级
//			if ( !checkSkillLevel() )
//			{
//				return;
//			}
			//判断背包是否满
			ManufactoryData.selectScenoType = ManufactoryData.clickScenoType;
			ManufactoryData.selectAppendType = ManufactoryData.clickAppendType;
			if ( manufatoryProxy.allInfoDic[ ManufactoryData.selectScenoType ].kind == GameCommonData.wordDic[ "mod_man_med_man_com_3" ] )//"半成品"
			{
				if ( BagUtils.TestBagIsFull(1) + 1 > BagData.BagNum[1] )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_man_med_man_com_4" ], color:0xffff00});//"材料物品背包已满，请先清理背包"
					return;
				}
			}else
			{
				if ( BagUtils.TestBagIsFull(0) + ManufactoryData.ManufatoryCount > BagData.BagNum[0] )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_man_med_man_com_5" ], color:0xffff00});//"普通物品背包空间不够，请先清理背包"
					return;
				}
			}
			
			mainView.startReadingBar();
		}
		
		private function clickBtnHandler(evt:MouseEvent):void
		{
			var index:uint = uint( ( evt.currentTarget as MovieClip ).name.split("_")[1] );
			if ( index == ManufactoryData.curTitleBtn ) return;
			ManufactoryData.curTitleBtn = index;
			setBtnState();
		}
		
		//单击下拉列表
		private function clickSkirtCellHandler(obj:Object):void
		{
			obj.dataObj = aCurDataObj;
			obj.view = this.mainView;
			facade.sendNotification( ClickManuSkirtCellCommand.NAME,obj );
		}
		
		//选中左边装备
		private function selectEquipHandler(obj:Object):void
		{
			( obj.equipCell as EquipCell ).highLight( eBgShape );
			mainView.showBaseItem( obj.dataObj.needBase );
//			trace ( obj.dataObj.kind );
			if ( obj.dataObj.kind != GameCommonData.wordDic[ "mod_man_med_man_com_3" ] )//"半成品"
			{
				mainView.showAppendItem( aCurApend );
				if ( ManufactoryData.selectAppendType == 0 )
				{
					mainView.changTextRate( manufatoryProxy.defaultRate );
				}
			}
			else
			{
				mainView.selectUnfinshProduct();
			}
		}
		
		//单击选中附加材料
		private function selectAppendHandler(obj:Object):void
		{
			mainView.defaultAppendCells( obj.appendCell );
			if ( ( obj.appendCell as AppendMaterialCell ).isSelect==true )
			{
				( obj.appendCell as AppendMaterialCell ).isSelect = false;
			}
			else
			{
				( obj.appendCell as AppendMaterialCell ).isSelect = true;
			}
		}
		
		//改变概率
		private function changSucRate(rArr:Array):void
		{
			if ( ManufactoryData.clickAppendType == 0 )
			{
				mainView.changTextRate( manufatoryProxy.defaultRate );
				return;
			}
			else
			{
				mainView.changTextRate( rArr );
			}
		}
		
		private function sucessHandler():void
		{
			ManufactoryData.ManufatoryCount --;
			if ( ManufactoryData.ManufatoryCount>0 )
			{
				mainView.startReadingBar();
			}
			else
			{
//				mainView.ctrlCommitBtn(false);
//				( mainView.main_mc.count_txt as TextField ).textColor = 0xff0000;
				mainView.stopReadingBar();
				ManufactoryData.ManufatoryCount = 1;
			}
			mainView.keepStep();
			( mainView.main_mc.count_txt as TextField ).text = ManufactoryData.ManufatoryCount.toString();
			sendNotification( CheckCommitBtnCommand.NAME );
		}
		
		private function setBtnState():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( mainView.main_mc[ "titleBtn_"+i ] as MovieClip ).gotoAndStop(2);
				( mainView.main_mc[ "titleBtn_"+i ] as MovieClip ).buttonMode = true;
			}
			( mainView.main_mc[ "titleBtn_"+ManufactoryData.curTitleBtn ] as MovieClip ).gotoAndStop(1);
			//设置当前数据数组
			switch ( ManufactoryData.curTitleBtn )
			{
				case 0:
					aCurDataObj = manufatoryProxy.aStockInfo;
					aCurType = manufatoryProxy.aStockType;
					aCurApend = manufatoryProxy.aStockAppend;
				break;
				case 1:
					aCurDataObj = manufatoryProxy.aLeatherInfo;
					aCurType = manufatoryProxy.aLeatherType;
					aCurApend = manufatoryProxy.aLeartherAppend;
				break;
				case 2:
					aCurDataObj = manufatoryProxy.aRefinementInfo;
					aCurType = manufatoryProxy.aRefinementType;
					aCurApend = manufatoryProxy.aRefinementAppend;
				break;
				default:
					
				break;
			}
//			if ( !ManufactoryData.isReadingBar )
//			{
//				ManufactoryData.selectAppendType = 0;
//				ManufactoryData.selectScenoType = 0;
//			}
			ManufactoryData.clickScenoType = 0;
			ManufactoryData.clickAppendType = 0;
			mainView.creatSkirtList( aCurType,manufatoryProxy.aItemLevel );
			mainView.setItemTypeTitle( GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ] );//"物品种类"
			mainView.setItemLevelTitle( GameCommonData.wordDic[ "mod_man_com_cli_exe_2" ] );//"物品等级"
			mainView.panelCleanUp();
			
			sendNotification( ClickManuSkirtCellCommand.NAME,{ view:mainView,dataObj:aCurDataObj } );
		}
		
		private function panelClose(evt:Event):void
		{
			if ( panelBase )
			{
				if ( GameCommonData.GameInstance.WorldMap.contains( panelBase ) )
				{
					GameCommonData.GameInstance.WorldMap.removeChild( panelBase );
				}
				else if ( GameCommonData.GameInstance.GameUI.contains( panelBase ) )
				{
					GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				}
			}
			dataProxy.EquipManufatoryIsOpen = false;
			if ( ManufactoryData.isReadingBar )
			{
				mainView.stopReadingBar();	
			}
			( mainView.main_mc.commit_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,commitManuHandler );
				
			for ( var i:uint=0; i<3; i++ )
			{
				( mainView.main_mc[ "titleBtn_"+i ] as MovieClip ).removeEventListener( MouseEvent.CLICK,clickBtnHandler );	
			}
			panelBase.removeEventListener(Event.CLOSE, panelClose);
				//移除command
			facade.removeCommand( CheckCommitBtnCommand.NAME );
			facade.removeCommand( ClickManuSkirtCellCommand.NAME );
			facade.removeCommand( RefreshManuDataCommand.NAME );
		}
		
		//判断技能等级是否足够
		private function checkSkillLevel():Boolean
		{
			var obj:Object = manufatoryProxy.allInfoDic[ ManufactoryData.selectScenoType ];
			if ( !obj ) return false;
			
			if ( obj.classTpye == "stock" )
			{
				if ( !GameCommonData.Player.Role.LifeSkillList[ stockSkillType ] )
				{
					showHint( "锻造技能未学会，无法生产" );
					return false;
				}
				else
				{
					var sLevel:uint = ( GameCommonData.Player.Role.LifeSkillList[ stockSkillType ] as GameSkillLevel ).Level;
					if ( obj.kind == "半成品" )
					{
						if ( obj.level>sLevel )
						{
							showHint( "锻造技能等级不够" );
							return false;
						}
					}
					else
					{
						if ( obj.level>sLevel*10+30 )
						{
							showHint( "锻造技能等级不够" );
							return false;
						}
					}
				}
			}
			
			if ( obj.classTpye == "leather" )
			{
				if ( !GameCommonData.Player.Role.LifeSkillList[ leatherSkillType ] )
				{
					showHint( "制皮技能未学会，无法生产" );
					return false;
				}
				else
				{
					var leLevel:uint = ( GameCommonData.Player.Role.LifeSkillList[ leatherSkillType ] as GameSkillLevel ).Level;
					if ( obj.kind == "半成品" )
					{
						if ( obj.level>leLevel )
						{
							showHint( "制皮技能等级不够" );
							return false;
						}
					}
					else
					{
						if ( obj.level>leLevel*10+30 )
						{
							showHint( "制皮技能等级不够" );
							return false;
						}
					}
				}
			}
			
			if ( obj.classTpye == "refinement" )
			{
				if ( !GameCommonData.Player.Role.LifeSkillList[ refinementSkillType ] )
				{
					showHint( "精工技能未学会，无法生产" );
					return false;
				}
				else
				{
					var reLevel:uint = ( GameCommonData.Player.Role.LifeSkillList[ refinementSkillType ] as GameSkillLevel ).Level;
					if ( obj.kind == "半成品" )
					{
						if ( obj.level>reLevel )
						{
							showHint( "精工技能等级不够" );
							return false;
						}
					}
					else
					{
						if ( obj.level>reLevel*10+30 )
						{
							showHint( "精工技能等级不够" );
							return false;
						}
					}
				}
			}
			
			return true;
		}
		
		private function showHint(info:String):void
		{
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:info, color:0xffff00});
		}
		
		//注册2个proxy
		private function initProxy():void
		{
			if ( !dataProxy )
			{
				dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			}
			if ( !manufatoryProxy )
			{
//				manufatoryProxy = facade.registerProxy( new ManufatoryProxy() ) as ManufatoryProxy;
				manufatoryProxy = facade.retrieveProxy( ManufatoryProxy.NAME ) as ManufatoryProxy;
			}
		}
		
	}
}