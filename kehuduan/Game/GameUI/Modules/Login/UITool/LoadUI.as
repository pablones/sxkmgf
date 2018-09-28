package GameUI.Modules.Login.UITool
{
	import flash.geom.Point;
	
	public class LoadUI
	{
		public function LoadUI()
		{
		}
		public static function getPoint(type:String):Point
		{
			var point:Point; 
			switch(type)
			{
				case "1_0":		//唐门男
					point = new Point(310,0);
				break;
				case "1_1":		//唐门女
					point = new Point(220,5);
				break;
				case "2_0":		//全真男
					point = new Point(330,5);
				break;
				case "2_1":		//全真女
					point = new Point(290,-5);
				break;
				case "4_0":		//峨眉男
					point = new Point(210,0);
				break;
				case "4_1":		//峨眉女
					point = new Point(300,5);
				break;
				case "8_0":		//丐帮男
					point = new Point(245,5);
				break;
				case "8_1":		//丐帮女
					point = new Point(300,5);
				break;
				case "16_0":	//少林男
					point = new Point(320,5);
				break;
				case "16_1":	//少林女
					point = new Point(200,-5);
				break;
				case "32_0":	//点苍男
					point = new Point(280,25);
				break;
				case "32_1":	//点苍女
					point = new Point(310,-3);
				break;
			}
			return point;
		}
	}
}