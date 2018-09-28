package GameUI.Modules.Pet.UI
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.Loader;
	import flash.events.Event;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	
	public class NewPetLoadComponent
	{
		private var loader:Loader;
		public var callBackFun:Function;
		public function NewPetLoadComponent()
		{
			loader = new Loader();
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoadComplete);
			loader.load(new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/newPet.swf"));
		}
		
		private function onLoadComplete(e:Event):void 
		{
			var domain:ApplicationDomain 	  = loader.contentLoaderInfo.applicationDomain;
			var _mainView:DisplayObject 	  = new (domain.getDefinition("newPetMiainView"))();
			var _moreInfoView:DisplayObject	  = new (domain.getDefinition("morePetInfo"))();
			var _fantasyView:DisplayObject	  = new (domain.getDefinition("fantasyGrain"))();
			var _changeSexView:DisplayObject  = new (domain.getDefinition("fantasyGrain"))();
			var _petPrivityView:DisplayObject = new (domain.getDefinition("petPrivityView"))();
			if(callBackFun != null)
			{
				callBackFun(_mainView,_moreInfoView,_fantasyView,_petPrivityView,_changeSexView);
			}
			this.dispose();
		}
		
		private function dispose():void
		{
			loader.contentLoaderInfo.removeEventListener(Event.COMPLETE,onLoadComplete);
			loader.unload();
			loader = null;
			callBackFun = null;
		}
	}
}
