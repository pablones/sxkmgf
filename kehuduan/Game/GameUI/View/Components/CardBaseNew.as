package GameUI.View.Components
{
	import GameUI.UIUtils;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.*;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	public class CardBaseNew extends MovieClip
	{
		private var cardBase:MovieClip;
		private var txtExp:TextField;
		public var initCardFun:Function;
		private var explainData:Dictionary;
		private var _showNumber:int;
		public function CardBaseNew()
		{
			init();
		} 
		
		private function init():void
		{
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/CardBase.swf",loadComplete);
		}			
		
		private function loadComplete():void
		{
			var tempMc:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/CardBase.swf");
			explainData = tempMc.NewExpData;
			if(!tempMc)
			{
				throw new Error("无法实例化或未找到资源");
			}
			cardBase = new (tempMc.loaderInfo.applicationDomain.getDefinition("cradComponent"))();
			this.addChild(cardBase);
			cardBase.x = 3;
			initTxtState(); 
			if(initCardFun != null)
			{
				initCardFun();
			}
		}
		public function getSendData(showNum:int):Object
		{
			return explainData[showNum].sendData;
		}
		private function initTxtState():void
		{ 
			(cardBase.txt2 as TextField).mouseEnabled = false;
			(cardBase.txt2 as TextField).wordWrap = false;
			(cardBase.txt3 as TextField).mouseWheelEnabled = false;
			(cardBase.txt3 as TextField).multiline = false;
			(cardBase.txt4 as TextField).mouseEnabled = false;
			if(cardBase.send_txt) (cardBase.send_txt as TextField).mouseEnabled = false;
			if(cardBase.cancel_txt) (cardBase.cancel_txt as TextField).mouseEnabled = false;
			
		}
		public function set isHasTxt1(boo:Boolean):void
		{
			if(!boo)
			{
				if(txtExp)
				{
					this.removeChild(txtExp)
					txtExp = null;
					this.cardBase.y = 5;
					setBackGround();
				}	
			}
		}
		/**
		 *调用swf里面的文字描述，与弹窗编号对应 
		 * @param showNumber
		 * 
		 */		
		public function set expData(showNumber:int):void
		{
			this._showNumber = showNumber;
//			var data:Array = this.explainData[showNumber];
			var data:Array = this.explainData[showNumber].showData;
			if(!data) return;
			txt1 = data[1];
			txt2 = data[2];
			txt4 = data[3];
			setBackGround();
		}
		/**
		 * 获得名称 
		 * @return 
		 * 
		 */		
		public function get cardName():String
		{
			return explainData[_showNumber].showData[0];
		}
		/**
		 * 设置txt1的文本 
		 * @param htmlTxt
		 * 
		 */		
		public function set txt1(htmlTxt:String):void
		{
			if(txtExp)
			{
				this.removeChild(txtExp);
				txtExp = null;
			}
			if(!htmlTxt) return;
			txtExp = new TextField();
			txtExp.width = 293; 
			txtExp.textColor = 0x00FF00;
			this.addChild(txtExp);
			txtExp.mouseEnabled = false;
			txtExp.multiline = true;
			txtExp.wordWrap = true;
			txtExp.htmlText = htmlTxt;
			txtExp.height = txtExp.numLines*15+10;
			txtExp.x = 2;
			txtExp.x = 3;
		}
		
		/**
		 * 设置txt1的高度 
		 * @param htmlTxt
		 * 
		 */
		public function set txt1Height(txtHeight:int):void
		{
			txtExp.height = txtHeight;
		}
		/**
		 * 设置txt1的焦点
		 * @param htmlTxt
		 * 
		 */
		public function set setTxtFocus(boo:Boolean):void
		{
			if(boo)
			{
				GameCommonData.GameInstance.stage.focus = cardBase.txt3;
				UIUtils.addFocusLis(cardBase.txt3);
			}
			else
			{
				GameCommonData.GameInstance.stage.focus = null;
				UIUtils.removeFocusLis(cardBase.txt3);
			}
		}
		
		public function set cancleTxtFocus(boo:Boolean):void
		{
			UIUtils.removeFocusLis(cardBase.txt3);
		}
		public function set sureTxt(txt:String):void
		{
			cardBase.send_txt.text = txt;
		}
		public function set cancelTxt(txt:String):void
		{
			cardBase.cancel_txt.text = txt;
		}
		/**
		 * 
		 * @return 输入的文本内容字符串 
		 * 
		 */		
		public function get inputContent():String
		{
			return cardBase.txt3.text;
		}
		/**
		 * 设置txt2的文本 
		 * @param htmlTxt 
		 * 
		 */
		public function set txt2(htmlTxt:String):void
		{
			if(!htmlTxt)
				htmlTxt = "";
			cardBase.txt2.htmlText = htmlTxt;
		}
		public function set txt2Length(len:int):void
		{
			(cardBase.txt3 as TextField).maxChars = len;
		}
		/**
		 * 设置txt3的文本 
		 * @param htmlTxt
		 * 
		 */
		public function set txt3(htmlTxt:String):void
		{
			if(!htmlTxt)
				htmlTxt = "";
			cardBase.txt3.htmlText = htmlTxt;
		}
		/**
		 * 设置txt4的文本 
		 * @param htmlTxt
		 * 
		 */
		public function set txt4(htmlTxt:String):void
		{
			if(!htmlTxt)
				htmlTxt = "";
			cardBase.txt4.htmlText = htmlTxt;
		}
		/**
		 *获取确定按钮 
		 * @return 
		 * 
		 */		
		public function get sureBtn():SimpleButton
		{
			return cardBase.send_btn;
		}
		/**
		 * 获取取消按钮 
		 * @return 
		 * 
		 */		
		public function get cancleBtn():SimpleButton
		{
			return cardBase.cancel_btn;
		}
		private function setBackGround():void
		{
			if(txtExp)
			{
				cardBase.y = txtExp.y + txtExp.height + 5;
			}
			else 
			{
				cardBase.y = 0;
			}
			this.graphics.clear();
			this.graphics.beginFill(0x000000);
			this.graphics.lineStyle(1,0x4D3C13);
			this.graphics.drawRect(0,0,this.width+2,this.height+8);
			this.graphics.endFill();
		}
	}
}