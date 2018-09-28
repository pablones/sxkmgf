package Controller
{
	import GameUI.ConstData.EventList;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	
	import flash.geom.Point;
	
	public class TransferSceneController
	{
		 //是否开启转场控制
         public static var IsTransferScene:Boolean = false;
         public static var IsStart:Boolean         = true;
         //同意进入PK场地
         public static var IsCheckPKScene:Boolean = true;
	     //弹出PK面板时间
         public static var PKShowTime:Number = 0;

		 public static function IsPKMap(mapID:String):Boolean
		 {		 	
			  if(mapID == "1001" ||
				 mapID == "1006" ||
		       	 mapID == "1021" ||
		       	 mapID == "1022" ||
		       	 mapID == "1023" ||
		       	 mapID == "1024" ||
		       	 mapID == "1025" ||
		       	 mapID == "1034" ||
		       	 mapID == "1036" ||
		       	 mapID == "1038" ||
		       	 mapID == "1040" )
		       {
		       		return false;
		       }
		        else
		        {
		        	return true;
		        }
		 } 
		 
		 public static function MoveToScene(movepoint:Point):void
		 {
		 	var nowdate:Date = new Date();
		 	PKShowTime = nowdate.time;
		 	TransferSceneController.IsCheckPKScene = false;
		 	if(GameCommonData.Scene.IsPlayerWalk())
			{			
		    	GameCommonData.Scene.PlayerMove(MapTileModel.GetTilePointToStage(movepoint.x,movepoint.y),false);
		    }
		 }
		 
		 public static function MoveCancel():void
		 {
		 }
		 
		 public static function clickTickFn(isClick:Boolean):void
		 {
		 	IsStart = !isClick;
		 }
		 		 		 
		 public static function IsNeedCheckPKScene(mapID:String):Boolean
		 {
		 	if(GameCommonData.GameInstance.GameScene.GetGameScene.MapId !=  mapID)
		 	{
			 	if(IsPKMap(mapID) && IsTransferScene)
			 	{
			 		return true;
			 	}
			 	else
			 	{
			 		return false;
			 	}
		 	}
		 	else 
		 	{
		 		return false;
		 	}	 			 	
		 }
		 
		 public static function TransferSceneCheckPoint(Fpoint:Point,Spoint:Point):Boolean
		 {
		 	var nowdate:Date = new Date();
		 	if(IsTransferScene && IsStart && TransferSceneController.IsCheckPKScene && nowdate.time - PKShowTime >= 10000)
		 	{		 		
				for each(var object:XML in GameCommonData.GameInstance.GameScene.GetGameScene.ConfigXml.Element)
		        {
		        	if(IsPKMap(object.@To))
		        	{        			        		
			        	var ScenePoint:Point = new Point(object.@X,object.@Y);
						var MovePoint:Point  = new Point(object.@MX,object.@MY);
						
			            //如果是站在传送点上
		        		if(DistanceController.Distance(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),ScenePoint,2))
		        		{
		        			return true;
		        		}
			        	
			        	if(Fpoint != null)
			        	{
			        		if(DistanceController.Distance(Fpoint,ScenePoint,2))
			        		{
			        			if(object.@L != null)
					        	{
					        		var level:int = object.@L;
					        		if(GameCommonData.Player.Role.Level < level)
					        		{
					        			UIFacade.UIFacadeInstance.showPrompt(level + GameCommonData.wordDic["con_tran_checkpoint_1"],0xffff00); //  "级以上才能进入，赶紧去升级吧"
					        			GameCommonData.Scene.PlayerStop(false);
					        			return false;
					        		}
					        	}	
					        	  UIFacade.UIFacadeInstance.sendNotification(EventList.SHOW_TICK_ALERT,{comfrim:MoveToScene,cancel:MoveCancel,extendsFn:MoveCancel,info:GameCommonData.wordDic["con_tran_checkpoint_2"],params:ScenePoint,title:GameCommonData.wordDic["con_tran_checkpoint_3"],clickTickFn:clickTickFn});	    
					        	                                                                                                                      // "  此场景为可强制PK场景，是否进入"  "PK场景提示"  			
			        			 //UIFacade.UIFacadeInstance.sendNotification(EventList.SHOWALERT,{comfrim:MoveToScene,cancel:MoveCancel,extendsFn:MoveCancel,info:"  此场景为可强制PK场景，是否进入",params:ScenePoint,title:"PK场景提示"});
			        			 GameCommonData.Scene.PlayerStop(false);
			        			 return false;
			        		}
			        	}
			        	if(Spoint != null)
			        	{
			        		if(DistanceController.Distance(Spoint,ScenePoint,2))
			        		{	        			
			        			if(object.@L != null)
					        	{
					        		level = object.@L;
					        		if(GameCommonData.Player.Role.Level < level)
					        		{
					        			UIFacade.UIFacadeInstance.showPrompt(level + GameCommonData.wordDic["con_tran_checkpoint_1"],0xffff00);
					        			GameCommonData.Scene.PlayerStop(false);
					        			return false;
					        		}
					        	}
					        	UIFacade.UIFacadeInstance.sendNotification(EventList.SHOW_TICK_ALERT,{comfrim:MoveToScene,cancel:MoveCancel,extendsFn:MoveCancel,info:GameCommonData.wordDic["con_tran_checkpoint_2"],params:ScenePoint,title:GameCommonData.wordDic["con_tran_checkpoint_3"],clickTickFn:clickTickFn});	        			
			        			//UIFacade.UIFacadeInstance.sendNotification(EventList.SHOWALERT,{comfrim:MoveToScene,cancel:MoveCancel,extendsFn:MoveCancel,info:"  此场景为可强制PK场景，是否进入",params:ScenePoint,title:"PK场景提示"});    		
			        			GameCommonData.Scene.PlayerStop(false);
			        			return false;	
			        		}
			        	}
		        	}	        	
		        }		        
		        return true; 		
		 	}
		 	return true;
		 }
	}
}