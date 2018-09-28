package OopsEngine.GameComponents
{
	import OopsFramework.DrawableGameComponent;
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	
	import flash.display.Shape;
	import flash.system.System;
	import flash.text.TextField;
	import flash.text.TextFormat;

	/** 计算刷帧频率组件 */
	public class FpsComponent extends DrawableGameComponent
	{
		private var frameElapsedTime:Number  = 0;
		private var currentFrameTime:Number  = 0;
		private var prevFrameTime:Number     = 0;
		private var secondTime:Number	     = 1000;
		private var secondElapsedTime:Number = 0;
		private var frames:Number 			 = 0;
		private var fps:String   			 = "...";
		
		private var txtFps:TextField;
		private var barFps:Shape;
		
		public function FpsComponent(game:Game)
		{
			super(game);
			this.mouseEnabled  = false;
			this.mouseChildren = false;
			this.Enabled	   = false;
		}
		
		public override function Initialize():void
		{
			barFps   = new Shape();
			barFps.x = 400;
			barFps.y = 5;
			barFps.graphics.beginFill(0xff0000, 0.5);
			barFps.graphics.lineStyle();
			barFps.graphics.drawRect(0, 0, 1, 15);
			barFps.graphics.endFill();
			
			addChild(barFps);

			txtFps 		  = new TextField();
			txtFps.x 	  = 400;
			txtFps.y      = 5;
			txtFps.width  = 200;
			txtFps.height = 20;
			txtFps.alpha  = 1;
			txtFps.defaultTextFormat = new TextFormat("Arial", 9, 0xFFFFFF);
			txtFps.mouseEnabled      = false;
			addChild(txtFps);
			
			super.Initialize();
		}
		
		public override function Update(gameTime:GameTime):void
		{
			prevFrameTime    = currentFrameTime;
			currentFrameTime = gameTime.TotalGameTime;
			frameElapsedTime = currentFrameTime - prevFrameTime;
		
			if(secondElapsedTime >= secondTime) 
			{
				fps   			   = frames.toString();
				frames			   = 0;
				secondElapsedTime -= secondTime;
				txtFps.text = "FPS：" + fps + " EMS：" + CalculateMemory(System.totalMemory) + " v2010.10.26 - 10:17";
			}
			else
			{
				frames++;
				secondElapsedTime += frameElapsedTime;
			}
			barFps.scaleX = frameElapsedTime;
			
			super.Update(gameTime);
		}
		
		private function CalculateMemory(memory:uint) : String
		{
			var result:String;
			if (memory < 1024)
			{
				result = String(memory) + "b";
			}
			else if (memory < 10240)
			{
				result = Number(memory / 1024).toFixed(2) + "kb";
			}
			else if (memory < 102400)
			{
				result = Number(memory / 1024).toFixed(1) + "kb";
			}
			else if (memory < 1048576)
			{
				result = Math.round(memory / 1024) + "kb";
			}
			else if (memory < 10485760)
			{
				result = Number(memory / 1048576).toFixed(2) + "mb";
			}
			else if (memory < 104857600)
			{
				result = Number(memory / 1048576).toFixed(1) + "mb";
			}
			else
			{
				result = Math.round(memory / 1048576) + "mb";
			}
			return result;
		}
	}
}