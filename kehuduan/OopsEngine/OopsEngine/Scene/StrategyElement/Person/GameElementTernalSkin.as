package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Content.ContentTypeReader;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	import OopsFramework.GameTime;

	public class GameElementTernalSkin extends GameElementSkins
	{
		private var body:GameElementTernalAnimation;				// 主体动作
		private var effect:GameElementTernalAnimation;              // 光影动作
		
		public function GameElementTernalSkin(gep:GameElementAnimal)
		{
			super(gep);
			
			this.FrameRate = 15;
		}
		
		/** 特效完成 */
		public function EffectComplete():void
		{
			//判断是否存在光影
			if(this.gep.Role.WeaponEffectName != null)
			{		
				var geed:GameElementTernalData = this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName).Content;
				
				//判断资源是否正确
				if(geed != null && geed.DataName == this.gep.Role.WeaponEffectName)
				{
					this.effect 			   = new GameElementTernalAnimation();
					geed.SetAnimationData(this.effect);
					
					this.effect.StartClip(this.currentActionType + this.currentDirection);
		
					if(this.body != null)
					{
						this.addChildAt(this.body, this.numChildren);
					}					
					if(this.effect != null)
					{
						this.addChildAt(this.effect, this.numChildren);
					}
				    this.ChangeAction(GameElementSkins.ACTION_STATIC,true);
						
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName]!=null)
					{
						var queue:Array 				   = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName];
						var tempSkin:GameElementTernalSkin = queue.shift();
						while(tempSkin!=null)
						{
							tempSkin.EffectComplete();
							tempSkin = queue.shift();
						}
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName] = null;
						delete this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName];
					}
				}
			}
		}
		
		
		public function BodyComplete():void
		{
			if(this.gep.Role.WeaponSkinName != null)
			{
				CommonData.owner[this.gep.Role.WeaponSkinName] = true;
				
				var geed:GameElementTernalData = this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponSkinName).Content;
				
				//判断武器是否存在 或 正确
				if(geed != null && geed.DataName == this.gep.Role.WeaponSkinName)
				{
					this.body 				   = new GameElementTernalAnimation();
								
					geed.SetAnimationData(this.body);
					
					this.body.StartClip(this.currentActionType + this.currentDirection);
						
					if(this.gep.Role.WeaponEffectName == null)
					{
						this.addChildAt(this.body, this.numChildren);
						this.ChangeAction(GameElementSkins.ACTION_STATIC,true);
					}
					else
					{
						if(this.body != null && this.effect != null)
						{
							this.addChildAt(this.body, this.numChildren);
							this.addChildAt(this.effect, this.numChildren);
							this.ChangeAction(GameElementSkins.ACTION_STATIC,true);
						}					
					}
						
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName]!=null)
					{
						var queue:Array 				   = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName];
						var tempSkin:GameElementTernalSkin = queue.shift();
						while(tempSkin!=null)
						{
							tempSkin.BodyComplete();
							tempSkin = queue.shift();
						}
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName] = null;
						delete this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName];
					}
				}
			}
			
			if(this.gep.Role.WeaponEffectName != null)			
			{
				CommonData.owner[this.gep.Role.WeaponEffectName] = true;
				
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName) != null)
				{
					if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName).State == ContentTypeReader.STATE_USED)
					{
						this.EffectComplete();
					}
					else
					{
						if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName] == null)
						{
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName] = new Array();
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName].push(this);
						}
						else
						{
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectName].push(this);
						}
					}
				}
				else
				{
				    this.gep.Games.Content.Cache.AddStrategyStorage(this.gep.Role.WeaponEffectName);	
					geed                               = new GameElementTernalData();
					geed.LoaderResource                = new BulkLoaderResourceProvider();
					geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
					geed.LoadComplete                  = onEffectResourceComplete;
					geed.AnalyzeComplete               = onEffectAnalyzeComplete;
					geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponEffectName; 
					geed.DataName                      = this.gep.Role.WeaponEffectName;
					geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponEffectName);
					geed.LoaderResource.Load();
				}
			}
		}
		
		protected override function SetActionAndDirection(frameIndex:int = 0):void
        { 
	        if(this.body!=null) this.body.StartClip(this.currentActionType + this.currentDirection, frameIndex);
	        if(this.effect!=null) this.body.StartClip(this.currentActionType + this.currentDirection, frameIndex);
        }
		
		protected override function ActionPlaying(gameTime:GameTime):void 
        {     	
        	if(gep.IsNotUpdate)
        	{  	
	        	if(this.body!=null) this.body.Update(gameTime);
	        	if(this.effect!=null) this.effect.Update(gameTime);
        	}
        }
		
		/** 加载皮肤 */
		public override function LoadSkin():void 
		{
			this.LoadBody();
		}
		
		/** 加载主体 */
		public function LoadBody():void
		{
			if(this.gep.Role.WeaponSkinName != null)
			{
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponSkinName) != null)
				{
					if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponSkinName).State == ContentTypeReader.STATE_USED)
					{
						this.BodyComplete();
					}
					else
					{
						if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName] == null)
						{
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName] = new Array();
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName].push(this);
						}
						else
						{
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName].push(this);
						}
					}
				}
				else
				{		
					this.gep.Games.Content.Cache.AddStrategyStorage(this.gep.Role.WeaponSkinName);	
					var geed:GameElementTernalData     = new GameElementTernalData();
					geed.LoaderResource                = new BulkLoaderResourceProvider();
					geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
					geed.LoadComplete                  = onBodyResourceComplete;
					geed.AnalyzeComplete               = onBodyAnalyzeComplete;
					geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponSkinName; 
					geed.DataName                      = this.gep.Role.WeaponSkinName;
					geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponSkinName);
					geed.LoaderResource.Load();
				}
			}
		}
		
		public function onBodyResourceComplete(geed:GameElementTernalData):void
		{
			if(geed.LoaderResource.GetResource(geed.DataUrl)!=null)
			{						
				geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());						
			}
		}
		
		public function onBodyAnalyzeComplete(geed:GameElementTernalData):void
		{						
			if(SkinLoadComplete!=null)		// 准备缓存武器数据
			{
				SkinLoadComplete(geed.DataName,geed);
				this.gep.Games.Content.Cache.GetStrategyStorage(geed.DataName).State = 4;
			}
			
			this.BodyComplete();				
		}
		
		public function onEffectResourceComplete(geed:GameElementTernalData):void
		{
			if(geed.LoaderResource.GetResource(geed.DataUrl)!=null)
			{						
				geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());						
			}
		}
		
		public function onEffectAnalyzeComplete(geed:GameElementTernalData):void
		{						
			if(SkinLoadComplete!=null)		// 准备缓存武器数据
			{
				SkinLoadComplete(geed.DataName,geed);
				this.gep.Games.Content.Cache.GetStrategyStorage(geed.DataName).State = 4;
			}
			
			this.EffectComplete();				
		}
		
			
		public override function Dispose():void
		{			
			super.Dispose();
		}	
	}
}