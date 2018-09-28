package GameUI.Modules.Unity.Command
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	
	import Net.ActionSend.UnityActionSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SendActionCommand extends SimpleCommand
	{
		public static const SENDACTION:String = "SendActionCommand";		/** 发送请求 */
		public function SendActionCommand()
		{
			super();
		}
		public override function execute(notification:INotification):void
		{
			
			switch((notification.getBody() as Object).type)
			{
				
				case 208:		//主堂数据
					//如果帮派详细数据有缓存，不打扰服务器
					if(UnityConstData.mainUnityDataObj == null)
					{
						UnityConstData.unityObj.type = 1107;														//协议号
						UnityConstData.unityObj.data = [0 , 0 , 208, 0 , 0];
						UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求后三分钟后才能再次请求
					}
				break;
				case 221:		//请求分堂数据
					UnityConstData.unityObj.type = 1107;														//协议号
					UnityConstData.unityObj.data = [0 , 0 ,221, 0 , 0];
					UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求后三分钟后才能再次请求
				break;
				case 222:		//发雇佣数据
					var selectOther:int = (notification.getBody() as Object).data;
					var dataArr:Array = (notification.getBody() as Object).list;			//雇佣人数的数组
					var craftsmanNum:int = selectOther * 10000 + int(dataArr[0]);	//分堂号*10000 + 建筑工人
					var busMasNum:int = int(dataArr[2] * 10000) + int(dataArr[1]) ;
					UnityConstData.unityObj.type = 1107;													//协议号
					UnityConstData.unityObj.data = [0 , 0 ,222, craftsmanNum , busMasNum];
					UnityActionSend.SendSynAction(UnityConstData.unityObj);										
				break;
				case 224:		//升级建造
					var d:int = (notification.getBody() as Object).data;
					UnityConstData.unityObj.type = 1107;														//协议号
					UnityConstData.unityObj.data = [0 , 0 , 224, (notification.getBody() as Object).data , 0];
					UnityActionSend.SendSynAction(UnityConstData.unityObj);										
				break;
				case 225:		//分堂技能研究
					UnityConstData.unityObj.type = 1107;														//协议号
					UnityConstData.unityObj.data = [0 , 0 , 225, (notification.getBody() as Object).data , 0];
					UnityActionSend.SendSynAction(UnityConstData.unityObj);										
				break;
				case 228:		//分堂打工
					UnityConstData.unityObj.type = 1107;														//协议号
					UnityConstData.unityObj.data = [0 , 0 , 228, (notification.getBody() as Object).data , 0];
					UnityActionSend.SendSynAction(UnityConstData.unityObj);										
				break;
				case 230:		//帮派捐献
					// 0是金钱， 1是物品 ， 后面接着金钱数或物品ID
					UnityConstData.unityObj.type = 1107;														//协议号
					UnityConstData.unityObj.data = [0 , 0 , 230, (notification.getBody() as Object).data[0] , (notification.getBody() as Object).data[1]];
					UnityActionSend.SendSynAction(UnityConstData.unityObj);		
				break;
			}
			
		}
		
	}
}