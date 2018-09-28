package GameUI.Modules.Manufactory.View
{
	import GameUI.Modules.Manufactory.Command.CheckCommitBtnCommand;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.Proxy.ManufatoryProxy;
	import GameUI.Modules.Manufactory.View.SkirtCell.ItemLevelSkirtCell;
	import GameUI.Modules.Manufactory.View.SkirtCell.ItemTypeSkirtCell;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class ManufatoryView extends Sprite
	{
		public var main_mc:MovieClip;
		private var typeSkirtArr:Array;
		private var levelSkirtArr:Array;
		private var manuProxy:ManufatoryProxy;
		
		//下拉框的背景高
		private var typeListHeight:Number;						//物品种类背景高
		private var levelListHeight:Number;
		//容器
		private var typeListContainer:Sprite = new Sprite();
		private var levelListContainer:Sprite = new Sprite();
		private var equipContainer:Sprite = new Sprite();									//装备列表容器
		private var baseItemContainer:Sprite = new Sprite();								//基础材料容器
		private var appendItemContainer:Sprite = new Sprite();							//附加材料容器
		
		private var aAppendCell:Array;
		private var aBaseCell:Array;
		private var aEquipCell:Array;
		private var readingBar:ReadingBar;
		
		//滚动条
		private var equipScroll:UIScrollPane;
		private var maskShape:Sprite = new Sprite();
		
		public function ManufatoryView(_main_mc:MovieClip):void
		{
			this.main_mc = _main_mc;
			manuProxy = UIFacade.GetInstance( UIFacade.FACADEKEY ).retrieveProxy( ManufatoryProxy.NAME ) as ManufatoryProxy;
			addContainer();								//添加容器
			inputNumHandler();							//处理输入数量
			refreshPower();									//活力文本
			initReadingBar();								//初始化进度条
		}
		
		//监听下拉列表事件
		public function listenSkirt():void
		{
			( main_mc.itemType_txt as TextField ).mouseEnabled = false;
			( main_mc.itemLevel_txt as TextField ).mouseEnabled = false;
			( main_mc.itemType_mc as MovieClip ).buttonMode = true;
			( main_mc.itemLevel_mc as MovieClip ).buttonMode = true;
			( main_mc.itemType_mc as MovieClip ).addEventListener( MouseEvent.MOUSE_DOWN,showTypeList );
			( main_mc.itemLevel_mc as MovieClip ).addEventListener( MouseEvent.MOUSE_DOWN,showLevelList );
			GameCommonData.GameInstance.stage.addEventListener(MouseEvent.MOUSE_DOWN,closeList);
		}
		
		//创建下拉列表
		public function creatSkirtList(aType:Array,aLevel:Array):void
		{
			clearContainer(typeListContainer);
			typeListHeight = 18 * aType.length;
			for ( var i:uint=0; i<aType.length; i++ )
			{
				var typeSkirtCell:ItemTypeSkirtCell = new ItemTypeSkirtCell(aType[i]);
				typeSkirtCell.x = 136;
				typeSkirtCell.y = 45+i*18;
				typeListContainer.addChild( typeSkirtCell );
			}
			
			clearContainer(levelListContainer);
			levelListHeight = 18 * aLevel.length;
			for ( var j:uint=0; j<aLevel.length; j++ )
			{
				var levelSkirtCell:ItemLevelSkirtCell = new ItemLevelSkirtCell(aLevel[j]);
//				levelSkirtCell.x = 216;
				levelSkirtCell.x = 215;
				levelSkirtCell.y = 45+j*18;
				levelListContainer.addChild( levelSkirtCell );
			}
		}
		
		//设置标头
		public function setItemTypeTitle(s:String):void
		{
			( main_mc.itemType_txt as TextField ).text = s;
		}
		
		public function setItemLevelTitle(s:String):void
		{
			( main_mc.itemLevel_txt as TextField ).text = s;
		}
		
		//物品装备布局
		public function showEquipItems(eArr:Array):void
		{
			clearContainer( equipContainer );
			var containerWidth:uint = 298;
			var containerHeight:uint = 288;
			aEquipCell = [];
			for ( var i:uint=0; i<eArr.length; i++ )
			{
				var equipCell:EquipCell = new EquipCell(eArr[i]);
				equipCell.x = 0;
				equipCell.y = i*20;
				equipContainer.addChild( equipCell );
				aEquipCell.push( equipCell );
			}
//			equipContainer.x = 10;
//			equipContainer.y = 50;
			equipContainer.graphics.clear();
			equipContainer.graphics.beginFill( 0xff0000,0 );
			equipContainer.graphics.drawRect( 0,0,containerWidth,containerHeight );
			equipContainer.graphics.endFill();
			
			equipScroll = new UIScrollPane( equipContainer );
//			equipScroll.x = 10;
			equipScroll.x = 11;
			equipScroll.y = 50;
			equipScroll.width = containerWidth;
			equipScroll.height = containerHeight;
			equipScroll.refresh();
//			if ( eArr.length>0 )
//			{
				main_mc.addChild( equipScroll );
//			}
		}
		
		//基础材料布局
		public function showBaseItem(bArr:Array):void
		{
			clearContainer( baseItemContainer );
			aBaseCell = [];
			for ( var i:uint=0; i<bArr.length; i++ )
			{
				var baseCell:BaseMaterialCell = new BaseMaterialCell( bArr[i] );
				baseCell.x = 0;
				baseCell.y = i*20;
				aBaseCell.push( baseCell );
				baseItemContainer.addChild( baseCell );
			}
		}
		
		//附加材料布局
		public function showAppendItem(aArr:Array):void
		{
			if ( appendItemContainer.numChildren>0 )
			{
				return;
			}
			aAppendCell = [];
			for ( var i:uint=0; i<aArr.length; i++ )
			{
				var appendCell:AppendMaterialCell = new AppendMaterialCell( aArr[i] );
				appendCell.x = 0;
				appendCell.y = i*20;
				appendItemContainer.addChild( appendCell );
				aAppendCell.push( appendCell );
			}
		}
		
		//附加材料全部初始化
		public function defaultAppendCells(appendCell:AppendMaterialCell):void
		{
			var jj:int = aAppendCell.indexOf( appendCell );
			if ( aAppendCell && aAppendCell.length>0 )
			{
				for ( var i:uint=0; i<aAppendCell.length; i++ )
				{
					if( i != jj )
					{
						( aAppendCell[i] as AppendMaterialCell ).isSelect = false;
					}
				}
			}
		}
		
		//显示概率变化
		public function changTextRate(aRate:Array):void
		{
			for ( var i:uint=0; i<aRate.length; i++ )
			{
//				( main_mc["rateTxt_"+i] as TextField ).text = aRate[i].toString() + "%";
				( main_mc["rateTxt_"+i] as TextField ).text = aRate[i].toString();
			}
		}
		
		//刷新活力值
		public function refreshPower():void
		{
			if ( ManufactoryData.selectScenoType == 0 )
			{
				( main_mc.power_txt as TextField ).text = "0"+ "/"+GameCommonData.Player.Role.Ene.toString();	
			}
			else
			{
				var needPower:uint = manuProxy.allInfoDic[ ManufactoryData.selectScenoType ].power;
				if ( !needPower )
				{
					return;
				}
				( main_mc.power_txt as TextField ).text =  needPower*ManufactoryData.ManufatoryCount+ "/"+GameCommonData.Player.Role.Ene.toString();	
			}
			if ( GameCommonData.Player.Role.Ene <= 0 )
			{
//				trace ( "活力值没了！！！！" );
			}
		}
		
		//控制制造按钮
		public function ctrlCommitBtn(isVisible:Boolean):void
		{
			( main_mc.commit_btn as SimpleButton ).visible = isVisible;
			isVisible ? ( main_mc.commit_txt as TextField ).textColor=0xffffff : ( main_mc.commit_txt as TextField ).textColor=0xff0000;
		}
		
		//开始读条
		public function startReadingBar():void
		{
			readingBar.go();
		}
		
		//停止读条
		public function stopReadingBar():void
		{
			readingBar.stop();
		}
		
		//实时更新
		public function keepStep():void
		{
			if ( aBaseCell && aBaseCell.length>0 )
			{
				for ( var i:uint=0; i<aBaseCell.length; i++ )
				{
					( aBaseCell[i] as BaseMaterialCell ).checkIsEnough();
				}
			}
			
			if ( aAppendCell && aAppendCell.length>0 )
			{
				for ( var j:uint=0; j<aAppendCell.length; j++ )
				{
					( aAppendCell[j] as AppendMaterialCell ).checkIsEnough();
				}
			}
			
			if ( aEquipCell && aEquipCell.length>0 )
			{
				for ( var k:uint=0; k<aEquipCell.length; k++ )
				{
					( aEquipCell[k] as EquipCell ).chgColor();
				}
			}
			
//			( main_mc.power_txt as TextField ).text = GameCommonData.Player.Role.Ene.toString();
			refreshPower();
		}
		
		//清理界面
		public function panelCleanUp():void
		{
			clearContainer (equipContainer);
			clearContainer (baseItemContainer);
			clearContainer (appendItemContainer);
			var startArr:Array = [0,0,0,0,0];
			changTextRate(startArr);
			ManufactoryData.limitLevel = null;
			ManufactoryData.limitType = null;
			aBaseCell = [];
			aAppendCell = [];
			ctrlCommitBtn(false);
			trace ( equipContainer.numChildren );
		}
		
		//选中了半成品
		public function selectUnfinshProduct():void
		{
			clearContainer(this.appendItemContainer);
			changTextRate( ["","","","",""] );
		}
		
		private function inputNumHandler():void
		{
			ManufactoryData.ManufatoryCount = 1;
			UIUtils.addFocusLis( main_mc.count_txt );
			( main_mc.count_txt as TextField ).text = ManufactoryData.ManufatoryCount.toString();
//			( main_mc.count_txt as TextField ).restrict = "1-9,20";
			( main_mc.count_txt as TextField ).maxChars = 3 ;
			( main_mc.count_txt as TextField ).addEventListener( Event.CHANGE,chgNumHandler );
		}
		
		private function chgNumHandler(evt:Event):void
		{
			var num:int = int( ( main_mc.count_txt as TextField ).text );
			
//			if ( num == 0 )
//			{
//				( main_mc.count_txt as TextField ).textColor = 0xff0000;
//			}
//			else
//			{
//				( main_mc.count_txt as TextField ).textColor = 0x000000;
//			}
			
			if ( num == 0 )
			{
				( main_mc.count_txt as TextField ).text = "1";
			}
			
			if ( num>20 )
			{
				( main_mc.count_txt as TextField ).text = "20";
			}
			
			ManufactoryData.ManufatoryCount = int( ( main_mc.count_txt as TextField ).text );
			
			if ( aBaseCell && aBaseCell.length>0 )
			{
				for ( var i:uint=0; i<aBaseCell.length; i++ )
				{
					( aBaseCell[i] as BaseMaterialCell ).checkIsEnough();
				}
			}
			
			if ( aAppendCell && aAppendCell.length>0 )
			{
				for ( var j:uint=0; j<aAppendCell.length; j++ )
				{
					( aAppendCell[j] as AppendMaterialCell ).checkIsEnough();
				}
			}
			
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( CheckCommitBtnCommand.NAME );
		}
		
		/**
		 * 封装下拉列表事件
		 * */
		private function showTypeList(evt:MouseEvent):void
		{
			evt.stopPropagation();
			if ( !main_mc.contains( typeListContainer ) )
			{
			    manuProxy.skirtListBack.height = typeListHeight+2;
				manuProxy.skirtListBack.x = 136;
				manuProxy.skirtListBack.y = 45;
				main_mc.addChild( manuProxy.skirtListBack );
				main_mc.addChild( typeListContainer );
			}
			else
			{
				main_mc.removeChild( manuProxy.skirtListBack );
				main_mc.removeChild( typeListContainer );
			}
			if ( main_mc.contains( levelListContainer ) )
			{
				main_mc.removeChild( levelListContainer );
			}
		}
		
		private function showLevelList(evt:MouseEvent):void
		{
			evt.stopPropagation();
			if ( !main_mc.contains( levelListContainer ) )
			{
			    manuProxy.skirtListBack.height = levelListHeight+2;
//				manuProxy.skirtListBack.x = 216;
				manuProxy.skirtListBack.x = 215;
				manuProxy.skirtListBack.y = 45;
				main_mc.addChild( manuProxy.skirtListBack );
				main_mc.addChild( levelListContainer );
			}
			else
			{
				main_mc.removeChild( manuProxy.skirtListBack );
				main_mc.removeChild( levelListContainer );
			}
			if ( main_mc.contains( typeListContainer ) )
			{
				main_mc.removeChild( typeListContainer );
			}
		}
		
		private function closeList(evt:MouseEvent):void
		{
			if ( main_mc.contains( typeListContainer ) )
			{
				main_mc.removeChild( manuProxy.skirtListBack );
				main_mc.removeChild( typeListContainer );
			}
			if ( main_mc.contains( levelListContainer ) )
			{
				main_mc.removeChild( manuProxy.skirtListBack );
				main_mc.removeChild( levelListContainer );
			}
		}
		
		private function addContainer():void
		{
			if ( !main_mc.contains( baseItemContainer ) )
			{
				baseItemContainer.x = 325;
				baseItemContainer.y = 53;
				main_mc.addChild( baseItemContainer );
			}
			
			if ( !main_mc.contains( appendItemContainer ) )
			{
				appendItemContainer.x = 317;
				appendItemContainer.y = 165;
				main_mc.addChild( appendItemContainer );
			}
			
			maskShape.x = 0;
			maskShape.y = 0;
			maskShape.graphics.beginFill( 0xff0000,0 );
			maskShape.graphics.drawRect( 0,0,5000,2000 );
			maskShape.graphics.endFill();
		}
		
		private function initReadingBar():void
		{
			var readingBarMC:MovieClip = new manuProxy.readingBar as MovieClip;
			readingBar = new ReadingBar( readingBarMC );
			readingBar.isManuing = setSkirtFalse;
			readingBar.manuClose = setSkirtTrue;
			readingBar.x = 373;
			readingBar.y = 356;
			main_mc.addChild( readingBar );
		}
		
		//清空容器
		private function clearContainer(listContainer:Sprite):void
		{
			while ( listContainer.numChildren>0 )
			{
				var disdroyItem:DisplayObject = listContainer.removeChildAt(0);
				disdroyItem = null;
			}
		}
		
		//禁用下拉菜单  正在打造中
		private function setSkirtFalse():void
		{
			( main_mc.itemType_mc as MovieClip ).mouseEnabled = false;
			( main_mc.itemLevel_mc as MovieClip ).mouseEnabled = false;
			this.appendItemContainer.mouseChildren = false;
			( main_mc.commit_btn as SimpleButton ).mouseEnabled = false;
			( main_mc.count_txt as TextField ).mouseEnabled = false;

//			if ( !GameCommonData.GameInstance.GameUI.contains(maskShape) )
//			{
//				GameCommonData.GameInstance.GameUI.addChildAt(maskShape,GameCommonData.GameInstance.GameUI.numChildren-1);
//			}
		}
		
		//终止打造
		private function setSkirtTrue():void
		{
			( main_mc.itemType_mc as MovieClip ).mouseEnabled = true;
			( main_mc.itemLevel_mc as MovieClip ).mouseEnabled = true;
			this.appendItemContainer.mouseChildren = true;
			( main_mc.commit_btn as SimpleButton ).mouseEnabled = true;
			( main_mc.count_txt as TextField ).mouseEnabled = true;

//			if ( GameCommonData.GameInstance.GameUI.contains(maskShape) )
//			{
//				GameCommonData.GameInstance.GameUI.removeChild(maskShape);
//			}
		}

	}
}