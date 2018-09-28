package GameUI.Modules.AutoPathFind.View
{
	import Controller.PlayerController;
	
	import GameUI.Modules.AutoPathFind.Datas.AutoPathData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.UICore.UIFacade;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;

	public class AutoPathCell extends Sprite
	{
		private var format:TextFormat = null;
		private var txtContainer:Sprite = null;
		
		private var txt1:TextField = null;
		private var txt2:TextField = null;
		private var id:uint;
		
		private var tag:String;
		private var remark:String;
		private var xPos:uint;
		private var yPos:uint;
		private var isNpc:Boolean;
		
		public function AutoPathCell(_id:uint)
		{
			super();
			id = _id;
			initData();
			initUI();
		}
		
		private function initData():void
		{
			var obj:Object = AutoPathData.autoPathDic[id] as Object;
			tag = obj.Name;
			remark = obj.Remark;
			xPos = obj.X;
			yPos = obj.Y;
			if(obj.Type == 1)
			{
				isNpc = false;
			}else
			{
				isNpc = true;
			}
		}
		
		private function initUI():void
		{
			txtContainer = new Sprite();
			this.addChild(txtContainer);
			txtContainer.addEventListener(Event.ADDED_TO_STAGE,addHandler);
		}
		
		private function addHandler(evt:Event):void
		{
			showTxt();
			txtContainer.addEventListener(Event.REMOVED_FROM_STAGE,removeHandler);
		}
		
		
		private function showTxt():void
		{
			format = new TextFormat();
			format.size = 12;
			format.align = TextFormatAlign.CENTER;
			
			txt1 = new TextField();
			txt1.x = 4;
			txt1.width = 100;
			txt1.text = tag;
			txt1.textColor = 0xffffff;
			txt1.height = 16;
			txt1.setTextFormat(format);
			
			txt2 = new TextField();
			txt2.x = 110;
			txt2.width = 30;
			txt2.height = 16;
			txt2.textColor = 0xffffff;
			txt2.text = remark;
			txt2.setTextFormat(format);
			
			txtContainer.addChild(txt1);
			txtContainer.addChild(txt2);
			txtContainer.height = 17;
			txtContainer.width = 160;
			txtContainer.mouseChildren = false;
			txtContainer.addEventListener(MouseEvent.CLICK,go);
			txtContainer.addEventListener(MouseEvent.MOUSE_OVER,overHandler);
			txtContainer.addEventListener(MouseEvent.MOUSE_OUT,outHandler);
		}
		
		private function overHandler(evt:MouseEvent):void
		{
			txt1.textColor = 0xFFCC00; 
			txt2.textColor = 0xFFCC00;
		}
		
		private function outHandler(evt:MouseEvent):void
		{
			txt1.textColor = 0xffffff;
			txt2.textColor = 0xffffff;
		}
		
		private function go(evt:MouseEvent):void
		{
			//自动寻路的时候停止挂机
			if(GameCommonData.Player.IsAutomatism)
            {
				PlayerController.EndAutomatism();	
				UIFacade.UIFacadeInstance.sendNotification(AutoPlayEventList.CANCEL_AUTOPLAY_EVENT);
            }
			GameCommonData.isAutoPath=true;
			GameCommonData.IsFollow = false;   //停止跟随
			if(isNpc)
			{	
				GameCommonData.targetID=this.id;
				GameCommonData.Scene.MapPlayerTitleMove(new Point(xPos,yPos),1,GameCommonData.GameInstance.GameScene.GetGameScene.name);
			}
			else
			{	GameCommonData.targetID=-1;
				GameCommonData.Scene.MapPlayerTitleMove(new Point(xPos,yPos),0,GameCommonData.GameInstance.GameScene.GetGameScene.name);
			}
			
		}
		
		private function removeHandler(evt:Event):void
		{
			txtContainer.removeEventListener(MouseEvent.CLICK,go);
			txtContainer.removeEventListener(MouseEvent.MOUSE_OVER,overHandler);
			txtContainer.removeEventListener(MouseEvent.MOUSE_OUT,outHandler);
			txtContainer.removeEventListener(Event.REMOVED_FROM_STAGE,removeHandler);
			txt1 = null;
			txt2 = null;
			format = null;
//			txtContainer.removeEventListener(Event.REMOVED_FROM_STAGE,removeHandler);
		}
		
	}
}