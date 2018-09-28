package GameUI.Modules.SmallWindow.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.view.MeridiansMediator;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Mediator.RoleMediator;
	import GameUI.Modules.SmallWindow.Data.SmallWindowData;
	import GameUI.Proxy.DataProxy;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.net.URLRequest;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SmallWindowMediator extends Mediator implements IUpdateable
	{
		public static const NAME:String="SmallWindowMediator";
		public static const DEFAULTPOS:Point=new Point(744,325);
		private var message:String;
		private var girlBmp:Bitmap;
		private var smallWindow:Sprite;
		private var mask:Shape;
		private var tf:TextField;
		private var format:TextFormat;
		private var loader:Loader;
		private var currentY:Number;
		private var currentHeight:int;
		private var type:String;
//		private var timer:Timer;
		private var isMove:Boolean = false;
		//关闭按钮
		private var closeBtn:SimpleButton;
		private var text:String;
		private var dataProxy:DataProxy;
		public static var SmallWindowIsOpen:Boolean = false;				/**  弹窗是否打开 */
		private var timer:Timer;
		private var fixedTime:FixedTime;
		
		public function SmallWindowMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.timer = new Timer();
			this.timer.DistanceTime = 10; 
		}
		
		public override function listNotificationInterests():Array
		{
			return [ 
			         SmallWindowData.INIT_SMALLWINDOW,
			         SmallWindowData.SHOW_WINDOW,
			         EventList.STAGECHANGE
			         ];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case SmallWindowData.INIT_SMALLWINDOW:
				    closeBtn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("CloseBtn");
//					resURL = GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/tankuang.swf";
					smallWindow = new Sprite();
					initUI();
				break;
				case SmallWindowData.SHOW_WINDOW:
				    type = notification.getBody() as String;
				    if( type )
				    {
				    	SmallWindowData.messages.push(type);
				    }
					if( SmallWindowIsOpen == false && SmallWindowData.messages.length > 0 )
					{
						if( !girlBmp )
						{
							loadGirl();
						}
						else
						{
							if( fixedTime )
							{
								fixedTime.reset();
							}
							addGirlImg();
						} 
					}
				case EventList.STAGECHANGE:
					if( !isMove ) changeUI();
				break;
			}
		}
		
		private function initUI():void
		{
			initButton();
			initMask();
			initText();
		}
		
		private function initButton():void
		{
			closeBtn.x = 229;
			closeBtn.y = 158;
		}
		
		private function initMask():void
		{
			mask = new Shape();
			mask.graphics.beginFill( 0xFF0000 );
            mask.graphics.drawRect(30, 177, 197, 55);
            mask.graphics.endFill();  
		}
		
		private function initText():void
		{
			tf = new TextField();
			tf.height = 55;
            tf.width = 197;
            tf.x = 30;
            tf.y = 177;
            tf.autoSize = TextFieldAutoSize.CENTER;
            tf.wordWrap = true;
            tf.selectable = false;
            tf.mouseWheelEnabled = false;
            
            if( !format )
            {
            	format = new TextFormat();
	            format.font = "Verdana";
	            format.color = 0xFFFFFF;
	            format.size = 14;
	            tf.defaultTextFormat = format; 
            }
            
            tf.mask = mask;
		}
		
		private function changeUI():void
		{
			if( GameCommonData.GameInstance.TooltipLayer.contains(smallWindow) )
			{
				if( smallWindow.stage.stageWidth >= GameConfigData.GameWidth )
				{
					smallWindow.x = smallWindow.stage.stageWidth - GameConfigData.GameWidth + DEFAULTPOS.x;
				}
				if( smallWindow.stage.stageHeight >= GameConfigData.GameHeight )
				{
					smallWindow.y = smallWindow.stage.stageHeight - GameConfigData.GameHeight + DEFAULTPOS.y;
				}
			}
		}
		
		private function addListener():void
		{
			smallWindow.addEventListener( MouseEvent.CLICK, click_FUN );
		}
		
		private function click_FUN( event:MouseEvent ):void
		{
//			tf.dispatchEvent( new TextEvent(TextEvent.LINK) );
			smallWindow.removeEventListener( MouseEvent.CLICK, click_FUN );
			GameCommonData.GameInstance.TooltipLayer.removeChild(smallWindow);
			removeChild();
			if( fixedTime == null )
			{
				fixedTime = new FixedTime(300000, dispose);
			}
			this.sendNotification( SmallWindowData.SHOW_WINDOW );
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			switch(text)
			{
				case "meridian":
					if(!dataProxy.meridiansLearnListIsOpen)
					{
						sendNotification( MeridiansEvent.SHOW_MERIDIANS_LEARN_LIST );
					}
//					facade.sendNotification(RoleEvents.SHOWPROPELEMENT, 1);
//					var a:int = RolePropDatas.CurView;
//					trace(RolePropDatas.CurView);
//					
//					RolePropDatas.CurView = 1;
//					facade.sendNotification(EventList.SHOWHEROPROP);
					
					if(!dataProxy.HeroPropIsOpen)
					{
						dataProxy.HeroPropIsOpen = true;
						facade.sendNotification(EventList.SHOWHEROPROP);
						sendNotification(EventList.OPEN_PANEL_TOGETHER);
					}
					if(RolePropDatas.CurView != MeridiansMediator.TYPE)
					{
						(facade.retrieveMediator(RoleMediator.NAME) as RoleMediator).heroProp["prop_1"].dispatchEvent(new MouseEvent(MouseEvent.CLICK));
					}
					
					break;
			}
		}
		
		private function removeChild():void
		{
			while( smallWindow.numChildren > 0 )
			{
				smallWindow.removeChildAt(0);
			}
			SmallWindowIsOpen = false;
		}
		
		private function dispose():void
		{
			girlBmp = null;
			fixedTime = null;
		}
		
		private function setText():void
		{
			if(SmallWindowData.messages.length == 0) 
			{
//				if ( facade.hasMediator( NAME ) )
//				{
//					facade.removeMediator( NAME );
//				}
				return;
			}
            tf.htmlText = SmallWindowData.messages.shift() as String;
            text = tf.htmlText.split("_")[1];
//          tf.htmlText = (SmallWindowData.messages.shift() as String) + '<font color="#00ff00"><a href="event:meridian"><u>点击这里</u></a></font>';
//          tf.setTextFormat( this.format );
//			tf.addEventListener( TextEvent.LINK,clickText );
            addWindow();
		}
		
//		private function clickText( evt:TextEvent ):void
//		{
//			var strs:Array = (evt.text as String).split("_");
//			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
//			tf.removeEventListener( TextEvent.LINK,clickText );
//			SmallWindowIsOpen = false;
//			switch(strs[1])
//			{
//				case "meridian":
//					sendNotification( MeridiansEvent.SHOW_MERIDIANS_LEARN_LIST );
////					facade.sendNotification(RoleEvents.SHOWPROPELEMENT, 1);
////					var a:int = RolePropDatas.CurView;
////					trace(RolePropDatas.CurView);
////					
////					RolePropDatas.CurView = 1;
////					facade.sendNotification(EventList.SHOWHEROPROP);
//					
//					if(!dataProxy.HeroPropIsOpen)
//					{
//						dataProxy.HeroPropIsOpen = true;
//						facade.sendNotification(EventList.SHOWHEROPROP);
//						sendNotification(EventList.OPEN_PANEL_TOGETHER);
//					}
//					if(RolePropDatas.CurView != MeridiansMediator.TYPE)
//					{
//						(facade.retrieveMediator(RoleMediator.NAME) as RoleMediator).heroProp["prop_1"].dispatchEvent(new MouseEvent(MouseEvent.CLICK));
//					}
//					
//					break;
//			}
//			evt.stopPropagation();
//		}
		
		private function addWindow():void
		{
			if ( !smallWindow.contains(mask) ) smallWindow.addChild(mask);
			if ( !smallWindow.contains(closeBtn) ) smallWindow.addChild(closeBtn);
			if ( !smallWindow.contains(tf) ) smallWindow.addChild(tf);
			isMove = true;
            GameCommonData.GameInstance.TooltipLayer.addChildAt(smallWindow,0);
            smallWindow.x = smallWindow.stage.stageWidth - GameConfigData.GameWidth + DEFAULTPOS.x;
            smallWindow.y = smallWindow.stage.stageHeight - smallWindow.height + 25;
            currentHeight = smallWindow.stage.stageHeight;
            smallWindow.alpha = 0;
            SmallWindowIsOpen = true;
            GameCommonData.GameInstance.GameUI.Elements.Add(this);	
//            timer = new Timer( 10 );
//            timer.addEventListener(TimerEvent.TIMER, moveUp);
//            timer.start();
            
		}
		
//		private function moveUp( event:TimerEvent ):void
//		{
//			smallWindow.y -= 3;
//			if( smallWindow.y <= smallWindow.stage.stageHeight - GameConfigData.GameHeight + DEFAULTPOS.y )
//			{
//				smallWindow.y = smallWindow.stage.stageHeight - GameConfigData.GameHeight + DEFAULTPOS.y;
//				timer.stop();
//				timer.removeEventListener(TimerEvent.TIMER, moveUp);
//				GameCommonData.GameInstance.GameUI.Elements.Remove(this);	
//				timer = null;
//				addListener();
//				isMove = false;
//				changeUI();
//			}
//		}
		
		private function loadGirl():void
		{
			loader = new Loader();
			var request:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/tankuang.swf");
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onPicComplete);
			loader.load(request);
		}
		
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "BeautifulGirl" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "BeautifulGirl" ) as Class;
				var bmpData:BitmapData = new BgClass( 256,255 ) as BitmapData;
				girlBmp = new Bitmap( bmpData );
//				girlBmp.bitmapData = bmpData;
				addGirlImg();
				BgClass = null;
				
			}
			e.target.removeEventListener(Event.COMPLETE, onPicComplete);
		}
		
		private function addGirlImg():void
		{
			smallWindow.addChild( girlBmp );
			setText();
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(this.timer!=null && this.timer.IsNextTime(gameTime))
			{
				smallWindow.y -= 4;
				smallWindow.alpha += .1;
				if( smallWindow.y <= currentHeight - GameConfigData.GameHeight + DEFAULTPOS.y - 4)
				{
					smallWindow.y = smallWindow.stage.stageHeight - GameConfigData.GameHeight + DEFAULTPOS.y;	
					smallWindow.alpha = 1;
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);			
					addListener();
					isMove = false;
					changeUI();
				}
			}
		}
		public function get Enabled():Boolean							// 是否启动更新
		{
			return true;
		}
		public function get UpdateOrder():int							// 更新优先级（数值小的优先更新）
		{
			return 0;
		}
		
		public function get EnabledChanged():Function
		{
			return null;
		}
		public function set EnabledChanged(value:Function):void
		{
			
		}	
        public function get UpdateOrderChanged():Function
		{
			return null;
		}
        public function set UpdateOrderChanged(value:Function):void
        {
        	
        }
	}
}