package GameUI.Modules.PlayerInfo.UI
{
	import GameUI.View.Components.UISprite;
	
	import flash.utils.Dictionary;

	public class HeadImgList extends UISprite
	{
		
		/** 头像之间的间距*/
		protected var imgSpace:uint=3;
		/** 数据提供者 [[.....],[....]] */
		protected var _dataPro:Array;
		/** 渲染集合[[][][]]*/
		protected var cells:Array;
		
		protected var cacheDic:Dictionary;
		
		public function HeadImgList(imgSpace:uint=3)
		{
			super();
			this.imgSpace=imgSpace;
			this.cacheDic=new Dictionary();
			this.createChildren();
		}
		
		/**
		 * 创建元素 
		 * 
		 */		
		protected function createChildren():void{
			this.cells=[];
			for each(var arr:Array in this._dataPro){
				var tempCells:Array=[];
				if(arr.length==0)continue;
				for each(var data:Object in arr){
					if(!data.hasOwnProperty("icon"))continue;
					var cell:ImgCell=this.getCell(data["icon"],data["tip"],data["isDeBuff"]);
					cell.cellData=data["cellData"];
					tempCells.push(cell);
					this.addChild(cell);
				}
				this.cells.push(tempCells);
			}
			this.doLayout();	
		}
				
		protected function toRepaint():void{
			this.removeAllCell();
			this.createChildren();
		}
		
		protected function doLayout():void{
			
			var currentY:uint=0;
			for each(var arr:Array in this.cells){
				if(arr.length==0)return;
				var currentX:uint=0
				var tempCell:ImgCell;
				for each(var cell:ImgCell in arr){
					
					cell.x=currentX;
					cell.y=currentY;
					currentX+=cell.width+imgSpace;
					tempCell=cell;
				}
				currentY+=tempCell.height+imgSpace;
			}
		}
		
		
		/**
		 * 获得渲染器 
		 * @return 
		 * 
		 */		
		protected function getCell(face:String,tip:String,isDeBuff:Boolean):ImgCell{
			var cell:ImgCell
			if(this.cacheDic[face]!=null){
				cell=this.cacheDic[face];
			}else{
				cell=new ImgCell(face,tip,isDeBuff);
			}
			return cell;
		}
		
		/**
		 * 设置列表数据 
		 * @param value
		 * 
		 */		
		public function set dataPro(value:Array):void{
			this._dataPro=value;
			this.toRepaint();
		}
		
		public function get dataPro():Array{
			return this._dataPro;
		}
		
		/**
		 * 清除所有的渲染器 
		 * 
		 */		
		protected function removeAllCell():void{
			for each(var arr:Array in this.cells){
				for each(var cell:* in arr){
					if(this.contains(cell)){
						this.removeChild(cell);
						this.cacheDic[cell.iconUrl]=cell;
					}
				}
			}
		}
		
	}
}