package GameUI.Modules.Help.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Help.Data.DataEvent;
	import GameUI.Modules.Pk.Data.PkEvent;
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class HelpMediator extends Mediator
	{
		public static const NAME:String = "HelpMediator";
		
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase;
		private var listView:ListComponent		  = null;				//帮助列表
		private var iScrollPane:UIScrollPane	  = null;				//Scroll Bar
		private var perfectView:MovieClip;
		private var firstPoint:Point = new Point(20 , 50);				/** 首页坐标 */
		private var otherPoint:Point = new Point(20 , 0);				/** 其它页页坐标 */
		private var blrP:BulkLoaderResourceProvider;
		private var selectHelp:SimpleButton;
		private var relDic:Dictionary ;
		private var proDic:Dictionary ;
		private var txtInfo:TextField;
		private var index:int;											/** 储存数据编号 */
		private var txtWidth:int = 180;										/** 文本宽度 */
		private var hip:int;											/** mc的原始深度*/
		private var txtSprite:Sprite;									/** 内容文本容器 */
		private var txtContent:TaskText;								/** 内容文本 */ 
		private var pkindex:int = 25;									/** PK数据序列号 */
		private var meridiansIndex:int = 47;									/** PK数据序列号 */
		///////////////
		private var pkInfo:String;										/** 打开pk信息 */
		//////////////
		
		public function HelpMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.INITVIEW,
					EventList.SHOWHELPVIEW,
					DataEvent.OUTSHOWPK,
					EventList.CLOSEHELPVIEW,
					DataEvent.LOADCOMPLETE,
					PkEvent.ABOUTPK,
					EventList.SHOW_MERIDIANS_HELP
			];
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					perfectView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcHelpView");
					panelBase = new PanelBase(perfectView, perfectView.width + 10,perfectView.height+12);
					panelBase.name = "mcHelpView";
//					panelBase.x = UIConstData.DefaultPos2.x - 400;
//					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_hep_med_hel_han" ]);//"江湖指南"
					this.hip = perfectView.numChildren;
					perfectView.mouseEnabled = false;
					perfectView.txtInfo.mouseEnabled = false;
					perfectView.txtBack.mouseEnabled = false;
				break;
				case EventList.SHOWHELPVIEW:
					showView();
					addLis();
					this.isFirstPage = true;
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
				break;
				case DataEvent.OUTSHOWPK:
					showView(true);			//使用外部打开
					addLis();
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
				break;
				case EventList.CLOSEHELPVIEW:
					gcAll();
				break;
				case DataEvent.LOADCOMPLETE:
					var loadcompleteArr:Array = notification.getBody() as Array;
					this.relDic = loadcompleteArr[0];
					this.proDic = loadcompleteArr[1];
					if(this.pkInfo == "pk")				//如果指令为Pk 
					{
						pkInit();
					}
					else if(this.pkInfo == "meridians")
					{
//						showIndexPage(meridiansIndex);	
						showOnePage("type59");					
					}
					else
					{
						HelpInit();						//加载完成后才显示选项 , 进入首页
					}
				break;
				case PkEvent.ABOUTPK:				//进入PK相关页面
					pkInfo = "pk";
					showView(false);
					addLis();
				break;
				case EventList.SHOW_MERIDIANS_HELP:
					pkInfo = "meridians";
					showView(false);
					addLis();
			}
		}
		
		private function showView(outOpen:Boolean = false):void
		{
			//是否使用外部打开
			if(outOpen == false)
			{
				if(dataProxy.HelpViewOpen == false)
				{
					show();
				}
				else
				{
					if(this.pkInfo == "pk")				//如果指令为Pk 
					{
						scrollInit();						//////滚动条初始化
						show();
					}
				}
			}
			else
			{
				if(dataProxy.HelpViewOpen == false)
				{
					show();
				}
				else
				{
					gcAll();
				}
			}
		}
		/** 显示视图 */
		private function show():void
		{
			clearALL();
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos2.x - 400 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos2.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = UIConstData.DefaultPos2.x - 400;
				panelBase.y = UIConstData.DefaultPos2.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			dataProxy.HelpViewOpen = true;
			blrP = new BulkLoaderResourceProvider();
			if(relDic == null && this.proDic == null)
			{
				blrP.Download.Add(GameCommonData.GameInstance.Content.RootDirectory +  GameConfigData.Help);
				blrP.LoadComplete = onLoabdComplete;
				blrP.Load();
			}
			else
			{
				facade.sendNotification(DataEvent.LOADCOMPLETE ,[relDic , proDic]);
			}
		}
		
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			perfectView.btnFirst.addEventListener(MouseEvent.CLICK, firstHandler);
		}
		
		private function gcAll():void
		{
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			if(perfectView.btnBack.hasEventListener(MouseEvent.CLICK))  
			{
				perfectView.btnBack.removeEventListener(MouseEvent.CLICK, backHandler);
			}
			if(perfectView.btnBack.hasEventListener(MouseEvent.CLICK))  
			{
				perfectView.btnBack.removeEventListener(MouseEvent.CLICK, closeHandler);
			}
			perfectView.btnFirst.removeEventListener(MouseEvent.CLICK, firstHandler);
			index = 0;
			pkInfo = "";
			dataProxy.HelpViewOpen = false;
			clearALL();
			scrollInit();
		}
		//////滚动条初始化
		private function scrollInit():void
		{
			if(iScrollPane && panelBase.contains(iScrollPane)) {
				panelBase.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		
		/** 初始化界面 */
		private function HelpInit():void
		{	
			clearALL();
			showOnePage("");
		}
		/** 设置帮助选择列表 */
		private function initHelpChoiceView():void
		{
			if(iScrollPane != null && panelBase.contains(iScrollPane))
			{
				panelBase.removeChild(iScrollPane);
			}
			if(this.txtSprite==null){
				
				iScrollPane = new UIScrollPane(listView);
			}else{
				iScrollPane = new UIScrollPane(this.txtSprite);
				this.txtSprite = null;
			}
			
			iScrollPane.x = 10;
			iScrollPane.y = 30;
			iScrollPane.width = perfectView.width;
			iScrollPane.height = 310;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_ALWAYS;
			iScrollPane.refresh();
			panelBase.addChild(iScrollPane);
		}
		/** 点击选项条 */
		private function clickHandler(e:MouseEvent):void
		{
			clearALL();
			index = e.currentTarget.name.split("_")[1];
			//无子集
			for(var key1:* in this.proDic)
			{
				if(proDic[key1].type == index)
				{
					/** 选项为:查看可接任务 */
					if(proDic[key1].content == "OpenTask")
					{
						var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
						if(dataProxy.TaskIsOpen == false)  facade.sendNotification(EventList.SHOWTASKVIEW,1);
						else facade.sendNotification(EventList.CLOSETASKVIEW);
						return;	
					}
					txtContent = new TaskText()//txtWidth + 20);
					txtSprite = new Sprite();
					/** 选项为:我现在去哪打怪 */
					if(proDic[key1].type == 3)
					{
						if(GameCommonData.Player.Role.Level < 11) txtContent.tfText = proDic[key1].content.split("_")[0];
						else if(GameCommonData.Player.Role.Level > 10 && GameCommonData.Player.Role.Level < 16) txtContent.tfText = (proDic[key1].content as String).split("_")[1];
						else if(GameCommonData.Player.Role.Level > 15 && GameCommonData.Player.Role.Level < 21) txtContent.tfText = proDic[key1].content.split("_")[2];
						else if(GameCommonData.Player.Role.Level > 20 && GameCommonData.Player.Role.Level < 26) txtContent.tfText = proDic[key1].content.split("_")[3];
						else if(GameCommonData.Player.Role.Level > 25 && GameCommonData.Player.Role.Level < 31) txtContent.tfText = proDic[key1].content.split("_")[4];
						else if(GameCommonData.Player.Role.Level > 30 && GameCommonData.Player.Role.Level < 36) txtContent.tfText = proDic[key1].content.split("_")[5];
						else if(GameCommonData.Player.Role.Level > 35 && GameCommonData.Player.Role.Level < 41) txtContent.tfText = proDic[key1].content.split("_")[6];
						else if(GameCommonData.Player.Role.Level > 40 && GameCommonData.Player.Role.Level < 46) txtContent.tfText = proDic[key1].content.split("_")[7];
						else if(GameCommonData.Player.Role.Level > 45 && GameCommonData.Player.Role.Level < 51) txtContent.tfText = proDic[key1].content.split("_")[8];
						else if(GameCommonData.Player.Role.Level > 50 && GameCommonData.Player.Role.Level < 56) txtContent.tfText = proDic[key1].content.split("_")[9];
						else if(GameCommonData.Player.Role.Level > 55 && GameCommonData.Player.Role.Level < 61) txtContent.tfText = proDic[key1].content.split("_")[10];
						else if(GameCommonData.Player.Role.Level > 60 && GameCommonData.Player.Role.Level < 66) txtContent.tfText = proDic[key1].content.split("_")[11];
						else if(GameCommonData.Player.Role.Level > 65 && GameCommonData.Player.Role.Level < 71) txtContent.tfText = proDic[key1].content.split("_")[12];
						else if(GameCommonData.Player.Role.Level > 70 && GameCommonData.Player.Role.Level < 76) txtContent.tfText = proDic[key1].content.split("_")[13];
						else if(GameCommonData.Player.Role.Level > 75 && GameCommonData.Player.Role.Level < 81) txtContent.tfText = proDic[key1].content.split("_")[14];
						else if(GameCommonData.Player.Role.Level > 80 && GameCommonData.Player.Role.Level < 86) txtContent.tfText = proDic[key1].content.split("_")[15];
						else if(GameCommonData.Player.Role.Level > 85 && GameCommonData.Player.Role.Level < 91) txtContent.tfText = proDic[key1].content.split("_")[16];
					}
					else{
						txtContent.tfText = proDic[key1].content;
					} 
					//无子集加下拉框
					btnChange();						//变换关闭，返回按钮
					txtSprite.addChild(txtContent);
					txtContent.x = otherPoint.x - 16;
					/** 每条数据都加了两个换行<br> */
					txtContent.y = otherPoint.y - 20//+ 15;
					initHelpChoiceView();			//加上滚动条
					return;
				}
			}
			showOnePage(String("type" + index));
			//访问关系集合的对象键
		}
		
		/** 点击返回按钮到上级菜单*/
		private function backHandler(e:MouseEvent):void
		{
			//如果为首页
			if(index == 0)
			{
				clearALL();
				HelpInit();
			}
			else
			{
				clearALL();
				showOnePage(relDic[index].fartherType as String);
				var fartherType:int = relDic[index].fartherType.split("e")[1];
				index = relDic[fartherType].index;
				if(index <= 0) index = 0;
			}
		} 
		
		/** 点击关闭面板 */
		private function closeHandler(e:MouseEvent):void
		{
			gcAll();
		}
		
		/** 点击首页按钮 */
		private function firstHandler(e:MouseEvent):void
		{
			clearALL();
			HelpInit();
		}
		
		/** 清除所有的选项条 */
		private function clearALL():void
		{
			for(var i:int = perfectView.numChildren ; i > this.hip - 1; i--)
			{
				perfectView.removeChildAt(perfectView.numChildren - 1);
			}
		}
		
		private function onLoabdComplete():void
		{
			if(this.blrP.GetResource(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.Help) == null ) return;
			var relDic:Dictionary		= this.blrP.GetResource(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.Help).GetDisplayObject()["relDic"];	 			// 江湖指南关系数据
			var proDic:Dictionary		= this.blrP.GetResource(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.Help).GetDisplayObject()["proDic"];	 			// 江湖指南数据
			facade.sendNotification(DataEvent.LOADCOMPLETE ,[relDic , proDic]);
		}
		
		private function showOnePage(fartherType:String ):void
		{
			
			scrollInit();						//////滚动条初始化
			listView = new ListComponent(false);
			listView.Offset = 0;
			//////////////
			var point:Point;
			if(fartherType == "")  
			{
				this.isFirstPage = true;										//监听首页事件
				perfectView.txtInfo.text = GameCommonData.wordDic[ "mod_hep_med_hel_sho_1" ];//"记得每到一个新的级别就打开我来看看，我会告诉你很多江湖当中的事情";		//首页的文本
				perfectView.txtBack.text = GameCommonData.wordDic[ "mod_hep_med_hel_sho_2" ];//"关  闭";
				perfectView.txtBack.mouseEnabled = false;
				listView.addChild(perfectView.txtInfo);
				point = this.firstPoint;
			}	
			else 
			{
				btnChange();
				point = this.otherPoint;
			}
			var i:int = 0;			//序号
			var y:int = point.y;
			for(var key:* in this.relDic)
			{
				i++;
				selectHelp = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btnSelectHelp");
				var currentsprite:Sprite = new Sprite();
				currentsprite.addChild(selectHelp);
				currentsprite.name = "btn_"+ String(i - 1);
				txtInfo = new TextField();
				var str:String = String("type" + index);
				//有子集
				if(relDic[key].fartherType == fartherType)
				{
					txtInfo.maxChars = 100;
					txtInfo.width = txtWidth;
					txtInfo.height = 35;
					txtInfo.text = relDic[key].title;
					txtInfo.mouseEnabled = false;
					txtInfo.textColor = 0xFFFFFF;
					currentsprite.addEventListener(MouseEvent.CLICK , clickHandler);
					listView.addChild(currentsprite)
					listView.addChild(txtInfo);
					txtInfo.x = point.x + 20;
					txtInfo.y = y + 20;
					currentsprite.x = point.x;
					currentsprite.y = y + 20;
					y += 20;
				}
			}
			initHelpChoiceView();			//加上滚动条
		}
		
		
		/** 显示pk相关页面 */
		private function pkInit():void
		{
			txtSprite = new Sprite();
			this.isFirstPage = false;
			perfectView.txtBack.text = GameCommonData.wordDic[ "mod_hep_med_hel_pkI" ];//"返 回";
			perfectView.txtBack.mouseEnabled = false;
			txtContent = new TaskText();
			perfectView.addChild(txtContent);
			txtContent.tfText = proDic[this.pkindex].content;
			txtContent.mouseEnabled = false;
			txtSprite.addChild(txtContent);
			txtContent.x = otherPoint.x - 16;
			/** 每条数据都加了两个换行<br> */
			txtContent.y = otherPoint.y - 20//+ 15;
			initHelpChoiceView();			//加上滚动条
		}
		
		/** 显示 index相对应的页面 抄袭pkInit()方法 */
		private function showIndexPage(index:int):void
		{
			txtSprite = new Sprite();
			this.isFirstPage = false;
			perfectView.txtBack.text = GameCommonData.wordDic[ "mod_hep_med_hel_pkI" ];//"返 回";
			perfectView.txtBack.mouseEnabled = false;
			txtContent = new TaskText();
			perfectView.addChild(txtContent);
			txtContent.tfText = proDic[index].content;
			txtContent.mouseEnabled = false;
			txtSprite.addChild(txtContent);
			txtContent.x = otherPoint.x - 16;
			/** 每条数据都加了两个换行<br> */
			txtContent.y = otherPoint.y - 20//+ 15;
			initHelpChoiceView();			//加上滚动条
		}
		
		/** 是否是首页*/
		public  function set isFirstPage(i:Boolean):void
		{
			if(i == true)
			{
				if(perfectView.btnBack.hasEventListener(MouseEvent.CLICK))  
				{
					perfectView.btnBack.removeEventListener(MouseEvent.CLICK, backHandler);
				}
				perfectView.btnBack.addEventListener(MouseEvent.CLICK, closeHandler);
			}
			else
			{
				if(perfectView.btnBack.hasEventListener(MouseEvent.CLICK))  
				{
					perfectView.btnBack.removeEventListener(MouseEvent.CLICK, closeHandler);
				}
				perfectView.btnBack.addEventListener(MouseEvent.CLICK, backHandler);
			}
		}
		/** 监听非第一页的页面 */
		private function btnChange():void
		{
			this.isFirstPage = false;										//监听其它页事件
			perfectView.txtBack.text = "返  回";
			perfectView.txtBack.mouseEnabled = false;
		}
	}
}