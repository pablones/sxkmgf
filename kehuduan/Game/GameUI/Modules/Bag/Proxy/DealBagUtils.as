package GameUI.Modules.Bag.Proxy
{
	import flash.utils.ByteArray;
	
	public class DealBagUtils
	{
		public function DealBagUtils()
		{
		}
		
		public static function DealBagItem(itemList:Array):Array
		{		
			var result:Array = new Array();
			itemList.sortOn("type", Array.DESCENDING);
			////////////////////////////////////////////////////
			var tmpArr:Array = new Array();
			var tmpObj:Object = itemList[0];
			var count:int = 0;
			tmpArr[count] = new Array();
			tmpArr[count].push(tmpObj);			
			for(var i:int = 1; i<itemList.length; i++)
			{
				if( tmpObj.type == itemList[i].type )
				{
					tmpArr[count].push(itemList[i]);
				}
				else 
				{
					count++;
					tmpObj = itemList[i];
					tmpArr[count] = new Array();					
					tmpArr[count].push(tmpObj);
				}
			} 
			
			for(var j:int = 0; j<tmpArr.length; j++)
			{
				for( var n:int = 0; n<getItemByTmp(tmpArr[j]).length; n++ )
				{
					result.push(getItemByTmp(tmpArr[j])[n]);
				}
			}
			///////////////////////////////////////////////////	
			for(var m:int = 0; m<result.length; m++) 
			{
				result[m].pos = m;
			}	
			return result;
		}
		
		private static function getItemByTmp(list:Array):Array
		{
			var result:Array = new Array();
			var isBindArr:Array = new Array();
			var noBindArr:Array = new Array();
			var isBindAllNum:int = 0;
			var noBindAllNum:int = 0;
			var curObj:Object = getObj(list[0]);
			var maxNum:int = curObj.maxNum;
			for( var i:int = 0; i<list.length; i++ )
			{
				if(list[i].isBind == 0)
				{
					noBindArr.push(list[i]);
					isBindAllNum += list[i].num;
				}
				if(list[i].isBind == 1)
				{
					isBindArr.push(list[i]);
					noBindAllNum += list[i].num;
				}
			}
			var j:int = 0;
			if(isBindAllNum > maxNum)
			{				
				getLast(isBindAllNum, maxNum, 1, result, curObj);
			} else {
				if(isBindAllNum != 0)				
				{
					var isBindObj:Object = getObj(curObj);
					isBindObj.num = isBindAllNum;
					result.push(isBindObj);
				}
			}
			if(noBindAllNum > maxNum)
			{
				getLast(noBindAllNum, maxNum, 0, result, curObj);
			} else {
				if(noBindAllNum != 0)				
				{
					var noBindObj:Object = getObj(curObj);
					noBindObj.num = noBindAllNum;
					result.push(noBindObj);
				}
			}
			return result;	
		}
		
		private static function getLast(allNum:uint, maxNum:Number, isBind:int, result:Object, data:Object):void
		{
			var count:int = allNum/maxNum;
			var leaving:int = allNum%maxNum;
			for(var j:int = 0;j<count;j++)
			{
				var obj:Object = getObj(data);
				obj.num = maxNum;
				obj.isBind = isBind;
				result.push(obj);
			}
			if(leaving!=0) 
			{
				var leave:Object = getObj(data);
				leave.num = leaving;
				leave.isBind = isBind;
				result.push(leave);
			}
		}
		
		private static function getObj(obj:Object):Object
		{
			var bytes:ByteArray = new ByteArray();
			bytes.writeObject(obj);
			bytes.position = 0;
			return bytes.readObject();
		}
	}
}