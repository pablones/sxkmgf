package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.Sprite;

	public class UIEnemyContainer extends Sprite
	{
		/** 滚动面板  */
		protected var scrollPane:UIScrollPane;
		/** 仇人列表  */
		public var enemyList:UIEnemyList;
		
		public function UIEnemyContainer()
		{
			super();
			createChildren();
		}
		
		/**
		 * 创建子元件 
		 * 
		 */		
		protected function createChildren():void{
			this.enemyList=new UIEnemyList();
			this.enemyList.addEventListener(MenuEvent.Cell_Click,onCellClickHandler);
			this.enemyList.addEventListener(MenuEvent.CELL_DOUBLE_CLICK,onDoubleClickHandler);
			
			this.enemyList.width=134;
		}	
		
		/**
		 * 点击菜单 
		 * @param e
		 * 
		 */		
		protected function onCellClickHandler(e:MenuEvent):void{
			this.dispatchEvent(new MenuEvent(e.type,false,false,e.cell,e.roleInfo));
		}
		
		
		/**
		 * 双击人物处理事件 
		 * @param e
		 * 
		 */		
		protected function onDoubleClickHandler(e:MenuEvent):void{
			this.dispatchEvent(new MenuEvent(e.type,false,false,null,e.roleInfo));
		}
		
		/**
		 *  更新仇人列表
		 * @param enemyList
		 * 
		 */		
		public function update(enemyList:Array):void{
			this.sortOn(enemyList);
			this.enemyList.dataPro=enemyList;
			if(this.scrollPane && this.contains(this.scrollPane)) {
				this.removeChild(this.scrollPane);
				this.scrollPane = null;
			}
			
			this.scrollPane=new UIScrollPane(this.enemyList);
			this.scrollPane.width=156;
			this.scrollPane.height=278;
			this.scrollPane.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.scrollPane.refresh(); 
			this.addChild(scrollPane);
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
		
	}
}