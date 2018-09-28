package GameUI.Modules.Unity.UnityUtils
{
	/** 下拉框组件*/
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.View.BaseUI.ListComponent;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class ComboBoxUtil extends Sprite
	{
		private var _list:Array = [];
//		private var _x:int;
//		private var _y:int;
		private var _width:int;
		private var _height:int;
		private var listView:ListComponent;
		private var iscomboBoxShow:Boolean = false;
		private var boxType:int = 0;
		private var boxItem:MovieClip;
		public function ComboBoxUtil(list:Array, x:int = 0 , y:int = 0 , width:int = 83 , height:int = 16)
		{
			_list = list;
			this.x = x;
			this.y = y;
//			_x = x;
//			_y = y;
			_width  = width;
			_height = height;
			showTxt(boxType); 
			addLis();
		}
		public function gcAll():void
		{
			boxItem.btnCombox.removeEventListener(MouseEvent.CLICK , showBoxHandler);
			GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.CLICK , closecomboBoxHandler);
			clearComboBox();
			listView = null;
		}
		private function addLis():void
		{
			boxItem.btnCombox.addEventListener(MouseEvent.CLICK , showBoxHandler);
			GameCommonData.GameInstance.GameUI.addEventListener(MouseEvent.CLICK , closecomboBoxHandler);	
		}
			
		private function showTxt(type:int):void
		{
			this.boxType = type; 
			if(boxItem == null) boxItem = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ComboBox") as MovieClip;
			boxItem.txtType.text = _list[type];
			boxItem.x = 1;
//			boxItem.y = _y;
			this.addChild(boxItem);
		}
		/** 点击下拉按钮 */
		private function showBoxHandler(e:MouseEvent):void
		{
			if(iscomboBoxShow == true)
			{
				clearComboBox();
				return;
			}
			clearComboBox();
			iscomboBoxShow = true;
			listView = new ListComponent(false);
//			listView.x = 
//			listView.y = 
			this.addChild(listView);    
			showComboBox();
			e.stopPropagation();
		}
		/** 显示下拉框*/
		private function showComboBox():void
		{
			for(var i:int = 0; i<_list.length; i++)
			{
				if(boxType == i) continue;
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("OrdainItem");
				item.name = i.toString();
				item.width  = _width;
				item.height = _height;
				item.txtList.text = _list[i];
				item.addEventListener(MouseEvent.CLICK, selectItem);
				listView.SetChild(item);
			}
			listView.width = 83;
//			listView.x = x;
			listView.y = boxItem.height; 
			listView.upDataPos();
		}
		/** 点击下拉框某一项 */
		private function selectItem(e:MouseEvent):void
		{
			clearComboBox();
			boxItem.txtType.text = UnityConstData.contributeArray[int(e.currentTarget.name)];
			boxType = e.currentTarget.name;
		}
		/** 清除下拉列表 */
		private function clearComboBox():void
		{
			if(listView != null && this.contains(listView))
			{
				this.removeChild(listView);
				iscomboBoxShow = false;
			}
		}
		/** 点击GameUI层让下拉列表消失 */
		private function closecomboBoxHandler(e:MouseEvent):void
		{
			clearComboBox();
		}
		
	}
}
		
		
