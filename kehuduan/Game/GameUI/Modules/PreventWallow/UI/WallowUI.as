package GameUI.Modules.PreventWallow.UI
{
	public class WallowUI
	{
		private static const COEFFICIENT:Array = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2];
        private static const AUTH:Array = [1, 0, "X", 9, 8, 7, 6, 5, 4, 3, 2];
        private static const DIVIDER:int = 11; 
        private static const LENGTH:int = 17;
		public function WallowUI()
		{
		}
        /**
         * 
         * @param    id  验证身份证是否存在
         * @return
         */
        public static function checkId(id:String):Boolean
        {            
            var list:Array = id.match(/\d{1}/g);
            
            //
            if (list.length < LENGTH || id.length != LENGTH + 1) return false;
            
            //
            var sum:int = 0;            
            for (var i:int = 0; i < LENGTH; i++ )
            {
                sum += list[i] * COEFFICIENT[i];
                
            }
            
            var mode:int = sum % DIVIDER;            
            if (String(AUTH[mode]).toLowerCase() == id.charAt(id.length - 1).toLowerCase())
                return true;
            return false;
        }
        /**
         * 验证是否到了合法年龄
         * 
         */ 
         public static function checkAge(id:String):Boolean
         {
         	var isOk:Boolean = false;
         	var birthday:int;
         	var nowTime:int;
         	var month:String;
         	var day:String;
         	if(id.length == 18)
         	{
         		birthday = int(id.slice(6 , 14));
         		
         	}
         	else if(id.length == 15)
         	{
         		birthday = int("19" + id.slice(6 , 12));
         	}
     		nowTime = int(String(GameCommonData.gameYear) + String(GameCommonData.gameMonth) + String(GameCommonData.gameDay));
     		if((nowTime - birthday) > 180000)
     		{
     			isOk = true;
     		}
         	return isOk;
         }
         
         public static function checkType(in_str:String):Boolean
         {
   	    	  var boo:Boolean;
  			  for (var i = 0; i<in_str.length; i++) 
  			  {
  		  		if(in_str.charCodeAt(i) > 255)
  		  		{
  		  			boo = true;
  		  			break;
  		  		}
  			  }
  			  return boo;
		}
    }
}
