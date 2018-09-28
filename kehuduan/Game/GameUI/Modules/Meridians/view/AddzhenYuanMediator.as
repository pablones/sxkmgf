package GameUI.Modules.Meridians.view
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.tools.Tools;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class AddzhenYuanMediator extends Mediator
	{
		
		public static const NAME:String = "AddzhenYuanMediator";
		
		private var addzhenYuan:MovieClip;							//补充真元
		private var panelBase:PanelBase;
		private var archeausNum:int = 0;							//真元丹数量
		private var fastPurchase:FastPurchase;							//快速购买
		
		
		public function AddzhenYuanMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					MeridiansEvent.INIT_ADDZHENYUAN,
					MeridiansEvent.SHOW_ADDZHENYUAN,
					MeridiansEvent.CLOSE_ADDZHENYUAN,
					MeridiansEvent.UPDATA_ARCHEAUS,
//					MeridiansEvent.UPDATA_ARCHEAUS_DAN,
//					BagEvents.UPDATEITEMNUM,
					EventList.ONSYNC_BAG_QUICKBAR,
				];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MeridiansEvent.INIT_ADDZHENYUAN:
					initUI();
					break;
				case MeridiansEvent.SHOW_ADDZHENYUAN:
					showUI();
					break;
				case MeridiansEvent.CLOSE_ADDZHENYUAN:
					closeUI(null);
					break;	
				case BagEvents.UPDATEITEMNUM:
					archeausNum = BagData.hasItemNum( 381001 );
					addzhenYuan.txt_1.htmlText= GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_1" ] + archeausNum + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_2" ] + (archeausNum * 500) + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_3" ]; //"   使用<font color='#00ffff'>真元丹</font>可迅速恢复大量真元。当前拥有<font color='#00ffff'>真元丹</font>数量为<font color='#00ff00'>"  "</font>个，每个可恢复真元<font color='#00ff00'>500</font>点，总共可恢复<font color='#00ff00'>" 
					break;
				case MeridiansEvent.UPDATA_ARCHEAUS_DAN:
					archeausNum = BagData.hasItemNum( 381001 );
					addzhenYuan.txt_1.htmlText=GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_1" ] + archeausNum + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_2" ] + (archeausNum * 500) + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_3" ];
					break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if(addzhenYuan && notification.getBody() == 381001)
					{
						archeausNum = BagData.hasItemNum( 381001 );
						addzhenYuan.txt_1.htmlText= GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_1" ] + archeausNum + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_2" ] + (archeausNum * 500) + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_3" ];
					}
					break;
			}
		}
		
		private function initUI():void
		{
			this.addzhenYuan = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AddzhenYuan");		//获取经脉修炼提示
			fastPurchase = new FastPurchase("381001");
			fastPurchase.y = 0;
			fastPurchase.x = 296;
			addzhenYuan.addChild(fastPurchase);
			panelBase = new PanelBase( addzhenYuan,addzhenYuan.width+67,addzhenYuan.height+12 );
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_ini_1" ] );//"补充真元"
			panelBase.x = UIConstData.DefaultPos2.x;
			panelBase.y = UIConstData.DefaultPos2.y;
			
			addzhenYuan.txt_1.mouseEnabled = false;
		}
		
		private function showUI():void
		{
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			panelBase.addEventListener( Event.CLOSE,closeUI );
			addzhenYuan.btn_ok.addEventListener( MouseEvent.CLICK,onButtonClick );
			archeausNum = BagData.hasItemNum( 381001 );
			
			addzhenYuan.txt_1.htmlText = GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_1" ] + archeausNum + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_2" ] + (archeausNum * 500) + GameCommonData.wordDic[ "Mod_Mer_mod_AddzhenYuanMediator_hand_3" ]; //"   使用<font color='#00ffff'>真元丹</font>可迅速恢复大量真元。当前拥有<font color='#00ffff'>真元丹</font>数量为<font color='#00ff00'>"  "</font>个，每个可恢复真元<font color='#00ff00'>500</font>点，总共可恢复<font color='#00ff00'>" 
		}
		
		private function onButtonClick(event:MouseEvent):void
		{
			switch(event.target.name)
			{
				case "btn_ok":
					if(archeausNum > 0)
					{		
						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,153);
						facade.sendNotification(MeridiansEvent.CLOSE_ADDZHENYUAN);
					}
					else
					{
						sendNotification(MeridiansEvent.CLOSE_ADDZHENYUAN);
//						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你的包裹里未发现真元丹", color:0xffff00});
					}
					break;
			}
		}
		
		private function closeUI( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeUI );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
		}
		
	}
}