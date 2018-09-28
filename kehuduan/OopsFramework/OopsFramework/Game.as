package OopsFramework
{
	import OopsFramework.Collections.ArrayCollection;
	import OopsFramework.Content.ContentManager;
	import OopsFramework.Content.Provider.ResourceProvider;
	
	import flash.display.*;
	import flash.events.*;
	import flash.ui.Mouse;
	import flash.utils.*;
	
	/** 游戏主框架 */	
	public class Game extends Sprite
	{
		private static var instance:Game;
		public static function get Instance():Game
		{
			return instance;
		}
		
		private var content:ContentManager;														// 资源管理
		
		private var isRun:Boolean;																// 是否开始运行
		private var isMouseVisible:Boolean  = true;												// 是否不显示鼠标
		private var keyboardEnabled:Boolean = true;												// 键盘事件是否启用
       
        private var gameTime:GameTime = new GameTime();											// 游戏时间对象
		
		private var updateableComponents:Array = [];											// 更新对象集合（IUpdateable）
		private var gameComponents:ArrayCollection;												// 游戏组件集合（IgameComponents）
		
        private var currentlyUpdatingComponents:Array = [];										// 当前要更新的组件集合（IUpdateable）
       
        private var notYetInitialized:Array = [];												// 未初始化的组件集合（IGameComponents）
        
		private var doneFirstUpdate:Boolean = false;											// 是否第一次更新完成（更新一次后才可以开始重绘）
	
		private var screenWidth:uint;															// 游戏窗口宽度
        private var screenHeight:uint;															// 游戏窗口高度
		private var interval : int;																// 每帧间隔时间	
		
		public function Game(stage:Stage = null)
		{
			instance = this;
            this.Run(stage);
		}
		
		/** 游戏开始运行 */
		private function Run(stage:Stage):void
		{
			if(stage == null)
			{
				this.MainStage = this.stage;
			}
			else
			{
				this.MainStage = stage;
			}
			
			this.MainStage.frameRate = 30;
			this.MainStage.align     = StageAlign.TOP_LEFT;
			this.MainStage.scaleMode = StageScaleMode.NO_SCALE;
			this.interval 			 = 1000 / this.MainStage.frameRate;
			this.screenWidth		 = this.MainStage.stageWidth;
			this.screenHeight		 = this.MainStage.stageHeight;
            
            this.mouseEnabled    = false;
			this.KeyboardEnabled = true;
			
			this.gameComponents 		= new ArrayCollection(false);
            this.gameComponents.Added   = GameComponentAdded;
            this.gameComponents.Removed = GameComponentRemoved;
			
			this.content = new ContentManager();
			
            this.Initialize();
            this.isRun = true;

		  	this.Update(this.gameTime);
			
			this.MainStage.addEventListener(Event.RESIZE      , onResize);		// 窗口大小改变事件
			this.MainStage.addEventListener(Event.ENTER_FRAME , onUpdate);		// 重绘事件
		}
		
        /** 可以添加ResourceProviderBase对象到Content.Provider集合 */
        protected virtual function Initialize():void
        {
            while (this.notYetInitialized.length != 0)							// 按顺序初始化一个组件后并从未初始化的组件集合中删除这个组件
            {
                var noYetInit:IGameComponent = this.notYetInitialized.shift() as IGameComponent;
                noYetInit.Initialize();
            }
            
            if(this.ResourceProviderEntity == null)
            {
				this.LoadContent();
            }
			else
			{
		    	this.ResourceProviderEntity.LoadComplete = LoadContent;
				this.ResourceProviderEntity.Load();
			}
        }
		
		/** 游戏资源加载处理（处理加载内存或本址资源逻辑） */
        protected virtual function LoadContent():void { }

        /** 游戏循环更新 */
        protected virtual function Update(gameTime:GameTime):void
        {
			this.currentlyUpdatingComponents = this.updateableComponents.concat();
			for each(var updateable:IUpdateable in this.currentlyUpdatingComponents)
			{
				if (updateable.Enabled)
                {
                    updateable.Update(gameTime);
                }
			}
            this.currentlyUpdatingComponents = null;
            this.doneFirstUpdate = true;
        }
        
        /** 添加游戏组件成功事件 */
        private function GameComponentAdded(item:IGameComponent):void
		{
			if(item is IUpdateable)
        	{
				this.updateableComponents.push(item);
				this.updateableComponents.sortOn("UpdateOrder", Array.NUMERIC);
				
				var gameComponent:IUpdateable    = item as IUpdateable;
				gameComponent.UpdateOrderChanged = onUpdateOrderChanged;
        	}
			
        	if(item is IDrawable)
        	{
				this.addChild(item as DrawableGameComponent);
        	}
			
			// 如果已完成第一次更新，则在组件添加时自动初始化。否则添加到未初始化组件集合中
			if(doneFirstUpdate) 
			{
				item.Initialize();
			}
			else
			{
				this.notYetInitialized.push(item);
			}
		}
		
		/** 删除游戏组件成功事件 */
		private function GameComponentRemoved(item:IGameComponent):void
		{
            var index:int;
			if (this.isRun == false)			// 删除时组件还没有初始化
            {
                index = this.notYetInitialized.indexOf(item);
	    		if(index > -1) 
	    		{
	    			this.notYetInitialized.splice(item,1);
	    		}
            }
            
            var gameComponent:IUpdateable = item as IUpdateable;
            if (gameComponent != null)
            {
				index = this.updateableComponents.indexOf(gameComponent);
	    		if(index > -1) 
	    		{
	    			this.updateableComponents.splice(gameComponent,1);
	    		}
            }
			
            if(item is IDrawable)
            {
				this.removeChild(item as DrawableGameComponent);
            }
		}
        
/** Event Function Start */
		protected function onKeyDown(e:KeyboardEvent):void { if(KeyDown != null) KeyDown(e); }
		protected function onKeyUp(e:KeyboardEvent)  :void { if(KeyUp   != null) KeyUp(e);   }

		/** 更新优先级改变事件 */
		private function onUpdateOrderChanged(item:GameComponent):void
		{
			this.updateableComponents.sortOn("UpdateOrder", Array.NUMERIC);
		}
		
		 /** 窗口大小修改事件 */
		private function onResize(e:Event):void
        {
        	this.screenWidth  = this.MainStage.stageWidth;
          	this.screenHeight = this.MainStage.stageHeight;
			if(Resize!=null) Resize(e);
        }
        
        /** 重绘消息循环 */
        private function onUpdate(e:Event):void
        {
        	// 经验：FlashPlayer在窗口最小化失去焦点时，帧频会变为2。为保证游戏数据计算正确，添加补差更新。
			this.gameTime.ElapsedGameTime = getTimer() - this.gameTime.TotalGameTime;
			var count:int 				  = this.gameTime.ElapsedGameTime / this.interval;
			count 						  = count == 0 ? 1 : count;
			this.gameTime.ElapsedGameTime = this.gameTime.ElapsedGameTime / count;
			for(var i:int = 0; i < count; i++)
			{
				this.gameTime.TotalGameTime = getTimer();
				this.Update(gameTime);
			}
        }
/** Event Function End */
		
		/** 游戏主舞台 */
		public var MainStage : Stage;
		/** 键盘按下事件 */
		public var KeyDown   : Function;
		/** 键盘弹起事件 */
		public var KeyUp     : Function;
		/** 舞台尺寸修改事件 */
		public var Resize    : Function;
		/** 资源提供器 */
		public var ResourceProviderEntity : ResourceProvider;
		
		/** 当前游戏时间对象 */	
		public function get CurrentGameTime():GameTime
		{
			return this.gameTime;
		}
		
        /** 游戏窗口宽度 */											
        public function get ScreenWidth():uint
        {
        	return this.screenWidth;
        }
        
        /** 游戏窗口高度 */
        public function get ScreenHeight():uint
        {
        	return this.screenHeight;
        }
		
		/** 游戏组件集合 */
		public function get Components():ArrayCollection
        {
			return this.gameComponents;
        }
        
        /** 游戏资源管理 */
        public function get Content():ContentManager
        {
			return this.content;
        }
        
        /** 是否显示鼠标 */
		public function get IsMouseVisible():Boolean
		{
			return this.isMouseVisible;
		}
        public function set IsMouseVisible(value:Boolean):void
		{
			if(value)
			{
				Mouse.show();
			}
			else
			{
				Mouse.hide();
			}
			this.isMouseVisible = value;
		}
		
		/** 键盘事件是否启用（默认是启用） */
		public function get KeyboardEnabled():Boolean
		{
			return this.keyboardEnabled;
		}
		public function set KeyboardEnabled(value:Boolean):void
		{
			this.keyboardEnabled = value;
			if(this.keyboardEnabled)
			{
				this.MainStage.addEventListener(KeyboardEvent.KEY_DOWN , onKeyDown);
				this.MainStage.addEventListener(KeyboardEvent.KEY_UP   , onKeyUp);
			}
			else
			{
				this.MainStage.removeEventListener(KeyboardEvent.KEY_DOWN , onKeyDown);
				this.MainStage.removeEventListener(KeyboardEvent.KEY_UP   , onKeyUp);
			}
		}
	}
}