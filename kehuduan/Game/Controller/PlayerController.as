package Controller
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayItemsCommand;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Graphics.Animation.AnimationEventArgs;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	import OopsEngine.Skill.GameAudioResource;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	import OopsEngine.Skill.JobGameSkill;
	import OopsEngine.Skill.WeirdyGameSkill;
	
	import OopsFramework.Audio.AudioEngine;
	
	import flash.geom.Point;
	import flash.ui.Keyboard;
	import flash.utils.Dictionary;
	import flash.utils.setTimeout;
	
	public class PlayerController
	{ 		
		//重新设置目标 用于走到目标继续选怪
		public static var IsChangeAutomatism:Boolean = false;
		//吃天灵丹 地灵丹协议
		public static var UseDrug:Number = 0;
		//发送时间
		public static var SendAttack:Number =0;
	
		/**重新选择目标**/
		public static function ClearAutomatism():void
		{
			if(GameCommonData.AutomatismPoint != null)
			{
				IsChangeAutomatism = true;
				GameCommonData.Scene.MapPlayerTitleMove(GameCommonData.AutomatismPoint);
			}
		}
		
		/**核对坐标点**/
		public static function CheckPoint():void
		{		
			var gamePoint:Point = MapTileModel.GetTilePointToStage(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY);		
			GameCommonData.Scene.gameScenePlay.IsUpdateNicety = false;
			GameCommonData.Player.X  = gamePoint.x;
			GameCommonData.Player.Y  = gamePoint.y;
			GameCommonData.Scene.ResetMoveState();
			GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC); 
			//重置宠物行走数据
			if(GameCommonData.Player.Role.UsingPetAnimal != null)
			{
				GameCommonData.Scene.PetResetMoveState();
			}
		} 
		
		/**人物选怪自动攻击**/
		public static function UpdateAttack():void
		{
			var time:Date = new Date();
		
			if(time.time - GameCommonData.AttackTargetTime > 1100  && GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_STATIC
			&& GameCommonData.AttackTargetID != 0 && GameCommonData.TargetAnimal != null)
			{
				GameCommonData.AttackTargetTime = time.time;
				GameCommonData.TargetAnimal.IsSelect(true);
				GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);	
			}
		}
		
		
		/**监控选择目标**/
		public static function UpdateAutomatism():void
		{
			var time:Date = new Date();
			
			if(time.time - UseDrug > 60000 && AutoPlayData.aSaveNum[8] != null && AutoPlayData.aSaveNum[9] != null )
			{
				//是否吃天灵丹
				if(!GameCommonData.Player.Role.IsBuff(11018) && AutoPlayData.aSaveNum[8] > 0)
				{
					 UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(AutoPlayItemsCommand.NAME,"sky");
				}
				
				//是否吃地灵丹
				if(!GameCommonData.Player.Role.IsBuff(11024) && AutoPlayData.aSaveNum[9] > 0)
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(AutoPlayItemsCommand.NAME,"earth");
				}
				
				UseDrug = time.time;
			}
			
			
			if((time.time - GameCommonData.AutomatismTime) > 9000)
			{
				//15秒内不打该怪则丢黑名单
				if(GameCommonData.TargetAnimal != null)
				{
					TargetController.DeadAnimal[GameCommonData.TargetAnimal.Role.Id] =  GameCommonData.TargetAnimal.Role.Id;				
					GameCommonData.TargetAnimal.IsSelect(false);
					GameCommonData.TargetAnimal = null;				
				}
				GameCommonData.AutomatismTime = time.time; 
				
				if(DistanceController.PlayerAutomatism(5,new Point(GameCommonData.Player.Role.TileX, GameCommonData.Player.Role.TileY)))
				{
					Automatism();
				}
				else
				{
					ClearAutomatism();
				}
			}		
			else if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_STATIC && GameCommonData.IsAttack == false)
			{
				if(time.time - SendAttack >= 2000)
				{
					SendAttack = time.time;
					if(GameCommonData.TargetAnimal != null)				
					{				
						if(GameCommonData.Scene.gameScenePlay.Map.IsPass(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY))
						{													
							GameCommonData.TargetAnimal.IsSelect(true);
							GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);	
						}
						else
						{
							if(AStarController.GetNearPoint(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),    //当前的点
						    new Point(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY),true) != null )
							{
								GameCommonData.TargetAnimal.IsSelect(true);
								GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);
							}
							else
							{
								if(DistanceController.PlayerAutomatism(5,new Point(GameCommonData.Player.Role.TileX, GameCommonData.Player.Role.TileY)))
								{
									Automatism();
								}
								else
								{
									ClearAutomatism();
								}
							}
						}
					}
					else
					{
						Automatism();
					}
				}
			}
		}
		
		/**开始自动打怪**/
		public static function BeginAutomatism():void
		{	
			//开始追加攻击
			GameCommonData.Player.IsAddAttack = true;
			
			TargetController.TargetAutomatismID = 0;
			GameCommonData.AutomatismPoint = new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY);
			GameCommonData.Player.IsAutomatism = true;
			GameCommonData.Player.Automatism = UpdateAutomatism;
			
			//设置挂机时间
			var time:Date = new Date();
			GameCommonData.AutomatismTime = time.time;
			
			if(GameCommonData.TargetAnimal != null)
			{
				GameCommonData.TargetAnimal.IsSelect(false);
				GameCommonData.TargetAnimal = null;
				GameCommonData.UIFacadeIntance.GetKeyCode(Keyboard.ESCAPE);
			}									
			
			TargetController.TargetList = new Dictionary();
			
			/**自动打怪**/
			Automatism();
		}

 		/**自动打怪**/  
        public static function Automatism():void
		{											
			if(GameCommonData.TargetAnimal != null && GameCommonData.TargetAnimal.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);
				return;
			}
			else if(GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				GameCommonData.Scene.PlayerStop();
			}
			
			var t:Date = new Date();
			
			//遇到击退3秒在选怪
			if(t.time - GameCommonData.Targettime > 1000)		
			{	
				GameCommonData.Player.Role.DefaultSkill = 0;
				//搜索目标
				TargetController.GetTarget();	
			}
		}

		/**结束自动打怪**/
		public static function EndAutomatism():void
		{
			if(GameCommonData.Player.IsAutomatism)
			{
				TargetController.TargetAutomatismID = 0;
				GameCommonData.Player.IsAutomatism = false;
				GameCommonData.AutomatismPoint = null;
				UIFacade.UIFacadeInstance.sendNotification(AutoPlayEventList.CANCEL_AUTOPLAY_EVENT);
				UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_play_end_1"],0xffff00); // "停止挂机，快捷键B"
			}
		}
		
		/**设置主人速度
		 * person         人物
		 * distanceTime   移动时间间隔**/
		public static function SetPlayerSpeed(person:GameElementAnimal,moveStepLength:int):void
		{
			person.SetMoveSpend(moveStepLength);
			PetController.SetPetSpeed(person,moveStepLength);
		}
		
		/**跟随**/
		public static function  SetFollowTargetAnimal():void
		{
			GameCommonData.Scene.MapPlayerTitleMove(new Point(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY),2);
			GameCommonData.IsFollow = true;		
		}		
		
		/**使用技能**/
		public  static function UseSkill(skillID:int):void
		{
			var now:Date = new Date();
            AutomatismController.AutomatismCoolTime = now.time;
			
			if(UIConstData.IS_BUSINESSING && skillID==9000){
				UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_play_useskill_1"],0xffff00);  // "跑商中不能使用回城技能"
				return;
			}
			
			if(TargetController.IsPKTeam() && skillID==9000){
				UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_play_useskill_3"],0xffff00);  // "该场景不能使用回城"
				return;
			}
			
			if(UIConstData.IS_BUSINESSING && skillID==9508){
				UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_play_useskill_2"],0xffff00);  // "跑商中不能使用坐骑"
				return;
			}
			
			if(TargetController.IsPKTeam() && skillID==9508){
				UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_play_useskill_4"],0xffff00);  // "该场景不能使用坐骑"
				return;
			}
			
			if( skillID<6000 || skillID>7000 )
			{
			//清空预备技能编号
			GameCommonData.Player.Role.NextSkill = 0;
			//预备技能攻击目标
			GameCommonData.NextSkillTarget = null;
		
			//获取技能信息
			var gamskill:GameSkill = GameCommonData.SkillList[skillID];
			
			if(gamskill != null)
			{
				//如果有地效则清空地效
				if(GameCommonData.Rect != null)
				{
					FloorEffectController.ClearFloorEffect();
				}	
						
				//判断是否有是地效效果魔法
				if(GameSkillMode.IsShowRect(gamskill.SkillMode))			
				{	
					//添加地面效果
					FloorEffectController.GetFloorEffect(gamskill.SkillArea);
					GameCommonData.RectSkillID = skillID;
				}
				else
				{
					//判断是否是宠物技能
					if(GameSkillMode.IsPetSkill(gamskill.SkillMode))
					{			
						PetController.PetUseSkill(skillID);
					}
					else
					{
						//判断是否是回城技能
						if(gamskill.SkillMode == 24)
						{
						    PlayerUseSkill(skillID);
						}
						else
						{
							//判断当前是否在使用技能 使用则预存技能
							if(GameCommonData.Player.handler != null && GameCommonData.Player.handler.SkillInfo != null
							&& !GameSkillMode.IsCommon(GameCommonData.Player.handler.SkillInfo.SkillMode))
							{
								SetNextSkill(skillID);
							}
							else
							{
								PlayerUseSkill(skillID);						
							}
						}
					}
				}
			}
			}
			else
			{
				SkillData.useLifeSkill(skillID);
			}
		}	
		
		/**使用预备技能 公共CD好了**/
		public 	static function UseNextSkill():void
		{
			if(GameCommonData.Player.Role.NextSkill != 0)
			{
				//目标一致
				if(GameCommonData.NextSkillTarget == GameCommonData.TargetAnimal)
				{
					UseSkill(GameCommonData.Player.Role.NextSkill);
				}
				GameCommonData.Player.Role.NextSkill = 0;
			    GameCommonData.NextSkillTarget = null;
			}
		}
		
		/**设置预备技能 公共CD没好 私有CD好**/
		public static function SetNextSkill(skillID:int):void
		{
			GameCommonData.Player.Role.NextSkill = skillID;
			GameCommonData.NextSkillTarget = GameCommonData.TargetAnimal;
		}
		
		/**判断是否是宠物技能**/
		public static function IsPetSkill(skillID:int):void
		{
			var gamskill:GameSkill = GameCommonData.SkillList[skillID];
			GameSkillMode.IsPetSkill(gamskill.SkillMode);

		}
		
		
		/** 获取角色信息*/
		public  static function GetPlayer(id:int):GameElementAnimal
		{
			var player:GameElementAnimal;
			
			if(GameCommonData.Player != null && id == GameCommonData.Player.Role.Id)												// 是否是当前玩家
			{
				player = GameCommonData.Player;
			}
			else if(GameCommonData.SameSecnePlayerList != null && GameCommonData.SameSecnePlayerList[id]!=null)						// 其它同屏玩家
			{
				player = GameCommonData.SameSecnePlayerList[id];
			}
			return player;
		}
		
		/**当动画播放完成后调用**/
		public static function  onActionPlayComplete(e:GameElementAnimal):void
		{
			//判断人物的状态是否死亡
			if(e.Role.ActionState == GameElementSkins.ACTION_DEAD)
			{
//				e.Dispose();
				if(e is GameElementPlayer)
				{
					(e as GameElementPlayer).RemoveTernal();
				}

				GameCommonData.GameInstance.GameScene.GetGameScene.MiddleLayer.Elements.Remove(e);
				
				GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.addChild(e);
//				GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.Elements.Add(e);
				
				//移除人身上的动画
				if(e.skillAnimation.length > 0)
				{
					for(var n:int = e.skillAnimation.length - 1;n >=0;n--)
					{
						try
						{
							e.removeChild(e.skillAnimation[n]);
						}
						catch(e:Error)
						{
							
						}
						e.skillAnimation[n] = null;
						e.skillAnimation.splice(n,1);
					}
				}
				
				if(e.Role.Type == GameRole.TYPE_ENEMY)
				{
					if(e.IsMustDel)
						GameCommonData.Scene.DeletePlayer(e.Role.Id,true);	
					else
				 		setTimeout(GameCommonData.Scene.DeletePlayer,1500,e.Role.Id,true);	
				}	
				
				e.MustMovePoint = null;		
			}
		}
		
		public static function MustMove(e:GameElementAnimal):void
        {	
        	if(e.Role.Id == GameCommonData.Player.Role.Id)
        	{
        		if(e.MustMovePoint != null)
				{				 
					if(!(e.Role.TileX == e.MustMovePoint.x && e.Role.TileY == e.MustMovePoint.y))
					{
						if(e.handler != null)
						  e.handler.Clear();
						
						if(e.MustMovePoint != null) 
							e.Move(e.MustMovePoint,e.Dis);
					}
				}
        	}
        	else
        	{
        		var endPoint:Point = MapTileModel.GetTileStageToPoint(e.GameX,e.GameY); 
        		if(e.Role.Type == GameRole.TYPE_PLAYER)
        		{
	        		if(!(e.Role.TileX == endPoint.x && e.Role.TileY == endPoint.y))
	        		{		
	        			e.MoveTile(new Point(e.Role.TileX ,e.Role.TileY),0,true);
	        		} 		        			
        		}
        	}

        }
	
		
		/** 设置人物指定帧处理事件 */
		public static function onActionPlayFrame(e:AnimationEventArgs):void
		{	
			var animal:GameElementAnimal = e.Sender as GameElementAnimal;			
			
			//声音处理
			if(e.CurrentClipName.indexOf(GameElementSkins.ACTION_NEAR_ATTACK)>=0 && e.CurrentClipFrameIndex==1)
			{
			   if(animal.Role.Type == GameRole.TYPE_PLAYER || animal.Role.Type == GameRole.TYPE_OWNER)
			   {			
				   if(GameCommonData.SkillLoadAudioEngine[animal.Role.CurrentJobID] == null)
				   {		   	
				   	  var gameAudioResource:GameAudioResource  = new GameAudioResource();			
				   	  gameAudioResource.AudioName              = animal.Role.CurrentJobID.toString();
				   	  gameAudioResource.OnLoadAudio            = AudioController.LoadAudio;
				   	  gameAudioResource.AudioPath              = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillAudio + animal.Role.CurrentJobID.toString()+".mp3";
				   	  gameAudioResource.AudioBR.LoadComplete   = gameAudioResource.onAudioComplete;
				   	  gameAudioResource.AudioBR.Download.Add(gameAudioResource.AudioPath);       
					  gameAudioResource.AudioBR.Load();
					  GameCommonData.SkillLoadAudioEngine[animal.Role.CurrentJobID] = animal.Role.CurrentJobID;
					}
					else if(GameCommonData.SkillOnLoadAudioEngine[animal.Role.CurrentJobID] != null)
					{
					    var gameAudio:AudioEngine  = GameCommonData.SkillOnLoadAudioEngine[animal.Role.CurrentJobID] as AudioEngine;
						AudioController.SoundAttack(gameAudio);		
					} 	
			    }
			}
			
			//攻击处理
			if(e.CurrentClipName.indexOf(GameElementSkins.ACTION_NEAR_ATTACK)>=0 && e.CurrentClipFrameIndex==5)
			{				 
				//判断是否是攻击招数  播放攻击动画          	
				if(animal.handler != null && animal.handler is PlayerActionHandler)
				{
					if(animal.handler.InfoHandler != null)
						animal.handler.InfoHandler.Run();
												
					animal.handler.NextHendler();		
			    }	
			    		    
			    //使用预备技能		    
			    if(animal.Role.Id == GameCommonData.Player.Role.Id)
			    {
			    	UseNextSkill();	
			    }
			    
			    //宠物自动释放技能
			    if(animal.Role.Type == GameRole.TYPE_PET && GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.Role.Id == animal.Role.Id)
			    {
			    	PetController.AutomatismUseSkill();
			    }
			}
			
			
//			if(animal.Role.Id == GameCommonData.Player.Role.Id && GameCommonData.Player.IsAutomatism)
//			{
//				if(e.CurrentClipName.indexOf(GameElementSkins.ACTION_NEAR_ATTACK)>=0 && e.CurrentClipFrameIndex==4)
//				{				             	
//					if(animal.handler != null && animal.handler is PlayerActionHandler && animal.handler.InfoHandler != null)
//					{
//						animal.handler.InfoHandler.Run();
//						animal.handler.NextHendler();		
//				    }				                                 
//	                UseNextSkill();	                
//				    if(animal.Role.Type == GameRole.TYPE_PET)
//				    {
//				    	PetController.AutomatismUseSkill();
//				    }		    
//				}
//			}			
//			else
//			{
//				if(e.CurrentClipName.indexOf(GameElementSkins.ACTION_NEAR_ATTACK)>=0 && e.CurrentClipFrameIndex==4)
//				{				             	
//	
//					if(animal.handler != null && animal.handler is PlayerActionHandler && animal.handler.InfoHandler != null)
//					{
//						animal.handler.InfoHandler.Run();		
//				    }				                 
//	                     
////	                UseNextSkill();
////		                
////				    if(animal.Role.Type == GameRole.TYPE_PET)
////				    {
////				    	PetController.AutomatismUseSkill();
////				    }		    
//				}
//			}
		}
		
		/**更新飞行技能动画信息*/	
		public static function onUpdateSkillEffect(e:GameElementAnimal):void
		{
			if(e.handler != null && e.handler  is PlayerSkillHandler)
			{
			 	 e.handler.Run();
			}
		}		
		
		
		/**根据获取普通技能*/
		public static function GetDefaultSkill(player:GameElementAnimal):GameSkill
		{
			var gameSkill:GameSkill;
			
			//判断是否是怪物	
			if(player.Role.Type == GameRole.TYPE_ENEMY)
		    {
		        var  weirdyGameSkill:WeirdyGameSkill =  GameCommonData.WeirdyGameSkillList[player.Role.MonsterTypeID];
		        if(weirdyGameSkill != null)
                     gameSkill = GameCommonData.SkillList[weirdyGameSkill.SkillID];
		    }
		    else
		    {
		         var jobGameSkill:JobGameSkill = GameCommonData.JobGameSkillList[player.Role.CurrentJobID];
		         if(jobGameSkill != null)
                    gameSkill = GameCommonData.SkillList[jobGameSkill.SkillID];
		    }
		        
		    if(gameSkill == null)
		    {
		        gameSkill = GameCommonData.SkillList[1];
		    }
		    
		    return gameSkill;
		}

		/**使用技能*/
		public static function UsePlayerSkill(playerSkill:GameSkillLevel,player:GameElementAnimal,point:Point):void
		{
			var combat:CombatController = new CombatController();	
 			var state:int = GameSkillMode.Affect(playerSkill.gameSkill.SkillMode);
 			
 			//友方增益魔法
 			if(GameSkillMode.TargetState(playerSkill.gameSkill.SkillMode) == 5)
 			{
 				if (player.Role.Type == GameRole.TYPE_ENEMY
 				|| player.Role.Type == GameRole.TYPE_NPC) 
 				{
 					player = GameCommonData.Player;
 				}
 			}

 			switch(state)
 			{
 				case 1 :combat.ReserveSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level,player);	
 				      break;
 				case 2 :combat.ReserveAffectSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level);	
 				      break;     
 			    case 3 :combat.ReservePointAffectSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level,point);
 			          break;   
 			    case 4 :combat.ReserveBuffSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level,player);
 			          break;
 			    case 5 :combat.ReserveSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level,GameCommonData.Player);	
 				      break;
 				case 6 :combat.ReserveBuffSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level,GameCommonData.Player);	
 				      break;
 				case 7 :combat.ReserveDirSkillAttack(playerSkill.gameSkill.SkillID,playerSkill.Level,GameCommonData.Player);	
 				      break;   
 			}		
 			GameCommonData.IsAttack = false;	
 			
 			if (playerSkill.gameSkill.SkillID == 1303) CooldownController.getInstance().triggerGCD();
		}
		
		/**玩家使用技能*/
		public static function PlayerUseSkill(UseskillID:int,point:Point = null):void
		{	
			//是否可以使用回城
			if(UseskillID == 9000)
			{
				if(GameCommonData.Player.Role.IsAttack)
				{
				   UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_play_playeruseskill_1"],0xffff00); // "交战状态，不能使用回城"
				   return;
				}
			}
			
			//是否是上坐骑
			if(UseskillID == 9508)
			{
				PlayerSkinsController.SetMount();
				return;
			}
			
			
			var skill:GameSkill = GameCommonData.SkillList[UseskillID] as GameSkill;
			
			//按下的是普通攻击
			if(skill.SkillMode == 100)
			{
				GameCommonData.Scene.Attack(GameCommonData.TargetAnimal);
				return;
			}
			
			//取消选地效果
			if(GameCommonData.Scene.loadCircle != null)
			{
				GameCommonData.GameInstance.GameUI.removeChild(GameCommonData.Scene.loadCircle);
				GameCommonData.Scene.loadCircle = null;
			}
			
     	    var playerSkill:GameSkillLevel =  GameCommonData.Player.Role.SkillList[UseskillID] as GameSkillLevel;
     	    //受击方
     	    var targetAnimal:GameElementAnimal;
     	    //如果受击方是自己
     	    if(GameCommonData.TargetAnimal == null)
     	    {
				targetAnimal = GameCommonData.Player;
     	    }
     	    else
     	    {
     	    	targetAnimal = GameCommonData.TargetAnimal;
     	    }
			
			//判断该玩家是否拥有该技能
			if(playerSkill != null)
			{
				//判断是否可以对目标使用技能
				if(IsUseSkill(GameCommonData.Player,targetAnimal,playerSkill.gameSkill))
				{	
					//是否有选中并向目标移动
					if(GameSkillMode.TargetState(playerSkill.gameSkill.SkillMode) == 1)
					{
						GameCommonData.Scene.Attack(GameCommonData.TargetAnimal, playerSkill.gameSkill.SkillID);
					} else if(GameSkillMode.TargetState(playerSkill.gameSkill.SkillMode) == 5)
					{
						if(targetAnimal != null && targetAnimal.Role.Type !=  GameRole.TYPE_ENEMY
						&& targetAnimal.Role.Type !=  GameRole.TYPE_NPC
						&& targetAnimal.Role.Id != GameCommonData.Player.Role.Id)	
						{
					  		 GameCommonData.Scene.Attack(GameCommonData.TargetAnimal,playerSkill.gameSkill.SkillID);
					 	}
					 	else
					 	{
			                if(!GameCommonData.Player.IsAutomatism)
			            		GameCommonData.Player.IsAddAttack = true;
			            		
					 		UsePlayerSkill(playerSkill,targetAnimal,point);
					 	}
					}					
					else //不需要移动则直接释放
					{	
						if(!GameCommonData.Player.IsAutomatism)
			            	GameCommonData.Player.IsAddAttack = true;
						
 					 	UsePlayerSkill(playerSkill,targetAnimal,point);
 				    }
 				}
			}		
		}
		
		/**判断该目标是否可以被攻击*/
		public static function IsUseSkill(player:GameElementAnimal,targetAnimal:GameElementAnimal,gameSkill:GameSkill=null):Boolean
		{	
			var IsUseSkill:Boolean = false;	
			var skillState:int = 1;           // 1 指定玩家伤害  2 只能对自己释放 3 宠物只能对主人释放  4 群放魔法不需要判断  5 只能对玩家释放的友善技能
			
			if(gameSkill != null)  //为空则为普通攻击
			{
				skillState = GameSkillMode.TargetState(gameSkill.SkillMode);
			}           
             
			switch(skillState)
			{
				case 1:
				    if(targetAnimal != null)
					{
						if(targetAnimal.Role.ActionState != GameElementSkins.ACTION_DEAD           //是否死亡
				   		&& targetAnimal.Role.HP > 0)
				  		 {
				   			if(targetAnimal.Role.Type == GameRole.TYPE_ENEMY)                      //是敌人
				   			{
				   			     IsUseSkill = true;
				   		    }
				   		    else if (targetAnimal.Role.Type == GameRole.TYPE_PLAYER)               //如果是玩家   
				   		    {
					   			if(player.Role.idTeam !=  targetAnimal.Role.idTeam ||              //是否同1队              
									player.Role.idTeam==0)
								{
									IsUseSkill = true;
								}
				   		    }
					   		else if (targetAnimal.Role.Type == GameRole.TYPE_PET)                   //如果是宝宝
					   		{
					   			if(player.Role.UsingPetAnimal == null)
					   			{
									IsUseSkill = true;
					   			}
					   			else if (player.Role.UsingPetAnimal.Role.Id != targetAnimal.Role.Id)
					   			{
					   				if(player.Role.idTeam !=  targetAnimal.Role.MasterPlayer.Role.idTeam ||              //是否同1队              
									targetAnimal.Role.MasterPlayer.Role.idTeam==0)
									{
					   					IsUseSkill = true;
					   				}
					   			}
					   		}
					   	}
				     }
				     break;
				case 2:
					 IsUseSkill = true;	
				     break;
				case 4:
				     IsUseSkill = true;	
				      break;
				case 5:
				     IsUseSkill = true;	
//					if(targetAnimal != null)
//					{	
//						if (targetAnimal.Role.Type == GameRole.TYPE_OWNER || targetAnimal.Role.Type == GameRole.TYPE_PLAYER || targetAnimal.Role.Type == GameRole.TYPE_PET)  
//						{
//							IsUseSkill = true;	
//						}
//					}
					break;
  
			}
			
			//如果对象是切磋的目标则可以攻击
			if(targetAnimal.Role.Id == GameCommonData.DuelAnimal)
			{
				IsUseSkill = true;
			}
			
			return IsUseSkill;				
		}
	}
}