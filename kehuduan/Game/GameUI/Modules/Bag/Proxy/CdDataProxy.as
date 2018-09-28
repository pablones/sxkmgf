package GameUI.Modules.Bag.Proxy
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;

	public class CdDataProxy implements IUpdateable
	{
		private static var _instance:CdDataProxy;
		public static var cdItemCollection:Array = [new Array(36), new Array(36), new Array(36)];
		
		protected var timer:Timer;
		
		public function CdDataProxy()
		{
			this.timer=new Timer();
			timer.DistanceTime=100;
		}
		
		public static function getInstance():CdDataProxy{
			if(_instance==null)_instance=new CdDataProxy();
			return _instance;
		}

		/**
		 * 更新 
		 * @param gameTime
		 * 
		 */		
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime)){
				for(var i:uint=0;i<36;i++){
					if(cdItemCollection[0][i]!=null && cdItemCollection[0][i]!=undefined){
						var obj=cdItemCollection[0][i];
						obj.count-=100;
						if(obj.count<=0){
							cdItemCollection[0][i]=undefined;
						}
					}					
				}		
			}
		}
		
		 
		/**
		 * 设置CD时间数据代理
		 * @param packageIdx
		 * @param pos
		 * @param cdTime
		 * @param type
		 * 
		 */		
		public function setCdData(packageIdx:int, pos:int, cdTime:int, type:int):void{
			var obj:Object = new Object();
			obj.isCd = true;
			obj.cdtimer = cdTime;
			obj.type = type;
			obj.count = cdTime;
			obj.packageIdx = packageIdx;
			obj.pos = pos;
			cdItemCollection[packageIdx][pos] = obj;
		}
		
		
		/**
		 *  
		 * @param type :物品type号
		 * @return  ：cd数据结构对象
		 * 
		 */		
		public static function searchCdObjByType(type:uint):Object{
			
			var len1:uint=cdItemCollection.length;
			var len2:uint=0;
			for(var i:uint=0;i<len1;i++){
				len2=cdItemCollection[i].length;
				for(var j:uint=0;j<len2;j++){
					
					var obj:*=cdItemCollection[i][j];
					if(obj==undefined)continue;
					if(obj.type==type){
						return obj;
					}
				}
			}
			return null;
		}
		
		
		
		
		

		
		
		//-----------------------------接口的空实现 ---------------------------------------------------
		public function get Enabled():Boolean
		{
			return false;
		}
		
		public function get UpdateOrder():int
		{
			return 0;
		}
		
		public function get EnabledChanged():Function
		{
			return null;
		}
		
		public function set EnabledChanged(value:Function):void
		{
		}
		
		public function get UpdateOrderChanged():Function
		{
			return null;
		}
		
		public function set UpdateOrderChanged(value:Function):void
		{
		}
		//----------------------------------------------------------------------------------------
	}
}