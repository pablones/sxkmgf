package GameUI.Modules.Build.view
{
	import GameUI.View.Components.UISprite;
	
	import flash.display.MovieClip;


	/**
	 *  修炼渲染器
	 * @author felix
	 * 
	 */	
	public class BuildCell extends UISprite
	{
		protected var view:MovieClip;
		
		protected var _des:String;
		protected var _buildName:String;
		protected var _needDes:String;
		protected var _isBuilding:Boolean;
		protected var _img:String;
		protected var _needTime:String;
		protected var _toolTipId:String;
		protected var _buildId:int;
		
		public function BuildCell()
		{
			super();
			this.createChildren();
		}
		
		
		protected function createChildren():void{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("buildCell");
			this.addChild(this.view);
			this.doLayout()
		}
		
		protected function repaint():void{
			
		}
		
		/**
		 * 布局 
		 * 
		 */		
		protected function doLayout():void{
			this.width=this.view.width;
			this.height=this.view.height;
		}
		
		/**
		 * 更新所有 
		 * 
		 */		
		public function update():void{
			this.doLayout();
		}
		
		/**
		 * 设置描述信息 
		 * @param value
		 * 
		 */		
		public function set des(value:String):void{
			
		}
		
		/**
		 * 设置修炼名称 
		 * @param value
		 * 
		 */		
		public function set buildName(value:String):void{
			
		}
		
		/**
		 * 设置修炼的需求的描述信息 
		 * @param value
		 * 
		 */		
		public function set needDes(value:String):void{
			
		}
		
		/**
		 * 设置修炼进行/未进行 
		 * @param value  true:进行中。。。。
		 * 
		 */		
		public function set isBuilding(value:Boolean):void{
			
		}
		
		/**
		 * 设置修炼时的图片 
		 * @param value
		 * 
		 */		
		public function set img(value:String):void{
			
		}
		
		/**
		 * 设置修炼需求时间 
		 * @param value
		 * 
		 */		
		public function set needTime(value:String):void{
			
		}
		
		/**
		 * 设置ToolTip的Id号 
		 * @param value
		 * 
		 */		
		public function set toolTipId(value:String):void{
			
		}
		
		/**
		 * 设置修炼选项ID号 
		 * @param id
		 * 
		 */		
		public function set buildId(id:uint):void{
			this._buildId=id;
		}
                                                         
	}
}