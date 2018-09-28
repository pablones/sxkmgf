package GameUI.Modules.Icon.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Icon.Data.IconData;
	import GameUI.Modules.MainSence.Data.MainSenceData;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.net.URLRequest;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class IconMediator extends Mediator
	{
		public static const NAME:String = "IconMediator";
		private const defaultPoint:Point = new Point(40, 12);
		private var moveTo_Point:Point;
		private var iconOne:SimpleButton;
		private var iconTwo:SimpleButton;
		private var iconThree:SimpleButton;
		private var iconFour:SimpleButton;
		private var tempIcon:SimpleButton;       //移动图标
		private var bitmap:Bitmap;
		private var bitmapData:BitmapData;
		private var loader:Loader;
		private var hasIcon:Array;
		private var showIcon:Array = new Array();
		private var countArr:Array = [40, 70];
		private var tempArr:Array = new Array();  //存储还未实现动画的图标数据
		private var tempx:Number;
		private var tempy:Number;
		private var timer:Timer;
		private var count:uint;
		private var _x:Number;
		private var _y:Number;
		private var obj:Object;
		private var _index:int;
		private var _message:Object;
		private var hasNewMessage:Boolean = false;
		private var isOld:Boolean;         //是否是新类型
		private var isLoad:Boolean = false;
		private var num:uint;              //存储tempArray需要左移的开始位置
		private var needMove:Boolean = false;     //是否需要左移
		private var isFirst:Boolean = true;
		private var filterCount:uint;
		private var interval:uint = 0;
		public static const REG:RegExp = /(<.*?>)/g;
		
		
		public function IconMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);			
		}
		
		override public function listNotificationInterests():Array
		{
			return [
					IconData.SHOW_ICON,
					EventList.STAGECHANGE,
					IconData.CLICK_BUTTON
					];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case IconData.SHOW_ICON:
				    var o:Object = notification.getBody() as Object;
				    if( hasNewMessage )
				    {
				    	tempArr.push( o );
				    	return;
				    }
					hasNewMessage = true;
				    _index = o.index - 1;
				    _message = o.message;
					if( !isLoad ) 
					{
						loadIcon();
						isLoad = true;
					}else{
						checkButton( _index+1 );
					}
					initTempArray();
					checkIsOld();
					
					break;
				case EventList.STAGECHANGE:
				    if( !hasNewMessage ) changePoint();
					break;
				case IconData.CLICK_BUTTON:
					var btnIndex:uint = notification.getBody() as uint;
					useButton( btnIndex );
					break;
			}
		}
		
		private function useButton( index:uint ):void
		{
			switch( index )
			{
				case 2:
					if( IconData.IconArray[1] && IconData.IconArray[1].length > 0 )
					{
						IconData.IconArray[1] = new Array();
        				changeIcon();
					}
					break;
			}
		}
		
		private function checkButton( index:int ):void
		{
			
			switch( index )
		 	{
		 		case 1:
		 			tempIcon = iconOne;
		 			break;
		 		case 2:
			 		tempIcon = iconTwo;
		 			break;
              	case 3:
              		tempIcon = iconThree;
              		break;
              	case 4:
              		tempIcon = iconFour;
              		break;
		 	}
		 	
		 	addMovingIcon();
		}
		
		private function loadIcon():void
		{
			loader = new Loader();
			var request:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/Icon.swf");
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onPicComplete);
			loader.load(request);
		}
		
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "Huodong" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "Huodong" ) as Class;
				iconOne = new BgClass() as SimpleButton;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "ZuDui" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "ZuDui" ) as Class;
				iconTwo = new BgClass() as SimpleButton;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "Friend" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "Friend" ) as Class;
				iconThree = new BgClass() as SimpleButton;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "Congratulate" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "Congratulate" ) as Class;
				iconFour = new BgClass() as SimpleButton;
			}
			e.target.removeEventListener(Event.COMPLETE, onPicComplete);
			
			checkButton( _index+1 );
		}
		
		// 判断是否为新的提示类型
		private function checkIsOld():void
		{
			if( IconData.hasIcon[_index] )
			{
				this.isOld = true;
				return;
			}
			this.isOld = false;
		}
		
		// 获得目标坐标
		private function getPoint():Object
		{
			if( !(IconData.pointArray) )  initPoint();
			
			var tempObject:Object;
			if( isOld )
			{
				for( var i:uint=0; i<IconData.tempArray.length; i++ )
				{
					tempObject = IconData.tempArray[i];
					if( tempObject.index == _index ) 
					{
						return IconData.pointArray[i];
					}
				}
			}else{
				if( IconData.tempArray && IconData.tempArray.length>0 )
				{
					for( var j:uint=0; j<IconData.tempArray.length; j++ )
					{
						tempObject = IconData.tempArray[j];
						if( tempObject.index > _index ) 
						{
							num = j;
							needMove = true;
							return IconData.pointArray[j];
						}
					}
				}else{
					return IconData.pointArray[0];
				}
			}
			return IconData.pointArray[IconData.tempArray.length];
		}
		
		private function addMovingIcon():void
		{
			bitmapData = new BitmapData(tempIcon.width, tempIcon.height);
			bitmapData.draw( tempIcon );
			bitmap = new Bitmap( bitmapData, "auto", true );
			bitmap.alpha = .6;
			
			if( GameCommonData.fullScreen == 1)
			{
				count = countArr[0];
			}else{
				count = countArr[1];
			}
			
			bitmap.x = defaultPoint.x;
			bitmap.y = defaultPoint.y;
			
			obj = this.getPoint();
			tempx = int((obj.x - defaultPoint.x)/count);
			tempy = (obj.y - defaultPoint.y)/count;
			if( !timer )
			{
				timer = new Timer(10);
				timer.addEventListener(TimerEvent.TIMER, Update);
				timer.start();
			}
			
			GameCommonData.GameInstance.GameUI.addChild(bitmap);
//			if( GameCommonData.GameInstance.GameUI.Elements.IndexOf( this ) == -1 )
//				GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		public function Update(event:TimerEvent):void
		{
//			if(this.timer!=null && this.timer.IsNextTime(gameTime))
//			{
				
				if( GameCommonData.GameInstance.GameUI.contains( bitmap ) && bitmap.x+100 > obj.x )
				{
					if( isFirst )
					{
						isFirst = false;
						bitmap.alpha = .8;
//						this.timer.DistanceTime = 10; 
						count = 30;
						tempx = int((obj.x - bitmap.x)/count);
						tempy = (obj.y - bitmap.y)/count;
					}
				}
				
				if( bitmap && GameCommonData.GameInstance.GameUI.contains( bitmap ) )
				{
					bitmap.x += tempx;
					bitmap.y += tempy;
					count--;
					
					if( count == 0 )
					{
						GameCommonData.GameInstance.GameUI.removeChild( bitmap );
						
						addToIconArray( _message );
						updateButton();
						isFirst = true;
					}
					
				}
				
				if( showIcon && showIcon.length > 0 )
				{
					interval += 1;
					if( interval == 3 )
					{
						for each( var icon:SimpleButton in showIcon )
						{
							if( icon ) icon.filters = [IconData.GLOWGILTER[this.filterCount]];
						}
						this.filterCount++;
						if( this.filterCount == 20) this.filterCount = 0;
						interval = 0;
					}
				}
//			}
		}
		
		private function updateButton():void
		{
			
			if( !isOld )
			{
				switch( _index+1 )
				{
					case 1:
						GameCommonData.GameInstance.GameUI.addChildAt( iconOne, 0 );
						iconOne.x = obj.x;
						iconOne.y = obj.y;
						showIcon.push( iconOne );
						break;
					case 2:
						GameCommonData.GameInstance.GameUI.addChildAt( iconTwo, 0 );
						iconTwo.x = obj.x;
						iconTwo.y = obj.y;
						showIcon.push( iconTwo );
						break;
					case 3:
						GameCommonData.GameInstance.GameUI.addChildAt( iconThree, 0 );
						iconThree.x = obj.x;
						iconThree.y = obj.y;
						showIcon.push( iconThree );
						break;
				}
				
				if( needMove )
				{
					for( var i:uint=num; i<IconData.tempArray.length; i++ )
					{
						switch( IconData.tempArray[i].index+1 )
						{
							case 2:
								if( GameCommonData.GameInstance.GameUI.contains( iconTwo ) ) iconTwo.x -= IconData.WIDTH + 2;
								break;
							case 3:
								if( GameCommonData.GameInstance.GameUI.contains( iconThree ) ) iconThree.x -= IconData.WIDTH + 2;
								break;
						}
					}
					needMove = false;
				}
			}
			this.filterCount = 0;
			changePoint();
			addListener();
			hasNewMessage = false;
			var len:uint = this.tempArr.length;
			if( len != 0 ) 
			{
				sendNotification( IconData.SHOW_ICON, this.tempArr.shift() );
//				if(len == this.tempArr.length) this.tempArr.splice(0, 1);
			}
		}
		
		private function addListener():void
		{
			if( iconOne && !iconOne.hasEventListener( MouseEvent.CLICK ) ) iconOne.addEventListener( MouseEvent.CLICK, clickOneFun );
			if( iconTwo && !iconTwo.hasEventListener( MouseEvent.CLICK ) ) iconTwo.addEventListener( MouseEvent.CLICK, clickTwoFun );
			if( iconThree && !iconThree.hasEventListener( MouseEvent.CLICK ) ) iconThree.addEventListener( MouseEvent.CLICK, clickThreeFun );
		}
		
		private function clickOneFun( event:MouseEvent ):void
		{
			var msg:String = IconData.IconArray[0].shift() as String;
//			trace( msg );
            changeIcon();
			sendNotification(EventList.SHOWALERT,{comfrim:new Function(),cancel:null,info:msg,params:null,extendsFn:new Function(),doExtends:0,canDrag:false});
		}
		
		private function clickTwoFun( event:MouseEvent ):void
		{
			IconData.IconArray[1] = new Array();
//			trace( msg );
            changeIcon();
			sendNotification( MainSenceData.USETEAMBUTTON, 7 );
		}
		
		private function clickThreeFun( event:MouseEvent ):void
		{
			sendNotification( FriendCommandList.INVATE_TO_FRIEND, IconData.IconArray[2].shift() );
			changeIcon();
		}
		
		private function changeIcon():void
		{
			for( var i:uint=0; i<IconData.IconArray.length; i++ )
			{
				if( !(IconData.IconArray[i]) || IconData.IconArray[i].length == 0 )
				{
					switch( i+1 )
					{
						case 1:
							if( this.iconOne && GameCommonData.GameInstance.GameUI.contains( this.iconOne ) ) 
							{
								GameCommonData.GameInstance.GameUI.removeChild( this.iconOne );
								this.iconOne.filters = [];
								remove( this.iconOne );
								changePoint();
							}
							break;
						case 2:
						    if( this.iconTwo && GameCommonData.GameInstance.GameUI.contains( this.iconTwo ) ) 
						    {
						    	GameCommonData.GameInstance.GameUI.removeChild( this.iconTwo );
						    	this.iconTwo.filters = [];
						    	remove( this.iconTwo );
						    	changePoint();
						    }
							break;
						case 3:
						    if( this.iconThree && GameCommonData.GameInstance.GameUI.contains( this.iconThree ) ) 
						    {
						    	GameCommonData.GameInstance.GameUI.removeChild( this.iconThree );
						    	this.iconThree.filters = [];
						    	remove( this.iconThree );
						    	changePoint();
						    }
							break;
					}
				}
			}
			if( (!iconOne || (iconOne && !GameCommonData.GameInstance.GameUI.contains( this.iconOne ))) && (!iconTwo || (iconTwo && !GameCommonData.GameInstance.GameUI.contains( this.iconTwo ))) && !hasNewMessage)
			{
				if( (!iconThree || (iconThree && !GameCommonData.GameInstance.GameUI.contains( this.iconThree ))) )
				{
					timer.stop();
					timer.removeEventListener(TimerEvent.TIMER, Update);
					timer = null;
				}
				
			}
			
		}
		
		private function remove( s:SimpleButton ):void
		{
			var inde:int = this.showIcon.indexOf( s );
			showIcon[inde] = null; 
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
        
		 
		private function initTempArray():void
		 {
		 	IconData.tempArray = [];
		 	for(var i:uint=0; i<IconData.IconArray.length; i++)
		 	{
		 		if( IconData.IconArray[i] && IconData.IconArray[i].length>0 )
		 		{
		 			IconData.tempArray.push( {index:i,arr:IconData.IconArray[i]} );
		 			IconData.hasIcon[i] = true;
		 		}else{
		 			IconData.hasIcon[i] = false;
		 		}
		 	}
		 }
		 
		 //初始化坐标点
		 private function initPoint():void
		 {
		 	if( GameCommonData.GameInstance.MainStage.stageWidth && GameCommonData.GameInstance.MainStage.stageHeight && GameCommonData.GameInstance.MainStage.stageWidth > 0 && GameCommonData.GameInstance.MainStage.stageHeight > 0 )
		 	{
//		 		trace( GameCommonData.GameInstance.MainStage.stageWidth, GameCommonData.GameInstance.MainStage.stageHeight );
			 	_x = (GameCommonData.GameInstance.MainStage.stageWidth - IconData.WIDTH)/2;
				_y = GameCommonData.GameInstance.MainStage.stageHeight/2 - 220;	
			 	if( GameCommonData.Player.Role.Sex == 1 ) _y = GameCommonData.GameInstance.MainStage.stageHeight/2 - 210;	
		 	}
		 	if( _x < 100 )
		 	{
		 		_x = (GameCommonData.GameInstance.ScreenWidth - IconData.WIDTH)/2;
				_y = GameCommonData.GameInstance.ScreenHeight/2 - 220;	
			 	if( GameCommonData.Player.Role.Sex == 1 ) _y = GameCommonData.GameInstance.ScreenHeight/2 - 210;
		 	}
			var x0:Number = _x;
			IconData.pointArray = [];
		 	for( var i:uint=0; i<IconData.LENGTH; i++ )
		 	{
//		 		trace( x0, _y );
		 		IconData.pointArray.push( {x:x0, y:_y} );
		 		x0 -= IconData.WIDTH + 2;
		 	}
		 }
		 
		 // 添加新消息到数组
		 private function addToIconArray( message:Object ):void
		 {
//		 	if( _index == 1 && (IconData.IconArray[_index] as Array).length > 0)
//		 	{
//		 		return;
//		 	}
		 	(IconData.IconArray[_index] as Array).push( message );
//		 	(IconData.IconArray[_index] as Array).push( cutStr(message) );
		 }
		 
		 private function cutStr(s:String):String 
		 {
			var arr:Array = s.split(REG);
			for(var i:int = 0; i < arr.length; i++) {
				if(arr[i].indexOf("<") >= 0) {
					arr[i] = arr[i].split("_")[1];
		 		}
		 	}
		    return arr.join("");
		 }
		 
		 private function changePoint():void
		 {
		 	initPoint();
		 	initTempArray();
		 	
		 	if( IconData.tempArray && IconData.tempArray.length>0 )
		 	{
			 	for( var i:uint=0; i<IconData.tempArray.length; i++ )
			 	{
			 		switch( IconData.tempArray[i].index+1 )
			 		{
			 			case 1:
			 			    if( GameCommonData.GameInstance.GameUI.contains( iconOne ) )
			 			    {
				 				iconOne.x = IconData.pointArray[i].x;
				 				iconOne.y = IconData.pointArray[i].y;  	
			 			    }
			 				break;
			 			case 2:
			 				if( GameCommonData.GameInstance.GameUI.contains( iconTwo ) )
			 				{
				 				iconTwo.x = IconData.pointArray[i].x;
				 				iconTwo.y = IconData.pointArray[i].y;
			 				}
			 				break;
		 				case 3:
			 				if( GameCommonData.GameInstance.GameUI.contains( iconThree ) )
			 				{
				 				iconThree.x = IconData.pointArray[i].x;
				 				iconThree.y = IconData.pointArray[i].y;
			 				}
			 				break;
			 		}
			 	}
		 	}
		 }
	}
}