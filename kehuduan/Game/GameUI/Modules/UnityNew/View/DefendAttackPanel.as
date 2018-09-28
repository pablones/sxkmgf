package GameUI.Modules.UnityNew.View
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Soul.Components.DownListComponent;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class DefendAttackPanel
	{
		private var list:Array;
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		private var downListComponent:DownListComponent;
		private var openState:Boolean = false;
		
		private var chooseIndex:int;
		
		public function DefendAttackPanel()
		{
			initData();
			initUI();
		}
		
		public function showMe():void
		{
			if ( !openState )
			{
				main_mc.btn_sure.addEventListener( MouseEvent.CLICK,onCommit );
				main_mc.btn_cancel.addEventListener( MouseEvent.CLICK,panelClosed );
				panelBase.addEventListener( Event.CLOSE, panelClosed );
			}
			
			initData();
			downListComponent.dataList = list;
			downListComponent.clickFun = this.clickFun;
			chooseIndex = 0;
			showMoney();
			
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			panelBase.x = UIConstData.DefaultPos2.x;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt( "抵抗袭击" );
			openState = true;
		}
		
		private function initData():void
		{
			var level:uint = NewUnityCommonData.unityPlaceLevelArr[ 1 ];
			var str:String;
			list = [];
			for ( var i:uint=1; i<=level; i++ )
			{
				str = i.toString() + "级分堂boss";
				list.push( str );
			}
		}
		
		private function initUI():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.DefendAttackRes ) as MovieClip;
//			panelBase = new PanelBase( main_mc,main_mc.width+8,main_mc.height+12 );
			panelBase = new PanelBase( main_mc,243,main_mc.height+14 );
			main_mc.txt_btn.mouseEnabled = false;
			main_mc.mouseEnabled = false;
//			panelBase.addEventListener( Event.CLOSE, panelClosed );
//			this.addChild( panelBase );
			
			downListComponent = new DownListComponent( 95,20 );
			downListComponent.clickFun = clickFun;
			downListComponent.dataList = list;
			downListComponent.x = 70;
			downListComponent.y = 38;
			main_mc.addChild( downListComponent );
			
		}
		
		private function clickFun( str:String ):void
		{
//			trace ( str );
			chooseIndex = list.indexOf( str );
			if ( chooseIndex == -1 ) return;
			showMoney();
		}
		
		//显示钱
		private function showMoney():void
		{
			var needMoney:int = 100 + 20*chooseIndex;
			
			
			if ( NewUnityCommonData.myUnityInfo.money >= needMoney*10000 )
			{
				main_mc.money_mc.txtMoney.textColor = 0x00ff00;
				main_mc.btn_sure.filters = null;
				main_mc.txt_btn.filters = null;
				MasterData.addGlowFilter( main_mc.txt_btn );
				main_mc.btn_sure.mouseEnabled = true;
			}
			else
			{
				main_mc.money_mc.txtMoney.textColor = 0xff0000;
				MasterData.setGrayFilter( main_mc.btn_sure );
				MasterData.setGrayFilter( main_mc.txt_btn );
				main_mc.btn_sure.mouseEnabled = false;
			}
			main_mc.money_mc.txtMoney.text = UIUtils.getMoneyStandInfo( needMoney*10000, ["\\ce","\\cs","\\cc"] );
			ShowMoney.ShowIcon( main_mc.money_mc, main_mc.money_mc.txtMoney, true );
		}
		
		private function onCommit( evt:MouseEvent ):void
		{
			RequestUnity.send( 322,0,chooseIndex+1 );
			panelClosed( null );
		}
		
		private function panelClosed( evt:Event ):void
		{
			panelBase.removeEventListener( Event.CLOSE, panelClosed );
			main_mc.btn_sure.removeEventListener( MouseEvent.CLICK,onCommit );
			main_mc.btn_cancel.removeEventListener( MouseEvent.CLICK,panelClosed );
			openState = false;
			this.downListComponent.gc();
			
			if ( GameCommonData.GameInstance.GameUI.contains( this.panelBase ) )
			{
				GameCommonData.GameInstance.GameUI.removeChild( this.panelBase );
			}
		}
		
	}
}