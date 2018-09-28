package GameUI.Modules.Manufactory.Command
{
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.View.ManufatoryView;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ClickManuSkirtCellCommand extends SimpleCommand
	{
		public static const NAME:String = "ClickManuSkirtCellCommand";
		
		public function ClickManuSkirtCellCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var obj:Object = notification.getBody();
			if ( obj.limit == undefined )
			{
				resetSomething();
			}
			if ( (obj.limit == "kind") )
			{
				if ( obj.content != GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ] )//"物品种类"
				{
					ManufactoryData.limitType = obj.content;		
				}
				else
				{
					obj.view.setItemTypeTitle( GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ] );//"物品种类"
					ManufactoryData.limitType = null;
				}
			}
			if ( (obj.limit == "level") )
			{
				if ( obj.content != GameCommonData.wordDic[ "mod_man_com_cli_exe_2" ] )//"物品等级"
				{
					ManufactoryData.limitLevel = obj.content;	
				}
				else
				{
					obj.view.setItemLevelTitle( GameCommonData.wordDic[ "mod_man_com_cli_exe_2" ] );//"物品等级"
					ManufactoryData.limitLevel = null;
				}
			}
			selectItem(obj);
		}
		
		private function selectItem(obj:Object):void
		{
			var limitArr:Array = [];
			var view:ManufatoryView = obj.view as ManufatoryView;
			if ( (ManufactoryData.limitType == null) && (ManufactoryData.limitLevel == null) )
			{
				view.setItemTypeTitle( GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ] );//"物品种类"
				view.setItemLevelTitle( GameCommonData.wordDic[ "mod_man_com_cli_exe_2" ] );//"物品等级"
				limitArr = obj.dataObj;
			}
			else if ( (ManufactoryData.limitType != null) && (ManufactoryData.limitLevel == null) )
			{
				for ( var i:uint=0; i<obj.dataObj.length; i++ )
				{
					var singleObj1:Object = obj.dataObj[i] as Object;
					if ( singleObj1.kind == ManufactoryData.limitType )
					{
						limitArr.push( singleObj1 );
					}
				}
				view.setItemTypeTitle( ManufactoryData.limitType );
			}
			else if ( (ManufactoryData.limitType == null) && (ManufactoryData.limitLevel != null) )
			{
				var startL:uint = uint( ManufactoryData.limitLevel.split("-")[0] );
				var endL:uint = uint( ManufactoryData.limitLevel.split("-")[1] );
				for ( var j:uint=0; j<obj.dataObj.length; j++ )
				{
					var singleObj2:Object = obj.dataObj[j] as Object;
					if ( (singleObj2.level<=endL) && (singleObj2.level>=startL) )
					{
						limitArr.push( singleObj2 );
					}
				}
				view.setItemLevelTitle( ManufactoryData.limitLevel );
			}
			else
			{
				var startLevel:uint = uint( ManufactoryData.limitLevel.split("-")[0] );
				var endLevel:uint = uint( ManufactoryData.limitLevel.split("-")[1] );
				for ( var k:uint=0; k<obj.dataObj.length; k++ )
				{
					var singleObj3:Object = obj.dataObj[k] as Object;
					if ( (singleObj3.kind == ManufactoryData.limitType) && (singleObj3.level<=endLevel) && (singleObj3.level>=startLevel) )
					{
						limitArr.push( singleObj3 );
					}
				}
				view.setItemTypeTitle( ManufactoryData.limitType );
				view.setItemLevelTitle( ManufactoryData.limitLevel );
			}
			view.showEquipItems( limitArr );
		}
		
		//重置
		private function resetSomething():void
		{
			ManufactoryData.limitType = null;
			ManufactoryData.limitLevel = null;
		}
		
	}
}