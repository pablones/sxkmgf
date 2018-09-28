package GameUI.Modules.Master.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.View.AwardCell;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MasterAwardMediator extends Mediator
	{
		public static const NAME:String = "MasterAwardMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var scroll:UIScrollPane = null;
		
		private var masterName:String;
		private var masterLevel:uint;
		private var rate:uint;
		private var remark:String;
		private var pLevel:uint;																		//徒弟的等级
		
		private var title_mc:MovieClip;
		private var cellContainer:Sprite;
		private var aCell:Array;
		
		public function MasterAwardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get masterAwardView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [EventList.INITVIEW,
						MasterData.SHOW_AWARD_VIEW,
						MasterData.SHOW_AWARD_LIST
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					initUI();
					break;
				case MasterData.SHOW_AWARD_VIEW:
					initData( notification.getBody() );
					showUI();
//					showList();
					break;
				case MasterData.SHOW_AWARD_LIST:
					MasterData.isRequestAward = false;
					showList( notification.getBody() );
					break;
			}
		}
		
		private function initUI():void
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"MasterInfo"});
			panelBase = new PanelBase(masterAwardView, masterAwardView.width+8, masterAwardView.height+12);
			panelBase.name = "masterAwardView";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos2.x - 220;
			panelBase.y = UIConstData.DefaultPos2.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_mas_med_mastaw_ini_1" ]);     //师徒奖励
			
			title_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("AwardState");
			title_mc.y = 6;
			title_mc.x = 20;
		}
		
		private function initData(obj:Object):void
		{
			if ( obj )
			{
				this.masterName = obj.mName;
				this.masterLevel = obj.mLevel;
				this.rate = obj.rate;
				this.remark = MasterData.getPrenticeRemark(this.rate);
				this.pLevel = obj.pLevel;
			}
		}
		
		private function showUI():void
		{
			if ( !GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
				dataProxy.masterAwardIsOpen = true;
			}
			
			( masterAwardView.name_txt as TextField ).text = this.masterName;
			( masterAwardView.level_txt as TextField ).text = this.masterLevel.toString();
			( masterAwardView.teRate_txt as TextField ).text = this.rate.toString() + "/" +MasterData.maxInitiator;
			( masterAwardView.teLevel_txt as TextField ).text = this.remark;
			
			clearContainer();
		}
		
		private function showList(_obj:Object):void
		{
			if ( !cellContainer )
			{
				cellContainer = new Sprite();
				aCell = new Array();	
			}
			
			aCell.push(title_mc);
			cellContainer.addChild(title_mc);
			
			//获取奖励掩码
			var len:uint = ( 60-25 )/5 + 1;
			var nGiftMask:int = _obj.list[0].nGiftMask;
			
			for ( var i:uint=0; i<len; i++ )
			{
				var obj:Object = new Object();
				obj.level = 25+i*5;
				obj.pLevel = this.pLevel;
				
				if ( (this.pLevel >= obj.level) )                                                  //判断是否领取过奖励
				{
					var arr:Array = ( nGiftMask>>i ).toString(2).split("");
					if ( arr[arr.length-1] == "0" )
					{
						obj.giftMask = 0;
					}
					else if ( arr[arr.length-1] == "1" )
					{
						obj.giftMask = 1;
					}
				}
				else
				{
					obj.giftMask = 2;
				}
				
				var awardCell:Sprite = new AwardCell(obj);
				awardCell.x = 20;
				awardCell.y = 25 + i*30;
				aCell.push(awardCell);
				cellContainer.addChild(awardCell);
			}
			
			cellContainer.graphics.beginFill(0xff0000,0);
			cellContainer.graphics.drawRect(0,0,228,265);
			cellContainer.graphics.endFill();
			
			scroll = new UIScrollPane(cellContainer);
			scroll.x = 0;
			scroll.y = 93;
			scroll.width = 243;
			scroll.height = 187;
			scroll.refresh();
			this.masterAwardView.addChild(scroll);
		}
		
		private function clearContainer():void
		{
			if ( cellContainer && cellContainer.numChildren>0 )
			{
				for ( var i:int=cellContainer.numChildren-1; i>=0; i-- )
				{
					cellContainer.removeChildAt(0);
				}
			}
			
			if ( aCell && aCell.length>0 )
			{
				for ( var j:uint=0; j<aCell.length; j++ )
				{
					aCell[j] = null;
				}
			}
			if ( scroll && masterAwardView.contains(scroll) )
			{
				masterAwardView.removeChild(scroll);
			}
//			cellContainer = null;
//			aCell = null;
		}
		
		private function panelCloseHandler(evt:Event):void
		{
			clearContainer();
			if ( GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				dataProxy.masterAwardIsOpen = false;
			}
			MasterData.isRequestAward = false;
		}
		
	}
}