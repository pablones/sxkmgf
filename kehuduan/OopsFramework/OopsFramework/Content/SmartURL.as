package OopsFramework.Content
{
	import OopsFramework.Debug.Logger;
	
	/** 整理一个网址数据，将其折分成一个数据对象。 */
    public class SmartURL  
    {
		public var url         : String;			// 未处理网址  （http://192.168.1.8:8080/A/B/C.aspx？a=1&b=2)
		public var protocol    : String;			// 协议  	 （http://）
		public var host 	   : String;			// 主机头		 （192.168.1.8）
		public var port 	   : int;				// 端口号		 （8080)
		public var path	   	   : String;			// 地址  	 （/A/B/C.aspx);
		public var queryString : String;			// 查询字符串  （a=1&b=2)
		public var queryObject : Object;			// 查询参数集合
		public var queryLength : int = 0;			// 查询参数个数

		public function SmartURL(url : String)
		{
			this.url = url;

			var URL_RE : RegExp = /((?P<protocol>[a-zA-Z]+: \/\/)   (?P<host>[^:\/]*) (:(?P<port>\d+))?)?  (?P<path>[^?]*)? ((?P<query>.*))? /x; 
			var match : * = URL_RE.exec(url);
			if (match)
			{
				protocol 	= Boolean(match.protocol) ? match.protocol : "http://";
				protocol 	= protocol.substr(0, protocol.indexOf("://"));
				host 		= match.host || null;
				port        = match.port ? int(match.port) : 80;
				path        = match.path;
				queryString = match.query;
				if (queryString)
				{
					queryObject = {};
					queryString = queryString.substr(1);
					var value   : String;
					var varName : String;
					for each (var pair : String in queryString.split("&"))
					{
						varName              = pair.split("=")[0];
						value   			 = pair.split("=")[1];
						queryObject[varName] = value;
						queryLength ++;
					}
				}
			}
			else
			{
				Logger.Print(this,"没有配置到数据");
			}
		}
		
		public function toString() : String
		{
			return "网址 :" + url + 
				   ", 协议: " + protocol + 
				   ", 端口: " + port + 
				   ", 主机头: " + host + 
				   ", 路径: " + path + 
				   ". 查询字符串: " + queryString;
		}
	}
}
