package GameUI.Modules.TimeCountDown.Mediator
{
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownData;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownEvent;
	import GameUI.Modules.Unity.Data.UnityConstData;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TimeCountDownMediator extends Mediator implements IUpdateable
	{
		public static const NAME:String = "TimeCountDownMediator";
		
		private const POINT:Point = new Point(830 , 490);
		private var enabled:Boolean = true;
		private var name:String;					/** 倒计时的标题 */
		private var time:int;						/** 倒计时的总时间 */
		private var timer:Timer = new Timer();		/** 定时器 */
		public function TimeCountDownMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					TimeCountDownEvent.SHOWTIMECOUNTDOWN,
					TimeCountDownEvent.CLOSETIMECOUNTDOWN,
					TimeCountDownEvent.CLOSEWORKCOUNTDOWN
					];
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case TimeCountDownEvent.SHOWTIMECOUNTDOWN:
					var controllerNum:int = (notification.getBody()as Object).taskId;
					var type:int = int(controllerNum.toString().slice(0 ,1));
					if(type == 1)  name = GameCommonData.wordDic[ "mod_tim_med_tim_han_1" ];      //副本
					else if(type == 2) name = GameCommonData.wordDic[ "mod_tim_med_tim_han_2" ];        //打工
					time = int(controllerNum.toString().slice(1));
					var view:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TimeCountDownView");
					if(!GameCommonData.GameInstance.GameUI.contains(TimeCountDownData.timeViewContent)) GameCommonData.GameInstance.GameUI.addChild(TimeCountDownData.timeViewContent);
					init(view);
					addView(view);
				break;
				case TimeCountDownEvent.CLOSETIMECOUNTDOWN:
					//手动删除接口 
					//副本
					if(GameCommonData.GameInstance.GameScene.GetGameScene.MapId != GameCommonData.GameInstance.GameScene.GetGameScene.name)	return;	//刚进副本
					deleteFb();
				break;
				case TimeCountDownEvent.CLOSEWORKCOUNTDOWN:	//删除打工
					deleteWork();
				break;
			}
		}
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
		
		private function addView(view:MovieClip):void
		{
			timer.DistanceTime = 1000 * 1;		//循环时间为1秒钟
			TimeCountDownData.timeViewContent.addChild(view);
			rankTxt();
			view.name = String(time);
			view.txtName.text = name + GameCommonData.wordDic[ "mod_tim_med_tim_add_1" ];          //倒计时
			if(TimeCountDownData.timeViewContent.numChildren == 1) GameCommonData.GameInstance.GameUI.Elements.Add(this);			//第一次才会添加心跳
			showText(view);
		}
		private function gcAll(mc:MovieClip):void
		{
			if(mc.txtName.text == GameCommonData.wordDic[ "mod_tim_med_tim_gca_1" ])   UnityConstData.isWorking = false;         //打工倒计时
			TimeCountDownData.timeViewContent.removeChild(mc);
			mc = null;
			if(TimeCountDownData.timeViewContent.numChildren == 0) GameCommonData.GameInstance.GameUI.Elements.Remove(this);		//全部倒计时删除才会删除心跳
			rankTxt();
		}
		private function init(view:MovieClip):void
		{
			view.mouseEnabled = false;
			view.txtTime.mouseEnabled = false;
			view.txtName.mouseEnabled = false;
			TimeCountDownData.timeViewContent.mouseEnabled = false;
		} 
		
		
		public function get Enabled():Boolean
		{
			return enabled;
		}
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				updateTime(gameTime);
			}
		}
		/** 更新时间 */
		private function updateTime(gameTime:GameTime):void
		{
			for(var i:int = 0 ; i < TimeCountDownData.timeViewContent.numChildren ; i++)
			{
				var mc:MovieClip = TimeCountDownData.timeViewContent.getChildAt(i) as MovieClip;
				mc.name = String(int(mc.name) - 1);
				showText(MovieClip(TimeCountDownData.timeViewContent.getChildAt(i)));
				if(int(mc.name) == 0) gcAll(mc); 
			}
		}
		/** 显示时间文本 */
		private function showText(mc:MovieClip):void
		{
			var miaoStr:String;
			if(int(mc.name) >= 1800)
			{
				var hour:int =  int(mc.name) / 3600;
			}
			var fen  : int = int(int(mc.name) % 3600) / 60;
			var miao : int = int(mc.name) % 60;
			if(miao < 10) miaoStr = "0" + miao;
			else  miaoStr = String(miao);
			mc.txtTime.text = (hour == 0) ? fen + ":" + miaoStr : hour + ":" + fen + ":" + miaoStr; 
		}
		/** 位置排序 */
		private function rankTxt():void
		{
			for(var i:int = 0;i < TimeCountDownData.timeViewContent.numChildren ; i++)
			{
				TimeCountDownData.timeViewContent.getChildAt(i).x =	POINT.x - i * TimeCountDownData.timeViewContent.getChildAt(i).width;
				TimeCountDownData.timeViewContent.getChildAt(i).y = POINT.y;
			}
		}
		/** 删除副本 */
		private function deleteFb():void
		{
			var deleteMc:MovieClip;
			for(var i:int = 0; i <  TimeCountDownData.timeViewContent.numChildren ; i++)
			{
				deleteMc = TimeCountDownData.timeViewContent.getChildAt(i) as MovieClip;
				if(deleteMc.txtName.text == GameCommonData.wordDic[ "mod_tim_med_tim_del_1" ])           //副本倒计时
				{
					gcAll(deleteMc);
				}
			}
		}
		/** 删除打工 */
		private function deleteWork():void
		{
			var deleteMc:MovieClip;
			for(var i:int = 0; i <  TimeCountDownData.timeViewContent.numChildren ; i++)
			{
				deleteMc = TimeCountDownData.timeViewContent.getChildAt(i) as MovieClip;
				if(deleteMc.txtName.text == GameCommonData.wordDic[ "mod_tim_med_tim_gca_1" ])           //打工倒计时
				{
					gcAll(deleteMc);
				}
			}
		}
	}
}