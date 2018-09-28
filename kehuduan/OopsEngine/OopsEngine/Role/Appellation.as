package OopsEngine.Role
{
	/**称号**/
	public class Appellation
	{
		public var appellationID:int;   // 称号编号
		public var name:String;         // 名字
		public var color:int;           // 字体颜色
		public var borderColor:int;     // 边框颜色
		public var typeID:int;          // 类别  1普通 2副本 3缔缘 4师门 5帮派 6排行 7江湖 
		public var level:int;           // 级别
		
		public function Appellation()
		{
		}
      
        public function get Color():uint        
        {
			return GetColor(color);
        }
          
        public function get BorderColor():uint
        {
        	return GetColor(borderColor);
        }
        
        /****/
        public function GetColor(nColorID:int):uint
        {
        	var colorNum:uint = 0x000000;     	
        	switch(nColorID)
        	{
        		case 1:
        		  colorNum = 0x00ff00;     
        		  break;	
        		case 2:  
        		  colorNum = 0x000000;     
        		  break;	
        		case 3:
        		  colorNum = 0x00fff6;
        		  break;
        	}
        	return colorNum;
        }
	}
}