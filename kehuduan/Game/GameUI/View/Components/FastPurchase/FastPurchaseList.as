package GameUI.View.Components.FastPurchase
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	
	public class FastPurchaseList extends MovieClip
	{
		private var bg:Sprite;
		public function FastPurchaseList( arr:Array, h:int ) 							//arr是由type组成的数组，h为背景高度
		{
			bg = draw_rect( h );
			addChild( bg );
			creatFP( arr );
		}
		
		private function creatFP( arr:Array ):void
		{
			for( var i:int = 0; i < arr.length; i++)
			{
				var fp:FastPurchase = new FastPurchase( arr[i] );
				fp.y = i * 117;
				bg.addChild( fp );
			}
		}
		
		private function draw_rect( h:int ):Sprite
		{
			var sp:Sprite = new Sprite();
			sp.graphics.beginFill( 0x000000 );
			sp.graphics.drawRect( 0, 0, 73, h);
			sp.graphics.endFill();
			return sp;
		}

	}
}