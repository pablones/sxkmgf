package GameUI.Modules.MusicPlayer.View
{
	import GameUI.Modules.MusicPlayer.Command.MusicPlayerCommandList;
	import GameUI.View.UIKit.components.InteractiveDataList;
	import GameUI.View.UIKit.components.ItemRendererBase;
	import GameUI.View.UIKit.events.DataEvent;
	
	import flash.events.MouseEvent;

	public class PlaylistUI extends InteractiveDataList
	{
		public function PlaylistUI()
		{
			this.itemRenderer = MusicPlayerPlaylistItemRenderer;
		}
		
		protected var _highlightedIndex:int = -1;
		
		public function get highlightedIndex():int
		{
			return _highlightedIndex;
		}
		
		public function set highlightedIndex(value:int):void
		{
			if (value == _highlightedIndex) return;
			
			if (_highlightedIndex > -1 && _highlightedIndex < this.numChildren)
			{
				(this.getChildAt(_highlightedIndex) as MusicPlayerPlaylistItemRenderer).highlighted = false;
			}
			
			_highlightedIndex = value;
			
			if (_highlightedIndex > -1 && _highlightedIndex < this.numChildren)
			{
				(this.getChildAt(_highlightedIndex) as MusicPlayerPlaylistItemRenderer).highlighted = true;
			} 
		}
		
		// ----------------------------------------
		//
		//		数据更新动作
		//
		// ----------------------------------------
		
		override protected function dataAddHandler(event:DataEvent):void
		{
			super.dataAddHandler(event);
			
			var ir:MusicPlayerPlaylistItemRenderer = this.getChildAt(event.index) as MusicPlayerPlaylistItemRenderer;
			
			ir.addEventListener(MouseEvent.DOUBLE_CLICK, itemRenderer_doubleClickHandler);
			
			if (_highlightedIndex >= event.index)
			{
				_highlightedIndex ++;
			}
		}
		
		override protected function dataRemoveHandler(event:DataEvent):void
		{
			var ir:MusicPlayerPlaylistItemRenderer = this.getChildAt(event.index) as MusicPlayerPlaylistItemRenderer;
			
			ir.removeEventListener(MouseEvent.DOUBLE_CLICK, itemRenderer_doubleClickHandler);
			
			if (event.index == _highlightedIndex)
			{
				highlightedIndex = -1;
			}
			else if (event.index < _highlightedIndex)
			{
				highlightedIndex --;
			}
			
			super.dataRemoveHandler(event);
		}
		
		override protected function dataClearHandler(event:DataEvent):void
		{
			var n:int = this.numChildren;
			
			for (var i:int = 0; i < n; i ++)
			{
				var ir:MusicPlayerPlaylistItemRenderer = this.getChildAt(i) as MusicPlayerPlaylistItemRenderer;
				
				ir.removeEventListener(MouseEvent.DOUBLE_CLICK, itemRenderer_doubleClickHandler);
			}
			
			highlightedIndex = -1;
			
			super.dataClearHandler(event);
		}
		
		override protected function dataRefreshHandler(event:DataEvent):void
		{
			var n:int = this.numChildren;
			var dn:int = _data.length;
			var i:int;
			var needLayout:Boolean = true;
			var ir:MusicPlayerPlaylistItemRenderer;
			
			if (n > dn)
			{
				for (i = dn; i < n; i++)
				{
					ir = this.getChildAt(event.index) as MusicPlayerPlaylistItemRenderer;
			
					ir.removeEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
					ir.removeEventListener(MouseEvent.DOUBLE_CLICK, itemRenderer_doubleClickHandler);
					
					this.$removeChild(ir);
				}
			}
			else if (dn > n)
			{
				for (i = n; i < dn; i++)
				{
					ir = new _itemRenderer();
					
					ir.addEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
					ir.addEventListener(MouseEvent.DOUBLE_CLICK, itemRenderer_doubleClickHandler);
					
					this.$addChild(ir);
				}
				needLayout = true;
			}
			
			for (i = 0; i < this.numChildren; i ++)
			{
				(this.getChildAt(i) as ItemRendererBase).data = _data.getItemAt(i);
				(this.getChildAt(i) as ItemRendererBase).index = i;
			}
			
			if (needLayout)
			{
				invalidateLayout();
			}
			
			// TODO(zhao): 捕捉选择项目，重定位
			setSelectedIndex( -1);
		}
		
		protected function itemRenderer_doubleClickHandler(event:MouseEvent):void
		{
//			highlightedIndex = (event.currentTarget as MusicPlayerPlaylistItemRenderer).index;
			GameCommonData.UIFacadeIntance.sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, event.currentTarget.index);
		}
	}
}