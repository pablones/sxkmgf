package GameUI.MouseCursor
{
	import GameUI.Modules.PlayerIcon.UI.CursorIcon;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.ui.Mouse;
	
	public class SysCursor
	{
		private static var _instance:SysCursor;
		protected var systemCursorLayer:Sprite;
		protected var type:int=-1;
		protected var cursor:Sprite;
		protected var cursors:Array;
		
		public var _isLock:Boolean=false;
		
		/** 默认模式*/
		public static const DEFALUT_CURSOR:uint=0;
		/**
		 * 攻击模式 
		 */		
		public static const ATTACK_CURSOR:uint=1;
		/**
		 *  不可移动模式
		 */		
		public static const NOMOVE_CURSOR:uint=2;
		/** 
		 * 拾东西
		 * */
		public static const PICK_CURSOR:uint=3;
		/**  
		 * 对话
		 */ 
		public static const STALK_CURSOR:uint=4;
		
		/** 选好友 */
		public static const FRIEND_CURSOR:uint=5;
		
		/** 修理 */
		public static const REPAIR_CURSOR:uint=6;
		
		public function SysCursor()
		{
			systemCursorLayer = GameCommonData.GameInstance.CursorLayer;
			Mouse.show();
			this.cursor=new Sprite();
			this.cursor.mouseEnabled=false;
			cursors=[];
			this.LoadIcon();
			systemCursorLayer.addChild(this.cursor);
			this.cursor.visible=false;
		}
		
		protected function LoadIcon():void{
			var icon1:CursorIcon=new CursorIcon("Default");
			cursors.push(icon1);
			var icon2:CursorIcon=new CursorIcon("Attack");
			cursors.push(icon2);
			var icon3:CursorIcon=new CursorIcon("NoMove");
			cursors.push(icon3);
			var icon4:CursorIcon=new CursorIcon("Pick");
			cursors.push(icon4);
			var icon5:CursorIcon=new CursorIcon("Talk");
			cursors.push(icon5);
			var icon6:CursorIcon=new CursorIcon("Friend");
			cursors.push(icon6);
			var icon7:CursorIcon=new CursorIcon("Repair");
			cursors.push(icon7);
		}
		public static function GetInstance():SysCursor{
			if(_instance==null){
				_instance=new SysCursor();
			}
			return _instance;
		}
		
		protected function onMouseMoverHandler(e:MouseEvent):void{
			this.cursor.x=systemCursorLayer.mouseX;
			this.cursor.y=systemCursorLayer.mouseY;
		}
		
		/**
		 * 设置鼠标模式
		 * @param type
		 * 
		 */		
		public function setMouseType(type:uint=0):void{
			if(this._isLock)return;
			if(type==0){
				this.showSystemCursor();
				return;
			}
			this.cursor.visible=true;
			GameCommonData.GameInstance.MainStage.addEventListener(MouseEvent.MOUSE_MOVE,onMouseMoverHandler);
			Mouse.hide();
			if(this.type==type)return;
			this.type=type;
			while(this.cursor.numChildren>0){
				this.cursor.removeChildAt(0);
			}
			var temp:DisplayObject=this.cursors[type];
			if(type==2 || type==4){
				temp.x=-Math.floor(temp.width/2);
				temp.y=-Math.floor(temp.height/2);
			}
			this.cursor.addChild(temp);
		}
		
		/**
		 *  还原
		 * 
		 */		
		
		public function revert():void{
			if(this._isLock)return;
			this.showSystemCursor();
		}
		
		public function showSystemCursor():void{
			GameCommonData.GameInstance.MainStage.removeEventListener(MouseEvent.MOUSE_MOVE,onMouseMoverHandler);
			Mouse.show();
			this.cursor.visible=false;
			
		}
		
		public function set isLock(value:Boolean):void{
			this._isLock=value;
			if(value==false){
				this.revert();	
			}		
		}
		
		public function get isLock():Boolean{
			return this._isLock;
		}
	}
}