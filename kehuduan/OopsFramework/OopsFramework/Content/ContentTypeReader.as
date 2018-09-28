package OopsFramework.Content
{
	import OopsFramework.Exception.ExceptionResources;
	import OopsFramework.Game;
	import OopsFramework.IDisposable;
	
	import flash.display.*;
	import flash.media.Sound;
	import flash.text.Font;
	import flash.utils.ByteArray;

	public class ContentTypeReader implements IDisposable
	{
		public static const STATE_LOADING : int = 1;
		public static const STATE_LOADED  : int = 2;
		public static const STATE_USED    : int = 4;
		
		private var content	 	 : *;			 		 // 资源内容
		private var theLastTime  : int;					 // 最后一次使用时间
		private var useIntervals : int;					 // 离上次使用后的间隔时间
		private var state 		 : int = STATE_LOADING;	 // 加载状态
		
		/**网址（即唯一编号） */
		public var Name : String;
		public function ContentTypeReader()
		{
			this.theLastTime = Game.Instance.CurrentGameTime.TotalGameTime;
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			if(this.content is IDisposable)
			{
				IDisposable(this.content).Dispose();
			}
			this.content = null;
		}
		/** IDisposable End */
		
		public function get State():int
		{
			return this.state;
		}	
		
		public function set State(value:int):void
		{
			this.state = value;
		}	
		
		public function get Content():*
		{
			this.useIntervals = Game.Instance.CurrentGameTime.TotalGameTime - this.theLastTime;
			this.theLastTime  = Game.Instance.CurrentGameTime.TotalGameTime;
			this.state 		  = STATE_USED;
			
			return content;
		}
		public function set Content(value:*):void
		{
			this.state   = STATE_LOADED;
			this.content = value;
		}
		
		/** 离上次使用后的间隔时间 */
		public function get UseIntervals():int
		{
			return useIntervals;
		}
		
        /** 藜取字节数组数据  */
        public function GetByteArray():ByteArray
        {
        	return ByteArray(Content);
        }
        
        /** 获取音频对象 */
        public function GetSound():Sound
        {
        	return Sound(Content);
        }
        
		/** 获取XML对象 */
		public function GetXML():XML
		{
			return XML(Content);
		}
		
		/** 获取可显示的对象 */
		public function GetDisplayObject():DisplayObject
		{
			return DisplayObject(Content.content);
		}
		
		/** 获取 Sprite 对象 */
		public function GetSprite():Sprite
		{
			return Sprite(Content.content);
		}
		
		/** 获取影片剪辑对象 */
		public function GetMovieClip():MovieClip
		{
			return MovieClip(Content.content);
		}

		/** 获取位图对象 */
		public function GetBitmap():Bitmap
		{
			return Bitmap(Content.content);
		}
		
		/** 获取位图数据源对象 */
		public function GetClassByBitmapData(className:String):BitmapData
		{
			var tempClass:Class = GetClass(className) as Class;
			return new tempClass(0,0) as BitmapData
		}
		
		/** 在库中获取位图对象 */
		public function GetClassByBitmap(className:String):Bitmap
		{
			return new Bitmap(this.GetClassByBitmapData(className));
		}
		
		/** 在库中获取影片剪辑对象 */
		public function GetClassByMovieClip(className:String):MovieClip
		{
			var tempClass:Class = GetClass(className) as Class;
			return MovieClip(new tempClass());
		} 
		
		/** 在库中获取按钮对象 */
		public function GetClassByButton(className:String):SimpleButton
		{
			var tempClass:Class = GetClass(className) as Class;
			return SimpleButton(new tempClass());
		}
		
		/** 在库中获取字体对象 */
		public function GetClassByFont(className:String):Font
		{
			var tempClass:Class = GetClass(className) as Class;
			return Font(new tempClass());
		}
		
		/** 在库中获取FLASH自代组件对象 */
		public function GetClassByFlashComponent(className:String):Sprite
		{
			var tempClass:Class = GetClass(className) as Class;
			return Sprite(new tempClass());
		}
		
		/** 获取声音数据源对象 */
		public function GetClassBySound(className:String):Sound
		{
			var tempClass:Class = GetClass(className) as Class;
			return Sound(new tempClass());
		}
				
		/** 在库中获取一个对象 */
		public function GetClass(className : String) : Class
		{
			if (Content.applicationDomain.hasDefinition(className))
			{
				return Content.applicationDomain.getDefinition(className) as Class;
			}
			throw new Error("\"" + className + "\"" + ExceptionResources.ResourcesClassIsNull);
		}
	}
}