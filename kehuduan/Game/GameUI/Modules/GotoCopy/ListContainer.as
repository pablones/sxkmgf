package GameUI.Modules.GotoCopy
{
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	/**
	 * 
	 * @author lh
	 * 
	 */
	public class ListContainer extends Sprite
	{
		private var _data:Array;//副本条件数组
		private var scroll:UIScrollPane;
		private var sprite:Sprite;
		private var _w:Number;
		private var _h:Number;
		private var _pData:Array;//玩家列表
		public function ListContainer(data:Array,playerData:Array,width:Number = 10,height:Number = 10)
		{
			_data = data;
			_pData = playerData;
			_w = width;
			_h = height;
			list();
		} 
		
		private function list():void
		{
			if(!sprite)
			{
				sprite = new Sprite();
			}
			var tag:int = 0;
			for each(var obj:* in _pData)
			{
				var item:GotoCopyCompent = new GotoCopyCompent(_data,obj);
				item.x = 0;
				item.y = tag*item.height;
				item.mouseChildren = false;
				item.mouseEnabled = false;
				sprite.addChild(item);
				tag ++;
			}
			scroll = new UIScrollPane(this.sprite);
			scroll.width = this._w;
			scroll.height = this._h;
			scroll.refresh();
			this.addChild(scroll);
		}
		
		public function update(copyData:Array,pData:Array):void
		{
			if(_data)	//条件列表
			{
				_data = copyData;
			}
			if(_pData)	//玩家列表
			{
				_pData = pData;
			}
			if(sprite)
			{
				var num2:int = this.sprite.numChildren - 1;
				while(num2 >= 0)
				{
					var dis:DisplayObject = sprite.getChildAt(0);
					if(dis is GotoCopyCompent)
					{
						(dis as GotoCopyCompent).dipose();
					}
					sprite.removeChild(dis);
					dis = null;
					num2 --;
				}
				if(sprite.parent)
				{
					sprite.parent.removeChild(sprite);
				}
			}
			if(scroll)
			{
				this.removeChild(scroll);
				scroll = null;
			}
			this.sprite = null;
			this.list();
		}
		
	}
}