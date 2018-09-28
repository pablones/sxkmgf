package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Team.Datas.TeamEvent;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Mediator.NewUnityOrderMediator;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Modules.UnityNew.Proxy.UnityMemberVo;
	import GameUI.View.BaseUI.ListComponent;
	
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.system.System;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class QuickSelectMediator extends Mediator
	{
		public static const NAME:String = "QuickSelectMediator";
		
		private var model:Array = [GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_2" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_4" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_5" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ]];//"查看资料", "加为好友", "设为私聊", "屏蔽此人", "复制姓名", "组    队"
		private var container:ListComponent = null;
		private var curName:String = "";
		private var parent:DisplayObjectContainer;
		
		//帮派
//		private var model:Array = [GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_2" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_4" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_5" ], GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ]];//"查看资料", "加为好友", "设为私聊", "屏蔽此人", "复制姓名", "组    队"
		private var unityModel:Array = [];
		private var currentMemberVo:UnityMemberVo;
		
		public function QuickSelectMediator(parent:DisplayObjectContainer = null)
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				ChatEvents.SHOWQUICKOPERATOR,
				ChatEvents.HIDEQUICKOPERATOR,
				NewUnityCommonData.SHOW_SINGLE_MEMBLER_SKIRT 
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ChatEvents.SHOWQUICKOPERATOR:
					curName = notification.getBody() as String;
					setView();
				break;
				case ChatEvents.HIDEQUICKOPERATOR:
					removeThis();
				break;
				case NewUnityCommonData.SHOW_SINGLE_MEMBLER_SKIRT:
					currentMemberVo = notification.getBody() as UnityMemberVo;
					removeThis();
					setUnityView();
				break;
			}
		}
		
		private function setView():void
		{
			ChatData.QuickChatIsOpen = true;
			container = new ListComponent();
			container.name = "QuickSelectList";
			for( var i:int = 0; i<model.length; i++ )
			{
				var quickOperator:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("QuickOperator");
				quickOperator.txtOpName.text = model[i];
				quickOperator.txtOpName.mouseEnabled = false;
				quickOperator.txtOpName.filters = Font.Stroke(0x000000);
				quickOperator.name = i.toString();
				quickOperator.addEventListener(MouseEvent.CLICK, onSelect);
				container.SetChild(quickOperator);
			}
			container.upDataPos();	
			GameCommonData.GameInstance.TooltipLayer.addChild(container);
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_DOWN, removeList);
			container.x = GameCommonData.GameInstance.TooltipLayer.mouseX;
			container.y = GameCommonData.GameInstance.TooltipLayer.mouseY - container.height;
		}
	
		private function onSelect(event:MouseEvent):void
		{
			switch(int(event.currentTarget.name))
			{
				case 0:
					 FriendSend.getInstance().getFriendInfo(0,curName);
				break;
				case 1:
					facade.sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:-1,name:curName});
				break;
				case 2:
					if(curName != GameCommonData.Player.Role.Name) 
					{
						facade.sendNotification(ChatEvents.QUICKCHAT, curName);
					}
				break;
				case 3:
					askForFilter();
				break;
				case 4:
					System.setClipboard(curName);
				break;
				case 5:
//					facade.sendNotification(TeamEvent.INVITETEAMBYNAME, {name:curName});  
					facade.sendNotification(TeamEvent.SUPER_MAKE_TEAM_BY_NAME, curName);  
				break;
				case 6:				//帮派任命
					if ( !facade.hasMediator( NewUnityOrderMediator.NAME ) )
					{
						facade.registerMediator( new NewUnityOrderMediator() );
					}
					sendNotification( NewUnityCommonData.OPEN_NEW_UNITY_ORDER_PANEL,currentMemberVo );
				break;
				case 7:				//踢出帮派
					if ( currentMemberVo.unityJob >= GameCommonData.Player.Role.unityJob )
					{
						sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00} );
						removeThis();
						return;
					}
					var hintInfo:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_uni_med_umme_let_1" ] + "<font color='#ffff00'>"+currentMemberVo.name+"</font>"+GameCommonData.wordDic[ "mod_uni_med_umme_let_2" ]+"</font>";
					sendNotification(EventList.SHOWALERT, {comfrim:letOutTrade, cancel:cancelFilter, isShowClose:false, info:hintInfo , 
																							title:GameCommonData.wordDic[ "often_used_tip" ], comfirmTxt:GameCommonData.wordDic[ "often_used_commit" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel2" ] } );
				break;
			}		 	
			removeThis();
		}
		
		//踢出帮派
		private function letOutTrade():void
		{
			RequestUnity.send( 213,0,currentMemberVo.id );
		}
		
		private function setUnityView():void
		{
			unityModel = model.concat();
			curName = currentMemberVo.name;
			if ( GameCommonData.Player.Role.unityJob > 60 )
			{
				unityModel.push( GameCommonData.wordDic[ "mod_uni_med_umme_sel_1" ] );
				unityModel.push( GameCommonData.wordDic[ "mod_uni_med_umme_sel_2" ] );
			}
			
			ChatData.QuickChatIsOpen = true;
			container = new ListComponent();
			container.name = "QuickSelectList";
			for( var i:int = 0; i<unityModel.length; i++ )
			{
				var quickOperator:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("QuickOperator");
				quickOperator.txtOpName.text = unityModel[i];
				quickOperator.txtOpName.mouseEnabled = false;
				quickOperator.txtOpName.filters = Font.Stroke(0x000000);
				quickOperator.name = i.toString();
				quickOperator.addEventListener(MouseEvent.CLICK, onSelect);
				container.SetChild(quickOperator);
			}
			container.upDataPos();	
			GameCommonData.GameInstance.TooltipLayer.addChild(container);
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_DOWN, removeList);
			container.x = GameCommonData.GameInstance.TooltipLayer.mouseX;
			container.y = GameCommonData.GameInstance.TooltipLayer.mouseY - container.height;
		}
		
		private function askForFilter():void
		{
			for(var i:int = 0; i < ChatData.FilterList.length; i++) {
				if(ChatData.FilterList[i] == curName) {
					removeThis();
					return;
				}
			}
			facade.sendNotification(EventList.SHOWALERT, {comfrim:sureFilter, cancel:cancelFilter, info:GameCommonData.wordDic[ "mod_chat_med_qui_ask" ]+"<font color='#FFFF00'>"+curName+"</font>？", title:"提 示"});//是否确定要屏蔽玩家
		}
		
		private function sureFilter():void
		{
			if(ChatData.FilterListIsOpen == true) {
				facade.sendNotification(ChatEvents.UPDATEFILTER, curName);
			} else {
				ChatData.FilterList.push(curName);
			}
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_chat_med_qui_sur" ]+"<font color='#ff0000'>["+curName+"]</font>", color:0xffff00});//你屏蔽了
		}
		
		private function cancelFilter():void
		{
			
		}
		
		private function removeList(event:MouseEvent):void
		{
//			trace(event.currentTarget.name);
//			trace(event.target.name);
			GameCommonData.GameInstance.removeEventListener(MouseEvent.MOUSE_DOWN, removeList);
			if(event.target.name == "selectBtn") return;
			removeThis();
		}
		
		private function removeThis():void
		{
			if(container)
			{
				var des:*;
				while ( container.numChildren>0 )
				{
					des = container.removeChildAt( 0 );
					des.removeEventListener(MouseEvent.CLICK, onSelect);
					des = null;
				}
				if(GameCommonData.GameInstance.TooltipLayer.contains(container))
				{
					ChatData.QuickChatIsOpen = false;
					GameCommonData.GameInstance.TooltipLayer.removeChild(container);
					container = null;
					facade.removeMediator(NAME);
				}
			}
		}
	}
}