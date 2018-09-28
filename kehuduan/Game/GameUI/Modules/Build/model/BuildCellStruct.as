package GameUI.Modules.Build.model
{
	import GameUI.Mediator.UiNetAction;
	
	public class BuildCellStruct
	{
		/** 修炼名称*/
		public var name:String;
		/** 修炼等级*/
		public var level:uint;
		/** 修炼需求描述 */
		public var needDes:String;
		/** 修炼描述 */
		public var des:String;
		/** 图片类型 */
		public var imgType:String
		/** 修炼需求时间 */
		public var needTime:String;
		/** 修炼ToolTip */
		public var toolTipDes:Object;
		/** 是否正在修炼中 */
		public var isBuilding:Boolean;
		
		
		public function BuildCellStruct()
		{
					
		}
		
		
		/**
		 * 输出调试信息 
		 * @return 
		 * 
		 */		
		public function toString():String{
			return "debug";
		}

	}
}