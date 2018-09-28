package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;

	public class MenuItem extends Sprite
	{
			
		/** 数据提供者 */
		protected var _dataPro:Array;
		/** 子菜单*/
		protected var childrenMenu:MenuItem;
		/** 内容面板 */
		public var contentPanel:MovieClip;
		
		/** 渲染器数组 */
		protected var cells:Array;
		
		
		public var roleInfo:PlayerInfoStruct;
		
		public var w:Number=100;
		
		public function MenuItem(data:Array=null)
		{
			super();
			this.name="MENU";
			this._dataPro=data;
			this.contentPanel=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ListBack");
			this.contentPanel.width=w;
			this.addChild(this.contentPanel);
			this.createChildren();
		}
		
		/**
		 * 初始化UI 
		 * 
		 */		 
		protected  function createChildren():void{
			this.cells=[];
			for each(var cellData:Object in this.dataPro){
				var cell:MenuItemCell=this.getCell();
				cell.setText(cellData["cellText"]);
				if(cellData.hasOwnProperty("data")){
					cell.data=cellData["data"];
				}
				
				this.addChild(cell);
				this.cells.push(cell);
			}
			this.doLayout();
		}
		
		/**
		 * 获取菜单选项渲染器
		 * @return 
		 * 
		 */		
		protected function getCell():MenuItemCell{
			var cell:MenuItemCell=new MenuItemCell();
			cell.addEventListener(MouseEvent.CLICK,onCellMouseClick);
			cell.addEventListener(MouseEvent.ROLL_OUT,onCellMouseRollOut);
			cell.addEventListener(MouseEvent.ROLL_OVER,onCellMouseRollOver);
			return cell;
		}
		
		
		/**
		 * 鼠标点击选项 
		 * @param e
		 * 
		 */		
		protected function onCellMouseClick(e:MouseEvent):void{
			this.dispatchEvent(new MenuEvent(MenuEvent.Cell_Click,false,false,e.currentTarget as MenuItemCell,this.roleInfo));
		}
		
		/**
		 * 鼠标滚出渲染器 
		 * @param e
		 * 
		 */		
		protected function onCellMouseRollOut(e:MouseEvent):void{
			var cell:MenuItemCell=e.target as MenuItemCell;
			var data:Object=cell.data;
			if(this.childrenMenu==null)return;
			if(!data["hasChild"] ){
				if(this.childrenMenu.parent!=null){
					this.removeChild(this.childrenMenu);
				}
			}		
		}
		
		/**
		 * 鼠标滚入渲染器 
		 * @param e
		 * 
		 */		
		protected function onCellMouseRollOver(e:MouseEvent):void{
			var cell:MenuItemCell=e.target as MenuItemCell;
			var data:Object=cell.data;
			
			if(data["hasChild"]){
				if(this.childrenMenu!=null && this.contains(this.childrenMenu)){
					this.removeChild(this.childrenMenu);
				}
				this.childrenMenu=new MenuItem(data["data"]);
				this.childrenMenu.addEventListener(MenuEvent.Cell_Click,onChildrenMenuClick);
				this.addChild(this.childrenMenu);
				
				
				var localPoint:Point=new Point(cell.x+cell.width+2+this.childrenMenu.width,cell.y);
				var globalPoint:Point=this.localToGlobal(localPoint);
				if(globalPoint.x>this.stage.stageWidth){
					this.childrenMenu.x=cell.x-this.childrenMenu.width-2;
				}else{
					this.childrenMenu.x=cell.x+cell.width+2;
				}
				this.childrenMenu.y=cell.y;		
				
				
			}	
		}
			
		protected function toRepaint():void{
			this.createChildren();
		}
		
		/**
		 * 点击子菜单选项处理 
		 * @param e
		 * 
		 */		
		protected function onChildrenMenuClick(e:MenuEvent):void{
			this.dispatchEvent(new MenuEvent(e.type,false,false,e.cell));
		}
			
		
		/**
		 * 布局 
		 * 
		 */		
		protected function doLayout():void{
			var currentY:Number=1;
			for each(var cell:MenuItemCell in this.cells){
				cell.content.width=w-4;
				cell.y=currentY;
				cell.x=2;
				currentY+=cell.height;
			}
			this.contentPanel.height=currentY+3;
		}
		
		
		public function set dataPro(value:Array):void{
			removeAll();
			this._dataPro=value;
			this.toRepaint();
		}
		
		protected function removeAll():void{
			for each(var cell:MenuItemCell in this.cells){
				if(this.contains(cell)){
					this.removeChild(cell);
				}
				
			}
		}
		
		
		public function get dataPro():Array{
			return this._dataPro;
		}
	
	}
}