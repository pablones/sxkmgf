package GameUI.View.BaseUI
{
	import GameUI.UIUtils;
	import GameUI.View.ResourcesFactory;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.Graphics;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.geom.Rectangle;
	import flash.text.TextField;
 
	public class ItemBase extends Sprite
	{
	
		protected var num:Number = 0;
		protected var isLock:Boolean = false;
		protected var maxCD:Number = 25;
		protected var mkDir:String = "icon";
		
		public var tmpX:Number = 0;
		public var tmpY:Number = 0;
		public var Pos:int = -1;	
		public var ItemParent:DisplayObjectContainer = null;
		public var iconName:String = "";
		public var IsBind:uint = 0;      // 0:绑定  1：不绑定
		public var Type:int = 0;
		public var Id:int = 0;
		
		public var dragged:Boolean = false;

	
//		/**  cd组件 */
//		protected var cdLayer:UICoolDownView;
//		/**  cd当前度数    */
//		public var curCdCount:int;
//		/**  cd总时间            */					
//		public var cdTotalTime:uint;
//		/** 是否正在Cd*/
//		public var IsCdTimer:Boolean = false;
//		private var timer:Timer;
		
	
		private var isEnable:Boolean = true;
		private var maskRect:Rectangle = new Rectangle(0,0,32,32);
		
		public static const WIDTH:uint = 32;
		public static const HEIGHT:uint = 32;
		public static const TXTHEIGHT:uint = 16;
		public static const OFFSET:uint = 2;
		
		/** 物品数量显示文本框 */
		public var tf:TextField;
		/** 锁定mask*/
		private var maskLock:Shape;
		/** 使能*/
		private var maskShape:Shape;
		/**循环对象，是cd循环转动*/
//		private var cdUpdateComp:CDUpdateComponent;
		public function ItemBase(icon:String, parent:DisplayObjectContainer, mkDir:String = "icon" )
		{
			this.cacheAsBitmap=true;
			tf = new TextField();
			this.iconName = icon;
			this.Type=uint(icon);
			this.doubleClickEnabled = true;
			this.mouseChildren = false;
			this.mouseEnabled = false;
			this.ItemParent = parent;
			this.mkDir = mkDir;
			this.maskLock=new Shape();
			this.maskShape=new Shape();	
			loadIcon();
			tf.filters =Font.Stroke(0);
			tf.mouseEnabled = false;
			tf.selectable = false;
			
			initCDSprite();
		}
		
		/**
		 * 初始化 
		 * @param icon
		 * @param parent
		 * @param mkDir
		 * 
		 */		
		public function init(icon:String, parent:DisplayObjectContainer, pos:uint=0,mkDir:String = "icon"):void{
			this.cacheAsBitmap=true;
			tf = new TextField();
			this.iconName = icon;
			this.Type=uint(icon);
			this.doubleClickEnabled = true;
			this.mouseChildren = false;
			this.mouseEnabled = false;
			this.ItemParent = parent;
			this.mkDir = mkDir;
			this.maskLock=new Shape();
			this.maskShape=new Shape();
			loadIcon();
			tf.filters =Font.Stroke(0);
			tf.mouseEnabled = false;
			tf.selectable = false;
			
		}
		
		
		
		/**
		 * 重置对象 
		 * 
		 */		
		public function reset():void{
			
			ResourcesFactory.getInstance().deleteCallBackFun(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + iconName + ".png",onLoabdComplete);
			num = 0;
			isLock = false;
			mkDir = "icon";
			
			tmpX = 0;
			tmpY = 0;
			Pos = -1;	
			ItemParent = null;
			iconName = "";
			IsBind = 0;      // 0:绑定  1：不绑定
			Type = 0;
			Id = 0;
			
//			if(this.cdLayer!=null && this.contains(cdLayer)){
//				this.removeChild(cdLayer);
//			}
//			curCdCount=0;
//			cdTotalTime=0;
//			cdUpdateComp.dipoise();
//			IsCdTimer = false;
			isEnable = true;
			maskRect = new Rectangle(0,0,32,32);
			if(tf!=null && this.contains(tf)){
				this.removeChild(tf);
			}
			this.tf=null;
			if(this.contains(this.maskShape)){
				this.removeChild(this.maskShape);
			}
			if(this.contains(this.maskLock)){
				this.removeChild(this.maskLock);
			}
		}	
		
		private function loadIcon():void
		{	
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + iconName + ".png",onLoabdComplete);
		}
		
//		/**
//		 * 开始Cd 
//		 * @param timer
//		 * @param curCount
//		 * 
//		 * 与服务器对Cd时间，当新的技能请求发来时，服务端CD已经结束，客户端会延时，
//			上一个CD可能还未结束，结束当前的CD，新启动新的cd
//		 */		
//		public function startCd(cdTotalTimer:uint, curCount:int):void
//		{	
//			
//			if(cdLayer==null){
//				cdLayer=new UICoolDownView();
//			}
//			this.addChild(cdLayer);
//			this.curCdCount = curCount;
//			this.cdTotalTime=cdTotalTimer;
//			this.cdLayer.update(curCdCount);
//			IsCdTimer = true;
//			var numTag:int = this.cdTotalTime/90+1;
//			cdUpdateComp = CDUpdateComponent.getInstance(numTag,Type);
//			cdUpdateComp.addEventListener("disposeCd",onCDUpdateAll);
//			cdUpdateComp.start(cdTime);
//
//		}
//		protected function onCDUpdateAll(e:Event):void
//		{
//			this.clearCd(); 
//		}
//	
//		/**
//		 * cd转操作 
//		 * 
//		 */		
//		protected function cdTime(name:String):void
//		{
//			switch(this.cdTotalTime)
//			{
//				case 1000:
//					this.curCdCount+=12;
//					break;
//				case 1500:
//					this.curCdCount+=9;
//					break;
//				case 2000:
//					this.curCdCount+=6;
//					break;
//				default:
//					this.curCdCount+=4;
//					break;
//			}
//			if(cdLayer == null) return;	
//			if(cdLayer.parent)
//			{
//				cdLayer.parent.setChildIndex(cdLayer, this.numChildren-1);
//				
//			}
//			if(this.curCdCount>=240)
//			{
//				this.clearCd();
//			}else{
//				this.cdLayer.update(this.curCdCount);
//			}
//		}
//		
//		/**
//		 *  清除已经有的Cd
//		 * 
//		 */		
//		public function clearCd():void{
//			IsCdTimer = false;
//			cdUpdateComp.removeEventListener("disposeCd",onCDUpdateAll);
//			cdUpdateComp.dipoise();
//			if(cdLayer)
//			{
//				if(cdLayer.parent)
//				{
//					cdLayer.parent.removeChild(cdLayer);
//				}
//				cdLayer = null;
//			}
//		}
		
		protected function onLoabdComplete():void
		{
			tf.width = WIDTH;
			tf.height = TXTHEIGHT;
			tf.y = HEIGHT - TXTHEIGHT;
			tf.setTextFormat(UIUtils.getTextFormat());
			this.addChild(tf);
		}
		
		//________________________________________________________________
		public function get Num():int
		{
			return this.num;
		}
		
		public function set Num(v:int):void
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
		
		public function get IsLock():Boolean
		{
			return this.isLock;
		}
		
		public function set IsLock(v:Boolean):void
		{
			if(this.isLock==v)return;
			this.isLock = v;
			if(v)
			{
				this.maskLock.graphics.clear();
				maskLock.graphics.beginFill(0xff0000,.6);
				maskLock.graphics.drawRect(0,0,32,32);
				maskLock.graphics.endFill();
				this.addChild(maskLock);
			}
			else
			{
				if(this.contains(this.maskLock))
				{
					this.removeChild(this.maskLock);
				}
			}
		}
		
		
		public function set MaskRect(rect:Rectangle):void
		{
			maskRect = 	rect;
		}
		
		public function set Enabled(v:Boolean):void
		{
			if(this.isEnable==v)return;
			this.isEnable=v;
			if(v)
			{
				if(this.contains(this.maskShape)){
					this.removeChild(this.maskShape);
				}		
			}
			else
			{
				this.maskShape.graphics.clear();
				maskShape.graphics.beginFill(0, 0.5);
				maskShape.graphics.drawRect(maskRect.x,maskRect.y,maskRect.width,maskRect.height);
				maskShape.graphics.endFill();
				this.addChild(maskShape);
			}
		}
		
		protected function setSkillMask():void
		{
			if( !contains(maskShape))
			{
				addChild(this.maskShape);
			}
			if(isEnable)
			{
				maskShape.graphics.clear();
			}
			else
			{
				maskShape.graphics.clear();
				maskShape.graphics.beginFill(0, 0.5);
				maskShape.graphics.drawRect(maskRect.x,maskRect.y,maskRect.width,maskRect.height);
				maskShape.graphics.endFill();
				if ( this.numChildren>1 )
				{
					this.addChildAt( maskShape,this.numChildren-1 );
				}
			}
		}

		// ------------------ 新冷却代码 -zhao
		private var cdSprite:Sprite;
		public function update(progress:Number):void
		{
			if (!cdSprite) return;
			
			var g:Graphics = cdSprite.graphics;
			g.clear();
//			g.lineStyle(1, 0xFFFF99, .5);
			g.beginFill(0xFFFF99, .5);
			
			if (progress == 1) return;
			if (progress == 0)
			{
				g.drawRect(0, 0, 32, 32);
				g.endFill();
				return;
			}
			
			var progRad:Number = Math.PI * 2 * progress;
			g.moveTo(16, 0);
			g.lineTo(16, 16);
			
			if (progress < .125)
			{
				g.lineTo(16 + Math.tan(progRad) * 16, 0);
				g.lineTo(32, 0);
				g.lineTo(32, 32);
				g.lineTo(0, 32);
				g.lineTo(0, 0);
				g.endFill();
			}
			else if (progress >= .125 && progress < .375)
			{
				g.lineTo(32, 16 - 16 / Math.tan(progRad));
				g.lineTo(32, 32);
				g.lineTo(0, 32);
				g.lineTo(0, 0);
				g.endFill();
			}
			else if (progress >= .375 && progress < .625)
			{
				g.lineTo((1 + Math.tan(Math.PI - progRad)) * 16, 32);
				g.lineTo(0, 32);
				g.lineTo(0, 0);
				g.endFill();
			}
			else if (progress >= .625 && progress < .875)
			{
				g.lineTo(0, (1 + Math.tan(Math.PI * 1.5 - progRad)) * 16);
				g.lineTo(0, 0);
				g.endFill();
			}
			else
			{
				g.lineTo((1 - Math.tan(Math.PI * 2 - progRad)) * 16, 0);
				g.endFill();
			}
		}
		
		public function initCDSprite():void
		{
			cdSprite = new Sprite();
			this.addChild(cdSprite);
		}
	}
}