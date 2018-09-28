package GameUI.Modules.Designation.Data
{
	import Net.ActionProcessor.UserTitle;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class DesignationCommand extends SimpleCommand
	{
		public static const NAME:String = "designationCommand";
		private var _data:Array;
		public function DesignationCommand()
		{
			super();
		}
		public override function execute(notification:INotification):void
		{
			var obj:Object = notification.getBody();
			switch(obj.type)
			{
				case UserTitle.MSG_SYNCHRO://同步称号
					_data = GameCommonData.Designation;
					var playerData:Array = obj.value as Array;
				
					for each(var objTitle:Object in playerData)
					{
						var num:int = int(objTitle.titleLev);
						 for(var tempLevel:int = 1; tempLevel <= num; tempLevel ++)
						{
							update(objTitle.titleType,tempLevel)
						} 
					}
					selectJob();
					selectType();
					orderArr();				
					break;
				case UserTitle.MSG_ADDTITLE://增加称号
					updateDesignation(obj.type,obj.value); 
					break;
				case UserTitle.MSG_UPDATETITLE://跟新称号  
					updateDesignation(obj.type,obj.value);
					break;
				case UserTitle.MSG_DELETETITLE://删除称号
					updateDesignation(obj.type,obj.value);
				 	break;
			}
		}
		/**选出当前的称号*/	
		private function update(tempId:int,tempLevel:int):void
		{
			for each(var objItem:Object in _data)
			{
				for each(var objEle:Object in objItem.data)
				{
					if(objEle.id == tempId)
					{
						if(objEle.level == tempLevel)
						{
							objEle.isSelect = 1;
						}
					}
				}
			}
		}
		
		/**跟新称号*/		
		public function updateDesignation(type:uint,data:Array):void
		{
			var tempData:Array = GameCommonData.Designation;
			end:for(var i:int = 0,length:int = tempData.length; i < length; i ++)
			{
				var num:int = tempData[i].num;
				var tempArr:Array = tempData[i].data;
				for(var j:int = 0,long:int = tempArr.length; j < long; j ++)
				{
					for(var z:int = 0,len:int = data.length; z < len; z ++)
					{
						if(tempArr[j].id == data[z].titleType)
						{
							if(tempArr[j].level == data[z].titleLev)
							{
								if(type == UserTitle.MSG_ADDTITLE)//增加称号
								{
									tempArr[j].isSelect = 1;
								}
								if(type == UserTitle.MSG_UPDATETITLE)//称号升级
								{
									tempArr[j].isSelect = 1;
								}
								dealWithData(tempArr[j]);
								break end;
							}
							else if(0 == data[z].titleLev)//删除称号
							{
								if(type == UserTitle.MSG_DELETETITLE)//删除称号
								{
									tempArr[j].isSelect = 0;
									var currentObj:Object = dealKind(tempArr,tempArr[j]);
									if(num == 4)//帮派称号
									{
										dealWithData(tempArr[j]);
									}
									else{
										dealWithData(currentObj);
									}
									break end;
								}
							}
						}
					}
				}
			} 
			orderArr();
			facade.sendNotification(DesignationEvent.UPDATE_DATAARR);
		}
		/**删除称号是对改类别称号处理*/
		private function dealKind(data:Array,obj:Object):Object
		{
			var _dataObj:Object;
			var oStyle:int = int(obj.id%100);
			var type:int = int(obj.extend);
			for each(var objEle:Object in data)
			{
				var eStyle:int = int(objEle.id%100);	
				if(oStyle == eStyle)
				{
					if(objEle.level == obj.level-1)
					{
						objEle.isSelect = 1;
						_dataObj = objEle;
					}
				}
			}
			return _dataObj;
		}
		/**跟新后排序处理*/
		private function dealWithData(currentObj:Object):void
		{
			var tempData:Array = GameCommonData.Designation;
			var tempId:int = int(currentObj.id/1000);
			var type:int = currentObj.extend;
			var currentStyle:int = int(currentObj.id%100);
			for each(var objItem:Object in tempData)
			{
				if(objItem.num == tempId - 1)//选出大类型
				{
					var dataArr:Array = objItem.data;
					if(type == 0)//冲突类型
					{
						for each(var objEle:Object in dataArr)
						{
							var style0:int = int(objEle.id%100);
							if(currentStyle == style0)
							{
								if(objEle.level != currentObj.level)
								{
									objEle.isSelect = 0;
								}
							}
						}
					}
					else if(type == 1)
					{
						for each(var objE:Object in dataArr)
						{
							var style1:int = int(objE.id%100);
							if(currentStyle == style1)
							{
								if(objE.level < currentObj.level)
								{
									objE.isSelect = 1;
								}
								if(objE.level > currentObj.level)
								{
									objE.isSelect = 0;
								}
							}
						}
					}
				}
			}
		}
		
		/**选择门派*/
		private function selectJob():void
		{
			var mainJobId:uint = GameCommonData.Player.Role.MainJob.Job;//当前主职业编号
			var viceJobId:uint = GameCommonData.Player.Role.ViceJob.Job;//当前副职业编号
			for each(var objItem:Object in _data)
			{
				var num:int = objItem.num;
				var extendArr:Array = [];
				if(num == 3)
				{
					for each(var objEle:Object in objItem.data)
					{
						if(mainJobId == 1 || viceJobId == 1)//唐门
						{
							if(int(objEle.id%10) == 1)
							{
								extendArr.push(objEle);
							}
						}
						if(mainJobId == 2 || viceJobId == 2)//全真
						{
							if(int(objEle.id%10) == 2)
							{
								extendArr.push(objEle);
							}
						}
						if(mainJobId == 4 || viceJobId == 4)//峨眉
						{
							if(int(objEle.id%10) == 3)
							{
								extendArr.push(objEle);
							}
						}
						if(mainJobId == 8 || viceJobId == 8)//丐帮
						{
							if(int(objEle.id%10) == 4)
							{
								extendArr.push(objEle);
							}
						}
						if(mainJobId == 16 || viceJobId == 16)//少林
						{
							if(int(objEle.id%10) == 5)
							{
								extendArr.push(objEle);
							}
						}
						if(mainJobId == 32 || viceJobId == 32 )//点苍
						{
							if(int(objEle.id%10) == 6)
							{
								extendArr.push(objEle);
							}
						}
						else if(mainJobId == 4096 || viceJobId == 4096 )//新手
						{
							
						}
					}
					if(num == 3)
					{
						objItem.data = extendArr;
					}
				}
			}
		}
		/**选择类型*/
		private function selectType():void
		{
			for each(var objItem:Object in _data)
			{
				var dataArr:Array = objItem.data;
				if(dataArr != null)
				{
					if(dataArr[0])
					{
						if(dataArr[0].extend == 0)
						{
							var tempNum:int = -1;
							var tempLevel:int = -1
							for(var i:int = 0,length:int = dataArr.length; i < length; i ++)
							{
								if(dataArr[i].isSelect == 1)
								{
									if(dataArr[i].level >tempLevel)
									{
										if(tempNum >= 0)
										{
											dataArr[tempNum].isSelect = 0;
										}
										tempLevel = dataArr[i].level;
										tempNum = i;
									}
								}
							}
						}	
					}
				}
			}
		}
		/**对数组中的对象排序，可选的排前面*/
		private function orderArr():void
		{
			var _data:Array = GameCommonData.Designation;
			for each(var objItem:Object in _data)
			{
				var dataArr:Array = [];
				var tempArr:Array = objItem.data;
				var lenght:int = tempArr.length;
				for(var i:int = 0; i < lenght; i ++)
				{
					if(tempArr[i].isSelect == 1)
					{
						dataArr.push(tempArr[i] );
						tempArr.splice(i,1);
						if(i == 0)
						{
							i = 0;
						}
						else{
							i --;
						}
						lenght --;
					}
					 if(i == lenght-1)
					{
						if(tempArr.length > 0)
						{
							if(tempArr[0].isSelect == 1)
							{
								dataArr[dataArr.length] = tempArr[0];
								tempArr.splice(0,1);
							}
						}
						listArr(dataArr);
						dataArr = dataArr.concat(tempArr);
					} 
				} 
				objItem.data = dataArr;
			} 
		}
		
		
		/**跟新level排序，等级小的排前面*/
		private function listArr(tempArr:Array):void
		{
			var len:int = tempArr.length;
			if(len > 1)
			{
				for(var i:int = 0; i < len; i ++)
				{
					 for(var j:int = len-1; j > i; j --)
					{
						if(int(tempArr[j].level) < int(tempArr[j-1].level))
						{
							var obj:Object = tempArr[j];
							tempArr[j] = tempArr[j-1];
							tempArr[j-1] = obj;
						}
					} 
				}
			} 
		}
	}
}