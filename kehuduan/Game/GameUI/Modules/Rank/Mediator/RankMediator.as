package GameUI.Modules.Rank.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Rank.Data.RankConstData;
	import GameUI.Modules.Rank.Data.RankEvent;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.Zippo;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class RankMediator extends Mediator
	{
		public static const NAME:String="RankMediator";
		private static var rankIndex:int;			                                //当前按钮的序号
		
		private var panelBase:PanelBase = null;
		
		private var dataProxy:DataProxy;
		private var ranking:MovieClip;
		private var totalPage  :int = RankConstData.totalPage;                      //总页数 
		private var currentPage:int = 1;					//当前页数					
		private var dataArr:Array;                                                 //接受数据的数组
		private var isSendOver:Boolean = true;										//是否数据接收成功
		
		public function RankMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
			    EventList.SHOWRANKVIEW,       //打开排行榜界面
			    EventList.CLOSERANKVIEW,      //关闭排行榜界面
			    RankEvent.UPDATERANKDATA,  	  //更新数据
			    RankEvent.MYRANK              //我的排行
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					RankConstData.rankAllListInit();
				break;
				
				case EventList.SHOWRANKVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
				  	if(dataProxy.RankIsOpen) {
				  		gcAll();
				  	 	return;
				  	}
//				  	currentPage = 1;                //初始页码为1
			   	 	totalPage   = 10;               //总页码为20
				  	initView();					//初始化视图
				 	ranking.txtPageInfo.text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_1" ] + currentPage + "/" + RankConstData.totalPage + GameCommonData.wordDic[ "mod_rank_med_rm_hn_2" ];   //"第"   "页"            //初始化页码显示
				  	addLis();						//添加事件
				  	dataProxy.RankIsOpen = true;	//排行榜已打开
				  	initRank(rankIndex);					//初始化排行界面,请求数据
				  	sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
				break;
				  
				case EventList.CLOSERANKVIEW:
				    if(dataProxy.RankIsOpen)
				    {
				  	  gcAll();//清理内存
				    }
				    break;
				  
				case RankEvent.UPDATERANKDATA:
				  isSendOver = true;					//传输成功	
				  if(ranking == null)
				  {
				  	RankConstData.rankAllList[rankIndex][currentPage] =  notification.getBody() as Array;
				  	var obj:Object = RankConstData.rankAllList[rankIndex];
				  	if(obj[1] != null) obj[1] = null;	
			        return;
				  }
				  dataArr = [];                         //初始化
				  cleartxt();
				  dataArr = notification.getBody() as Array;
				  updateData(dataArr);//得到数据，排行位置
				  pageBtnSee(true);
				  ranking.txtPageInfo.text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_1" ] + currentPage + "/" + RankConstData.totalPage + GameCommonData.wordDic[ "mod_rank_med_rm_hn_2" ];    //"第"  "页"               //初始化页码显示
				break;
				
				case  RankEvent.MYRANK:
				   if(ranking == null)
				   {
				  	if(RankConstData.rankAllList[rankIndex].myRank != null) RankConstData.rankAllList[rankIndex].myRank = null;;
			        return;
				   }
				   var myrank:int = notification.getBody() as int;
				   if(myrank == 9999)
				   {
			   		 ranking.txtmyrank.text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ];    //"无"
				   }
				   else
				   {
				   	 if(int(RankConstData.rankAllList[rankIndex].myRank) > 999)  ranking.txtmyrank.text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ];    //"无"
					 else  ranking.txtmyrank.text = int(myrank) + 1;
				   }
				   RankConstData.rankAllList[rankIndex].myRank = ranking.txtmyrank.text;				//缓存
				  
				break;
			}
		}
		
		/** 初始化第type个按钮所对应的排行界面，并向服务器请求数据 */
		private function initRank(type:int):void
		{
			 btnvisible(type);                                     //选中效果
		     setLab(type);                                         //设置Lab位置
			 rankIndex = type;                                     //序列号为1
			 cleartxt();
			 sendInfo();											//数据传送成功才会发送请求
		}
		
		/** 垃圾回收 */
		private function gcAll():void
		{
			removeLis();
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) GameCommonData.GameInstance.GameUI.removeChild(panelBase);
//			rankIndex   = 0;
//			currentPage = 0;
			totalPage   = 0;
//			currentPage = 0;
			dataProxy.RankIsOpen = false;
			ranking   = null;
			dataProxy = null;
			dataArr = null;                            //数据数组为空
		}
		
		/** 初始化视图 */
		private function initView():void
		{
			ranking = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Ranking");
			ranking.mouseEnabled = false;
			ranking.txtLab_0.mouseEnabled = false;
			ranking.txtmyrank.x -= 10; 
			(ranking.txtmyrank as TextField).width = 40; 
			panelBase = new PanelBase(ranking, ranking.width+7, ranking.height+11);                    //创建一个外壳，将ranking添加到外壳里
			panelBase.name = "rankPanel";
			panelBase.addEventListener(Event.CLOSE, rankCloseHandler);
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase);                                   //添加到游戏画面中
			isbtnenable();                                                                            //初始化前后按钮
			for(var j:int = 0; j < 20; j++) 
			{
				(ranking["txtData_1_"+j] as TextField).autoSize = TextFieldAutoSize.CENTER;
			}
			if(ranking != null)
			{
				ranking.mouseEnabled = false;
			}
		}
		
		/** 设置Lab的位置 */
		private function setLab(i:int):void
		{
			ranking.mcLab_1.x 		= RankConstData.LAB_POS[i][0];
			ranking.mcLab_2.x 		= RankConstData.LAB_POS[i][1];
	
			ranking.txtLab_1.x		= RankConstData.LAB_POS[i][2];
		    ranking.txtLab_2.x 		= RankConstData.LAB_POS[i][3];
			ranking.txtLab_3.x 		= RankConstData.LAB_POS[i][4];
			ranking.txtLab_1.text   = RankConstData.LAB_POS[i][5];
			ranking.txtLab_2.text   = RankConstData.LAB_POS[i][6];
			ranking.txtLab_3.text   = RankConstData.LAB_POS[i][7];
			panelBase.SetTitleTxt(RankConstData.LAB_POS[i][8]);	
		}
		
		/** 添加事件监听 */
		private function addLis():void
		{
			for(var i:int = 0; i < 13; i++) {
				ranking["btnList_"+i].addEventListener(MouseEvent.CLICK,btnHandler);
			}
			for(var j:int = 0; j < 20; j++)
			{
				ranking["txtData_1_"+j].addEventListener(MouseEvent.ROLL_OVER, onMouseRoll);
				ranking["txtData_1_"+j].addEventListener(MouseEvent.ROLL_OUT, onMouseOut);
				ranking["txtData_1_"+j].addEventListener(TextEvent.LINK, selectName);
			}
			ranking.btnFrontPage.addEventListener(MouseEvent.CLICK, frontPageHandler);
			ranking.btnBackPage.addEventListener(MouseEvent.CLICK, backPageHandler);
		}
		
		/** 移除事件监听 */
		private function removeLis():void
		{
			if(ranking) {
				for(var i:int = 0; i < 13; i++) {
					ranking["btnList_"+i].removeEventListener(MouseEvent.CLICK,btnHandler);
				}
				for(var j:int = 0; j < 20; j++){
					ranking["txtData_1_"+j].removeEventListener(MouseEvent.ROLL_OVER, onMouseRoll);
					ranking["txtData_1_"+j].removeEventListener(MouseEvent.ROLL_OUT, onMouseOut);
					ranking["txtData_1_"+j].removeEventListener(TextEvent.LINK, selectName);
				}
				ranking.btnFrontPage.removeEventListener(MouseEvent.CLICK, frontPageHandler);
				ranking.btnBackPage.removeEventListener(MouseEvent.CLICK, backPageHandler);
			}
		}
		
		/** 点击名字 弹出组件 */
		private function selectName(e:TextEvent):void
		{
			var text:String = e.text.split("_")[0];
			if(text) {
				switch(text)
				{
					case "name":
						if(ChatData.QuickChatIsOpen) return;
						var playerName:String  = e.text.split("_")[1];
						if(playerName != GameCommonData.Player.Role.Name) {
						 	var quickSelectMediator:QuickSelectMediator = new QuickSelectMediator();
						 	facade.registerMediator(quickSelectMediator);
							facade.sendNotification(ChatEvents.SHOWQUICKOPERATOR, playerName);
						}
					break;
				}
			}
		}
		
		/** 鼠标移到(名字)文字上 使用系统鼠标形状 */
		private function onMouseRoll(e:MouseEvent):void
		{
			if(RankMediator.rankIndex > 9 && RankMediator.rankIndex < 12)
			 return;
			SysCursor.GetInstance().showSystemCursor();
		}
		
		/** 鼠标移出 恢复默认鼠标形状 */
		private function onMouseOut(e:MouseEvent):void
		{
			if(RankMediator.rankIndex > 9 && RankMediator.rankIndex < 12)
			 return;
			SysCursor.GetInstance().revert();
		}
		
		/** 点击某个排行榜按钮 */
		private function btnHandler(e:MouseEvent):void
		{
			if(isSendOver == false)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rank_med_rm_bh" ], color:0xffff00});    //"数据传输中"
				return;
			}
			var i:int = e.target.name.split("_")[1];
			currentPage = 1;
			ranking.txtPageInfo.text=GameCommonData.wordDic[ "mod_rank_med_rm_hn_1" ]+currentPage+"/"+totalPage+GameCommonData.wordDic[ "mod_rank_med_rm_hn_2" ];   //"第"  "页"
			initRank(i);
			isbtnenable();                                                                           		 //初始化前后按钮
		}
		
		/** 点击上一页 */
		private function frontPageHandler(e:MouseEvent):void
		{
			if(currentPage > 1)                	 															//如果currentpage>1得话向服务器请求前一页数据
		 	{
		 		if(isSendOver == false)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rank_med_rm_bh" ], color:0xffff00});  //"数据传输中"
					return;
				}
		 		currentPage--;
		 		ranking.txtPageInfo.text=GameCommonData.wordDic[ "mod_rank_med_rm_hn_1" ]+currentPage+"/"+totalPage+GameCommonData.wordDic[ "mod_rank_med_rm_hn_2" ];   //"第"  "页"
		 		cleartxt();
		 		sendInfo();
		  	}
//	  		isbtnenable();                                                                           		 //初始化前后按钮
		}
		
		/** 点击下一页 */
		private function backPageHandler(e:MouseEvent):void
		{
			if(currentPage < totalPage)         //如果cruuentpage<totalPage得话向服务器请求后一页得数据
		  	{
		  		if(isSendOver == false)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rank_med_rm_bh" ], color:0xffff00});  //"数据传输中"
					return;
				}
		  		currentPage++;
		  		ranking.txtPageInfo.text=GameCommonData.wordDic[ "mod_rank_med_rm_hn_1" ]+currentPage+"/"+totalPage+GameCommonData.wordDic[ "mod_rank_med_rm_hn_2" ];  //"第"   "页" 
		  		cleartxt();
		  		sendInfo();
		    }
		    isbtnenable();                                                                           		 //初始化前后按钮
		}
		
		/** 初始化按钮的可见性 */
		private function btnvisible(index:int):void
		{
			for(var i:int = 0; i < 13; i++) {
				i == index ? ranking["btnList_"+i].visible = false : ranking["btnList_"+i].visible = true;
			}
		}
		
		/** 关闭排行榜 */
		private function rankCloseHandler(e:Event):void
		{
			gcAll();
		}
		
		/** 获得数据 */
		private function updateData(arr:Array):void
		{	
			var tol:int ;
			var xx:int = RankConstData.total_h;
			var rankobj:Object = RankConstData.rankAllList[rankIndex];
			RankConstData.total_h > 20 ? tol = 20 : tol = RankConstData.total_h;
				
			for(var i:int = 0; i<tol ; i++)                                   								//排列名次，从上往下排
			{
				ranking["txtData_0_"+i].text = 20 * (int(currentPage)-1) + arr[i][0];                		//将dataArr数组里的数据放到动态文本中
				if(rankIndex > 9 && rankIndex < 12)
				ranking["txtData_1_"+i].text = dataArr[i][1];
				else
				ranking["txtData_1_"+i].htmlText = "<font color='#ffffff'><a href='event:name_"  + dataArr[i][1] + "'>" + dataArr[i][1] + "</a></font>";//#0096cd
				
				ranking["txtData_2_"+i].text = arr[i][2]; 
				if(rankIndex == 10)			//如果是帮派人数，则限制一下上线
				{
					if(arr[i][3] >= UnityNumTopChange.change(int(arr[i][2]))) ranking["txtData_3_"+i].text = String(UnityNumTopChange.change(int(arr[i][2])));
					else ranking["txtData_3_"+i].text = String(arr[i][3]); 
				}
				else ranking["txtData_3_"+i].text = arr[i][3]; 
				/* if(GameCommonData.wordVersion == 2)
				{
					if(rankIndex == 0 || rankIndex == 3)
					{
						ranking["txtData_2_"+i].text = "無";
					}
				} */
				var obj:Object = new Object();
				obj.data0 = ranking["txtData_0_"+i].text;
				obj.data1 = ranking["txtData_1_"+i].htmlText;
				obj.data2 = ranking["txtData_2_"+i].text;
				obj.data3 = ranking["txtData_3_"+i].text;
				rankobj[currentPage].push(obj);
			}
			
		}
		/** 清空排行榜列表 */
		private function cleartxt():void
		{
			if(ranking == null) return;
			for(var i:int = 0; i<20 ; i++)                                     //排列名次，从上往下排
			{
				ranking["txtData_0_"+i].text = "";                 
				if(rankIndex == 11) {
					ranking["txtData_1_" + i].mouseEnabled = false;
				} else {
					ranking["txtData_1_" + i].mouseEnabled = true;
				}
				ranking["txtData_1_" + i].htmlText = '';
				ranking["txtData_2_" + i].text = ""; 
				ranking["txtData_3_" + i].text = ""; 
			}
			
		}
		/** 判断是否为第一页和第十页，如果是，按钮变黑 */
		private function isbtnenable():void
		{
			if(currentPage == 1)
				return;
			if(currentPage == totalPage)
				return;
		}
		
		private function sendInfo():void
		{
			if(isSendOver == false)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rank_med_rm_bh" ], color:0xffff00});   //"数据传输中"
				return;
			}
			var obj:Object = RankConstData.rankAllList[rankIndex];
			if(obj[this.currentPage] == null)										//如果缓存中没有当前页数组，那么就从服务器中读取
			{
				obj[this.currentPage] = [];
				Zippo.QueryTopList(rankIndex,currentPage);
				isSendOver = false;						//正在传输						
				pageBtnSee(false);
			}
			else																								//否则读取当前数组中的数据
			{
				var tol:int;
				var arr:Array = obj[this.currentPage];
				obj[this.currentPage].length > 20 ? tol = 20 : tol = obj[this.currentPage].length;
				for(var i:int = 0; i < tol ; i++)           //排列名次，从上往下排
				{
					var dataObj:Object = new Object();
					dataObj.data0 = 20 * (int(currentPage)-1) + arr[i][0];
					if(rankIndex > 9 && rankIndex < 12)
					dataObj.data1 = arr[i][1];
					else
					dataObj.data1 = "<font color='#ffffff'><a href='event:name_"  + arr[i][1] + "'>" + arr[i][1] + "</a></font>";//#0096cd
					dataObj.data2 = arr[i][2]; 
					dataObj.data3 = arr[i][3]; 
					ranking["txtData_0_"+i].text = (arr[i].data0 == null ? dataObj.data0  : arr[i].data0);
					ranking["txtData_1_"+i].htmlText =	(arr[i].data1 == null ? dataObj.data1  : arr[i].data1);
					ranking["txtData_2_"+i].text = (arr[i].data2 == null ? dataObj.data2 : arr[i].data2);
					ranking["txtData_3_"+i].text = (arr[i].data3 == null ? dataObj.data3 : arr[i].data3);
					dataObj = null;
					/* if(GameCommonData.wordVersion == 2)
					{
						if(rankIndex == 0 || rankIndex == 3)
						{
							ranking["txtData_2_"+i].text = "無";
						}
					} */
				}
				if(RankConstData.rankAllList[rankIndex].myRank)
				{
					if(int(RankConstData.rankAllList[rankIndex].myRank) > 999)  ranking.txtmyrank.text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ];  //"无"
					else  ranking.txtmyrank.text = RankConstData.rankAllList[rankIndex].myRank;
				}
				else
				{
					ranking.txtmyrank.text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ];  //"无"
				}
				if(ranking["txtData_0_"+0].text == "") pageBtnSee(false);
				else pageBtnSee(true);
			}
		}
		private function pageBtnSee(isSee:Boolean):void
		{
//			ranking.btnFrontPage.visible = isSee;
			ranking.btnBackPage.visible = isSee;
		}
		
	}
}