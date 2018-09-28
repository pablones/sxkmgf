package Net
{
	import Net.ActionSend.AccLogin;
	
	import OopsFramework.Net.BaseSocket;
	import OopsFramework.Net.BaseSocketEvent;
	
	import flash.utils.ByteArray;
	
	/** 登录服务器Socket*/
	public class AccNet
	{
		private var socket:BaseSocket;
		private var gam:GameActionManager;
		public function AccNet(ip:String = "", port:uint = 0)
		{
			if(GameCommonData.Tiao)
			{
				if ( GameCommonData.wordVersion == 1 )
				{
					GameCommonData.Tiao.content_txt.text = "正在验证账号信息.....";          //"正在验证账号信息....." 
				}
				else
				{
					GameCommonData.Tiao.content_txt.text = "正在驗證帳號信息";         //"正在验证账号信息....." 
				}
			}
			this.gam = new GameActionManager();
			this.socket = new BaseSocket(ip,port);
			this.socket.addEventListener(BaseSocketEvent.CONNECT,onConnect);
			this.socket.addEventListener(BaseSocketEvent.RECEIVED,onReceived);
			this.socket.addEventListener(BaseSocketEvent.CLOSE,onClose);
			this.socket.Connect();
		}
		
		/** 发送数据并序列化给Socket */
		public function Send(datas:ByteArray):void 
		{
			this.socket.Send(datas);	
		}
		
		/** 关闭Socket  */
		public function Close():void 
		{
			this.socket.Close();	
		}
		
		private function onConnect(e:BaseSocketEvent):void
		{
			//测试登陆信息，正式版删除
			var obj:Object = new Object();
			var parm:Array = new Array();
			parm.push(GameCommonData.Accmoute);
			parm.push(GameCommonData.Password);
			if ( GameConfigData.GameSocketName == GameConfigData.specialLineName )
			{
				var lineStr:String;
				if(GameCommonData.wordVersion == 1)	//大陆
				{
					lineStr = "六线";
				}
				else if(GameCommonData.wordVersion == 2) //台服
				{
					lineStr = "六線";
				}
				parm.push( lineStr);//晴川阁 测试 封测一服
			}
			else
			{
				parm.push(GameConfigData.GameSocketName);	//晴川阁 测试 封测一服
			}
			parm.push("111111111111");
			obj.type = Protocol.LOGIN_ACCSERVER;
			obj.data = parm;
			AccLogin.AccLoginCreate(obj);
		}
		
		private function onReceived(e:BaseSocketEvent):void
		{
			if(e.Data.data!=null && e.Data.protocol!="")
			{
				var protocol:String = e.Data.protocol;
				var data:ByteArray  = e.Data.data;
				if(this.gam.ActionList[protocol]!=null)
				{
					//测试登陆信息，正式版删除
//					if(DebugInfo.loginText_txt)
//					{
//						DebugInfo.loginText_txt.htmlText += "收到消息data.length："+data.length+"   data.position："+data.position;
//					}
					this.gam.ActionList[protocol].Processor(data);
				}
			}
		}
		
		private function onClose(e:BaseSocketEvent):void
		{
			GameCommonData.SameSecnePlayerList = null;
			removeLis();
			this.socket    				       = null;
		}
		
		private function removeLis():void 
		{
			this.socket.removeEventListener(BaseSocketEvent.CONNECT,onConnect);
			this.socket.removeEventListener(BaseSocketEvent.RECEIVED,onReceived);
			this.socket.removeEventListener(BaseSocketEvent.CLOSE,onClose);
		}
		
		/** 随机进入一条线 */
		private function randomLine(lineIndex:int = -1):String
		{
			if(lineIndex == -1) {
				var i:int = Math.random() * GameConfigData.LINE_ARR.length;
				return GameConfigData.LINE_ARR[i];
			} else {
				return GameConfigData.LINE_ARR[lineIndex];
			}
		}
	}
}