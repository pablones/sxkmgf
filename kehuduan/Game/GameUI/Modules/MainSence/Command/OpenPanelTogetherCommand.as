package GameUI.Modules.MainSence.Command
{
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.DisplayObject;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;
	
	/** 组合多开面板 */
	public class OpenPanelTogetherCommand extends SimpleCommand
	{
		private var dataProxy:DataProxy
		
		public function OpenPanelTogetherCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			if(GameCommonData.Player.Role.Level < 30) {
				return;	
			}
			
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			
			var leftPanel:DisplayObject;
			var rightPanel:DisplayObject;
			
			var num:uint = GameCommonData.GameInstance.GameUI.numChildren;
			for(var i:int = 0; i < num; i++) {
				var child:DisplayObject = GameCommonData.GameInstance.GameUI.getChildAt(i);
				if(child is PanelBase && child.name) {
					var name:String = child.name;
					switch(name) {
						case "HeroProp":		//人物属性面板
							leftPanel = child;
							break;
						case "SkillView":		//技能面板
							leftPanel = child;
							break;
						case "PetBag":			//宠物背包
							leftPanel = child;
							break;
//						case "TaskView":		//任务面板
//							leftPanel = child;
//							break;
						case "Bag":				//背包
							rightPanel = child;
							break;
						case "Friend":			//好友面板
							rightPanel = child;
							break;
						case "unity":			//帮派面板
							rightPanel = child;	
							break;
						case "teamPanelBase":	//组队面板
							rightPanel = child;
							break;
//						case "settingPanel":	//系统设置
//							rightPanel = child;
//							break;
//						case "designationPanel"://称号
//							rightPanel = child;
//							break;
//						case "SystemMediator":	//系统消息
//							rightPanel = child;
//							break;
					}
				}
			}
			
			if(leftPanel && rightPanel) {		//有两个面板
				leftPanel.x  = (1000 - leftPanel.width -  rightPanel.width)  >> 1;
				leftPanel.y  = ((580  - leftPanel.height) >> 1) - 10;
				rightPanel.x = leftPanel.x + leftPanel.width + 1;
				rightPanel.y = leftPanel.y;
				GameCommonData.GameInstance.GameUI.addChild(leftPanel); 
				GameCommonData.GameInstance.GameUI.addChild(rightPanel); 
			} else if(leftPanel) {				//有左面板
				var posL:Point = UIUtils.getMiddlePos(leftPanel);
				leftPanel.x = posL.x;
				leftPanel.y = posL.y - 10;
			} else if(rightPanel){							//有右面板
				var posR:Point = UIUtils.getMiddlePos(rightPanel);
				rightPanel.x = posR.x;
				rightPanel.y = posR.y - 10;
			}
			
		}
		
	}
}