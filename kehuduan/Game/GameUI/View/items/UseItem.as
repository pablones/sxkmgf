package GameUI.View.items
{
	import GameUI.ConstData.UIConstData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.ResourcesFactory;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;

	public class UseItem extends ItemBase
	{
		private var icon:Bitmap;
		
		//拖动镜像
		private var startDragTmp:Sprite = null;
		/**  标记装备不适合职业遮照  */
		private var noFitJobShape:Shape;
//		/** 位置 */
//		private var pos:int;
//		/** 图标名字符串 */
//		private var iconStr:String;
//		/** 父对象*/
//		private var itemParent:DisplayObjectContainer;
		
		public function UseItem(pos:int, icon:String, parent:DisplayObjectContainer)
		{
			
			tf = new TextField();
			var iconStr:String;			/** 储存图片名 */ 
			if(uint(icon) > 100000) 
			{	//410101
				if(UIConstData.getItem(uint(icon)).img != null) 
				{
					iconStr = String(UIConstData.getItem(uint(icon)).img);
				}
			}
			else
			{ 
				iconStr=String(icon);
			}
			var loadingIcon:MovieClip=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BuffGrid");
			loadingIcon.gotoAndStop(1);
			var bitMapData:BitmapData=new BitmapData(loadingIcon.width,loadingIcon.height);
			bitMapData.draw(loadingIcon);
			this.icon=new Bitmap(bitMapData);
			this.addChildAt(this.icon,0);
			super(iconStr, parent);
			this.Type=uint(icon);
			tf.filters = Font.Stroke(0);
			tf.mouseEnabled = false;
			tf.selectable = false;
			this.Pos = pos;
		}
		
		/**
		 * 初始化 
		 * @param icon
		 * @param parent
		 * @param mkDir
		 * 
		 */		
		public override function init(icon:String, parent:DisplayObjectContainer, pos:uint=0,mkDir:String = "icon"):void{
			
			tf = new TextField();
			var iconStr:String;			/** 储存图片名 */ 
			if(uint(icon) > 100000) {	//410101
				if(UIConstData.getItem(uint(icon)).img != null)
				{
					iconStr = String(UIConstData.getItem(uint(icon)).img);
				}
			}else{
				iconStr=String(icon);
			}
			var loadingIcon:MovieClip=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BuffGrid");
			loadingIcon.gotoAndStop(1);
			var bitMapData:BitmapData=new BitmapData(loadingIcon.width,loadingIcon.height);
			bitMapData.draw(loadingIcon);
			this.icon=new Bitmap(bitMapData);
			this.addChildAt(this.icon,0);
			super.init(iconStr, parent);
			this.Type=uint(icon);
			tf.filters = Font.Stroke(0);
			tf.mouseEnabled = false;
			tf.selectable = false;
			this.Pos = pos;
			
		}
		
		/**
		 * 重设置与回收 
		 * 
		 */		
		public override function reset():void{
			super.reset();
			
			ResourcesFactory.getInstance().deleteCallBackFun(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + iconName + ".png",onLoabdComplete);
			if(this.icon!=null && this.contains(this.icon)){
				this.removeChild(this.icon);
			}
			this.icon=null; 
			if(this.startDragTmp!=null && this.startDragTmp.parent!=null){
				this.startDragTmp.parent.removeChild(this.startDragTmp);
			}
			this.startDragTmp=null;
			if(this.noFitJobShape!=null && this.contains(this.noFitJobShape)){
				this.removeChild(this.noFitJobShape);
			}
		}
		
		
		
		protected override function onLoabdComplete():void
		{
			
			super.onLoabdComplete();
 			this.removeChild(this.icon);
			this.icon=ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/icon/" + this.iconName + ".png")
 			if(icon==null){
	 			icon = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
 			}
 			
			this.addChildAt(icon,0);
			tf.width = WIDTH;
			tf.height = TXTHEIGHT;
			tf.y = HEIGHT - TXTHEIGHT;
			tf.setTextFormat(UIUtils.getTextFormat());
			this.addChildAt(tf,1);
			if(this.getChildByName("maskLock"))
			{
				this.setChildIndex(this.getChildByName("maskLock"), this.numChildren-1);
			}
		}
		
		public function onMouseDown():void
		{
			this.ItemParent=this.parent;
//			if(IsCdTimer) return;
			this.dragged = false;
			tmpX = this.x;
			tmpY = this.y;
			//绘制可拖的bitmap_____________________________________________________
			var bitmap:Bitmap = new Bitmap();
			var bmd:BitmapData = new BitmapData(WIDTH, HEIGHT);
			bmd.draw(this.icon);
			bitmap.bitmapData = bmd;
			startDragTmp = new Sprite();
			startDragTmp.alpha = .8;
			startDragTmp.addChild(bitmap);
			startDragTmp.mouseChildren = false;
			startDragTmp.mouseEnabled = false;
			var point:Point = this.ItemParent.localToGlobal(new Point(this.x, this.y)) as Point;
			startDragTmp.x = point.x;
			startDragTmp.y = point.y;
			startDragTmp.startDrag(); //------------------------------------9.1
			GameCommonData.GameInstance.GameUI.addChild(startDragTmp);
			startDragTmp.name = "useItemTmp";
			//____________________________________________________________________
			//在主场景中添加该绘制
			GameCommonData.GameInstance.GameUI.addEventListener(MouseEvent.MOUSE_UP, gameUIMouseUp);
			GameCommonData.GameInstance.GameUI.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			GameCommonData.GameInstance.GameUI.mouseEnabled = true;
			GameCommonData.GameInstance.addEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
		}
		
		private function mouseOutHandler(e:MouseEvent):void
		{
			gc();
		}
		
		//移动物品，在拖动过程中永远让该拖动对象处在最上层
		private function onMouseMove(e:MouseEvent):void
		{
			this.dragged = true;
			GameCommonData.GameInstance.GameUI.setChildIndex(startDragTmp, GameCommonData.GameInstance.GameUI.numChildren-1);
			e.updateAfterEvent();
		}
		
		public function gc():void
		{ 
			GameCommonData.GameInstance.removeEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
			GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_UP, gameUIMouseUp);
			GameCommonData.GameInstance.GameUI.mouseEnabled = false;
			startDragTmp.stopDrag();
			GameCommonData.GameInstance.GameUI.removeChild(startDragTmp);
			startDragTmp = null;
		}
		
		public function getBitmap():Bitmap
		{
			var btmData:BitmapData = new BitmapData(WIDTH, HEIGHT);
			btmData.draw(this.icon);
			var btm:Bitmap = new Bitmap(btmData);
			return btm;
		}
		
		//场景UP事件
		private function gameUIMouseUp(e:MouseEvent):void
		{
//			trace("");
			//移除选择框
			removeFrame(e.target.parent);
			GameCommonData.GameInstance.removeEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
			GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			//关闭GameUI的鼠标事件
			GameCommonData.GameInstance.GameUI.mouseEnabled = false;
			startDragTmp.stopDrag();
			GameCommonData.GameInstance.GameUI.removeEventListener(MouseEvent.MOUSE_UP, gameUIMouseUp);
			GameCommonData.GameInstance.GameUI.removeChild(startDragTmp);
			startDragTmp = null;
			var type:String = e.target.name.split("_")[0];
			var index:int = int(e.target.name.split("_")[1]);
			var obj:Object = new Object();
			if(type == "UILayer")
			{
				this.dispatchEvent(new DropEvent(DropEvent.DRAG_THREW, this));
			}
			else
			{	
				obj.type = type;
				obj.index = index;
				obj.target = e.target;
				obj.source = this;
				this.dispatchEvent(new DropEvent(DropEvent.DRAG_DROPPED,obj));	
			}			
		}
				
		//移除外框
		//移除黄框
		private function removeFrame(parent:DisplayObjectContainer):void
		{
			if(parent.getChildByName("yellowFrame")) 
    		{
    			parent.removeChild(parent.getChildByName("yellowFrame"));
    		}	
		} 
			
		//___________________________________________________________
		//物品数量
		//当物品少于或等于1的时候，隐藏该数量
		public override function get Num():int
		{
			return this.num;
		}
		
		public override function set Num(v:int):void
		{
			this.num = v;
			tf.visible = true;
			tf.text = v.toString();
			if(v<=1)
			{
				tf.visible = false;
			} 	
			tf.setTextFormat(UIUtils.getTextFormat());
		}
		
		/**
		 * 设置不符合人物职业装备标记
		 * @param isShow ：true 设置标记 false :取消此标记
		 * 
		 */		
		public function setNoFitJobShape(isShow:Boolean):void{
			if(isShow){
				if(this.noFitJobShape==null){
					this.noFitJobShape=new Shape();
					this.noFitJobShape.graphics.clear();
					this.noFitJobShape.graphics.beginFill(0xff0000,0.8);
					this.noFitJobShape.graphics.drawRect(0,0,32,32);
					this.noFitJobShape.graphics.endFill();
				}
				this.addChild(this.noFitJobShape);
			}else{
				if(this.noFitJobShape!=null && this.contains(this.noFitJobShape)){
					this.removeChild(this.noFitJobShape);
				}
			}
		}	
	}
}