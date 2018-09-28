package GameUI.Modules.Manufactory.View
{
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;

	public class BaseMaterialCell extends Sprite
	{
		private var cosumeNum:uint;					//消耗数量
		private var base_txt:TextField;
		private var hasNum_txt:TextField;
		private var needNum_txt:TextField;
		private var format:TextFormat;
		private var dataObj:Object;
		
		private var bName:String;
		private var needNum:uint;
		private var hasNum:uint;
		private var baseType:uint;
		
		public function BaseMaterialCell(obj:Object)
		{
			super();
			dataObj = obj;
			initData();
			initUI();
		}
		
		private function initData():void
		{
			bName = dataObj.name;
			needNum = dataObj.num;
			baseType = dataObj.type;
			hasNum = BagData.hasItemNum( baseType );
		}
		
		private function initUI():void
		{
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function addStageHandler(evt:Event):void
		{
			initTexts();
			this.addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function initTexts():void
		{
			format = new TextFormat();
			format.size = 12;
			format.font = GameCommonData.wordDic["mod_cam_med_ui_UIC1_getI"];//"宋体";
//			format.color = 0xffffff;
			
			base_txt = this.creatText();
			base_txt.textColor = 0xffffff;
			base_txt.text = bName;
			base_txt.x = 0;
			base_txt.width = 87;
			base_txt.setTextFormat( format );
			this.addChild( base_txt );
			
			needNum_txt = this.creatText();
			needNum_txt.textColor = 0xffffff;
			needNum_txt.x = 189;
			needNum_txt.width = 85;
			needNum_txt.text = (needNum*ManufactoryData.ManufatoryCount).toString();
			needNum_txt.setTextFormat( format );
			this.addChild( needNum_txt );
			
			hasNum_txt = this.creatText();
			hasNum_txt.x = 92;
			hasNum_txt.width = 85;
			////////////////////////////////////////
			hasNum_txt.x = 92;
			hasNum_txt.width = 90;
			////////////////////////////////////////
			hasNum_txt.text = hasNum.toString();
			hasNum_txt.setTextFormat( format );
			this.addChild( hasNum_txt );
			checkIsEnough();
		}
		
		//判断数量是否足够   外部调用接口
		public function checkIsEnough():void
		{
			hasNum = BagData.hasItemNum( baseType );
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
		
		private function removeStageHandler(evt:Event):void
		{
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			try
			{
				this.removeChild( base_txt );
				this.removeChild( hasNum_txt );
				this.removeChild( needNum_txt );
				base_txt = null;
				hasNum_txt = null;
				needNum_txt = null;	
			}
			catch (e:Error)
			{
				
			}
		}
		
		private function creatText():TextField
		{
			var txt:TextField = new TextField();
			txt.height = 20;
			txt.mouseEnabled = false;
			txt.autoSize = TextFieldAutoSize.CENTER;
			return txt;
		}
		
	}
}