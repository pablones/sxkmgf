package Controller
{
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	public class PKController
	{		
	    /** PK值字体色设置  */
        public static function GetFontColor(pk:int):String
        {
        	var color:String = "#ffffff";     	
			if(pk >= 7)
			{
				color = "#fe0202";
			}
			else if(pk >= 4)
			{
				color = "#ff5555";
			}
			else if(pk >= 1)
			{
				color = "#ffacac";
			}
			else
			{
			    color = "#ffffff";
			}
			return color;
        }
        
         /** PK值字体边框色设置  */
        public static function GetBorderColor(pk:int):uint
        {
        	var color:uint = 0x000000;     	
			if(pk >= 7)
			{
				color = 0x000000;
			}
			else if(pk >= 4)
			{
				color = 0x310000;
			}
			else if(pk >= 1)
			{
				color = 0x590000;
			}
			else
			{
			    color =0x000000;
			}
			return color;
        }
        
        public static  function GetPKTeamBorderColor(pk:int):uint
        {
        	var color:uint = 0x000000;    
            switch(pk)	
            {
            	case 1:color = 0x000000;
            		   break;
                case 2:color = 0x000000;
            		   break;
            	case 3:color = 0x000000;
            		   break;   
            }
			return color;
        }				
        
//        贪狼 #FF32CC
//	    破军 #00CBFF
//        七杀 #E08E1F
        
        /** PK值字体色设置  */
        public static function GetPKTeamFontColor(pk:int):String
        {
        	var color:String = "#ffffff";     	
        	switch(pk)	
            {
            	case 1:color = "#FF32CC";
            		   break;
                case 2:color = "#00CBFF";
            		   break;
            	case 3:color = "#E08E1F";
            		   break;   
            }
			return color;
        }   
         /** PK场小地图圆点颜色  */
        public static function getPKPersonColor(pk:int):uint
        {
        	var color:uint;
        	switch(pk)
        	{
        		case 1:color = 0xFF32CC;
            		   break;
                case 2:color = 0x00CBFF;
            		   break;
            	case 3:color = 0x00CBFF;
            		   break; 
            	default: color = 0xffffff;
        	}
        	return color;
        }
	}
}