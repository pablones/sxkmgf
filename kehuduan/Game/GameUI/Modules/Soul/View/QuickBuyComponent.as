package GameUI.Modules.Soul.View
{
	import GameUI.View.Components.FastPurchase.FastPurchase;
	
	import flash.display.MovieClip;
	import flash.display.Shape;

	public class QuickBuyComponent extends MovieClip
	{
		private var _content:MovieClip;
		private var _type:int;
		private var fast:FastPurchase;
		private var shape:Shape;
		private var _tag:String;
		public function QuickBuyComponent(content:MovieClip,type:int = 0,tag:String = "")
		{
			this._content = content;
			_type = type;
			_tag = tag;
		 	init();
		}
		
		private function init():void
		{
			this.addChild(_content);
			fast = new FastPurchase(_type.toString());
			fast.x = _content.width - 12;
			if(_tag == "GorwUpPercentPanel")
			{
				fast.x = _content.width - 16;
			}
			else if(_tag == "RepeatExtendProPanel")
			{
				fast.x = _content.width - 30;
			}
			else if(_tag == "RepeatSkillPanel")
			{
				fast.x = 196;
			}
			else if(_tag == "RepeatStylePanel")
			{
				fast.x = 196;
			}
			else if(_tag == "UseExtendGroovePanel")
			{
				fast.x = _content.width - 16;
			}
			
			fast.y = 1;
			this.addChild(fast);
			
			shape = new Shape();
            shape.graphics.lineStyle(0, 0, 0);
            shape.graphics.beginFill(0, 0);
            shape.graphics.drawRect(0, 0,this._content.width+73 , this._content.height);
            shape.graphics.endFill();
            this.addChildAt(shape,0)
		}
		
		public function setFast(type:int):void
		{
			this.gc();
			fast = new FastPurchase(type.toString());
			fast.x = _content.width - 12;
			fast.y = 1;
			this.addChild(fast);
		}
		
		private function gc():void
		{
			if(fast)
			{
				fast.parent.removeChild(fast);
				fast = null;
			}
		}
	}
}