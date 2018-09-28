package Net.ActionSend
{
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionProcessor.TopList;
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	/** 
	 * 游戏玩法  
	 * 1、主角移动信息
	 * */
	public class Zippo
	{
		/** 捡取物品 idType为 999999时候是全部捡取 */
		public static function SendPickItem(idMapItem:uint,idType:uint):void
		{
			var parm:Array = new Array();
			parm.push(4);//范围捡取
			parm.push(idMapItem);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(1);
			parm.push(idType);
			SendMapItem(parm);		
		}
		
		/** 地图物品  */
		public static function SendMapItem(parm:Array):void
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(56);
			sendBytes.writeShort(Protocol.PLAYER_MAPITEM);
//		OBJID			nAction;
//		OBJID			id;
			sendBytes.writeUnsignedInt(parm.shift());
			sendBytes.writeUnsignedInt(parm.shift());
//		USHORT			nPosX;
//		USHORT			nPosY;
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(parm.shift());
//		OBJID			ReTime;
//		DWORD			nItemAmount;
			sendBytes.writeUnsignedInt(parm.shift());
			sendBytes.writeUnsignedInt(parm.shift());

//	OBJID			idType[8];
			sendBytes.writeUnsignedInt(parm.shift());
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);			
			
			GameCommonData.GameNets.Send(sendBytes);
		}

		/** 控制玩家死亡（只可以控制自己死亡） */
		public static function PlayerDie():void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(PlayerAction.DIE);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**  停止攻击 */
		public static function PlayerAttackStop():void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(288);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		
		/** 控制玩家复活（只可以控制自己复活） 0-不用春哥，1-用春哥 */
		public static function PlayerRelive(type:uint):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(type);			//0-不用春哥，1-使用春哥      新手发用春哥的
			obj.data.push(PlayerAction.RELIVE);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/** 
		 * 发人物移动信息
		 * @param tileX    A*格X
         * @param tileY    A*格Y
         * @param dir1     下一步方向
         * @param dir2     下二步方向
		 **/
		public static function PlayerWalk(roleId:int, tileX:int, tileY:int, dir1:int, dir2:int):void
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian 		= Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(20);
			sendBytes.writeShort(Protocol.PLAYER_WALK);
					
			sendBytes.writeUnsignedInt(roleId);						// 角色编号
			sendBytes.writeShort(tileX);							// A*格X
			sendBytes.writeShort(tileY);							// A*格Y
			
			sendBytes.writeByte(dir1);								// 下一步方向
			sendBytes.writeByte(dir2);								// 下二步方向
			sendBytes.writeByte(0); 								// 对齐
			sendBytes.writeByte(0);
								
			sendBytes.writeShort(1234); 
			sendBytes.writeShort(1234);
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		/**
		*参数1：发出者ID
		*参数2：攻击目标ID
		*参数3：攻击目标的坐标X
		*参数4：攻击目标的坐标Y
		*参数5：攻击的方法
		*/		
		public static function SendAttack(idUser:uint, idTarget:uint, xPos:uint, yPos:uint, method:uint):void
		{
			var parm:Array = new Array();
			parm.push(idUser);
			parm.push(idTarget);
			parm.push(method);				
			parm.push(0);
			parm.push(xPos);
			parm.push(yPos);				
			parm.push(0);
			SendInteract(parm);	
		}
		
		public static function SendBuffSkill(idUser:uint, idTarget:uint, xPos:uint, yPos:uint, method:uint,skillID:int,level:int,dwData:int=0,dwBattleAddInfo:int =0):void
		{
			var  sendBytes:ByteArray=new ByteArray;
			sendBytes.endian=Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(32);
			sendBytes.writeShort(Protocol.MSG_INTERACT);

			sendBytes.writeUnsignedInt(idUser);//使用者
			sendBytes.writeUnsignedInt(idTarget);//攻击目标
			sendBytes.writeShort(method);//攻击方式21
			sendBytes.writeShort(0);//格式对齐
			sendBytes.writeUnsignedInt(dwData);//附加信息

			sendBytes.writeShort(xPos);//X坐标
			sendBytes.writeShort(yPos);//Y坐标

			sendBytes.writeShort(skillID);//技能ID
			sendBytes.writeShort(level);//技能等级
			sendBytes.writeUnsignedInt(dwBattleAddInfo);//附加信息
			
			GameCommonData.GameNets.Send(sendBytes);
		}

		
		
		public static function SendSkill(idUser:uint, idTarget:uint, xPos:uint, yPos:uint, method:uint,skillID:int,level:int,dwData:int=0,dwBattleAddInfo:int =0):void
		{
			var  sendBytes:ByteArray=new ByteArray;
			sendBytes.endian=Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(32);
			sendBytes.writeShort(Protocol.MSG_INTERACT);

			sendBytes.writeUnsignedInt(idUser);//使用者
			sendBytes.writeUnsignedInt(idTarget);//攻击目标
			sendBytes.writeShort(method);//攻击方式21
			sendBytes.writeShort(0);//格式对齐
			sendBytes.writeUnsignedInt(dwData);//附加信息

			sendBytes.writeShort(xPos);//X坐标
			sendBytes.writeShort(yPos);//Y坐标

			sendBytes.writeShort(skillID);//技能ID
			sendBytes.writeShort(level);//技能等级
			sendBytes.writeUnsignedInt(dwBattleAddInfo);//附加信息
			
			GameCommonData.GameNets.Send(sendBytes);
		}

		/**
		*参数1：发出者ID
		*参数2：攻击目标ID
		*参数3：攻击目标的坐标X
		*参数4：攻击目标的坐标Y
		*参数5：魔法的类型
		*/	
		public static function SendMagicAttack(idUser:uint, idTarget:uint, xPos:uint, yPos:uint, magicType:uint):void
		{
			var parm:Array = new Array();
			parm.push(0);
			parm.push(idUser);
			parm.push(idTarget);
			parm.push(0);
			parm.push(xPos);
			parm.push(yPos);
			parm.push(21);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(magicType);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);	
			SendInteract(parm);
		}
		
		/** 战斗信息压包  */
		private static function SendInteract(parm:Array):void 
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(32);
			sendBytes.writeShort(Protocol.MSG_INTERACT);
//			OBJID	idSender;
//			OBJID	idTarget;
			sendBytes.writeUnsignedInt(parm.shift());
			sendBytes.writeUnsignedInt(parm.shift());
//			USHORT	unType;
//			DWORD	dwData;
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(parm.shift());
//			USHORT	usPosX;
//			USHORT	usPosY;
//			USHORT	usMagicType;
//			USHORT	usMagicLev;
//			DWORD dwBattleAddInfo;
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(parm.shift());
			sendBytes.writeUnsignedInt(parm.shift());
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		/**发送NPC对话消息（业务层） */
		public static function SendAnswerNPC(nData:uint):void
		{
			var parm:Array = new Array();
			parm.push(201);
			parm.push(nData);
			SendNPCDialog(parm);
		}
		
		
		/** 发送NPC对话消息 (数据层) */
		private static function SendNPCDialog(parm:Array):void 
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(12);
			sendBytes.writeShort(Protocol.MSG_NPCDIALOG);
			sendBytes.writeUnsignedInt(parm.shift());
			sendBytes.writeUnsignedInt(parm.shift());
			GameCommonData.GameNets.Send(sendBytes);
		}
		/** 排行榜请求 */
		 public static function QueryTopList(nAction:uint, nPage:uint):void 
		{
			var parm:Array = new Array();
			parm.push(nAction + TopList.MSGTOPLIST_QUERYTOTAL);
			parm.push(nPage);
			SendQueryTopList(parm);		
		}
		/** 发送NPC对话消息 (数据层) */
		private static function SendQueryTopList(parm:Array):void 
		{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(12);
			sendBytes.writeShort(Protocol.MSG_TOPLIST);
			
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(parm.shift());
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
			GameCommonData.GameNets.Send(sendBytes);
		}
		/** 发送新手卡卡号 (数据层) */
		public static function SendPickNewCard( szCard:String,bussiness:int ):void
		{
			var m_GetLen:ByteArray;
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(szCard,GameCommonData.CODE); 
			
			if(m_GetLen.length >= 64)
				return;
				
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(84);
			sendBytes.writeShort(1067);
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
//			sendBytes.writeUnsignedInt(1);
			sendBytes.writeUnsignedInt( bussiness );
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(szCard,GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=84) {
				sendBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		/*公会卡*/
		public static function SendPickSocietyCard(szCard:String):void
		{
			var m_GetLen:ByteArray;
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(szCard,GameCommonData.CODE);
			
			if(m_GetLen.length >= 64)
				return;
				
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(84);
			sendBytes.writeShort(1067);
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(5);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(szCard,GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=84) {
				sendBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		//投票卡
		/** 发送新手卡卡号 (数据层) */
		public static function SendDevoteCard(szCard:String):void
		{
			var m_GetLen:ByteArray;
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(szCard,GameCommonData.CODE);
			
			if(m_GetLen.length >= 64)
				return;
				
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(84);
			sendBytes.writeShort(1067);
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(3);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(szCard,GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=84) {
				sendBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}

	
		//4399投票卡
		/** 发送媒体卡卡号 (数据层) */
		public static function SendMediaCard(szCard:String):void
		{
			var m_GetLen:ByteArray;
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(szCard,GameCommonData.CODE);
			
			if(m_GetLen.length >= 64)
				return;
				
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(84);
			sendBytes.writeShort(1067);
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(5);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(szCard,GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=84) {
				sendBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		/** 360抽奖 */
		public static function Send360Lottery(szCard:String):void
		{
			var m_GetLen:ByteArray;
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(szCard,GameCommonData.CODE);
			
			if(m_GetLen.length >= 64)
				return;
				
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(84);
			sendBytes.writeShort(1067);
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(9);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(szCard,GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=84) {
				sendBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}
		
		/** 新手卡 */
		public static function SendCard(szCard:String,type:int = 0):void
		{
			var m_GetLen:ByteArray;
			m_GetLen= new ByteArray( );
			m_GetLen.writeMultiByte(szCard,GameCommonData.CODE);
			
			if(m_GetLen.length >= 64)
				return;
				
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			sendBytes.writeShort(84);
			sendBytes.writeShort(1067);
			sendBytes.writeShort(0);
			sendBytes.writeShort(0);
			sendBytes.writeUnsignedInt(type);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			var tempBytes:ByteArray = new ByteArray();
			tempBytes.writeMultiByte(szCard,GameCommonData.CODE);
			sendBytes.writeBytes(tempBytes,0,tempBytes.length);

			while (sendBytes.position!=84) {
				sendBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(sendBytes);
		}
	
	}
}