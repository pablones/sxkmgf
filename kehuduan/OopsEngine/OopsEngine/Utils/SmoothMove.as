package OopsEngine.Utils
{
	import OopsFramework.GameTime;
	
	import flash.display.DisplayObject;
	import flash.geom.Point;
	
	/** 平滑移动组件 */
	public class SmoothMove
	{
		private var isMoving:Boolean = false;							// 是否在移动
		private var moveDistance:Number;								// 已移动了的距离
		private var moveBetweenDistance:Number;							// 两点之间的距离
		private var moveIncrement:Point;							    // 平滑移动增量
		private var startPoint:Point;									// 起始点
		private var endPoint:Point;										// 目标点
		private var displayObject:DisplayObject;			         	// 移动对象
		private var path:Array;											// 多点移动路径
		private var direction:int;										// 方向
		
		public var MoveNode:Function;									// 每移动一个节点
		public var MoveStep:Function;									// 每移动一步事件
		public var MoveComplete:Function;								// 移动完成事件
		public var CheckPoint:Function;                                 // 核对坐标点处理地图平移问题
		
		/** 每步步长 */
		public var MoveStepLength:int;
		
		public function get IsMoving():Boolean
		{
			return this.isMoving;
		}
		public function set IsMoving(value:Boolean):void
		{
			this.isMoving = value;
			if(this.isMoving == false)
			{
				this.path!=null;
			}
		}
		
		public function SmoothMove(displayObject:DisplayObject,moveStepLength:int)
		{
			this.displayObject      = displayObject;
			this.MoveStepLength     = moveStepLength;
		}
		
		/** 追加路径  */
		public function AddPath(p:Point):void
		{
			if(this.path!=null)
			{
				this.path.push(p);
			}
		}
		
		/** 开始移动 */
		public function Move(path:Array):void
		{
			this.Reset();
			this.path 				 = path;
			this.startPoint  		 = new Point(this.displayObject.x,this.displayObject.y);
			this.endPoint   		 = this.path.shift();
			this.direction			 = Vector2.DirectionByTan(this.startPoint.x, this.startPoint.y, endPoint.x, endPoint.y);	// 方向					
		    var dic:Number           = Point.distance(startPoint,endPoint);		    
		    if(dic < 1)
		    {
		    	if(CheckPoint != null)
            	 {
//            	 	trace("起计算调整");
            	 	this.moveIncrement = null;
            	 	CheckPoint(); 
            	 	return;           	 	
            	 }   	 
		    }
			
			this.moveIncrement       = Vector2.MoveIncrement(this.startPoint,this.endPoint,this.MoveStepLength);                // 平移偏移向量
			this.moveBetweenDistance = Point.distance(this.startPoint,this.endPoint);											// 两点距离

			this.moveDistance		 = 0;
			this.isMoving     		 = true;
			if(MoveNode!=null) MoveNode(this.direction);				// 移动到下一节前之前
		}
		
		/** 平滑移动动画逻辑更新 */
        public function Update(gameTime:GameTime):void
        {
        	if(this.isMoving && moveIncrement != null)
        	{
        		var px:Number = this.displayObject.x;
        		var py:Number = this.displayObject.y;
	        	
				this.moveDistance += this.MoveStepLength;
				px 				  += moveIncrement.x;
				py 				  += moveIncrement.y;
				
				if(this.moveDistance >= this.moveBetweenDistance)		// 当前移动距离大于或等于总距离
				{
					if(this.path != null && this.path.length > 0)
					{
						this.startPoint = this.endPoint;
						this.endPoint   = this.path.shift();
						
						var direction:int   = Vector2.DirectionByTan(this.startPoint.x, this.startPoint.y, endPoint.x, endPoint.y);	// 下一节点方向
						var distance:Number = Point.distance(this.startPoint,this.endPoint);										// 下一节点距离（Ａ＊格横向60，纵向30，斜向33.54)
																						
				        var dic:Number           = Point.distance(startPoint,endPoint);									
					    if(dic < 1)
					    {
					    	if(CheckPoint != null)
			            	 {
			            	 	trace("方向改变调整");
			            	 	this.moveIncrement = null;
			            	 	CheckPoint(); 
			            	 	return;           	 	
			            	 }   	 
					    }							
						this.moveIncrement  = Vector2.MoveIncrement(this.startPoint,this.endPoint,this.MoveStepLength);				// 转向后要计算新每步偏移量
						if(direction == this.direction)					// 同一方向（把要走的距离加上新距离）
						{
							this.moveBetweenDistance += distance;
						}
						else											// 不同方向
						{
							px 						 = this.startPoint.x;
							py 				         = this.startPoint.y;
							this.moveBetweenDistance = Point.distance(this.startPoint,this.endPoint);
							this.moveDistance 		 = 0;
						}
						this.direction = direction;
						if(MoveNode!=null)
						{
						   if(!MoveNode(this.direction))  // 移动到下一节前之前
						   	{
						   		return;
						   	}	
						}
					}
				}
				
				if(this.isMoving)
				{
					if(this.moveDistance >= this.moveBetweenDistance) 
					{
						px 			  = this.endPoint.x;
						py 			  = this.endPoint.y;
						this.isMoving = false;
						this.Reset();
						if(MoveComplete!=null) MoveComplete();
					}
        		}
        		else
        		{
        			this.Reset();
					if(MoveComplete!=null) MoveComplete();
        		}

                // 如果出现漂到左上角的BUG后，通过此判断将人物坐标迅速纠正坐标。
                if(Point.distance(new Point(this.displayObject.x,this.displayObject.y),new Point(px,py)) > 500)
                {
                	 if(CheckPoint != null)
                	 {
                	 	trace("移动距离过大: 起始点为 x:"+this.displayObject.x + " y:"+this.displayObject.y + " 目标点:x:" + px+ " y:" + py);
                	 	CheckPoint();            	 	
                	 }
                	 return;
                }
                
				this.displayObject.x = px;
				this.displayObject.y = py;					
			    if(MoveStep!=null) MoveStep();
                
        	}
        }
        
		private function Reset():void
		{
			this.moveDistance		 = 0;
			this.moveBetweenDistance = 0;
			this.moveIncrement 		 = null;
			this.path 				 = null;
			this.endPoint			 = null;
		}
	}
}