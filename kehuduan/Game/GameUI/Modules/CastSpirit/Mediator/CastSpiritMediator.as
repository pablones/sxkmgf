package GameUI.Modules.CastSpirit.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.AutoResize.Data.AutoSizeData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.CastSpirit.Net.CastSpiritNet;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CastSpiritMediator extends Mediator
	{
		public static var NAME:String = "castSpiritMediator";
		private var loader:Loader;
		private var panelBase:PanelBase;
		private var bagItem:UseItem;            //背包里锁定的武器
		private var useItem:UseItem;            //装备
		private var reelItem:UseItem;           //卷轴
		private var container:Sprite;           //装备容器类
		private var reelContainer:Sprite;       //卷轴容器类
		
		private var addReelItem:UseItem;        //要购买的卷轴
		private var addReelContainer:Sprite;    //要购买的卷轴容器类
		
		private var _lockIndex:int;            //背包里锁定的位置
		
		private var selectView:CastSpiritSelect;
		private var btn:SimpleButton;
		
		private var _obj:Object;               //选择购买的物品信息
		
		private var dataProxy:DataProxy;
		
		public function CastSpiritMediator(viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get castSpirit():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					CastSpiritData.SHOW_CASTSPIRIT_VIEW,
					CastSpiritData.DROP_EQUIP_FROM_BAG,
					CastSpiritData.SUCCESS_CASTSPIRIT,
					CastSpiritData.FAILD_CASTSPIRIT,
					CastSpiritData.CLOSE_CASTSPIRIT_VIEW
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case CastSpiritData.SHOW_CASTSPIRIT_VIEW:
					if( CastSpiritData.CastSpiritIsOpen == false )
					{
						if( this.castSpirit == null )
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
				case CastSpiritData.DROP_EQUIP_FROM_BAG:
					if( CastSpiritData.CastSpiritIsOpen == true )
					{
						AddUseItem(notification.getBody());
					}
					break;
				case CastSpiritData.SUCCESS_CASTSPIRIT:
					if( CastSpiritData.CastSpiritIsOpen == true )
					{
						UiNetAction.GetItemInfo(useItem.Id, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
						if( reelContainer )
						{
							clickFun(null);
						}
					}
					break;
				case CastSpiritData.FAILD_CASTSPIRIT:
					if( CastSpiritData.CastSpiritIsOpen == true )
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("铸灵失败", 0xffff00);
					}
				 	break;
				case CastSpiritData.CLOSE_CASTSPIRIT_VIEW:
					if( CastSpiritData.CastSpiritIsOpen == true )
					{
						panelCloseHandler(null);
					}
					break;
			}
		}
		
		private function load():void
		{
			loader = new Loader();
			var urlRequest:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/CastSpirit.swf");
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE, onComplete );
			loader.load( urlRequest );
		}
		
		private function onComplete( e:Event ):void
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "CastSpiritView" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "CastSpiritView" ) as Class;
				this.viewComponent = new BgClass() as MovieClip;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "btn_commit" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "btn_commit" ) as Class;
				btn = new BgClass() as SimpleButton;
			}
			loader = null;
			initView();
		}
		
		private function initView():void
		{
			panelBase = new PanelBase(this.castSpirit, this.castSpirit.width-29, this.castSpirit.height+12);
			panelBase.SetTitleTxt("铸灵");
			panelBase.name = "CastSpiritView";
			AutoSizeData.setStartPoint( panelBase, UIConstData.DefaultPos1.x, UIConstData.DefaultPos1.y, 3 );
			this.selectView = new CastSpiritSelect( castSpirit );
			btn.x = 100.5;
			btn.y = 165;
			this.castSpirit.addChild(btn);
			showView();
		}
		
		private function showView():void
		{
			CastSpiritData.CastSpiritIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			this.castSpirit.btn_1.addEventListener(MouseEvent.CLICK, showSelectView);
			this.castSpirit.btn_buyItem.addEventListener(MouseEvent.CLICK, buyReel);
			this.castSpirit.txt_4.addEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			this.castSpirit.txt_4.addEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
			this.castSpirit.txt_1.mouseEnabled = false;
			btn.addEventListener(MouseEvent.CLICK, castSpiritFun);
			btn.visible = false;
		}
		
		private function showSelectView( e:MouseEvent ):void
		{
			selectView.showView();
			selectView.updateFun = updateView;
		}
		
		private function updateView( obj:Object ):void
		{
			_obj = obj;
			this.castSpirit.txt_1.text = obj.name;
			addReelItem = new UseItem(0, obj.type, this.castSpirit.reel);
			addReelItem.x = 7;
			addReelItem.y = 7;
			if( addReelContainer == null || this.castSpirit.reel.contains(addReelContainer) == false )
			{
				addReelContainer = new Sprite();
				addReelContainer.addChild(addReelItem);
				this.castSpirit.reel.addChild(addReelContainer);
			}
			addReelContainer.name = "TaskEqu_"+obj.type;
		}
		
		private function buyReel( e:MouseEvent ):void
		{
			if(this.addReelItem==null){
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyI_1" ], color:0xffff00});//"请选择你要购买的道具"
			}else{
				var num:uint=uint(this.castSpirit.txt_4.text);
				if(num == 0){
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onBuyI_2" ], color:0xffff00});//"请输入有效的购买数"
					return;
				}
				_obj = UIUtils.getGoodsAtMarket(_obj.type);
				var strInfo:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+_obj.PriceIn * num+'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+num+'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ff00">'+castSpirit.txt_1.text+'</font>';
				//花费		购买		个
				sendNotification(EventList.SHOWALERT, {comfrim:onSureToBuy, cancel:new Function(), info:strInfo,title:GameCommonData.wordDic[ "often_used_tip" ]});//"提 示"
			}
		}
		
		private function onSureToBuy():void{
			var num:uint=uint(this.castSpirit.txt_4.text);
			sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:this.addReelItem.Type,count:num});
		}
		
		private function AddUseItem(obj:Object):void
		{
			if( int(obj.index) === 0 )
			{
				if( container != null ) 
				{
					onClick(null);
				}
				container = new Sprite();
				_lockIndex = obj.lockIndex as int;
				bagItem = obj.source as UseItem;
				useItem = new UseItem(0, String(bagItem.Type), container);
				useItem.Id = bagItem.Id;	
				useItem.Type = bagItem.Type;
				useItem.x = 7;
				useItem.y = 7;
				container.addChild(useItem);
				container.name = "castSpiritItem_" + useItem.Id;
				this.castSpirit.castSpirit_0.addChild(container);
				container.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				container.addEventListener(MouseEvent.CLICK, onClick );
				checkBtn();
				UiNetAction.GetItemInfo(useItem.Id, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
			}
 			else if( int(obj.index) === 1 )
			{
				if( container != null )
				{
					var item:UseItem = obj.source as UseItem;
					var equipType:uint = getUseItemType();
					var reelType:uint = CastSpiritData.checkReel(item.Type);
					if( equipType != 0 && reelType != 0 && equipType == reelType )
					{
						if( reelContainer )
						{
							clickFun(null);
						}
						reelContainer = new Sprite();
						reelItem = new UseItem(0, String(item.Type), container);
						reelItem.Id = item.Id;
						reelItem.Type = item.Type;
						reelItem.x = 2;
						reelItem.y = 2;
						reelContainer.addChild(reelItem);
						reelContainer.name = "castSpiritItem_" + reelItem.Id;
						this.castSpirit.castSpirit_1.addChild(reelContainer);
						reelContainer.addEventListener(MouseEvent.MOUSE_DOWN, mouseDownFun);
						reelContainer.addEventListener(MouseEvent.CLICK, clickFun );
						checkBtn();
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("卷轴类型不对", 0xffff00);
					}
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("请先放上装备", 0xffff00);
				}
			}
		}
		
		private function castSpiritFun( e:MouseEvent ):void
		{
			var obj:Object = IntroConst.ItemInfo[useItem.Id];
			if( reelContainer != null )
			{
				if( obj.castSpiritLevel > 0 )
				{
					sendNotification(EventList.SHOWALERT,{comfrim:sendMessage,cancel:new Function(),info:"<font color='#E2CCA5'>该装备已经铸灵，请确定是否覆盖？</font>",extendsFn:null,doExtends:0,canDrag:false} );
				}
				else
				{
					sendMessage();
				}
			}
			else
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("请先放上卷轴", 0xffff00);
			}
		}
		
		private function sendMessage():void
		{
			CastSpiritNet.UpCastSpiritSend( useItem.Id, reelItem.Id );
		}
		
		public function getUseItemType():uint
		{
			var equipType:uint = CastSpiritData.checkEquip(useItem.Type);
			return equipType;
		}
		
		private function mouseDownFun( e:MouseEvent ):void
		{
			reelItem.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			reelItem.onMouseDown();
		}
		
		private function onMouseDown( e:MouseEvent ):void
		{
			useItem.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			useItem.onMouseDown();
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
			if( this.castSpirit.castSpirit_0.contains(container) == true )
			{
				this.castSpirit.castSpirit_0.removeChild(container);
			}
			if( container.contains(useItem) )
			{
				container.removeChild(useItem);
			}
			if( this.castSpirit.castSpirit_0.contains(container) )
			{
				this.castSpirit.castSpirit_0.removeChild(container);
			}
			BagData.AllLocks[0][_lockIndex] = false; 
			bagItem.IsLock = false;
			useItem = null;
			container = null;
			checkBtn();
			sendNotification( EventList.UPDATEBAG );
			bagItem = null;
			if( reelContainer )
			{
				clickFun(null);
			}
		}
		
		private function clickFun( e:MouseEvent ):void
		{
			if( reelContainer && reelContainer.hasEventListener(MouseEvent.MOUSE_DOWN) == true )
			{
				reelContainer.removeEventListener(MouseEvent.MOUSE_DOWN, mouseDownFun);
			}
			if( reelContainer && reelContainer.hasEventListener(MouseEvent.CLICK) == true )
			{
				reelContainer.removeEventListener(MouseEvent.CLICK, clickFun );
			}
			if( this.castSpirit.castSpirit_1.contains(reelContainer) == true )
			{
				this.castSpirit.castSpirit_1.removeChild(reelContainer);
			}
			if( reelContainer && reelContainer.contains(reelItem) )
			{
				reelContainer.removeChild(reelItem);
			}
			if( reelContainer && this.castSpirit.castSpirit_1.contains(reelContainer) )
			{
				this.castSpirit.castSpirit_1.removeChild(reelContainer);
			}
			reelItem = null;
			reelContainer = null;
			checkBtn();
		}
		
		private function checkBtn():void
		{
			if( container && reelContainer )
			{
				btn.visible = true;
			}
			else
			{
				btn.visible = false;
			}
		}
		
		private function dragDroppedHandler( e:DropEvent ):void
		{
			if( useItem && useItem.hasEventListener(DropEvent.DRAG_DROPPED) )
			{
				useItem.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			}
			if( reelItem && reelItem.hasEventListener(DropEvent.DRAG_DROPPED) )
			{
				reelItem.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			}
			switch(e.Data.type)
			{
				case "bag":
					if( e.Data.source == useItem )
					{
						onClick(null);
					}
					else if( e.Data.source == reelItem )
					{
						clickFun(null);
					}
				break;
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			CastSpiritData.CastSpiritIsOpen = false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			this.castSpirit.btn_1.removeEventListener(MouseEvent.CLICK, showSelectView);
			this.castSpirit.btn_buyItem.removeEventListener(MouseEvent.CLICK, buyReel);
			this.castSpirit.txt_4.removeEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			this.castSpirit.txt_4.removeEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
			if( btn.hasEventListener(MouseEvent.CLICK) == true )
			{
				btn.removeEventListener(MouseEvent.CLICK, castSpiritFun);
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
		
		private function onFoucsIn(event:FocusEvent):void
		{ 
			GameCommonData.isFocusIn = true; 
		}
		
		private function onFoucsOut(event:FocusEvent):void
		{
			GameCommonData.isFocusIn = false;
		}
	}
}