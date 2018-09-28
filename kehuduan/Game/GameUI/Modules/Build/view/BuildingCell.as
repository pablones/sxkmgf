package GameUI.Modules.Build.view
{
	import GameUI.View.Components.UISprite;
	
	import flash.display.MovieClip;

	public class BuildingCell extends UISprite
	{
		protected var view:MovieClip;
		protected var _des:String;
		protected var _needTime:String;
		protected var _buildId:uint;

		public function BuildingCell()
		{
			super();
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("buildingCell");
			this.addChild(this.view);
			this.doLayout();
		}
		
		protected function repaint():void{
			
		}
		
		protected function doLayout():void{
			this.width=170;
			this.height=47;
			
		}
		
		protected function update():void{
			
		}
		
		public function set des(value:String):void{
			
		}
		
		public function set needTime(value:String):void{
			
		}
		
		public function set buildId(id:uint):void{
			
		}
				
	}
}