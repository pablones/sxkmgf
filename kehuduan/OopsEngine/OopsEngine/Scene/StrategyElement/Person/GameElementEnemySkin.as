package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	import OopsFramework.GameTime;
	
	import flash.events.MouseEvent;
	
	public class GameElementEnemySkin extends GameElementSkins
	{
		public var action:GameElementEnemyAnimation;				// 怪物动做
		private var actionResource:BulkLoaderResourceProvider;		// 攻击皮肤资源加载对象
		
		/** 设置变异variation  */
		public override function Setvariation(state:int):void 
		{
			this.gep.Role.variation = state;
			if(action != null) {action.variation = state;}
		}
		
		public override function Dispose():void
		{
			actionResource.Dispose();
			
			super.Dispose();
		}	
		
		public function GameElementEnemySkin(gep:GameElementAnimal)
		{
			super(gep);
		}
		
//		protected override function onMouseOver(e:MouseEvent):void
//		{
//			if(this.isEffect)
//			{
//				this.AddHighlight();
//			}
//		}
//		
		protected override function onMouseOut(e:MouseEvent):void
		{
			if(this.isEffect)
			{
				this.DeleteHighlight();
			}
		}
		
		protected override function onMouseMove(e:MouseEvent):void
		{
			if(this.isEffect)
			{
				this.AddHighlight();
			}
//			var piexl1:uint = action.bitmapData.getPixel32(Math.abs(this.mouseX - action.x), Math.abs(this.mouseY - action.y));
//			var alpha1:uint = piexl1 >> 24 & 0xFF;
//			
//			if(piexl1!=0)
//			{
//				this.AddHighlight();
//			}
//			else
//			{
//				this.DeleteHighlight();
//			}
		}
				
		/** 修改人体皮肤  */
		public override function LoadSkin():void 
		{
			if(this.gep.gameScene.CacheResource[this.gep.Role.PersonSkinName]!=null)
			{
				if(this.gep.gameScene.CacheResource[this.gep.Role.PersonSkinName]!=true)
				{
					this.LoadComplete();
				}
				else
				{
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName] == null)
					{
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName] = new Array();
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName].push(this);
					}
					else
					{
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName].push(this);
					}
				}
			}
			else
			{										
				this.gep.gameScene.CacheResource[this.gep.Role.PersonSkinName] = true;
				
				var geed:GameElementEnemyData      = new GameElementEnemyData();
				geed.LoaderResource                = new BulkLoaderResourceProvider();
				geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
				geed.LoadComplete                  = onPersonResourceComplete;
				geed.AnalyzeComplete               = onPersonAnalyzeComplete;	
				geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName;
				geed.DataName                      = this.gep.Role.PersonSkinName;
				geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName);
				CommonData.DataAnalyze.push(geed);	
//				geed.LoaderResource.Load();
				
//				this.actionResource    		     = new BulkLoaderResourceProvider();
//				this.actionResource.LoadComplete = onPersonResourceComplete;
//				this.actionResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName);
////				this.gep.gameScene.LoadingQueue.push(this.actionResource);
////				this.gep.gameScene.Loading();
//				this.actionResource.Load();
			}
		}
		
		public override function LoadComplete():void
		{
			this.action 			 = new GameElementEnemyAnimation();
			this.action.PlayComplete = this.ActionPlayComplete;
			this.action.PlayFrame    = this.ActionPlayFrame;
			
			var geed:GameElementEnemyData = this.gep.gameScene.CacheResource[this.gep.Role.PersonSkinName];
			geed.SetAnimationData(this.action);
			this.addChild(this.action);
			Setvariation(this.gep.Role.variation);
			this.MaxBodyWidth  = this.action.MaxWidth;
			this.MaxBodyHeight = this.action.MaxHeight;
			
			if(BodyLoadComplete!=null) BodyLoadComplete();
			
			if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName]!=null)
			{
				var queue:Array 			  = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName];
				var tempSkin:GameElementSkins = queue.shift();
				while(tempSkin!=null)
				{
					tempSkin.LoadComplete();
					tempSkin = queue.shift();
				}
			}
		}
		
		/** 怪物皮肤下载完成  */
		private function onPersonResourceComplete(geed:GameElementEnemyData):void
		{
//			this.gep.gameScene.Loaded();

			// 如果换装备太快会报错
//			if(geed.LoaderResource.GetResource(this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName)!=null)
//			{
//				geed.AnalyzeComplete           = onPersonAnalyzeComplete;	
//			}
		}
		
		/** 人物皮肤解析完成  */
		private function onPersonAnalyzeComplete(geed:GameElementEnemyData):void
		{
			if(SkinLoadComplete!=null)
			{
				SkinLoadComplete(geed.DataName,geed);
			}
			
			this.LoadComplete();			
			
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_PERSON,this.gep);
			
		}
		
        protected override function SetActionAndDirection(frameIndex:int = 0):void
        { 
			if(this.gep.Role.ActionState.indexOf(GameElementSkins.ACTION_STATIC) > -1)			// 休闲状态
			{
				this.FrameRate = 4;
			}
			else if(this.gep.Role.ActionState.indexOf(GameElementSkins.ACTION_DEAD) > -1)		// 死亡状态
			{
				this.FrameRate = 9;
			}
			else if(this.gep.Role.ActionState.indexOf(GameElementSkins.ACTION_RUN) > -1)		// 行走
			{
				this.FrameRate = 8;
			}
			else
			{
				this.FrameRate = 15;
			}
	        if(this.action!=null) this.action.StartClip(this.currentActionType + this.currentDirection, frameIndex);
        }
		
        protected override function ActionPlaying(gameTime:GameTime):void 
        {
        	if(this.action!=null) this.action.Update(gameTime);
        }
	}
}