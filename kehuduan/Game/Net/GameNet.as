package Net
{
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.GameConnect;
	
	import OopsFramework.Debug.Logger;
	import OopsFramework.Net.BaseSocket;
	import OopsFramework.Net.BaseSocketEvent;
	
	import flash.external.ExternalInterface;
	import flash.utils.ByteArray;
	
	/** 游戏服务器Socket */	
	public class GameNet implements IGameNet
	{
		private var socket:BaseSocket;
		private var gam:GameActionManager;
		private var intervalId:uint;
		
		public function GameNet(ip:String = "", port:uint = 0)
		{
			this.gam    = new GameActionManager();
			this.socket = new BaseSocket(ip,port);
			this.socket.addEventListener(BaseSocketEvent.CONNECT,onConnect);
			this.socket.addEventListener(BaseSocketEvent.RECEIVED,onReceived);
			this.socket.addEventListener(BaseSocketEvent.DECODE_ERROR, deCodeError);
			this.socket.addEventListener(BaseSocketEvent.CLOSE,onClose); 
			this.socket.addEventListener(BaseSocketEvent.SECURITY_ERROR,deCodeError);
			this.socket.Connect();
		}
		
		/** 发送数据并序列化给Socket */
		public function Send(datas:ByteArray):void
		{
			if(socket)
			{
//				trace("发送数据并序列化给Socket");
				this.socket.Send(datas);
			} 
			else
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).showBreak();
			}
		}
		
		private function onConnect(e:BaseSocketEvent):void
		{
			var obj:Object = new Object();
			var parm:Array = new Array();
			obj.type       = Protocol.GAMESERVER_INFO;
			parm.push(GameCommonData.GServerInfo.idAccount);
			parm.push(GameCommonData.GServerInfo.dwData);
			parm.push("1");
			obj.data = parm;
			GameConnect.GameServerConnect(obj);
		}
		
		private function onReceived(e:BaseSocketEvent):void
		{
			//清除重连延时器
//			if(e.Data.protocol == 1006)
//			{
//				clearInterval(GameCommonData.reConnectId);
//			}
			 
			if(e.Data.data!=null && e.Data.protocol!="")
			{
//				trace("GameNet:e.Data.protocol:"+e.Data.protocol);
				var protocol:uint   = e.Data.protocol;
				var data:ByteArray  = e.Data.data;
				if(this.gam.ActionList[protocol]!=null)
				{	
					this.gam.ActionList[protocol].Processor(data);
				}
			}
		}
		
		private function deCodeError(e:BaseSocketEvent):void
		{
			Logger.Info(this,"deCodeError","安全错");
		}
		
		private function onClose(e:BaseSocketEvent):void
		{
//			trace("GameNet socket结束")
			GameCommonData.SameSecnePlayerList = null;
			this.socket    				       = null;
			if(!ChgLineData.isChgLine)
			{
				if (UIFacade.UIFacadeInstance)
				{
					UIFacade.GetInstance(UIFacade.FACADEKEY).showBreak();
				}
				else
				{
					callJs("showbreak");
				}
//				callJs("ajUser","-2");
			}
			//统计掉线人数
		}
		
		public function endGameNet():void
		{
			this.socket.removeEventListener(BaseSocketEvent.CONNECT,onConnect);
			this.socket.removeEventListener(BaseSocketEvent.RECEIVED,onReceived);
			this.socket.removeEventListener(BaseSocketEvent.DECODE_ERROR, deCodeError);
			this.socket.removeEventListener(BaseSocketEvent.CLOSE,onClose); 
			this.socket.removeEventListener(BaseSocketEvent.SECURITY_ERROR,deCodeError);
			this.socket.Close();
			GameCommonData.SameSecnePlayerList = null;
			this.socket    				       = null;
		}
		
		public function get Gam():GameActionManager
		{
			return gam;
		}
		
		private function callJs(name:String,para:String=""):void
		{
			try
			{
				ExternalInterface.call(name,-2);
			}
			catch (e:Error)
			{
				
			}
		}
	}
}