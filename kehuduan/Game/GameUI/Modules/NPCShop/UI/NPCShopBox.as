package GameUI.Modules.NPCShop.UI
{
	import GameUI.Modules.NPCShop.Data.NPCShopConstData;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	public class NPCShopBox extends Sprite
	{
		private static const CHOICE_ARR:Array = [
													{name:GameCommonData.wordDic[ "mod_npcs_ui_npcsb_init_1" ], data:2},    //"碎银"
													{name:GameCommonData.wordDic[ "mod_npcs_ui_npcsb_init_2" ], data:1}      //"银两"
													];
		
		private var _itemSprite:Sprite = null;
		private var _itemBase:MovieClip = null;
		private var _index:uint = 0;
		
		public function NPCShopBox(index:uint=0)
		{
			this._index = index;
			init();
		}
		
		/** 初始化 */
		private function init():void
		{
			_itemBase = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ComboxBase");
			_itemBase.txtPayType.text = CHOICE_ARR[_index].name;
			_itemBase.btnShowList.addEventListener(MouseEvent.MOUSE_DOWN, showListHandler);
			this.addChild(_itemBase);
			NPCShopConstData.payWay = CHOICE_ARR[_index].data;
			_itemSprite = new Sprite();
			_itemSprite.x = 0;
			_itemSprite.y = _itemBase.height;
			for(var i:int = 0; i < CHOICE_ARR.length; i++) {
				var item:MovieClip =  GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BoxItem");
				item.txtName.mouseEnabled = false; 
				item.txtName.text = CHOICE_ARR[i].name;
				item.name = "npcShopBox_"+i;
				item.btnSelect.addEventListener(MouseEvent.MOUSE_DOWN, clickHandler);
				_itemSprite.addChild(item);
				item.x = 0;
				item.y = i * item.height; 
			}
		}
		
		private function showListHandler(e:MouseEvent):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(_itemSprite)) {
				GameCommonData.GameInstance.GameUI.removeChild(_itemSprite);
			} else {
				GameCommonData.GameInstance.GameUI.addChild(_itemSprite);
				var p:Point = localToGlobal(new Point(_itemBase.x, _itemBase.y));
				_itemSprite.x = p.x;
				_itemSprite.y = p.y + _itemBase.height;
			}
			e.stopPropagation();
		}
		
		private function clickHandler(e:MouseEvent):void
		{
			var index:int = String(e.target.parent.name).split("_")[1];
			if(CHOICE_ARR[index]) {
				NPCShopConstData.payWay = CHOICE_ARR[index].data;
				_itemBase.txtPayType.text = CHOICE_ARR[index].name;
				this.dispatchEvent(new NPCShopBoxEvent(NPCShopBoxEvent.COMBOXEVENT_CLICKITEM));
			}
			if(GameCommonData.GameInstance.GameUI.contains(_itemSprite)) GameCommonData.GameInstance.GameUI.removeChild(_itemSprite);
		}
		
		public function gc():void
		{
			if(_itemSprite)  {
				var index:int = _itemSprite.numChildren-1;		
				while(index >= 0) {
					_itemSprite.getChildAt(index).removeEventListener(MouseEvent.MOUSE_DOWN, clickHandler);
					_itemSprite.getChildAt(index).removeEventListener(MouseEvent.MOUSE_DOWN, clickHandler);
					_itemSprite.removeChildAt(index);
					index--;
				}
				if(GameCommonData.GameInstance.GameUI.contains(_itemSprite)) GameCommonData.GameInstance.GameUI.removeChild(_itemSprite);
			}
			if(_itemBase) {
				_itemBase.btnShowList.removeEventListener(MouseEvent.MOUSE_DOWN, showListHandler);
				if(this.contains(_itemSprite)) this.removeChild(_itemBase);
			}
			_itemSprite = null;
			_itemBase = null;
		}
		
		public function hide():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(_itemSprite)) GameCommonData.GameInstance.GameUI.removeChild(_itemSprite);
		}
		
		
	}
}