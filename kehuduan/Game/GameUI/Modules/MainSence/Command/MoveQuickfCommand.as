package GameUI.Modules.MainSence.Command
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.MainSence.Data.MainSenceData;
	import GameUI.Modules.MainSence.Mediator.MainSenceMediator;
	import GameUI.Modules.OnlineGetReward.Mediator.OnlineRewardMediator;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class MoveQuickfCommand extends SimpleCommand implements IUpdateable
	{
		private var obj:Object;
		private var object:Object;
		private var timer:Timer;
		private var _point:Point;
		private var objectPoint:Point;
		private var hasOnlineAward:Boolean = false;
		private var distance:Number = 40;
		private var leftX:Number = 0;
		private var rightX:Number = 0;
		private var intervalId:uint;
		
		private var mcQuickBar:MovieClip;
		private var mainSenceMed:MainSenceMediator;
		private var onlineRewardMed:OnlineRewardMediator;  
		
		public function MoveQuickfCommand()
		{
			super();
			timer = new Timer();
			timer.DistanceTime = 3;
		}
		
		public override function execute(notification:INotification):void
		{
			obj = notification.getBody();
			if( facade.hasCommand( MainSenceData.MOVEQUICKF ) ) facade.removeCommand( MainSenceData.MOVEQUICKF );
			mainSenceMed = facade.retrieveMediator( MainSenceMediator.NAME ) as MainSenceMediator;
			mcQuickBar = mainSenceMed.mainSence.mcQuickBar1;
			if( mainSenceMed.isVisible() )
			{
				sendNotification( SkillConst.MOVESKILL, obj ); 
				mainSenceMed = null;
				mcQuickBar = null;
				return;
			}
			objectPoint = mainSenceMed.getQuickfPoint();
			_point = new Point( mcQuickBar.x, mcQuickBar.y );
			GameCommonData.GameInstance.GameUI.addChildAt( mcQuickBar, 0 );
			
			mcQuickBar.x = objectPoint.x;
			mcQuickBar.y = objectPoint.y + 36;
			onlineRewardMed = facade.retrieveMediator( OnlineRewardMediator.NAME ) as OnlineRewardMediator;
			if( GameCommonData.GameInstance.GameUI.contains( onlineRewardMed.onLineAwardView ) )
			{
				hasOnlineAward = true;
				leftX = mcQuickBar.x - onlineRewardMed.onLineAwardView.x;
				rightX = mcQuickBar.x + mcQuickBar.width;
			}
			
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(this.timer!=null && this.timer.IsNextTime(gameTime))
			{
				mcQuickBar.y -= 2;
				
				if( hasOnlineAward )
				{
					if( (leftX <= onlineRewardMed.onLineAwardView.x <= rightX) && (mcQuickBar.y - onlineRewardMed.onLineAwardView.y < distance) )
					{
						onlineRewardMed.onLineAwardView.y = mcQuickBar.y - distance;
					}
				}
				
				if( mcQuickBar.y <= objectPoint.y )
				{
					mcQuickBar.y = objectPoint.y;
					
					mainSenceMed.mainSence.addChild( mcQuickBar );
					mcQuickBar.x = _point.x;
					mcQuickBar.y = _point.y;
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
					
					if( hasOnlineAward )
					{
						if( (leftX <= onlineRewardMed.onLineAwardView.x <= rightX) && (objectPoint.y - onlineRewardMed.onLineAwardView.y < distance) )
						{
							onlineRewardMed.onLineAwardView.y = objectPoint.y - distance;
						}
					}
					mcQuickBar = null;
					mainSenceMed = null;
					onlineRewardMed = null;
					
					intervalId = setTimeout( onTime , 500 );
					
				}
				
				
			}
		}
		
		private function onTime():void
		{
			 clearTimeout( intervalId ); 
			 sendNotification( SkillConst.MOVESKILL, obj ); 
		}
		
		public function get Enabled():Boolean							// 是否启动更新
		{
			return true;
		}
		public function get UpdateOrder():int							// 更新优先级（数值小的优先更新）
		{
			return 0;
		}
		
		public function get EnabledChanged():Function
		{
			return null;
		}
		public function set EnabledChanged(value:Function):void
		{
			
		}	
        public function get UpdateOrderChanged():Function
		{
			return null;
		}
        public function set UpdateOrderChanged(value:Function):void
        {
        	
        }
	}
}