package GameUI.Modules.Bag.Command
{
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	public class SetCdData
	{
		public static var cdItemCollection:Array = [new Array(36), new Array(36), new Array(36)];
		
		public function SetCdData()
		{
			
			
		}
		
		/**
		 * cd的数据层设置 
		 * @param packageIdx
		 * @param pos
		 * @param cdTime
		 * @param type
		 * @param count
		 * @param cdType
		 * 
		 */		 
		public static function setData(packageIdx:int, pos:int, cdTime:int, type:int, count:int = -120,cdType:uint=0):void
		{
			var obj:Object = new Object();
			obj.cdType=cdType;
			obj.isCd = true;
			obj.cdtimer = cdTime;
			obj.type = type;
			obj.count = count;
			obj.packageIdx = packageIdx;
			obj.pos = pos;
			var cdID:uint = 0;
			cdItemCollection[packageIdx][pos] = obj;
			cdID = setInterval(endCd, obj.cdtimer/90, obj);
			obj.cdDelayID = cdID;
		}
		
		/**
		 * 数据层触发 
		 * @param data
		 * 
		 */	
		private static function endCd(data:Object):void
		{
			if(cdItemCollection[data.packageIdx][data.pos]==null)return;
			cdItemCollection[data.packageIdx][data.pos].count+=4;
			if(cdItemCollection[data.packageIdx][data.pos].count >= 240)
			{
				clearInterval(data.cdDelayID);
				cdItemCollection[data.packageIdx][data.pos] = undefined;
			}			
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
		
		
		
		public static function searchCdDataByType(cdType:uint):Object{
			if(cdType==0)return null;
			var len1:uint=cdItemCollection.length;
			var len2:uint=0;
			for(var i:uint=0;i<len1;i++){
				len2=cdItemCollection[i].length;
				for(var j:uint=0;j<len2;j++){
					var obj:*=cdItemCollection[i][j];
					if(obj==undefined)continue;
					if(obj.cdType==cdType){
						return obj;
					}
				}
			}
			return null;
		}
		
		
		
		
	}
}