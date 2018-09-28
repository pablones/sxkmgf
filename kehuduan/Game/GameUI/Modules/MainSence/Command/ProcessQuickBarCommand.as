package GameUI.Modules.MainSence.Command
{
	import Controller.CooldownController;
	
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.MainSence.Proxy.QuickGridManager;
	import GameUI.View.items.UseItem;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Skill.GameSkillLevel;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ProcessQuickBarCommand extends SimpleCommand
	{

		public function ProcessQuickBarCommand()
		{
			super();
		}

		public override function execute(notification:INotification):void
		{

			var obj:Object = notification.getBody();
			for (var i:int = 0; i < 8; i++)
			{
				var id:uint = obj["key" + (i + 1)];
				var realId:uint = Math.floor(id / 10);
				var isBind:uint = (id - realId * 10);
				var idF:uint = obj["keyF" + (i + 1)];
				var realIdF:uint = Math.floor(idF / 10);
				var idBindF:uint = (idF - realIdF * 10);
				var useItem:UseItem;

				if (id != 0 && isRightType(realId))
				{
					useItem = new UseItem(-1, String(this.changeIdToType(realId)), null);
					useItem.IsBind = isBind;

					if (realId > 300000 && this.getNumFromBag(realId, isBind) > 0 || realId < 10000)
					{
						useItem.Num = this.getNumFromBag(realId, isBind);
						QuickBarData.getInstance().quickKeyDic[String(i)] = useItem;

						CooldownController.getInstance().registerCDItem(realId, useItem);
					}
					else
					{
						QuickBarData.getInstance().quickKeyDic[i] = null;
					}
				}
				else
				{
					QuickBarData.getInstance().quickKeyDic[i] = null;
				}

				if (idF != 0 && isRightType(realIdF))
				{
					if (i == 7)
						continue;
					useItem = new UseItem(-1, String(this.changeIdToType(realIdF)), null);
					useItem.IsBind = idBindF;
					if (realIdF > 300000 && this.getNumFromBag(realIdF, idBindF) > 0 || realIdF < 10000)
					{
						useItem.Num = this.getNumFromBag(realIdF, idBindF);
						QuickBarData.getInstance().expandKeyDic[String(i)] = useItem;

						CooldownController.getInstance().registerCDItem(realIdF, useItem);
					}
					else
					{
						QuickBarData.getInstance().expandKeyDic[i] = null;
					}
				}
				else
				{
					if (i == 7)
						continue;
					QuickBarData.getInstance().expandKeyDic[i] = null;
				}
			}

			var quickGridManager:QuickGridManager = facade.retrieveProxy(QuickGridManager.NAME) as QuickGridManager;
			quickGridManager.refresh();
		}

		/**
		 * 判断是否是正确的数据（如果宠物没有出战，宠物技能就不会有）
		 * @param type：技能Type
		 * @return
		 *
		 */
		protected function isRightType(type:uint):Boolean
		{
			if (type > 300000)
			{
				return true;
			}
			else
			{
				if (type > 7000 && type < 8999 && GameCommonData.Player.Role.UsingPet == null)
				{
					if (GameCommonData.Player.Role.UsingPet == null)
					{
						return false
					}
					else
					{
						var role:GamePetRole = GameCommonData.Player.Role.PetSnapList[GameCommonData.Player.Role.UsingPet.Id] as GamePetRole;
						if (role != null)
						{
							var arr:Array = role.SkillLevel;
							for each (var a:GameSkillLevel in arr)
							{
								if (a.gameSkill.SkillID == type)
								{
									return true;
								}
							}
						}
						return false;
					}
				}
				else
				{
					return true;
				}
			}
		}


		/**
		 * 获得物品的数量
		 * @param type
		 * @param isBind
		 * @return
		 *
		 */
		protected function getNumFromBag(type:uint, isBind:uint):uint
		{
			var count:uint = 0;
			for each (var obj:* in(BagData.AllItems[0] as Array))
			{
				if (obj != undefined)
				{
					if (obj.type == type && obj.isBind == isBind)
					{
						count += obj.amount;
					}
				}
			}
			return count;
		}





		/**
		 * 更据ID号获得type类型，以用来生成图标
		 * @param id
		 * @return type:物品的type类型
		 *
		 */
		protected function changeIdToType(id:uint):uint
		{
			if (id < 100000)
			{
				return id;
			}
			else
			{
				var obj:Object = UIConstData.getItem(id);
				if (obj != null)
				{
					return obj.type;
				}
			}
			return 0;
		}

	}
}
