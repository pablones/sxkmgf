/**
 * ---- 日常表任务组件 ----- 
 * 
 */
package GameUI.Modules.Campaign.Mediator.UI
{
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Campaign.Mediator.CalendarMediator;
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;

	public class UICalendarTitle extends Sprite
	{
		/**
		 * 组件的视图
		 */
		private var _mainView:MovieClip;
		/**
		 * 图标名称容器 
		 */
		private var iconSprite:FaceItem;
		/**
		 * 数据 
		 */
		private var _dataRro:Array;
		/**
		 * 图标的位置 
		 */
		private const POINT:Point = new Point(33 , 35); 
		/**
		 * 序列号
		 */
		private var _index:int;
		/**
		 * 任务是否完成 
		 */
		private var _taskIsFinish:Boolean = false;
		/**
		 * 任务可得经验
		 */
		private var _taskExp:int;
				/**
		 * 任务可得碎银
		 */
		private var _taskMoneyS:int;
				/**
		 * 任务可得银两
		 */
		private var _taskMoneyY:int;
		public function UICalendarTitle(mainView:MovieClip)
		{
			super();
			this._mainView = new mainView.constructor();
			this.createChildren();
			
			this._mainView.txtTitle.mouseEnabled = false;
			this._mainView.txtTartet.mouseEnabled = false;
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
			_mainView.mcFinish.visible = vaule;
			_mainView.txtFastFinish.visible = !vaule;
			_mainView.txtFastFinish.mouseEnabled = false;
			_mainView.btnFastFinish.visible = !vaule;
			if(vaule == false)		//没完成任务
			{
				_mainView.btnFastFinish.addEventListener(MouseEvent.CLICK , fastFinishHandler);
			}
			else _mainView.btnFastFinish.removeEventListener(MouseEvent.CLICK , fastFinishHandler);
			
			//领取奖励后，快速完成消失
			if((GameCommonData.UIFacadeIntance.retrieveMediator(CalendarMediator.NAME) as CalendarMediator).isGetAward)		//奖励已经领取的话
			{
				_mainView.btnFastFinish.visible = false;
				_mainView.txtFastFinish.visible = false;
			}
			else
			{
				_mainView.btnFastFinish.visible = true;
				_mainView.txtFastFinish.visible = true;
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
			this.addChild(_mainView);
			taskIsFinish = false;
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
			_mainView.txtTitle.text 		= _dataRro[0];					//标题
			_mainView.txtTartet.text 		= _dataRro[2];					//目标
			this._taskExp = getExp(_dataRro[3]);
			if(this._taskExp > 0)  //有经验情况下
				_mainView.txtAwardExp.text 	= this._taskExp + GameCommonData.wordDic[ "often_used_exp" ];			//经验奖励
			else 
				_mainView.txtAwardExp.text 	= "";			//经验奖励
			getMoney(_dataRro[4]);		//金钱奖励
			if(iconSprite == null) iconSprite = new FaceItem(_dataRro[1]);					//图标ID
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
			this._taskMoneyS = 0;
			this._taskMoneyY = 0;
			if(s.search("Y") > -1)  s = s.split("Y")[1];
			if(s.search("S") > -1)
			{
				var sList:Array = s.split("S");
				this._taskMoneyS = int(sList[1]);
				this._taskMoneyY = int(sList[0]);
			}
			if(this._taskMoneyS > 0 && this._taskMoneyY == 0)		//只有碎银
			{
				_mainView.txtAwardMoney_S.text = int(this._taskMoneyS / 10000);
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = true;
				_mainView.mcMoney_S.gotoAndStop(1);
				_mainView.txtAwardExp.y = 59;
			}
			else if(this._taskMoneyY > 0 && this._taskMoneyS == 0)	//只有银两
			{
				_mainView.txtAwardMoney_S.text = int(this._taskMoneyY / 10000);
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = true;
				_mainView.mcMoney_S.gotoAndStop(2);
				_mainView.txtAwardExp.y = 59;
			}
			else if(this._taskMoneyS > 0 && this._taskMoneyY > 0)	//都有
			{
				_mainView.txtAwardMoney_S.text = int(this._taskMoneyS / 10000);
				_mainView.txtAwardMoney_Y.text = int(this._taskMoneyY / 10000);
				_mainView.mcMoney_Y.visible = true;
				_mainView.mcMoney_S.visible = true;
				_mainView.mcMoney_Y.gotoAndStop(2);
				_mainView.mcMoney_S.gotoAndStop(1);
				_mainView.txtAwardExp.y = 41;
			}
			else
			{
				_mainView.txtAwardMoney_S.text = "";
				_mainView.txtAwardMoney_Y.text = "";
				_mainView.mcMoney_Y.visible = false;
				_mainView.mcMoney_S.visible = false;
				_mainView.txtAwardExp.y = 59;
			}
		}
		/** 得到奖励经验数值 */
		private function getExp(vaule:Number):int
		{
			var exp:int = 0;
			if(vaule == 0) return exp;
			exp = vaule*(332* GameCommonData.Player.Role.Level + 6000);		

			return exp;
		} 
		/** 快速完成任务 */
		private function fastFinishHandler(e:MouseEvent):void
		{
			GameCommonData.UIFacadeIntance.sendNotification(CampaignData.SHOWFASTFINISH , this._index);
		}
		/**
		 * 得到经验
		 *  
		 */
		public function get taskExp():int
		{
			return this._taskExp;
		}
		/**
		 * 得到碎银 
		 * 
		 */
		public function get taskMoneyS():int
		{
			return this._taskMoneyS;
		}
		/**
		 * 得到银两 
		 * 
		 */
		public function get taskMoneyY():int
		{
			return this._taskMoneyY;
		}
	}
}