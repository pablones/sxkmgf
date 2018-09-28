package GameUI.Modules.Pet.Proxy
{
	import Net.ActionSend.OperatorItemSend;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	public class PetNetAction
	{
		public function PetNetAction()
		{
		}
		
		/** 操作宠物 */
		public static function operatePet(action:int, petId:uint=0, ownerId:uint=0, ownerName:String=""):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			
			obj.data.push(action);
			obj.data.push(1);
			obj.data.push(0);
			obj.data.push(ownerId);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");
			
			obj.data.push(petId);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		/** 操作宠物 */
		public static function opPet(action:int, petId:uint=0, petName:String=""):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(petId);							//自己添加250，自己锁定251, 取消繁殖253
			obj.data.push(action);
			obj.data.push(0);
			obj.data.push(petName);							//名字  改名时用，
			PlayerActionSend.PlayerAction(obj);
		}
		
		/** 宠物加点 */
		public static function addPointPet(action:int, petId:uint, points:Array, petName:String=""):void
		{
			for(var i:int = 0; i < points.length; i++) {
				if(points[i] != 0) {
					var obj:Object = new Object();
					obj.type = Protocol.PLAYER_ACTION;
					obj.data = new Array();
					obj.data.push(0);
					obj.data.push(GameCommonData.Player.Role.Id);
					obj.data.push(0);
					obj.data.push(0);
					obj.data.push(i+1);		//1-6   1-5对应 力量==， 6表示确定加点
					obj.data.push(petId);
					obj.data.push(action);
					obj.data.push(points[i]);
					obj.data.push(petName);
					PlayerActionSend.PlayerAction(obj);
				}
			}
			var objSure:Object = new Object();
			objSure.type = Protocol.PLAYER_ACTION;
			objSure.data = new Array();
			objSure.data.push(0);
			objSure.data.push(GameCommonData.Player.Role.Id);
			objSure.data.push(0);
			objSure.data.push(0);
			objSure.data.push(6);		//1-6   1-5对应 力量==， 6表示确定加点
			objSure.data.push(petId);
			objSure.data.push(action);
			objSure.data.push(0);
			objSure.data.push(petName);	
			PlayerActionSend.PlayerAction(objSure);
		}
		
		/** 确定宠物繁殖 (确定还童) 灵性提升*/
		public static function petBreed(action:int, petId_0:uint, petId_1:uint=0, userId_1:uint=0, cardId:String=""):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(userId_1);						//userId_1  对方ID
			obj.data.push(GameCommonData.Player.Role.Id);	//userId_0  自己ID
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(petId_0);							//petId_0  自己宠物ID  
			obj.data.push(action);
			obj.data.push(petId_1);							//petId_1  对方宠物ID  
			obj.data.push(cardId);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 宠物改版  幻化  316 ，默契 318，灵性 317	玩耍 325
		 * @param action
		 * @param petId_0
		 * @param petId_1
		 * @param userId_1
		 * @param cardId
		 * 
		 */		
		public static function newPetOperate(action:int, petId_0:uint, petId_1:uint=0, userId_1:uint=0, cardId:String=""):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(userId_1);						//userId_1  对方ID
			obj.data.push(GameCommonData.Player.Role.Id);	//userId_0  自己ID
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(petId_0);							//petId_0  自己宠物ID  
			obj.data.push(action);
			obj.data.push(petId_1);							//petId_1  对方宠物ID  
			obj.data.push(cardId);
			PlayerActionSend.PlayerAction(obj);
		}
		
		
	}
}