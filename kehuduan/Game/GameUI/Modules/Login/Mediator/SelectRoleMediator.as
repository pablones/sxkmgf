
package GameUI.Modules.Login.Mediator
{ 
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.UIConfigData;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SelectRoleMediator extends Mediator
	{
		public static const NAME:String = "SelectRoleMediator";
		
		public function SelectRoleMediator()
		{
			super(NAME);
		}
		
		private function get selectRole():MovieClip
		{
			return viewComponent as MovieClip
		}
		
		override public function listNotificationInterests():Array 
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWSELECTROLE,
				EventList.REMOVEELECTROLE
			];
		}
		
		override public function handleNotification(notification:INotification):void 
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SELECTROLE});
				break;
				case EventList.SHOWSELECTROLE:
					show();
				break;
				case EventList.REMOVEELECTROLE:
					gc();
					facade.removeMediator(SelectRoleMediator.NAME);
				break;
			}
		}
		
		private function show():void 
		{
			GameCommonData.GameInstance.GameUI.addChild(selectRole);
			initView();
			showRoles();
		}
		
		/** 初始化  */
		private function initView():void
		{
			for(var i:int = 0; i<4; i++)
			{
				this.selectRole["role_" + i].visible = false;
			}
		}
		
		/** 显示可选的角色  */
		private function showRoles():void
		{
			for(var i:int = 0; i<GameCommonData.RoleList.length; i++)
			{
				this.selectRole["role_" + i].visible = true;
				this.selectRole["role_" + i].mouseEnabled = true;
				this.selectRole["role_" + i].SzName.text = GameCommonData.RoleList[i].SzName;
				this.selectRole["role_" + i].SzName.mouseEnabled = false;
				this.selectRole["role_" + i].Level.text = GameCommonData.RoleList[i].Level;
				this.selectRole["role_" + i].Level.mouseEnabled = false;
				this.selectRole["role_" + i].addEventListener(MouseEvent.CLICK, selectedRole);
			} 
		}
		
		/** 处理gc  */
		private function gc():void
		{
			GameCommonData.GameInstance.GameUI.removeChild(selectRole);
			for(var i:int = 0; i<GameCommonData.RoleList.length; i++)
			{
				this.selectRole["role_" + i].removeEventListener(MouseEvent.CLICK, selectedRole);
			}
		}
		
		/** 选择角色  */
		private function selectedRole(e:MouseEvent):void
		{
			var index:int = int(e.target.name.split("_")[1]);
			facade.sendNotification(CommandList.SELECTROLECOMMAND, index);
		}
	}
}