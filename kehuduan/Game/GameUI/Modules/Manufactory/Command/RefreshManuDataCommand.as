package GameUI.Modules.Manufactory.Command
{
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.Proxy.ManufatoryProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class RefreshManuDataCommand extends SimpleCommand
	{
		public static const NAME:String = "RefreshManuDataCommand";
		private var manufactoryProxy:ManufatoryProxy;
		
		public function RefreshManuDataCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			manufactoryProxy = facade.retrieveProxy( ManufatoryProxy.NAME ) as ManufatoryProxy;
			manufactoryProxy.aStockInfo = [];
			manufactoryProxy.aLeatherInfo = [];
			manufactoryProxy.aRefinementInfo = [];
			
			for ( var i:uint=0; i<ManufactoryData.scenographyList.length; i++ )
			{
				var id:uint = uint( ManufactoryData.scenographyList[i] );
				if ( !manufactoryProxy.allInfoDic[id] ) continue;
				
				if ( manufactoryProxy.allInfoDic[id].classTpye == "stock" )
				{
					manufactoryProxy.aStockInfo.push( manufactoryProxy.allInfoDic[id] );
				}
				else if ( manufactoryProxy.allInfoDic[id].classTpye == "leather" )
				{
					manufactoryProxy.aLeatherInfo.push( manufactoryProxy.allInfoDic[id] );
				}
				else if ( manufactoryProxy.allInfoDic[id].classTpye == "refinement" )
				{
					manufactoryProxy.aRefinementInfo.push( manufactoryProxy.allInfoDic[id] );
				}
			}
			
			manufactoryProxy.aStockInfo.sortOn( "level",Array.NUMERIC );
			manufactoryProxy.aLeatherInfo.sortOn( "level",Array.NUMERIC );
			manufactoryProxy.aRefinementInfo.sortOn( "level",Array.NUMERIC );
		}
		
	}
}