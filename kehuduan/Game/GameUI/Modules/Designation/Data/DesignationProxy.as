package GameUI.Modules.Designation.Data
{
	import GameUI.Modules.Designation.view.mediator.DesignationMediator;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class DesignationProxy extends Proxy
	{
		public static const NAME:String	= "designationProxy";
		private var totalDesignationArr:Array;
		private var dataPro:Array;
		public function DesignationProxy(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
			totalDesignationArr = copyArr(GameCommonData.Designation);
		}
		public  function offerData():Array
		{
			return GameCommonData.Designation;
		}
		
		/**
		 * 初始化时获取当前的玩家的称号信息
		 * */
		public function offerPlayerDesignationInfo(playerInfo:int,playerId:int):Object
		{
			var _dataArr:Array;
			if(playerId == GameCommonData.Player.Role.Id)
			{
				_dataArr = offerData();
			}
			else{
				_dataArr = totalDesignationArr;
			}
			var _id:int = int(playerInfo/100);
			var _level:int = int(playerInfo%100);
			var dataObj:Object;
			end:for each(var objItem:Object in _dataArr)
			{
				for each(var objEle:Object in objItem.data)
				{
					if(_id == objEle.id)
					{
						if(_level == objEle.level)
						{
							dataObj = objEle;
							break end;
						}
					}
				}
			}
			return dataObj;
		}
		/**查看当前是否拥有该称号*/
		public function isHaveDesignation(obj:Object):Boolean
		{
			var boo:Boolean = false;
			var _data:Array = offerData();
			if(obj)
			{
				for each(var objItem:Object in _data)
				{
					for each(var objEle:Object in objItem.data)
					{
						if(obj.id == objEle.id)
						{
							if(obj.level == objEle.level)
							{
								boo = objEle.isSelect == 1? true:false;
							}
						}
					}
				}
			}
			return boo;
		}
		private function copyArr(targetArr:Array):Array
		{
			var _data:Array = [];
			for each(var objItem:Object in targetArr)
			{
				var _item:Object = {};
				_item.name = objItem.name;
				_item.type = objItem.type;
				_item.num = objItem.num;
				_item.explain = objItem.explain;
				var _Arr:Array = [];
				for each(var objEle:Object in objItem.data)
				{
					var _ele:Object = {};
					_ele.id = objEle.id;
					_ele.extend = objEle.extend;
					_ele.level = objEle.level;
					_ele.color = objEle.color;
					_ele.bordercolor = objEle.bordercolor;
					_ele.name = objEle.name;
					_ele.isSelect = objEle.isSelect;
					_ele.introduce = objEle.introduce;
					_ele.condition = objEle.condition;
					_Arr.push(_ele);
				}
				_item.data = _Arr;
				_data.push(_item);
			}
			
			return _data;
		}
		
		/**好友显示 称号*/
		public function getDesignationName(id:int):String
		{
			var _id:int = int(id/100);
			var _level:int = int(id%100);
			var _name:String = GameCommonData.wordDic[ "mod_des_dat_des_getD" ];//"暂无称号"
			end:for each(var objItem:Object in totalDesignationArr)
			{
				for each(var objEle:Object in objItem.data)
				{
					if(_id == objEle.id)
					{
						if(_level == objEle.level)
						{
							_name = objEle.name;
							break end;
						}
					}
				}
			}
			return _name;
		}
		
		/**人物属性 称号*/
		public function offerDName():String
		{
			var _name:String = GameCommonData.wordDic[ "mod_des_dat_des_getD" ];//"暂无称号"
			var _dId:int = (facade.retrieveMediator(DesignationMediator.NAME) as DesignationMediator).getDesignatinId();
			if(_dId > 0)
			{
				var _pId:int = GameCommonData.Player.Role.Id;
				var dObj:Object = this.offerPlayerDesignationInfo(_dId,_pId); 
				if(dObj)
				{
					_name = dObj.name;
				}
			}
			return _name;
		}
	}
}
