package GameUI.Modules.Login.UITool
{
	import GameUI.Modules.Login.Data.CreateRoleConstData;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class LoadManPng
	{
		public var sendShow:Function;				/** 完成后显示UI*/
		private var path:String;
		
		private var _mediator:Mediator;
		public function LoadManPng(path:String , mediator:Mediator)
		{
			this.path = path;
			_mediator = mediator;
		}
		public function load():void
		{
			if(CreateRoleConstData.rolePicList[path] != null)
			{
				if(sendShow != null) sendShow(CreateRoleConstData.rolePicList[path]);			//缓存
				return;
			}
			var picLoader:ImageItem = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + path, 
																	BulkLoader.TYPE_MOVIECLIP ,
																	path);
			picLoader.addEventListener(ProgressEvent.PROGRESS , onProgressHandler);
			picLoader.addEventListener(Event.COMPLETE, onPicComplete);
			picLoader.load();
			CreateRoleConstData.roleLoading = true;
		}
		/** 加载中 */
		private function onProgressHandler(e:Event):void
		{
			CreateRoleLoading.getInstance().showLoading();
		}
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			CreateRoleConstData.roleLoading = false;
			CreateRoleLoading.getInstance().removeLoading();
			var picLoader:Object  = e.target as Object;
			var view:MovieClip = picLoader.content.content as MovieClip;
			CreateRoleConstData.rolePicList[path] = view;
			picLoader.destroy();
			picLoader.removeEventListener(Event.COMPLETE, onPicComplete);
			if(sendShow != null) sendShow(view);
			view = null;
		}

	}
}