package OopsEngine.Scene
{
	import OopsFramework.Collections.DictionaryCollection;
	import OopsFramework.DrawableGameComponent;
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	import OopsFramework.Memory;
	
	import flash.system.System;
	import flash.utils.Dictionary;
	
	/** 管理不同类型的场影实例 */
	public class GameSceneComponent extends DrawableGameComponent
	{
		/** 转场开始事件 */
		public var GameSceneTransferOpen:Function;
		/** 转场数据加载进度事件 */
		public var GameSceneDataProgress:Function;
		/** 转场数据加载完成事件 */
		public var GameSceneLoadComplete:Function;
		
		private var gameScenes:DictionaryCollection;					// 游戏场影集合
		private var previousGameScene : GameScene;						// 上一个游戏场景
		private var currentGameScene  : GameScene;						// 当前活动的游戏场景
		
		private var gameSceneClass:Class;
		
		/** 场景集合 */
		public function get GameScenes():DictionaryCollection
		{
			return this.gameScenes;
		}
		
		/** 获取当前场景 */
		public function get GetGameScene():GameScene
		{
			return this.currentGameScene;
		}
		
		public function GameSceneComponent(game:Game)
		{
			super(game);
			this.name         = "GameSceneComponent";
			this.mouseEnabled = false;
			this.gameScenes   = new DictionaryCollection();
		}
		
		/** 开启一个指定名称的场景 */
		public function StartScene(sceneName:String):void
		{
			if(this.gameScenes[sceneName]!=null)
			{
				this.gameSceneClass = this.gameScenes[sceneName] as Class;
			}
		}
		
		/** 转场 */
		public function TransferScene(name:String, mapId:String):void
		{
			if(this.previousGameScene == null)													// 上一个场景没成功卸载之间不可以进行转场操作
			{
				this.Enabled 				= false;
				this.previousGameScene      = this.currentGameScene;
				this.currentGameScene       = new this.gameSceneClass(this.Games);
				this.currentGameScene.name  = name;
				this.currentGameScene.MapId = mapId;
				this.currentGameScene.GameSceneDataProgress = this.GameSceneDataProgress;
				this.currentGameScene.GameSceneLoadComplete = onGameSceneLoadComplete;
				
				if(GameSceneTransferOpen!=null) GameSceneTransferOpen();
				
				this.currentGameScene.Initialize();
				this.addChild(this.currentGameScene);
			}
		}

		public override function Update(gameTime:GameTime):void
		{
			if(this.currentGameScene!=null && this.currentGameScene.Enabled)
			{
				this.currentGameScene.Update(gameTime);
			}
        	super.Update(gameTime);
		}
		
		public var StrategyStorageMemory:int = 524288000;

		/** 新场景加载完成事件 */
		private function onGameSceneLoadComplete():void
		{
			if(this.previousGameScene!=null)
			{
				this.previousGameScene.Dispose();
				this.previousGameScene = null;
		
				if(System.totalMemory > this.StrategyStorageMemory)
				{
					for(var frame:* in CommonData.AnimationFrameList)
					{
						if(CommonData.owner[frame] == null)
						{
							CommonData.AnimationFrameList[frame].Dispose();
							CommonData.AnimationFrameList[frame] = null;
						    delete	CommonData.AnimationFrameList[frame];
						}
					}
					Game.Instance.Content.Cache.permanentStorage = new Dictionary();
					Game.Instance.Content.Cache.strategyStorage = new DictionaryCollection();
				}
			    							
				// 清除上一场景占用的内存
				Memory.CollectEMS();
			}
			if(GameSceneLoadComplete!=null) GameSceneLoadComplete(this.currentGameScene);
			
			this.Enabled = true;
		}
	}
}