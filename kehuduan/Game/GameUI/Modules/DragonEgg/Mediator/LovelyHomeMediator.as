package GameUI.Modules.DragonEgg.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.DragonEgg.Data.DragonEggData;
	import GameUI.Modules.DragonEgg.Data.DragonEggVo;
	import GameUI.View.BaseUI.AutoPanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	
	import Net.ActionSend.PetEggSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LovelyHomeMediator extends Mediator
	{
		public static const NAME:String = "LovelyHomeMediator";
		private var main_mc:MovieClip;
		private var panelBase:AutoPanelBase;
		
		private var fastPur:FastPurchase;
		
		private var itemType:int = 630032;
		private var eggId:int;
		private var openState:Boolean = false;
		
		private var dragonEggMed:DragonEggMediator;
		
		public function LovelyHomeMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							DragonEggData.SHOW_LOVELY_HOME,
							DragonEggData.CLOSE_LOVELY_HOME,
							EventList.ONSYNC_BAG_QUICKBAR
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case DragonEggData.SHOW_LOVELY_HOME:
					eggId = notification.getBody() as int;
					showMe();
				break;
				case DragonEggData.CLOSE_LOVELY_HOME:
					if ( openState )
					{
						closeMe(null);
					}
				break;
				case EventList.ONSYNC_BAG_QUICKBAR:
					if ( openState )
					{
						showTxt();
					}
				break;
			}
		}
		
		private function showMe():void
		{
			if ( !main_mc )
			{
				main_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip( "LovelyHome" );
				
				panelBase = new AutoPanelBase( main_mc,main_mc.width+68,main_mc.height+15 );
				panelBase.x = UIConstData.DefaultPos2.x;
				panelBase.y = UIConstData.DefaultPos2.y;
				panelBase.SetTitleTxt( "孵化器" );
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				panelBase.addEventListener( Event.CLOSE,closeMe );
				fastPur = new FastPurchase( itemType.toString() );
				fastPur.x = main_mc.width - 12;
				main_mc.addChild( fastPur );
				openState = true;
				
				dragonEggMed = facade.retrieveMediator( DragonEggMediator.NAME ) as DragonEggMediator;
				
				main_mc.commit_btn.addEventListener( MouseEvent.CLICK,onCommit );
				main_mc.cancel_btn.addEventListener( MouseEvent.CLICK,closeMe );
				
				showTxt();
//				fastPur.y = 
			}
		}
		
		private function showTxt():void
		{
			var num:int = BagData.hasItemNum( this.itemType );
			if ( num>0 )
			{
				main_mc.txt.htmlText = "<font color='#e2cca5'>    使用孵化器可立即诞生5只品质更好的皮丘卡。当前拥有<font color='#00ff00'>"+num+"</font>个孵化器，每次使用消耗<font color='#00ff00'>1</font>个，是否确认使用？</font>";
			}
			else
			{
				main_mc.txt.htmlText = "<font color='#e2cca5'>    使用孵化器可立即诞生5只品质更好的皮丘卡。当前的孵化器数量为<font color='#00ff00'>0</font>，请在右边的快速购买栏中购买后使用。</font>";
			}
		}
		
		private function onCommit( evt:MouseEvent ):void
		{
//			var item:Object = BagData.getItemByType( this.itemType );
//			NetAction.UseItem( OperateItem.USE, 1, item.index, item.id );
			var num:int = BagData.hasItemNum( this.itemType );
			if ( num == 0 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( "您的孵化器不足" );
				return;
			}
			
			if ( hasGoodEgg() )
			{
				var info:String = "<font color='#e2cca5'>当前5只宠物中含有成长和档次都较高的宠物，是否确认刷新？</font>";    //是否要与            解除师徒关系？
				GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});  //提 示      确 定      取 消 
				return;
			}

			sendData();
		}
		
		private function commitHandler():void
		{
			sendData();
		}
		
		private function cancelClose():void{}
		
		private function sendData():void
		{
			var obj:Object = new Object();
			obj.itemid = eggId;
			obj.action = 2;
			obj.petid = 0;
			obj.level = 0;
			obj.grow = 0;
			PetEggSend.SendPetEggAction( obj );
		}
		
		private function hasGoodEgg():Boolean
		{
			var curList:Array = dragonEggMed.dataList;
			for ( var i:uint=0; i<curList.length; i++ )
			{
				var dragonVo:DragonEggVo = curList[ i ];
				if ( dragonVo.level>6 && dragonVo.grow>=81 )
				{
					return true;
				}
			}
			return false;
		}
		
		private function closeMe( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeMe );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				main_mc.commit_btn.removeEventListener( MouseEvent.CLICK,onCommit );
				main_mc.cancel_btn.removeEventListener( MouseEvent.CLICK,closeMe );
				fastPur.gc();
				fastPur = null;
			}
			main_mc = null;
			panelBase = null;
			openState = false;
		}
		
	}
}