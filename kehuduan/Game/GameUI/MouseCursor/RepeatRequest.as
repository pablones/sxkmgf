package GameUI.MouseCursor
{
	import flash.utils.Dictionary;
	import flash.utils.clearTimeout;
	import flash.utils.setInterval;
	
	
	/**
	 * 错误命令进行重新请求 
	 * @author felix
	 * 
	 */	
	public class RepeatRequest
	{
		private static var _instance:RepeatRequest;
		/** 处理DIC*/
		private var processDic:Dictionary;
		/** 背包与装备的数量统计*/
		public var bagItemCount:uint;
		/** 宠物数量的统计 */
		public var petItemCount:uint;
		/** 技能数量统计 */
		public var skillItemCount:uint;
		/** 快捷键*/
		public var quickKeyCount:uint;
		/** cd数量统计*/
		public var cdCount:uint;
		/** 任务数量*/
		public var taskCount:uint;
		/** 其它杂项数量*/
		public var otherCount:uint;
		
		
		                           /**[背包0][宠物1][技能2][快捷键3][cd4][任务5][other6]   */
		public var successFlags:Array=[false,false,false,false,false,false,false]
		
		public function RepeatRequest()
		{
			processDic=new Dictionary();
		}
		
		public static function getInstance():RepeatRequest{
			if(_instance==null)_instance=new RepeatRequest();
			return _instance;
		}
		
		
		/**
		 *  
		 * @param key :关键字
		 * @param fun ：执行方法
		 * @param delayTime ：延迟时间
		 * 
		 */		
		public function addDelayProcessFun(key:String,fun:Function,delayTime:uint):void{
			var delayTimeId:uint=setInterval(fun,delayTime);
			this.processDic[key]=delayTimeId;
		}
		
		/**
		 * 册除延时要执行FUN
		 * @param key :关键字
		 * 
		 */		
		public function removeDelayProcessFun(key:String):void{
			var id:*=this.processDic[key];
			if(id!=null){
				clearTimeout(id);
			}
		}
		
		public function clearAllInterval():void{
			for each(var id:* in processDic){
				clearTimeout(id);
			}
		}

	}
}