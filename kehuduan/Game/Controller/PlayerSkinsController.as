package Controller
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.PlayerActionSend;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Role.SkinNameController;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayerSkin;
	import OopsEngine.Skill.JobGameSkill;
	
	import flash.display.BlendMode;
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PlayerSkinsController
	{
		/** 切换装备
		 * accouterID    装备编号
		 * player        人物对象
		 * IsAccouter    装备 还是取下
		 * */
		public static function SetAccouter(accouterID:int,player:GameElementAnimal,IsAccouter:Boolean = true):void
		{			
			//装备类别
			var skinType :String = "";
			
			//武器
			if(accouterID  >= 140000 && accouterID < 149999)
			{
				skinType = GameElementSkins.EQUIP_WEAOIB;
			}
			
			//衣服
			if(accouterID  >= 230000 && accouterID  <  239999)
			{
				skinType = GameElementSkins.EQUIP_PERSON;
			}
				
			//是否可以换装 装备类别
			if(skinType.length > 0)
			{
				//判断是换上还是取下
				if(IsAccouter)
				{
					 if(skinType == GameElementSkins.EQUIP_PERSON)
					 {
					 	var data:Array =[0,GameCommonData.Player.Role.Id,0,0,1,0,299,0,0];
					 	var obj:Object={type:1010,data:data};
						PlayerActionSend.PlayerAction(obj);
						data = null;
						obj = null;
					 }
					 							 
				}
				else
				{
					if(skinType == GameElementSkins.EQUIP_PERSON)
					{
						var data0:Array =[0,GameCommonData.Player.Role.Id,0,0,0,0,299,0,0];
					 	var obj0:Object={type:1010,data:data0};
						PlayerActionSend.PlayerAction(obj0);
						data0 = null;
						obj0 = null;
					}	
					if(skinType == GameElementSkins.EQUIP_WEAOIB)
					{
						
					}
				}
			}
		}
		
		/**设置更新皮肤名称**/
		public static function GetSkinName(player:GameElementAnimal):SkinNameController
		{				
			var skinNameController:SkinNameController = new SkinNameController();
			      
            //判断是否显示时装
            if(player.Role.IsShowDress)
            {            	         	
            	//判断是否有时装
	            if(player.Role.PersonSkinID == 0)
	            {
	            	//没有时装则为其默其职业所对应的时装
                	skinNameController.PersonSkinName =  GetDress(player,true);
                	
                	//则显示当前武器
                	skinNameController.WeaponSkinName = GetWeapen(player,skinNameController);
	            }
	            else
	            {
	            	//判断该时装是否是可以用 切换职业时候需要判断 并且该时装是否所有门派都可以使用
            		if(UIConstData.getItem(player.Role.PersonSkinID).Job  == player.Role.CurrentJobID || UIConstData.getItem(player.Role.PersonSkinID).Job == 0)
            		{
            			//获取时装
            			skinNameController.PersonSkinName =  GetDress(player,false);   
            			
            			//获取时装武器
            			skinNameController.WeaponSkinName =  GetDressWeapon(player,skinNameController);         			        			
            		}
            		else
            		{
            			//时装不能显示
            			skinNameController.PersonSkinName =  GetDress(player,true);
            			
            			//则显示当前武器
                		skinNameController.WeaponSkinName = GetWeapen(player,skinNameController);
            		}
	            }
            }
            else
            {
            	//没有选择时装则为其默其职业所对应的时装
                skinNameController.PersonSkinName =  GetDress(player,true);
                
                //则显示当前武器
				skinNameController.WeaponSkinName = GetWeapen(player,skinNameController);
            }
            return skinNameController;
		}
		
		
		public static function SetSkinData(player:GameElementAnimal,skinNameController:SkinNameController):void
		{
			//皮肤更换
			if(player.Role.PersonSkinName !=  skinNameController.PersonSkinName)
			{							 	
				var mcLevUp:MovieClip;  
				mcLevUp = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("McLevUp") as MovieClip;
				mcLevUp.alpha = 0.85;
						

				mcLevUp.x = player.GameX;
				mcLevUp.y = player.GameY - 40;
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.addChild(mcLevUp);
				
				
				//更换皮肤
				if(player.Role.WeaponSkinName != null && player.Role.WeaponSkinName.length >0)
				{
					player.Role.WeaponSkinName = null;
				    player.Skins.ChangeSkins = OnChangeSkins;		
					RemoveSkin(GameElementSkins.EQUIP_WEAOIB,player);
				}
				
				//要下坐骑
				if(player.Role.MountSkinName != null && player.Role.MountSkinName.length > 0 )
				{
					player.Role.MountSkinName = null;
                    player.MountOffset        = null;
					PlayerController.SetPlayerSpeed(player,5);
				    player.Skins.ChangeSkins = OnChangeSkins;		
					RemoveSkin(GameElementSkins.EQUIP_MOUNT,player);
				}
				
				player.Role.PersonSkinName = skinNameController.PersonSkinName;			
				player.SetDirection(2);
				
				var playerData:XML;
				if(player.Role.IsShowDress == false || player.Role.PersonSkinID == 0)
					playerData = GameCommonData.ModelOffsetPlayer[GetOffsetPlayer(0,player.Role.Sex,player.Role.CurrentJobID)];
			    else
			        playerData = GameCommonData.ModelOffsetPlayer[GetOffsetPlayer(player.Role.PersonSkinID,player.Role.Sex,player.Role.CurrentJobID)];  
			        
		        if(playerData!=null)
			    {
				    player.Offset		= new Point(playerData.@X,playerData.@Y);		 // 时装偏移值		
				    player.OffsetHeight = playerData.@H;
			    }
		        if(player.Skins != null )
                {      
					player.Skins.ChangeSkins = OnChangeSkins;	
					player.Role.skinNameController = skinNameController;
					player.SetPlayer   = SetPlayer;									
					player.SetSkin(GameElementSkins.EQUIP_PERSON,player.Role.PersonSkinName);				
                }
			}	
			else
			{
				SetPlayer(player,skinNameController);
			}
			

		}
		
		public static function SetPlayer(player:GameElementAnimal,skinNameController:SkinNameController):void 
		{
			player.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
			player.Role.WeaponDiaphaneity = skinNameController.WeaponDiaphaneity;
			player.Role.WeaponEffectName  = skinNameController.WeaponEffectSkinName;
			//判断是卸下还是安装武器
			if(skinNameController.WeaponSkinName == null)
			{
				if(player.Role.WeaponSkinName != null && player.Role.WeaponSkinName.length >0)
				{
					player.Role.WeaponSkinName = null;
				    player.Skins.ChangeSkins = OnChangeSkins;		
					RemoveSkin(GameElementSkins.EQUIP_WEAOIB,player);
				}
			}
			else
			{
				//判断武器是否是同一把武器
		    	if(skinNameController.WeaponSkinName != player.Role.WeaponSkinName)
		    	{
		    		player.Role.WeaponSkinName = skinNameController.WeaponSkinName;
		    		if(player.Skins != null)
		    		{
			    		player.Skins.ChangeSkins = OnChangeSkins;		
						player.SetSkin(GameElementSkins.EQUIP_WEAOIB,player.Role.WeaponSkinName);	
		    		}
		    	}
		    	else
		    	{
	    			player.Skins.ChangeSkins = OnChangeSkins;
	    			player.SetSkin(GameElementSkins.EQUIP_WEAOIBEFFECT,player.Role.WeaponEffectName);				    			
		    	}
			}
			
			//设置坐骑
			SetSkinMountData(player);
		}
		
		//是否显示武器特效
		public static function IsShowWeaponEffect(nDress:uint):Boolean
		{
			if(nDress == 142000 || nDress == 141000 || nDress == 144000)
				return false;
			else
				return true;
		}
		
        
        /**获取人物偏移值**/
        public static function GetOffsetPlayer(nDress:uint,nSex:uint,job:int):String
        {	
        	var jobgameskill:JobGameSkill;
        	if(nDress==0)
			{
			    jobgameskill = GameCommonData.JobGameSkillList[job] as JobGameSkill;
				return jobgameskill.DressID + "_" + nSex;		
			}
			else
			{
				if(UIConstData.getItem(nDress).Job  == job || UIConstData.getItem(nDress).Job == 0) 
				{
					return nDress + "_" + nSex;
				}
				else
				{
				    jobgameskill = GameCommonData.JobGameSkillList[job] as JobGameSkill;
				    return jobgameskill.DressID + "_" + nSex;		
				}
			}
        }
     	
		/**获取时装   是否更据职业获取默认时装**/
		public static function GetDress(player:GameElementAnimal,IsJobDefault:Boolean):String
		{
			//是否默认职业套装
			if(IsJobDefault)			
			{
				var jobgameskill:JobGameSkill;
				jobgameskill = GameCommonData.JobGameSkillList[player.Role.CurrentJobID] as JobGameSkill;
				return "Resources/Player/Person/" + jobgameskill.DressID + "_" + player.Role.Sex + ".swf";
			}
			else
			{
				return "Resources/Player/Person/" + player.Role.PersonSkinID + "_" + player.Role.Sex + ".swf";
			}
		}
			
 
        /**通过武器ID获取武器类型ID（显示对应的效果编号）**/
        public static function GetWeapen(player:GameElementAnimal,skinNameController:SkinNameController):String
        {     	
        	//判断改武器是否在武器列表里
            if(UIConstData.getItem(player.Role.WeaponSkinID) != null && IsCanUse(player.Role.WeaponSkinID,player.Role.CurrentJobID))
            {
            	//设置武器光影
            	skinNameController.WeaponEffectSkinName = GetWeapenEffect(player,UIConstData.getItem(player.Role.WeaponSkinID).modelType,skinNameController);
				return "Resources/Player/Weapon/" + UIConstData.getItem(player.Role.WeaponSkinID).modelType + "_" + player.Role.Sex + ".swf";				
            }
			else
			{
				skinNameController.WeaponEffectSkinName = null;
				skinNameController.WeaponEffectModelName = null;
				skinNameController.WeaponDiaphaneity  = 1;
			    return null;
			}
        }
        
        /**根据时装获取对应的武器**/
		public static function GetDressWeapon(player:GameElementAnimal,skinNameController:SkinNameController):String
		{		 		
			var weaponData:XML = GameCommonData.ModelOffsetPlayer[GetOffsetPlayer(player.Role.PersonSkinID,player.Role.Sex,player.Role.CurrentJobID)];
			if(weaponData != null)
			{
				var waeponName:String = weaponData.@W;
				
				// 浏览时装特殊处理
				if(player.Role.isSkinTest)
				{
					player.Role.WeaponSkinID = weaponData.@W;
				}
				
				if(waeponName.length == 0)
				{					
				    skinNameController.WeaponEffectSkinName = null;
					return null;
				}
				else
				{
					if(UIConstData.getItem(player.Role.WeaponSkinID) != null && IsCanUse(player.Role.WeaponSkinID,player.Role.CurrentJobID))
            		{		
            		    skinNameController.WeaponEffectSkinName = GetWeapenEffect(player,int(waeponName),skinNameController);			   
            		}
            		else
            		{
            		   skinNameController.WeaponEffectSkinName = null;
					   skinNameController.WeaponEffectModelName = null;
					   skinNameController.WeaponDiaphaneity  = 1;
            		}
            		return "Resources/Player/Weapon/" + waeponName + "_" + player.Role.Sex + ".swf";
				}
			}
			else
			{		
				skinNameController.WeaponEffectSkinName = null;
				return null;
			}			
		}
        
		
		/** 获取武器特效数据  WeaponID 可以是当前武器 也可能是时装的武器 */
		public static function GetWeapenEffect(player:GameElementAnimal,WeaponID:int,skinNameController:SkinNameController):String
		{		
			var WeapenEffectName:String
			
			//武器是否发光
			if(player.Role.WeaponEffectModel != 0)
			{			
				//判断该武器是否是特殊武器
				if(GameCommonData.WeaponEffect[WeaponID] != null)
				{	
					var weaponEffectData:XML = GameCommonData.WeaponEffect[WeaponID];	
					//是否拥有数据
					if(weaponEffectData != null)
					{
						var WeapenEffect:String = weaponEffectData.@E;	
						if(WeapenEffect.length != 0)
							WeapenEffectName = "Resources/Player/WeaponEffect/" + WeapenEffect + "_" + player.Role.Sex + ".swf";		
						else
							WeapenEffectName = "Resources/Player/WeaponEffect/" + player.Role.CurrentJobID + "_" + player.Role.Sex + ".swf";
					}
					else
						WeapenEffectName = "Resources/Player/WeaponEffect/" + player.Role.CurrentJobID + "_" + player.Role.Sex + ".swf";
				}
				else
				{
					WeapenEffectName = "Resources/Player/WeaponEffect/" + player.Role.CurrentJobID + "_" + player.Role.Sex + ".swf";
				}   
				
				//设置发光
				GetWeapenEffectModel(player,WeaponID,skinNameController);
			}
			else
			{
				skinNameController.WeaponDiaphaneity = 1;
				skinNameController.WeaponEffectModelName = null;
			}
			return WeapenEffectName;
		}
        
        /**获取模式编号**/
        public static function GetWeapenModel(modelID:int,skinNameController:SkinNameController):void
        {
        	skinNameController.WeaponDiaphaneity = 1;
        	var imageModel:String;
        	switch(modelID)
         	{
             	 case 1:
             	 	imageModel = BlendMode.NORMAL;           	 	
             	 	break;
             	 case 2:
             	    imageModel = BlendMode.LAYER;
             	    break;
             	 case 3:  
             	   	imageModel = BlendMode.DARKEN;
             	   	break;
             	 case 4:  
             	   	imageModel = BlendMode.MULTIPLY;
             	   	break;
             	 case 5:  
             	   	imageModel = BlendMode.LIGHTEN;
             	    break;
             	 case 6:  
             	   	imageModel = BlendMode.SCREEN;
             	    break;
             	 case 7:  
             	   	imageModel = BlendMode.OVERLAY;
             	    break;
             	 case 8:  
             	   	imageModel = BlendMode.HARDLIGHT;
             	    break;
             	 case 9:  
             	   	imageModel = BlendMode.ADD;
             	    break;
             	 case 10:  
             	   	imageModel = BlendMode.SUBTRACT;
             	   	skinNameController.WeaponDiaphaneity = 0.75;
             	    break;
             	 case 11:  
             	   	imageModel = BlendMode.DIFFERENCE;
             	    break;
             	 case 12:  
             	   	imageModel = BlendMode.INVERT;
             	    break;
             	 case 13:  
             	   	imageModel = BlendMode.ERASE;
             	    break;               	
            }   
            skinNameController.WeaponEffectModelName = imageModel;
        }
			
	
		/** 获取武器模式 */
		public static function GetWeapenEffectModel(player:GameElementAnimal, WeaponID:int,skinNameController:SkinNameController):void
		{
			//判断是否是特殊故效果的武器
			if(GameCommonData.WeaponEffect[WeaponID] != null)
			{
				var weaponEffectData:XML = GameCommonData.WeaponEffect[WeaponID];
				 switch(player.Role.WeaponEffectModel)
             	{
	             	 case 1:
	             	 	GetWeapenModel(weaponEffectData.@M1,skinNameController);
	             	 	break;
	             	 case 2:
	             	    GetWeapenModel(weaponEffectData.@M2,skinNameController);
	             	    break;
	             	 case 3:  
	             	    GetWeapenModel(weaponEffectData.@M3,skinNameController);
	             	    break;
	            }  
			}
			else
			{
				switch(player.Role.WeaponEffectModel)
             	{
	             	 case 1:
	             	 	GetWeapenModel(7,skinNameController);
	             	 	break;
	             	 case 2:
	             	    GetWeapenModel(10,skinNameController);
	             	    break;
	             	 case 3:  
	             	    GetWeapenModel(9,skinNameController);
	             	    break;
	            }    
			}
		}
		
		
		
	     /**获取坐骑信息**/
		public static function GetMount(nDress:uint):String
		{		
			if(nDress==0)
			{
				return  null;
			}
			else
			{
				return "Resources/Player/Mount/" + nDress + ".swf";
			}
		}
		
		
		
		/** 获取动物皮肤*/
		public static function GetPetSkins(nDress:uint):String
		{		 
			return  "Resources\\Enemy\\" +  nDress + ".swf"; 
		}
		  		
		
		/**是否可以使用该物品**/
		public static function IsCanUse(accouterID:int,job:int):Boolean
		{
			if((accouterID != 0 && UIConstData.getItem(accouterID) != null && UIConstData.getItem(accouterID).Job  ==  job)
			|| (accouterID == 144000 && job == 4096 ))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		/**设置坐骑数据**/
		public static function SetSkinMountData(player:GameElementAnimal):void
		{
			if(player.Role.MountSkinID == 0)			
			{
				//表示有坐骑要下坐骑
				if(player.Role.MountSkinName != null && player.Role.MountSkinName.length > 0 )
				{
					player.Role.MountSkinName = null;
                    player.MountOffset        = null;
					PlayerController.SetPlayerSpeed(player,5);
				    player.Skins.ChangeSkins = OnChangeSkins;		
					RemoveSkin(GameElementSkins.EQUIP_MOUNT,player);
				}
			}
			else
		    {
		    	//判断坐骑是否可以用
		    	if(UIConstData.getItem(player.Role.MountSkinID).Job  == player.Role.CurrentJobID)
			    {
			    	var mountSkinName:String = GetMount(player.Role.MountSkinID);
			    	if(mountSkinName != player.Role.MountSkinName)
			    	{
			    		var mountData:XML;
			    		if(player.Role.PersonSkinID == 0 || player.Role.IsShowDress == false)
			    		{
			    	    	 mountData = GameCommonData.ModelOffsetMount[player.Role.MountSkinID  + "_" + player.Role.CurrentJobID] as XML;
			    	 	}			
			    	 	else
			    	 	{
			    	 		mountData = GameCommonData.ModelOffsetMount[player.Role.MountSkinID  + "_" + player.Role.PersonSkinID] as XML;
			    	 	}	
						player.MountOffset        = new Point(mountData.@X,mountData.@Y);
						player.MountHeight	      = mountData.@H;
				        player.Role.MountSkinName = mountSkinName;
				        
				        PlayerController.SetPlayerSpeed(player,6);
				        if(player.Skins != null)
				        {
					        player.Skins.ChangeSkins = OnChangeSkins;		
						    player.SetSkin(GameElementSkins.EQUIP_MOUNT,player.Role.MountSkinName);
				        }
			    	}
			    	else
			    	{			    		
			    		if(player.Role.PersonSkinID == 0 || player.Role.IsShowDress == false)
			    		{
			    	    	 mountData = GameCommonData.ModelOffsetMount[player.Role.MountSkinID  + "_" + player.Role.CurrentJobID] as XML;
			    	 	}			
			    	 	else
			    	 	{
			    	 		mountData = GameCommonData.ModelOffsetMount[player.Role.MountSkinID  + "_" + player.Role.PersonSkinID] as XML;
			    	 	}			
			    	 			
					    player.Skins.ChangeSkins = OnChangeSkins;
					    var gameElementPlayerSkin:GameElementPlayerSkin = player.Skins as GameElementPlayerSkin;
//					    gameElementPlayerSkin.ResetMount(new Point(mountData.@X,mountData.@Y),mountData.@H);

			    	}
			    }
			    else
			    {
			    	//表示有坐骑要下坐骑
					if(player.Role.MountSkinName != null && player.Role.MountSkinName.length > 0 )
					{
						player.Role.MountSkinName = null;
	                    player.MountOffset        = null;
						PlayerController.SetPlayerSpeed(player,5);
					    player.Skins.ChangeSkins = OnChangeSkins;		
						RemoveSkin(GameElementSkins.EQUIP_MOUNT,player);
					}
			    }
		    }
			
		}	
			
		/**装备切换完成事件**/
		public static function OnChangeSkins(skinType:String,player:GameElementAnimal):void
		{
			if(skinType == GameElementSkins.EQUIP_MOUNT)
			{
				if(player.Role.Id == GameCommonData.Player.Role.Id)
				{
					GameCommonData.Scene.ResetMoveState();	
					if(GameCommonData.Player.Role.UsingPetAnimal != null)
					{							
				    	GameCommonData.Scene.PetResetMoveState();
				    	GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);
				 	}
				 	GameCommonData.Player.MustMovePoint = null;
					GameCommonData.Player.PathMap = null;
			        UIFacade.GetInstance(UIFacade.FACADEKEY).changePath(1);
				}
				else
				{				
					player.Stop();
				}
				if(player.Role.HP > 0)
				{
					player.Skins.ChangeAction(GameElementSkins.ACTION_STATIC,true);
				}

			}
			else
			{
				player.Skins.ChangeAction(player.Role.ActionState,true);
			}
			
			if(player.handler != null)
			{
				player.handler.Clear();
			}			
		}
		
        /**移除皮肤*/
		public static function RemoveSkin(skinType:String,player:GameElementAnimal):void
		{
			 player.RemoveSkin(skinType);
		}	
		
		/**上坐骑**/
		public static function SetMount():void
		{
			if(RolePropDatas.ItemList[9] != null &&                                                                              //判断是否穿上                                                                                                              //是否装备上坐骑
			GameCommonData.GameInstance.GameScene.GetGameScene.name == GameCommonData.GameInstance.GameScene.GetGameScene.MapId  //是否副本 
			)   
			{
			    var combat:CombatController = new CombatController();
			    combat.ReserveMount(RolePropDatas.ItemList[9].id);
		    }
		}
		
		//下坐骑
		public static function UnMount():void
		{
			if(RolePropDatas.ItemList[9] != null && GameCommonData.Player.Role.MountSkinName != null && GameCommonData.Player.Role.MountSkinName.length > 0)
			{
				 var combat:CombatController = new CombatController();
				 combat.ReserveMount(RolePropDatas.ItemList[9].id);
			}
		} 
		
		/**人物死亡 人物死亡流程是先下上坐骑 在播放死亡动画**/
		public static function PlayerDead(player:GameElementAnimal):void
		{	
			//判断是否死亡并且有坐骑
			if(player.Role.HP == 0 && player.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				if(player.Role.MountSkinName != null &&  player.Role.MountSkinName.length > 0)
				{
					player.RemoveSkin(GameElementSkins.EQUIP_MOUNT);
					player.SetAction(GameElementSkins.ACTION_DEAD);
				}
				else
				{
					player.SetAction(GameElementSkins.ACTION_DEAD);
				}		
				
				if(player.Role.Type == GameRole.TYPE_PLAYER &&
			    	player.Role.Type == GameRole.TYPE_OWNER )	
			    {
			    	(player as GameElementPlayer).RemoveTernal();
			    }		
			}
		}
		
		/**获取宠物皮肤**/
		public static function GetPetPersonSkinName(PetSkinID:String,Model:int):String
		{
			var PetPersonSkinID:String = PetSkinID;
			if(Model != 0)
			{
				if(GameCommonData.ModelPet[PetSkinID] != null)
				{
					var modelPetXML:XML = GameCommonData.ModelPet[PetSkinID];
					switch(Model)
					{
						case 1:
						     PetPersonSkinID = modelPetXML.@M1;
							break;
						case 2:
						     PetPersonSkinID = modelPetXML.@M2;
						    break;
						case 3:
						     PetPersonSkinID = modelPetXML.@M3;
						    break;
					}
				}
			}			
			return PetPersonSkinID;
		}
		
		/**获取宠物皮肤**/
		public static function GetVariationPetPersonSkinName(PetSkinID:String,level:int):Object
		{
			var obj:Object;
			for(var n:int=1;n <= level;n++)
			{
				if(GameCommonData.VariationPet[PetSkinID.toString() + "|"] != null)
				{
					
				}
			}
		    var PetPersonSkinID:int = 0;
		    
			if(GameCommonData.VariationPet[PetSkinID] != null)
			{
				var modelPetXML:XML = GameCommonData.VariationPet[PetSkinID];
				PetPersonSkinID = modelPetXML.@M;
			}				
			return PetPersonSkinID;
		}
		
		/** 获取宠物变异xml */
		public static function GetPetV( faceType:String , level:int ):XML
		{
			var xml:XML;
			for each(var object:XML in GameCommonData.VariationPet)
	        {
	        	if( object.@Id.toString() == faceType && object.@minlevel <= level && level <= object.@maxlevel) 
	        	{
	        		xml = object;
		        	break;
	        	}
	        	xml = null;
	        }
			return xml;
		}
	}
}