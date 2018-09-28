package OopsEngine.Scene.StrategyElement
{
	import OopsFramework.GameTime;
	import OopsFramework.IDisposable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	
	/** 人物皮肤抽象类  */
	public class GameElementSkins extends Sprite implements IDisposable
	{
		public var MouseOutTarger     : Function;
		public var MouseOverTarger    : Function;
		/** 选为被攻击目标 */
		public var ChooseTarger       : Function;
		/** 动画播放完成 */
		public var ActionPlayComplete : Function;
		/** 动画每一帧事件 */
		public var ActionPlayFrame    : Function;
		/** 身体资源加载完成 */
		public var BodyLoadComplete   : Function;
		/** 换装备事件 */
		public var ChangeEquip		  : Function;
		/** 装备完成事件 */
		public var ChangeSkins        : Function;
		/** 皮肤加载事件 */
		public var SkinLoadComplete   : Function;
		
		public var MaxBodyWidth:int;
		public var MaxBodyHeight:int;

		protected var gep:GameElementAnimal;						// 人物元件
		protected var currentDirection:int     = DIRECTION_DOWN;	// 人物上一次方向
		public  var currentActionType:String   = ACTION_STATIC;		// 人物上一次动做类型		
		protected var isEffect:Boolean 		   = true;				// 是否有选中特效
		private var isCanSelect:Boolean 	   = false;				// 是否能选中人物皮肤状态
		public var frameRate:Timer;								// 动画播放速度控制
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.MouseOutTarger 	= null;
			this.MouseOverTarger    = null;
			this.ChooseTarger 		= null;
			this.ActionPlayComplete = null;
			this.ActionPlayFrame    = null;
			this.BodyLoadComplete   = null;
			this.ChangeEquip 	    = null;
			this.ChangeSkins 	    = null;
			this.SkinLoadComplete   = null;
			
//			this.gep	   = null;
//			this.frameRate = null;
		}
		/** IDisposable End */
		
		/** 是否有选中特效 */
		public function IsEffect(value:Boolean):void
		{
			this.isEffect     = value;
			this.mouseEnabled = value;
		}
		
		/** 每秒多少帧（默认24帧）*/
		public function get FrameRate():uint
		{
			return this.frameRate.Frequency;
		}
		public function set FrameRate(value:uint):void
		{
			this.frameRate.Frequency = value; 
		}
		
		public function GameElementSkins(gep:GameElementAnimal)
		{
			this.gep = gep;
			this.addEventListener(MouseEvent.MOUSE_OVER , onMouseOver);
			this.addEventListener(MouseEvent.MOUSE_OUT  , onMouseOut);
			
			this.frameRate = new Timer();
			
			this.mouseChildren = false;
		} 
		
		public virtual function LoadComplete():void { }
		
		/** 修改人体皮肤  */
		public virtual function LoadSkin():void { }
		
		/** 设置变异variation  */
		public virtual function Setvariation(state:int):void { }
		
		/** 移除体皮肤  */
		public virtual function RemovePersonSkin(skinType:String):void { }
		
		/** 
		 * 修改动做
		 * @param actionType  动做类型
		 * @param isForce  	  是否强制改变动做
		 */
		public function ChangeAction(actionType:String, isForce:Boolean = false, frameIndex:int = 0):void
        {     	
		    // 此处定义人物方向和美术图布局有关
		    if(isForce || this.currentActionType != actionType || this.currentDirection != this.gep.Role.Direction)
		    {
		    	this.gep.Role.ActionState = actionType
		    	this.currentActionType    = actionType;
		    	this.currentDirection     = this.gep.Role.Direction;
		    	this.SetActionAndDirection(frameIndex);
	 	    }
        }
        
        public function InitActionDead(frameIndex:int):void
        {
       		this.gep.Role.ActionState = GameElementSkins.ACTION_DEAD;
		    this.currentActionType    = GameElementSkins.ACTION_DEAD;
		    this.currentDirection     = this.gep.Role.Direction;
		    this.ChangeAction(this.currentActionType, true, frameIndex);
        }
        
        /** 设置新动做和方向  */
        protected virtual function SetActionAndDirection(frameIndex:int = 0):void { }
        
		public function Update(gameTime:GameTime):void 
		{ 
			if(this.frameRate.IsNextTime(gameTime))
			{
				this.ActionPlaying(gameTime);
			}
		}
		
		protected virtual function ActionPlaying(gameTime:GameTime):void {}
        
		/** Event Function Start */
		protected function onMouseOver(e:MouseEvent):void
		{
			if(this.isEffect)
			{
				this.addEventListener(MouseEvent.MOUSE_MOVE,onMouseMove);
			}
		}
		
		protected function onMouseOut(e:MouseEvent):void
		{
			if(this.isEffect)
			{
				this.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
				this.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			 	this.filters     = null;
			 	this.isCanSelect = false;
			 	if(MouseOutTarger!=null) MouseOutTarger();
			}
		}
		
		/** 鼠标指在人上的效果  */
		protected virtual function onMouseMove(e:MouseEvent):void { }
		
		/** 选中高亮  */
		protected function AddHighlight():void
		{
			if(isCanSelect==false)
			{
				var glow:GlowFilter = new GlowFilter(0xFF0000, 1, 10, 10, 1, 1, false, false);	// 0xFFFFFF 可修改高光颜色
				var filters:Array   = new Array(glow);
				this.filters        = filters;
				this.isCanSelect    = true;
				this.addEventListener(MouseEvent.MOUSE_DOWN,onMouseDown);
				if(MouseOverTarger!=null) MouseOverTarger();
			}		
		}
		
		/** 删除高亮  */
		protected function DeleteHighlight():void
		{
			this.filters     = null;
			this.isCanSelect = false;
			this.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			if(MouseOutTarger!=null) MouseOutTarger();
		}
		
		/** 用于选中目标记下目标编号  */
		private function onMouseDown(e:MouseEvent):void
		{
			if(ChooseTarger!=null) ChooseTarger(this.gep);
		}
		/** Event Function End */
		
		/* ---------- 枚举 ---------- */
		/** 人物皮肤  */
		public static const EQUIP_PERSON : String = "person";
		/** 武器皮肤  */
		public static const EQUIP_WEAOIB : String = "weaoib";
		/** 武器光影 */
		public static const EQUIP_WEAOIBEFFECT : String = "weaoibeffect";
		/** 坐骑皮肤  */
		public static const EQUIP_MOUNT  : String = "mount";
		/** 人物身体光影  */
		public static const EQUIP_PERSONEFFECT  : String = "personeffect";
		
		/** 站立动做 */
		public static const ACTION_STATIC 		 : String = "static";
		/** 移动动做 */
		public static const ACTION_RUN   		 : String = "run";
		/** 魔法&近战攻击动作 */
		public static const ACTION_NEAR_ATTACK   : String = "action_near_attack";
		/** 魔法&远程攻击动做 */
		public static const ACTION_FAR_ATTACK    : String = "action_far_attack";
		/** 受攻击动做 */
		public static const ACTION_ACCEPT_ATTACK : String = "accept_attack";
		/** 死亡动做 */
		public static const ACTION_DEAD 		 : String = "dead";
//		/**后摇 */
//		public static const ACTION_AFTER         : String = "action_after";
		
		/** 方向上 */
		public static const DIRECTION_UP         : int = 8;
		/** 方向下 */
		public static const DIRECTION_DOWN       : int = 2;
		/** 方向左 */
		public static const DIRECTION_LEFT       : int = 4;
		/** 方向右 */
		public static const DIRECTION_RIGHT      : int = 6;
		/** 方向左上 */
		public static const DIRECTION_LEFT_UP    : int = 7;
		/** 方向左下 */
		public static const DIRECTION_LEFT_DOWN  : int = 1;
		/** 方向右上 */
		public static const DIRECTION_RIGHT_UP   : int = 9;
		/** 方向右下 */
		public static const DIRECTION_RIGHT_DOWN : int = 3;
	}
}
