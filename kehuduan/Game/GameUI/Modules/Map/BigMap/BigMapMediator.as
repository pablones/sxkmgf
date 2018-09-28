package GameUI.Modules.Map.BigMap
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class BigMapMediator extends Mediator
	{
		public static const NAME:String = "BigMapMediator";
		private var dataProxy:DataProxy;
		private var maxNumMap:uint=31;
		private var container:Sprite;
		
		public function BigMapMediator()
		{
			super(NAME);
		}
		
		private function get bigMap():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWBIGMAP,
				EventList.CLOSEBIGMAP
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					/* facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.BIGMAP});
					
				
					(this.viewComponent as DisplayObjectContainer).mouseEnabled=true;
					bigMap.btnCloseMap.addEventListener(MouseEvent.CLICK, closeMap);
					initSet(); */
				break;
				case EventList.SHOWBIGMAP:
					if(!this.bigMap)
					{
						ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/BigMap.swf",onLoadComplete);
						LoadingView.getInstance().showLoading();
					}
					else{
						this.showBigMap();
					}
				break;
				case EventList.CLOSEBIGMAP:
					this.closeMap(null);
				break;
			}
		}
		/**
		 * 地图加载完毕
		 **/ 
		private function onLoadComplete():void
		{
			LoadingView.getInstance().removeLoading();	
			var tempMc:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/BigMap.swf");
			var mapMc:MovieClip = new (tempMc.loaderInfo.applicationDomain.getDefinition("BigMap"))();
			this.setViewComponent(mapMc);
			this.bigMap.name = "BigMap";
			this.bigMap.mouseEnabled = true;
			this.bigMap.btnCloseMap.addEventListener(MouseEvent.CLICK, closeMap);
			this.initSet();
			this.showBigMap();
		}
		
		private function showBigMap():void{
			if(ChatData.txtIsFoucs) return;
			if(dataProxy.BigMapIsOpen)
			{
				closeMap(null);
			}
			else
			{
				dataProxy.BigMapIsOpen = true;
				bigMap.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - bigMap.width)/2;
				bigMap.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - bigMap.height)/2;
				GameCommonData.GameInstance.WorldMap.addChild(bigMap);

			}
			
			for(var i:uint=1;i<=this.maxNumMap;i++){
				var mc:MovieClip=this.bigMap["map_"+i] as MovieClip;
				var mask:uint=GameCommonData.BigMapMaskLow & (Math.pow(2,(i-1)));
				if(mask>0){
					mc.visible=true;
				}else{
					mc.visible=false;
				}	
			}
		}
		/**
		 *  初始化操作
		 * 
		 */		
		private function initSet():void{
			for(var i:uint=1;i<=this.maxNumMap;i++){
				this.setMc(this.bigMap["map_"+i]);	
			}
		}
		
		private function setMc(mc:MovieClip):void{
			
			mc.addEventListener(MouseEvent.CLICK,onMapClickHandler);
			mc.addEventListener(MouseEvent.ROLL_OVER,onRollOverHandler);
			mc.addEventListener(MouseEvent.ROLL_OUT,onRollOutHandler);
			mc.addEventListener(MouseEvent.MOUSE_MOVE,onMouseMoveHandler);
		}
		
		
		private function onMouseMoveHandler(e:MouseEvent):void{
		
		}
		
		private function onRollOverHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			var glFilter:GlowFilter = new GlowFilter(0xffff00, 0.5, 0.5, 0.5, 2, 1, true, false);
			mc.filters=[glFilter];
		}
		
		private function onRollOutHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			mc.filters=null;
		}
		
		private function onMapClickHandler(e:MouseEvent):void{
		
			var str:String=e.currentTarget.name;
			var arr:Array=str.split("_");
			var name:String=SmallConstData.getInstance().getSceneNameById(uint(arr[1]));
			if(name==null)return;
			
			if(name==String(GameCommonData.GameInstance.GameScene.GetGameScene.name)){
				sendNotification(EventList.SHOWSENCEMAP);
			}else{
				sendNotification(EventList.SHOWSENCEMAP,name);
			}
		}
		
		private function closeMap(event:MouseEvent = null):void
		{
			if(bigMap)
			{
				if(GameCommonData.GameInstance.contains(bigMap))
				{
					GameCommonData.GameInstance.WorldMap.removeChild(bigMap);
					dataProxy.BigMapIsOpen = false;
				}
			}
		}
	}
}