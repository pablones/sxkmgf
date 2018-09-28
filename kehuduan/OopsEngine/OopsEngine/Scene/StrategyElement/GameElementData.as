package OopsEngine.Scene.StrategyElement
{
	import OopsEngine.Graphics.Animation.*;
	import OopsEngine.Scene.CommonData;
	
	import OopsFramework.Collections.DictionaryCollection;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	import OopsFramework.IDisposable;
	
	import flash.display.*;
	import flash.geom.*;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	public class GameElementData implements IDisposable
	{
		protected var action:DictionaryCollection = new DictionaryCollection();
		
		private var clips:DictionaryCollection  = new DictionaryCollection();		// 动画剪辑集合
//		private var frames:DictionaryCollection = new DictionaryCollection();		// 动画帧集合
		private var maxWidth:int  = 0;												// 动画最大宽
		private var maxHeight:int = 0;												// 动画最大高
		public var ClipHeight:int = 0;
		
		public var DataUrl:String = "";                                             // 动画剪辑名称 对应的资源路径路径
		public var DataName:String = "";                                            // 动画剪辑名称 对应的资源路径名称
		public var AnalyzeComplete :Function;                                       // 动画解析完成
		public var LoaderResource       : BulkLoaderResourceProvider;               // 下载资源
		public var LoadComplete         : Function;                                 // 下载完成
		public var IsLoad               : Boolean = false;                          // 是否下载完成
		private var AnalyzeIndex        : int = 1;                                  // 
		private var timeoutId : int = -1;
		
		public function onLoadComplete():void
		{
			if(LoadComplete != null)
			{			
				LoadComplete(this);
				IsLoad     = true;
				//加载完成没有可以需要下载的
				CommonData.IsLoad = false;	
			}
		}
		
		public function Dispose():void
		{
//			frames.Dispose();
//			frames = null;
//			
			this.clips.Dispose();
			this.clips = null;
			
			this.action.Dispose();
			this.action = null;
			
			if(LoaderResource!=null)
			{
				LoaderResource.Dispose();
				LoaderResource = null;
			}
			
			if(timeoutId!=-1)
			{
				clearTimeout(timeoutId);
			}
			
			AnalyzeComplete = null;
		}
		
    	/** 创建动画方向  */
    	private function CreateDirection(i:int):int
    	{
    		switch(i)
    		{
    			case 1:
    				return 8;
    			case 2:
    				return 2;
    			case 3:
    				return 4;
    			case 4:
    				return 7;
    			case 5:
    				return 1;
    			case 6:
    				return 6;
    			case 7:
    				return 9;
    			case 8:
    				return 3;
    		}
    		return 5;
    	}
    	
    	/** 生成影片剪辑  */
    	private function CreateClip(action:String, frameIndex:String,actionIndex:int):void
    	{
    		// 创建影片剪辑
    		var clip:AnimationClip = new AnimationClip();
    		clip.Name    		   = action + this.CreateDirection(actionIndex);
    		
		 	if(actionIndex==6 || actionIndex==7 || actionIndex==8)
		 	{
		 		clip.TurnType = true;
		 	}
		 	else
		 	{
		 		clip.TurnType = false;
		 	}
		 		
		 	// 创建帧
    		var index:Array = frameIndex.split("-");
    		if(index.length > 1)
    		{
    			for(var num:int=int(index[0]);num<=int(index[1]);num++)
    			{
    				clip.Frame.push(num);
    			    this.clips[clip.Name] = clip;
    			}
    		}
    		else
    		{
    			clip.Frame.push(index[0]);
    		    this.clips[clip.Name] = clip;
    		}
    	}
    	
    	/** 生成一个动做的所有影片剪辑 */
    	protected function CreateActionClips(action:String, data:Array):void
    	{
    		var i:int = 1;
    		for each(var frameIndex:String in data)
    		{
//    			if(action == GameElementSkins.ACTION_STATIC)
//    			{
//    				this.ClipHeight  = this.frames[frameIndex].Height;
//    			}
    			
		 		this.CreateClip(action,frameIndex,i);
		 		if(i==3 || i==4 || i==5)
		 		{
		 			this.CreateClip(action,frameIndex,i + 3);
		 		}
    			i++;
    		}
    	}
      	
//      	/** 解析动画格式 */
//        public virtual function Analyze(data:*):void
//        {
//        	var mc:MovieClip = data as MovieClip;
//			var rect:Rectangle;
//			var frame:AnimationFrame;
//		 	for (var i:int = 1 ; i <= mc.totalFrames ; i++)
//		 	{
//		 		mc.gotoAndStop(i);
//				rect 				  = mc.getBounds(mc);
//				frame  				  = new AnimationFrame();
//		 		frame.X				  = rect.x;
//		 		frame.Y				  = rect.y;
//		 		frame.Width			  = rect.width;
//		 		frame.Height		  = rect.height;
//				frame.FrameBitmapData = new BitmapData(rect.width, rect.height,true, 0);
//				frame.FrameBitmapData.draw(mc, new Matrix(1, 0, 0, 1, -rect.x, -rect.y));
//				
//		 		if(frame.Width  > this.maxWidth)  this.maxWidth  = frame.Width;
//		 		if(frame.Height > this.maxHeight) this.maxHeight = frame.Height;
//				this.frames.Add(i.toString(),frame);
//		 	}		 	
//		 	
//		 	if(LoaderResource != null)
//		 	{
//		 		LoaderResource.Dispose();
//		 		LoaderResource = null;
//		 	} 		 	
//		 	
//	 		//动画解析完成
//		 	if(AnalyzeComplete != null)
//		 	{
//		 		AnalyzeComplete(this);
//		 	}
//        }

      	/** 解析动画格式 */
        public virtual function MountAnalyze(data:*):void
        {
        	var mc:MovieClip = data as MovieClip;
			var rect:Rectangle;
			var frame:AnimationFrame;
			var frames:DictionaryCollection = new DictionaryCollection();
							
		 	for (var i:int = 1 ; i <= mc.totalFrames ; i++)
		 	{
		 		mc.gotoAndStop(i);
				rect 				  = mc.getBounds(mc);
				frame  				  = new AnimationFrame();
		 		frame.X				  = rect.x;
		 		frame.Y				  = rect.y;
		 		frame.Width			  = rect.width;
		 		frame.Height		  = rect.height;
				frame.FrameBitmapData = new BitmapData(rect.width, rect.height,true, 0);
				frame.FrameBitmapData.draw(mc, new Matrix(1, 0, 0, 1, -rect.x, -rect.y));
				
		 		if(frame.Width  > this.maxWidth)  this.maxWidth  = frame.Width;
		 		if(frame.Height > this.maxHeight) this.maxHeight = frame.Height;
		 		
				frames.Add(i.toString(),frame);
		 	}	
		 	
		 	CommonData.AnimationFrameList[this.DataName] = frames;
		 	
		 	if(LoaderResource != null)
		 	{
		 		LoaderResource.Dispose();
		 		LoaderResource = null;
		 	} 		 	
		 	
	 		//动画解析完成
		 	if(AnalyzeComplete != null)
		 	{
		 		AnalyzeComplete(this);
		 	}
        }

        
        public virtual function Analyze(data:*,frames:DictionaryCollection = null):void
        {
        	var mc:MovieClip = data as MovieClip;
			var rect:Rectangle;
			var frame:AnimationFrame;
			var n:int = AnalyzeIndex + 15;
			if(frames == null)
			{
				frames = new DictionaryCollection();
			}

		 	for (AnalyzeIndex ; AnalyzeIndex <= n; AnalyzeIndex++)
		 	{		 		
		 		if(AnalyzeIndex <= mc.totalFrames)
		 		{
			 		mc.gotoAndStop(AnalyzeIndex);
					rect 				  = mc.getBounds(mc);
					frame  				  = new AnimationFrame();
			 		frame.X				  = rect.x;
			 		frame.Y				  = rect.y;
			 		frame.Width			  = rect.width;
			 		frame.Height		  = rect.height;
					frame.FrameBitmapData = new BitmapData(rect.width, rect.height,true, 0);
					frame.FrameBitmapData.draw(mc, new Matrix(1, 0, 0, 1, -rect.x, -rect.y));					
			 		if(frame.Width  > this.maxWidth)  this.maxWidth  = frame.Width;
			 		if(frame.Height > this.maxHeight) this.maxHeight = frame.Height;
					frames.Add(AnalyzeIndex.toString(),frame);
		 		}
		 	}	
		 	
		 	if(AnalyzeIndex > mc.totalFrames)
		 	{
		 	    CommonData.AnimationFrameList[this.DataName] = frames;
		 		
		 		if(LoaderResource != null)
			 	{
			 		LoaderResource.Dispose();
			 		LoaderResource = null;
			 	} 		 	
			 	
				//动画解析完成
			 	if(AnalyzeComplete != null)
			 	{
			 		clearTimeout(timeoutId);
			 		AnalyzeComplete(this);
			 	}
		 	}
		 	else
		 	{
		 		timeoutId = setTimeout(this.cAnalyze,5,mc,frames);
		 	}
        }
        
        
        public function cAnalyze(data:*,frames:DictionaryCollection = null):void
        {
        	var mc:MovieClip = data as MovieClip;
			var rect:Rectangle;
			var frame:AnimationFrame;
			var n:int = AnalyzeIndex + 15;
			if(frames == null)
			{
				frames = new DictionaryCollection();
			}
		
		 	for (AnalyzeIndex ; AnalyzeIndex <= n; AnalyzeIndex++)
		 	{		 		
		 		if(AnalyzeIndex <= mc.totalFrames)
		 		{
			 		mc.gotoAndStop(AnalyzeIndex);
					rect 				  = mc.getBounds(mc);
					frame  				  = new AnimationFrame();
			 		frame.X				  = rect.x;
			 		frame.Y				  = rect.y;
			 		frame.Width			  = rect.width;
			 		frame.Height		  = rect.height;
					frame.FrameBitmapData = new BitmapData(rect.width, rect.height,true, 0);
					frame.FrameBitmapData.draw(mc, new Matrix(1, 0, 0, 1, -rect.x, -rect.y));					
			 		if(frame.Width  > this.maxWidth)  this.maxWidth  = frame.Width;
			 		if(frame.Height > this.maxHeight) this.maxHeight = frame.Height;
					frames.Add(AnalyzeIndex.toString(),frame);
		 		}
		 	}	
		 	
		 	if(AnalyzeIndex > mc.totalFrames)
		 	{
		 	    CommonData.AnimationFrameList[this.DataName] = frames;
		 		
		 		if(LoaderResource != null)
			 	{
			 		LoaderResource.Dispose();
			 		LoaderResource = null;
			 	} 		 	
			 	
				//动画解析完成
			 	if(AnalyzeComplete != null)
			 	{
			 		clearTimeout(timeoutId);
			 		AnalyzeComplete(this);
			 	}
		 	}
		 	else
		 	{
		 		timeoutId = setTimeout(this.cAnalyze,5,mc,frames);
		 	}
        }
        
        public function SetAnimationData(animationPlayer:AnimationPlayer):void
        {
        	animationPlayer.MaxWidth  = this.maxWidth;
        	animationPlayer.MaxHeight = this.maxHeight;
		 	animationPlayer.Clips     = this.clips;
		 	animationPlayer.DataName  = this.DataName;
//		 	animationPlayer.Frames    = this.frames;
        }
	}
}