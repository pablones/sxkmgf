package GameUI.Modules.Stall.UI
{
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.UICore.UIFacade;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	public class StallSkinManager
	{
		private static var _instance:StallSkinManager;
		
		public function StallSkinManager(s:Singleton)
		{
			if(!s) throw new Error("Error getting stallSkin");
		}
		
		public static function getInstance():StallSkinManager
		{
			if(!_instance) _instance = new StallSkinManager(new Singleton());
			return _instance;	
		}
		
		/** 获取摆摊皮肤 */
		public function getStallSkin(stallId:int):MovieClip
		{
			if(stallId == 0) return null;
			var stallSkin:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("StallSkin");
			stallSkin.tabEnabled = false;
			stallSkin.tabChildren = false;
			stallSkin.txtStallName.mouseEnabled = false;
			stallSkin.name = "stallSkin_" + stallId;
			stallSkin.addEventListener(MouseEvent.ROLL_OVER, overHandler);
			stallSkin.addEventListener(MouseEvent.ROLL_OUT, outHandler);
			stallSkin.addEventListener(MouseEvent.MOUSE_DOWN, skinClickHandler);
			if(StallConstData.stallLookDic[stallId]) {
				if(StallConstData.stallLookDic[stallId] == 1) {			//查看过的
					stallSkin.gotoAndStop(2);
				} else if(StallConstData.stallLookDic[stallId] == 2) {	//自己关注的
					stallSkin.gotoAndStop(3);
				}
			} else {
				stallSkin.gotoAndStop(1);
			}
			return stallSkin;
		}
		
		private function skinClickHandler(e:MouseEvent):void {
			e.stopPropagation();
			GameCommonData.Scene.PlayerStop();
			var stallId:int = (e.currentTarget.name as String).split("_")[1];
			UIFacade.GetInstance(UIFacade.FACADEKEY).selectStall(stallId);
			if(!StallConstData.stallLookDic[stallId]) {
				StallConstData.stallLookDic[stallId] = 1;
			}
			e.currentTarget.gotoAndStop(2);
		}
		
		private function overHandler(e:MouseEvent):void {
			e.currentTarget.gotoAndStop(1);
//			SysCursor.GetInstance().setMouseType(SysCursor.STALK_CURSOR);
		}
		
		private function outHandler(e:MouseEvent):void {
			var stallId:int = (e.currentTarget.name as String).split("_")[1];
			if(StallConstData.stallLookDic[stallId]) {
				e.currentTarget.gotoAndStop(2);
			}
//			SysCursor.GetInstance().revert();
		}
	}
}

class Singleton
{
	
}