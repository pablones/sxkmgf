package GameUI.Modules.Login.Jobchange
{
	public class Jobchange
	{
		public function Jobchange()
		{
		}
		public static function homechange(index:int):int
		{
			var i:int ;
			switch(index)
			{
				case 0:
				  i = 16;
				break;
				
				case 1:
				  i = 8;
				break;
				
				case 2:
				  i = 4;
				break;
				
				case 3:
				  i = 2;
				break;
				
				case 4:
				  i = 32;
				break;
				
				case 5:
				  i = 1;
				break;
			}
			return i;
		}
		
		public static function homechangeback(index:int):int
		{
			var i:int ;
			switch(index)
			{
				case 16:
				  i = 0;
				break;
				
				case 8:
				  i = 1;
				break;
				
				case 4:
				  i = 2;
				break;
				
				case 2:
				  i = 3;
				break;
				
				case 32:
				  i = 4;
				break;
				
				case 1:
				  i = 5;
				break;
				
				case 4096:               //新手数值
				  i = 6;
				break;
				
				case 0:                 //暂无
				  i = 6;
				break;
				
			}
			return i;
		}

	}
}