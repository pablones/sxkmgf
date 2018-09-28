package Net.ActionProcessor
{
	import Controller.PlayerController;
	
	import GameUI.Modules.IdentifyingCode.Data.IdentifyingCodeConst;
	import GameUI.Modules.IdentifyingCode.Mediator.IdentifyingCodeMediator;
	import GameUI.Modules.ReName.Data.ReNameData;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	
	import Net.GameAction;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.utils.ByteArray;

	public class EncyptAction extends GameAction
	{
		public function EncyptAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public static const SYSSET_MODIFY_PWD:uint = 1;		//修改密码
		public static const SYSSET_OPEN_PWD:uint   = 2;		//加锁
		public static const SYSSET_COLSE_PWD:uint  = 3;		//解锁
		public static const SYSSET_RECV_KEY:uint   = 4;		//密钥
		public static const RENAME_KEY:uint        = 5;     //改名
		
		/** 跑商验证码 */
		public static const YANZHENGMA_GET_UPDATA:uint	   = 9;		//接受与发送跑商验证码
		public static const YANZHENGMA_SEND_ACC:uint   = 8;			//发送验证码 与 验证码成功
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			
			var idUser:uint = bytes.readUnsignedInt();	// 用户ID      //帮派ID
			var action:uint = bytes.readShort();		// Action
			var data:uint = bytes.readShort();			// data       //改名时，0为不成功、1为成功
			
			var oldPwd:String = bytes.readMultiByte(24, GameCommonData.CODE);//旧密码        //0为个人，1为帮派
			var newPwd:String = bytes.readMultiByte(24, GameCommonData.CODE);//新密码        //更新的名字 
			
			switch(action) {
				case SYSSET_COLSE_PWD:		//系统设置  关闭账号保护锁   type:操作类型  1-关闭锁， 2-修改密码   res:操作结果  0-失败，1-成功。
					sendNotification(SystemSettingData.SYS_SET_PWD_OP_RETURN, {type:1, res:data});
					break;
				case SYSSET_MODIFY_PWD:		//系统设置  修改密码
					sendNotification(SystemSettingData.SYS_SET_PWD_OP_RETURN, {type:2, res:data});
					break;
				case SYSSET_RECV_KEY:		//接收密钥
					sendNotification(SystemSettingData.SYS_SET_PWD_OP_RETURN, {type:3, res:data, key:newPwd});
					break;
				case RENAME_KEY:          
					if( GameCommonData.Player.Role.Id == idUser || GameCommonData.Player.Role.unityId == idUser )
					{
						sendNotification(ReNameData.REPLY_RENAME, {state:data, index:int( oldPwd ), id:idUser, name:newPwd});
					}else{
						var player:GameElementAnimal = PlayerController.GetPlayer(idUser);
						if( player != null )
						{
							player.Role.Name = newPwd;
							player.UpdatePersonName();
						}
					}
					break;  
				case YANZHENGMA_GET_UPDATA:
					if(!facade.hasMediator(IdentifyingCodeMediator.NAME))
					{
						facade.registerMediator(new IdentifyingCodeMediator());
					}
					GameCommonData.UIFacadeIntance.sendNotification(IdentifyingCodeConst.UPDATE_YANZHENGMA_CODE,{id:idUser,time:data});
					break;
				case YANZHENGMA_SEND_ACC:
					GameCommonData.UIFacadeIntance.sendNotification(IdentifyingCodeConst.CLOSE_YANZHENGMA_VIEW);
					break;
			}
			
		}
		
	}
}