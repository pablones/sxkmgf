package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Utils.Vector2;
	
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	
	import flash.geom.Point;

	public class GameElementEnemy extends GameElementAnimal
	{			
		public var Dither:int = 6;	
			
		public function GameElementEnemy(game:Game)
		{
			super(game, new GameElementEnemySkin(this));
		}
		
		
//		public override function Update(gameTime:GameTime):void
//		{
//			//屏幕抖动效果
//			if(Dither < 2)
//			{
//				switch(Dither % 2)
//				{
//					case 0:this.y = this.y + 5;  
//					       break;   	
//					case 1:this.y = this.y - 5;     
//					       break;              
//				}
//		    	Dither ++;  	    				  
//		    }
//			
//			if(this.skins!=null)
//			{
//				super.Update(gameTime);
//			}
//		}
		
		/** 敌人移动 */
		public override function Move(targetPoint:Point,distance:int = 0):void 
		{
			if(this.Role.ActionState != GameElementSkins.ACTION_DEAD &&
			   this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
			{
				var p:Point = targetPoint.add(new Point(-this.excursionX, -this.excursionY));
				if(this.smoothMove.IsMoving)
				{
					this.smoothMove.AddPath(p);
				}
				else
				{
					this.smoothMove.Move([p]);
				}
				this.Role.Direction = Vector2.DirectionByTan(this.GameX, this.GameY, targetPoint.x, targetPoint.y);
				this.SetAction(GameElementSkins.ACTION_RUN, this.Role.Direction);	
			}
		}
		
		public override function MoveTile(targetPoint:Point,distance:int = 0,IsStagePoint:Boolean = false):void
		{
//			if(this.Role.ActionState != GameElementSkins.ACTION_DEAD)
//			{
//				this.MustMovePoint = targetPoint;
//				this.Dis         = distance;
//			}
		
			if(this.Role.ActionState != GameElementSkins.ACTION_DEAD &&
			this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
			{
				this.Move(MapTileModel.GetTilePointToStage(targetPoint.x,targetPoint.y));
			}
		}
		
		protected override function onMoveComplete():void
		{
			super.onMoveComplete();
			
			if(this.Role.ActionState != GameElementSkins.ACTION_DEAD &&
			this.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
			{
				this.smoothMove.IsMoving = false;
				this.SetAction(GameElementSkins.ACTION_STATIC);
			}
		}
	}
}