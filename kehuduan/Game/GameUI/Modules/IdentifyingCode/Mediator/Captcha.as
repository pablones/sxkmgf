package GameUI.Modules.IdentifyingCode.Mediator
{
	import GameUI.Modules.IdentifyingCode.Data.IdentifyingCodeData;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	import flash.geom.Matrix;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;

	public class Captcha extends Sprite
	{

		private var _length:uint;
		private var _size:uint;
		private var randCode:String="";
		private var tft:TextFormat=new TextFormat();
		private var sp_cont:Sprite=new Sprite();
		private var sp_noise:Sprite=new Sprite();

		public function Captcha(length:uint=5, size:uint=18):void
		{
			_length=length;
			_size=size;
			tft.font="Arial";
			tft.size=_size;
			tft.bold=true;
			sp_cont.x=_size / 2;
			addChild(sp_cont);
		}
		
		public function setCaptcha(id:int):void
		{
			removeAllChild(sp_cont);
			randCaptchaWithCharacter(getCode(id));
			drawNoise();
			drawBorder();
		}

		private function getCode(id:int):String
		{
//			var id:int = randInt(0, YanZhengMaData.YZM.length);
			var number:String = IdentifyingCodeData.YZM[id];
			var str:String = "";
			var num:int = number.length/IdentifyingCodeData.YZMLEN;
			var i:int = 0;
			for(i =0; i< num; i++)
			{
				str += IdentifyingCodeData.str.charAt(int(number.slice(i*IdentifyingCodeData.YZMLEN,(i+1)*IdentifyingCodeData.YZMLEN))%IdentifyingCodeData.str.length);
			}
			return str;
		}

		private function randCaptchaWithCharacter(randCode:String):void
		{
			for (var i:uint=0; i < randCode.length; i++)
			{
				var tf:TextField=new TextField();
				tf.text=randCode.charAt(i);
				tf.selectable=false;
				tf.textColor=IdentifyingCodeData.colors[randInt(0, IdentifyingCodeData.colors.length)];
				tf.autoSize=TextFieldAutoSize.LEFT;
				tft.size=_size + randInt(-3, 3);
				tf.setTextFormat(tft);

				var bitmapData:BitmapData=new BitmapData(_size + 2, _size + 8, true, 0x00000000);
				bitmapData.draw(tf, new Matrix());

				var bitmap:Bitmap=new Bitmap(bitmapData);
				var angle:Number=randNumber(-0.3, 0.3);
				bitmap.transform.matrix=new Matrix(Math.cos(angle), Math.sin(angle), -Math.sin(angle), Math.cos(angle), 0, 0);
				bitmap.x=_size * i;

				sp_cont.addChild(bitmap);
			}
			randCode=randCode.toUpperCase();
		}

		private function drawNoise():void
		{
			sp_noise.graphics.clear();
			for (var i:int=0; i < 25; i++)
			{
				var ptx:int=randInt(1, width);
				var pty:int=randInt(1, height);
				var ex:int=ptx + randInt(-width, width);
				var ey:int=pty + randInt(-height, height);
				ex=(ex <= 1) ? 1 : ((ex >= width) ? width : ex);
				ey=(ey <= 1) ? 1 : ((ey >= height - 1) ? height - 1 : ey);
				sp_noise.graphics.lineStyle(1, randInt(0, 0xFFFFFF), 0.25);
				sp_noise.graphics.moveTo(ptx, pty);
				sp_noise.graphics.lineTo(ex, ey);
			}
			sp_noise.width=_size * (_length + 1);
			sp_noise.height=27;
			addChild(sp_noise);
		}

		private function drawBorder():void
		{
			graphics.clear();
			if(IdentifyingCodeData.YZMBackColor)
			{
				graphics.beginFill(IdentifyingCodeData.YZMBackColor[0],IdentifyingCodeData.YZMBackColor[0]);
			}
			else
			{
				graphics.beginFill(0xffffff,0.5);
			}
			graphics.drawRect(0, 0, _size * (_length + 1), _size + 9);
			graphics.beginFill(0);
		}

		private function removeAllChild(container:DisplayObjectContainer):void
		{
			while (container.numChildren > 0)
			{
				container.removeChildAt(0);
			}
		}

		private function randInt(min:int, max:int):int
		{
			return Math.random() * (max - min) + min;
		}

		private function randNumber(min:Number, max:Number):Number
		{
			return Math.random() * (max - min) + min;
		}
	}
}
