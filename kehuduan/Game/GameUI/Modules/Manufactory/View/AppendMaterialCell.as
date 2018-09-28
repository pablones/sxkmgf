package GameUI.Modules.Manufactory.View
{
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Manufactory.Command.CheckCommitBtnCommand;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.Proxy.ManufatoryProxy;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.UISprite;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;

	public class AppendMaterialCell extends UISprite
	{
		private var consumeNum:uint;
		private var manuProxy:ManufatoryProxy;
		private var ratioCircle:MovieClip;
		private var dataObj:Object;
		private var format:TextFormat;
		
		private var aName:String;
		private var hasNum:uint;
		private var needNum:uint;
		private var appendType:uint;
		
		private var name_txt:TextField;
		private var hasNum_txt:TextField;
		private var needNum_txt:TextField;
		
		private var _isSelect:Boolean;
		private var shape:Shape;
		private var dataProxy:DataProxy;
		
		public function AppendMaterialCell(obj:Object)
		{
			super();
			this.mouseEnabled = true;
			dataObj = obj;
			initData();
			initUI();
		}
		
		private function initData():void
		{
			aName = dataObj.aName;
			needNum = 1;
			appendType = dataObj.id;
			hasNum = BagData.hasItemNum( appendType );
			dataProxy = UIFacade.GetInstance( UIFacade.FACADEKEY ).retrieveProxy( DataProxy.NAME ) as DataProxy;
		}
		
		private function initUI():void
		{
//			this.width = 286;
			this.width = 284;
			this.height = 20;
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function addStageHandler(evt:Event):void
		{
			manuProxy = UIFacade.UIFacadeInstance.retrieveProxy( ManufatoryProxy.NAME ) as ManufatoryProxy;	
			ratioCircle = new (manuProxy.ratioCircle) as MovieClip;
			ratioCircle.x = 16;
			ratioCircle.y = 3;
			this.addChild( ratioCircle );
			
			shape = new Shape();
//			shape.width = this.width;
//			shape.height = this.height;
//			shape.x = 2;
			shape.graphics.clear();
			shape.graphics.beginFill( 0x878581 );
			shape.graphics.drawRect(0,0,width,height);
			shape.graphics.endFill();
			
			initTexts();
			this.addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			this.addEventListener( MouseEvent.MOUSE_OVER,overHandler );
			this.addEventListener( MouseEvent.MOUSE_OUT,outHandler );
			this.addEventListener( MouseEvent.CLICK,clickHandler );
		}
		
		private function initTexts():void
		{
			format = new TextFormat();
			format.size = 12;
			format.font = GameCommonData.wordDic["mod_cam_med_ui_UIC1_getI"];//"宋体"
//			format.color = 0xffffff;
			
			name_txt = creatText();
			name_txt.textColor = 0xffffff;
			name_txt.text = aName;
			name_txt.x = 34;
			name_txt.width = 57;
			name_txt.autoSize = TextFieldAutoSize.LEFT;
			name_txt.setTextFormat( format );
			addChild( name_txt );
			
			needNum_txt = creatText();
			needNum_txt.textColor = 0xffffff;
			needNum_txt.autoSize = TextFieldAutoSize.CENTER;
			needNum_txt.x = 199;
			needNum_txt.width = 85;
			needNum_txt.text = (needNum*ManufactoryData.ManufatoryCount).toString();
			needNum_txt.setTextFormat( format );
			addChild( needNum_txt );
			
			hasNum_txt = creatText();
			hasNum_txt.x = 102;
			hasNum_txt.x = 95;
			hasNum_txt.width = 100;
//			hasNum_txt.border = true;
//			hasNum_txt.borderColor = 0xff0000;
			hasNum_txt.text = hasNum.toString();
			hasNum_txt.textColor = 0xff0000;
			hasNum_txt.setTextFormat( format );
//			hasNum_txt.autoSize = TextFieldAutoSize.CENTER;
			addChild( hasNum_txt );
			checkIsEnough();
		}
		
		//判断数量是否足够   外部调用接口
		public function checkIsEnough():void
		{
			hasNum = BagData.hasItemNum( appendType );
			hasNum_txt.text = hasNum.toString();
			hasNum_txt.setTextFormat( format );
			needNum_txt.text = (needNum*ManufactoryData.ManufatoryCount).toString();
			if ( hasNum<needNum*ManufactoryData.ManufatoryCount )				//数量不够
			{
				hasNum_txt.textColor = 0xff0000;
			}
			else
			{
				hasNum_txt.textColor = 0xffffff;
			}
		}
		
		//设置是否选中
		public function set isSelect(_select:Boolean):void
		{
			this._isSelect = _select;
			if ( _select )
			{
				ratioCircle.gotoAndStop(2);
				if ( !this.contains( shape ) )
				{
					this.addChildAt( shape,0 );
				}
				ManufactoryData.clickAppendType = dataObj.id;
			}
			else
			{
				ratioCircle.gotoAndStop(1);
				if ( this.contains( shape ) )
				{
					this.removeChild( shape );
				}
				ManufactoryData.clickAppendType = 0;
			}
			UIFacade.UIFacadeInstance.sendNotification( ManufactoryData.CHANGE_SUCESS_RATE,dataObj );
		}
		
		public function get isSelect():Boolean
		{
			return _isSelect;
		}
		
		//外部接口 设置高亮
		public function highLight(shape:Shape):void
		{
			this.addChildAt( shape,0 );
		}
		
		private function clickHandler(evt:MouseEvent):void
		{
			var obj:Object = new Object();
			obj.appendCell = this;
			obj.dataObj = dataObj;
			UIFacade.UIFacadeInstance.sendNotification( ManufactoryData.SELECT_APPEND_CELL,obj );
			UIFacade.UIFacadeInstance.sendNotification( CheckCommitBtnCommand.NAME );
//			trace ( "ManufactoryData.clickAppendType: "+ManufactoryData.clickAppendType );
		}
		
		private function overHandler(evt:MouseEvent):void
		{
			graphics.clear();
			graphics.beginFill(0x4d3c13,.7);
			graphics.drawRect(0,0,this.width,this.height);
			graphics.endFill();
		}
		
		private function outHandler(evt:MouseEvent):void
		{
			this.graphics.clear();
		}
		
		private function creatText():TextField
		{
			var txt:TextField = new TextField();
			txt.height = 20;
			txt.y = 2;
			txt.mouseEnabled = false;
			txt.autoSize = TextFieldAutoSize.CENTER;
			return txt;
		}
		
		private function removeStageHandler(evt:Event):void
		{
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			this.removeEventListener( MouseEvent.MOUSE_OVER,overHandler );
			this.removeEventListener( MouseEvent.MOUSE_OUT,outHandler );
			this.removeEventListener( MouseEvent.CLICK,clickHandler );
			manuProxy = null;
			
			while ( this.numChildren>0 )
			{
				var obj:Object = this.removeChildAt(0);
				obj = null;
			}
		}
		
	}
}