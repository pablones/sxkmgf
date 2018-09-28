package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Graphics.Animation.AnimationEventArgs;
	import OopsEngine.Graphics.Animation.AnimationPlayer;
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Utils.RandomUtility;
	
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	import OopsFramework.GameTime;
	
	import flash.events.MouseEvent;
	
	public class GameElementNPCSkin extends GameElementSkins
	{
		private var action:AnimationPlayer;				// 怪物动做
		private var actionResource:BulkLoaderResourceProvider;		// 攻击皮肤资源加载对象
		private var normalActionCountMax:int = 3;
		private var normalActionCount   :int = 0;
		
		public override function Dispose():void
		{
			actionResource.Dispose();
			
			super.Dispose();
		}	
		
		public function GameElementNPCSkin(gep:GameElementAnimal)
		{
			super(gep);
			
			this.FrameRate = 8;
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
				
				var gAnimationObj:Object = new Object();
				if (this.gep.Role.IsSimpleNPC)
				{
					gAnimationObj[GameElementSkins.ACTION_STATIC] = ["1-2", "1-2", "1-2", "1-2", "1-2"];
					this.FrameRate = 1;
				}
				var geed:GameElementNPCData        = new GameElementNPCData(this.gep.Role.IsSimpleNPC ? gAnimationObj : null);
				geed.LoaderResource                = new BulkLoaderResourceProvider();
				geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
				geed.LoadComplete                  = onPersonResourceComplete;
				geed.AnalyzeComplete               = onPersonAnalyzeComplete;	
				geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName;
				geed.DataName                      = this.gep.Role.PersonSkinName;
				geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName);
			    CommonData.DataAnalyze.push(geed);		
			}
		}
		
		public override function LoadComplete():void
		{
			this.action 			 = new AnimationPlayer();
			this.action.PlayComplete = onActionPlayComplete;
			
			var geed:GameElementNPCData = this.gep.gameScene.CacheResource[this.gep.Role.PersonSkinName];
			geed.SetAnimationData(this.action);
			this.addChild(this.action);
			
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
		private function onPersonResourceComplete(geed:GameElementNPCData):void
		{
			// 如果换装备太快会报错
//			if(geed.LoaderResource.GetResource(this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName)!=null)
//			{
//				geed.AnalyzeComplete           = onPersonAnalyzeComplete;				
//			}			
		}
		
		/** 人物皮肤解析完成  */
		private function onPersonAnalyzeComplete(geed:GameElementNPCData):void
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
	        if(this.action!=null) this.action.StartClip(this.currentActionType + this.currentDirection, frameIndex);
        }
		
        protected override function ActionPlaying(gameTime:GameTime):void 
        {
        	if(this.action!=null)
        	{
        		if (this.gep.Role.IsSimpleNPC)
        			this.FrameRate = 1;
        		this.action.Update(gameTime);
        	}
        }
        
		private function onActionPlayComplete(e:AnimationEventArgs):void
        {
        	if (this.gep.Role.IsSimpleNPC)
        	{
        		if(this.gep.Role.ActionState == GameElementSkins.ACTION_RUN)																// 播放默认动做
				{
					this.gep.Role.ActionState = GameElementSkins.ACTION_STATIC;
					this.ChangeAction(this.gep.Role.ActionState);
				}
				
				if(this.gep.Role.ActionState == GameElementSkins.ACTION_STATIC && this.normalActionCount < this.normalActionCountMax)				// 默认动做计数器
				{
					this.normalActionCount++;
				}
        	}
        	else
        	{
	        	var r:int = RandomUtility.RandomBetweenTwoNumbers(1,10);		// 百分之七十的几率出随机动做
	        	if(this.gep.Role.ActionState == GameElementSkins.ACTION_STATIC && r > 3 && this.normalActionCount == this.normalActionCountMax)	// 默认动做做3次且70%几率播放随机动做
				{
					this.normalActionCount    = 0;
					this.gep.Role.ActionState = GameElementSkins.ACTION_RUN;
					this.ChangeAction(this.gep.Role.ActionState);
				}
				else if(this.gep.Role.ActionState == GameElementSkins.ACTION_RUN)																// 播放默认动做
				{
					this.gep.Role.ActionState = GameElementSkins.ACTION_STATIC;
					this.ChangeAction(this.gep.Role.ActionState);
				}
				
				if(this.gep.Role.ActionState == GameElementSkins.ACTION_STATIC && this.normalActionCount < this.normalActionCountMax)				// 默认动做计数器
				{
					this.normalActionCount++;
				}
        	}
        }
	}
}