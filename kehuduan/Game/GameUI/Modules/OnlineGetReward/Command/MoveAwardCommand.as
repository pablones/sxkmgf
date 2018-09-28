package GameUI.Modules.OnlineGetReward.Command
{
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Bitmap;
	import flash.geom.Point;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class MoveAwardCommand extends SimpleCommand implements IUpdateable
	{
		private var obj:Object;
		private var bitmap:Bitmap;
		private var timer:Timer;
		private var arr:Array = new Array();
		private var count:int;
		private var _point:Point;
		private var tempx:Number;
		private var tempy:Number;
		private var delay:Number = 20;
		private var intervalId:uint;
		private var countArr:Array = [30, 60];
		private var isFirst:Boolean = true;
		
		public function MoveAwardCommand()
		{
			super();
			this.timer = new Timer();
			this.timer.DistanceTime = 4; 
		}
		
		public override function execute(notification:INotification):void
		{
			_point = notification.getBody() as Point;
			if ( facade.hasCommand( OnLineAwardData.MOVE_AWARD ) )
			{
				facade.removeCommand( OnLineAwardData.MOVE_AWARD );
			}
			
			var length:uint = OnLineAwardData.items.length;
			if( GameCommonData.fullScreen == 1)
			{
				count = countArr[0];
			}else{
				count = countArr[1];
			}
			for(var i:uint=0; i<length; i++)
			{
				obj = OnLineAwardData.items.shift();
				bitmap = obj.bmp as Bitmap;
				GameCommonData.GameInstance.GameUI.addChild(bitmap);
				
				bitmap.x = obj.x;
				bitmap.y = obj.y;
				
				tempx = (_point.x - obj.x)/count;
				tempy = (_point.y - obj.y)/count;
				arr.push( {x:tempx, y:tempy} );
				OnLineAwardData.items.push( bitmap );
			}
			
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(this.timer!=null && this.timer.IsNextTime(gameTime))
			{
				
				if( bitmap.x+40 > _point.x && bitmap.y+40 > _point.y )
				{
					if( isFirst )
					{
						isFirst = false;
						this.timer.DistanceTime = 10; 
						count = 10
						arr = new Array();
//						_point.x -= 3;
						for each(var b:Bitmap in OnLineAwardData.items)
						{
							b.alpha = .7;
							tempx = (_point.x - b.x)/count;
							tempy = (_point.y - b.y)/count;
							arr.push( {x:tempx, y:tempy} );
//							_point.x += 3;
						}
					}
				}
				
				for(var i:uint=0; i<OnLineAwardData.items.length; i++)
				{
					bitmap = OnLineAwardData.items[i] as Bitmap;
					tempx = arr[i].x;
					tempy = arr[i].y;
					bitmap.x += tempx;
					bitmap.y += tempy;
					
				}
				count--;
				if( count == 0 )
				{
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
//					this.intervalId = setTimeout( removeUI, this.delay );
                    removeUI();
				}
			}
		}
		
		private function removeUI():void
		{
//			clearTimeout(intervalId);
			for(var i:uint=0; i<OnLineAwardData.items.length; i++)
			{
				bitmap = OnLineAwardData.items[i] as Bitmap;
				if(GameCommonData.GameInstance.GameUI.contains(bitmap))
				{
					GameCommonData.GameInstance.GameUI.removeChild(bitmap);
				}
			}
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