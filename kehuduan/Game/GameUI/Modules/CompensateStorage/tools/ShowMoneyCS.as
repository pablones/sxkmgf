package GameUI.Modules.CompensateStorage.tools
{
	import GameUI.ConstData.UIConstData;
	import GameUI.View.items.ShopCostItem;

	import flash.display.Bitmap;
	import flash.display.BlendMode;
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.text.TextField;

	public class ShowMoneyCS extends Sprite
	{
		public function ShowMoneyCS(type:String)
		{

		}

		public static function ShowIcon(parant:DisplayObjectContainer, tf:TextField, clearAll:Boolean=false):void
		{
			//\ss
			var regExp:RegExp=/(\\[a-z]{2})/g;
			var chatArr:Array=tf.text.split(regExp);
			if (!chatArr || chatArr.length == 0)
			{
				return;
			}
			var index:int=0;
			var pos:int=0;
			if (clearAll)
			{
				var count:int=parant.numChildren - 1;
				while (count)
				{
					if (parant.getChildAt(count) is Bitmap || parant.getChildAt(count) is Shape || parant.getChildAt(count) is MovieClip)
					{
						parant.removeChild(parant.getChildAt(count));
					}
					count--;
				}
			}
			while (index < chatArr.length)
			{
				if (regExp.test(chatArr[index]))
				{
//					if (parant.getChildByName(chatArr[index].slice(1, 3)))
//					{
//						if (parant.contains(parant.getChildByName(chatArr[index].slice(1, 3))))
//						{
//							parant.removeChild(parant.getChildByName(chatArr[index].slice(1, 3)));
//						}
//					}
					if (!testIsMoney(chatArr[index].slice(1, 3)))
						return;
					var bitmap:Bitmap=new Bitmap();
					bitmap.bitmapData=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData(chatArr[index].slice(1, 3));
					bitmap.x=int(tf.getCharBoundaries(pos).x + tf.x + 1);
					bitmap.y=int(tf.getCharBoundaries(pos).y + tf.y - 1);
					bitmap.name=chatArr[index].slice(1, 3);
					parant.blendMode=BlendMode.LAYER;
					setBmpMask(bitmap, parant);
					parant.addChild(bitmap);
					pos=pos + chatArr[index].length;
				}
				else
				{
					pos=pos + chatArr[index].length;
				}
				index++;
			}
		}

		private static function testIsMoney(str:String):Boolean
		{
			var arr:Array=["se", "ss", "sc", "ce", "cs", "cc", "ab", "zb", "dq", "aa"];
			for (var i:int=0; i < arr.length; i++)
			{
				if (str == arr[i])
				{
					return true;
				}
			}
			return false;
		}

		private static function setBmpMask(bitmap:Bitmap, parant:DisplayObjectContainer):void
		{
			var s:Shape=new Shape();
			s.graphics.beginFill(0xffffff, 1);
			s.graphics.drawRect(bitmap.x - 1, bitmap.y, bitmap.width + 2.6, bitmap.height + 1);
			s.graphics.endFill();
			s.blendMode=BlendMode.ERASE;
			parant.addChild(s);
		}

		/** 物品兑换商店消耗的物品 图标显示  */
		public static function showGoodCostItem(parant:DisplayObjectContainer, tf:TextField, clearAll:Boolean=true):void
		{
			if (clearAll)
			{ //移除现有的
				var count:int=parant.numChildren - 1;
				while (count)
				{
					if (parant.getChildAt(count) is Bitmap || parant.getChildAt(count) is Shape || parant.getChildAt(count) is MovieClip)
					{
						parant.removeChild(parant.getChildAt(count));
					}
					count--;
				}
			}
			var dataArr:Array=tf.text.split("_"); // 5_300001   5 个 300001_d图标  	物品数据的img后面加_d表示小图标名字
			if (!dataArr || dataArr.length == 0)
				return;

			var itemType:String=dataArr[1]; // + "_d";
			var imgName:String=UIConstData.getItem(uint(itemType)).img;
			var item:ShopCostItem=new ShopCostItem(imgName); //获得图标
			//-------
			var parentMc:MovieClip=new MovieClip();
			parentMc.addChild(item);
			parentMc.name="npcGoodToGood_" + itemType;
			//---------
//			item.name = "npcGoodToGood_" + itemType;
			var pos:int=dataArr[0].length;
			tf.text=dataArr[0] + "\aa";

			parentMc.x=int(tf.getCharBoundaries(pos).x + tf.x);
			parentMc.y=int(tf.getCharBoundaries(pos).y + tf.y - 1);
			parant.blendMode=BlendMode.LAYER;
			setItemMask(parentMc, parant);
			parant.addChild(parentMc);
		}

		private static function setItemMask(item:DisplayObject, parant:DisplayObjectContainer):void
		{
			var s:Shape=new Shape();
			s.graphics.beginFill(0xffffff, 1);
			s.graphics.drawRect(item.x - 1, item.y, item.width + 2.6, item.height + 1);
			s.graphics.endFill();
			s.blendMode=BlendMode.ERASE;
			parant.addChild(s);
		}

	}
}
