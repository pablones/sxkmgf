package GameUI.Modules.Login.UITool
{
	import GameUI.Modules.Login.Data.CreateRoleConstData;
	import GameUI.Modules.Login.InterFace.IRoleController;
	import GameUI.Modules.Login.InterFace.IRoleMemento;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;

	public class RoleMegController extends Sprite implements IRoleController
	{
		public function RoleMegController()
		{
			super();
			this.alpha = .7;
			this.mouseEnabled = false;
		}
		
		public function add(memento:IRoleMemento):void
		{
			this.addChild(memento as DisplayObject);
			if(this.numChildren > CreateRoleConstData.megTotal) deletes();
			updateSpace();
		}
		
		public function deletes():void
		{
			this.removeChildAt(0);
		}
		
		public function updateSpace():void
		{
			for(var i:int = 0; i < this.numChildren; i++)
			{
				if(this.numChildren < 4) this.getChildAt(i).alpha = 1;
				else 
				{
					if(i < 2) this.getChildAt(i).alpha = .4 * i + .2;
				}
				this.getChildAt(i).x = 10;
				this.getChildAt(i).y = 20 * i + 50;
			}
		}
		
	}
}