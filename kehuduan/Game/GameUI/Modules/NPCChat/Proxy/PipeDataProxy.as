package GameUI.Modules.NPCChat.Proxy
{
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	/**
	 * 当数据满的时候自动向显示层推数据 
	 * @author felix
	 * 
	 */	
	public class PipeDataProxy extends Proxy
	{
		public var desText:String="";
		public var linkArr:Array;
		
		public static const NAME:String="PipeDataProxy";
		
		public function PipeDataProxy(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
			this.linkArr=[];
		}
		
		/**
		 *重置（复位） 
		 * 
		 */		
		public function reset():void{
			this.desText="";
			this.linkArr=[];
		}	
	}
}