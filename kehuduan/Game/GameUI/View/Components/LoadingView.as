package GameUI.View.Components
{
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;

	public class LoadingView extends Sprite
	{
		private static var _instance:LoadingView;
		private var _loadingMc:MovieClip;
		private var _parentContainer:DisplayObjectContainer=null;
		
		private var _w:Number=1000;
		private var _h:Number=580;
		
		public function LoadingView()
		{
			super();
			this._loadingMc=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("LoadCircle");
			this._loadingMc.gotoAndStop(1);
			var shape:Shape=new Shape();
			shape.graphics.beginFill(0x0,0);
			shape.graphics.drawRect(0,0,_w,_h);
			shape.graphics.endFill();
			this.addChild(shape);
		}
		
		public static function getInstance():LoadingView{
			if(_instance==null)_instance=new LoadingView();
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
				if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth ) this._loadingMc.x=500 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				if( GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight ) this._loadingMc.y=290 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
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