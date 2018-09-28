package OopsEngine.Scene.StrategyElement
{
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Engine;
	import OopsEngine.Graphics.Animation.AnimationEventArgs;
	import OopsEngine.Graphics.Font;
	import OopsEngine.Role.Appellation;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.GameElement;
	import OopsEngine.Scene.GameScene;
	import OopsEngine.Scene.Handler;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	import OopsEngine.Skill.SkillAnimation;
	import OopsEngine.Utils.SmoothMove;
	
	import OopsFramework.Collections.DictionaryCollection;
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	
	import flash.display.Bitmap;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import flash.utils.Dictionary;
	
	/** 
	 * 人物抽象元件（ 和服务器通讯坐标点为人物脚下的坐标点－源坐标减人物偏移值）
	 * 功能：
	 * 1、人物皮肤加载
	 * 2、人物动画
	 * 3、人物移动
	 * 4、人物名称
	 */
	public class GameElementAnimal extends GameElement
	{
		/** 人物角色信息 */
		public var Role:GameRole;
		public var gameScene:GameScene;							     // 人物当前所在场景对象
		public var handler:Handler;								     // 技能动做职责链对像
		public var Offset:Point;								     // 人物动做偏移坐标
		public var MountOffset:Point;							     // 坐骑偏移坐标
		public var OffsetHeight:int;							     // 偏移高度
		public var MountHeight:int;								     // 坐骑高度
		public var golem:Bitmap;								     // 人偶
		
		public    var smoothMove:SmoothMove;					     // 人物平滑移动对象
		protected var personTitle:TextField;					     // 人物心情显示区
		protected var personAppellation:TextField;                   // 人物称号显示区
		protected var personName:TextField;						     // 人物名字显示区
		public    var IsShowAppellation:Boolean;                     // 是否显示称号
		public    var SelectAppellationID:int;                       // 当前选择称号的编号
		
		protected var skins:GameElementSkins;					     // 人物皮肤对象
		protected var targetPoint:Point;						     // 移动场景终点
		protected var teamLeaderSign:Bitmap;					     // 队长标
		protected var teamMemberSign:Bitmap;					     // 队员标
		protected var shadow:Bitmap;							     // 影子
		protected var playerStall:Bitmap;						     // 玩家摊位图标
		protected var playerBanner:Bitmap;                           // 旗帜
		protected var missionPrompt:DisplayObject;				     // 任务提示
		
		public var IsStall:Boolean = false;						     // 是否在摆摊
		
		protected var isInitialize:Boolean    = false;			     // 是否加载人物资源完成，没完成时人物因为服务器发信息而不会移动。
		private var isActionPlaying:Boolean = true;				     // 是否正在播放动做动画
		public  var golemSprite:Sprite  = new Sprite();
		private var textFieldHeight:int = 20;					     // 文本框高
		public  var elementWidth:int    = 130;					     // 元件宽
			
		public var IsAutomatism:Boolean = false;                     // 是否挂机
		public var IsLoadSkins:Boolean  = false;                     // 是否加载成功
		
		public var IsAttack:Boolean     = false;                     //是否指定了攻击目标
		public var Attack:Function      = null;                      //攻击函数
		 
		public var AppellationList:Array = [];         				 // 人物拥有称号列表 只有编号
	    public var skillAnimation:Array  = [];     			 	     // 技能播放动画数组
	    public var skillEffectList:Array = [];            			 // 飞行动画列表
	    
	    public var IsNotUpdate:Boolean  = true;                      // 切换时候不更新动画
	    public var QuickPlay:Boolean    = false;                     // 快速播放
	    
	    public var MustMovePoint:Point = null;                       // 必须移动到的点
	    public var Dis:int             = 0;                          // 攻击距离		
		public var IsMustDel:Boolean = false;                        // 立即删除
		
		public var IsAddAttack:Boolean = false;                      // 是否进入攻击状态	
		
		public var MustMove:Function   = null;                       // 移动
		public var StopAttack:Function = null;                       // 取消攻击
		
		public var playerEffect:SkillAnimation;
			
		public var SetPlayer:Function;
		
		public var txftalk:DisplayObjectContainer ;
		public var talkmc :MovieClip ;
		public var talkId :int;                      
			
		/** 设置变异variation  */
		public function Setvariation(state:int):void 
		{ 
			this.skins.Setvariation(state);
		}
		
		public override function Dispose():void
		{
			if(this.Role.Type != GameRole.TYPE_OWNER)
			{
//				this.gameScene	 = null;			// 删除后还有对象用，有问题
//				this.Role 	     = null;			// 删除后还有对象用，有问题
//				this.smoothMove  = null;
//				if(this.skins!=null)
//				{
//					this.skins.Dispose();			// 删除后会让现有的模型动画不在播放
//					this.skins = null;
//				}
				
//				this.AppellationList = null;         				 
//	    		this.skillAnimation  = null;     	// 删除后还有对象用，有问题
//	   			this.skillEffectList = null;            			
				
				this.golemSprite 	   = null;
//				this.personTitle 	   = null;
//				this.personName  	   = null;      //人物死亡在原地复活在次死亡
				this.personAppellation = null;
				this.StopAttack  	   = null;
				this.MustMove 		   = null;
				this.MustMovePoint	   = null;
				this.targetPoint	   = null;
				this.teamLeaderSign    = null;
				this.teamMemberSign    = null;
//				this.shadow    		   = null;		// 删除后还有对象用，有问题（死亡后复活，其它人看不到脚下影子）
				this.playerStall       = null;
				this.playerBanner      = null;
				this.missionPrompt     = null;
				this.handler		   = null;
				this.Offset		       = null;
				this.MountOffset       = null;
				this.golem			   = null;
				
				super.Dispose();
			}
//			else
//			{
//				this.initialized  = false;
//				this.IsLoadSkins  = false;
//				this.isInitialize = false;
//				this.isActionPlaying = true;
//				this.golemSprite  = new Sprite();
//			}
		}
			
		private function onSkinLoadComplete(key:String,data:GameElementData):void
		{
			if(this.gameScene!=null)
			{
				if(this.gameScene.CacheResource==null)
				{
					this.gameScene.CacheResource = new DictionaryCollection();
				} 
				if(this.gameScene.ResourceLoadingQueue==null)
				{
					this.gameScene.ResourceLoadingQueue = new Dictionary();
				}
				if(key.toLowerCase().indexOf("player")==-1)
				{
					this.gameScene.CacheResource.Add(key,data);
				}
				else
				{
					this.gameScene.Games.Content.Cache.AddStrategyStorage(key,data);	// 永久缓存
				}
			}
		}
		
		/** 人物皮肤  */
		public function get Skins():GameElementSkins
		{
			return this.skins;
		}
	
	  	/** 移除装备*/
		public virtual function RemoveSkin(skinType:String):void { }
		
		/** 人物换装备 */
		public virtual function SetSkin(skinType:String,skinName:String):void{}
		
		public function GameElementAnimal(game:Game, skins:GameElementSkins)
		{ 
			super(game);
			this.skins = skins;
			if(this.skins!=null)
			{
				this.skins.ActionPlayComplete = onActionPlayComplete;
				this.skins.ActionPlayFrame    = onActionPlayFrame;
				this.skins.BodyLoadComplete   = onBodyLoadComplete;
				this.skins.MouseOutTarger	  = onMouseOutTarger;
				this.skins.MouseOverTarger	  = onMouseOverTarger;
				this.skins.SkinLoadComplete   = onSkinLoadComplete;
			}
			this.mouseChildren = true;					// 用于人手身体可以被选中
			
			// 经验：加载皮肤前的小蓝人的可点击逻辑要和皮肤的点击逻辑设计到一起。
			this.golemSprite.addEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
			this.golemSprite.addEventListener(MouseEvent.MOUSE_OUT,  onMouseOut);
		}
		
		private function onMouseOver(e:MouseEvent):void
		{
			if(this.golem!=null)
			{
				this.golemSprite.addEventListener(MouseEvent.MOUSE_MOVE,onMouseMove);
			}
		}
		
		private function onMouseMove(e:MouseEvent):void
		{
			if(this.golem!=null)
			{
				var piexl1:uint = this.golem.bitmapData.getPixel32(Math.abs(this.golemSprite.mouseX - this.golem.x), Math.abs(this.golemSprite.mouseY - this.golem.y));
				var alpha1:uint = piexl1 >> 24 & 0xFF;
				if(alpha1 !=0 && MouseOverTarger!=null) 
				{
					MouseOverTarger(this);
					this.golemSprite.addEventListener(MouseEvent.MOUSE_MOVE,onMouseDown);
				}
				else if(MouseOutTarger!=null) 
				{
					MouseOutTarger(this);
					this.golemSprite.removeEventListener(MouseEvent.MOUSE_MOVE,onMouseDown);
				}
			}
		}
		
		private function onMouseOut(e:MouseEvent):void
		{
			if(this.golem!=null)
			{
				this.golemSprite.removeEventListener(MouseEvent.MOUSE_MOVE,onMouseMove);
				this.golemSprite.removeEventListener(MouseEvent.MOUSE_MOVE,onMouseDown);
				if(MouseOutTarger!=null) MouseOutTarger(this);
			}
		}
		
		private function onMouseDown(e:MouseEvent):void
		{
			if(this.golem!=null)
			{
				if(ChooseTarger!=null) ChooseTarger(this);
			}
		}
		
		/** 设置父级场景对象  */
		public virtual function SetParentScene(gameScene:GameScene):void
		{
			this.gameScene = gameScene;
			
			if(this.gameScene!=null)
			{
				if(this.gameScene.CacheResource==null)
				{
					this.gameScene.CacheResource = new DictionaryCollection(true);
				}
				if(this.gameScene.ResourceLoadingQueue==null)
				{
					this.gameScene.ResourceLoadingQueue = new Dictionary(true);
				}
			}
		}
		
		/** 设置移动速度  */
		public function SetMoveSpend(moveStepLength:int):void
		{
			if(this.smoothMove==null)
			{
				// 定义人物移动滚屏动画
				this.smoothMove 			 = new SmoothMove(this, moveStepLength);
				this.smoothMove.MoveNode	 = onMoveNode;			// 每当准备走到下一节点时的事件
				this.smoothMove.MoveStep	 = onMoveStep;			// 每走一步后事件
				this.smoothMove.MoveComplete = onMoveComplete;		// 触发移动完成事件
				this.smoothMove.CheckPoint   = CheckPoint;          // 核对点
			}
			else
			{
				this.smoothMove.MoveStepLength = moveStepLength;
			}
		}
		
		public override function Initialize():void
		{
            if(this.skins != null)
			{
				if(this.isInitialize == false)	// 人物不可以初始化一次以前
				{
					
					this.SetMoveSpend(5);		// 16 20				
					// 人物称号显示区
					this.personTitle       = new TextField();
					var format1:TextFormat = new TextFormat();
					format1.align 		   = TextFormatAlign.CENTER;
					format1.color		   = this.Role.TitleColor;
					format1.size		   = 12;
					format1.font		   = "宋体";
					this.personTitle.defaultTextFormat = format1;
				    this.personTitle.cacheAsBitmap     = true;
					this.personTitle.mouseEnabled      = false;
					this.personTitle.selectable 	   = false;
					this.personTitle.type			   = TextFieldType.DYNAMIC;
					this.personTitle.text    = this.Role.Title==null ? "" : this.Role.Title;
					this.personTitle.filters = Font.Stroke(this.Role.TitleBorderColor);
					this.personTitle.x  	 = 0;
					this.personTitle.y  	 = 0;
					this.personTitle.width   = this.elementWidth;
					this.personTitle.height  = this.textFieldHeight;
					this.addChild(this.personTitle);
					
					// 人物名称显示区
					this.personName        = new TextField();
					var format2:TextFormat = new TextFormat();
					format2.align 		   = TextFormatAlign.CENTER;
					format2.color		   = this.Role.NameColor;
					format2.size		   = 12;
					format2.font		   = "宋体";
					this.personName.defaultTextFormat = format2;
			        this.personName.cacheAsBitmap     = true;
					this.personName.mouseEnabled      = false;
					this.personName.selectable 		  = false;
					this.personName.type			  = TextFieldType.DYNAMIC;
					this.SetName(this.Role.Name);
					
					this.personName.filters = Font.Stroke(this.Role.NameBorderColor);	

					this.personName.x  		= 0;
					this.personName.y  		= 15;
					
					this.personName.width   = this.elementWidth;
					this.personTitle.height = this.textFieldHeight;
					this.addChild(this.personName);
					
					if(this.Role.Type == GameRole.TYPE_PLAYER || this.Role.Type == GameRole.TYPE_OWNER)
					{
						this.HideTitle();
					}
 
                    if(this.Role.Type == GameRole.TYPE_BANNER) 
           		    {
            			// 显示旗帜
						PlayerBanner();
                    }           
                    else  if((this.Role.Type == GameRole.TYPE_PLAYER || this.Role.Type == GameRole.TYPE_OWNER) && this.Role.StallId !=0)
					{
						// 显示摊位
						this.PlayerStall();
					}
					else
					{
						// 预显示人偶
						this.golem   = new Bitmap(CommonData.Golem(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Golem")));
						this.golem.x = (this.elementWidth - this.golem.width) / 2;
						this.golem.y = 40;
						this.golemSprite.addChild(this.golem);
						this.addChild(this.golemSprite);
					}
			
					if(this.Role.StallId !=0)		// 摆滩时不显示
					{
						this.HideName();
						this.HideTitle();
					}
					
					if(this.Role.Type == GameRole.TYPE_OWNER)
					{	
						this.mouseEnabled  = false;
						this.mouseChildren = false;
					}					
					
					// 深度判断数据
					this.excursionX = this.elementWidth / 2;						// 人物名宽度偏移	(this.width / 2)
					
					if(this.Role.Type == GameRole.TYPE_BANNER)
					{
						this.excursionY = this.height - 45;
					}
					else
					{  
						this.excursionY = this.height;									// 人物名高度偏移
					}
				}
				else
				{
					this.Enabled = true;
					this.Stop();
					this.SetAction(GameElementSkins.ACTION_STATIC,GameElementSkins.DIRECTION_DOWN);
				}
			}
			
			// 经验：坐标的对齐问题要考虑到一个人物中所有子元件的添加和删除时的Y轴坐标是否对应。在元件第二次初始化时是否Y轴坐标还是正常。
			this.SetPosition();
			if(this.skins!=null)
			{
				if((this.Role.Type == GameRole.TYPE_PLAYER || this.Role.Type == GameRole.TYPE_OWNER) && this.Role.StallId != 0)
				{
					
				}
				else if(this.isInitialize == false)
				{
					// 显示人物动画
					this.skins.ChangeEquip  = onChangeEquip;
					this.skins.ChooseTarger = ChooseTarger;
					this.skins.LoadSkin();
					
					this.isInitialize = true;
				}
			}
			
			super.Initialize();
		}
		
		public function ternalInitialize():void
		{
			super.Initialize();
		}
		
		/** 设置人物起启点 */
		protected function SetPosition():void
		{
			if(this.Role!=null && this.Role.isSkinTest == false)
			{
				var p:Point = MapTileModel.GetTilePointToStage(this.Role.TileX, this.Role.TileY);
				this.X      = p.x;
				this.Y      = p.y;
				
				// 判断半透明区
				this.Translucence();
			}
			else if(this.Role.isSkinTest)
			{
				this.X = this.Role.showSkinPoint.x;
				this.Y = this.Role.showSkinPoint.y;
			}
		}
		
		private function onBodyLoadComplete():void
		{
			// 删除小蓝人
			if(this.golem != null)
			{
				this.golemSprite.removeEventListener(MouseEvent.MOUSE_OVER, onMouseOver);
				this.golemSprite.removeEventListener(MouseEvent.MOUSE_OUT,  onMouseOut);
				this.removeChild(this.golemSprite);
				this.golem        = null;
				this.golemSprite  = null;
				this.mouseEnabled = false;
				if(this.Role.PersonSkinID == 5001)
				{
					this.personTitle.x  	 = 0;
					this.personTitle.y  	 = 85;
				}
				if(this.Role.PersonSkinID == 5001)
				{
					this.personName.x  		= 0;
					this.personName.y  		= 100;
				}
			}
			
			if(this.skins!=null)
			{
				// 影子坐标
				if(this.shadow == null)
				{
					if(this.Role.Type == GameRole.TYPE_ENEMY)
					{
						 if(this.skins.MaxBodyWidth > 180)
						 {
						 	this.shadow      = new Bitmap(CommonData.BigShadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("BigShadow"))); 
						 	this.shadow.width = this.skins.MaxBodyWidth / 3 * 2;
						 	this.shadow.height = this.shadow.width /2 ;
						 	this.shadow.alpha = 0.6;
						 }
						 else
						 {
						 	this.shadow      = new Bitmap(CommonData.Shadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Shadow"))); 					 	
						 }
					}
					else
					{
						this.shadow      = new Bitmap(CommonData.Shadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Shadow"))); 
					}
					this.shadow.name = "Shadow";
					this.shadow.x    = (this.elementWidth - this.shadow.width) / 2;
					this.shadow.y    = this.skins.MaxBodyHeight + this.textFieldHeight - 15 + this.OffsetHeight;
					if(this.Role.MountSkinName==null)
					{
						this.addChildAt(this.shadow, 0);
					}
				}
				else
				{			
					
					this.shadow.x = (this.elementWidth - this.shadow.width) / 2;
					this.shadow.y = this.skins.MaxBodyHeight + this.textFieldHeight - 15 + this.OffsetHeight;
				}
				
				if(!(this is GameElementPet))
				{ 
					// 上线死亡
			        if(this.Role.HP == 0)  
			        {
			        	this.skins.ChangeAction(GameElementSkins.ACTION_DEAD,true);
			        	if(this.Role.Type == GameRole.TYPE_OWNER)
			        	{	
			        		this.skins.InitActionDead(7);		
			        	}
			        	else if(this.Role.Type == GameRole.TYPE_PLAYER)
			        	{
			       			this.skins.InitActionDead(7);				// 玩家死亡为8帧
			       		}
			       		else if(this.Role.Type == GameRole.TYPE_ENEMY)
			       		{
			       			this.skins.InitActionDead(3);				// 敌人死亡为4帧
			       		}
			        }
			    }
			   	if(this.Role.HP > 0)
			   	{
					this.skins.ChangeAction(GameElementSkins.ACTION_STATIC,true);
				}
			    
				// 人物坐标
				if(this.Offset!=null)
				{
					this.skins.x = this.Offset.x - 95;
					this.skins.y = this.Offset.y + this.OffsetHeight;
				}
				this.addChildAt(this.skins,1);
				
				// 删除以有的任务提示
				if(this.missionPrompt!=null)
				{
					this.removeChild(this.missionPrompt);
					this.missionPrompt = null;
				}
								
        	    // 深度判断数据
				this.excursionX = this.elementWidth / 2;						// 人物名宽度偏移	(this.width / 2)
				SetExcursionY();
								
//				this.excursionY = this.height - 30;			                    // 人物高度不应该以人物类的高去计算，而是应该以皮肤的高度进行计算，因为人物类的高度可以放各种不同的信息，
				                                                                // 如果要省去皮肤的容器，也应该把人物的高度保存在1个值中
//				this.excursionY = this.height - 30;		

//				if(this.Role.MountSkinName!=null)
//				{
//					this.excursionY += this.MountHeight;			
//					this.y 		    -= this.MountHeight;
//				}

				this.SetPosition();
				this.SetMissionPrompt(this.Role.MissionState);
			}
					
			if(SetPlayer != null)
			{
				SetPlayer(this,this.Role.skinNameController);
				SetPlayer = null;
			}
			
			this.IsLoadSkins = true;
		}

        public function SetExcursionY():void
        {		
        	if(this.txftalk != null && this.contains(this.txftalk))
			{
				this.removeChild(this.txftalk);
				this.txftalk = null;
			}
			
			if(this.talkmc != null && this.contains(this.talkmc))
			{
				this.removeChild(this.talkmc);
				this.talkmc = null;
			}
        	
        	if(this.golem == null)
        	{		
				var NameCount:int = 1;
				
				if(this.personTitle != null && this.personTitle.visible)		
				{
					NameCount ++;			
				}
				
				if(this.personAppellation != null && IsShowAppellation)		
				{
					NameCount ++;			
				}
				
			    this.excursionY = this.height - 15 * NameCount;
        	}
        }
        


		protected virtual function onChangeEquip(equipType:String):void {}
		
		/** 决斗皮肤（旗帜） */
		public function PlayerBanner():void
		{
			this.playerBanner      = new Bitmap(CommonData.PlayerBanner(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("PlayerBanner")));
			this.playerBanner.name = "PlayerBanner";
			this.playerBanner.x    = (this.elementWidth - this.playerBanner.width) / 2;
			this.playerBanner.y    = 40;						        
			this.addChild(this.playerBanner);
		}

        /**称号处理**/
        public function ShowAppellation(appellation:Appellation):void
        {
        	//是否显示称号
        	if(IsShowAppellation)
        	{
        		if(this.personAppellation == null)
        		{
        			this.personAppellation = new TextField();
					var format1:TextFormat = new TextFormat();
					format1.align 		   = TextFormatAlign.CENTER;
					format1.color		   = appellation.Color;
					format1.size		   = 12;
					format1.font		   = "宋体";
					this.personAppellation.defaultTextFormat = format1;
				    this.personAppellation.cacheAsBitmap     = true;
					this.personAppellation.mouseEnabled      = false;
					this.personAppellation.selectable 	     = false;
					this.personAppellation.type			     = TextFieldType.DYNAMIC;
					this.personAppellation.text              = appellation.name;
					this.personAppellation.x  	             = 0;
					if(this.personTitle != null && this.personTitle.visible)
					  this.personAppellation.y  	         = -15;
					else
					   this.personAppellation.y  	         = 0;
					this.personAppellation.width             = this.elementWidth;
					this.personAppellation.height            = this.textFieldHeight;
					this.personAppellation.filters           = Font.Stroke(appellation.BorderColor);					
					this.addChild(this.personAppellation);
								
					if(Role.StallId != 0)
					{
						this.personAppellation.visible = false;
					}
        		}
        		else
        		{
        			this.personAppellation.filters           = Font.Stroke(appellation.BorderColor);
        			this.personAppellation.text              = appellation.name;
        			this.personAppellation.textColor         = appellation.Color;
        		}
        		
//        		if(this.missionPrompt!=null)
//        		{
//        			this.missionPrompt.y = -this.missionPrompt.height - 15;
//        		}
        	}
        	else
        	{
        		if(this.personAppellation!=null)
				{
					this.removeChild(this.personAppellation);
					this.personAppellation = null;
					SelectAppellationID    = 0;
				}
        	}
        	
        	SetExcursionY();
        }

		/** 摆摊皮肤（招财猫） */
		public function PlayerStall():void
		{
			// 玩家或自己才可以摆摊
			if((this.Role.Type == GameRole.TYPE_PLAYER || this.Role.Type == GameRole.TYPE_OWNER) && this.Role.StallId !=0)
			{
				if(this.playerStall==null && this.IsStall==false)
				{
					
					this.IsStall		  = true;
					this.playerStall = new Bitmap(CommonData.PlayerStall(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("PlayerStall")));
					this.playerStall.name = "PlayerStall";
					this.playerStall.x    = (this.elementWidth - this.playerStall.width) / 2;
					
					if(this.personAppellation != null)
						this.personAppellation.visible = false;
					
					//判断名字称号是否显示 如果显示怎增加偏移值
				    if(this.personName.visible == false)
				    {
				    	this.playerStall.y    = 0;
				    }
				    else
				    {
				    	this.playerStall.y    = 40 ;
				    }
				        
					this.addChild(this.playerStall);
					
                    if(this.skins != null && this.contains(this.skins))
                    {
						this.removeChild(this.skins);
                    }
					if(this.shadow != null && this.contains(this.shadow))	
					{
						this.removeChild(this.shadow);
					}
				}
			}
			else if(this.IsStall)
			{
				if(this.personAppellation != null)
					this.personAppellation.visible = true;
						
				this.IsStall = false;
				this.removeChild(this.playerStall);
				this.playerStall = null;
				try
				{
					this.addChildAt(this.shadow, 0);
					this.addChildAt(this.skins, 1);
				}
				catch(e:Error)
				{
					this.ShowSkin();
				}
			}
		}
		
		/** 显示加载皮肤 */
		private function ShowSkin():void
		{
			// 预显示人偶
			this.golem   = new Bitmap(CommonData.Golem(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Golem")));
			this.golem.x = (this.elementWidth - this.golem.width) / 2;
			this.golem.y = 40;
			this.golemSprite.addChild(this.golem);
			this.addChild(this.golemSprite);
			
			// 显示人物动画
			this.skins.ChangeEquip  = onChangeEquip;
			this.skins.ChooseTarger = ChooseTarger;
			this.skins.LoadSkin();
		}
		
		/** 是否为VIP  */
		public function IsVip():void
		{
			if(this.personName!=null)
			{
				this.SetVIP();
				
				// 解决在组队时 VIP 字样的位置错误
				if(this.Role.idTeam != 0 )
				{
					if(this.teamLeaderSign!=null)
					{
						this.teamLeaderSign.x = this.personName.getCharBoundaries(0).x - this.teamLeaderSign.width  - 4;
						this.teamLeaderSign.y = this.personName.getCharBoundaries(0).y + this.teamLeaderSign.height - 4;
					}
					if(this.teamMemberSign != null)
					{
						this.teamMemberSign.x = this.personName.getCharBoundaries(0).x - this.teamMemberSign.width  - 4;
						this.teamMemberSign.y = this.personName.getCharBoundaries(0).y + this.teamMemberSign.height + 9;
					}
				}
			}
		}
		
		/** 更新人物名颜色 */
		public function UpdatePersonName():void
		{
			this.personName.filters = Font.Stroke(this.Role.NameBorderColor);	
			this.SetName(this.Role.Name);
		}
		
		/** 是否选中效果  */
		public function IsSelect(isSelect:Boolean):void
		{
			if(this.shadow && this.Role.HP > 0)
			{
				try
				{
					this.removeChild(this.shadow);
				}	
				catch(e:Error)
				{
					
				}
				
				if(isSelect)
				{
					if(this.Role.Type == GameRole.TYPE_ENEMY)
					{
						 if(this.skins.MaxBodyWidth > 180)
						 {
						 	this.shadow      = new Bitmap(CommonData.BigSelect(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("BigSelect"))); 
						 	this.shadow.width = this.skins.MaxBodyWidth / 3 * 2;
						 	this.shadow.height = this.shadow.width /2 ;
						 	this.shadow.alpha = 0.6;
						 }
						 else
						 {
			       			this.shadow = new Bitmap(CommonData.SelectShadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Select")));
						 }
					}
					else
					{
						 this.shadow = new Bitmap(CommonData.SelectShadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Select")));
					}
					
					this.shadow.name = "Select";
					this.shadow.x = (this.elementWidth - this.shadow.width) / 2;
					this.shadow.y = this.skins.MaxBodyHeight + this.textFieldHeight - 15 + this.OffsetHeight;
					if(this.Role.MountSkinName!=null)
					{
						this.shadow.y += this.MountHeight;
					}
					this.addChildAt(this.shadow, 0);
				}
				else if(this.Role.MountSkinName==null)
				{
					if(this.Role.Type == GameRole.TYPE_ENEMY)
					{
						 if(this.skins.MaxBodyWidth > 200)
						 {
						 	this.shadow      = new Bitmap(CommonData.BigShadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("BigShadow"))); 
						 	this.shadow.width = this.skins.MaxBodyWidth / 3 * 2;
						 	this.shadow.height = this.shadow.width /2 ;
						 	this.shadow.alpha = 0.6;
						 }
						 else
						 {
						 	this.shadow      = new Bitmap(CommonData.Shadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Shadow"))); 
						 }
					}
					else
					{
						this.shadow      = new Bitmap(CommonData.Shadow(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("Shadow"))); 
					}
					
					this.shadow.name = "Shadow";
					this.shadow.x = (this.elementWidth - this.shadow.width) / 2;
					this.shadow.y = this.skins.MaxBodyHeight + this.textFieldHeight - 15 + this.OffsetHeight;
					this.addChildAt(this.shadow, 0);
				}
			}
		}
		
		public override function Update(gameTime:GameTime):void
		{
			if(this.skins!=null)
			{
				if(this.playerEffect != null)
				{
					this.playerEffect.Update(gameTime);
				}
				
				if(this.isActionPlaying)
				{
					this.skins.Update(gameTime);		// 人物动做动画
				}
                 
                if(this.handler != null && this.Role.HP > 0 
                 && this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
                {               
                	if(this.Role.Type == GameRole.TYPE_OWNER)
                	{
	                	if(this.golem != null)  	
	                	{                		
	                		 this.handler.Clear();
	                	}
	                	else
	                	{
	                		if(this.IsAddAttack)
	                		 	this.handler.Run();
	                		else
	                		{
	                		   this.handler.Clear();
	                		   
	                		   if(this.StopAttack != null)
	                		   		StopAttack();
	                		}
	                	}
                	}
                	else
                	{
                	    if(this.golem != null)  	
	                	{
	                		 this.handler.Clear();
	                	}
	                	else
	                	{
	                		 this.handler.Run();
	                	}
                	}
                }  
                
                if(this.handler == null && QuickPlay)
                {
                	QuickPlay = false;
                }

				// 宠物击杀怪回到人物边（后期此事件放到人物每移动一A*格触发）
				if(PetDistance != null)
				{
					PetDistance();
				}
				
				if(this.smoothMove.IsMoving)
				{
					this.smoothMove.Update(gameTime);	// 人物移动动画
				}
				
				if(this.Automatism != null && this.IsAutomatism)
				{
					Automatism();
				}
				
				if( this.Attack != null && IsAttack)
				{
					Attack();
				}
								
				// 人物受击技能特效动画
				if(skillAnimation.length > 0)
				{
					for(var n:int = 0 ;n <= skillAnimation.length - 1;n++)
					{
						if(skillAnimation[n].IsPlayComplete)
						{ 
							skillAnimation[n] = null;
							skillAnimation.splice(n,1);
							break;
						}
						else
						{
							skillAnimation[n].Update(gameTime);
						}
					}
				}
				
				if(skillEffectList.length > 0)
				{
					for(n = 0;n <= skillEffectList.length - 1;n++)
					{
						if(skillEffectList[n].smoothMove != null)
						{
							skillEffectList[n].smoothMove.Update(gameTime);
							 if(skillEffectList[n].SkillFly())
							 {
							    skillEffectList[n] = null;
								skillEffectList.splice(n,1);
								break;
							 }	
						}
						else
						{
							skillEffectList[n] = null;
							skillEffectList.splice(n,1);
							break;
						}
					}
				}		
			}
			super.Update(gameTime);
		}

		/** 人物移动 */
		public virtual function Move(targetPoint:Point,distance:int = 0):void {}
		
		public virtual function MoveTile(targetPoint:Point,distance:int = 0,IsStagePoint:Boolean = false):void {}

		/** 设置人物方向  */
		public function SetDirection(direction:int):void
		{
			this.Role.Direction = direction;
			this.skins.ChangeAction(this.Role.ActionState);
		}
		
		/** 任务提示 */
		public function SetMissionPrompt(missionState:int):void
		{
			if(this.Role.Type == GameRole.TYPE_NPC || 
			   this.Role.Type == GameRole.TYPE_OWNER)
			{
				if(this.missionPrompt!=null)
				{
					this.removeChild(this.missionPrompt);
					this.missionPrompt = null;
				}
				
				if(missionState == 0)
				{
					this.Role.MissionState = missionState;
				}
				else
				{
					if(missionState <=3)				// 任务提示
					{
						this.Role.MissionState = missionState;
						if(missionState == 3)
						{
							this.missionPrompt = new Bitmap(CommonData.Taskfinish(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("task_finish")));
						}
						if(missionState == 2)
						{
							this.missionPrompt = new Bitmap(CommonData.TaskUnAccpet(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("task_unAccpet")));
						}
						if(missionState == 1)
						{
							this.missionPrompt = new Bitmap(CommonData.Taskunfinish(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("task_unfinish")));
						}
					}
					else if(missionState == 4)			// 生活技能提示
					{
						this.missionPrompt = this.Games.Content.Load(Engine.UILibrary).GetClassByMovieClip("LifeSeekState");
					}
					
					this.missionPrompt.x = (this.elementWidth - this.missionPrompt.width) / 2;
					
//					//判断是否有称号
//					if(this.IsShowAppellation)
					this.missionPrompt.y = -this.missionPrompt.height - 15;
//					else
//						this.missionPrompt.y = -this.missionPrompt.height;
					
					this.addChild(this.missionPrompt);
				}
			}
		}
		
		/** 设置队长  */
		public function SetTeamLeader(isTeamLeader:Boolean):void
		{
			if(isTeamLeader && this.personName!=null)
			{
				if(teamMemberSign && this.contains(teamMemberSign)) 
				{
					this.removeChild(teamMemberSign);
					teamMemberSign = null;
				}
				if(teamLeaderSign && this.contains(teamLeaderSign)) return;
				this.teamLeaderSign   = new Bitmap(CommonData.TeamLeaderSign(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("TeamLeaderSign")));
				this.teamLeaderSign.x = this.personName.getCharBoundaries(0).x - this.teamLeaderSign.width  - 4;
				this.teamLeaderSign.y = this.personName.getCharBoundaries(0).y + this.teamLeaderSign.height - 4;
				this.addChild(teamLeaderSign);
			}
			else
			{
				if(teamLeaderSign && this.contains(teamLeaderSign)) 
				{
					this.removeChild(this.teamLeaderSign);
					teamLeaderSign = null;
				}
			}
		}
		
		/** 设置队员  */
		public function SetTeam(isTeam:Boolean):void
		{
			if(isTeam)
			{
				if(teamLeaderSign && this.contains(teamLeaderSign)) 
				{
					this.removeChild(teamLeaderSign);
					teamLeaderSign = null;
				}
				if(teamMemberSign && this.contains(teamMemberSign)) return;
				if(this.personName!=null)
				{
					this.teamMemberSign   = new Bitmap(CommonData.TeamMemberSign(this.Games.Content.Load(Engine.UILibrary).GetClassByBitmap("TeamMemberSign")));
					this.teamMemberSign.x = this.personName.getCharBoundaries(0).x - this.teamMemberSign.width  - 4;
					this.teamMemberSign.y = this.personName.getCharBoundaries(0).y + this.teamMemberSign.height + 9;
					this.addChild(teamMemberSign);
				}
			}
			else
			{
				if(teamMemberSign && this.contains(teamMemberSign)) 
				{
					this.removeChild(this.teamMemberSign);
					teamMemberSign = null;
				}
			}
		}
		
		/** 修改名称 */
		public function SetName(name:String):void
		{
			if(this.Role.Name!=null)
			{
				this.Role.Name = name;
				this.SetVIP();
			}
		}
		
		private function SetVIP():void
		{
			this.personName.htmlText = "<font color='" + this.Role.NameColor + "'>" + (this.Role.Name==null ? "" : this.Role.Name) + "</font>";
			if(this.Role.VIP==1)
			{
				this.Role.VIPColor 		  = "#0098FF";
				this.personName.htmlText += "<font color='#0098FF'>［VIP］</font>";
			}
			else if(this.Role.VIP==2)
			{
				this.Role.VIPColor 		  = "#7a3fe9";
				this.personName.htmlText += "<font color='#7a3fe9'>［VIP］</font>";
			}
			else if(this.Role.VIP==3)
			{
				this.Role.VIPColor 		  = "#FF6532";
				this.personName.htmlText += "<font color='#FF6532'>［VIP］</font>";
			}
			else if(this.Role.VIP==4)
			{
				this.Role.VIPColor 		  = "#00FF00";
				this.personName.htmlText += "<font color='#00FF00'>［VIP］</font>";
			}
			else
			{
				this.Role.VIPColor = null;
			}
		}
		
		/** 修改心情、称号 */
		public function SetTitle(title:String):void
		{
			if(this.Role.Feel != null)
			{
				this.Role.Feel = title;
			}
			if(this.personTitle != null && this.personTitle.text !=null && title !=null)
			{
				this.personTitle.text = title;
			}			
		}
		
		/** 显示心情、称号 */
		public function ShowTitle():void
		{
			if(this.Role.Feel!=null)
			{
				this.personTitle.text    = this.Role.Feel;
				this.personTitle.visible = true;
			}
			if(IsShowAppellation && personAppellation != null)
			{
				personAppellation.y = -15; 
			}
			
			SetExcursionY();
		}
		
		/** 隐藏心情、称号 */
		public function HideTitle():void
		{
			this.personTitle.visible = false;
			if(IsShowAppellation && personAppellation != null)
			{
				personAppellation.y = 0; 
			}
		}
		
		/** 显示名称 */
		public function ShowName():void
		{
			this.SetName(this.Role.Name);
			this.personName.visible = true;
		}
		
		/** 隐藏名称 */
		public function HideName():void
		{
			this.personName.visible = false;
		}
		
		/** 人物移动深度排序-人物移动每一步事件 */
		protected virtual function onMoveStep():void 
		{
			if(MoveStep!=null)MoveStep(this);
		}
		
		/** 人物半透明 */
		protected function Translucence():void
		{
			var p:Point = MapTileModel.GetTileStageToPoint(this.GameX, this.GameY);
			if(this.gameScene.Map!=null && this.gameScene.Map.IsBlock(0,0,p.x,p.y) == MapTileModel.PATH_TRANSLUCENCE)
			{ 
				this.alpha = 0.5;
			}
			else
			{
				this.alpha = 1;
			}
		}
		
		/** 人移移动完成事件  */
		protected virtual function onMoveComplete():void 
		{
			this.Translucence();							// 判断人物半透明
			this.gameScene.MiddleLayer.DepthSort(this);		// 判断人物层次
		}
		
		protected var previousDepth:Number = 0;
		protected var currentDepth:Number  = 0;
		/** 每准备移动到下一节点事件 */
		protected virtual function onMoveNode(direction:int):Boolean 
		{
			this.Translucence();						// 判断人物半透明
			this.gameScene.MiddleLayer.DepthSort(this);		// 判断人物层次
			
			return true;
		}
		
		private function onMouseOutTarger():void
		{
			if(MouseOutTarger!=null) MouseOutTarger(this);
		}
		
		private function onMouseOverTarger():void
		{
			if(MouseOverTarger!=null) MouseOverTarger(this);
		}
		
		private function onActionPlayComplete(e:AnimationEventArgs):void
		{
			// 只有移动动画播放完后不停止（移动、休闲状态循环播放）
			if(this.Role.ActionState != GameElementSkins.ACTION_RUN && this.Role.ActionState != GameElementSkins.ACTION_STATIC)
			{
				this.isActionPlaying = false;
			}
						
			// 攻击完后动做变为休闲状态
			if(this.Role.ActionState == GameElementSkins.ACTION_NEAR_ATTACK	)				// 后摇状态 在帧播放完成时候 也要切回静止
			{
				this.SetAction(GameElementSkins.ACTION_STATIC);
				if(MustMove != null)
				 	MustMove(this);
			}
			
			// 玩家死后就没有鼠标事件了
			if(this.Role.ActionState == GameElementSkins.ACTION_DEAD)
			{
				if(this.Role.Type != GameRole.TYPE_PLAYER)
				{
					this.mouseChildren = false;
				}
				else
				{
					this.mouseChildren = true;
				}
				
				// 怪物死了不显示名称 
				if(this.Role.Type == GameRole.TYPE_ENEMY)
				{
					this.HideName();
					this.HideTitle();
				}
			}
				
			if(ActionPlayComplete != null)
			{
			 	ActionPlayComplete(this);
			}
		}
		
		private function onActionPlayFrame(e:AnimationEventArgs):void
		{
			if(ActionPlayFrame!=null)
			{	
				e.Sender = this;
				ActionPlayFrame(e);
			}
		}
		
        /** 判断显示人物方向动做 */
        public function SetAction(actionType:String, direction:int = 0):void
        {
        	if(direction!=0)
        	{
				this.Role.Direction = direction;
			}
			this.Role.ActionState = actionType;
		    this.skins.ChangeAction(this.Role.ActionState);
		    this.isActionPlaying = true;
		    
		    // 是否显示影子，死亡后不显示影子
		    if(this.shadow != null)
		    {
			    if(actionType != GameElementSkins.ACTION_DEAD)
			    {
			    	this.shadow.visible = true;		// 人物复活后有影子
			    }
			    else
			    {
					this.shadow.visible = false;	// 人物死亡后没有影子
			    }
		    }
        }
        
		public virtual function Stop():void { }
		
		public function get GetToplenght():int
		{
			var top:int = 0;
			
			if(this.personTitle.visible)
			{
				top += 15; 
			}
			
			if(this.IsShowAppellation)
			{
				top += 15;
			}
			return top;
		}
		
		/** 鼠标离开目标 */
		public var MouseOutTarger  : Function;
		/** 鼠标移动到目标上 */
		public var MouseOverTarger : Function;
		/** 选为被目标 */
		public var ChooseTarger    : Function;
		/** 换装备事件 */
		public var OnChangeSkin     : Function;
		/** 动画帧事件  */
		public var ActionPlayFrame : Function;
		/**动画帧完成事件*/
		public var ActionPlayComplete:Function;
		/**更新玩家技能效果信息*/
		public var UpdateSkillEffect:Function;
		/**更新玩家大地图路线信息*/
		public var MapPathUpdate:Function;
		/**更新挂机状态**/
		public var Automatism:Function;
		
		/** 更新场景地图路径信息*/
		public var SMapPathUpdate:Function;
		/** 宠物攻击距离远回来判断**/
		public var PetDistance:Function;
		
		/** 每准备移动到下一节点事件 */
		public var MoveNode:Function;
		/** 每移动一步事件 */
		public var MoveStep:Function;
		/** 移动完成事件 */
		public var MoveComplete:Function;
		/**校对点**/
		public var CheckPoint:Function;
		
		/** 人物是否在移动 */
		public function get IsMoving():Boolean
		{
			return this.smoothMove.IsMoving;
		}
		
		/** 游戏中人物X坐标 */
		public function get GameX():Number
		{
			return this.X + this.excursionX;
		}
		
		/** 游戏中人物Y坐标 */
		public function get GameY():Number
		{
			return this.Y + this.excursionY;
		}
		
		/**受击点X坐标**/
		public function get HitX():Number
		{
			return  GameX;
		}
		
		/**受击点Y坐标**/
		public function get HitY():Number
		{
			if(this.Role.MountSkinName != null)
			{
				return GameY - Math.abs(this.excursionY - this.MountHeight) * 2/5 - this.MountHeight;
			}
			else
			{
				return GameY - Math.abs(this.excursionY) * 2/5;
			}
		}
		
		
		public function get PlayerHitX():Number
		{
			return  elementWidth / 2;
		}
		
		public function get PlayerHitY():Number
		{
			return this.skins.MaxBodyHeight / 2 + this.textFieldHeight + 5;
		}
		
		public override function set X(value:Number):void
		{
			this.x = value - this.excursionX;
		}
		
		public override function set Y(value:Number):void
		{
			this.y = value - this.excursionY;
		}
	}
}