package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	/**
	 * 
	 * @author felix
	 * 
	 */
	public class UIFriendContainer extends Sprite
	{
		/**  数据提供者 */		
		protected var _dataPro:Array;
		/** 渲染器数组  */
		protected var cells:Array;
		/** 渲染器缓冲  */		
		protected var cacheCells:Array;
		/** 好友容器 */		
		protected var friendCon:UIScrollPane;
		/**  各组好友的数据提供者（【组】【name,name....】） */
		protected var _fListDataPro:Array;
		/**  好友列表组件  */
		public var fList:UIFriendsList;

		/** 选中索引 */
		protected var selectedIndex:uint=0;
		
	
		public var w:Number=148;
		public var h:Number=279;
		
		public function UIFriendContainer(value:Array=null,friendListDataPro:Array=null)
		{
			super();  
			this._dataPro=value;
			this._fListDataPro=friendListDataPro;
			this.createChildren();
		}
		
		/**
		 * 
		 * @param value
		 * 
		 */		
		public function set fListDataPro(value:Array):void{
			this.sort2On(value);
			this._fListDataPro=value;
			this.toRepaint({rePaintType:0});	
		}
		
		
		public function get fListDataPro():Array{
			return this._fListDataPro;
		}
		
		
		/**
		 * 对二维数组进行排序 
		 * @param arr
		 * 
		 */		
		protected function sort2On(arr:Array):void{
			for each(var data:Array in arr){
				this.sortOn(data);
			}
		}
		
		/**
		 *  对一维数组进行排序 
		 * @param arr
		 * 
		 */		
		protected function sortOn(arr:Array):void{
			for(var i:uint=0;i<arr.length;i++){
				for(var j:uint=0;j<arr.length-i-1;j++){
					var val1:PlayerInfoStruct=arr[j].roleInfo as PlayerInfoStruct;
					var val2:PlayerInfoStruct=arr[j+1].roleInfo as PlayerInfoStruct;
					if(val1!=null && val2!=null){
						if(val1.isOnline<val2.isOnline){
							var temp:Object=arr[j];
							arr[j]=arr[j+1];
							arr[j+1]=temp;
						}
					}
						
				}
			}
		}
		
		/**
		 * 创建并初始化UI 
		 * 
		 */		
		protected function createChildren():void{
			this.cells=new Array();
			this.cacheCells=new Array();
			
			this.fList=new UIFriendsList(this.fListDataPro[this.selectedIndex]);
			this.fList.addEventListener(MenuEvent.Cell_Click,onCellClickHandler);
			this.fList.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onDoubleClickHandler);
			this.friendCon=new UIScrollPane(fList);
			this.friendCon.width=this.w+6;
			this.friendCon.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.friendCon.refresh();
			this.addChild(this.friendCon);
			
			if(this._dataPro==null || this._dataPro.length==0)return;
			for each(var cellText:String in this.dataPro){
				
				var tf:FriendGroup=new FriendGroup();
				tf.setName(cellText);
				tf.addEventListener(MouseEvent.CLICK,onMouseClickHandler);
				this.addChild(tf);
				this.cells.push(tf);
			}
			this.doLayout();
		}
		
		protected function onDoubleClickHandler(e:MenuEvent):void{
			this.dispatchEvent(new MenuEvent(e.type,false,false,null,e.roleInfo));
		}
		
		
		protected function onCellClickHandler(e:MenuEvent):void{
			
			this.dispatchEvent(new MenuEvent(e.type,false,false,e.cell,e.roleInfo));
		}
		
		protected function onMouseClickHandler(e:MouseEvent):void{
			var index:int=this.cells.indexOf(e.currentTarget);
			this.selectedIndex=index;
			this.removeChild(this.friendCon);
			this.fList=new UIFriendsList(this.fListDataPro[this.selectedIndex])
			this.fList.addEventListener(MenuEvent.Cell_Click,onCellClickHandler);
			this.fList.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onDoubleClickHandler);
			this.friendCon=new UIScrollPane(fList);
			this.friendCon.width=this.w+6;
			this.friendCon.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.friendCon.refresh();
			this.addChild(this.friendCon);
	
			this.doLayout();
		}
		
		/**
		 * 重绘 
		 * 
		 */		
		protected function toRepaint(updateHash:Object=null):void{
			if(this._dataPro==null || this._dataPro.length==0)return;
			this.removeChild(this.friendCon);
			this.fList=new UIFriendsList(this.fListDataPro[this.selectedIndex]);
			this.fList.addEventListener(MenuEvent.Cell_Click,onCellClickHandler);
			this.fList.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onDoubleClickHandler);
			this.friendCon=new UIScrollPane(fList);
			this.friendCon.width=this.w+4;
			this.friendCon.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.friendCon.refresh();
			this.addChild(this.friendCon);
			
			this.doLayout();
		}
		
		
		/**
		 * 检测重绘的类型 
		 * @return -1:旧的数据比新的少  0：新旧数据数量相同  1：旧的数据比新的数据多
		 * 
		 */		
		protected function checkRepaintType(oldData:Array,newData:Array):int{
			if(oldData.length==newData.length)return 0;
			return oldData.length>newData.length ? 1 : -1;
		}
		
		
		/**
		 * 移除所有的渲染器 
		 * 
		 */		
		protected function removeAllCells():void{
			
		}
		
		/**
		 * 获取渲染器
		 * @return 渲染器 
		 * 
		 */		
		protected function getCell():*{
			var cell:*=this.cacheCells.shift();
			if(cell==null){
				cell=new FriendGroup();
				this.addChild(cell);
			}
			return cell;
		}
		
		/**
		 * 布局 
		 * 
		 */		
		protected function doLayout():void{
			var currentY:uint=0;
			for(var i:int=0;i<=this.selectedIndex;i++){
				this.cells[i].x=3;
				this.cells[i].y=currentY;
				currentY+=(this.cells[index]).height;
			}
			this.friendCon.height=(this.h-this.cells.length*this.cells[0].height)-2;
			this.friendCon.refresh();
			this.friendCon.x=1;
			this.friendCon.y=currentY;
			
			currentY=this.h;
			var len:uint=this._dataPro.length;
			for(var index:uint=len-1;index>this.selectedIndex;index--){
				currentY-=(this.cells[index]).height;
				this.cells[index].x=3;
				this.cells[index].y=currentY;
			}
			this.friendCon.refresh();
		}
		/**
		 * 获取与设置数据提供者
		 * @param value
		 * 
		 */		
		public function set dataPro(value:Array):void{
			if(value==this._dataPro)return;
			var obj:Object={};
			obj["rePaintType"]=this.checkRepaintType(this._dataPro,value);
			this._dataPro=value;
			this.toRepaint(obj);
		}
		
		/**
		 * 获取与设置数据提供者
		 * @return  value
		 * 
		 */		
		public function get dataPro():Array{
			return this._dataPro;
		}
			
	}
}