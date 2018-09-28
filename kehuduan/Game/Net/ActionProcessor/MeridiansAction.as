package Net.ActionProcessor
{
	import GameUI.Modules.Meridians.Components.MeridiansTimeOutComponent;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class MeridiansAction extends GameAction
	{
		public function MeridiansAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
	    public override function Processor(bytes:ByteArray):void 
		{
			
			bytes.position = 4;			
			var roleID:int =bytes.readUnsignedInt(); // 角色ID
			//自己的经脉信息
			if(MeridiansData.meridiansVO.roleID == GameCommonData.Player.Role.Id)
			{
				MeridiansData.meridiansVO.roleID = roleID;
				MeridiansData.meridiansVO.nAmount = bytes.readUnsignedInt(); // 修炼的经脉数量
				bytes.readUnsignedInt(); //所有经脉修炼到达的等级（到达一定等级后会有4个效果参数，配置去写)
				MeridiansData.meridiansVO.nAllStrengthLevAdd = bytes.readUnsignedInt();//所有经脉强化到达的等级(到达一定等级会有1个加成效果参数，配置去写）
				
				
				for (var i:int = 0 ;i < MeridiansData.meridiansVO.nAmount;i ++) 
				{
					var type:int = bytes.readUnsignedInt(); // 经脉类型
					MeridiansData.meridiansVO.meridiansArray[type - 1].nLev = bytes.readUnsignedInt();  // 经脉等级
	//				MeridiansData.meridiansVO.meridiansArray[type - 1].nValue = 
					bytes.readUnsignedInt(); //经脉属性
					MeridiansData.meridiansVO.meridiansArray[type - 1].nLeaveTime = bytes.readUnsignedInt();//经脉当前等级剩余修炼时间
					MeridiansData.meridiansVO.meridiansArray[type - 1].nStrengthLev = bytes.readUnsignedInt();//经脉强化等级
	//				MeridiansData.meridiansVO.meridiansArray[type - 1].nAddRate = 
					bytes.readUnsignedInt();   // 经脉强化加成
					MeridiansData.meridiansVO.meridiansArray[type - 1].nState = bytes.readUnsignedInt();   // 经脉状态
					MeridiansData.meridiansVO.meridiansArray[type - 1].nOrderTimer = bytes.readUnsignedInt();   //加入队列时间
	
				}
				MeridiansData.upDataNAllLevGrade();
				MeridiansData.upDataNAllStrengthLevAdd();
				trace("roleId:",MeridiansData.meridiansVO.nAllLevGrade);
			

//				MeridiansData.meridiansVO = meridiansVo;
				sendNotification(MeridiansEvent.UPDATA_MERIDIANS_DATA);
				MeridiansTimeOutComponent.getInstance().addFun1("upDataTime",MeridiansData.upDataTime);
			}
			else
			{
				
			}
		}
		
	}
}