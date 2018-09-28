package Net.ActionProcessor
{
	import Controller.PlayerController;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	
	import flash.utils.ByteArray;

	public class SoulPictureAction extends GameAction
	{
		public function SoulPictureAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			
			var obj:Object  = new Object();
			obj.idItem 	  = bytes.readUnsignedInt();				//idItem 玩家物品ID
			obj.idUser    = bytes.readUnsignedInt();				//idUser	玩家ID
			
			obj.usAction	  = bytes.readUnsignedInt();        //usAction 1:出现   2：消失
			obj.usType		  = bytes.readUnsignedShort();		//力量 + 属相
			obj.usLevel		  = bytes.readUnsignedShort();		//等级
			obj.usGrow		  = bytes.readUnsignedShort();		//成长率
			obj.usMixLev	  = bytes.readUnsignedShort();		//合成等级

			var player:GameElementPlayer = PlayerController.GetPlayer(obj.idUser) as GameElementPlayer;
			if(player != null)
			{
				if(obj.usAction == 2)
				{
					player.Role.TernalType = obj.usType / 10;
					player.Role.TernalMutually = obj.usType % 10;
					player.Role.TernalGrow = obj.usGrow;
					player.Role.TernalMixLev = obj.usMixLev;
					player.Role.isTernal = true;
//					gameRole.WeaponSkinName = "Resources/Player/Ternal/2.swf"
//					
//					if(gameRole.TernalMixLev >= 5)				
//					{
//						gameRole.WeaponEffectName = "Resources/Player/Ternal/" + gameRole.TernalMutually +".swf";
//					}
//					
					player.AddTernal();
					
					if(player.Visible == false && player.Role.gameElementTernal != null)
					{
						player.Role.gameElementTernal.Visible = false;	
					}
				}
				else if(obj.usAction == 1)
				{
					player.Role.isTernal = false;
					player.RemoveTernal();	
				}
			}
		}
		
	}
}