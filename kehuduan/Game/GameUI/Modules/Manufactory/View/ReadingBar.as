package GameUI.Modules.Manufactory.View
{
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.EquipSend;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class ReadingBar extends Sprite implements IUpdateable
	{
		private var timer:Timer = new Timer();
		private var main_mc:MovieClip;
		private var speed:Number = 1;
		
		public var isManuing:Function;				//打造中函数
		public var manuClose:Function;				//终止打造
		
		public function ReadingBar(mainMC:MovieClip)
		{
			super();
			main_mc = mainMC;
			initUI();
		}
		
		public function go():void
		{
			//屏蔽掉所有操作
			canHandle(false);
			
			( main_mc.scale_mc as MovieClip ).scaleX = 0;
			ManufactoryData.isReadingBar = true;
			if ( isManuing != null )
			{
				isManuing();
			}
//			ManufactoryData.selectScenoType = ManufactoryData.clickScenoType;
//			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( ManufactoryData.IS_MANU_ING );
			GameCommonData.GameInstance.GameUI.Elements.Add( this );
		}
		
		public function stop():void
		{
			( main_mc.scale_mc as MovieClip ).scaleX = 0;
			GameCommonData.GameInstance.GameUI.Elements.Remove( this );	
			ManufactoryData.isReadingBar = false;
//			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( ManufactoryData.CLOSE_MANU_ING );
			canHandle(true);
			if ( manuClose != null )
			{
				manuClose();
			}
		}
		
		public function Update(gameTime:GameTime):void
		{
			if ( main_mc.scale_mc.width<175 )
			{
				( main_mc.scale_mc as MovieClip ).width += speed;
			}
			else
			{
				( main_mc.scale_mc as MovieClip ).width = 175;
				//发送数据给服务器
				var dataArr:Array = [ 0,5,ManufactoryData.selectScenoType,ManufactoryData.selectAppendType ];
				EquipSend.createMsgCompound( dataArr );
				GameCommonData.GameInstance.GameUI.Elements.Remove( this );	
			}
		}
		
		private function initUI():void
		{
			timer.DistanceTime = 800;
			( main_mc.scale_mc as MovieClip ).width = 175;
			( main_mc.scale_mc as MovieClip ).scaleX = 0;
			this.addChild( main_mc );
		}
		
		private function canHandle(isHand:Boolean):void
		{
//			GameCommonData.GameInstance.GameUI.mouseChildren = isHand;
//			GameCommonData.GameInstance.GameUI.mouseEnabled = isHand;
			GameCommonData.GameInstance.GameScene.mouseChildren = isHand;
			GameCommonData.GameInstance.GameScene.mouseEnabled = isHand;
//			main_mc.parent.mouseChildren = isHand;
//			main_mc.parent.mouseEnabled = isHand;
		}
		
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
        
        public function get Enabled():Boolean
		{
			return true;
		}
		
	}
}