package Net.ActionProcessor
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.UICore.UIFacade;
	
	import Net.GameAction;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;

	/** 地图掉包  */
	public class MapItem extends GameAction
	{
		/* 消息类型  */
		public static const CREATE:uint 	 = 1;     //接收 创建
		public static const DELETE:uint 	 = 2; 	  //接收删除 已经无用
		public static const PICK:uint        = 3; 	  //发送捡取
		public static const PICKINRANGE:uint = 4; 	  //发送范围捡取
		
		public static const PackageTypeName:String = "Package";
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position       = 4;
			var obj:Object       = null;
			var action:uint      = bytes.readUnsignedInt();			// action 
			var id:uint  	     = bytes.readUnsignedInt();			// 物口编号
			var nPosX:uint       = bytes.readUnsignedShort();		// X
			var nPosY:uint       = bytes.readUnsignedShort();		// Y
			var nTime:uint       = bytes.readUnsignedInt();			// time
			var nItemAmount:uint = bytes.readUnsignedInt();			// 物品数量

			var article:Array = new Array();
			for(var i:int = 0; i < nItemAmount ; i ++)
			{
				var idType:uint = bytes.readUnsignedInt();			// 掉落物品type
				article.push(idType);
			}
				
			// 显示掉落的包
			var Package:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("McPackage");
			Package.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
			Package.addEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
			Package.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStageHandler);
			Package.name		  = PackageTypeName + id;
			Package.ID   		  = id;
			Package.Data		  = article;
			Package.TileX         = nPosX;
			Package.TileY         = nPosY;
			Package.isPicked    = false;
				
			// 延时显示物品包
			setTimeout(ShowPackage,1000, id, Package);
		}
		
		/** 显示掉包  */
		private function ShowPackage(id:uint, Package:MovieClip):void
		{
			if(GameCommonData.Scene.IsSceneLoaded == true)
			{
				var p:Point = MapTileModel.GetTilePointToStage(Package.TileX,Package.TileY);
				Package.x   = p.x;
				Package.y 	= p.y;
				GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.addChild(Package);
				GameCommonData.PackageList[id] = Package;
			}
			if(GameCommonData.Player.IsAutomatism && (AutoPlayData.aSaveTick[16]==1) )
			{
				UIFacade.UIFacadeInstance.autoPickBag();
			}
		}
		
		private function onMouseOver(e:MouseEvent):void
		{
			SysCursor.GetInstance().setMouseType(SysCursor.PICK_CURSOR);
		}
		
		private function onMouseOut(e:MouseEvent):void
		{
			SysCursor.GetInstance().revert();
		}
		
		protected function onRemoveFromStageHandler(e:Event):void
		{
			var mc:MovieClip = e.target as MovieClip;
			mc.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
			mc.removeEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
			mc.removeEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStageHandler);
		}
	}	
}