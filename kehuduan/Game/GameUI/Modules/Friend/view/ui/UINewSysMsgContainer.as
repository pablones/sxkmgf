package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.SystemMessage.Data.SysMessageData;
	import GameUI.Modules.SystemMessage.Memento.MessageMemento;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	/**
	 * 
	 * @author felix
	 * 
	 */
	public class UINewSysMsgContainer extends Sprite
	{
		/** 渲染器数组  */
		protected var cells:Array = new Array();
		/** 渲染器缓冲  */		
		protected var cacheCells:Array = new Array();
		/** 好友容器 */		
		protected var friendCon:UIScrollPane;
		/**  各组好友的数据提供者（【组】【name,name....】） */
		protected var _fListDataPro:Array;
		/**  好友列表组件  */
		public var fList:UIFriendsList;

		/** 选中索引 */
		protected var selectedIndex:uint=0;
		
		private var sysCell:MovieClip;
		
		private var friendView:MovieClip;
		private static var onePageNum:int = 14;
		private var currentPage:int = 0;
		private var pageNum:int = 0;
		private var roleCount:int = 0;		
		private var roleNum:int;
		private var msgList:Array = new Array();
		private var friendSwf:MovieClip;
		/** 是否已经单击*/
		protected var countClick:uint=0;		
		/** 菜单*/
		public var menu:MenuItem;		
		/** 当前位置 */
		protected var currendPos:Point;
		protected var isShow:Boolean=false;

		public static var clorWidth:int  = 336;
		public static var clorHeight:int = 23;

		public var selectFunction:Function; 
		public var openFunction:Function 
		
		public var w:Number=148;
		public var h:Number=279;
		
		
		public function UINewSysMsgContainer(friendView:MovieClip)
		{
			super();  
			//this.createChildren();
			this.friendView = friendView;
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf", onPanelLoadComplete);
		}
		
		private function onPanelLoadComplete():void
		{
			friendSwf = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf");			
			this.sysCell = new (friendSwf.loaderInfo.applicationDomain.getDefinition("sysCell"));
		}
		
		/**
		 * 
		 * @param value
		 * 
		 */		
		public function set fListDataPro(value:Array):void{
			//this.sortOn(value);
			this._fListDataPro=value;
			//this.setRoleList(this._fListDataPro);
			this.setPage();
			this.toRepaint(this.currentPage);	
		}
		
		
		public function get fListDataPro():Array{
			return this._fListDataPro;
		}
		
		public function setPage():void
		{
			var num:int = int((SysMessageData.messageList.length - 1)/onePageNum) + 1;
			var currentNum:int = this.currentPage + 1;
			if(currentNum == 1)
			{
				(this.friendView.btn_left as SimpleButton).mouseEnabled = false;
			}else{
				(this.friendView.btn_left as SimpleButton).mouseEnabled = true;
			}
			if(currentNum == num)
			{
				(this.friendView.btn_right as SimpleButton).mouseEnabled = false;
			}else{
				(this.friendView.btn_right as SimpleButton).mouseEnabled = true;
			}
			if(currentNum > num){
				num = currentNum;
			}			
			this.friendView.txt_pagenum.text = "第" + currentNum.toString() + "/" + num.toString() + "页";
		}
		
		
		/**
		 * 创建并初始化UI 
		 * 
		 */		
		public function createChildren(data:MessageMemento):MovieClip
		{			
			var syscell:MovieClip = this.getCell();
			this.addChild(syscell);
			syscell.addEventListener(MouseEvent.CLICK, clickHandler);
			syscell.btn_del.addEventListener(MouseEvent.CLICK, onDelHandler);
			//syscell.btn_del.name = "btn_del" + data.index;
			syscell.name = "MessageItem_" + data.index;
			syscell["sysMessage"] = data;
			(syscell.hight as TextField).mouseEnabled = false;
			(syscell.txt_time as TextField).autoSize = TextFieldAutoSize.CENTER;
			syscell.txt_time.htmlText = data.timeStr;
			syscell.txt_state.htmlText = data.state;
			var index:int = (SysMessageData.messageList.length > SysMessageData.messageTotalNum ? SysMessageData.messageTotalNum:SysMessageData.messageList.length);			
			syscell.txt_num.htmlText = data.index + "/" + SysMessageData.messageTotalNum;
			syscell.x = 10;
			syscell.y = 85 + (syscell.height)*roleCount;	
			syscell.txt_time.mouseEnabled = false;
			syscell.txt_state.mouseEnabled = false;
			syscell.txt_num.mouseEnabled = false;			
			return syscell;
		}

		private function onDelHandler(e:MouseEvent):void
		{
			var name:String = (e.target as DisplayObject).parent.name;
			delOneCell(name);
			this.fListDataPro = SysMessageData.messageList;
		}

		private function delOneCell(name:String):void
		{
			var index:int = uint(name.split("_")[1]);
			if(name != null){
				for(var i:int = 0;i < SysMessageData.messageList.length ; i++){
					if(SysMessageData.messageList[i].index == index){
						SysMessageData.messageList.splice(i , 1);
					}
				}
			}
		}

		private function clickHandler(e:MouseEvent):void
		{
			countClick++;
			var id:int;
			if (countClick == 1) {
				id = setTimeout(selected , 200,e.currentTarget);
				e.stopPropagation();
			} 
			else if (countClick == 2)
			{
				SysMessageData.selectItemIndex = e.currentTarget["sysMessage"].index;		//选中的消息序列
				countClick = 0;
				clearTimeout(id);
				e.stopPropagation();
				//if(openFunction != null) openFunction();
				(GameCommonData.UIFacadeIntance.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator).openMegItem();
			}
		}
		
		/** 选中 */
		public var shape:Shape = new Shape();
		private function selected():void
		{
			this.countClick = 0;	
			this.addChild(shape);	
			shape.x = arguments[0].x;
			shape.y = arguments[0].y;
			shape.graphics.clear();
			shape.graphics.lineStyle(1,0xfffe65);
			shape.graphics.drawRect(0,0,clorWidth,clorHeight);
			shape.graphics.endFill();
			
			(GameCommonData.UIFacadeIntance.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator).clickMegItem(arguments[0]["sysMessage"]);
		}

		/**
		 * 移除所有的渲染器 
		 * 
		 */		
		protected function removeAllCells():void{
			for each(var cell:MovieClip in this.cells){
				if(this.contains(cell)){
					if(this.contains(this.shape))
					{
						this.removeChild(this.shape);
					}					
					this.removeChild(cell);
					this.roleCount = 0;
					//this.cacheCells.push(cell);
				}	
			}
			this.cells=[];				
		}
						
		/**
		 * 重绘 
		 * 
		 */		
		protected function toRepaint(page:int):void{
			this.removeAllCells();
			SysMessageData.selectItemIndex = 0;
			for(var i:int = this.currentPage * onePageNum;i < this._fListDataPro.length; i++){
				if(i < (this.currentPage +1)*onePageNum){
					SysMessageData.messageList[i].index = i+1;
					this._fListDataPro[i].index = i+1;
					this.createChildren(this._fListDataPro[i] as MessageMemento);
					this.roleCount += 1;
				}
			}
		}
		
		
		public function pageDown():void
		{
			if(this.currentPage < (this._fListDataPro.length/onePageNum -1))
			{
				this.currentPage += 1;
				this.setPage();
				this.toRepaint(this.currentPage);
			}
		}
		
		public function pageUp():void
		{
			if(this.currentPage > 0)
			{
				this.currentPage -= 1;
				this.setPage();
				this.toRepaint(this.currentPage);
			}
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
		 * 获取渲染器
		 * @return 渲染器 
		 * 
		 */		
		protected function getCell():*{
			//var cell:*=this.cacheCells.shift();
			var cell:*;
			if(cell==null){
				cell=new (friendSwf.loaderInfo.applicationDomain.getDefinition("sysCell"));
				//this.addChild(cell);
			}
			this.cells.push(cell);
			return cell;
		}
		
			
	}
}