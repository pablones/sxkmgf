package Net.ActionProcessor
{
	import Controller.PKController;
	import Controller.PlayerController;
	
	import GameUI.UICore.UIFacade;
	
	import Net.GameAction;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;
	
	public class PlayerSkillBuff extends GameAction
	{
		public function PlayerSkillBuff()
		{
			super(false);
		}
		

		public  override function Processor(bytes:ByteArray):void 
		{
           bytes.position = 4;
           var nAction:uint = bytes.readUnsignedInt(); //功能 1.添加,2.删除,3.更新
           var idUser:uint = bytes.readUnsignedInt(); // 用户ID
           var buffID:uint = bytes.readUnsignedInt(); //状态类型
           var nTime:int   = bytes.readInt();         //持续时间或者间隔时间
           var nCount:uint = bytes.readUnsignedInt(); //多次状态的次数			
           var nPower:uint = bytes.readUnsignedInt(); // 效果数值
           var nData7:uint = bytes.readUnsignedInt(); //int nParam1;
           var nData8:uint = bytes.readUnsignedInt(); //int nParam2;
           var nData9:uint = bytes.readUnsignedInt(); //int nParam3;          
   			
   		   if(nCount != 0)
   		   {
   		   	  	nTime = nTime * nCount
   		   	    if(buffID == 34999) nTime = -1;
   		   }	
   			
           if(GameCommonData.Player != null)
           {           			
	           var player:GameElementAnimal =  PlayerController.GetPlayer(idUser);
	           var useid:int = 0;             

	           if(player != null)
	           {    	
	           		if(player.Role.Id != GameCommonData.Player.Role.Id)
		           {
		           		useid = player.Role.Id;
		           }
		           
	           	   if(buffID != 54003) 
	           	   {
	           	       var buffInfo:GameSkillBuff = GameCommonData.BuffList[buffID] as GameSkillBuff ;
		           	   if(buffInfo != null)
		           	   {
			           	   var buff:GameSkillBuff = new GameSkillBuff();
	
			           	   buff.BuffID =  buffID;
			           	   buff.BuffTime = nTime;
			           	   buff.BuffType = buffInfo.BuffType;
			           	   buff.BuffName = buffInfo.BuffName;
			           	   buff.TypeID   = buffInfo.TypeID;

				           switch(nAction)
				           {
				           	    case 1:			           	    	
				           	        if(!GameSkillBuff.IsBuff(buff.BuffType))
				           	        {
				           	        	player.Role.UpdateDot(buff);
				           	        	UIFacade.GetInstance(UIFacade.FACADEKEY).changeBuffStatus(4,useid,buff); 
				           	        }
				           	        else
				           	        {
				           	        	player.Role.UpdateBuff(buff);
				           	        	UIFacade.GetInstance(UIFacade.FACADEKEY).changeBuffStatus(1,useid,buff); 
				           	        }
				           	        break;
				           		case 2:
				           		    //是DOT 还是 BUFF
		 		           	        if(!GameSkillBuff.IsBuff(buff.BuffType))
				           	        {   
				           	        	//判断 数组 是否存在值
				           	        	if(player.Role.DotBuff.length  > 0)
				           	        	{  
				           	        		//是否删除成功  	        	
					           	        	if(player.Role.DelteDot(buff))
					           	        		UIFacade.GetInstance(UIFacade.FACADEKEY).changeBuffStatus(6,useid,buff); 
				           	        	}
				           	        }
				           	        else
				           	        {
				           	        	if(player.Role.PlusBuff.length  > 0)
				           	        	{
				           	        		if(player.Role.DelteBuff(buff))
				           	        			UIFacade.GetInstance(UIFacade.FACADEKEY).changeBuffStatus(3,useid,buff); 
				           	        	}
				           	        }
				           		break;
				           		case 3:
				           		    if(!GameSkillBuff.IsBuff(buff.BuffType))
				           	        {
				           	        	player.Role.UpdateDot(buff);
				           	        	UIFacade.GetInstance(UIFacade.FACADEKEY).changeBuffStatus(5,useid,buff); 
				           	        }
				           	        else
				           	        {
				           		     	player.Role.UpdateBuff(buff);
				           		     	UIFacade.GetInstance(UIFacade.FACADEKEY).changeBuffStatus(2,useid,buff); 
				           	        }
				           		break;	           		              		
				           } 
				          
				           if((buffID == 21028 || buffID == 21025) && nAction == 1)
				           {
				           		player.Setvariation(2);
				           }
				           if(buffID == 21029 && nAction == 1)
				           {
				           		player.Setvariation(1);
				           }
				           
				           if((buffID == 21028 || buffID == 21025) && nAction == 2)
				           {
				           		player.Setvariation(0);
				           }
				           if(buffID == 21029 && nAction == 2)
				           {
				           		player.Setvariation(0);
				           }
		           	   }
	           	    }
	           	    else
	           	    {
	           	    	 switch(nAction)
				         {
				         	 case 1:
								player.Role.NameColor       = "#fd70ff";
								player.Role.NameBorderColor = 0x000000;
								player.UpdatePersonName();
				         	    break;
				         	 case 2:
				         	    player.Role.NameColor       = PKController.GetFontColor(player.Role.PkValue);
								player.Role.NameBorderColor = PKController.GetBorderColor(player.Role.PkValue);
								player.UpdatePersonName();
				         	    break;
				         	 case 3:
								player.Role.NameColor       = "#fd70ff";
								player.Role.NameBorderColor = 0x000000;
								player.UpdatePersonName();
				         	  break;
				         }
	           	    }
			     }       
           }
		}	

	}
}