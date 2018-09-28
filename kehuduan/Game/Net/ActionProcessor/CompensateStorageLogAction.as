package Net.ActionProcessor
{
	import GameUI.Modules.CompensateStorage.data.CompensateStorageConst;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class CompensateStorageLogAction extends GameAction
	{
		public static const GET_LOG:uint	 =	1;		//获取补偿日志
		
		public function CompensateStorageLogAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			var action:int = bytes.readShort();		// 动做编号
			
			/** 补偿日志 */
			var str:String = "";
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int    = 0;	
			for(var i:int = 0;i < nDataSeeNum; i++) 
			{
				nDataSee = bytes.readUnsignedByte();	//发言内容长度  
				if(nDataSee != 0) 
				{
					var s:String = bytes.readMultiByte(nDataSee , GameCommonData.CODE);
					str += CompensateStorageData.stringTransform(s);
					trace("s = ", s);
				}
			}
			switch(action)
			{
				case GET_LOG:
					CompensateStorageData.compensateDetails += str;
					break;
			}
		}
		
	}
}