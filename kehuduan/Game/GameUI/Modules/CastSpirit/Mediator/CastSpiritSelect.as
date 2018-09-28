package GameUI.Modules.CastSpirit.Mediator
{
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.Components.UIScrollPane;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	public class CastSpiritSelect
	{
		private var _parent:MovieClip;
		private var container:ListComponent;
		private var scrollPanel:UIScrollPane;
		private var model:Array = [
									{name:"力量卷轴", type:430901}, 
									{name:"灵力卷轴", type:431001}, 
									{name:"体力卷轴", type:431101}, 
									{name:"定力卷轴", type:431201}, 
									{name:"身法卷轴", type:431301}, 
									{name:"冰属性卷轴", type:432001}, 
									{name:"火属性卷轴", type:432101}, 
									{name:"玄属性卷轴", type:432201}, 
									{name:"毒属性卷轴", type:432301}
									];
		public var updateFun:Function;
		
		public function CastSpiritSelect( parent:MovieClip )
		{
			_parent = parent;
		}
		
		public function showView():void
		{
			container = new ListComponent();
			container.name = "QuickSelectList";
			for( var i:int = 0; i<model.length; i++ )
			{
				var quickOperator:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("QuickOperator");
				quickOperator.txtOpName.text = model[i].name;
				quickOperator.txtOpName.mouseEnabled = false;
				quickOperator.txtOpName.filters = Font.Stroke(0x000000);
				quickOperator.name = i.toString();
				quickOperator.addEventListener(MouseEvent.CLICK, onSelect);
				container.SetChild(quickOperator);
			}
			container.upDataPos();	
			scrollPanel = new UIScrollPane(container);
			scrollPanel.x = 226;
			scrollPanel.y = 120;
			scrollPanel.width = 84;
			scrollPanel.height = 90;
			scrollPanel.refresh();
			_parent.addChild(scrollPanel);
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_DOWN, removeList);
		}
		
		private function onSelect( e:MouseEvent ):void
		{
			if( updateFun != null )
			{
				updateFun(model[uint(e.currentTarget.name)]);
			}
			removeThis();
		}
		
		private function removeList(event:MouseEvent):void
		{
			GameCommonData.GameInstance.removeEventListener(MouseEvent.MOUSE_DOWN, removeList);
			if(event.target.name == "selectBtn") return;
			removeThis();
		}
		
		private function removeThis():void
		{
			if(scrollPanel)
			{
				var des:*;
				while ( container.numChildren>0 )
				{
					des = container.removeChildAt( 0 );
					des.removeEventListener(MouseEvent.CLICK, onSelect);
					des = null;
				}
				if(_parent.contains(scrollPanel))
				{
					_parent.removeChild(scrollPanel);
					container = null;
					scrollPanel = null
				}
			}
		}
	}
}