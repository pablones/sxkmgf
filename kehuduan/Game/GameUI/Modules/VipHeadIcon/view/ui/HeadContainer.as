package GameUI.Modules.VipHeadIcon.view.ui
{
	import GameUI.Modules.Login.SetFrame.SetFrame;
	import GameUI.Modules.VipHeadIcon.view.mediator.VipHeadIconMediator;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	/**九个方格容器*/
	public class HeadContainer extends Sprite
	{
		private var _gridArr:Array;
		private var _mianMediator:VipHeadIconMediator;
		private var _gridClass:Class;

		public function HeadContainer(mainMediator:VipHeadIconMediator,gridClass:Class)
		{
			_mianMediator = mainMediator;
			_gridClass = gridClass;
			_gridArr = [];
		}
		
		public function initGrid(sex:int,pageIndex:int):void   
		{
			var iconNameNum:int = 0;
			for(var i:int = 0; i < 9; i ++)
			{
				var item:GridItem = new GridItem();
				item.contextMc = new _gridClass();
				item.x = (i%3)*61;
				item.y = int(i/3)*61;
				if(0 == sex)
				{
					iconNameNum = 60 + i+(pageIndex-1)*9;
				}
				else if(1 == sex)
				{
					iconNameNum = 20 + i+(pageIndex-1)*9;
				}
				item.loadSource(iconNameNum);
				item.setData(iconNameNum);
				item.mouseChildren = false;
				_gridArr.push(item);
				this.addChild(item);
				item.addEventListener(MouseEvent.CLICK,mouseEventHandler);
			}
		}
		private function mouseEventHandler(me:MouseEvent):void
		{
			_mianMediator.dataNum = int((me.target as GridItem).getData());
			var frameName:String = "YellowFrame";
			SetFrame.RemoveFrame(this);                                   //先清空
			SetFrame.UseFrame(me.target as DisplayObject,"YellowFrame",2,2,55,55);                            //再添加黄框
		}
		
		/* private function addFrame(targetDisplay:DisplayObject,frameName:String):void
		{
			var frame:MovieClip;
			if(GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary))
			{
				frame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(frameName);
			}
			else{
				frame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip(frameName);
			}
			frame.name = frameName;
			targetDisplay.parent.addChildAt(frame,targetDisplay.parent.numChildren - 1);
			frame.x = targetDisplay.x + 2;
			frame.y = targetDisplay.y + 2;
			frame.width = 55;
			frame.height = 55;
		}
		private function removeFrame(parentDisplay:DisplayObjectContainer,frameName:String):void
		{
			if(parentDisplay.getChildByName(frameName))
			{
				parentDisplay.removeChild(parentDisplay.getChildByName(frameName));
			}
			 var num:int = parentDisplay.numChildren - 1;
			for(; num >= 0; num--)
			{
				if(parentDisplay.getChildAt(num).name == frameName)
				{
					parentDisplay.removeChildAt(num);
					return;
				}
			} 
		} */
		
		public function gc():void
		{
			for each(var item:GridItem in _gridArr)
			{
				item.removeEventListener(MouseEvent.CLICK,mouseEventHandler);
				this.removeChild(item);
				item.gc();
				item = null;
			}
			_gridArr = null;
			_mianMediator = null;
			_gridClass = null;
		}
	}
}