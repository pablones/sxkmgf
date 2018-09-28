package GameUI.Modules.CastSpirit.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.AutoResize.Data.AutoSizeData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.CastSpirit.Net.CastSpiritNet;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CastSpiritTransferMediator extends Mediator
	{
		public static const NAME:String = "castSpiritTransferMed";
		
		private var loader:Loader;
		private var panelBase:PanelBase;
		private var leftBagItem:UseItem;         //背包里锁定的取灵装备
		private var rightBagItem:UseItem;        //背包里锁定的铸灵装备
		private var leftItem:UseItem;            //取灵装备
		private var rightItem:UseItem;           //铸灵装备
		private var container:Sprite;           //取灵装备容器类
		private var CSContainer:Sprite;       //铸灵装备容器类
		
		private var fastPurchase:FastPurchase;
		
		private var leftLockIndex:int;          //背包里取灵装备锁定的位置
		private var rightLockIndex:int;         //背包里铸灵装备锁定的位置
		
		private var pos:uint = 0;      //1为按下左边装备图标, 2为按下右边装备图标
		
		private var viewContainer:MovieClip;
		private var obj:Object;
		private var dataProxy:DataProxy;
		
		public function get castSpiritTransfer():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public function CastSpiritTransferMediator( viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					CastSpiritData.SHOW_CASTSPIRIT_TRANSFER_VIEW,
					CastSpiritData.CLOSE_CASTSPIRIT_VIEW,
					CastSpiritData.DROP_EQUIP_FROM_BAG,
					CastSpiritData.SUCCESS_CASTSPIRIT_TRANSFER,
					CastSpiritData.FAILD_CASTSPIRIT_TRANSFER,
					CastSpiritData.UPDATE_CASTSPIRIT_NUMBER
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case CastSpiritData.SHOW_CASTSPIRIT_TRANSFER_VIEW:
					if( CastSpiritData.CastSpiritTransferIsOpen == false )
					{
						if( this.castSpiritTransfer == null )
						{
							load();
							dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
						}
						else
						{
							showView();
						}
						if( dataProxy.BagIsOpen == false )
						{
							sendNotification(EventList.SHOWBAG);
						}
					}
					break;
				case CastSpiritData.CLOSE_CASTSPIRIT_VIEW:
					if( CastSpiritData.CastSpiritTransferIsOpen == true )
					{
						panelCloseHandler(null);
					}
					break;
				case CastSpiritData.DROP_EQUIP_FROM_BAG:
					if( CastSpiritData.CastSpiritTransferIsOpen == true )
					{
						AddUseItem(notification.getBody());
					}
					break;
				case CastSpiritData.SUCCESS_CASTSPIRIT_TRANSFER:
					if( CastSpiritData.CastSpiritTransferIsOpen == true )
					{
						UiNetAction.GetItemInfo(leftItem.Id, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
						UiNetAction.GetItemInfo(rightItem.Id, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
						castSpiritTransfer.txt_1.htmlText = "需要<font color='#00ff00'>1</font>个<font color='#00FFFF'>铸灵转移符</font>，当前拥有<font color='#00ff00'>"+ BagData.hasItemNum(610059) +"</font>个";
						onClick(null);
					}
					break;
				case CastSpiritData.FAILD_CASTSPIRIT_TRANSFER:
					
					break;
				case CastSpiritData.UPDATE_CASTSPIRIT_NUMBER:
					if( CastSpiritData.CastSpiritTransferIsOpen == true )
					{
						castSpiritTransfer.txt_1.htmlText = "需要<font color='#00ff00'>1</font>个<font color='#00FFFF'>铸灵转移符</font>，当前拥有<font color='#00ff00'>"+ int(notification.getBody()) +"</font>个";
					}
					break;
			}
		}
		
		private function load():void
		{
			loader = new Loader();
			var urlRequest:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/CastSpiritTransfer.swf");
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE, onComplete );
			loader.load( urlRequest );
		}
		
		private function onComplete( e:Event ):void
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "CastSpiritTransferView" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "CastSpiritTransferView" ) as Class;
				this.viewComponent = new BgClass() as MovieClip;
			}
//			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "btn_commit" ) )
//			{
//				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "btn_commit" ) as Class;
//				btn = new BgClass() as SimpleButton;
//			}
			loader = null;
			initView();
		}
		
		private function initView():void
		{
			fastPurchase = new FastPurchase("610059");
			fastPurchase.x = castSpiritTransfer.width + 1;
			fastPurchase.y = 2;
			viewContainer = new MovieClip();
			viewContainer.addChild(castSpiritTransfer);
			viewContainer.addChild(fastPurchase);
			
			panelBase = new PanelBase(this.viewContainer, this.viewContainer.width+80, this.viewContainer.height+12);
			panelBase.SetTitleTxt("铸灵转移");
			panelBase.name = "CastSpiritTransferView";
			panelBase.addChild(viewContainer)
			AutoSizeData.setStartPoint( panelBase, UIConstData.DefaultPos1.x, UIConstData.DefaultPos1.y, 3 );
			showView();
		}
		
		private function showView():void
		{
			CastSpiritData.CastSpiritTransferIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			castSpiritTransfer.btn_commit.addEventListener(MouseEvent.CLICK, onTransfer);
			castSpiritTransfer.btn_commit.visible = false;
			var num:uint = BagData.hasItemNum(610059);
			castSpiritTransfer.txt_1.htmlText = "需要<font color='#00ff00'>1</font>个<font color='#00FFFF'>铸灵转移符</font>，当前拥有<font color='#00ff00'>"+ num +"</font>个";
			castSpiritTransfer.txt_1.mouseEnabled = false;
		}
		
		private function AddUseItem(obj:Object):void
		{
			if( container == null )
			{
				container = new Sprite();
				leftLockIndex = obj.lockIndex as int;
				leftBagItem = obj.source as UseItem;
				leftItem = new UseItem(0, String(leftBagItem.Type), container);
				leftItem.Id = leftBagItem.Id;
				leftItem.Type = leftBagItem.Type;
				leftItem.x = 7;
				leftItem.y = 7;
				container.addChild(leftItem);
				container.name = "castSpiritItem_" + leftItem.Id;
				this.castSpiritTransfer.castSpiritTransfer_0.addChild(container);
				container.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				container.addEventListener(MouseEvent.CLICK, onClick );
				checkBtn();
			}
 			else if( CSContainer == null )
			{
				CSContainer = new Sprite();
				rightLockIndex = obj.lockIndex as int;
				rightBagItem = obj.source as UseItem;
				rightItem = new UseItem(0, String(rightBagItem.Type), container);
				rightItem.Id = rightBagItem.Id;
				rightItem.Type = rightBagItem.Type;
				rightItem.x = 7;
				rightItem.y = 7;
				CSContainer.addChild(rightItem);
				CSContainer.name = "castSpiritItem_" + rightItem.Id;
				this.castSpiritTransfer.castSpiritTransfer_1.addChild(CSContainer);
				CSContainer.addEventListener(MouseEvent.MOUSE_DOWN, mouseDownFun);
				CSContainer.addEventListener(MouseEvent.CLICK, clickFun );
				checkBtn();
			}
		}
		
		private function checkBtn():void
		{
			if( container && CSContainer )
			{
				castSpiritTransfer.btn_commit.visible = true;
			}
			else
			{
				castSpiritTransfer.btn_commit.visible = false;
			}
		}
		
		private function onTransfer( e:MouseEvent ):void
		{
			if( BagData.hasItemNum(610059) > 0 )
			{
				obj = IntroConst.ItemInfo[rightItem.Id];
				if( obj.castSpiritLevel > 0 )
				{
					if( ( obj.isBind == 1 || leftBagItem.IsBind == 1) && obj.isBind != leftBagItem.IsBind )
					{
						sendNotification(EventList.SHOWALERT,{comfrim:sendMessage,cancel:new Function(),info:"<font color='#E2CCA5'>目标装备已铸灵，转移后目标装备会绑定，是否转移？</fon>",extendsFn:null,doExtends:0,canDrag:false} );
					}
					else
					{
						sendNotification(EventList.SHOWALERT,{comfrim:sendMessage,cancel:new Function(),info:"<font color='#E2CCA5'>目标装备已经铸灵，请确定是否转移？</fon>",extendsFn:null,doExtends:0,canDrag:false} );
					}
				}
				else
				{
					if( ( obj.isBind == 1 || leftBagItem.IsBind == 1) && obj.isBind != leftBagItem.IsBind  )
					{
						sendNotification(EventList.SHOWALERT,{comfrim:sendMessage,cancel:new Function(),info:"<font color='#E2CCA5'>转移后目标装备会绑定，请确定是否转移？</fon>",extendsFn:null,doExtends:0,canDrag:false} );
					}
					else
					{
						sendMessage();
					}
				}
			}
			else
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("铸灵转移符数量不足", 0xffff00);
			}
		}
		
		private function sendMessage():void
		{
			CastSpiritNet.TransferCastSpiritSend( leftItem.Id, rightItem.Id );
		}
		
		private function mouseDownFun( e:MouseEvent ):void
		{
			rightItem.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			pos = 2;
			rightItem.onMouseDown();
		}
		
		private function onMouseDown( e:MouseEvent ):void
		{
			leftItem.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			pos = 1;
			leftItem.onMouseDown();
		}
		
		private function onClick( e:MouseEvent ):void
		{
			if( container.hasEventListener(MouseEvent.MOUSE_DOWN) == true )
			{
				container.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			}
			if( container.hasEventListener(MouseEvent.CLICK) == true )
			{
				container.removeEventListener(MouseEvent.CLICK, onClick );
			}
			if( this.castSpiritTransfer.castSpiritTransfer_0.contains(container) == true )
			{
				this.castSpiritTransfer.castSpiritTransfer_0.removeChild(container);
			}
			if( container.contains(leftItem) )
			{
				container.removeChild(leftItem);
			}
			if( this.castSpiritTransfer.castSpiritTransfer_0.contains(container) )
			{
				this.castSpiritTransfer.castSpiritTransfer_0.removeChild(container);
			}
			BagData.AllLocks[0][leftLockIndex] = false; 
			leftBagItem.IsLock = false;
			leftItem = null;
			container = null;
			checkBtn();
			leftBagItem = null;
			if( CSContainer )
			{
				clickFun(null);
			}
			else
			{
				sendNotification( EventList.UPDATEBAG );
			}
		}
		
		private function clickFun( e:MouseEvent ):void
		{
			if( CSContainer.hasEventListener(MouseEvent.MOUSE_DOWN) == true )
			{
				CSContainer.removeEventListener(MouseEvent.MOUSE_DOWN, mouseDownFun);
			}
			if( CSContainer.hasEventListener(MouseEvent.CLICK) == true )
			{
				CSContainer.removeEventListener(MouseEvent.CLICK, clickFun );
			}
			if( this.castSpiritTransfer.castSpiritTransfer_1.contains(CSContainer) == true )
			{
				this.castSpiritTransfer.castSpiritTransfer_1.removeChild(CSContainer);
			}
			if( CSContainer.contains(rightItem) )
			{
				CSContainer.removeChild(rightItem);
			}
			if( this.castSpiritTransfer.castSpiritTransfer_1.contains(CSContainer) )
			{
				this.castSpiritTransfer.castSpiritTransfer_1.removeChild(CSContainer);
			}
			BagData.AllLocks[0][rightLockIndex] = false; 
			rightBagItem.IsLock = false;
			rightItem = null;
			CSContainer = null;
			checkBtn();
			rightBagItem = null;
			sendNotification( EventList.UPDATEBAG );
		}
		
		private function dragDroppedHandler( e:DropEvent ):void
		{
			switch(e.Data.type)
			{
				case "bag":
					if( pos == 1 )
					{
						if( leftItem.hasEventListener(DropEvent.DRAG_DROPPED) )
						{
							leftItem.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
						}
						if( rightItem && rightItem.hasEventListener(DropEvent.DRAG_DROPPED) )
						{
							rightItem.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
						}
						onClick(null);
						if( rightItem )
						{
							clickFun(null);
						}
					}
					else if( pos == 2 )
					{
						if( rightItem.hasEventListener(DropEvent.DRAG_DROPPED) )
						{
							rightItem.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
						}
						clickFun(null);
					}
				break;
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			CastSpiritData.CastSpiritTransferIsOpen = false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			if( castSpiritTransfer.btn_commit.hasEventListener(MouseEvent.CLICK) == true )
			{
				castSpiritTransfer.btn_commit.removeEventListener(MouseEvent.CLICK, onTransfer);
			}
			if( container )
			{
				onClick(null);
			}
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
		}
		
		public function checkLevel( ob:Object ):Boolean
		{
			if( container == null )
			{
				if( ob.castSpiritLevel < 1 )
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("该装备还没铸灵", 0xffff00);
					return false;
				}
				return true;
			}
			if( CSContainer == null )
			{
				if( CastSpiritData.isSameEquip(leftItem.Type, ob.type) == true )
				{
					var obj:Object = IntroConst.ItemInfo[leftItem.Id];
					if( ob.castSpiritLevel < obj.castSpiritLevel )
					{
						return true;
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("目标装备铸灵等级需低于原始装备铸灵等级", 0xffff00);
					}
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("同类型装备才能转移", 0xffff00);
				}
			}
			return false;
		}
		
		public function checkPos( object:Object ):Boolean
		{
			if( container == null )
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("请先在左框放上武器", 0xffff00);
				return false;
			}
			if( CSContainer == null )
			{
				if( CastSpiritData.isSameEquip(leftItem.Type, object.type) == true )
				{
					var obj:Object = IntroConst.ItemInfo[leftItem.Id];
					if( object.castSpiritLevel < obj.castSpiritLevel )
					{
						return true;
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("目标装备铸灵等级需低于原始装备铸灵等级", 0xffff00);
						return false;
					}
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("同类型装备才能转移", 0xffff00);
					return false;
				}
			}
			return true;
		}
	}
}