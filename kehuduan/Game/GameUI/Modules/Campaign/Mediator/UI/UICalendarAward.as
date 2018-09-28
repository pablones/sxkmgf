package GameUI.Modules.Campaign.Mediator.UI
{
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFormat;

	public class UICalendarAward extends Sprite
	{
		/**
		 * 组件的视图
		 */
		private var _mainView:MovieClip;
		/**
		 * 图标名称容器 
		 */
		private var iconSprite:Sprite;
		/**
		 * 数据 
		 */
		private var _dataRro:Array;
		/**
		 * 图标的位置 
		 */
		private const POINT:Point = new Point(8 , 38); 
		/**
		 * 序列号
		 */
		private var _index:int;
		/**
		 * 任务是否完成 
		 */
		private var _taskIsFinish:Boolean = false;
		/**
		 * 组件背景 
		 */
		private var _back:Shape;

		public function UICalendarAward(mainView:MovieClip)
		{
			super();
			this._mainView = new mainView.constructor();
			this.createChildren();
			
			this._mainView.mouseChildren = true;
			this._mainView.txtFinish.mouseEnabled = false;
			this._mainView.txtAwardExp.mouseEnabled = false;
			this._mainView.txtAwardMoney_Y.mouseEnabled = false;
			this._mainView.txtAwardMoney_S.mouseEnabled = false;
		}
		/**
		 * 设置数据 
		 * 
		 */		
		public function set dataPro(value:Array):void
		{
			this._dataRro = value;
			this.toRepaint();
		}
		/**
		 * 设置任务是否完成
		 * 
		 */
		public function set taskIsFinish(vaule:Boolean):void
		{
			this._taskIsFinish = vaule;
			if(vaule)
			{
				_mainView.txtFinish.textColor 		= 0x00FF00;
				_mainView.txtAwardExp.textColor 			= 0xE2CCA5;
				_mainView.txtAwardMoney_S.textColor = 0xE2CCA5;
				_mainView.txtAwardMoney_Y.textColor = 0xE2CCA5;
				_mainView.mcSelect.visible = true;
				if(_dataRro[2] == null)		//无物品
					_mainView.mcSelect.gotoAndStop(1);
				else
					_mainView.mcSelect.gotoAndStop(2);
			}
			else
			{
				_mainView.txtFinish.textColor 		= 0x666666;
				_mainView.txtAwardExp.textColor 			= 0x666666;
				_mainView.txtAwardMoney_S.textColor = 0x666666;
				_mainView.txtAwardMoney_Y.textColor = 0x666666;
				_mainView.mcSelect.visible = false;
			}
		}
		/**
		 * 得到任务是否完成 
		 */
		public function get taskIsFinish():Boolean
		{
			return _taskIsFinish;
		}
		/**
		 * 设置序列号
		 * 
		 */
		public function set index(vaule:int):void
		{
			_index = vaule;
		}
		/**
		 * 得到序列号 
		 */
		public function get index():int
		{
			return _index;
		}
		/**
		 * 创建子容器
		 *  
		 */
		private function createChildren():void
		{
			iconSprite = new Sprite();
			iconSprite.mouseChildren = true;
			_back = new Shape();
			_back.graphics.lineStyle(0 , 0 , 0);
			_back.graphics.drawRect(0 , 0 , 232 , 0);
			_back.graphics.endFill();
			this.addChild(_back);
			this.addChild(_mainView);
		} 
		/**
		 * 重绘
		 * 
		 */
		private function toRepaint():void
		{
			this.removeAllCells();
			this.createCells();
			this.doLayout();
		}
		/**
		 * 清除渲染
		 *  
		 */
		private function removeAllCells():void
		{
			
		}
		/**
		 * 创建渲染 
		 * 
		 */
		private function createCells():void
		{
			taskIsFinish = false;
			_mainView.txtFinish.text 				= _index + GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+"      ：";//个
			if(getExp(_dataRro[0]) > 0)  //有经验情况下
				_mainView.txtAwardExp.text 	= getExp(_dataRro[0]) + GameCommonData.wordDic[ "often_used_exp" ];	//"经验"		//经验奖励,参数没用到
			getMoney(_dataRro[1]);		//金钱奖励
			if(_dataRro[2] == null) return;
			getItemIcon(_dataRro[2]);										//得到物品图标
		}
		/**
		 * 布局 
		 * 
		 */
		private function doLayout():void
		{
			_mainView.addChild(iconSprite);
			iconSprite.x = POINT.x;
			iconSprite.y = POINT.y;
			
			var posy:int = 45 + iconSprite.height;
			_mainView.txtAwardExp.y 			= posy;
			_mainView.txtAwardMoney_S.y = posy;
			_mainView.txtAwardMoney_Y.y = posy;
			_mainView.mcMoney_S.y 		= posy;
			_mainView.mcMoney_Y.y 		= posy;
		}
		/** 得到银两 或 碎银 */
		private function getMoney(s:String):void
		{
			if(s == "")
			{
				_mainView.txtAwardMoney_S.text = "";
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = false;
				return;
			} 
			var sy:int = 0;
			var yl:int= 0;
			if(s.search("Y") > -1)  s = s.split("Y")[1];
			if(s.search("S") > -1)
			{
				var sList:Array = s.split("S");
				sy = int(sList[1]);
				yl = int(sList[0]);
			}
			if(sy > 0 && yl == 0)		//只有碎银
			{
				_mainView.txtAwardMoney_S.text = int(sy / 10000);
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = true;
				_mainView.mcMoney_S.gotoAndStop(1);
			}
			else if(yl > 0 && sy == 0)	//只有银两
			{
				_mainView.txtAwardMoney_S.text = int(yl / 10000);
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = true;
				_mainView.mcMoney_S.gotoAndStop(2);
			}
			else if(sy > 0 && yl > 0)	//都有
			{
				_mainView.txtAwardMoney_S.text = int(sy / 10000);
				_mainView.txtAwardMoney_Y.text = int(yl / 10000);
				_mainView.mcMoney_Y.visible = true;
				_mainView.mcMoney_S.visible = true;
				_mainView.mcMoney_S.gotoAndStop(1);
				_mainView.mcMoney_Y.gotoAndStop(2);
			}
			else
			{
				_mainView.txtAwardMoney_S.text = "";
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = false;
			}
		}
		/** 得到奖励经验数值 */
		private function getExp(vaule:Number):int
		{
			var exp:int = 0;
			if(vaule == 0) return exp;
			exp = Math.pow(_index , 1.2) * (1000*GameCommonData.Player.Role.Level+24000);		

			return exp;
		}
		/** 得到物品 */
		private function getItemIcon(arr:Array):void
		{
			var len:int = arr.length;
			for(var i:int = 0; i < len; i++)
			{
				var s:String = arr[i];
				var num:String = s.charAt(0);
				var id:String  = s.substring(1 , s.length);
				var icon:FaceItem = new FaceItem(id);					//图标ID
				var grid:MovieClip = new CampaignData.calendarGrid_mc.constructor();
				grid.name = "goodQuickBuy_" + id;
				iconSprite.addChild(grid);
				iconSprite.addChild(icon);
				icon.x = 1 + int(i % 4) * 50;
				icon.y = 1 + int(i / 4) * 45;
				grid.x = 0 + int(i % 4) * 50;
				grid.y = 0 + int(i / 4) * 45;
				
				if(int(num) > 1)
				{
					var txtNum:TextField = new TextField();					//个数文本
					txtNum.mouseEnabled = false;
					txtNum.setTextFormat(new TextFormat(GameCommonData.wordDic["mod_cam_med_ui_UIC1_getI"] , 12));
					txtNum.textColor = 0xFFFFFF;
					txtNum.filters = Font.Stroke(0x000000);
					txtNum.wordWrap = false;
					txtNum.width  = 18;
					txtNum.text = num;
					iconSprite.addChild(txtNum);
					txtNum.x = grid.x+ grid.width - txtNum.textWidth - 6;
					txtNum.y = grid.y+ grid.height - txtNum.textHeight - 4;
					txtNum.height = 18;
				}
				
			}
		} 	
		
	}
}
