package GameUI.Modules.HeroSkill.Command
{	
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.UICore.UIFacade;
	import GameUI.View.items.SkillItem;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Bitmap;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class MoveSkillIconCommand extends SimpleCommand implements IUpdateable
	{
		private var obj:Object;
		private var bitmap:Bitmap;
		private var timer:Timer;
		private var count:int;
		private var _point:Point;
		private var objectPoint:Point;
		private var tempx:Number;
		private var tempy:Number;
		private var countArr:Array = [30, 60];
		private var isFirst:Boolean = true;
		private var skillItem:SkillItem;
		
		public function MoveSkillIconCommand()
		{
			super();
			this.timer = new Timer();
			this.timer.DistanceTime = 2; 
		}
		
		public override function execute(notification:INotification):void
		{
			obj = notification.getBody() as Object;
			if ( facade.hasCommand( SkillConst.MOVESKILL ) )
			{
				facade.removeCommand( SkillConst.MOVESKILL );
			}
			
			if( GameCommonData.fullScreen == 1)
			{
				count = countArr[0];
			}else{
				count = countArr[1];
			}
			
			skillItem = obj.source as SkillItem;
			_point = skillItem.parent.localToGlobal( new Point(skillItem.x, skillItem.y) );
			objectPoint = obj.point as Point;
			bitmap = skillItem.getBitmap();
			GameCommonData.GameInstance.GameUI.addChild(bitmap);
			
			bitmap.x = _point.x;
			bitmap.y = _point.y;
			bitmap.alpha = .6;
			tempx = (objectPoint.x - _point.x)/count;
			tempy = (objectPoint.y - _point.y)/count;
			
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(this.timer!=null && this.timer.IsNextTime(gameTime))
			{
				
				if( bitmap.x+40 > objectPoint.x && bitmap.y+40 > objectPoint.y )
				{
					if( isFirst )
					{
						isFirst = false;
						this.timer.DistanceTime = 10; 
						count = 10
						bitmap.alpha = .7;
						tempx = (objectPoint.x - bitmap.x)/count;
						tempy = (objectPoint.y - bitmap.y)/count;
					}
				}
				
				bitmap.x += tempx;
				bitmap.y += tempy;
				count--;
				if( count == 0 )
				{
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
                    GameCommonData.GameInstance.GameUI.removeChild(bitmap);
                    UIFacade.GetInstance(UIFacade.FACADEKEY).dragItem( {type:obj.type, index:obj.index, source:obj.source} );
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