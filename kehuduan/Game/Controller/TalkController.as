package Controller
{
	import GameUI.Modules.Chat.Data.ChatEvents;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import flash.utils.setTimeout;
	
	public class TalkController
	{
		public static function talk(personName:String):GameElementAnimal
		{	
			var playertalk:GameElementAnimal;	
			
			// 判断是否是自己
			if(personName == GameCommonData.Player.Role.Name)
			{
				playertalk = GameCommonData.Player;			
			}
			else
			{
				for(var player:* in GameCommonData.SameSecnePlayerList)
				{
					var play:GameElementAnimal = GameCommonData.SameSecnePlayerList[player];
					if(play.Role.Name == personName 
					&& (play.Role.Type == GameRole.TYPE_PLAYER || play.Role.Type == GameRole.TYPE_OWNER))
					{
						playertalk = play;
					}	
				}
			}		
			return playertalk;
		} 
		
		public static function createBossMC(play:GameElementAnimal,talk:DisplayObjectContainer):void
		{
			removeTalk(play);
			
			
			play.talkmc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("talkpp");
			play.talkmc.mouseEnabled = false;
			play.talkmc.mouseChildren= false;
			play.txftalk = talk;
			play.talkmc.height        = play.txftalk.height + 30;
			play.talkmc.width        = play.txftalk.width + 30;			
			if(play.Role.PersonSkinID == 5001 && play.golem == null)
			{
				play.talkmc.y = - play.talkmc.height + play.GetToplenght + 105;
			}
			else
			{
				play.talkmc.y = - play.talkmc.height + play.GetToplenght + 20;
			}
			play.talkmc.x = - play.talkmc.width + 50;	
			talk.x = play.talkmc.x + 10;
			talk.y = play.talkmc.y + 10;

		    var date:Date = new Date();
		    play.talkId = date.time;
			play.addChild(play.talkmc);	
			play.addChild(talk);	
			setTimeout(removeTalk,3000,play,play.talkId);
		}
		
		public static function createMC(play:GameElementAnimal,talk:DisplayObjectContainer):void
		{
			if(play.IsLoadSkins)
			{
				removeTalk(play);
				
				
				play.talkmc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("talkpp");
				play.talkmc.mouseEnabled = false;
				play.talkmc.mouseChildren= false;
				play.txftalk = talk;
				play.talkmc.height        = play.txftalk.height + 30;		
				play.talkmc.y = - play.talkmc.height + play.GetToplenght + 20;
				play.talkmc.x = - play.talkmc.width + 50;	
				talk.x = play.talkmc.x + 10;
				talk.y = play.talkmc.y + 10;

			    var date:Date = new Date();
			    play.talkId = date.time;
				play.addChild(play.talkmc);	
				play.addChild(talk);	
				setTimeout(removeTalk,5000,play,play.talkId);
			}
		}
		
		public static function BossTalk(state:int,play:GameElementAnimal,skillID:int=0):void
		{		
			var talk:String;
			if(GameCommonData.BossTalk[play.Role.PersonSkinID] != null)
			{
				var talktxt:Object = GameCommonData.BossTalk[play.Role.PersonSkinID];
				switch(state)
				{
					// 开始 
					case 1:
						if(GameCommonData.Scene.begin)
						{
							GameCommonData.Scene.begin = false;
							talk = talktxt.begin;
						}
						break;
					// 技能
					case 2:
						if(play.txftalk != null && play.contains(play.txftalk))
						{
							talk = null;
						}
						else
						{
							if(skillID != 0)
							{
								if(talktxt.skillID1 == skillID)
								{
									 talk = talktxt.skill1;
								}
								if(talktxt.skillID2 == skillID)
								{
									 talk = talktxt.skill2;
								}
								if(talktxt.skillID3 == skillID)
								{
									 talk = talktxt.skill3;
								}
							}
 						}
				    	break;
					// hp
					case 3:
					    if(GameCommonData.Scene.hp)
					    {
					    	var num:int = play.Role.HP / play.Role.MaxHp * 100; 
					    	if(num < talktxt.percent)
					    	{
					    	    talk = talktxt.hp;
					 	   		GameCommonData.Scene.hp = false;
					 	   	}
					    }
					break;
					// 死亡
					case 4:
					    GameCommonData.Scene.begin = true;
						talk = talktxt.dead;
					break;
				}
				
			}
			
			if(talk != null && talk.length > 0)
			{
				var container:Sprite = new Sprite();
	 			var talk_txt:TextField = new TextField();
				var format2:TextFormat = new TextFormat();
				format2.align 		   = TextFormatAlign.LEFT;
				format2.size		   = 12;
				format2.font		   = "宋体";
				format2.leading        = 4;
				talk_txt.defaultTextFormat = format2;
			    talk_txt.cacheAsBitmap     = true;
				talk_txt.mouseEnabled      = false;
				talk_txt.width             = 180;
				talk_txt.multiline         = true;
				talk_txt.wordWrap          = true;
				talk_txt.selectable 	   = false;
				talk_txt.textColor         = 0xffffff;		
				talk_txt.autoSize          = TextFieldAutoSize.LEFT;
				
				talk_txt.htmlText = talk;
				talk_txt.filters = Font.Stroke(0x000000);	
				
				container.graphics.clear();
				container.graphics.beginFill( 0xff0000,0 );
				container.graphics.drawRect( 0,0,talk_txt.textWidth,talk_txt.textHeight );
				container.graphics.endFill();
				talk_txt.height = talk_txt.textHeight;
				container.addChild( talk_txt );	
				
				GameCommonData.UIFacadeIntance.sendNotification(ChatEvents.BOSS_MESSAGE, [{boss:play.Role.Name, msg:talk, dfColor:0xFFFFFF}]);
				
				createBossMC(play,container);
			}
		}
		
		public static function removeTalk(play:GameElementAnimal,talkId:int = 0):void
		{
			if(talkId != 0 && play.talkId != talkId)
			{
				return;
			}

			if(play.txftalk != null && play.contains(play.txftalk))
			{
				while(play.txftalk.numChildren > 0)
				{
					var a:* = play.txftalk.removeChildAt(0) ;
					a= null;
				}
				
				play.removeChild(play.txftalk);
				play.txftalk = null;
			}
			
			if(play.talkmc != null && play.contains(play.talkmc))
			{
				play.removeChild(play.talkmc);
				play.talkmc = null;
			}
		}
	}
}