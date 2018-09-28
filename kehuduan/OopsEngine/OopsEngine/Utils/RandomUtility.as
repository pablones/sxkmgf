package OopsEngine.Utils
{
	public class RandomUtility
	{
		/**
		 * 给入一个数量，给出一个数组
		 * 数组中为一组数字，数字自0开始至 （数量-1）
		 * 数组为一个随机数组
		 * trace(random_num(3))  //1,2,0
		 * trace(random_num(14)) //6,4,12,0,2,3,5,10,9,11,1,7,13,8
		 */
		public static function RandomNumbersArray(nums:uint):Array 
		{
			var array:Array = new Array();
			var i:uint = 0;
			while (i < nums) 
			{
				array[i] = i;
				i++;
			}
			return (RandomArray(array));
		}
		
		/**
		 * 给入一个数量，给出一个数组
		 * 数组中为一组数字，数字自0开始至9
		 * 数组中的数量为 参数指定的数量，最大不能超过10
		 */
		public static function RandomNumbersFormSingleNumbersArray(nums:uint):Array 
		{
			nums = Math.min(nums,10);
			nums = Math.max(nums,0);
			var formArray:Array = RandomNumbersArray(10);
			var array:Array = new Array(nums);
			for (var i:uint=0; i<nums; i++) 
			{
				array[i] = formArray[i];
			}
			return array;
		}
		
		/** 
		 * 给入一个数组，返回一个打乱顺序的数组
		 * 将作为 参数 传入的 数组
		 * 返回一个打乱顺序的数组
		 * 例如 var array:Array=new Array(4,5,6)
		 * 返回 5,6,4
		 */
		public static function RandomArray(inputArray:Array):Array
		{
			var cf:Function = function ():int 
			{
				var r:Number = Math.random() - 0.5;
				if (r < 0) 
				{
					return -1;
				}
				else 
				{
					return 1;
				}
			}	
			var resultArray:Array = ArrayUtility.Clone(inputArray) as Array;
			resultArray.sort(cf);
			
			return resultArray;
		}
	
		/** 
		 * 从参数ary（数组）中随机返回一个 数组项
		 * 例如 var array:Array = new Array(4,5,6)
		 * 返回 5或者6或者4
		 */
		public static function RandomExtract(ary:Array):* 
		{
			return (ary[Math.floor(Math.random() * ary.length)]);
		}
		
		/** 
		 * 给入2个数字
		 * 返回从 数字1 到 数字2 之间的数组
		 * 并且为随机
		 * trace(random_num2(4,8))
		 * 返回 4，6，7，5，8
		 */
		public static function RandomBetweenTwoNumbersArray(n1:int, n2:int):Array 
		{
			var array:Array = new Array();
			var i:int = n1;
			while (i<= n2) 
			{
				array.push(i);
				i++;
			}
			return RandomArray(array);
		}
		
		/**
		 *  生成一个介于 参数1 和 参数2 之间的随机数，精度为参数3
	     *  @param  minimum 	最小值
		 *  @param  maximum     最大值，如果缺少默认为0
		 *  @param  精度          默认值为自然数
		 *  @return             一个随机数
		 *  RandomBetweenTwoNumbers(0,100)//0-100的随机数
		 *  RandomBetweenTwoNumbers(0,100，5)//0-100的5的倍数随机数
		 *  RandomBetweenTwoNumbers(-1,1，.05)//-1至1的随机数，精确到百分之5
		 */
		public static function RandomBetweenTwoNumbers(nMinimum:Number,nMaximum:Number = 0,nRoundToInterval:Number = 1):Number 
		{
			if (nMinimum > nMaximum) 
			{
				var nTemp:Number = nMinimum;
				nMinimum = nMaximum;
				nMaximum = nTemp;
			}
			var nDeltaRange:Number=nMaximum - nMinimum + 1 * nRoundToInterval;			
			var nRandomNumber:Number=Math.random() * nDeltaRange;
			nRandomNumber += nMinimum;
			return MathUtility.Floor(nRandomNumber,nRoundToInterval);
		}
		
		/** 
		 * 将字符串打乱
		 * trace(randomString("我们的爱")) //的我爱们
		 */
		public static function RandomString(str:String):String 
		{
			var local4:Array = new Array ();
			var local2:uint = 0;
			while (local2 < str.length) 
			{
				local4.push(str.charAt(local2));
				local2++;
			}
			local4 = RandomArray(local4);
			str = local4.join("");
			return str;
		}
	}
}