package GameUI.Modules.PlayerInfo.UI
{
	import GameUI.Modules.PlayerInfo.Command.TeamEvent;
	
	import OopsEngine.Role.GameRole;
	
	import flash.display.Sprite;

	public class TeamList extends Sprite
	{
		
		/**
		 * 数据提供者 
		 */		
		protected var _dataPro:Array;
		/**
		 * 渲染器集合
		 */		
		protected var cells:Array;
		/** 间隔 */
		public static const PADDING:uint=5;
		
		
		public function TeamList(dataPro:Array=null)
		{
			super();
			this._dataPro=dataPro;
			this.cells=[];
		}
		
		protected function createChildren():void{
			this.cells=[];
			var i:uint=0;
			for each(var role:GameRole in this._dataPro){
				var cell:TeamCell=new TeamCell(role)
				var mc:Sprite = new Sprite();
				mc.name = "Role_"+i;
				mc.graphics.beginFill(0xffffff,0);
				mc.graphics.drawRect(38, 15, 123, 19);
				mc.graphics.endFill();
				cell.contentMc.addChild(mc);
				cell.addEventListener(TeamEvent.CELL_CLICK,onCellCLick);
				this.addChild(cell);
				this.cells.push(cell);
				i++;
			}
			this.doLayout();
		}
		
		
		protected function onCellCLick(e:TeamEvent):void{
			var event:TeamEvent=new TeamEvent(TeamEvent.CELL_CLICK,false,false);
			event.flagStr=e.flagStr;
			event.role=e.role;
			this.dispatchEvent(event);
		}
		
		
		protected function doLayout():void{
			var currentY:Number=0;
			for each(var cell:TeamCell in this.cells){
				cell.x=0;
				cell.y=currentY;
				currentY+=cell.height+PADDING;
			}
		}
		
		public function removeAll():void{
			for each(var cell:TeamCell in this.cells){
				if(this.contains(cell)){
					cell.removeEventListener(TeamEvent.CELL_CLICK,onCellCLick);
					this.removeChild(cell);
					cell = null;
				}
			}
		}
		
		public function set dataPro(value:Array):void{
			this.removeAll();
			this._dataPro=value;
			this.createChildren();
		}
		
		public function get dataPro():Array{
			return _dataPro;
		}
		
		public function upDate(arr:Array):void{
			var len:uint=arr.length;
			for(var i:int=0;i<len;i++){
				var role:GameRole=arr[i] as GameRole;
				var cell:TeamCell=cells[i] as TeamCell;
				cell.setHp(role.HP,role.MaxHp+role.AdditionAtt.MaxHP);
				cell.setMp(role.MP,role.MaxMp+role.AdditionAtt.MaxMP);
				cell.setLevel(role.Level); 
				cell.setFace(role.Face);
				cell.setBuffs(role);
			}
		}
		
		/**
		 * 改变人物离线与在线的状态（离线，一线，二线，三线） 
		 * @param roleId
		 * @param status
		 * 
		 */		
		public function updateStatus(roleId:uint,status:uint=0):void{
			var teamCell:TeamCell=this.searchTeamCellById(roleId);
			if(teamCell==null)return;
			var des:String="";
			switch (status){
				case 0:
					des='<font color="#ff0000">' + GameCommonData.wordDic[ "often_used_leave" ] + '</font>';  // 离开
					break;
				case 1:
					des="";
					break;
				case 2:
					break;
				case 3:
					break;
				default:
					break;								
			}
			teamCell.updateLineDes(des,status);
		}
		
		/**
		 * 根据玩家ID号寻找队伍玩家Cell 
		 * @param roleId
		 * @return 
		 * 
		 */		
		protected function searchTeamCellById(roleId:uint):TeamCell{
			for each(var teamCell:TeamCell in this.cells){
				if(teamCell.role.Id==roleId)return teamCell;
			}
			return null;
		}
			
	}
}