package GameUI.Modules.Login.UITool
{
	import GameUI.Modules.Login.InterFace.IRoleMemento;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Sprite;
	import flash.text.TextField;

	public class RoleMegMemento extends Sprite implements IRoleMemento
	{
		public var sex:int;
		public var roleName:String;
		
		private var txtName:TextField;
		private var txtIcon:TextField;
		private var nameColor:String;
		public function RoleMegMemento(sex:int , roleName:String)
		{
			this.sex = sex;
			this.roleName = roleName;
			updateRank();
		}
		public function updateRank():void
		{
			iconInit();
			textInit();
		}
		private function textInit():void
		{
			txtName = new TextField();
			txtName.mouseEnabled = false;
			txtName.width = 200;
			txtName.filters = Font.Stroke(0x00000);
			txtName.htmlText = "<font color = '" + nameColor + "'> "+roleName+"</font>"  + "<font color = '#FFFFFF'> "+GameCommonData.wordDic[ "mod_log_sta_UIT_tex" ]+"</font>";//踏入了江湖
			this.addChild(txtName);
			txtName.x = 10;
			txtName.y = 0;
		}
		private function iconInit():void
		{
			txtIcon = new TextField();
			txtIcon.mouseEnabled = false;
			txtIcon.filters = Font.Stroke(0x000000);
			txtIcon.y = 0;
			this.addChild(txtIcon);
			switch(sex)
			{
				case 0:		//男
					nameColor = "#0098FF";
					txtIcon.htmlText = "<font color = '" + nameColor + "'>♂</font>";
				break;
				case 1:		//女
					nameColor = "#CC66FF";
					txtIcon.htmlText = "<font color = '" + nameColor + "'>♀</font>";
				break;
			}
		}

	}
}