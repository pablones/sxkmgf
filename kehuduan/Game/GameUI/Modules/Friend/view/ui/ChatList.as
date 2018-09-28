package GameUI.Modules.Friend.view.ui
{
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	
	import flash.display.DisplayObject;

	public class ChatList extends UISprite
	{
		/**
		 * 滚动面板 
		 */		
		protected var scrollPane:UIScrollPane;
		/**
		 * 列表容器 
		 */		
		protected var container:UISprite;
		/**
		 * 列表数据提供者 
		 */		
		protected var _dataPro:Array;
		/**
		 * 渲染集合 
		 */		
		protected var cells:Array;
		/**
		 * 渲染集合缓存 
		 */		
		protected var cacheArray:Array;
		/**
		 * 宽 
		 */		
		protected var  _w:uint;
		/**
		 *  高 
		 */		
		protected var _h:uint;
		
		
		public function ChatList(w:uint,h:uint)
		{
			super();
			this._w=w;
			this._h=h;
			this.createChildren();
		}
		
		/**
		 * 创建子元素 
		 * 
		 */		
		protected function createChildren():void{
			this.cacheArray=[];
			this.cells=[];
			this._dataPro=[];
			
			this.container=new UISprite();
			this.container.width=this._w-16;
			this.scrollPane=new UIScrollPane(this.container);
			this.addChild(this.scrollPane);
			this.scrollPane.width=this._w;
			this.scrollPane.height=this._h;
			this.scrollPane.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.scrollPane.refresh();
			
				
		}
		
		/**
		 * 将列表滚动到最低端 
		 * 
		 */		
		public function scrollBottom():void{
			this.scrollPane.refresh();
			this.scrollPane.scrollBottom();
		}
		
		/**
		 * 重绘
		 *  
		 * 
		 */		
		protected function toRepaint():void{
			this.removeAllCells();
			this.createCells();
			this.doLayout();
		}
		
		/**
		 * 布局 
		 * 
		 */		
		protected function doLayout():void{
			var len:uint=this.cells.length;
			var posY:Number=1;
			for(var i:uint=0;i<len;i++){
				var cell:DisplayObject=this.cells[i];
				cell.x=1;
				cell.y=posY;
				posY+=cell.height;
			}
			this.container.height=posY;
			this.scrollPane.refresh();
		}
		
		/**
		 * 创建渲染 
		 * 
		 */		
		public function createCells():void{
			var len:uint=this._dataPro.length;
			if(len==0)return;
			for(var i:uint=0;i<len;i++){
				var obj:Object=this._dataPro[i];
				var cell:FriendChatCell=this.getCell();
				this.container.addChild(cell);
				if(obj is String){
					cell.des=String(obj);
				}else{
					var str:String=obj.sendPersonName;
					var temp:Array=str.split("_");
					var data:Date=new Date(obj.sendTime);
					cell.des='<font color="#00ffff">'+temp[0]+'</font>&nbsp;&nbsp;&nbsp;<font color="#ff00ff">'+data.toLocaleTimeString()+'</font><br>&nbsp;&nbsp;<font color="#'+obj.style+'">'+obj.msg+'</font>';
				}
				cells.push(cell);	
			}
		}
		
		/**
		 * 获得渲染器 
		 * @return 
		 * 
		 */		
		public function getCell():FriendChatCell{
			var cell:FriendChatCell=this.cacheArray.shift();
			if(cell==null)cell=new FriendChatCell(this._w-16);
			return cell;	
		}
		
		/**
		 * 清除所有的渲染器 
		 * 
		 */		 
		public function removeAllCells():void{
			var len:uint=this.cells.length;
			for(var i:uint=0;i<len;i++){
				var cell:DisplayObject=this.cells.shift();
				if(this.container.contains(cell)){
					this.container.removeChild(cell);
				}
				this.cacheArray.push(cell);
			}
		}
		
		/**
		 * 设置数据提供者 
		 * @param data
		 * 
		 */		
		public function set dataPro(data:Array):void{
			this._dataPro=data;
			this.toRepaint();	
		}
		
		/**
		 * 再有的聊天记录上追加一天聊天记录 
		 * @param des
		 * 
		 */		
		public function addChatCell(obj:*):void{
			
			this._dataPro.push(obj);
			var cell:FriendChatCell=this.getCell();
			this.container.addChild(cell);
			if(obj is String){
				cell.des=String(obj);
			}else{
				var str:String=obj.sendPersonName;
				var temp:Array=str.split("_");
				var data:Date=new Date(obj.sendTime);
				cell.des='<font color="#00ffff">'+temp[0]+'</font>&nbsp;&nbsp;&nbsp;<font color="#ff00ff">'+data.toLocaleTimeString()+'</font><br>&nbsp;&nbsp;<font color="#'+obj.style+'">'+obj.msg+'</font>';
			}
			cells.push(cell);	
			this.doLayout();
			this.scrollPane.scrollBottom();
		}
	}
}