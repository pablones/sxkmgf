package OopsEngine.Scene.StrategyElement.Person
{
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import OopsFramework.Game;
	
	import flash.geom.Point;

	public class GameElementTernal extends GameElementAnimal
	{
		public function GameElementTernal(game:Game)
		{
			super(game, new GameElementTernalSkin(this));
		}
		
		public override function Initialize():void
		{
            if(this.skins != null)
			{
				if(this.isInitialize == false)
				{
					this.mouseChildren = false;
					this.mouseEnabled  = false;
					
					this.addChild(this.skins);
					
					this.SetMoveSpend(5);
					this.skins.LoadSkin();	
					
//					var p:Point = MapTileModel.GetTilePointToStage(this.Role.TileX, this.Role.TileY);
//					this.X      = p.x;
//					this.Y      = p.y;
					
					this.skins.x = - 50;
					this.skins.y = - 200;	
					
					this.excursionY = 75;
				}
			}
			super.ternalInitialize();
		}
		
		/** 移动 */
		public override function Move(targetPoint:Point,moveStepLength:int = 0):void 
		{
			this.smoothMove.MoveStepLength = moveStepLength;
            this.smoothMove.Move([targetPoint]);
		}
		
		public override function Dispose():void
		{
			super.Dispose();
		}
		
		/** 人物半透明 */
		protected override function Translucence():void {}	
	}
}