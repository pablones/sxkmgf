package GameUI.Modules.AutoResize.Data
{
	import flash.display.DisplayObject;
	
	public class AutoSizeData
	{
		public static const MSG_MED_CREATE_OVER:String = "MSG_MED_CREATE_OVER";
		
		//state为1表示只变动x       2表示只变动y        3表示居中处理 或 x和y只移动一半的情况        4表示x,y都变动（非居中）
		public static function setStartPoint( disObj:DisplayObject, X:Number, Y:Number, state:uint ):void
		{
			if( GameCommonData.fullScreen != 2 )
			{
				disObj.x = X;
				disObj.y = Y;
			}
			else
			{
				switch(state)
				{
					case 1:   //只变动x
						disObj.x = X + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
						break;
					case 2:   //只变动y
						disObj.y = Y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
						break;
					case 3:    //居中处理 或 x和y只移动一半的情况
						disObj.x = X + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						disObj.y = Y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					 	break;
					 case 4:   //x,y都变动（非居中）
						disObj.x = X + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
						disObj.y = Y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
					 	break;
				}
			}
		}
	}
	
}		