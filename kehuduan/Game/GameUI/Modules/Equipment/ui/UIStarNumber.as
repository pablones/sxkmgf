package GameUI.Modules.Equipment.ui
{
	import GameUI.View.Components.UISprite;
	
	import flash.display.DisplayObject;

	public class UIStarNumber extends UISprite
	{
		protected var cells:Array=[];
		
		protected var bottomCells:Array=[];
		protected var num:uint;
		
		public function UIStarNumber()
		{
			super();
			this.init();
		}
		
		protected function init():void{
			var x:Number=0;
			for(var i:uint=0;i<10;i++){
				var cell:NumberItem=new NumberItem("star5");
				this.addChild(cell);
				bottomCells.push(cell);
			}	
		}
		
		protected function toRepaint():void{
			this.clear();
			this.createCells();
			this.doLayout();			
		}
		
		protected function createCells():void{
			
			
			
			
			for(var i:uint=0;i<this.num;i++){
				var cell:NumberItem;
				if(num<=3){
					cell=new NumberItem("star1");
				}else if(num<=6){
					cell=new NumberItem("star2");
				}else if(num<=8){
					cell=new NumberItem("star3");
				}else if(num<=10){
					cell=new NumberItem("star4");
				}	
				this.addChild(cell);
				this.cells.push(cell);
			}
		}
		
		protected function doLayout():void{
			var x:uint=0;
		
			for each(var obj:DisplayObject in this.bottomCells){
				obj.x=x;
				x+=14;
				
			}
			
			
			var len:uint=this.cells.length;
			x=0;
			for(var i:uint=0;i<len;i++){
				 this.cells[i].x=x;
				 this.cells[i].y=0;
				 x+=14;
			}
		}
		
		protected function clear():void{
			for each(var obj:DisplayObject in this.cells){
				this.removeChild(obj);
			}
			this.cells=[];
		}
		
		/**
		 * 设置星星显示数量 
		 * @param value
		 * 
		 */		
		public function setStarNum(value:uint):void{
			if(this.num==value)return;
			this.num=value;
			this.toRepaint();
		}
		
	}
}