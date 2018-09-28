package GameUI.Modules.Task.View
{
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public final class TaskAccPanel extends TaskFollowPanel
	{
		public function TaskAccPanel(maxHeight:uint)
		{
			super(maxHeight);
		}
		
		override protected function createCells():void
		{
			var currentY:Number = 0;
			var targetArr:Array = this.sortTaskType(this._dataDic)
			var cellWidth:int;
			if(GameCommonData.wordVersion != 2)
			{
				cellWidth = 200;
			}
			else
			{
				cellWidth = 190;
			}
			for each (var arr:Array in targetArr)
			{
				if (arr==null || arr.length == 0) continue;
				
				var tf:TextField = this.getAreaTf();
				container.addChild(tf);
				tf.text = arr[0].taskType;
				tf.y = currentY;
				currentY += tf.height;
				
				if (arr != null && arr.length > 0)
				{
					for each (var taskInfo:TaskInfoStruct in arr)
					{
						var headTf:TextField = this.getAreaTf();
						headTf.name = String(taskInfo.id);
						headTf.mouseEnabled = true;
						headTf.doubleClickEnabled = true;
						
						headTf.selectable = false;
						headTf.addEventListener(MouseEvent.CLICK, onDoubleClickTask);

						headTf.text = "[" + taskInfo.taskLevel + "]" + taskInfo.taskName
						headTf.textColor = 0x00FFFF;
						container.addChild(headTf);
						headTf.x = padding;
						headTf.y = currentY; 
						currentY += headTf.height;
						
						var accInfo:TaskText = new TaskText(cellWidth);
						/* var obj1:* = getNPCAreaId(taskInfo.taskNPC);
						if(!getNPCAreaId(taskInfo.taskNPC))
						{
							continue;
						}
						var obj2:* = SmallConstData.getInstance().mapItemDic[getNPCAreaId(taskInfo.taskNPC)];
						var obj3:* = SmallConstData.getInstance().mapItemDic[getNPCAreaId(taskInfo.taskNPC)].name;
						 */
						if(!getNPCAreaId(taskInfo.taskNPC))
						{
							continue;
						}
						accInfo.tfText = '<font color="#ffffff">' + GameCommonData.wordDic['mod_task_view_taska_cre_1'] + '</font>'  // 去
												+ '<font color="#fffe65">' + SmallConstData.getInstance().mapItemDic[getNPCAreaId(taskInfo.taskNPC)].name + '</font>'
												+ '<font color="#ffffff">' + GameCommonData.wordDic['mod_task_view_taska_cre_2'] + '</font>' // 找
												+ taskInfo.taskNPC;
						accInfo.x = padding;
						accInfo.y = currentY;
						container.addChild(accInfo);
						currentY += accInfo.height;
					}
				}
			}
			
			this.container.height = currentY;
			this.scrollPanel.refresh();
		}
		
		protected function getNPCAreaId(npcStr:String):Number
		{
			var index:int = npcStr.indexOf("event:");
			var idStr:String = npcStr.substr(index + 6).split(",")[0];
			return Number(idStr);
		}
	}
}