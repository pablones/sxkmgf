package Net
{
	import GameUI.UICore.UIFacade;
	
	import flash.utils.ByteArray;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;
	
	/** 游戏网站动作抽象类 */
	public class GameAction extends Proxy
	{
		public function GameAction(isUsePureMVC:Boolean = true)
		{
			if(isUsePureMVC) 
			{
				this.initializeNotifier(UIFacade.FACADEKEY);
				facade.registerProxy(this);
			}
		}
		
		public virtual function Processor(bytes:ByteArray):void 
		{
			
		}
	}
}