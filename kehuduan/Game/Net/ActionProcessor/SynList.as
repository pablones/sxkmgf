package Net.ActionProcessor
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityVo;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class SynList extends GameAction
	{
		private var pageArr:Array = new Array();				//	响应面板所需的数组
		private var appArr:Array  =new Array();					// 申请面板所需的数组
		public static var isFirst:int = 0;
		public function SynList(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			appArr  = [];
			var listInfo:Object 		= new Object();
			var actionData:uint 			= bytes.readUnsignedShort();
			listInfo.usAction 		= actionData % 100;
//			listInfo.usAction = actionData;
			listInfo.usPage         = actionData / 100;
			var data:uint 			= bytes.readUnsignedShort();
			listInfo.usAmount 		= data % 100;
			listInfo.usMaxAmount 	= data / 100;  
			var vo:UnityVo;
				
			switch(listInfo.usAction)
			{
				case 4:
					for(var i:uint = 0 ; i < listInfo.usAmount ; i ++)
					{
						vo = new UnityVo();
						vo.rank = NewUnityCommonData.allUnityArr.length+1;
						vo.id				= bytes.readUnsignedInt();
						vo.name 		= bytes.readMultiByte(16,GameCommonData.CODE);
						vo.boss 	= bytes.readMultiByte(16,GameCommonData.CODE); 
						vo.level   		 	= bytes.readUnsignedShort();
						vo.currentPeople       	= bytes.readUnsignedShort(); 
						if ( !isMemExist( vo,NewUnityCommonData.allUnityArr ) )
						{
							NewUnityCommonData.allUnityArr.push( vo );
						}
					}
//					if ( NewUnityCommonData.allUnityArr.length == listInfo.usMaxAmount )
//					{
					sendNotification( NewUnityCommonData.RECEIVE_ALL_UNITYS_LIST,NewUnityCommonData.allUnityArr );
//					}
				break;
				default:
					bytes.readUnsignedInt();
					bytes.readMultiByte(16,GameCommonData.CODE);
					bytes.readMultiByte(16,GameCommonData.CODE); 
					bytes.readUnsignedShort();
					bytes.readUnsignedShort(); 
				break;
			}
		}
		
		//是否有相同的帮派
		private function isMemExist( vo:UnityVo,arr:Array ):Boolean
		{
			for ( var i:int=0; i<arr.length; i++ )
			{
				if ( arr[ i ].id == vo.id )
				{
//					trace ( "收到相同的人了" + vo.name );
					arr[ i ] = vo;
					return true;
				}
			}
			return false;
		}
		
	}
}