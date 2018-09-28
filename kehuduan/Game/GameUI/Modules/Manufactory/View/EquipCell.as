package GameUI.Modules.Manufactory.View
{
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Manufactory.Command.CheckCommitBtnCommand;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.UISprite;
	
	import flash.display.DisplayObject;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class EquipCell extends UISprite
	{
		private var dataObj:Object;
		private var eName:String;
		private var eKind:String;
		private var eLevel:String;
		private var name_txt:TextField;
		private var kind_txt:TextField;
		private var level_txt:TextField;
		private var w:uint = 298;
		private var h:uint = 20;
		private var aNeedBase:Array = [];					//所需要的基础材料
		private var numArr:Array;								//用来计算打造数量的数组
		
		public function EquipCell(obj:Object)
		{
			super();
			this.mouseEnabled = true;
			dataObj = obj;
			initData();
			initUI();
		}
		
		//外部接口 设置高亮
		public function highLight(shape:Shape):void
		{
			this.addChildAt( shape,0 );
		}
		
		//外部接口，计算材料能打造多少个
		public function chgColor():void
		{
			var num:uint = countBuildNum();
			name_txt.text = "";
			if ( num==0 )
			{
				name_txt.text = eName;
				name_txt.textColor = 0xff0000;
				kind_txt.textColor = 0xff0000;
				level_txt.textColor = 0xff0000;
			}
			else
			{
				name_txt.text = eName+" × "+num;
				name_txt.textColor = 0x00ff00;
				kind_txt.textColor = 0x00ff00;
				level_txt.textColor = 0x00ff00;
			}
		}
		
		public function countBuildNum():uint
		{
			numArr = [];
			for ( var i:uint=0; i<aNeedBase.length; i++ )
			{
				var hasNum:uint = BagData.hasItemNum( aNeedBase[i].type );
				var needNum:uint = aNeedBase[i].num;
				numArr.push( Math.floor( hasNum/needNum ) );
			} 	
			if ( numArr.length>0 )
			{
				numArr.sort( Array.NUMERIC );
				return numArr[0];
			}
			return 0;
		}
		
		private function initData():void
		{
			eName = dataObj.eName;
			eKind = dataObj.kind;
			eLevel = dataObj.level;
			aNeedBase = dataObj.needBase;
		}
		
		private function initUI():void
		{
			this.width = w;
			this.height = h;
			
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function addStageHandler(evt:Event):void
		{
			creatTexts();
			chgColor();
			this.addEventListener( MouseEvent.MOUSE_OVER,overHandler );
			this.addEventListener( MouseEvent.MOUSE_OUT,outHandler );
			this.addEventListener( MouseEvent.CLICK,clickHandler );
			this.addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function clickHandler(evt:MouseEvent):void
		{
			var obj:Object = new Object();
			obj.dataObj = this.dataObj;
			obj.equipCell = this;
//			if ( !ManufactoryData.isReadingBar )
//			{
//				ManufactoryData.selectScenoType = dataObj.id;	
//			}
			ManufactoryData.clickScenoType = dataObj.id;
//			trace ( "ManufactoryData.clickScenoType  "+ManufactoryData.clickScenoType );
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( CheckCommitBtnCommand.NAME );
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( ManufactoryData.SELECT_EQUIP_MANUFA,obj );
		}
		
		private function overHandler(evt:MouseEvent):void
		{
			this.graphics.clear();
			this.graphics.beginFill(0x4d3c13,.7);
			this.graphics.drawRect(0,0,w,h);
			this.graphics.endFill();
		}
		
		private function outHandler(evt:MouseEvent):void
		{
			this.graphics.clear();
		}
		
		private function creatTexts():void
		{
			var format:TextFormat = new TextFormat();
			format.font = GameCommonData.wordDic["mod_cam_med_ui_UIC1_getI"];//"宋体";
			format.size = 12;
			format.color = 0xffffff;
			
			name_txt = new TextField();
			name_txt.x = 14;
			name_txt.y = 0;
			name_txt.height = 20;
			name_txt.width = 110;
			name_txt.autoSize = TextFieldAutoSize.CENTER;
			name_txt.mouseEnabled = false;
			name_txt.text = eName;
			name_txt.setTextFormat( format );
			this.addChild( name_txt );	

			
			kind_txt = new TextField();
			kind_txt.x = 127;
			kind_txt.y = 2;
			kind_txt.height = 20;
			kind_txt.width = 76;
			kind_txt.autoSize = TextFieldAutoSize.CENTER;
			kind_txt.mouseEnabled = false;
			kind_txt.text = eKind;
			kind_txt.setTextFormat( format );
			this.addChild( kind_txt );
			
			level_txt = new TextField();
			level_txt.x = 213;
			level_txt.y = 2;
			level_txt.height = 20;
			level_txt.width = 76;
			level_txt.autoSize = TextFieldAutoSize.CENTER;
			level_txt.mouseEnabled = false;
			level_txt.text = eLevel;
			level_txt.setTextFormat( format );
			this.addChild( level_txt );
		}
		
		private function removeStageHandler(evt:Event):void
		{
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			this.removeEventListener( MouseEvent.MOUSE_OVER,overHandler );
			this.removeEventListener( MouseEvent.MOUSE_OUT,outHandler );
			this.removeEventListener( MouseEvent.CLICK,clickHandler );
			while ( this.numChildren>1 )
			{
				var obj:DisplayObject = this.removeChildAt( 1 );
				obj = null;
			}
			
			if ( ManufactoryData.selectScenoType == dataObj.id )
			{
//				ManufactoryData.selectScenoType = 0;
				UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( CheckCommitBtnCommand.NAME );
			}
//			trace ( "ManufactoryData.selectScenoType  "+ManufactoryData.selectScenoType );
		}
		
	}
}