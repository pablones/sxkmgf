package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import OopsFramework.Content.ContentTypeReader;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	import OopsFramework.GameTime;
	
	import flash.events.MouseEvent;
	import flash.geom.Point;

	public class GameElementPlayerSkin extends GameElementSkins
	{
		private var person:GameElementPlayerAnimation;							                     // 人体
		private var weapon:GameElementPlayerAnimation;							                     // 武器
		private var weaponEffect:GameElementPlayerAnimation; 	                                     // 武器光影
		private var mount:GameElementPlayerAnimation;							                     // 坐骑			
//		private var personEffect:GameElementPlayerAnimation;							             // 人体发光效果	
		
		private var isUseMoust:Boolean = false;						 		                         // 是否在使用坐骑
		
		/** 设置变异variation  */
		public override function Setvariation(state:int):void 
		{
			this.gep.Role.variation = state;
			if(person != null) {person.variation = this.gep.Role.variation ;}
			if(weapon != null) {weapon.variation = this.gep.Role.variation ;}
			if(weaponEffect != null) {weaponEffect.variation = this.gep.Role.variation ;}
		}
		
		/**设置人物的层次**/
		public function SetSkinIndex():void
		{
			var index:int = 0;
			if(mount != null)
			{
				this.addChildAt(mount,index);
				index ++;
			}
			if(person != null)
			{
				this.addChildAt(person,index);
				index ++;
			}
			if(weapon != null)
			{
				this.addChildAt(weapon,index);
				index ++;
			}
			
			if(weaponEffect != null && weapon != null)
			{
				this.addChildAt(weaponEffect,index);
				index ++;
			}
			
			Setvariation(this.gep.Role.variation);
		}
		
		public override function Dispose():void
		{	
			super.Dispose();
		}		
		
		public function GameElementPlayerSkin(gep:GameElementAnimal)
		{
			super(gep);
		}
		
		protected override function onMouseMove(e:MouseEvent):void
		{
			var piexl1:uint = person.bitmapData.getPixel32(Math.abs(this.mouseX - person.x), Math.abs(this.mouseY - person.y));
			var alpha1:uint = piexl1 >> 24 & 0xFF;
			
			var alpha2:uint = 0;
			if(weapon!=null)		// 有时间人物没有武器
			{
				var piexl2:uint = weapon.bitmapData.getPixel32(Math.abs(this.mouseX - weapon.x), Math.abs(this.mouseY - weapon.y));
				alpha2			= piexl2 >> 24 & 0xFF;
			}
			
			if(piexl1!=0 || piexl2!=0)
			{
				this.AddHighlight();
			}
			else
			{
				this.DeleteHighlight();
			}
		}
		
		/**移除装备*/
		public override function RemovePersonSkin(skinType:String):void 
		{ 
			switch(skinType)
			{
				case GameElementSkins.EQUIP_PERSON:
					if(this.person!=null && this.contains(this.person))
 					{
						this.removeChild(this.person);
						this.person 			= null;
						gep.Role.PersonSkinName = null;
 					}
					break;
				case GameElementSkins.EQUIP_WEAOIB:
				    //移除武器
 					if(this.weapon!=null && this.contains(this.weapon))
 					{
						this.removeChild(this.weapon);
						this.weapon			    = null;
 					}
 					//移除武器特效
 					if(this.weaponEffect != null)
 					{
						if(this.contains(weaponEffect))
						{
 							this.removeChild(this.weaponEffect);
 						}
 							
						this.weaponEffect			    = null;
 					}
 					gep.Role.WeaponSkinName = null;
 					gep.Role.WeaponEffectModelName  = null;
					gep.Role.WeaponEffectName       = null;
					break;
				case GameElementSkins.EQUIP_MOUNT:
				    if(this.mount!=null)
 					{
						this.removeChild(this.mount);
						this.isUseMoust 	   = false;
						this.mount			   = null;
						this.SetActionAndDirection();
 					}
 					gep.Role.MountSkinName = null;
					break;
				default:
					break
			}
			if(ChangeEquip!=null) ChangeEquip(skinType);
			
			if(ChangeSkins != null) ChangeSkins(skinType,this.gep);
			
		}
		
		/** 换衣服  */
		public function ChangePerson(isChange:Boolean = false):void
		{
			//换衣服则不允许加载资源
			this.gep.IsLoadSkins = false;
			// 时装
			if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.PersonSkinName)!=null)
			{
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.PersonSkinName).State == ContentTypeReader.STATE_USED)
				{
					this.PersonLoadComplete();
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
				this.gep.Games.Content.Cache.AddStrategyStorage(this.gep.Role.PersonSkinName);							
				var geed:GameElementPlayerData     = new GameElementPlayerData();			
				geed.LoaderResource                = new BulkLoaderResourceProvider();
				geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
				geed.LoadComplete                  = onPersonResourceComplete;
				geed.AnalyzeComplete               = onPersonAnalyzeComplete;	
				geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName;
				geed.DataName                      = this.gep.Role.PersonSkinName;
				geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.PersonSkinName);
				
				if(this.gep.Role.Type == GameRole.TYPE_OWNER)
				{			
					geed.LoaderResource.Load();
				}
				else
				{
					CommonData.DataAnalyze.push(geed);
				}
			}
		}
		
		/** 换武器  */
		public function ChangeWeapon(isChange:Boolean = false):void
		{
			if(this.gep.Role.WeaponSkinName!=null)
			{
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponSkinName) != null)
				{					
					if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponSkinName).State == ContentTypeReader.STATE_USED)
					{
						this.WeaponLoadComplete();
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
				      
					var geed:GameElementPlayerData     = new GameElementPlayerData();
					geed.LoaderResource                = new BulkLoaderResourceProvider();
					geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
					geed.LoadComplete                  = onWeaponResourceComplete;
					geed.AnalyzeComplete               = onWeaponAnalyzeComplete;
					geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponSkinName;
					geed.DataName                      = this.gep.Role.WeaponSkinName;
					geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponSkinName);
									
					//判断是否是自己
					if(this.gep.Role.Type == GameRole.TYPE_OWNER)
					{
						geed.LoaderResource.Load();
					}
					else
					{
					    CommonData.DataAnalyze.push(geed);
					}
					
				}
			}		
			else
			{
				if(this.weapon!=null && this.weapon.parent!=null &&  this.contains(weapon))
				{
					this.removeChild(this.weapon);
					this.weapon = null;
				}
				
				if(this.weaponEffect != null && this.contains(weaponEffect))
				{
					this.removeChild(this.weaponEffect);
					this.weaponEffect = null;
				}
			}
		}
		
		/** 换武器特效  */
		public function ChangeWeaponEffect(isChange:Boolean = false):void
		{		
			//武器光影
			if(this.gep.Role.WeaponEffectName != null)
			{
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName) != null)
				{				
					if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName).State == ContentTypeReader.STATE_USED)
					{
						this.WeaponEffectComplete();
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
					var geed:GameElementPlayerData     = new GameElementPlayerData();
					geed.LoaderResource                = new BulkLoaderResourceProvider();
					geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
					geed.LoadComplete                  = onWeaponEffectResourceComplete;
					geed.AnalyzeComplete               = onWeaponEffectAnalyzeComplete;	
					geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponEffectName;
					geed.DataName                      = this.gep.Role.WeaponEffectName;
					geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponEffectName);
					
					if(this.gep.Role.Type == GameRole.TYPE_OWNER)
					{	
						geed.LoaderResource.Load();
					}
					else
					{
						CommonData.DataAnalyze.push(geed);
					}
				}
			}
			else
			{
				//删除特效
				if(this.weaponEffect != null && this.contains(weaponEffect))
				{
					this.removeChild(this.weaponEffect);
					this.weaponEffect = null;
				}
			}
		}
		
		/**重置坐骑高度**/
		public function ResetMount(point:Point,h:int):void
		{
		    // 模型针对坐骑的坐标偏移
			if(this.gep.MountOffset!=null)
			{
				this.mount.offsetX  += this.gep.MountOffset.x;
				this.mount.offsetY  += this.gep.MountOffset.y;
				this.gep.MountOffset = point;
				this.mount.offsetX  -= this.gep.MountOffset.x;
				this.mount.offsetY  -= this.gep.MountOffset.y;
				
				if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_MOUNT,this.gep);		
			}
		}
		
		/** 换坐骑  */
		public function ChangeMount(isChange:Boolean = false):void
		{
			// 坐骑
			if(this.gep.Role.MountSkinName!=null)
			{
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.MountSkinName) != null)
				{		
					if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.MountSkinName).State == ContentTypeReader.STATE_USED)
					{
						this.MountLoadComplete();
					}
					else
					{
						if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName] == null)
						{
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName] = new Array();
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName].push(this);
						}
						else
						{
							this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName].push(this);
						}
					}
				}				
				else
				{
					this.gep.Games.Content.Cache.AddStrategyStorage(this.gep.Role.MountSkinName);				
					var geed:GameElementMountData      = new GameElementMountData();
					geed.LoaderResource                = new BulkLoaderResourceProvider();
					geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
					geed.LoadComplete                  = onMountResourceComplete;
					geed.AnalyzeComplete               = onMountAnalyzeComplete;	
					geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.MountSkinName;
					geed.DataName                      = this.gep.Role.MountSkinName;
					geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.MountSkinName);
					
					if(this.gep.Role.Type == GameRole.TYPE_OWNER)
					{	
						geed.LoaderResource.Load();
					}
					else
					{
						CommonData.DataAnalyze.push(geed);
					}
				}
			}
			else
			{
				if(this.mount!=null && this.contains(this.mount))
				{
					this.removeChild(this.mount);
					this.mount = null;
				}
			}
		}
		
		public override function LoadSkin():void 
		{
			// 衣服
			this.ChangePerson();
			// 人物加载完成后在加载武器
			this.ChangeWeapon();
			// 坐骑
			this.ChangeMount();
		}
		
		/** 人物皮肤加载  */
		public function PersonLoadComplete():void
		{
			if(this.person!=null && this.contains(person))
			{
				this.removeChild(this.person);			
				this.person = null;			
			}
			
			if(ChangeEquip != null) ChangeEquip(GameElementSkins.EQUIP_PERSON);
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_PERSON,this.gep);
				
			//判断加载是时候是否拥有皮肤 这个时候也许皮肤正在切换
			if(this.gep.Role.PersonSkinName != null)
			{
				var geed:GameElementPlayerData = this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.PersonSkinName).Content;
				
				//判断是否是同一个皮肤
				if(geed != null && geed.DataName == this.gep.Role.PersonSkinName)
				{
					this.person 			 = new GameElementPlayerAnimation();
					this.person.PlayFrame    = this.ActionPlayFrame;
					this.person.PlayComplete = this.ActionPlayComplete;
					
					geed.SetAnimationData(this.person);
					
		
		            //设置层次
					SetSkinIndex();
					
					this.MaxBodyWidth  = this.person.MaxWidth;
					this.MaxBodyHeight = this.person.MaxHeight;
					
					if(BodyLoadComplete!=null) BodyLoadComplete();
		
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName]!=null)
					{
						var queue:Array 			  	   = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName];
						var tempSkin:GameElementPlayerSkin = queue.shift();
						while(tempSkin!=null)
						{
							tempSkin.PersonLoadComplete();
							tempSkin = queue.shift();
						}
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName] = null;
						delete this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.PersonSkinName];
					}						
				}
			}
		}

		/** 武器加载  */
		public function WeaponLoadComplete():void
		{
			if(this.weapon!=null && this.weapon.parent!=null &&  this.contains(weapon))
			{
				this.removeChild(this.weapon);
				this.weapon = null;
			}
			
			if(this.weaponEffect != null && this.contains(weaponEffect))
			{
				this.removeChild(this.weaponEffect);
				this.weaponEffect = null;
			}
			
			if(ChangeEquip != null) ChangeEquip(GameElementSkins.EQUIP_WEAOIB);
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_WEAOIB,this.gep);
			
			//判断是否存在武器
			if(this.gep.Role.WeaponSkinName != null)
			{
				var geed:GameElementPlayerData = this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponSkinName).Content;
				
				//判断武器是否存在 或 正确
				if(geed != null && geed.DataName == this.gep.Role.WeaponSkinName)
				{
					this.weapon 				   = new GameElementPlayerAnimation();
								
					geed.SetAnimationData(this.weapon);
					
					if(this.gep.Role.HP == 0)
					{
						this.weapon.StartClip(this.currentActionType + this.currentDirection,7);
					}
					else
					{
						this.weapon.StartClip(this.currentActionType + this.currentDirection);
					}
						
					if(this.gep.Role.WeaponEffectName == null)
					{
						//设置层次
						SetSkinIndex();
					}
						
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName]!=null)
					{
						var queue:Array 				   = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName];
						var tempSkin:GameElementPlayerSkin = queue.shift();
						while(tempSkin!=null)
						{
							tempSkin.WeaponLoadComplete();
							tempSkin = queue.shift();
						}
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName] = null;
						delete this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponSkinName];
					}
				}
			}
				
			//武器光影
			if(this.gep.Role.WeaponEffectName != null)
			{
				if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName) != null)
				{
					if(this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName).State == ContentTypeReader.STATE_USED)
					{
						this.WeaponEffectComplete();
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
					geed                               = new GameElementPlayerData();
					geed.LoaderResource                = new BulkLoaderResourceProvider();
					geed.LoaderResource.LoadComplete   = geed.onLoadComplete;
					geed.LoadComplete                  = onWeaponEffectResourceComplete;
					geed.AnalyzeComplete               = onWeaponEffectAnalyzeComplete;
					geed.DataUrl                       = this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponEffectName; 
					geed.DataName                      = this.gep.Role.WeaponEffectName;
					geed.LoaderResource.Download.Add(this.gep.Games.Content.RootDirectory + this.gep.Role.WeaponEffectName);
					if(this.gep.Role.Type == GameRole.TYPE_OWNER)
					{	
						geed.LoaderResource.Load();
					}
					else
					{
						CommonData.DataAnalyze.push(geed);
					}
				}
			}
		}
		
		public function WeaponEffectComplete():void
		{
		    //删除特效
			if(this.weaponEffect != null && this.contains(weaponEffect))
			{
				this.removeChild(this.weaponEffect);
				this.weaponEffect = null;
			}
			
			if(ChangeEquip != null) ChangeEquip(GameElementSkins.EQUIP_WEAOIB);
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_WEAOIB,this.gep);
			
			//判断是否存在光影
			if(this.gep.Role.WeaponEffectName != null)
			{		
				var geed:GameElementPlayerData = this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.WeaponEffectName).Content;
				
				//判断资源是否正确
				if(geed != null && geed.DataName == this.gep.Role.WeaponEffectName)
				{
					this.weaponEffect 			   = new GameElementPlayerAnimation();
					geed.SetAnimationData(this.weaponEffect);
					
					if(this.gep.Role.HP == 0)
					{
						this.weaponEffect.StartClip(this.currentActionType + this.currentDirection,7);
						weaponEffect.blendMode = this.gep.Role.WeaponEffectModelName;
						weaponEffect.alpha     = this.gep.Role.WeaponDiaphaneity;
					}
					else
					{
						this.weaponEffect.StartClip(this.currentActionType + this.currentDirection);
						weaponEffect.blendMode = this.gep.Role.WeaponEffectModelName;
						weaponEffect.alpha     = this.gep.Role.WeaponDiaphaneity;
					}
		
		            //设置层次
					SetSkinIndex();
						
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectModelName]!=null)
					{
						var queue:Array 				   = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectModelName];
						var tempSkin:GameElementPlayerSkin = queue.shift();
						while(tempSkin!=null)
						{
							tempSkin.WeaponEffectComplete();
							tempSkin = queue.shift();
						}
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectModelName] = null;
						delete this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.WeaponEffectModelName];
					}
				}
			}
		}
		
			
		/** 坐骑加载  */
		public function MountLoadComplete():void
		{
			if(this.mount!=null && this.contains(this.mount))
			{
				this.removeChild(this.mount);
				this.mount = null;
			}
					
			this.isUseMoust = true;
								
			if(this.gep.Role.MountSkinName != null)
			{
				var geed:GameElementMountData = this.gep.Games.Content.Cache.GetStrategyStorage(this.gep.Role.MountSkinName).Content;
				if(geed != null && geed.DataName == this.gep.Role.MountSkinName)
				{
					this.mount = new GameElementPlayerAnimation();
					geed.SetAnimationData(this.mount);
					
					// 模型针对坐骑的坐标偏移
					if(this.gep.MountOffset!=null)
					{
						this.mount.offsetX -= this.gep.MountOffset.x;
						this.mount.offsetY -= this.gep.MountOffset.y;
					}
					
					this.SetActionAndDirection();				// 修改动做和方向
					
					SetSkinIndex();
					
					if(this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName]!=null)
					{
						var queue:Array 			  	   = this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName];
						var tempSkin:GameElementPlayerSkin = queue.shift();
						while(tempSkin!=null)
						{
							tempSkin.MountLoadComplete();
							tempSkin = queue.shift();
						}
						this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName] = null;
						delete this.gep.gameScene.ResourceLoadingQueue[this.gep.Role.MountSkinName];
					}
				}
			}
			
			if(ChangeEquip != null) ChangeEquip(GameElementSkins.EQUIP_MOUNT);
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_MOUNT,this.gep);	
		}
		
		/** 人物皮肤下载完成  */
		private function onPersonResourceComplete(geed:GameElementPlayerData):void
		{											
			if(geed.LoaderResource.GetResource(geed.DataUrl)!=null)
			{				
				if(this.gep.Role.Type == GameRole.TYPE_OWNER)
				{
					geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());
			 	}
			}
		}
		
		/** 人物皮肤解析完成  */
		private function onPersonAnalyzeComplete(geed:GameElementPlayerData):void
		{
			if(SkinLoadComplete!=null)		// 准备缓存皮肤数据
			{
				SkinLoadComplete(geed.DataName,geed);
				this.gep.Games.Content.Cache.GetStrategyStorage(geed.DataName).State = 4;
			}
			
			this.PersonLoadComplete();			
			
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_PERSON,this.gep);
			
		}
		
		
		private function onWeaponResourceComplete(geed:GameElementPlayerData):void
		{								
			if(geed.LoaderResource.GetResource(geed.DataUrl)!=null)
			{				
				if(this.gep.Role.Type == GameRole.TYPE_OWNER)
				{
			    	geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());
			 	}			
			}
		}
		
		/** 人物皮肤解析完成  */
		private function onWeaponAnalyzeComplete(geed:GameElementPlayerData):void
		{				
			if(SkinLoadComplete!=null)		// 准备缓存武器数据
			{
				SkinLoadComplete(geed.DataName,geed);
				this.gep.Games.Content.Cache.GetStrategyStorage(geed.DataName).State = 4;
			}
			
			this.WeaponLoadComplete();
				
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_WEAOIB,this.gep);
		}
		
		public function onWeaponEffectResourceComplete(geed:GameElementPlayerData):void
		{
			if(geed.LoaderResource.GetResource(geed.DataUrl)!=null)
			{						
				if(this.gep.Role.Type == GameRole.TYPE_OWNER)
				{
					geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());
			 	}								
			}
		}
		
		public function onWeaponEffectAnalyzeComplete(geed:GameElementPlayerData):void
		{						
			if(SkinLoadComplete!=null)		// 准备缓存武器数据
			{
				SkinLoadComplete(geed.DataName,geed);
				this.gep.Games.Content.Cache.GetStrategyStorage(geed.DataName).State = 4;
			}
			
			this.WeaponEffectComplete();	
			
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_WEAOIB,this.gep);
		}
		
		private function onMountResourceComplete(geed:GameElementMountData):void
		{
			if(geed.LoaderResource.GetResource(geed.DataUrl)!=null)
			{								
				if(this.gep.Role.Type == GameRole.TYPE_OWNER)
				{
					geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());
			 	}			
			}
		}
		
		public function onMountAnalyzeComplete(geed:GameElementMountData):void
		{
			if(SkinLoadComplete!=null)		
			{
				SkinLoadComplete(geed.DataName,geed);
			    this.gep.Games.Content.Cache.GetStrategyStorage(geed.DataName).State = 4;
			}
			
			this.MountLoadComplete();
				
			if(ChangeSkins != null) ChangeSkins(GameElementSkins.EQUIP_MOUNT,this.gep);
		}
	
        protected override function ActionPlaying(gameTime:GameTime):void 
        {
        	if(gep.IsNotUpdate)
        	{      		
	        	if(this.person!=null) this.person.Update(gameTime);
	        	if(this.weapon!=null) this.weapon.Update(gameTime);
	        	if(this.weaponEffect!=null) this.weaponEffect.Update(gameTime);
	        }
        }
        
        protected override function SetActionAndDirection(frameIndex:int = 0):void   
        {
        	gep.IsNotUpdate = false;
			if(this.gep.Role.ActionState.indexOf(GameElementSkins.ACTION_STATIC) > -1)			// 休闲动做
			{
				this.FrameRate = 3;
			}
			else if(this.gep.Role.ActionState.indexOf(GameElementSkins.ACTION_DEAD) > -1)		// 死亡动做
			{
				this.FrameRate = 10;
			}
			else if(this.gep.Role.ActionState.indexOf(GameElementSkins.ACTION_RUN) > -1)		// 行走动做
			{
				this.FrameRate = 12;
			}
			else 																				// 攻击动做
			{
				if((this.gep.handler != null && this.gep.handler.Floor > 5) || this.gep.QuickPlay)
				{
					this.FrameRate = 50;
					//快播开始
					this.gep.QuickPlay = true;
				}
				else
				{
					this.FrameRate = 14;
				}
			}
			
			var type:String = this.currentActionType;
			var index:int   = frameIndex;	
			
	        if(this.person!=null)
	        {
		        if(this.isUseMoust==true && this.gep.Role.ActionState == GameElementSkins.ACTION_RUN)
		        {
		        	this.person.StartClip(GameElementSkins.ACTION_STATIC + this.currentDirection, index);
		        }
		        else
		        {
		        	this.person.StartClip(type + this.currentDirection, index);
		        }
		    }
		    

	 	    if(this.weapon!=null)
	 	    {
		 	    if(this.isUseMoust==true && this.gep.Role.ActionState == GameElementSkins.ACTION_RUN)
		 	    {
		 	     	this.weapon.StartClip(GameElementSkins.ACTION_STATIC + this.currentDirection, index);
		 	    }
		 	    else
		 	    {
		 	    	this.weapon.StartClip(type + this.currentDirection, index);
		 	    }
		 	}
		 	
		 	if(this.weaponEffect !=null)
	 	    {
		 	    if(this.isUseMoust==true && this.gep.Role.ActionState == GameElementSkins.ACTION_RUN)
		 	    {
		 	     	this.weaponEffect.StartClip(GameElementSkins.ACTION_STATIC + this.currentDirection, index);
		 	    }
		 	    else
		 	    {
		 	    	this.weaponEffect.StartClip(type + this.currentDirection, index);
		 	    }
		 	}

	 	    if(this.mount!=null)  this.mount.StartClip(type + this.currentDirection, index);
	 	    
	 	    gep.IsNotUpdate = true;
        }
	}
}