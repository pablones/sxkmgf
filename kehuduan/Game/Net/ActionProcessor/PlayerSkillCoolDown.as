package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.MouseCursor.RepeatRequest;
	
	import Net.GameAction;
	
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	
	import flash.utils.ByteArray;
	
	/**
	 * 技能与物品cd协议 
	 * @author net
	 * 
	 */	
	public class PlayerSkillCoolDown extends GameAction
	{
		public function PlayerSkillCoolDown(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public  override function Processor(bytes:ByteArray):void 
		{				
			var obj:Object={}		
            obj.idUser = bytes.readUnsignedInt();//用户ID
            obj.action = bytes.readUnsignedShort();//功能
            obj.infotype = bytes.readUnsignedShort();//冷却类型
            obj.magicid = bytes.readUnsignedInt();//type
            obj.privateCdTime = bytes.readUnsignedInt();//私有冷却时间
            obj.publicCdTime = bytes.readUnsignedInt();//公共冷却时间
             
                   
			RepeatRequest.getInstance().cdCount++;
			if(!(obj.privateCdTime==0 && obj.publicCdTime==0))
			{
            	sendNotification(EventList.RECEIVE_COOLDOWN_MSG,obj);
            }
            
            var date:Date = new Date();
            
            if(GameCommonData.Player != null && GameCommonData.Player.Role.Id == obj.idUser)
            {
            	if ( obj.magicid == 2501 )			//回帮
            	{
	            	NewUnityCommonData.nextComeUnityTime = obj.privateCdTime; 
	            	NewUnityCommonData.lastComeUnityDate = new Date();
	            	return;
            	} 
            }
            
            if(GameCommonData.Player != null && GameCommonData.Player.Role.Id == obj.idUser)
            {
            	 var skilllevel:GameSkillLevel = GameCommonData.Player.Role.SkillList[obj.magicid] as GameSkillLevel;
            	 //设置自动释放技能的冷却时间 (无论是否设置自动释放都要设置冷却时间)
            	if(skilllevel != null)
            	{
            		//小于10秒的要加延迟 治疗技能CD一好就用
            		if(obj.privateCdTime < 5000 && !GameSkillMode.IsDoctorSkill(skilllevel.gameSkill.SkillMode) )
            		{
            			skilllevel.AutomatismUseTime = date.time + 5000;           	
            		}
            		else
            		{
	            		//计算下次可以使用技能的时间
	            		skilllevel.AutomatismUseTime = date.time + obj.privateCdTime + 1000;     
	            	}      		
            	}
            }  
            
            
            if(GameCommonData.Player != null && GameCommonData.Player.Role.UsingPetAnimal != null && 
               GameCommonData.Player.Role.UsingPetAnimal.Role.Id == obj.idUser
               && GameCommonData.Player.Role.UsingPet != null
               && GameCommonData.Player.Role.UsingPet.SkillLevel != null)
            {
			    	//查找是否宠物拥有自动释放的技能
			      	for(var n:int=0;n<GameCommonData.Player.Role.UsingPet.SkillLevel.length;n++)
					{
						if(!GameCommonData.Player.Role.UsingPet.SkillLevel[n]) continue;
						var skill:GameSkillLevel = GameCommonData.Player.Role.UsingPet.SkillLevel[n];
						if(skill.gameSkill.SkillID == obj.magicid)
						{
							skill.AutomatismUseTime =  date.time + obj.privateCdTime + 1000;     
						}
					}
            }

		}

	}
}