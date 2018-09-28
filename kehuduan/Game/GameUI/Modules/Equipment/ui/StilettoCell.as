package GameUI.Modules.Equipment.ui
{
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
	import GameUI.UIUtils;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;

	public class StilettoCell extends Sprite  implements IDataGridCell
	{
		
		protected var view:MovieClip;
		protected var fourthStiletto:MovieClip;	//第四个孔
		protected var normalStiletto:MovieClip;
		protected var data:Object;
		
		
		/** 灰色遮挡  */
		protected var shape:Shape;
		/** 描述文本 */ 
		protected var tf:TextField;
		/** 宝石 */
		protected var stone:NumberItem;
		
		protected var id:uint;
		/**是否是第四个孔*/
		protected var isFourthStiletto:Boolean;
		public function StilettoCell()
		{
			super();
			this.normalStiletto=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Stiletto");
			this.fourthStiletto=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("fourthStiletto");
			this.view = normalStiletto;
			initStiletto(false);
		}
		
		private function initStiletto(isFourth:Boolean):void
		{
			if(this.contains(view)) 
			{
				this.removeChild(view);
			}
			if(isFourth)
			{
				view = fourthStiletto;
			}
			else
			{
				view = normalStiletto;
			}
			this.view.mouseEnabled=false;
			this.addChild(this.view);
			this.view.gotoAndStop(2);  
		}
		
		/**
		 *  value.type : 1.开孔但还没有打宝石  2.开了孔并且放入了宝石  3.已经打上了宝石的孔   4:可以开孔  5：开了孔  6:选中准备取的孔 8:第四孔
		 *  @param value
		 * 
		 */		
		 
		 
		public function set cellData(valueObj:Object):void
		{
			this.clearAll();
			this.data=valueObj;
			var value:Object = UIUtils.DeeplyCopy(valueObj);
			if(value.type >= 10){
				isFourthStiletto = true;
				data.isFourthStiletto = true;
				initStiletto(true);
				value.type = int((value.type-1)/10);
			}else{
				initStiletto(false);
			}
			this.visible=true;
			if(value.type==1){
				this.view.gotoAndStop(2);
				if(this.tf==null){
					this.tf=new TextField();
					this.tf.text=GameCommonData.wordDic[ "mod_equ_ui_sti1_cel_1" ];//"可以\n镶嵌"
					this.tf.textColor=0xffffff;
					this.tf.mouseEnabled=false;
					this.tf.autoSize=TextFieldAutoSize.LEFT;
					this.tf.x=Math.floor((56-this.tf.width)/2);
					this.tf.y=Math.floor((56-this.tf.height)/2);
				}
				if(this.shape==null){
					this.shape=new Shape();
				}
				this.shape.graphics.clear();
				this.shape.graphics.beginFill(0x999999,0.5);
				this.shape.graphics.drawRect(0,0,32,32);
				this.shape.graphics.endFill();
				this.addChild(this.shape);
				this.addChild(this.tf);
				this.mouseEnabled=false;
				this.mouseChildren=false;
				this.shape.x=this.shape.y=12;
				this.view.x=this.view.y=28;
			}else if(value.type==2){
				this.view.gotoAndStop(2);
				this.mouseEnabled=true;
				this.stone=new NumberItem(value.stoneType,"icon");
				this.stone.mouseEnabled=false;
				this.addChildAt(this.stone,0);
				this.stone.x=12;
				this.stone.y=12;
				this.addEventListener(MouseEvent.CLICK,mouseClick);
				this.view.x=this.view.y=28;
			}else if(value.type==3){
				this.view.gotoAndStop(1);
				this.stone=new NumberItem(value.stoneType,"icon");
				this.stone.mouseEnabled=false;
				this.addChildAt(this.stone,0);
				this.stone.x=12;
				this.stone.y=12;
				this.view.x=this.view.y=28;
				
			}else if(value.type==4){
				this.view.gotoAndStop(2);
				if(this.tf==null){
					this.tf=new TextField();
					var expStr:String = GameCommonData.wordDic[ "mod_equ_ui_sti1_cel_2" ];//"可以\n打孔";
//					if(isFourthStiletto){
//						expStr = "尚未\n激活";
//					}
					this.tf.text=expStr;
					this.tf.textColor=0xffffff;
					this.tf.mouseEnabled=false;
					this.tf.autoSize=TextFieldAutoSize.LEFT;
					this.tf.x=Math.floor((56-this.tf.width)/2);
					this.tf.y=Math.floor((56-this.tf.height)/2);
				}
				if(this.shape==null){
					this.shape=new Shape();
				}
				this.shape.graphics.clear();
				this.shape.graphics.beginFill(0x999999,0.5);
				this.shape.graphics.drawRect(0,0,32,32);
				this.shape.graphics.endFill();
				this.addChild(this.shape);
				this.addChild(this.tf);
				this.mouseEnabled=false;
				this.mouseChildren=false;
				this.shape.x=this.shape.y=12;
				this.view.x=this.view.y=28;
			}else if(value.type==5){
				this.view.gotoAndStop(2);
				this.view.x=this.view.y=28;
			}else if(value.type==6){
				this.view.gotoAndStop(1);
				this.mouseEnabled=false;
				this.stone=new NumberItem(value.stoneType,"icon");
				this.stone.mouseEnabled=false;
				this.addChildAt(this.stone,0);
				this.stone.x=12;
				this.stone.y=12;
				this.view.x=this.view.y=28;
				this.id=setInterval(animationFun,500);
			}else if(value.type==7){
				this.view.gotoAndStop(1);
				this.mouseEnabled=true;
				this.addEventListener(MouseEvent.CLICK,mouseClick);
				this.stone=new NumberItem(value.stoneType,"icon");
				this.stone.mouseEnabled=false;
				this.addChildAt(this.stone,0);
				this.stone.x=12;
				this.stone.y=12;
				this.view.x=this.view.y=28;
			}else{
				this.mouseEnabled=false;
				this.visible=false;
			}		
			if(isFourthStiletto){
				if(shape && this.contains(shape)){
					shape.x = shape.x + 56;
				}
				if(tf && this.contains(tf)){
					tf.x = 70;
				}
				if(this.stone && this.contains(stone)){
					stone.x = stone.x + 56;
				}
				this.view.x = 84;
				this.mouseEnabled = true;
				this.name = "fourthStiletto";
			} 
		}
		
		
		protected function animationFun():void{
			if(this.view.currentFrame==1){
				this.view.gotoAndStop(2);
			}else{
				this.view.gotoAndStop(1);
			}
		}
		
		
		protected function mouseClick(e:MouseEvent):void{
			var e1:GridCellEvent=new GridCellEvent(GridCellEvent.GRIDCELL_CLICK);
			e1.data=this.data;
			this.dispatchEvent(e1);
		}
		
		
		public function clearAll():void{
			if(this.shape!=null && this.contains(this.shape)){
				this.removeChild(this.shape);
			}
			if(this.tf!=null && this.contains(this.tf)){
				this.removeChild(this.tf);
			}
			if(this.stone!=null && this.contains(this.stone)){
				this.removeChild(this.stone);
			}
			this.removeEventListener(MouseEvent.CLICK,mouseClick);
			this.mouseEnabled=false;
			this.mouseChildren=false;
			this.view.gotoAndStop(2);	
			if(id!=0){
				clearInterval(id);
			}
				
		}
		
		public function dispose():void
		{
			this.clearAll();
		}
		
		public function get cellWidth():uint
		{
			return 56;
		}
		
		public function get cellHeight():uint
		{
			return 56;
		}
		
	}
}