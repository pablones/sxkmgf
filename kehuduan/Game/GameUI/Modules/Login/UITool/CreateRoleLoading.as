package GameUI.Modules.Login.UITool
{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;

	public class CreateRoleLoading extends Sprite
	{
		private static var _instance:CreateRoleLoading;
		private var _loadingMc:MovieClip;
		private var _parentContainer:DisplayObjectContainer=null;
		
		private var _w:Number=1000;
		private var _h:Number=580;
		
		public function CreateRoleLoading()
		{
			super();
			this._loadingMc=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip("LoadCircle");
			this._loadingMc.gotoAndStop(1);
			var shape:Shape=new Shape();
			shape.graphics.beginFill(0x0,0);
			shape.graphics.drawRect(0,0,_w,_h);
			shape.graphics.endFill();
			this.addChild(shape);
		}
		
		public static function getInstance():CreateRoleLoading{
			if(_instance==null)_instance=new CreateRoleLoading();
			return _instance;
		}
		
		/**
		 *  
		 * @param parent  ：以此父容器居中显示加载图标
		 * 
		 */		
		public function showLoading(p:DisplayObjectContainer=null):void{
			if(this.stage!=null)return;
			this._loadingMc.gotoAndPlay(1);
			this._parentContainer=p;
			if(this._parentContainer==null){
				this.addChild(this._loadingMc);
				this._loadingMc.x=500;
				this._loadingMc.y=290;
			}else{
				this._parentContainer.addChild(this._loadingMc);
				this._loadingMc.x=Math.floor(this.parent.width/2);
				this._loadingMc.y=Math.floor(this.parent.height/2);
			}
			GameCommonData.GameInstance.GameUI.addChild(this);
		}
		/**
		 * 移出正在Loading的显示图标 
		 * 
		 */		
		public function removeLoading():void{
			this._loadingMc.stop();
			if(this._parentContainer!=null && _parentContainer.contains(this._loadingMc)){
				_parentContainer.removeChild(this._loadingMc);
			}else{
				if(this.contains(this._loadingMc)){
					this.removeChild(this._loadingMc);
				}
			}
			if(GameCommonData.GameInstance.GameUI.contains(this)){
				GameCommonData.GameInstance.GameUI.removeChild(this);
			}
		}
		
		
		
	}
}