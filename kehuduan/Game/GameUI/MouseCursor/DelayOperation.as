package GameUI.MouseCursor
{
	import flash.utils.Dictionary;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	
	/**
	 *  廷时操作类，可对系统中的某些操作设设置一定的时间限制
	 *  @author felix
	 * 
	 */	
	public class DelayOperation
	{
		private static var _instance:DelayOperation;
		
		/**NPC对话锁 */
		public  var isNpcTalkLock:Boolean=false;
		/** NPC对话锁ID */
		private var isNpcTalkLockId:uint;
		/** 锁字典 key[使用物品的ID]=500S（剩余延迟时间） */
		public var lockDic:Dictionary;
		
		
		public function DelayOperation()
		{
			
		}
		
		/**
		 * 获取单例 
		 * @return ：返回单例
		 * 
		 */		
		public static function getInstance():DelayOperation{
			if(_instance==null)_instance=new DelayOperation();
			return _instance;
		}
		
		/**
		 *  锁定NPC对话 
		 * 
		 */		
		public function lockNpcTalk():void{
			isNpcTalkLock=true;
			isNpcTalkLockId=setTimeout(unLockNpcTalk,10*1000);
		}
		
		/**
		 * 解除NPC对话的锁定 
		 * 
		 */		 
		public function unLockNpcTalk():void{
			clearTimeout(isNpcTalkLockId);
			isNpcTalkLock=false;
		}
		
		/**
		 * 添加物品的 
		 * @param key
		 * @param delayTime
		 * 
		 */		
		public function addDelayItem(key:*,delayTime:uint):void{
			
		}

	}
}