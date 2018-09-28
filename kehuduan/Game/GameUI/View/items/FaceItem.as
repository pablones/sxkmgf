package GameUI.View.items
{
	import GameUI.ConstData.UIConstData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.geom.Point;

	public class FaceItem extends ItemBase
	{	
		public var icon:Bitmap;
		private var scale:Number;
		public var offsetPoint:Point = new Point(2,2);		
				
		public function FaceItem(iconName:String, parent:DisplayObjectContainer=null, mkDir:String = "icon", scale:Number = 1.0,offset:Point=null)
		{
			var iconName:String;			/** 储存图片名 */ 
			if(offset){
				this.offsetPoint=offset;
			}else{
				this.offsetPoint=new Point(2,2);
			}
			if(uint(iconName) > 100000 && uint(iconName)!=800001) {	//410101
				if(UIConstData.getItem(uint(iconName)).img != null) 
				{
					iconName = String(UIConstData.getItem(uint(iconName)).img);
				}
			}
			
			//
			var loadingIcon:MovieClip=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BuffGrid");
			loadingIcon.gotoAndStop(1);
			var bitMapData:BitmapData=new BitmapData(loadingIcon.width,loadingIcon.height);
			bitMapData.draw(loadingIcon);
			this.icon=new Bitmap(bitMapData);
			this.addChildAt(this.icon,0);
			//
			this.scale = scale;
			this.graphics.beginFill(1,0);
			this.graphics.drawRect(0,0,32,32);
			this.graphics.endFill();
			super(iconName, parent, mkDir);
		}
		
		protected override function onLoabdComplete():void
		{ 
//			try {
//				icon = this.blrP.GetResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + this.iconName + ".png").GetBitmap();
//			} catch(e:Error) {
//			}
			this.removeChild(this.icon);
 			icon = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + this.iconName + ".png");
 			if(icon==null){
	 			icon = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
 			}
 			 
		   /* if(this.blrP.GetResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + this.iconName + ".png")) {
 				icon = this.blrP.GetResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + this.iconName + ".png").GetBitmap();	//this.iconName
 			} else {
 				icon = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
 			}  */
			if(icon) {
				icon.scaleX = scale;
				icon.scaleY = scale;
				this.addChildAt(icon,0);
				icon.x = offsetPoint.x;
				icon.y = offsetPoint.y;
				this.dispatchEvent(new Event(Event.COMPLETE));
				super.onLoabdComplete();
			}
		}
		
		public function setEnable(bool:Boolean):void
		{
			this.mouseChildren = bool;
			this.mouseEnabled = bool;	
		}
		
	}
}