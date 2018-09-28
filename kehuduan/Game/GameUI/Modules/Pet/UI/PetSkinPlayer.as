package GameUI.Modules.Pet.UI
{
	import GameUI.ConstData.UIConstData;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.geom.Matrix;
	import flash.geom.Rectangle;
	import flash.net.URLRequest;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	
	/**
	 * 宠物模型逐帧播放器
	 * @author:Ginoo 
	 * @version:1.0
	 * @playerVersion:10.0
	 * @langVersion:3.0
	 * @date:7/21/2010
	 */
	public class PetSkinPlayer extends Sprite
	{
		private var loader:Loader = null;			/** 加载器 */
		private var timer:Timer;					/** 定时器 */
		private var bitMapDic:Dictionary = null;	/** 帧数据字典(BitMap) */
		private var totalFrames:int;				/** 总帧数 */
		private var curFrame:int  = 1;				/** 当前帧 */
		private var frameFrom:int = 1;				/** 播放开始幁 */
		private var frameTo:int   = 0;				/** 播放器停止幁 */
		private var spParent:MovieClip = null;
		
		/////////////////////////////////////////////////////////////////
		//constructor function
		
		public function PetSkinPlayer(spParent:MovieClip, skinType:int=11, frameFrom:int=1, frameTo:int=0)
		{
			this.spParent = spParent;
			this.frameFrom = frameFrom;
			this.frameTo   = frameTo;
			this.curFrame  = frameFrom;
			timer 	  = new Timer(200);
			loader	  = new Loader();
			bitMapDic = new Dictionary();
			timer.addEventListener(TimerEvent.TIMER, timerHandler);
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, loadCompleteHandler);
			loader.load(new URLRequest(UIConstData.PET_MODEL_DIC + skinType + ".swf"));
		}
		
		/////////////////////////////////////////////////////////////////
		//public function
		
		/** gc */
		public function gc():void
		{
			timer.stop();
			timer = null;
			removeAllChild();
			if(loader) {
				loader.unload();
				loader = null;
			}
			bitMapDic = null;
		}
		
		/////////////////////////////////////////////////////////////////
		//private function
		
		/** 加载完成 */
		private function loadCompleteHandler(e:Event):void
		{
			var mc:MovieClip = e.target.content as MovieClip;
			if(!mc) return;
			if(frameTo == 0) frameTo = mc.totalFrames;
			var spWidth:uint = 0;
			var spHeight:uint = 0; 
			for(var i:int = frameFrom; i < frameTo; i++) {
				mc.gotoAndStop(i);
		 		var rect:Rectangle = mc.getBounds(mc);
		 		var matrix:Matrix  = new Matrix();
		 		var x:Number = rect.x;
		 		var y:Number = rect.y;
		 		matrix.translate(-rect.x,-rect.y);
		 		var bitmapData:BitmapData = new BitmapData(rect.width, rect.height, true, 0);
		 		bitmapData.draw(mc, matrix);
		 		var bitMap:Bitmap = new Bitmap(bitmapData);
		 		bitMap.height -= 4;
		 		spHeight = rect.height;
		 		spWidth = rect.width;
		 		if(bitMapDic) {			//外部调用gc过快 会导致这里bitMapDic=null
		 			bitMapDic[i] = bitMap;
		 		} else {
		 			return;
		 		}
			}
			if(spParent) {
				spParent.addChild(this);
				this.x = (107 - spWidth) / 2;
				this.y = (109 - spHeight) / 2;
			}
	 		totalFrames = frameTo - frameFrom;
			timer.start();
		}
		
		/** 步频监听 */
		private function timerHandler(e:TimerEvent):void
		{
			if(curFrame == frameTo) curFrame = frameFrom;
			removeAllChild();
			if(bitMapDic) {
				this.addChild(bitMapDic[curFrame]);
				curFrame++;
			} else {
				gc();
			}
		}
		
		/** 移除所有子对象 */
		private function removeAllChild():void
		{
			var count:int = this.numChildren;
			while(count > 0) {
				this.removeChildAt(0);
				count--;
			}
		}
	}
}