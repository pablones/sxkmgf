package GameUI.Modules.IdentifyTreasure.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Modules.IdentifyTreasure.Net.TreasureNet;
	import GameUI.Modules.IdentifyTreasure.UI.TreaGridManager;
	import GameUI.Modules.IdentifyTreasure.UI.TreaGridUnit;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TreasurePackageMediator extends Mediator
	{
		public static const NAME:String = "TreasurePackageMediator";
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		private var curBtn:uint = 0;
		private var gridManager:TreaGridManager;
		private var dataProxy:DataProxy;
		
		private var currentPage:uint = 1;									//默认第一页
		private var totalPage:uint = 4;
		private var getOutTimer:Timer = new Timer(1000, 1);	//物品取出计时器
		
		//5个数据数组
		private var aDrag:Array = [];
		private var aEquip:Array = [];
		private var aStone:Array = [];
		private var aPet:Array = [];
		private var aStuff:Array = [];			//杂货
		
		private var aCurData:Array = [];
		
		public function TreasurePackageMediator( mediatorName:String=null, viewComponent:Object=null )
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							TreasureData.OPEN_MY_TREA_PACKAGE,
							TreasureData.CLOSE_MY_TREA_PACKAGE,
							TreasureData.CLICK_SINGLE_GRID,
							TreasureData.DOUBLE_CLICK_GRID,
							TreasureData.CHANGE_TREA_PACK_DATA
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case TreasureData.OPEN_MY_TREA_PACKAGE:
					openPackage();
				break;
				case TreasureData.CLOSE_MY_TREA_PACKAGE:
					closePackage( null );
				break;
				case TreasureData.CLICK_SINGLE_GRID:
					clickGridHandler( notification.getBody() );
				break;
				case TreasureData.DOUBLE_CLICK_GRID:
					doubleClickGridHandler( notification.getBody() );
				break;
				case TreasureData.CHANGE_TREA_PACK_DATA:
					if ( gridManager )
					{
						changPackageItem();	
					}
				break;
			}
		}
		
		private function openPackage():void
		{
			if ( !main_mc )
			{
				initUI();
				initGridManager();
			}
			panelBase.addEventListener( Event.CLOSE,closePackage );
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			dataProxy.treasurePackageIsOpen = true;
			
			if ( TreasureData.selectGrid == null )
			{
				setTwoBtnVisible( false );
			}
			
//			if ( TreasureData.packageDateArr.length == 0 ) 
//			{
//				setRemAllBtn( false );
//			}
//			else
//			{
//				setRemAllBtn( true );
//			}
			
			initBtns();
			changPackageItem();
		}
		
		private function initUI():void
		{
			main_mc = this.viewComponent as MovieClip;
			panelBase = new PanelBase( main_mc, main_mc.width+8, main_mc.height+15 );
			panelBase.x = UIConstData.DefaultPos2.x;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_ide_med_treasureP_initU" ]);//"宝物包裹"
			
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy; 
		}
		
		private function initGridManager():void
		{
			gridManager = new TreaGridManager();
			gridManager.x = 5;
			gridManager.y = 22;
			main_mc.addChild( gridManager );
		}
		
		private function initBtns():void
		{
			setBtnState();
			for ( var i:uint=0; i<6; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).buttonMode = true;
				( main_mc[ "mcPage_"+i ] as MovieClip ).addEventListener( MouseEvent.CLICK,chgBtn );
			}
			//翻页按钮
			( main_mc.left_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goRight );
			
			( main_mc.throw_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,diuQi );
			( main_mc.remove_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,quChu );
			( main_mc.removeAll_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,quChuAll );
		}
		
		//点击改变页签
		private function chgBtn( evt:MouseEvent ):void
		{
			var index:uint = uint( evt.target.name.split( "_" )[1] );		
			if ( index == curBtn ) return;									
			TreasureData.selectGrid = null;
			setTwoBtnVisible( false );
			currentPage = 1;
			curBtn = index;																											
			setBtnState();		
			showItems();		
			
			//新加
			if ( aCurData.length == 0 )
			{
				setRemAllBtn( false );
			}
			else
			{
				setRemAllBtn( true );
			}																							
		}
		
		private function setBtnState():void
		{
			for ( var i:uint=0; i<6; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).gotoAndStop( 2 );
			}
			( main_mc[ "mcPage_"+curBtn ] as MovieClip ).gotoAndStop( 1 );
		}
		
		private function clickGridHandler( obj:Object ):void
		{
			var treaGridUnit:TreaGridUnit = obj.gridUnit;
			if ( TreasureData.selectGrid )
			{
				setTwoBtnVisible( true );
			}
			else
			{
				setTwoBtnVisible( false );
			}
		}
		
		//翻页
		private function goLeft( evt:MouseEvent ):void
		{
			if ( currentPage > 1 )
			{
				currentPage --;
				showCurrentPage();
				showItems();
				TreasureData.selectGrid = null;
				setTwoBtnVisible( false );
			}
		}
		
		private function goRight( evt:MouseEvent ):void
		{
			if ( currentPage < totalPage )
			{
				currentPage ++;
				showCurrentPage();
				showItems();
				TreasureData.selectGrid = null;
				setTwoBtnVisible( false );
			}
		}
		
		private function doubleClickGridHandler( obj:Object ):void
		{
			quChu( null );
		}
		
		private function diuQi( evt:MouseEvent ):void
		{
			if ( !TreasureData.selectGrid ) return;
			if ( !TreasureData.selectGrid.item ) return;
			if ( !startTimer() ) return;
			facade.sendNotification( EventList.SHOWALERT, {comfrim:comfrimDrop, cancel:cancelDrop, info:GameCommonData.wordDic[ "mod_ide_med_treasureP_dui" ]} );//"物品丢弃将消失，确定要丢弃么？"
		}
		
		private function comfrimDrop():void
		{
			TreasureNet.diuqiItems( TreasureData.selectGrid.item.Id );
		}
		
		private function cancelDrop():void {}
		
		private function quChu( evt:MouseEvent ):void
		{
			if ( !TreasureData.selectGrid ) return;
			if ( !TreasureData.selectGrid.item ) return;
			if ( !startTimer() ) return;
			if ( BagData.bagIsFull( TreasureData.selectGrid.item.Type ) ) return;
			TreasureNet.quchuItems( TreasureData.selectGrid.item.Id );
		}
		
		private function quChuAll( evt:MouseEvent ):void
		{
			/////////////////////////////////////////////////////////////测试全部丢弃
//			var aId:Array = [];
//			for ( var i:uint=0; i<TreasureData.packageDateArr.length; i++ )
//			{
//				aId.push( TreasureData.packageDateArr[i].id );
//			}
//			for ( i=0; i<aId.length; i++ )
//			{
//				TreasureNet.diuqiItems( aId[i] );
//			}
//			return;
			/////////////////////////////////////////////////////////////
			
			if ( TreasureData.packageDateArr.length == 0 ) return;
			if ( !startTimer() ) return;
			var arr:Array = [];
//			for ( var i:uint=0; i<TreasureData.packageDateArr.length; i++ )
//			{
//				arr.push( TreasureData.packageDateArr[i].type );
//			}

			for ( var i:uint=0; i<this.aCurData.length; i++ )
			{
				arr.push( aCurData[i].type );
			}
			
			if ( BagData.canPushGroupBag( arr ) ) return;
//			TreasureNet.quchuItems( 0 );
			TreasureNet.quchuItems( curBtn );				//新加
//			sendNotification( TreasureData.UPDATE_TREA_PACKAGE_SPACE )
		}
		
		//设置2个按钮是否可见
		private function setTwoBtnVisible( value:Boolean ):void
		{
			( main_mc.throw_btn as SimpleButton ).visible = value;
			( main_mc.remove_btn as SimpleButton ).visible = value;
		}
		
		private function setRemAllBtn( value:Boolean ):void
		{
			( main_mc.removeAll_btn as SimpleButton ).visible = value;
		}
		
		//更改包裹里的物品
		private function changPackageItem():void
		{
			analyData();
			showItems();		
			TreasureData.selectGrid = null;
			setTwoBtnVisible( false );
			if ( aCurData.length == 0 )
			{
				setRemAllBtn( false );
			}
			else
			{
				setRemAllBtn( true );
			}
		}
		
		//拆分数据
		private function analyData():void
		{
			aDrag = [];
			aEquip = [];
			aStone = [];
			aPet = [];
			aStuff = [];
			for ( var i:uint=0; i<TreasureData.packageDateArr.length; i++ )
			{
				switch ( TreasureData.packageDateArr[i].divide )
				{
					case 1:
						aDrag.push( TreasureData.packageDateArr[i] );
					break;
					case 2:
						aEquip.push( TreasureData.packageDateArr[i] );
					break;
					case 3:
						aStone.push( TreasureData.packageDateArr[i] );
					break;
					case 4:
						aPet.push( TreasureData.packageDateArr[i] );
					break;
					case 5:
						aStuff.push( TreasureData.packageDateArr[i] );
					break;
					
				}
			}
		}
		
		private function showItems():void
		{
			aCurData = [];
			switch ( curBtn )
			{
				case 0:
					aCurData = TreasureData.packageDateArr;
				break;
				case 1:
					aCurData = aDrag;
				break;
				case 2:
					aCurData = aEquip;
				break;
				case 3:
					aCurData = aStone;
				break;
				case 4:
					aCurData = aPet;
				break;
				case 5:
					aCurData = aStuff;
				break;
			}
			totalPage = Math.ceil( aCurData.length/50 );
			if ( totalPage < currentPage )
			{
				totalPage = currentPage;
			}
			var startIndex:uint = ( currentPage-1 )*50;
			var endIndex:uint;
			if ( aCurData.length>currentPage*50 )
			{
				endIndex = currentPage*50;
			}
			else
			{
				endIndex = aCurData.length;
			}
			gridManager.typeData = aCurData.slice( startIndex,endIndex );
			showCurrentPage();
		}
		
		//显示当前页
		private function showCurrentPage():void
		{
			( main_mc.page_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ]+currentPage+"/"+totalPage+GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第"	"页"
		}
		
		private function closePackage( evt:Event ):void
		{
			( main_mc.left_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goRight );
			
			( main_mc.throw_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,diuQi );
			( main_mc.remove_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,quChu );
			( main_mc.removeAll_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,quChuAll );
			
			for ( var i:uint=0; i<6; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).removeEventListener( MouseEvent.CLICK,chgBtn );
			}
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closePackage );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
			dataProxy.treasurePackageIsOpen = false;
			
		}
		
		/** 取出计时器 */
		private function startTimer():Boolean
		{
			if(getOutTimer.running) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_pro_ite_sta" ], color:0xffff00});//"请稍后"
				return false;
			} 
			else 
			{
				getOutTimer.reset();
				getOutTimer.start();
				return true;
			}
		}
		
	}
}