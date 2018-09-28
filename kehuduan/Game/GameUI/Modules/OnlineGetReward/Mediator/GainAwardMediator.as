package GameUI.Modules.OnlineGetReward.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.OnlineGetReward.Command.MoveAwardCommand;
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	import GameUI.Modules.OnlineGetReward.Event.TimeOutEvent;
	import GameUI.Modules.OnlineGetReward.Net.GainAwardAction;
	import GameUI.Modules.OnlineGetReward.UI.TimeOutTxt;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.UseItem;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class GainAwardMediator extends Mediator
	{
		public static const NAME:String = "GainAwardMediator";
		
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var time_txt:TimeOutTxt;
		private var giftContainer:Sprite;
		private var aGrid:Array;
		
		public function GainAwardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get gainAwardView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [EventList.INITVIEW,
						OnLineAwardData.ONLINE_GAINWARD_PANENL,
						OnLineAwardData.CLOSE_GAINAWARD_PAN
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:OnLineAwardData.ONLINE_GAINWARD_PANENL});
					panelBase = new PanelBase(gainAwardView, gainAwardView.width-2, gainAwardView.height+14);
					panelBase.name = "gainAwardView";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_onl_med_gai_han_1" ] );  // 领取在线奖励
					dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
				break;
				case OnLineAwardData.ONLINE_GAINWARD_PANENL:
					if ( !dataProxy.GainAwardPanIsOpen )
					{
						if ( !time_txt )
						{
							time_txt = notification.getBody().txt;
						}
						initData();
						initUI();	
					}
					else
					{
						panelCloseHandler(null);
					}
				break;
				case OnLineAwardData.CLOSE_GAINAWARD_PAN:
					panelCloseHandler(null);
				break;
			}
		}
		
		private function initData():void
		{
			aGrid = new Array();
		}
		
		private function initUI():void
		{
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;
			}else{
				panelBase.x = 350;
				panelBase.y = 158;
			}
			if ( !GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
			}
			dataProxy.GainAwardPanIsOpen = true;
			( gainAwardView.close_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,panelCloseHandler);
			( gainAwardView.gainAward_txt as TextField ).mouseEnabled = false;
			// 小提示：在线礼物共有8份，以下是第     份礼物！
			( gainAwardView.hint_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_gai_ini_1" ]+(GameCommonData.Player.Role.OnLineAwardTime+1)+GameCommonData.wordDic[ "mod_onl_med_gai_ini_2" ];
			( gainAwardView.gainAward_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,gainAward);

			setGainButton( OnLineAwardData.canGain );
			
			time_txt.x = 115;
			time_txt.y = 9;
			gainAwardView.addChild( time_txt );
			time_txt.addEventListener( TimeOutEvent.TIME_IS_ZERO,timeOutHandler );
			
			giftContainer = new Sprite();
			gainAwardView.addChild(giftContainer);
			creatGrid();
			showGifts();
		}
		
		private function creatGrid():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				var grid:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				grid.x = 75+i*58;
				grid.y = 106;
				gainAwardView.addChild(grid);
				aGrid.push(grid);
			}
		}
		
		private function showGifts():void
		{
			var aGift:Array = OnLineAwardData.giftArr[ GameCommonData.Player.Role.OnLineAwardTime ];
			OnLineAwardData.items = new Array();
			if ( aGift.length>0 )
			{
				for ( var i:uint=0; i<aGift.length; i++ )
				{
					var obj:Object = aGift[i] as Object;
					var container:MovieClip = aGrid[i] as MovieClip;	
					var giftItem:UseItem = new UseItem(0,obj.type,container);
					if ( obj.num>1 )
					{
						giftItem.Num = obj.num;
					}
					giftItem.x = 2;
					giftItem.y = 1.5;
					container.name = "TaskEqu_"+obj.type;
					container.addChild(giftItem);
					OnLineAwardData.items.push( giftItem );
				}
			}
		}
		
		private function timeOutHandler(evt:TimeOutEvent):void
		{
			setGainButton( OnLineAwardData.canGain );
		}
		
		private function gainAward(evt:MouseEvent):void
		{
			var bmp:Bitmap;
			var bmpData:BitmapData;
			var point:Point;
			var length:uint = OnLineAwardData.items.length;
			
			var typeArr:Array = [];
			for ( var j:uint=0; j<length; j++ )
			{
				typeArr.push( OnLineAwardData.items[ j ].Type );
			}
			
			if ( BagData.canPushGroupBag( typeArr ) )
			{
				return;
			}
			
			//发送请求
			GainAwardAction.send();
			
			for ( var i:uint=0; i<length; i++ )
			{
				var item:UseItem = OnLineAwardData.items.shift() as UseItem;
				point = item.parent.localToGlobal( new Point(item.x, item.y) );
				bmp = item.getBitmap();
				bmp.x = point.x;
				bmp.y = point.y;
//				bmp.scaleX = .5;
//				bmp.scaleY = .5;
				bmp.alpha = .6;
				OnLineAwardData.items.push( {bmp:bmp, x:point.x, y:point.y} );
			}
			facade.registerCommand( OnLineAwardData.MOVE_AWARD, MoveAwardCommand );
			sendNotification( OnLineAwardData.GET_AWARD_POINT );
			this.time_txt = null;
//			OnLineAwardData.canGain = false;
//			facade.sendNotification( OnLineAwardData.NEXT_ONLINE_GIFT );
			panelCloseHandler(null);
		}
		
		private function panelCloseHandler(evt:Event):void
		{
			if ( aGrid && aGrid.length>0 )
			{
				for ( var i:uint=0; i<aGrid.length; i++ )
				{
					gainAwardView.removeChild(aGrid[i]);
					aGrid[i] = null;
				}
				aGrid = null;
			}
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				( gainAwardView.gainAward_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,gainAward);
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				dataProxy.GainAwardPanIsOpen = false;
			}
		}
		
		private function setGainButton(isSee:Boolean):void
		{
			if ( isSee )
			{
				( gainAwardView.gainAward_btn as SimpleButton ).visible = true;
				( gainAwardView.gainAward_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_gai_set_1" ];   // 领  奖
			}
			else
			{
				( gainAwardView.gainAward_btn as SimpleButton ).visible = false;
				( gainAwardView.gainAward_txt as TextField ).text = GameCommonData.wordDic[ "mod_onl_med_gai_set_2" ];   // 请等待
			}
		}
		
	}
}