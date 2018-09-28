package Controller
{
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Skill.GameSkillResource;
	import OopsEngine.Skill.SkillAnimation;
	
	import flash.display.Sprite;
	import flash.utils.setTimeout;
	
	public class FloorEffectController
	{
	    public static var playlist:Array = new Array();

		/**得到方形的区域快**/
		public static function  GetFloorEffect(gridCount:int):void			
		{
			var width:int   = 60*gridCount;											//设置方块的宽度
			var height:int  = 30*gridCount;											//设置其高度
			var rect:Sprite = new Sprite();										    //菱形的方块
			rect.graphics.lineStyle(0);
			rect.graphics.beginFill(0xff0000,.5);
			rect.graphics.moveTo(0,0);
			rect.graphics.lineTo(width/2,-(height/2));
			rect.graphics.lineTo(width,0);
			rect.graphics.lineTo(width/2,height/2);
			rect.graphics.lineTo(0,0);
			rect.graphics.endFill();
			
			GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.addChild(rect);		//添加rect到游戏的场景
			//设置方格的坐标
			rect.x=GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.mouseX-width/2;
			rect.y=GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.mouseY-rect.height/2+height/2;		
			GameCommonData.Rect = rect;
			
			rect.startDrag();															        //开始拖动rect
			
		}
	
		public static function ClearFloorEffect():void									
		{
           	if(GameCommonData.Rect != null  && GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.contains(GameCommonData.Rect)) 
			{  
				GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.removeChild(GameCommonData.Rect);	//移除rect			
			}
			GameCommonData.Rect = null;
		}
		
		public static function LoadPlayerEffect(gameSkillResource:GameSkillResource):void
		{
			GameCommonData.FloorOnLoadEffectList[gameSkillResource.SkillID] = gameSkillResource;
//			var player:GameElementAnimal = PlayerController.GetPlayer(gameSkillResource.playerID);
//			 if(player != null)
//			 {
//			 	AddElementEffect(player,gameSkillResource.SkillID);
//			 }
			
			for(var n:int = 0;n<FloorEffectController.playlist.length;n++)
			{
				if(playlist[n] != null && playlist[n].Role.isAddEffect)
				{
        			AddElementEffect(playlist[n],gameSkillResource.SkillID);
				}
			}
			FloorEffectController.playlist = new Array();
		} 
		
		public static function LoadEffect(gameSkillResource:GameSkillResource):void
		{
			GameCommonData.FloorOnLoadEffectList[gameSkillResource.SkillID] = gameSkillResource;		
			 var player:GameElementAnimal = PlayerController.GetPlayer(gameSkillResource.playerID);
			 if(player != null)
			 {
			 	AddFlootEffect(player,gameSkillResource.SkillID);
			 }
		} 
		
		public static function RemoveElementEffect(player:GameElementAnimal):void
		{
			if(player.playerEffect != null && player.contains(player.playerEffect))
			{
				player.removeChild(player.playerEffect);
			}
			player.playerEffect = null;
		}
		
		public static function AddElementEffect(player:GameElementAnimal,effectID:int = 100000):void
		{
	       if(GameCommonData.FloorEffectList[effectID] == null)
	       {       	
	       	    var gameSkillResource:GameSkillResource = new GameSkillResource();
	       	    gameSkillResource.OnLoadEffect = LoadPlayerEffect;
	       	    gameSkillResource.SkillID      = effectID;
		        gameSkillResource.playerID     = player.Role.Id;
		        gameSkillResource.EffectPath   = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF +effectID+".swf";
		        gameSkillResource.EffectBR.LoadComplete = gameSkillResource.onEffectComplete;
		        gameSkillResource.EffectBR.Download.Add(gameSkillResource.EffectPath);
		        gameSkillResource.EffectBR.Load();
		        GameCommonData.FloorEffectList[effectID] = effectID;	
		        FloorEffectController.playlist.push(player);		        	       				        			        		        					        
	       } 		  
	       else if (GameCommonData.FloorOnLoadEffectList[effectID] != null)
	       {
	       	   if(player.playerEffect == null)
	       	   {
		       	   var ongameSkillResource:GameSkillResource = GameCommonData.FloorOnLoadEffectList[effectID] as GameSkillResource;				
			  	   var animationSkill:SkillAnimation = ongameSkillResource.GetAnimation();
			  	   animationSkill.offsetX = -350;
			  	   animationSkill.offsetY = -270;  	
			  	   animationSkill.skillID = 100000;
			  	   animationSkill.frameRate.Frequency = 6;
			  	   animationSkill.StartClip("jn");	   
			  	   player.addChild(animationSkill);
			  	   player.playerEffect = animationSkill;
			  	   animationSkill.gameScene = GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer;
			  	   player.skillAnimation.push(animationSkill);
	       	   }
	       }	
	       else
	       {
	       		FloorEffectController.playlist.push(player);	
	       }
		}
		
		/**添加地面效果 比如圣诞光影 礼花**/
		public static function AddFlootEffect(player:GameElementAnimal,flootID:int):void
		{
			//判断是否加载了地面效果
	       if(GameCommonData.FloorEffectList[flootID] == null)
	       {       	
	       	    var gameSkillResource:GameSkillResource = new GameSkillResource();
	       	    gameSkillResource.OnLoadEffect = LoadEffect;
	       	    gameSkillResource.SkillID      = flootID;
		        gameSkillResource.playerID     = player.Role.Id;
		        gameSkillResource.EffectPath   = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF +flootID+".swf";
		        gameSkillResource.EffectBR.LoadComplete = gameSkillResource.onEffectComplete;
		        gameSkillResource.EffectBR.Download.Add(gameSkillResource.EffectPath);
		        gameSkillResource.EffectBR.Load();
		        GameCommonData.FloorEffectList[flootID] = flootID;			       				        			        		        					        
	       } 		  
	       else if (GameCommonData.FloorOnLoadEffectList[flootID] != null)
	       {
	       	   var ongameSkillResource:GameSkillResource = GameCommonData.FloorOnLoadEffectList[flootID] as GameSkillResource;				
		  	   var animationSkill:SkillAnimation = ongameSkillResource.GetAnimation();
		  	   animationSkill.offsetX = player.GameX;
		  	   animationSkill.offsetY = player.GameY;  	   
		  	   animationSkill.StartClip("jn");	   
	  	   	   GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(animationSkill);
		  	   animationSkill.gameScene = GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer;
		  	   animationSkill.PlayComplete = ScenePlayComplete;	  	    	
		  	   //修改   
		  	   setTimeout(MustDeleteHeffect,4000,animationSkill);
		  	   player.skillAnimation.push(animationSkill); 
	       }	
		}
		
		/**移除人场景上的技能效果**/
		public static function ScenePlayComplete(animationSkill:SkillAnimation):void
		{
			if(animationSkill != null && animationSkill.gameScene.contains(animationSkill))
		    {				
				animationSkill.gameScene.removeChild(animationSkill);
				animationSkill = null;
		    }
		}
		
		
		//删除地面效果
		public static function MustDeleteHeffect(animationSkill:SkillAnimation):void
		{
			try
        	{
	            //移除飞行物体		
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.removeChild(animationSkill);	
				animationSkill.IsPlayComplete = true;		
			    animationSkill                =  null;
        	}
        	catch(e:Error)
        	{
        		
        	}
		}
		
		
	}
}