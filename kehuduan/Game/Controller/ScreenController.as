package Controller
{
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	
	public class ScreenController
	{
		//是否可以切换 
		public static var IsCanChangeScreen:Boolean = true;
		
		public static var NowScenePlayerCount:int = 0;
		
		public function ScreenController()
		{
		}
        
        /**设置当前屏蔽的状态**/ // 0 屏蔽  1 显示玩家  2 显示宠物
        public static function SetScreen():void
        {
        	//判断是否可以执行屏蔽
        	if(IsCanChangeScreen)
        	{
        		IsCanChangeScreen = false;
	        	if(GameCommonData.Screen != 2)
	        	{
	        		GameCommonData.Screen += 1;
	        	}
	        	else
	        	{
	        		GameCommonData.Screen = 0;
	        	}
	        	
	        	switch(GameCommonData.Screen)
	        	{
	        		case 0:ScreenAll();break;
	        	    case 1:ShowPet();break;		
	        		case 2:ScreenNone();break;
	        	}
	        	IsCanChangeScreen = true;
        	}
        }
        
        /**屏蔽宠物**/
        public static function ShowPet():void
        {
        	for(var playerName:String in GameCommonData.SameSecnePlayerList)
			{	
                var player:GameElementAnimal = GameCommonData.SameSecnePlayerList[playerName];
			    if(player.Role.Type == GameRole.TYPE_PLAYER)
			    {
					if(!player.Role.isHidden && player.Enabled)
					{
						player.Visible = true;	
						if(player.Role.gameElementTernal != null)
						{
							player.Role.gameElementTernal.Visible = true;
						}				
					}
				}
			}
        }
        
        /**屏蔽所有**/
        public static function ScreenAll():void
        {
        	for(var playerName:String in GameCommonData.SameSecnePlayerList)
			{
				var player:GameElementAnimal = GameCommonData.SameSecnePlayerList[playerName];
				if(player.Role.Type == GameRole.TYPE_PET || player.Role.Type == GameRole.TYPE_PLAYER)
				{
					//不是自己的宠物
					if(!(GameCommonData.Player.Role.UsingPetAnimal != null &&
					GameCommonData.Player.Role.UsingPetAnimal.Role.Id  ==  player.Role.Id))
					{
						player.Visible = false;
					}
					
					// 不显示武魂
					if(player.Role.Type == GameRole.TYPE_PLAYER)
					{
						if(player.Role.gameElementTernal != null)
						{
							player.Role.gameElementTernal.Visible = false;
						}
					}
				}
			}
        }
        
        /**显示所有**/
        public static function ScreenNone():void
        {
        	for(var playerName:String in GameCommonData.SameSecnePlayerList)
			{
				var player:GameElementAnimal = GameCommonData.SameSecnePlayerList[playerName];
				if(player.Role.Type == GameRole.TYPE_PET || player.Role.Type == GameRole.TYPE_PLAYER)
				{
					if(!player.Role.isHidden && player.Enabled)
						player.Visible = true;
						
					// 显示武魂
					if(player.Role.Type == GameRole.TYPE_PLAYER)
					{
						if(player.Role.gameElementTernal != null)
						{
							player.Role.gameElementTernal.Visible = true;
						}
					}
				}
			}
        }
        
        /**添加宠物和人物信息**/
        public static function AddPlayer(player:GameElementAnimal):void
        {      	      	        		
     	
        	//人物信息
        	if(player.Role.Type == GameRole.TYPE_PLAYER)
        	{      
        		//判断是否超过最大数量
        		if(NowScenePlayerCount <= GameCommonData.SameSecnePlayerMaxCount)
        		{
        			//添加人物信息
        			GameCommonData.Scene.AddPlayer(player);
        		}
        		else
        		{
        			//添加待添加场景的人物信息
        			GameCommonData.VisibleSameSecnePlayerList[player.Role.Id] = player;
        		}
        	} 
        	//宠物信息
        	else if(player.Role.Type == GameRole.TYPE_PET)
        	{
        		
        	}  	
        	
        	//增加一个场景元素
        	NowScenePlayerCount += 1; 
        }
        
        /**删除宠物和人物信息**/
        public static function DeletePlayer(player:GameElementAnimal):void
        {
        	//人物信息
        	if(player.Role.Type == GameRole.TYPE_PLAYER)
        	{    
        		 //判断是否超过最大数量
        		if(NowScenePlayerCount < GameCommonData.SameSecnePlayerMaxCount - 1)
        		{
        			//添加人物信息
        			 for(var playerName:String in GameCommonData.VisibleSameSecnePlayerList)
        			 {
        			 	var p:GameElementAnimal = GameCommonData.VisibleSameSecnePlayerList[playerName];
        			 	GameCommonData.Scene.AddPlayer(p);
        			 	GameCommonData.VisibleSameSecnePlayerList[playerName] = null;
        			 	
        			 	//判断该玩家是否有宠物
        			 	if(p.Role.UsingPetAnimal != null)
        			 	{
        			 		//判断宠物是否在不显示的列表中
        			 		if(GameCommonData.VisibleSameSecnePlayerList[p.Role.UsingPetAnimal.Role.Id] != null)
        			 		{
        			 			var pet:GameElementAnimal = GameCommonData.VisibleSameSecnePlayerList[p.Role.UsingPetAnimal.Role.Id];
		        			 	GameCommonData.Scene.AddPlayer(pet);
		        			 	GameCommonData.VisibleSameSecnePlayerList[p.Role.UsingPetAnimal.Role.Id] = null;
        			 		}
        			 	}
        			 }

        		}
        	}
        	else
        	{
//        		//判断是否超过最大数量
//        		if(NowScenePlayerCount <= GameCommonData.SameSecnePlayerMaxCount)
//        		{
//        			//添加人物信息
//        			GameCommonData.Scene.AddPlayer(player);
//        		}
        	}
        	
        	//判断该人物是否显示
        	if(GameCommonData.VisibleSameSecnePlayerList[player.Role.Id] == null)
        	{
        		//珊瑚场景中显示的人或宠物
        		GameCommonData.Scene.DeletePlayer(player.Role.Id);
        	}
        	else
        	{
	        	//删除场景中的不显示的人或宠物
	        	GameCommonData.VisibleSameSecnePlayerList[player.Role.Id] = null;
	        	GameCommonData.SameSecnePlayerList[player.Role.Id]        = null;
        	}
            	
            //减少一个场景中的人或宠物
        	NowScenePlayerCount -= 1;  
        }
	}
}