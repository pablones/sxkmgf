package GameUI.Modules.CompensateStorage.view
{
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.Modules.CompensateStorage.tools.ShowMoneyCS;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class CompensateStorageDetailsViewManager implements CSViewManager
	{
		private var panelBase:PanelBase;
		private var detailsView:MovieClip;
		/**
		 * 滚动面板  
		 */
		private var scrollPanel:UIScrollPane;
		/** 补偿详细描述*/
		protected var dtailsText:DetailsText;
		
		private var _parent:DisplayObjectContainer;
		
		private var _maxHeight:uint = 100;
		
		public function CompensateStorageDetailsViewManager(parent:DisplayObjectContainer)
		{
			_parent = parent;
		}
		
		public function init():void
		{
			if( CompensateStorageData.domain.hasDefinition( "CompensateStorageDetails" ) )
			{
				var CompensateStorageDetails:Class = CompensateStorageData.domain.getDefinition( "CompensateStorageDetails" ) as Class;
				detailsView = new CompensateStorageDetails();
			}
			panelBase = new PanelBase( detailsView,detailsView.width + 8 ,detailsView.height + 12 );
			panelBase.SetTitleTxt( CompensateStorageData.CompensateStorageDetailsName );
			
			dtailsText=new DetailsText(240);
			scrollPanel=new UIScrollPane(dtailsText);
			scrollPanel.x = 10;
			scrollPanel.y = 3;
			scrollPanel.width=detailsView.width-13;
			scrollPanel.height=detailsView.height - 28;
			scrollPanel.scrollPolicy=UIScrollPane.SCROLLBAR_ALWAYS;
			scrollPanel.mouseEnabled=false;
			scrollPanel.refresh();
			detailsView.addChild(scrollPanel);
			
			CompensateStorageData.isInitDetailsView = true;
		}

		public function show(object:Object = null):void
		{
			if(!CompensateStorageData.isInitDetailsView)
			{
				init();
			}
			if(!CompensateStorageData.isShowDetailsView)
			{
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				CompensateStorageData.isShowDetailsView = true;
				addLis();
				var mainView:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("CompensateStorageView");
				if(mainView != null)
				{
					panelBase.x = mainView.x + mainView.width;
					panelBase.y = mainView.y;
				}
			}
			update(CompensateStorageData.compensateDetails);
		}
		
		/**
		 * 更新显示数据
		 */		
		public function update(str:String):void{
			dtailsText.tfText=str;	
			ShowMoneyCS.ShowIcon(dtailsText,dtailsText.textField);
			scrollPanel.refresh();
		}
		
		private function addLis():void
		{
			panelBase.addEventListener( Event.CLOSE, close );
			detailsView.btn_CompensateStorage_close.addEventListener( MouseEvent.CLICK, close );
		}
		
		private function removeLis():void
		{
			panelBase.removeEventListener( Event.CLOSE, close );
			detailsView.btn_CompensateStorage_close.removeEventListener( MouseEvent.CLICK, close );
		}
		
		public function close(event:Event = null):void
		{
			if(CompensateStorageData.isShowDetailsView)
			{
				removeLis();
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				CompensateStorageData.isShowDetailsView = false;
			}
		}
	}
}