package GameUI.Modules.Task.View
{
	import GameUI.SetFrame;
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class EquPanel extends Sprite
	{
		
		/** 物品数组[type,{num:1,type:310000},{},.....]*/
		protected var goods:Array;
		protected var cells:Array;
		protected var padding:uint=10;
		protected var choose:Boolean;
		protected var selectedIndex:int=-1;
		
		public var changeSelectedGood:Function;
		
		public function EquPanel(goods:Array=null,choose:Boolean=true)
		{
			super();
			this.goods=goods;
			this.choose=choose;
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.cells=[];
			var len:uint=goods.length;
			for(var i:uint=1;i<len;i++){
				if(goods[i]["goodJob"]!=GameCommonData.Player.Role.CurrentJobID && goods[i]["goodJob"]!=null){
					continue;
				}
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				var faceItem:FaceItem=new FaceItem(goods[i].goodId);
				faceItem.Num=goods[i].goodNum;
				gridUnit.addChildAt(faceItem,1);
				gridUnit.name="TaskEqu_"+goods[i].goodId;
				gridUnit.index=i-1;
				if(this.choose){
					gridUnit.addEventListener(MouseEvent.CLICK,onGridUintClick);
				} 
				this.cells.push(gridUnit);
				this.addChild(gridUnit);		
			}
			this.doLayout();
		}
		
		protected function onGridUintClick(e:MouseEvent):void{
			var index:uint=e.target.index;
			var len:uint=this.cells.length;
			for (var i:uint=0;i<len;i++){
				SetFrame.RemoveFrame(this.cells[i]);	
			}
			var faceItem:FaceItem=(this.cells[index] as MovieClip).getChildAt(1) as FaceItem;
			SetFrame.UseFrame(faceItem);
			if(this.changeSelectedGood!=null){
				this.changeSelectedGood(goods[index+1].goodId);
			}
		}
			
		protected function doLayout():void{
			var currentX:Number=0;
			var currentY:Number=0;		
			for each(var cell:MovieClip in this.cells){
				cell.x=currentX;
				cell.y=currentY;
				currentX+=cell.width+padding;
			}
		}
		
	}
}