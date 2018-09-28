package GameUI.Modules.Buff.UI
{
	import GameUI.Modules.Buff.InterFace.IBuffController;
	
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.display.Sprite;
	import flash.geom.Point;
	
	public class BuffController extends Sprite implements IBuffController
	{
		private static const STARTPOS:Point = new Point(7 , 20);
		public var timeUpdateControll:Function;			/** 控制心跳 */
		public function BuffController()
		{
			
		}
		/** 添加Buff */
		public function addBuff(buff:GameSkillBuff):void
		{
			for(var i:int = 0; i < this.numChildren ; i++)
			{
				if((this.getChildAt(i) as BuffItem).index == buff.BuffID)
				{
					return;
				}
			}
			var buffItem:BuffItem = new BuffItem(buff);
			buffItem.deleteOwn = deleteBuff;			//手动删除时间到的Buff
			this.addChild(buffItem);
			updateBuff();
			timeUpdateControll("add");					//添加心跳
			
		}
		/** 删除指定Buff */
		public function deleteBuff(buff:GameSkillBuff):void
		{
			for(var i:int = 0; i < this.numChildren ; i++)
			{
				if((this.getChildAt(i) as BuffItem).index == buff.BuffID)
				{
					this.removeChildAt(i);
				}
			}
			updateBuff();
			timeUpdateControll("delete");				//删除心跳
		}
		/** 排列 */
		public function updateBuff():void
		{
			for(var i:int = 0; i < this.numChildren ; i++)
			{
				this.getChildAt(i).x = (this.getChildAt(i).width  + STARTPOS.x)  * int((11-i) % 6) ;
				this.getChildAt(i).y = (this.getChildAt(i).height + STARTPOS.y ) * int(i / 6) ;
			}
		}
		/** 全部Buff计时 */
		public function BuffGo():void
		{
			for(var i:int = 0; i < this.numChildren ; i++)
			{
				if((this.getChildAt(i) as BuffItem).time == -1) continue;
				(this.getChildAt(i) as BuffItem).time -= 1; 
			}
		}
		

	}
}