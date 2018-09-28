package GameUI.Modules.Arena.View
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.ProConversion;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.View.UIKit.components.ItemRenderer;
	
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.Graphics;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;

	public class ArenaScoreItemRenderer extends ItemRenderer
	{
		public function ArenaScoreItemRenderer()
		{
			super();
			
			var g:Graphics = this.graphics;
			g.beginFill(0xFF9900, 0);
			g.drawRect(0, 0, 542, 23);
			g.endFill();
		}
		
		protected var initialized:Boolean = false;
		
		protected var txtRank:TextField;
		protected var txtName:TextField;
		protected var txtCurrentScore:TextField;
		protected var txtAwardScore:TextField;
		protected var txtKill:TextField;
		protected var txtCamp:TextField;
		protected var txtLevel:TextField;
		protected var txtPro:TextField;
		
		override protected function redraw():void
		{
			if (!data) return; // TODO(zhao): 之前要清场
			
			if (!initialized)
			{
				initialize();
			}
			
			txtRank.text = data.rank;
			txtName.text = data.name;
			txtCurrentScore.text = data.currentScore;
			txtAwardScore.text = data.awardScore;
			txtKill.text = data.kill;
			txtLevel.text = data.level;
			
			switch (data.vipLevel)
			{
				case 0:
					txtName.textColor = 0xE2CCA5;
					break;
				case 1:
					txtName.textColor = 0x0098FF;
					break;
				case 2:
					txtName.textColor = 0x7A3FE9;
					break;
				case 3:
					txtName.textColor = 0xFF6532;
					break;
				case 4:
					txtName.textColor = 0x00FF00;
					break;
			}
			
			switch (data.camp)
			{
				case 1:
					txtCamp.text = GameCommonData.wordDic["mod_are_med_are_upd_7"]; // 贪狼
					txtCamp.textColor = 0xFF32CC;
					break;
				case 2:
					txtCamp.text = GameCommonData.wordDic["mod_are_med_are_upd_8"]; // 七杀
					txtCamp.textColor = 0x00CBFF;
					break;
				case 3:
					txtCamp.text = GameCommonData.wordDic["mod_are_med_are_upd_9"]; // 破军
					txtCamp.textColor = 0xE08E1F;
					break;
			}
			
			txtPro.text = ProConversion.getInstance().RolesListDic[data.pro];
		}
		
		protected function initialize():void
		{
			initialized = true;
			
			txtRank = new TextField();
			txtName = new TextField();
			txtCurrentScore = new TextField();
			txtAwardScore = new TextField();
			txtKill = new TextField();
			txtCamp = new TextField();
			txtLevel = new TextField();
			txtPro = new TextField();
			
			txtRank.height = txtName.height = txtCurrentScore.height = txtAwardScore.height
			= txtKill.height = txtCamp.height = txtLevel.height = txtPro.height = 16;
			
			txtRank.y = txtName.y = txtCurrentScore.y = txtAwardScore.y
			= txtKill.y = txtCamp.y = txtLevel.y = txtPro.y = 5;
			
			txtRank.mouseEnabled = txtName.mouseEnabled = txtCurrentScore.mouseEnabled = txtAwardScore.mouseEnabled
			= txtKill.mouseEnabled = txtCamp.mouseEnabled = txtLevel.mouseEnabled = txtPro.mouseEnabled = false;
			
			txtRank.filters = txtName.filters = txtCurrentScore.filters = txtAwardScore.filters
			= txtKill.filters = txtCamp.filters = txtLevel.filters = txtPro.filters = Font.Stroke();
			
			txtRank.x = 12;
			txtRank.width = 25;
			txtName.x = 55;
			txtName.width = 88;
			txtCurrentScore.x = 159;
			txtCurrentScore.width = 43;
			txtAwardScore.x = 227;
			txtAwardScore.width = 43;
			txtKill.x = 291;
			txtKill.width = 43;
			txtCamp.x = 367;
			txtCamp.width = 28;
			txtLevel.x = 442;
			txtLevel.width = 25;
			txtPro.x = 494;
			txtPro.width = 32;
			
			var defaultFormat:TextFormat = new TextFormat("宋体", 12, 0xE2CCA5, null, null, null, null, null, TextFormatAlign.CENTER);
			
			txtRank.defaultTextFormat = txtName.defaultTextFormat = txtCurrentScore.defaultTextFormat = txtAwardScore.defaultTextFormat
			= txtKill.defaultTextFormat = txtCamp.defaultTextFormat = txtLevel.defaultTextFormat = txtPro.defaultTextFormat = defaultFormat;
			
			this.addChild(txtRank);
			this.addChild(txtName);
			this.addChild(txtCurrentScore);
			this.addChild(txtAwardScore);
			this.addChild(txtKill);
			this.addChild(txtCamp);
			this.addChild(txtLevel);
			this.addChild(txtPro);
			
			this.useHandCursor = this.buttonMode = true;
			this.addEventListener(MouseEvent.CLICK, clickHandler);
		}
		
		protected function clickHandler(event:MouseEvent):void
		{
			popUpMenu();
		}
		
		protected function popUpMenu():void
		{
			if (!menu)
			{
				buildMenu();
			}
			
			var m:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if (m)
			{
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			
			menu.x = stage.mouseX;
			menu.y = stage.mouseY;
			GameCommonData.GameInstance.GameUI.addChild(menu);
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_DOWN, stage_mouseDownHandler);
		}
		
		protected var menu:MenuItem;
		
		protected function buildMenu():void
		{
			menu = new MenuItem();
			
			var menuData:Array = [{cellText: GameCommonData.wordDic["mod_chat_med_qui_model_2"], data:{v: 1}}, // 加为好友
												{cellText: GameCommonData.wordDic["mod_chat_med_qui_model_3"], data:{v: 2}}, // 设为私聊
												{cellText: GameCommonData.wordDic["mod_mas_view_mys_ini_1"], data:{v: 3}}, // 邀请组队
												{cellText: GameCommonData.wordDic["mod_are_med_are_bui_1"], data:{v: 4}}]; // 查看信息
			
			if (data.id == GameCommonData.Player.Role.Id)
			{
				menuData.shift();									
			}
			menu.dataPro = menuData;
			menu.addEventListener(MenuEvent.Cell_Click, menu_cellClickHandler);
		}
		
		protected function stage_mouseDownHandler(event:MouseEvent):void
		{
			if (GameCommonData.GameInstance.GameUI.contains(menu) && !menu.contains(event.target as DisplayObject))
			{
				destroyMenu();
			}
		}
		
		protected function menu_cellClickHandler(event:MenuEvent):void
		{
			switch (event.cell.data.v)
			{
				case 1: // 加为好友
					GameCommonData.UIFacadeIntance.sendNotification(FriendCommandList.ADD_TO_FRIEND, {id:-1, name:data.name});
					break;
				case 2: // 设为私聊
					GameCommonData.UIFacadeIntance.sendNotification(ChatEvents.QUICKCHAT, data.name);
					break;
				case 3: // 邀请组队
					GameCommonData.UIFacadeIntance.sendNotification(EventList.INVITETEAM, {id: data.id});
					break;
				case 4: // 查看信息
					FriendSend.getInstance().getFriendInfo(data.id, data.name);
					break;
			}
			
			destroyMenu();
		}
		
		protected function destroyMenu():void
		{
			menu.removeEventListener(MenuEvent.Cell_Click, menu_cellClickHandler);
			menu.parent.removeChild(menu);
			menu = null;
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_DOWN, stage_mouseDownHandler);
		}
	}
}