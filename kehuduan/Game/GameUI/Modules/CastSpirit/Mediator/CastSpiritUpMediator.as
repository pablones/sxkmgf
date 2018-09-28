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
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CastSpiritUpMediator extends Mediator
	{
		public static const NAME:String = "castSpiritUpMed";
		private var loader:Loader;
		private var panelBase:PanelBase;
		private var bagItem:UseItem;            //背包里锁定的武器
		private var useItem:UseItem;            //装备
		private var container:Sprite;           //装备容器类
		
		private var _lockIndex:int;            //背包里锁定的位置
		
		private var count:int;                 //拥有的魔灵数
		private var num:int                    //使用的魔灵数
		
		private var object:Object;
		private var btn:SimpleButton;
		
		private var dataProxy:DataProxy;
		
		public function CastSpiritUpMediator(viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get castSpiritUp():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					CastSpiritData.SHOW_CASTSPIRIT_UP_VIEW,
					CastSpiritData.CLOSE_CASTSPIRIT_VIEW,
					CastSpiritData.DROP_EQUIP_FROM_BAG,
					CastSpiritData.FAILD_CASTSPIRIT,
					CastSpiritData.SUCCESS_CASTSPIRIT
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case CastSpiritData.SHOW_CASTSPIRIT_UP_VIEW:
					if( CastSpiritData.CastSpiritUpIsOpen == false )
					{
						if( this.castSpiritUp == null )
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
					if( CastSpiritData.CastSpiritUpIsOpen )
					{
						AddUseItem(notification.getBody());
					}
					break;
				case CastSpiritData.CLOSE_CASTSPIRIT_VIEW:
					if( CastSpiritData.CastSpiritUpIsOpen )
					{
						panelCloseHandler(null);
					}
					break;
				case CastSpiritData.FAILD_CASTSPIRIT:
					if( CastSpiritData.CastSpiritUpIsOpen )
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("升级铸灵失败", 0xffff00);
					}
					break;
				case CastSpiritData.SUCCESS_CASTSPIRIT:
					if( CastSpiritData.CastSpiritUpIsOpen )
					{
						onChange(null);
 						UiNetAction.GetItemInfo(useItem.Id, GameCommonData.Player.Role.Id, GameCommonData.Player.Role.Name);
						updateText( notification.getBody() );
					}
					break;
			}
		}
		
		private function load():void
		{
			loader = new Loader();
			var urlRequest:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/CastSpiritUp.swf");
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE, onComplete );
			loader.load( urlRequest );
		}
		
		private function onComplete( e:Event ):void
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "CastSpiritUpView" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "CastSpiritUpView" ) as Class;
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
			panelBase = new PanelBase(this.castSpiritUp, this.castSpiritUp.width, this.castSpiritUp.height+12);
			panelBase.SetTitleTxt("铸灵升级");
			panelBase.name = "CastSpiritUpView";
			AutoSizeData.setStartPoint( panelBase, UIConstData.DefaultPos1.x, UIConstData.DefaultPos1.y, 3 );
			btn.x = 73.45;
			btn.y = 184.45;
			this.castSpiritUp.addChild(btn);
			showView();
		}
		
		private function showView():void
		{
			CastSpiritData.CastSpiritUpIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			btn.addEventListener(MouseEvent.CLICK, upCastSpirit);
			btn.visible = false;
			
			count = BagData.hasItemNum(610061);
			if( count > 999 )
			{
				this.castSpiritUp.txt_1.text = 999;
			}
			else
			{
				this.castSpiritUp.txt_1.text = count;
			}
			TextField(this.castSpiritUp.txt_1).addEventListener( Event.CHANGE, onChange );
			TextField(this.castSpiritUp.txt_1).addEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			TextField(this.castSpiritUp.txt_1).addEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
			this.castSpiritUp.txt_2.text = "";
			this.castSpiritUp.txt_3.text = "";
			this.castSpiritUp.txt_2.mouseEnabled = false;
			this.castSpiritUp.txt_3.mouseEnabled = false;
		}
		
		private function onChange( e:Event ):void
		{
			count = BagData.hasItemNum(610061);
			var num:uint = int(this.castSpiritUp.txt_1.text);
			if( num > 999 )
			{
				if( count >= 999 )
				{
					this.castSpiritUp.txt_1.text = 999;
				}
				else
				{
					this.castSpiritUp.txt_1.text = count;
				}
			}
			else
			{
				if( num > count )
				{
					this.castSpiritUp.txt_1.text = count;
				}
				else if( num <= 0 )
				{
					this.castSpiritUp.txt_1.text = 0;
				}
				else
				{
					this.castSpiritUp.txt_1.text = num;
				}
			}
		}
		
		private function checkBtn():void
		{
			if( container )
			{
				btn.visible = true;
			}
			else
			{
				btn.visible = false;
			}
		}
		
		private function AddUseItem(obj:Object):void
		{
			object = IntroConst.ItemInfo[obj.source.Id];
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
			this.castSpiritUp.castSpiritUp_0.addChild(container);
			
			this.castSpiritUp.txt_2.htmlText = "当前铸灵等级<font color='#00FF00'>" + object.castSpiritLevel + "</font>级";
			this.castSpiritUp.txt_3.htmlText = "升级进度：<font color='#00FF00'>"+ object.castSpiritCount +"</font>/<font color='#00ff00'>"+ IntroConst.castSpiritLevelCount[object.castSpiritLevel-1] + "</font>";
			container.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			container.addEventListener(MouseEvent.CLICK, onClick );
			checkBtn();
			
		}
		
		private function updateText( obj:Object ):void
		{
			this.castSpiritUp.txt_2.htmlText = "当前铸灵等级<font color='#00FF00'>" + obj.level + "</font>级";
			if( obj.level == 10 )
			{
				this.castSpiritUp.txt_3.text = "";
			}
			else
			{
				this.castSpiritUp.txt_3.htmlText = "升级进度：<font color='#00FF00'>"+ obj.amount +"</font>/<font color='#00ff00'>"+ IntroConst.castSpiritLevelCount[obj.level-1] + "</font>";
			}
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
			if( this.castSpiritUp.castSpiritUp_0.contains(container) == true )
			{
				this.castSpiritUp.castSpiritUp_0.removeChild(container);
			}
			if( container.contains(useItem) )
			{
				container.removeChild(useItem);
			}
			if( this.castSpiritUp.castSpiritUp_0.contains(container) )
			{
				this.castSpiritUp.castSpiritUp_0.removeChild(container);
			}
			BagData.AllLocks[0][_lockIndex] = false; 
			bagItem.IsLock = false;
			bagItem = null;
			useItem = null;
			container = null;
			sendNotification( EventList.UPDATEBAG );
			this.castSpiritUp.txt_2.text = "";
			this.castSpiritUp.txt_3.text = "";
			checkBtn();
		}
		
		
		private function upCastSpirit( e:MouseEvent ):void
		{
			var obj:Object = IntroConst.ItemInfo[useItem.Id];
			if( obj )
			{
				if( container == null )
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("你还没有放上已经铸灵的装备", 0xffff00);
					return;
				}
				count = BagData.hasItemNum(610061);
				num = int(castSpiritUp.txt_1.text);
				if( count >= num && count > 0 )
				{	
					if( num > 0 )
					{
						if( obj.castSpiritCount + num > IntroConst.castSpiritLevelCount[8] )
						{
	//						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("超出"+ (obj.castSpiritCount + num - IntroConst.castSpiritLevelCount[obj.castSpiritLevel-1])+"个魔灵", 0xffff00);
							sendNotification(EventList.SHOWALERT,{comfrim:sendMessage,cancel:new Function(),info:"<font color='#E2CCA5'>超出<font color='#00FF00'>"+ (obj.castSpiritCount + num - IntroConst.castSpiritLevelCount[obj.castSpiritLevel-1])+"</font>个魔灵，请确定是否提交？</font>",extendsFn:null,doExtends:0,canDrag:false} );
						}
						else
						{
							CastSpiritNet.UpCastSpiritSend( useItem.Id, num );
						}
					}
					else
					{
						UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("请输入注入的魔灵数", 0xffff00);
					}
				}
				else
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showPrompt("你没有足够的魔灵", 0xffff00);
				}
			}
		}
		
		private function sendMessage():void
		{
			CastSpiritNet.UpCastSpiritSend( useItem.Id, num );
		}
		
		private function onMouseDown( e:MouseEvent ):void
		{
			useItem.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			useItem.onMouseDown();
		}
		
		private function dragDroppedHandler( e:DropEvent ):void
		{
			useItem.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":
					onClick(null);
				break;
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			CastSpiritData.CastSpiritUpIsOpen = false;
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			if( btn.hasEventListener(MouseEvent.CLICK) == true )
			{
				btn.removeEventListener(MouseEvent.CLICK, upCastSpirit);
			}
			TextField(this.castSpiritUp.txt_1).removeEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			TextField(this.castSpiritUp.txt_1).removeEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
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