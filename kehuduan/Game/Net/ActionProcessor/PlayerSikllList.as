package Net.ActionProcessor
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.MouseCursor.RepeatRequest;
	
	import Net.GameAction;
	
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.utils.ByteArray;

	public class PlayerSikllList extends GameAction
	{
		public function PlayerSikllList(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
					
			var action:uint = bytes.readUnsignedInt(); //action
		    bytes.readUnsignedInt(); //用户ID						
		    var familiar:int = bytes.readUnsignedInt(); //技能经验  熟练度
		    
			var  skillID:int	  = bytes.readUnsignedShort();//技能类型ID
		    var  level:int        = bytes.readUnsignedShort();//技能等级
		    if ( skillID == 2501 )
		    {
		    	RepeatRequest.getInstance().skillItemCount+=1;
		    	return;					// 隐形的回帮技能
		    } 
		    
		    var gameSkillLevel:GameSkillLevel;
		    if ( skillID>6000 && skillID<7000 )
		    {
		    	gameSkillLevel = new GameSkillLevel(GameCommonData.LifeSkillList[skillID] as GameSkill);
//		    	gameSkillLevel.Familiar = familiar;
		    }
		    else
		    { 
			    gameSkillLevel = new GameSkillLevel(GameCommonData.SkillList[skillID] as GameSkill);
		    }
		    
		    //更新帮派技能
			if ( skillID>2100 && skillID<2500 )
			{
				var index:int;
				if ( ( UnityConstData.greenUnityDataObj.skillIconList as Array ).indexOf( skillID ) > -1 )
				{
					index = ( UnityConstData.greenUnityDataObj.skillIconList as Array ).indexOf( skillID );
					UnityConstData.greenUnityDataObj[ "skillStudySelf"+int(index+1) ] = level;
				}
				else if ( ( UnityConstData.whiteUnityDataObj.skillIconList as Array ).indexOf( skillID ) > -1 )
				{
					index = ( UnityConstData.whiteUnityDataObj.skillIconList as Array ).indexOf( skillID );
					UnityConstData.whiteUnityDataObj[ "skillStudySelf"+int(index+1) ] = level;
				}
				else if ( ( UnityConstData.redUnityDataObj.skillIconList as Array ).indexOf( skillID ) > -1 )
				{
					index = ( UnityConstData.redUnityDataObj.skillIconList as Array ).indexOf( skillID );
					UnityConstData.redUnityDataObj[ "skillStudySelf"+int(index+1) ] = level;
				}
				else if ( ( UnityConstData.xuanUnityDataObj.skillIconList as Array ).indexOf( skillID ) > -1 )
				{
					index = ( UnityConstData.xuanUnityDataObj.skillIconList as Array ).indexOf( skillID );
					UnityConstData.xuanUnityDataObj[ "skillStudySelf"+int(index+1) ] = level;
				}
			}
		    
		    gameSkillLevel.Level = level;
		    gameSkillLevel.Familiar = familiar;

			if(action == 0)      //学新技能
			{
				RepeatRequest.getInstance().skillItemCount+=1;
				if ( skillID>6000 && skillID<7000 )
				{
					GameCommonData.Player.Role.LifeSkillList[skillID] = gameSkillLevel;
				}
				else
				{
					GameCommonData.Player.Role.SkillList[skillID] = gameSkillLevel;
				}
			}
			//技能升级
			if( action==3 || action==4 )
			{
				sendNotification(SkillConst.NEWSKILL_RECEIVE,{id:skillID,level:level,familiar:familiar});
			}
			
			if ( action==5 )
			{
				if ( GameCommonData.Player.Role.LifeSkillList[skillID] )
				{
					( GameCommonData.Player.Role.LifeSkillList[skillID] as GameSkillLevel ).Familiar = familiar;
				}
			} 
			
			
		}
		
	}
}