package OopsEngine.Scene.StrategyScene
{
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.GameScene;
	import OopsEngine.Utils.Vector2;
	
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	
	import flash.geom.Point;
	
	/** 游戏场景（场景平移保证在3像素左右抖动会好很多,解决方法可以用提高场景移动的频率去减小移动的位移量大小）*/
	public class GameScenePlay extends GameScene
	{
		//缓冲校对开启
		public var IsUpdateNicety:Boolean = false;	
		//最小移动距离
        public var MinMoveDistance:Number = 30;
        //校对点
        public var IsUpdateNicetyPoint:Point = null;
        
        public var Dither:int = 12;	

		public function GameScenePlay(game:Game)
		{
			super(game);
			this.MouseEnabled = true;
		}
		
		public override function Update(gameTime:GameTime):void
		{ 
			//屏幕抖动效果
			if(Dither < 12)
			{
				switch(Dither % 4)
				{
					case 0:this.x = this.x;        	
					       this.y = this.y + 10;  
					       break;   	
					case 1:this.x = this.x;        	
					       this.y = this.y - 10;     
					       break;    
					case 2:this.x = this.x + 10;        	
					       this.y = this.y;        
					       break; 
					case 3:this.x = this.x - 10;        	
					       this.y = this.y;     
					       break;           
				}
		    	Dither ++;  	    				  
		    }
			
			//是否启动校对
			if(IsUpdateNicety &&  IsUpdateNicetyPoint != null)
			{
				//当前场景的点
				var nowPoint:Point = new Point(this.x,this.y);

                //两点间的距离
                var Distance:Number = Point.distance(nowPoint,IsUpdateNicetyPoint);        	
        	    
	        	//判断是否大于最小灵敏距离
	            if(Distance > 4)
	            {
	            	var MovePoint:Point = Vector2.MoveDistance(nowPoint,IsUpdateNicetyPoint,4);
	            	this.x = MovePoint.x;        	
	            	this.y = MovePoint.y;
	            }
	            else
	            {
	            	//缓冲校对完成
	            	IsUpdateNicety =  false;
	            	IsUpdateNicetyPoint = null
	            }
	            
	            if(UpdateNicety != null)
	               UpdateNicety();
			}
			
			//解析动画控制
//			CommonData.AnalyzeUpdate();
			
			super.Update(gameTime);
		}
		
		/** 地址平移范围计算 */
		public function SceneMove(x:Number, y:Number,IsNicety:Boolean = false):Point
		{
			var ScenePosX:Number = this.Games.ScreenWidth  / 2 - x;
			var ScenePosY:Number = this.Games.ScreenHeight / 2 - y;
			
			if(ScenePosX > 0)
			{
				ScenePosX = 0;
			}
			else if(ScenePosX < -(this.MapWidth + this.OffsetX - this.Games.ScreenWidth))
			{
				ScenePosX = -(this.MapWidth + this.OffsetX - this.Games.ScreenWidth);
			}
			
			if(ScenePosY > 0)
			{
				ScenePosY = 0;
			}
			else if(ScenePosY < -(this.MapHeight + this.OffsetY - this.Games.ScreenHeight))
			{
				ScenePosY = -(this.MapHeight + this.OffsetY - this.Games.ScreenHeight);
			}

		    IsUpdateNicetyPoint = new Point(ScenePosX,ScenePosY);
		   
			//判断是否需要准确的点
			if(IsNicety)
			{
				return  new Point(ScenePosX,ScenePosY);
			}
			else
			{
				//当前的点
				var nowPoint:Point = new Point(this.x,this.y);
				//要移动的点
				var targetPoint:Point = new Point(ScenePosX,ScenePosY);
                //两点间的距离
                var Distance:Number = Point.distance(nowPoint,targetPoint);
                
	        	//判断是否大于最小灵敏距离
	            if(Distance < MinMoveDistance)
	            {
	            	return  nowPoint;
	            }
	            else
	            {        	
	            	var MovePoint:Point = Vector2.MoveDistance(nowPoint,targetPoint,Distance - MinMoveDistance);
	            	return  MovePoint;
	            }
	            
   			}
		}
	}
}