package GameUI.Modules.Designation.view.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;

	/**条目组件*/
	public class DesignationItem extends Sprite
	{
		public var content:MovieClip;
		private var _dataObj:Object;
		public function DesignationItem(dataObj:Object)
		{
			this._dataObj = dataObj;
			init();
		}
		public function init():void
		{
			this.content=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("itemGroup");
			(content.txt_name as TextField).mouseEnabled=false;
			content.height = 17;
			this.content.txt_name.text = _dataObj.name;
			this.addChild(content);
		}
		public function getExplain():String
		{
			return _dataObj.explain;
		}
		public function getNum():int
		{
			return _dataObj.num;
		}
		public function getDataArr():Array
		{
			return _dataObj.data as Array;
		}
		public function gc():void
		{
			this.removeChild(content);
			content = null;
			_dataObj = null;
			
		}
	}
}