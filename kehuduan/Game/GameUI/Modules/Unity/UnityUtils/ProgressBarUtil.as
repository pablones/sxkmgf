package GameUI.Modules.Unity.UnityUtils
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class ProgressBarUtil extends Sprite
	{
		private var _curNum:Number;
		private var _tolNum:Number;
		private var _length:Number;
		private var mcBar:MovieClip;
		private var _parent:MovieClip;
		private var txtBar:TextField;
//		private var progressBarSingle:Boolean = false;
		public function ProgressBarUtil(parent:MovieClip , curNum:Number , tolNum:Number , length:Number , x:Number = 0 , y:Number = 0)
		{
//			if(progressBarSingle == true)return;
//			progressBarSingle = true;
			/////////////////////////////////防止益出  by 陈明
			if ( curNum > tolNum )
			{
				curNum = tolNum;	
			}			
			////////////////////////////////////
			mcBar  = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ProgressBar") as MovieClip;
			mcBar.mouseEnabled = false;
			_curNum = curNum;
			_tolNum = tolNum;
			_length = length;
			_parent = parent;
			this.x = x;
			this.y = y;
			this.addChild(mcBar);
			mcBar.x = 0;
			txtInit();
			_parent.addChild(this);
		}
		
		public function showBar(curNum:int , tolNum:Number):void
		{
			_curNum = curNum;
			_tolNum = tolNum;
			mcBar.width = Number(_curNum / _tolNum * _length);
			if(_curNum < 0) txtBar.text = "0/" + _tolNum;
			else txtBar.text = _curNum + "/" + _tolNum;
			if(_curNum >= _tolNum) mcBar.width = _length;
			else if(_curNum <= 0) mcBar.width  = 0;
		}
		private function txtInit():void
		{
			txtBar = new TextField();
			this.addChild(txtBar);
			txtBar.mouseEnabled = false;
			txtBar.textColor = 0xFFFFFF;
			txtBar.defaultTextFormat = new TextFormat(null,11);

			txtBar.multiline = false;
			txtBar.text = _curNum + "/" + _tolNum;
			txtBar.autoSize = TextFieldAutoSize.LEFT;
			txtBar.x = (_length - txtBar.textWidth)/2;
			txtBar.y = -3;
		}

	}
}