package Controller
{
	import GameUI.View.BaseUI.ItemBase;
	
	import OopsFramework.Debug.Logger;
	import OopsFramework.GameTime;
	import OopsFramework.IGameComponent;
	import OopsFramework.IUpdateable;
	
	import flash.utils.Dictionary;
	import flash.utils.getTimer;

	/**
	 * 冷却控制器。
	 *
	 * @author zhao
	 */
	public class CooldownController implements IGameComponent, IUpdateable
	{
		protected var cooldownTable:Dictionary;

		public function CooldownController(key:SingletonKey):void
		{
			cooldownTable = new Dictionary();
		}
		
		// ----------------------------------------
		//
		//		冷却状态查询
		//
		// ----------------------------------------
		public function cooldownReady(typeId:int):Boolean
		{
			var actualTypeId:int = getActualTypeId(typeId);
			
			if (!cooldownTable[actualTypeId]) return true;
			if (GCDState) return false;
			
			return !cooldownTable[actualTypeId].cooldownState;
		}
		

		// ----------------------------------------
		//
		//		普通 CD 控制
		//
		// ----------------------------------------
		public function triggerCD(typeId:int, timeSpan:Number, timeTotal:Number = NaN, GCD:Boolean = true):void
		{
			var actualTypeId:int = getActualTypeId(typeId);
			
			//Logger.Info(this, "triggerCD", "id=" + actualTypeId + ", tS=" + timeSpan);

			var cdData:CooldownData = getCooldownData(actualTypeId);

			if (isNaN(timeTotal))
			{
				cdData.startTime = getTimer();
				cdData.dueTime = cdData.startTime + timeSpan;
			}
			else
			{
				cdData.startTime = getTimer() + timeSpan - timeTotal;
				cdData.dueTime = cdData.startTime + timeTotal;
			}

			cdData.cooldownState = true;
			
			if (isSkill(actualTypeId) && GCD) triggerGCD();
		}

		public function registerCDItem(typeId:int, item:ItemBase):void
		{
			var actualTypeId:int = getActualTypeId(typeId);
			
			if (actualTypeId >= 6000 && actualTypeId <= 6006) // 这是生活技能
				return;
				
			if (actualTypeId >= 7000 && actualTypeId <= 8999 // 这是宠物主动技能
					&& !(actualTypeId >= 7031 && actualTypeId <= 7038)) // 而且不是群攻技能
				return;
			
//			Logger.Info(this, "registerCDItem", "id=" + actualTypeId);

			var cdData:CooldownData = getCooldownData(actualTypeId);

			if (cdData.CDObjectArray.indexOf(item) == -1)
			{
				cdData.CDObjectArray.push(item);
			}
		}

		public function unregisterCDItem(typeId:int, item:ItemBase):void
		{
			var actualTypeId:int = getActualTypeId(typeId);
			
//			Logger.Info(this, "unregisterCDItem", "id=" + actualTypeId);

			var cdData:CooldownData = getCooldownData(actualTypeId);
			var index:int = cdData.CDObjectArray.indexOf(item);

			if (index != -1)
			{
				cdData.CDObjectArray.splice(index, 1);
			}

			// 清理
			if (!cdData.cooldownState && !GCDState && (cdData.CDObjectArray.length == 0))
			{
				delete cooldownTable[actualTypeId];
//				Logger.Info(this, "Update", "冷却完成，清除" + actualTypeId);
			}
		}
		
		protected function isSkill(typeId:int):Boolean
		{
			return ((typeId > 1000) && (typeId < 10000)
						&& (typeId != 9500) && (typeId != 9501)
						&& (typeId != 9502) && (typeId != 9503)
						&& (typeId != 9504) && (typeId != 9505)
						&& (typeId != 9506) && (typeId != 9507)
						&& (typeId != 9509));
		}

		protected function getCooldownData(typeId:int):CooldownData
		{
			var cdData:CooldownData = cooldownTable[typeId];
			if (!cdData)
			{
				cdData = new CooldownData();
				cooldownTable[typeId] = cdData;
			}
			return cdData;
		}

		protected function updateCDProgress(typeId:String, cdData:CooldownData, progress:Number, isGCD:Boolean):void
		{
			if ((progress - cdData.lastUpdate < .01)
				&& (progress - cdData.lastUpdate > 0)
				&& (progress != 0)
				&& (progress != 1)) return;

//			Logger.Info(this, "Update", "技能 " + typeId + " 冷却进度" + (isGCD ? "(GCD): " : ": ") + Math.round(progress * 10000) / 100 + "% -> " + cdData.CDObjectArray.length);
			cdData.lastUpdate = progress;
		
			for each (var item:ItemBase in cdData.CDObjectArray)
			{
				item.update(cdData.lastUpdate);
			}
		}


		// ----------------------------------------
		//
		//		GCD 控制
		//
		// ----------------------------------------
		public var GCDState:Boolean = false;

		public var GCDStartTime:Number = 0;

		public var GCDDueTime:Number = 0;

		public function triggerGCD(timeSpan:Number = 1000):void
		{
//			Logger.Info(this, "triggerGCD", "tS=" + timeSpan);
			GCDState = true;
			GCDStartTime = getTimer();
			GCDDueTime = GCDStartTime + timeSpan;
		}
		
		
		// ----------------------------------------
		//
		//		CD 重置
		//
		// ----------------------------------------
		public function resetCD(typeId:int):void
		{
			var actualId:int = getActualTypeId(typeId);
			if (!cooldownTable[actualId]) return;
			
			var cdData:CooldownData = cooldownTable[actualId];
			cdData.cooldownState = false;
			cdData.dueTime = 0;
			
//			Logger.Info(this, "resetCD", "重置技能CD: " + actualId);
		}
		
		public function resetAllCDs():void
		{
			// 重置每个技能
			for (var key:String in cooldownTable)
			{
				var cdData:CooldownData = cooldownTable[key];
				cdData.cooldownState = false;
				cdData.dueTime = 0;				
			}
			
			// 重置 GCD
			this.GCDState = false;
			this.GCDDueTime = 0;
			
//			Logger.Info(this, "resetAllCDs", "重置所有CD");
		}
		
		public function resetPetCDs():void
		{
			for (var k:String in cooldownTable)
			{
				var key:int = parseInt(k);
				if (key >= 7000 && key < 9000) // 这是宠物技能
				{
					delete cooldownTable[key];
				}
			}
		}
		
		
		// ----------------------------------------
		//
		//		其他
		//
		// ----------------------------------------
		public function getActualTypeId(typeId:int):int
		{
			if (typeId > 300000) return int(typeId/1000);
			return typeId;
		}


		// ----------------------------------------
		//
		//		接口实现: IUpdateable
		//
		// ----------------------------------------

		/**
		 * @inheritDoc
		 */
		public function Update(gameTime:GameTime):void
		{
			var GCDProgress:Number = 1;

			// 处理 GCD
			if (this.GCDState)
			{
				if (gameTime.TotalGameTime > this.GCDDueTime)
				{
					this.GCDState = false;
//					Logger.Info(this, "Update", "GCD 结束");
				}
				else
				{
					GCDProgress = (gameTime.TotalGameTime - this.GCDStartTime) / (this.GCDDueTime - this.GCDStartTime);
//					Logger.Info(this, "Update", "GCD 进度: " + Math.round(GCDProgress * 10000) / 100 + "%");
				}
			}

			var CDProgress:Number = 1;

			// 处理普通 CD
			for (var key:String in cooldownTable)
			{
				var data:CooldownData = cooldownTable[key];

				if (!(this.GCDState && isSkill(parseInt(key))) && data.cooldownState && (gameTime.TotalGameTime > data.dueTime))
				{
					data.cooldownState = false;
//					Logger.Info(this, "Update", "CD 结束, id=" + key);

					// 清理
					if (data.CDObjectArray.length == 0)
					{
						delete cooldownTable[key];
//						Logger.Info(this, "Update", "冷却完成，清除" + key);
					}
				}
				else if ((this.GCDState && isSkill(parseInt(key))) || (gameTime.TotalGameTime < data.dueTime))
				{
					if ((gameTime.TotalGameTime < data.dueTime) && (this.GCDDueTime < data.dueTime))
					{
						CDProgress = (gameTime.TotalGameTime - data.startTime) / (data.dueTime - data.startTime);
						updateCDProgress(key, data, CDProgress, false);
					}
					else
					{
						updateCDProgress(key, data, GCDProgress, true);
					}
				}
				else if (data.lastUpdate != 1)
				{
					updateCDProgress(key, data, 1, true);
				}
			}
		}

		/**
		 * @inheritDoc
		 */
		public function get Enabled():Boolean
		{
			return true;
		}

		/**
		 * @inheritDoc
		 */
		public function get UpdateOrder():int
		{
			return 0;
		}

		/**
		 * @inheritDoc
		 */
		public function get EnabledChanged():Function
		{
			return null;
		}

		public function set EnabledChanged(value:Function):void
		{
		}

		/**
		 * @inheritDoc
		 */
		public function get UpdateOrderChanged():Function
		{
			return null;
		}

		public function set UpdateOrderChanged(value:Function):void
		{
		}


		// ----------------------------------------
		//
		//		接口实现: IGameComponent
		//
		// ----------------------------------------

		/**
		 * @inheritDoc
		 */
		public function Initialize():void
		{
		}


		// ----------------------------------------
		//
		//		单体定义
		//
		// ----------------------------------------
		protected static var instance:CooldownController = null;

		public static function getInstance():CooldownController
		{
			if (!instance)
			{
				instance = new CooldownController(new SingletonKey());
				GameCommonData.GameInstance.Components.Add(instance);
			}

			return instance;
		}
	}
}

/**
 * 冷却描述对象。
 *
 * @author zhao
 */
class CooldownData
{
//	/**
//	 * 冷却持续时间。 
//	 */	
//	public var span:int;
	
	/**
	 * 冷却开始时间。
	 */
	public var startTime:Number = NaN;

	/**
	 * 冷却结束时间。
	 */
	public var dueTime:Number = NaN;

	/**
	 * 上次呈现更新时间。
	 */
	public var lastUpdate:Number = 0;

	/**
	 * 是否正在冷却。
	 */
	public var cooldownState:Boolean = false;

	/**
	 * 是否受 GCD 影响。
	 */
	public var GCDEnabled:Boolean = true;

	/**
	 * 冷却实体。<i>可能有一个或多个。</i>
	 */
	public var CDObjectArray:Array = [];
}

class SingletonKey
{
}
