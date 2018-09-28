package GameUI.Modules.Equipment.ui
{
	import GameUI.ConstData.UIConstData;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	import flash.display.Sprite;

	public class NumberItem extends Sprite
	{
		protected var _type:String;
		protected var _dir:String;
		
		public function NumberItem(type:String,dir:String="NumberIcon")
		{
			super();
			if(uint(type) > 100000) {	//410101
				if(UIConstData.getItem(uint(type)).img != null) 
				{
					_type = String(UIConstData.getItem(uint(type)).img);
				}else{
					this._type=String(type);
				}
			}else{
				this._type=String(type);
			}
			this._dir=dir;
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+_dir+"/" + _type + ".png",onLoabdComplete);
		}
		
		protected function onLoabdComplete():void{
			var bitMap:Bitmap=ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+_dir+"/" + _type + ".png");
			this.addChild(bitMap);		
		}
		
	}
}