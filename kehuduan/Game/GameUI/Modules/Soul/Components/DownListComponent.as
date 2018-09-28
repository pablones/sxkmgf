package GameUI.Modules.Soul.Components
{
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
	//下拉列表组件
	public class DownListComponent extends Sprite
	{
		public var clickFun:Function;
		
		private var title_mc:MovieClip;
		private var back_mc:MovieClip;
		private var down_btn:SimpleButton;
		private var _dataList:Array;
		
		private var mcWidth:uint;
		private var mcHeight:uint;
		private var container:Sprite = new Sprite();
		private var title_txt:TextField;							//标题文本
		private var format:TextFormat;
//		private var menu:MovieClip;
		
		public function DownListComponent( _mcWidth:uint=80,_mcHeight:uint=20 )
		{
			mcWidth = _mcWidth;
			mcHeight = _mcHeight;
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );	
		}
		
		//设置数据
		public function set dataList( value:Array ):void
		{
			_dataList = value;
			initUI();
		}
		
		private function addStageHandler( evt:Event ):void
		{
			if ( !_dataList ) return;
			addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );	
//			initUI();
		}
		
		//初始化UI
		private function initUI():void
		{
			title_mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "DownListComponentsRes" );
			back_mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "DownListBackgroundRes" ); 
			down_btn = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByButton( "DownListBtnRes" );
			
			title_mc.width = mcWidth;
			title_mc.height = mcHeight;
			
			down_btn.x = title_mc.x + title_mc.width - 18;
			down_btn.y = title_mc.y + 1;
			
			down_btn.addEventListener( MouseEvent.MOUSE_DOWN,clickTitleHandler );
			GameCommonData.GameInstance.stage.addEventListener( MouseEvent.MOUSE_DOWN,clickUIHandler );
			addChild( title_mc );
			addChild( down_btn );
			container.addChild( back_mc );
			container.mouseEnabled = false;
//			container.y = mcHeight;
			addMenue();
			initTitle();
			
			back_mc.y = title_mc.height;
			back_mc.width = mcWidth;
			back_mc.height = _dataList.length * mcHeight;
		}
		
		//初始化标题
		private function initTitle():void
		{
			format = new TextFormat();
			format.size = 12;
			format.font = "宋体";//"宋体";
			format.align = "center";
			format.color = 0xE2CCA5;
			
			this.title_txt = new TextField();
			title_txt.mouseEnabled = false;
			title_txt.width = mcWidth - 18;				//减去按钮的宽度
			title_txt.height = mcHeight;
			title_txt.setTextFormat( format );
			title_txt.x = 30;
			title_txt.x = 33;
			
			
			title_txt.x = 2;
			title_txt.y = 2;
//			title_txt.filters = Font.Stroke( 0x000000 );
			addChild( title_txt );
			
			setTitle( _dataList[0].toString() );
		}
		
		//设置标题文字
		public function setTitle( value:String ):void
		{
			title_txt.text = value;
			title_txt.setTextFormat( format );
		}
		
		private function addMenue():void
		{
			for ( var i:uint=0; i<_dataList.length; i++ )
			{
				var listCell:DownListCell = new DownListCell( _dataList[ i ].toString(),mcWidth,mcHeight );
				listCell.y = ( i+1 ) * this.mcHeight;
				listCell.clickFun = clickCellHandler;
				container.addChild( listCell );
			}
		}
		
		private function clickCellHandler( str:String ):void
		{
			setTitle( str );
			if ( clickFun != null )
			{
				clickFun( str );
			}
		}
		
		private function clickTitleHandler( evt:MouseEvent ):void
		{
			evt.stopPropagation();
			if ( this.contains( container ) )
			{
				removeChild( container );
			}
			else
			{
				addChild( container );	
			}
		}
		
		private function clickUIHandler( evt:MouseEvent ):void
		{
			if ( this.contains( container ) )
			{
				removeChild( container );
			}
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );	
			down_btn.removeEventListener( MouseEvent.MOUSE_DOWN,clickTitleHandler );
			GameCommonData.GameInstance.stage.removeEventListener( MouseEvent.MOUSE_DOWN,clickUIHandler );
		}
		
		public function gc():void
		{
			this.clickFun = null;
//			this.title_mc = null;
			this.back_mc = null;
			removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );	
			var des:*;
			while ( container.numChildren > 0 )
			{
				des = container.removeChildAt( 0 );
				if ( des is DownListCell )
				{
					( des as DownListCell ).gc();
				}
				des = null;
			}
		}
	}
}