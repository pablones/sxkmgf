package GameUI.Modules.Designation.view.ui
{
	import flash.display.MovieClip;
	import flash.text.TextField;

	/***小条目组件*/
	public class TextContent extends TextField
	{
		private var _dataObj:Object;
		public function TextContent(obj:Object)
		{
			super();
			_dataObj = obj;
			init();
			
		}
		private function init():void
		{
			this.text = _dataObj.name;
			this.height = 18;
			this.width = 145;
			if(1 == _dataObj.isSelect) 
			{
				this.textColor = 0x00FF33;
			}
			else 
			{
			    this.textColor = 0x777777;
			}		 	
			this.selectable = false;
		}

		public function get dataObj():Object
		{
			return this._dataObj;
		}


	}
}