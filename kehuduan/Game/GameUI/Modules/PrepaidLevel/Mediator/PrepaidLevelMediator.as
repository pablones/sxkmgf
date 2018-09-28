package GameUI.Modules.PrepaidLevel.Mediator
{
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoResize.Data.AutoSizeData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidDataProxy;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Modules.PrepaidLevel.Net.PrepaidLevelNet;
	import GameUI.Modules.PrepaidLevel.view.PrepaidModelManager;
	import GameUI.Modules.Wish.Data.WishData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.ColorMatrixFilter;
	import flash.filters.GlowFilter;
	import flash.net.URLRequest;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PrepaidLevelMediator extends Mediator
	{
		public static const NAME:String = "prepaidLevelMediator";
		
		private var dataProxy:PrepaidDataProxy;
		private var modelManager:PrepaidModelManager;
		private var panelBase:PanelBase;
		private var loader:Loader;
		private var faceItem:FaceItem;
		private var allPrepaidBmp:Bitmap = new Bitmap();     //累计充值元宝
		private var needPrepaidBmp:Bitmap = new Bitmap();    //还需充值元宝
		private var showTipBmp:Bitmap = new Bitmap();        //提示还需充值元宝
		private var youLiBmp:Bitmap = new Bitmap();          //游历所需元宝
		private var fugueMC:MovieClip;
		private var wishView:MovieClip;                        //许愿面板
		private var lastTime:uint;                             //上次点击许愿按钮的时间
		private var isFirst:Boolean = true;                    //是否为第一次点击许愿按钮
		private var Question:Dictionary;                       //问题集
//		private var isShowView:Boolean = false;                //是否显示御剑江湖
		private var isFisrtRecive:Boolean = true;                    //是否为第一次收到服务器消息
		
		private const WishGridCoun:int=25;                                	//格子数量总数
		private var faceNum:int=0;
	    private var grayFilter:ColorMatrixFilter;                           //灰色滤镜，用于将按钮变灰色
	    private var yellowFilter:GlowFilter;                                //黄色框
	    private var redFilter:GlowFilter;                                   //红色框
	    private var mat:Array=new Array();                                  //滤镜颜色控制数组
	    private var srcArray:Array=new Array();                             //加载Face图片数组
	    private var faceArray:Array=new Array();                            //储存已加载Face图片数组
	    private var btn:SimpleButton;                       					//"接收回礼"按钮
	    private var txt_accept:TextField									//"接收回礼"按钮
	    private var clickCoun:int;											//剩余可点击次数
	    private var isFirstCoupons:Boolean = true;                          //是否第一次获取服务器消息

		private var textArray:Array= new Array();
		private var minCon:int=12;											//小份礼物总数量
		private var midCon:int=8;											//中份礼物总数量
		private var maxCon:int=2;											//大份礼物总数量
		private var supCon:int=2;											//超级回礼
		private var extCon:int=1;											//御剑江湖至尊回礼
		private var MoviePosition:Object=new Object();  					//存储被点击对象的name
		private var firstClick:Boolean=true;								//判断玩家是否第一次点击抽奖
		private var textData:Array=new Array();								//存储玩家获得的礼物相对应的id和type
		private var typeData:Array=new Array();								//存储玩家获得的礼物对应的type
		private var specialShowMc:MovieClip;								//特效
		private var day:uint;												//天数
		private var con:uint;	
		
		private var txt:TextField;
		private var txt2:TextField;
		
		public function PrepaidLevelMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.INITVIEW,
					EventList.ENTERMAPCOMPLETE,
					PrepaidUIData.SHOW_PREPAID_VIEW,
					PrepaidUIData.SHOW_OFFLINE_VIEW,
					PrepaidUIData.SHOW_WISH_VIEW,
					PrepaidUIData.SHOW_LOADINGVIEW,
					PrepaidUIData.UPDATE_TRAVEL_VIEW,
					PrepaidUIData.UPDATE_OFFLINE_VIEW,
					PrepaidUIData.UPDATE_PREPAID_VIEW,
					PrepaidUIData.CLOSE_PREPAID_VIEW,
					WishData.RETURN_CLICK_GRID,
					WishData.OPEN_RETURN_GRID
					];
		}
		
		private function get prepaidLevel():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case EventList.INITVIEW:
					load();
					facade.registerProxy( new PrepaidDataProxy() );
					dataProxy = facade.retrieveProxy( PrepaidDataProxy.NAME ) as PrepaidDataProxy;
					break;
				case EventList.ENTERMAPCOMPLETE:
					PrepaidLevelNet.sendPrepaidDemand(1);
//					PrepaidUIData.openFrom = "updateData"
					break;
				case PrepaidUIData.SHOW_PREPAID_VIEW:
					if( dataProxy.prepaidIsOpen == false )
					{
						initData();
						showView( notification.getBody() as String );
						dataProxy.prepaidIsOpen = true;
//						if( isShowView == true )
//						{
//							isShowView = false;
//							LoadingView.getInstance().removeLoading();
//						}
					}
					else
					{
						showPrepaid();
					}
//					PrepaidUIData.openFrom = null;
					break;
				case PrepaidUIData.SHOW_WISH_VIEW:
					dataProxy.wishState = notification.getBody() as uint;
					this.showWish();
					break;
				case PrepaidUIData.SHOW_OFFLINE_VIEW:
					if( dataProxy.prepaidIsOpen == false )
					{
						checkBtns();
						initWish();
						showView( "offline_btn" );
						dataProxy.prepaidIsOpen = true;
					}
					else 
					{
						if( dataProxy.currrentBtn == "offline_btn" )
						{
							panelCloseHandler(null);
						}
						else
						{
							showOffLine();
							dataProxy.giftPage = 0;
						}
					}
					break;
				case PrepaidUIData.SHOW_LOADINGVIEW:
					if( dataProxy.prepaidIsOpen == false )
					{
//						if( isShowView == false )
//						{
//							isShowView = true;
//							LoadingView.getInstance().showLoading();
							sendNotification(PrepaidUIData.SHOW_PREPAID_VIEW, "prepaid_btn");
							PrepaidLevelNet.sendPrepaidDemand(1);
//							PrepaidUIData.openFrom = "prepaid";
//						}
					}
					else
					{
						if( dataProxy.currrentBtn == "prepaid_btn" )
						{
							panelCloseHandler(null);
						}
						else
						{
							sendNotification(PrepaidUIData.SHOW_PREPAID_VIEW, "prepaid_btn");
							PrepaidLevelNet.sendPrepaidDemand(1);
						}
					}
					break;
				case PrepaidUIData.UPDATE_TRAVEL_VIEW:
					updateTravelView(notification.getBody());
					break;
				case PrepaidUIData.UPDATE_PREPAID_VIEW:
					dataProxy.updatePrepaid(notification.getBody());
					sendNotification( MarketEvent.UPDATE_COUPONS_BUTTON, GameCommonData.couponsCanOpen );
					if( isFirstCoupons )
					{
						isFirstCoupons = false;
						return;
					}
//					if( dataProxy.prepaidIsOpen == false )
//					{
//						sendNotification(PrepaidUIData.SHOW_PREPAID_VIEW, "prepaid_btn");
//					}
//					else
//					{
						if( dataProxy.currrentBtn == "prepaid_btn" )
						{
							updatePrepaidView();
						}
						else
						{
							showPrepaid();
						}
//					}
					break;
				case PrepaidUIData.UPDATE_OFFLINE_VIEW:
					if( dataProxy.currrentBtn == "offline_btn" )
					{
						showModelView( "offline_btn" );
						modelManager.setModel("offline_btn");
					}
					break;
				case WishData.RETURN_CLICK_GRID:
					if( dataProxy.prepaidIsOpen == true && dataProxy.currrentBtn == "wish_btn" )
					{
						var _greenFrame:MovieClip=yellowRect();
						var removeface:MovieClip=wishView.getChildByName(MoviePosition.name) as MovieClip;
						firstClick=false;
						remove_Adl( removeface );
						removeface.removeChildAt(0);
						removeface.addChild(_greenFrame);
						removeface.extra.id=- 1;
						
						var textObj:Object=notification.getBody() as Object;
						if( ( textObj.nTimeStamp + textObj.nRoleId + textObj.nMapId + textObj.nNPC ) == 0 )break;
						typeData.push(textObj.nTimeStamp);
						typeData.push(textObj.nRoleId);
						typeData.push(textObj.nMapId);
						if(textObj.nNPC!==0)
						{
						typeData.push(textObj.nNPC);
						}
						textData=judgment(typeData);
						textData=ranArray(textData);
						var tArray:Array=new Array();
						var minArray:Array=new Array();
						var midArray:Array=new Array();
						var maxArray:Array=new Array();
						var supArray:Array=new Array();
						var extArray:Array=new Array();
						if(textObj.nNPC==0)																		//判断是VIP玩家，还是非VIP玩家
						{
							clickCoun=2;
						}else
						{
							clickCoun=3;
						}
	
						tArray=add_tArray(tArray,textData);								//将玩家获得礼物的对应文字图片加入tArray数组中
						minArray=addCoun(minCon,minArray,set_min);						//将剩下的小份礼物的对应文字图片加入minArray数组中
						midArray=addCoun(midCon,midArray,set_mid);						//将剩下的中份礼物的对应文字图片加入midArray数组中
						maxArray=addCoun(maxCon,maxArray,set_max);						//将剩下的大份礼物的对应文字图片加入maxArray数组中
						supArray=addCoun(supCon,supArray,set_sup);						//将剩下的超级礼物的对应文字图片加入supArray数组中
						extArray=addCoun(extCon,extArray,set_ext);						//将剩下的御剑江湖至尊礼物的对应文字图片加入extArray数组中
						
						add_Array(minArray);
						add_Array(midArray);
						add_Array(maxArray);
						add_Array(supArray);
						add_Array(extArray);
						textArray=ranArray(textArray);									//将数组随机
						add_Array(tArray);
						add_Text(textArray,wishView);
					}
					break;
				case WishData.OPEN_RETURN_GRID:
					if( dataProxy.prepaidIsOpen == true && dataProxy.currrentBtn == "wish_btn" )
					{
						var obj:Object=notification.getBody() as Object;
						if(obj.nRoleId==0)break;
					
						if( ( obj.nTimeStamp + obj.nRoleId + obj.nMapId + obj.nNPC ) == 0 )break;
						day = obj.nPosX ;
						con = obj.nPosY ;
						typeData=null;
						typeData=new Array();
						textData=null;
						textData=new Array();
						typeData.push(obj.nTimeStamp);
						typeData.push(obj.nRoleId);
						typeData.push(obj.nMapId);
						if(obj.nNPC!==0)
						{
							typeData.push(obj.nNPC);
						}
						textData=judgment(typeData);
						receive_Text(textData);
						
						(prepaidLevel.accept_btn as SimpleButton).visible = false;
						(prepaidLevel.accept_txt as TextField).visible = false;
						(prepaidLevel.wish_txt1 as TextField).visible = false;
						(prepaidLevel.wish_txt2 as TextField).visible = false;
					}
				break;
				case PrepaidUIData.CLOSE_PREPAID_VIEW:
					if( dataProxy.prepaidIsOpen == true )
					{
						panelCloseHandler(null);
					} 
					break;
			}
		}
		
		private function load():void
		{
			loader = new Loader();
			var request:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/PrepaidLevel.swf");
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onPicComplete);
			loader.load(request);	
		}
		
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "PrePaidView" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "PrePaidView" ) as Class;
				this.viewComponent = new BgClass() as MovieClip;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "moneyIcon" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "moneyIcon" ) as Class;
				var bmpData:BitmapData = new BgClass(18, 12) as BitmapData;
				allPrepaidBmp.bitmapData = bmpData;
				needPrepaidBmp.bitmapData = bmpData;
				showTipBmp.bitmapData = bmpData;
				youLiBmp.bitmapData = bmpData;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "fugueMC" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "fugueMC" ) as Class;
				fugueMC = new BgClass() as MovieClip;
				fugueMC.x = 428;
				fugueMC.y = 125;
				fugueMC.name = "fugue_btn";
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "xiaoYaoPay" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "xiaoYaoPay" ) as Class;
				var xiaoyao:MovieClip = new BgClass() as MovieClip;
				facade.registerMediator( new AddXiaoYaoMediator() );
				sendNotification( PrepaidUIData.INIT_ADDXIAOYAO, xiaoyao );
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "travelHelp" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "travelHelp" ) as Class;
				PrepaidUIData.travelHelp = BgClass;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "gainGift" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "gainGift" ) as Class;
				var giftBtn:MovieClip = new BgClass() as MovieClip;
				PrepaidUIData.giftBtn = giftBtn;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "gift" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "gift" ) as Class;
				var gift:MovieClip = new BgClass() as MovieClip;
				gift.x = 78;
				gift.y = 308;
				PrepaidUIData.gift = gift;
			}
			e.target.removeEventListener(Event.COMPLETE, onPicComplete);
			Question = ( loader.content as Object).question as Dictionary;
//			trace ( ( (loader.content as Object).question as Dictionary)[1] );
			modelManager = new PrepaidModelManager( prepaidLevel, dataProxy );
			loader = null;
			initView();
			sendNotification(TerraceController.NAME , "pay");
		}
		
		private function initData():void
		{
			
			checkBtns();
			initWish();
		}
		
		private function checkBtns():void
		{
			if( dataProxy.allPrepaid>=100 )
			{
				dataProxy.hasPrepaid = true;
			}
			else
			{
				dataProxy.hasPrepaid = false;
			}
			dataProxy.unShowBtnArr = [];
			dataProxy.unShowTxtArr = [];
			if( dataProxy.hasPrepaid )
			{
				prepaidLevel.gotoAndStop(2);
				dataProxy.btnArr = ["prepaid_btn", "travel_btn"];
				dataProxy.btnTxtArr = ["prepaid_txt", "travel_txt"];
			}
			else
			{
				prepaidLevel.gotoAndStop(1);
				dataProxy.btnArr = ["prepaid_btn"];
				dataProxy.btnTxtArr = ["prepaid_txt"];
				dataProxy.unShowBtnArr.push( "travel_btn" );
				dataProxy.unShowTxtArr.push( "travel_txt" );
			}
			if( GameCommonData.Player.Role.Level >= 10 )
			{
				dataProxy.btnArr.push( "offline_btn" );
				dataProxy.btnTxtArr.push( "offline_txt" );
				if( GameCommonData.Player.Role.Level >= 25 )
				{
					dataProxy.btnArr.splice( dataProxy.btnArr.length-1, 0, "wish_btn" );
					dataProxy.btnTxtArr.splice( dataProxy.btnTxtArr.length-1, 0, "wish_txt" );
				}
				else
				{
					dataProxy.unShowBtnArr.push( "wish_btn" );
					dataProxy.unShowTxtArr.push( "wish_txt" );
				}
			}
			else
			{
				dataProxy.unShowBtnArr.push( "offline_btn" );
				dataProxy.unShowBtnArr.push( "wish_btn" );
				dataProxy.unShowTxtArr.push( "offline_txt" );
				dataProxy.unShowTxtArr.push( "wish_txt" );
			}
		}
		
		private function initView():void
		{
			if( !faceItem )
			{
				var face:uint = GameCommonData.Player.Role.Face;
				faceItem = new FaceItem(String(face),null,"face",(50/50));
//				faceItem.offsetPoint=new Point(0,0);
			}
			panelBase = new PanelBase( prepaidLevel, prepaidLevel.width+8, prepaidLevel.height+12 );
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_pre_med_pre_ini_2"]);   //充值
			panelBase.name = "SkillView";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
		}
		
		private function showView( btnStr:String ):void
		{
			AutoSizeData.setStartPoint( panelBase, UIConstData.DefaultPos1.x, UIConstData.DefaultPos1.y, 3 );
			dataProxy.currrentBtn = btnStr;
			if( btnStr == "prepaid_btn" )
			{
				if( dataProxy.hasPrepaid )
				{
					prepaidLevel.gotoAndStop(2);
				}
				else
				{
					prepaidLevel.gotoAndStop(1);
				}
			}
			else if( btnStr == "offline_btn" )
			{
				prepaidLevel.gotoAndStop(3);
			}
			stopMC();
			sortBtns();
			modelManager.setModel( btnStr );
			showModelView( btnStr );
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			addLis();
		}
		
		private function sortBtns():void
		{
			var count:uint = 0;
			for each( var str:String in dataProxy.btnArr )
			{
				prepaidLevel[str].x = 14 + count*62;
				prepaidLevel[str].visible = true;
				count++;
			}
			for each( str in dataProxy.unShowBtnArr )
			{
				prepaidLevel[str].visible = false;
			}
			count = 0;
			var tf:TextField;
			for each( str in dataProxy.btnTxtArr )
			{
				tf = prepaidLevel[str] as TextField;
				tf.x = 24 + count*62;
				tf.visible = true;
				tf.mouseEnabled = false;
				count++;
			}
			for each( str in dataProxy.unShowTxtArr )
			{
				tf = prepaidLevel[str] as TextField;
				tf.visible = false;
			}
		}
		
		private function showPrepaid():void
		{
			removeBmp();
			if( dataProxy.allPrepaid>=100 )
			{
				dataProxy.hasPrepaid = true;
			}
			else
			{
				dataProxy.hasPrepaid = false;
			}
			if( dataProxy.hasPrepaid )
			{
				prepaidLevel.gotoAndStop(2);
			}else{
				prepaidLevel.gotoAndStop(1);
			}
			stopMC();
			checkBtns();
			sortBtns();
			dataProxy.currrentBtn = "prepaid_btn";
			showModelView( "prepaid_btn" );
			modelManager.setModel("prepaid_btn");
			addLis();
		}
		
		private function showOffLine():void
		{
			removeBmp();
			prepaidLevel.gotoAndStop(3);
			stopMC();
			dataProxy.currrentBtn = "offline_btn";
			showModelView( "offline_btn" );
			modelManager.setModel("offline_btn");
		}
		
		private function showWish():void
		{
			if(dataProxy.wishState == 2)
			{
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pre_med_pre_sho_1"], color:0xffff00});  //你今天已经许愿过了，明天再来吧
				return;
			}
			removeBmp();
			prepaidLevel.gotoAndStop(4);
	 		stopMC();
			if(dataProxy.wishState == 0)
 			{
 				(prepaidLevel.wish_txt1 as TextField).visible = true;
 				(prepaidLevel.wish_txt1 as TextField).mouseEnabled = false;
 				(prepaidLevel.wish_txt2 as TextField).visible = false;
 				(prepaidLevel.accept_btn as SimpleButton).mouseEnabled = false;
 				MasterData.setGrayFilter( TextField(prepaidLevel.accept_txt) );
 				MasterData.setGrayFilter( (prepaidLevel.accept_btn as SimpleButton) );
 			}
 			else if(dataProxy.wishState == 1)
 			{
 				(prepaidLevel.wish_txt1 as TextField).visible = false;
 				(prepaidLevel.wish_txt2 as TextField).visible = true;
 				(prepaidLevel.wish_txt2 as TextField).mouseEnabled = false;
 				(prepaidLevel.accept_btn as SimpleButton).mouseEnabled = true;
 				MasterData.delGrayFilter( TextField(prepaidLevel.accept_txt) );
 				MasterData.delGrayFilter( (prepaidLevel.accept_btn as SimpleButton) );
 			}
			dataProxy.currrentBtn = "wish_btn";
	 		showModelView( "wish_btn" );
	 		modelManager.setModel("wish_btn");
		}
		
		private function updateTravelView( obj:Object ):void
		{
			dataProxy.updateTravel(obj);
			dataProxy.isWait = false;
			if( dataProxy.prepaidIsOpen == false || dataProxy.currrentBtn != "travel_btn" )
			{
				return;
			}
			prepaidLevel.gotoAndStop(5);
			stopMC();
			showModelView( "travel_btn" );
			modelManager.setModel( "travel_btn" );
		}
		
		private function updatePrepaidView():void
		{
			if( dataProxy.prepaidIsOpen == false || dataProxy.currrentBtn != "prepaid_btn" )
			{
				return;
			}	
			if( dataProxy.allPrepaid>=100 )
			{
				dataProxy.hasPrepaid = true;
			}
			else
			{
				dataProxy.hasPrepaid = false;
			}
			if( dataProxy.hasPrepaid )
			{
				prepaidLevel.gotoAndStop(2);
			}else{
				prepaidLevel.gotoAndStop(1);
			}
			stopMC();
			checkBtns();
			sortBtns();
			showModelView( "prepaid_btn" );
			modelManager.setModel("prepaid_btn");
			addLis();
//			modelManager.checkPageTxt();
		}
		
		private function addLis():void
		{
			for ( var i:uint=0; i<dataProxy.btnArr.length; i++ )
			{
				if( !( prepaidLevel[ dataProxy.btnArr[i] ] as MovieClip ).hasEventListener( MouseEvent.CLICK ) )
				{
					( prepaidLevel[ dataProxy.btnArr[i] ] as MovieClip ).addEventListener( MouseEvent.CLICK,clickBtns );
					( prepaidLevel[ dataProxy.btnArr[i] ] as MovieClip ).buttonMode = true;
				}
			}
		}
		
		private function clickBtns( event:MouseEvent ):void
		{
			if( event.target.name == dataProxy.currrentBtn ) return;
			if( event.target.name != "prepaid_btn" )
			{
				dataProxy.giftPage = 0;
			}
			switch( event.currentTarget.name )
			{
				case "prepaid_btn":
//					PrepaidLevelNet.sendPrepaidDemand(1);
					showPrepaid();
					break;
				case "offline_btn":
					showOffLine();
				 	break;
			 	case "wish_btn":
//					if( isFirst )
//					{
//						isFirst = false;
//						lastTime = getTimer();
						requestSerInfo(315); 
//					}
//					else
//					{
//						if( getTimer()-lastTime >= 60000 )
//				 		{
//				 			lastTime = getTimer();
//				 			requestSerInfo(315); 
//				 		}
//				 		else
//				 		{
//				 			this.showWish();
//				 		}
//					}
			 		
				 	break;
				case "travel_btn":
//					if( dataProxy.hasPrepaid == false )
//					{
//						sendNotification(HintEvents.RECEIVEINFO, {info:"你还没有充值，不能游历", color:0xffff00});
//						return;
//					}
					removeBmp();
					dataProxy.currrentBtn = "travel_btn";
					if( dataProxy.questionIndex>0 && dataProxy.questionIndex<101 )
					{
						prepaidLevel.gotoAndStop(5);
						stopMC();
						showModelView( "travel_btn" );
						modelManager.setModel( "travel_btn" );
						return;
					}
					PrepaidLevelNet.sendPrepaidDemand( 10 );
					break;
			}
			
		}
		
		private function stopMC():void
		{
			
			if( MovieClip(prepaidLevel.VIPLevelView) != null ) 
			{
				(prepaidLevel.VIPLevelView as MovieClip).gotoAndStop(0);
			}
			if( MovieClip(prepaidLevel.question) != null && MovieClip(prepaidLevel.question.answer1_btn) != null )
			{
				(prepaidLevel.question.answer1_btn as MovieClip).gotoAndStop(0);
			} 
			if( MovieClip(prepaidLevel.question) != null && MovieClip(prepaidLevel.question.answer2_btn) != null )
			{
				(prepaidLevel.question.answer2_btn as MovieClip).gotoAndStop(0);
			} 
			if( MovieClip(prepaidLevel.youLiTip) != null )
			{
				(prepaidLevel.youLiTip as MovieClip).gotoAndStop(0);
			}
		}
		
		private function showModelView( btnStr:String ):void
		{
			switch( btnStr )
			{
				case "prepaid_btn":
					changeToTextField(prepaidLevel.allPrepaid_txt).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_1"]+"<font color='#ff6600'>"+ dataProxy.allPrepaid + "</font>";     //累计充值：
					if( dataProxy.prepaidLevel != 10 )
					{
						changeToTextField(prepaidLevel.needPrepaid_txt).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_2"]+"<font color='#ff6600'>"+ dataProxy.needPrepaid + "</font>";  //还需充值
						changeToTextField(prepaidLevel.upToLevel_txt).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_3"]+"<font color='#E2CCA5'>"+ (dataProxy.prepaidLevel+1) + GameCommonData.wordDic[ "often_used_level" ]+"</font><font color='#ff6600'>" + PrepaidUIData.stateArr[dataProxy.prepaidLevel] + "</font>";
					}                                                           //提升到                                                               级
					else
					{
						changeToTextField(prepaidLevel.needPrepaid_txt).htmlText = "";
						changeToTextField(prepaidLevel.upToLevel_txt).htmlText = "";
						changeToMovieClip(prepaidLevel.showTip).visible = false;
					}
					var tf:TextField = prepaidLevel.upToLevel_txt as TextField;
					checkPos( tf, 1 );
					tf = prepaidLevel.allPrepaid_txt as TextField;
					checkPos( tf, 2 );
					if( dataProxy.hasPrepaid )
					{
//						if( dataProxy.prepaidLevel > 0 )
//						{
							changeToTextField(prepaidLevel.currentLevel_txt).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_4"]+"<font color='#E2CCA5'>"+ dataProxy.prepaidLevel + GameCommonData.wordDic[ "often_used_level" ]+"</font><font color='#ff6600'>" + PrepaidUIData.stateArr[dataProxy.prepaidLevel-1] +"</font>" ;
							if( dataProxy.canGetZhuBao == 1 )                          //充值等级：                                                                                                          //级
							{
								changeToSimpleButton(prepaidLevel.zhuBao_btn).visible = true;
								changeToTextField(prepaidLevel.zhuBao_txt).visible = true;
								changeToTextField(prepaidLevel.zhuBao_txt).mouseEnabled = false;
								changeToSimpleButton(prepaidLevel.zhuBao_btn).addEventListener( MouseEvent.CLICK, onClikFun );
							}
							else
							{
								changeToSimpleButton(prepaidLevel.zhuBao_btn).visible = false;
								changeToTextField(prepaidLevel.zhuBao_txt).visible = false;
							}
//							prepaidLevel.currentLevel_txt.visible = true;
//						}
//						else
//						{
//							prepaidLevel.currentLevel_txt.visible = false;
//						}
						if( dataProxy.prepaidLevel != 10 )
						{
							changeToTextField(prepaidLevel.topLevel_txt).visible = false;
							changeToTextField(prepaidLevel.showTip.upLevelPrepaid_txt).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_2"]+"<font color='#ff6600'>"+ dataProxy.needPrepaid + "</font>";   //还需充值
							tf = prepaidLevel.showTip.upLevelPrepaid_txt as TextField;
							checkPos( tf, 3 );
							changeToMovieClip(prepaidLevel.progressBar.progress).width = Number(dataProxy.upLevelPrepaid-dataProxy.needPrepaid)/dataProxy.upLevelPrepaid * 318;
							changeToMovieClip(prepaidLevel.showTip).x = 11 + changeToMovieClip(prepaidLevel.progressBar.progress).width;
							if( changeToMovieClip(prepaidLevel.progressBar.progress).width < 21 )
							{
								changeToMovieClip(prepaidLevel.progressBar.blur).width = changeToMovieClip(prepaidLevel.progressBar.progress).width;
								prepaidLevel.progressBar.blur.x = 0.9;
							}
							else
							{
								(prepaidLevel.progressBar.blur as MovieClip).x = (prepaidLevel.progressBar.progress as MovieClip).x + (prepaidLevel.progressBar.progress as MovieClip).width - 18.4;
							}
						}
						else
						{
							(prepaidLevel.topLevel_txt as TextField).visible = true;
							(prepaidLevel.progressBar as MovieClip).visible = false;
						}
					}
					if( !prepaidLevel.playerFace.contains( faceItem ) )
					{
						faceItem.x = 2;
						faceItem.y = 2;
						prepaidLevel.playerFace.addChild( faceItem );
					} 
					if( !prepaidLevel.contains( PrepaidUIData.gift ) ) prepaidLevel.addChild( PrepaidUIData.gift );
					if( !prepaidLevel.contains( allPrepaidBmp ) ) prepaidLevel.addChild( allPrepaidBmp );
					break;
				case "offline_btn":
					(prepaidLevel.gain1_txt as TextField).mouseEnabled = false;
					(prepaidLevel.gain2_txt as TextField).mouseEnabled = false;
					(prepaidLevel.gain3_txt as TextField).mouseEnabled = false;
					(prepaidLevel.gain4_txt as TextField).mouseEnabled = false;
					if(AutoPlayData.offLineTime  < 1)
					{
						prepaidLevel.offLineTime_txt.htmlText = "<font color='#00ff00'>0</font><font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_pre_med_pre_showm_5"]+"</font><font color='#ff0000'>"+GameCommonData.wordDic[ "mod_pre_med_pre_showm_6"]+"</font>";//个小时   （最多累计8小时）
	 					MasterData.setGrayFilter( prepaidLevel.gain1_txt );
	 					MasterData.setGrayFilter( prepaidLevel.gain1_btn );
	 					MasterData.setGrayFilter( prepaidLevel.gain2_txt );
	 					MasterData.setGrayFilter( prepaidLevel.gain2_btn );
	 					MasterData.setGrayFilter( prepaidLevel.gain3_txt );
	 					MasterData.setGrayFilter( prepaidLevel.gain3_btn );
	 					MasterData.setGrayFilter( prepaidLevel.gain4_txt );
	 					MasterData.setGrayFilter( prepaidLevel.gain4_btn );
	 					(prepaidLevel.gain1_btn as SimpleButton).mouseEnabled = false;
						(prepaidLevel.gain2_btn as SimpleButton).mouseEnabled = false;
						(prepaidLevel.gain3_btn as SimpleButton).mouseEnabled = false;
						(prepaidLevel.gain4_btn as SimpleButton).mouseEnabled = false;
					}
					else
					{
						(prepaidLevel.offLineTime_txt as TextField).htmlText = "<font color='#00ff00'>"+ AutoPlayData.offLineTime +"</font><font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_pre_med_pre_showm_5"]+"</font><font color='#ff0000'>"+GameCommonData.wordDic[ "mod_pre_med_pre_showm_6"]+"</font>";//个小时    （最多累计8小时）
	 					MasterData.delGrayFilter( prepaidLevel.gain1_txt );
	 					MasterData.delGrayFilter( prepaidLevel.gain1_btn );
	 					MasterData.delGrayFilter( prepaidLevel.gain2_txt );
	 					MasterData.delGrayFilter( prepaidLevel.gain2_btn );
	 					MasterData.delGrayFilter( prepaidLevel.gain3_txt );
	 					MasterData.delGrayFilter( prepaidLevel.gain3_btn );
	 					MasterData.delGrayFilter( prepaidLevel.gain4_txt );
	 					MasterData.delGrayFilter( prepaidLevel.gain4_btn );
					}
					var offLineExp:uint = AutoPlayData.offLineTime *int(-1.06 * 0.01 * Math.pow(GameCommonData.Player.Role.Level ,3) + 3.26 * Math.pow(GameCommonData.Player.Role.Level , 2) + 32.7 * GameCommonData.Player.Role.Level + 384);
					(prepaidLevel.offLine_txt1 as TextField).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_7"]+"<font color='#FFFF00'>"+ offLineExp +"</font>";//直接可领取的经验值为：
					(prepaidLevel.offLine_txt2 as TextField).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_8"]+"<font color='#FFFF00'>"+ offLineExp * 2 +"</font>";//使用碎银可领取的经验值为：
					(prepaidLevel.offLine_txt3 as TextField).htmlText = GameCommonData.wordDic[ "mod_pre_med_add_han_1"]+"<font color='#00FF00'>1</font>"+GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ask_1" ]+"<font color='#00FFFF'>"+GameCommonData.wordDic[ "mod_pre_med_add_han_2"]+"</font>"+GameCommonData.wordDic[ "mod_pre_med_pre_showm_9"]+"<font color='#FFFF00'>"+ offLineExp * 4 +"</font>";//使用     个    逍遥丹     可领取的经验值为：
					(prepaidLevel.offLine_txt4 as TextField).htmlText = GameCommonData.wordDic[ "mod_pre_med_add_han_1"]+"<font color='#00FF00'>2</font>"+GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ask_1" ]+"<font color='#00FFFF'>"+GameCommonData.wordDic[ "mod_pre_med_add_han_2"]+"</font>"+GameCommonData.wordDic[ "mod_pre_med_pre_showm_9"]+"<font color='#FFFF00'>"+ offLineExp * 8 +"</font>";//使用     个   逍遥丹     可领取的经验值为：
					break;
				case "wish_btn":
					(prepaidLevel.accept_txt as TextField).mouseEnabled = false;
					for(var x:int = 0 ;x < WishGridCoun ;x++ )
					{
						var bitmapdata:BitmapData=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("qMark");
						var bitmap:Bitmap=new Bitmap(bitmapdata);
						var bmc:MovieClip=new MovieClip();
						bmc.addChild(bitmap);
						wishView.x = 42;
						wishView.y = 82;
						prepaidLevel.addChild( wishView );
						var _x:int= x % 5;
						var _y:int= Math.floor( x/5 );
						bmc.x= 59 * _x;
						bmc.y= 59 * _y;
						bmc.name="bitmap_"+x;
						this.wishView.addChild(bmc);
					}
					for(var dx: int = 0; dx < WishGridCoun ; dx++)
					{
						var loader:Loader=new Loader();
						var r:URLRequest=new URLRequest();
						r.url = GameCommonData.GameInstance.Content.RootDirectory+"Resources/Face/"+srcArray[dx];
		//				faceNum++;
						loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onComplete);
						loader.load(r);
					}
				 	break;
				case "travel_btn":
					(prepaidLevel.canYouLi_txt as TextField).htmlText = GameCommonData.wordDic[ "mod_pre_med_pre_showm_10"]+ dataProxy.usedYouLiCount +"/"+ dataProxy.canYouLiCount;//今日可游历次数：
					(prepaidLevel.money_txt as TextField).htmlText = "<font color='#ff6600'>"+ dataProxy.fugueNeedMoney +"</font>";
					tf = prepaidLevel.money_txt as TextField;
					dataProxy.selectAnswer = 0;
					checkPos( tf, 4 );
					
					if( !prepaidLevel.contains( fugueMC ) ) prepaidLevel.addChild( fugueMC );
					if( 0 < dataProxy.questionIndex && dataProxy.questionIndex < 101 )
					{
						var arr:Array = Question[dataProxy.questionIndex] as Array;
						(prepaidLevel.question.title_txt as TextField).htmlText = arr[0];
						(prepaidLevel.question.description_txt as TextField).htmlText = arr[1];
						(prepaidLevel.question.answer1_txt as TextField).htmlText = arr[2];
						(prepaidLevel.question.answer2_txt as TextField).htmlText = arr[3];
						(prepaidLevel.question.answer1_btn as MovieClip).gotoAndStop(2);
						(prepaidLevel.question.answer2_btn as MovieClip).gotoAndStop(1);
						dataProxy.selectAnswer = 1;
					}
					break;
			}
		}
		
		private function onClikFun( e:MouseEvent ):void
		{
			PrepaidLevelNet.sendPrepaidDemand(13);
			(prepaidLevel.zhuBao_btn as SimpleButton).removeEventListener( MouseEvent.CLICK, onClikFun );
			(prepaidLevel.zhuBao_btn as SimpleButton).visible = false;
			(prepaidLevel.zhuBao_txt as TextField).visible = false;
		}
		
		private function changeToMovieClip( disObj:* ):MovieClip
		{
			return MovieClip(disObj);
		}
		
		private function changeToTextField( disObj:* ):TextField
		{
			return TextField(disObj);
		}
		
		private function changeToSimpleButton( disObj:* ):SimpleButton
		{
			return SimpleButton(disObj);
		}
		
		private function checkPos( tf:TextField, pos:uint ):void
		{
			var num:uint;
			switch( pos )
			{
				case 1:
					if( dataProxy.prepaidLevel != 10 )
					{
						num = checkIndex( dataProxy.needPrepaid, 6 );
						tf.x = 184-num*6;
						this.needPrepaidBmp.x = 163-num*6;
						this.needPrepaidBmp.y = 173;
						prepaidLevel.addChild( needPrepaidBmp );
					}
					break;
				case 2:
					num = checkIndex( dataProxy.allPrepaid, 7 );
					tf.x = 417+num*3;
					allPrepaidBmp.x = 523-num*3;
					allPrepaidBmp.y = 151;
					prepaidLevel.addChild( allPrepaidBmp );
					break;
				case 3:
					num = checkIndex( dataProxy.needPrepaid, 6 )
					tf.x = 4+num*3;
					this.showTipBmp.x = 91-num*3;
					this.showTipBmp.y = 4;
					prepaidLevel.showTip.addChild( showTipBmp );
					break;
				case 4:
					num = checkIndex( dataProxy.fugueNeedMoney, 6 );
					tf.x = 441+num*3;
					youLiBmp.x = 482-num*3;
					youLiBmp.y = 162;
					prepaidLevel.addChild( youLiBmp );
					break;
			}
		}
		
		private function checkIndex( num:uint, n:int ):uint
		{
			var count:uint = 0;
			while( n-1 >= 1 )
			{
				if( num/Math.pow(10, n-1) >= 1 )
				{
					return count;
				}
				count++;
				n--;
			}
			return count;
		}
		
		private function removeBmp():void
		{
			if( prepaidLevel.contains( needPrepaidBmp ) )
			{
				prepaidLevel.removeChild( needPrepaidBmp );
			} 
			if( prepaidLevel.contains( allPrepaidBmp ) ){
				prepaidLevel.removeChild( allPrepaidBmp );
			}
			if( MovieClip(prepaidLevel.showTip) && MovieClip(prepaidLevel.showTip).contains( needPrepaidBmp ) )
			{
				MovieClip(prepaidLevel.showTip).removeChild( showTipBmp );
			}
			if( prepaidLevel.contains( youLiBmp ) )
			{
				prepaidLevel.removeChild( youLiBmp ) ;
			}
			if( prepaidLevel.contains( fugueMC ) )
			{
				prepaidLevel.removeChild( fugueMC );
			}
			if( prepaidLevel.contains( PrepaidUIData.giftBtn ) )
			{
				prepaidLevel.removeChild( PrepaidUIData.giftBtn );
			}
			if( prepaidLevel.contains( PrepaidUIData.gift ) )
			{
				prepaidLevel.removeChild( PrepaidUIData.gift );
			}
			if( txt && prepaidLevel.contains( txt ) )
			{
				prepaidLevel.removeChild( txt );
			}
			if( txt2 && prepaidLevel.contains( txt2 ) )
			{
				prepaidLevel.removeChild( txt2 );
			}
//			if( prepaidLevel.contains( faceItem ) )
//			{
//				prepaidLevel.removeChild( faceItem );
//			} 
			removeAll( wishView );
		}
		
		private function panelCloseHandler(event:Event):void
		{
			dataProxy.prepaidIsOpen = false;
			modelManager.removeAllLis();
			modelManager.removeYouliBtn();
			for ( var i:uint=0; i<dataProxy.btnArr.length; i++ )
			{
				( prepaidLevel[ dataProxy.btnArr[i] ] as MovieClip ).removeEventListener( MouseEvent.CLICK,clickBtns );
			}
			
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			
			removeBmp();
		}
		
		private function initWish():void
		{
			wishEffects();
			firstClick=true;
			minCon=12;
			midCon=8;
			maxCon=2;
			supCon=2;
			extCon=1;
			faceNum=0;
			textArray = null ;
			textArray = new Array();
			textData = null ;
			textData = new Array();
			typeData = null ;
			typeData = new Array();
			redFilter=new GlowFilter(0xFF0000, 0.8, 8, 8, 2, 1, true);
			yellowFilter=new GlowFilter(0xFFFF00, 0.8, 8, 8, 2,1, true);
			srcArray=["21.png","5.png","23.png","6.png","18.png",
		         	"64.png","26.png","61.png","20.png","66.png",
		          	"16.png","9.png","27.png","65.png","28.png",
		          	"60.png","11.png","62.png","24.png","68.png",
		         	"10.png","67.png","25.png","63.png","22.png"];
		    wishView = new MovieClip();
		}
		
		/*接受礼物后，显示的对应文字*/
		private function receive_Text(arr:Array):void
		{
			var _arr:Array=new Array();
			txt = new TextField();
			txt2 = new TextField();
			var TF:TextFormat=new TextFormat();
			var TF2:TextFormat =new TextFormat();
			txt.text= "  " + GameCommonData.wordDic[ "mod_wis_med_wis_2" ] +"\r";						//"恭喜你得到了以下礼品："
				_arr=arr.slice();
				_arr.sortOn("num",Array.NUMERIC);
				for(var j:int=0;j<_arr.length;j++) 
				{
					switch(_arr[j].num)
					{
						case 1:

						txt.text = txt.text + "      " + GameCommonData.wordDic[ "mod_wis_med_wis_3" ] + _arr[j].name + "\r";					//"小份感谢："			
						
						break;
						case 2:

						txt.text = txt.text + "      " + GameCommonData.wordDic[ "mod_wis_med_wis_4" ] + _arr[j].name + "\r";					//"中份感谢："
						break;
						case 3:

						txt.text = txt.text + "      " + GameCommonData.wordDic[ "mod_wis_med_wis_5" ] + _arr[j].name + "\r";						//"大份回礼："
						break;
						case 4:

						txt.text = txt.text + "      " + GameCommonData.wordDic[ "mod_wis_med_wis_6" ] + _arr[j].name + "\r";						//"超级回礼："
						break;
					}					
				}
				
				txt.text = txt.text + "      " + GameCommonData.wordDic[ "mod_wis_med_wis_7" ] + GameCommonData.wordDic[ "mod_wis_med_wis_8" ] + con + GameCommonData.wordDic[ "mod_wis_med_wis_9" ] + "\r";				//"活动回赠："  "许愿花"    "朵"
				txt2.text = "      " + GameCommonData.wordDic[ "mod_wis_med_wis_10" ] + day + GameCommonData.wordDic[ "mod_wis_med_wis_11" ] + "\r" ;     //"你已连续许愿了"    "天"
				txt.multiline= true;
				
				TF.size=12;
				TF.font="宋体";
				TF.color=0xE2CCA5;
				TF.leading=5;
				TF.align = TextFormatAlign.LEFT;
				
				TF2.size = 12;
				TF2.font = "宋体";
				TF2.color = 0x00FF00;
				TF2.leading = 5;
				TF2.align = TextFormatAlign.LEFT;
				txt.setTextFormat(TF);
				txt.x=340;
				txt.y=69;
				txt.width=210;
				txt.height=140;
				txt.selectable=false;
				
				txt2.selectable = false ;
				txt2.x = 340;
				txt2.width = 210 ;
				txt2.y = 192;
				txt2.setTextFormat( TF2 );
				prepaidLevel.addChild(txt);
				prepaidLevel.addChild(txt2);
		}
		
		private function onComplete(e:Event):void
		{
			if( GameCommonData.GameInstance.GameUI.contains(panelBase) && dataProxy.currrentBtn == "wish_btn" )
			{
				faceNum++;
				var j:int=faceNum-1;
				faceArray[j]=e.target.content as Bitmap;
				var _name:String= "bitmap_" + j ;
				var bm:MovieClip=wishView.getChildByName(_name) as MovieClip ;
				if( bm == null )
				{
					return;
				}
				var container:MovieClip=new MovieClip();
				container.addChild(faceArray[j]);
				container.x = bm.x - 9;
				container.y = bm.y - 9;
				wishView.addChild( container );
				container.name="face_"+String(j);
				wishView.removeChild(bm);
			}
			if(faceNum==WishGridCoun)
			{
				mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // red
            	mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // green
            	mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // blue
            	mat = mat.concat([0, 0, 0, 1, 0]);                                                                  // alpha
            	grayFilter=new ColorMatrixFilter(mat);
            	clickCoun=3;                                                                                       //可点击次数

            	faceAddLis(wishView);
            	e.currentTarget.loader.unload();
			}
//			faceNum++;
		}
		
		private function faceAddLis(wishView:MovieClip):void                                               //给每个组件添加鼠标事件
		{
			for(var i:int=0;i< WishGridCoun ;i++)
			{
				var _name:String="face_"+i;
				var face:MovieClip=wishView.getChildByName(_name) as MovieClip;
				face.extra={id:i};
				face.addEventListener(MouseEvent.MOUSE_DOWN,facedown);
				face.addEventListener(MouseEvent.MOUSE_UP,faceup);
				face.addEventListener(MouseEvent.MOUSE_OVER,faceover);
				face.addEventListener(MouseEvent.MOUSE_OUT,faceout);
			}
			btn=prepaidLevel.accept_btn as SimpleButton;
			if(!btn.hasEventListener(MouseEvent.CLICK))
			{
	    		btn.addEventListener(MouseEvent.CLICK,btnclick);
			}
//			if(dataProxy.wishState != 1)btn.filters=[grayFilter];                                                           //运用滤镜，将“接受回礼”颜色变成灰色
		}
		
		private function facedown(e:MouseEvent):void
		{
			e.currentTarget.filters=[yellowFilter];
		}
		
		private function faceup(e:MouseEvent):void
		{
			if(e.currentTarget.filters[0] && e.currentTarget.filters[0].color==16776960)                                    //黄色的颜色代码16776960，红色的颜色代码是16711680
			{
				if(clickCoun!==0)
				{
					var greenFrame:MovieClip=yellowRect();
					if(!firstClick && textArray.length>0)
					{
						e.currentTarget.filters=[];
						remove_Adl( e.currentTarget as MovieClip );
						(e.currentTarget as MovieClip).removeChildAt(0);
						
						(e.currentTarget as MovieClip).addChild(greenFrame);
						(e.currentTarget as MovieClip).extra.id=- 1;
						clickCoun--;
						MoviePosition={name:(e.currentTarget as MovieClip).name};
						add_Text(textArray,wishView);
					}
					if(firstClick)
					{
						e.currentTarget.filters=[];
						requestSerInfo(311);                                               
						MoviePosition={name:(e.currentTarget as MovieClip).name};
						dataProxy.wishState = 1;
					}
				}                                                                                    
				if(clickCoun==0)
				{
//					if(btn.filters!==null)btn.filters=null;                                             //将btn按钮的滤镜去除，恢复btn原有的色彩\
					prepaidLevel.wish_txt1.visible = false;
	 				prepaidLevel.wish_txt2.visible = true;
	 				prepaidLevel.wish_txt2.mouseEnabled = false;
	 				prepaidLevel.accept_btn.mouseEnabled = true;
	 				MasterData.delGrayFilter( prepaidLevel.accept_txt );
 					MasterData.delGrayFilter( prepaidLevel.accept_btn );
// 					prepaidLevel.accept_txt.filters = null;

					MasterData.addGlowFilter( prepaidLevel.accept_txt );
					
//					var blur:BlurFilter = new BlurFilter(2, 2, 6, 
 					prepaidLevel.accept_btn.filters = null;
					replaceAll(wishView,textArray);
				}
			}
		}
		
		private function faceover(e:MouseEvent):void
		{
			e.currentTarget.filters=[redFilter];
		}
		
		private function faceout(e:MouseEvent):void
		{
			e.currentTarget.filters=[];
		}
		
		private function btnclick(e:MouseEvent):void          //点击“接受回礼”按钮时触发的事件
		{
//			if(dataProxy.wishState == 1) dataProxy.wishState = 2;
//			prepaidLevel.accept_btn.mouseEnabled = false;
//			MasterData.setGrayFilter( prepaidLevel.accept_txt );
//	 		MasterData.setGrayFilter( prepaidLevel.accept_btn );
			requestSerInfo(310);                           //发送接收礼物信息           
			
		}
		
		/*移除单个监听*/
		private function remove_Adl(mc:MovieClip):void	
		{
			mc.removeEventListener(MouseEvent.MOUSE_DOWN,facedown);
			mc.removeEventListener(MouseEvent.MOUSE_UP,faceup);
			mc.removeEventListener(MouseEvent.MOUSE_OVER,faceover);
			mc.removeEventListener(MouseEvent.MOUSE_OUT,faceout);
		}
		
		/*移除所有监听*/
		private function removeAll(wishView:MovieClip):void
		{
			if( dataProxy.currrentBtn != "wish_btn" ) return;
			prepaidLevel.removeChild( this.wishView );
			for(var i:int=0;i<wishView.numChildren;i++)
			{
//				trace( prepaidLevel.getChildAt( i ).name );
				if( wishView.getChildAt( i ).name.split("_")[0] == "face" )
				{
					var face:* = wishView.getChildAt( i );
//					if( ( face is MovieClip ) && face.extra != undefined )
//					{
						if(face.hasEventListener(MouseEvent.MOUSE_DOWN))
						{
							face.removeEventListener(MouseEvent.MOUSE_DOWN,facedown);
							face.removeEventListener(MouseEvent.MOUSE_UP,faceup);
							face.removeEventListener(MouseEvent.MOUSE_OVER,faceover);
							face.removeEventListener(MouseEvent.MOUSE_OUT,faceout);  
						}
//					}
					wishView.removeChild(face);
				}
			}
			initWish();
		}
		
		/*发送指令*/
		private function requestSerInfo(num:Number):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(num);							//进入地图
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		/*将人物头像换成文字*/
		private function replaceAll(wishView:MovieClip,array:Array):void
		{
			for(var i:int=0;i<wishView.numChildren;i++)
			{
				var face:*=wishView.getChildAt( i );
				if((face is MovieClip) && (face.extra != undefined) && (face.extra.id!=-1))
				{
					
					var z:int=array.length-1;
					face.removeEventListener(MouseEvent.MOUSE_DOWN,facedown);
					face.removeEventListener(MouseEvent.MOUSE_UP,faceup);
					face.removeEventListener(MouseEvent.MOUSE_OVER,faceover);
					face.removeEventListener(MouseEvent.MOUSE_OUT,faceout);
					face.removeChildAt(0);
					face.addChild(array[z]);
					textArray.pop();
				}
			}
		}
		
		/*生成黄色框*/
		private function yellowRect():MovieClip
		{
			var mc:MovieClip=new MovieClip()
			mc.graphics.lineStyle( 2, 0xFFFF00, 1);
			mc.graphics.lineTo( 50, 0);
			mc.graphics.lineTo( 50, 50);
			mc.graphics.lineTo( 0, 50);
			mc.graphics.lineTo( 0, 0);
			return mc;
		}
		
		private function wishEffects():void
		{
        	if(!specialShowMc){
	        	ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/wishEffects.swf",onLoadComplete);
        	}
        	else{
        		onLoadComplete();
        	}
        }
		
		private function onLoadComplete():void
		 {
        	if(!specialShowMc){
        		specialShowMc=ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/wishEffects.swf");
        	}
        	specialShowMc.gotoAndStop(1);
        	
        }
        
        /*加入文字*/
		private function add_Text(array:Array,wishView:MovieClip):void
		{
			var z:int=array.length-1;
			var n:int=wishView.numChildren;
			var _name:String=MoviePosition.name;
			var face:MovieClip=wishView.getChildByName(_name) as MovieClip;
			face.extra.id=-1;
			face.addChild(specialShowMc);
			specialShowMc.gotoAndPlay(1);
			specialShowMc.x=25;
			specialShowMc.y=25;
			face.addChild(array[z]);
			face.setChildIndex(array[z],face.numChildren-1);
			array.pop();
		}
		
		private function judgment(array:Array):Array
		{
			var arr:Array=new Array();
			for(var i:int=0;i<array.length;i++)
			{
				var num:Number=array[i];
				var id:int;
				if(Math.floor(num/1000000)==1)
					{
						num=num-1000000;
						if( num < ( GameCommonData.Player.Role.Level * 95 ))
						{
							id = 1 ;
						}else
						{
							id = 2 ;
						}
						arr[i]={num:id,name:String(num)+GameCommonData.wordDic[ "mod_wis_med_wis_20" ]};				//"经验"
					}else
					{
						num=num - 2000000;
						if( num== 501028 || num == 610053 || num== 610055 )
						{
							id = 3 ;
						}else if( num == 501000 || num == 501029 || num == 610018 )
						{
							id = 4; 
						}
						arr[i] = {num:id,name:String(UIConstData.getItem( num ).Name)};
					}
			}
			
			return arr;
		}
		/*随机数组*/
		private function ranArray(array:Array):Array
		{
			var ran:Function=function():int
			{
				var id:Number=Math.random()-0.5
				if(id<0)
				{
					return -1;
				}else
				{
					return 1;
				}
			}
			var _arr:Array=new Array();
				_arr=array.slice();
				_arr.sort(ran);
				return _arr;
		}
		/*生成对应的文字，加入对应数组中*/
		private function addCoun(num:int,array:Array,fun:Function):Array
		{
			var arr:Array=new Array();
			for(var j:int=0;j<num;j++)
			{
				array[j]= fun();
			}
			arr=array.slice();
			return arr;
		}
		/*将玩家获得的奖品对应的文字加入到数组中*/
		/*1：小份感谢 2：中份感谢 3：大份回礼 4：超级回礼 5：御剑江湖至尊回礼*/
		private function add_tArray(array:Array,dataArray:Array):Array
		{
			var arr:Array=new Array();
			for(var i:int=0;i<dataArray.length;i++)
			{
				switch(dataArray[i].num)
				{
					case 1:
					array[i]=set_min();
					minCon--;
					break;
					case 2:
					array[i]=set_mid();
					midCon--;
					break;
					case 3:
					array[i]=set_max();
					maxCon--;
					break;
					case 4:
					array[i]=set_sup();
					supCon--;
					break;
					case 5:
					array[i]=set_ext();
					extCon--;
					break;
					default:
					break;
				}
			}
			arr=array.slice();
			return arr;
		}
		
		/*生成小份感谢*/
		private function set_min():Sprite
		{
			var sp:Sprite=new Sprite();
	   		var min_tf:TextField = new TextField();						//小份感谢
	   		var min_TF:TextFormat = new TextFormat();
	   		min_tf.text = GameCommonData.wordDic[ "mod_wis_med_wis_14" ] + "\n" + GameCommonData.wordDic[ "mod_wis_med_wis_15" ];						//"小份"  "感谢"
			min_TF.font = "宋体";
			min_TF.size = 12;
			min_TF.color = 0xE2CCA5;
			min_tf.setTextFormat(min_TF);
			min_tf.selectable= false ;
			sp.addChild(min_tf);
			min_tf.x=10;
			min_tf.y=10;
	   		return sp;
		}
		
		/*生成中份感谢*/
		private function set_mid():Sprite
		{
			var sp:Sprite=new Sprite();
			var mid_tf:TextField = new TextField();						//中份感谢
			var mid_TF:TextFormat = new TextFormat();
			mid_tf.text = GameCommonData.wordDic[ "mod_wis_med_wis_16" ] + "\n" + GameCommonData.wordDic[ "mod_wis_med_wis_15" ];			//"中份"
			mid_TF.font = "宋体";
			mid_TF.size = 12;
			mid_TF.color = 0x00FF00;
			mid_tf.setTextFormat(mid_TF);
			mid_tf.selectable = false;
			mid_tf.x=10;
			mid_tf.y=10;
			sp.addChild(mid_tf);
			return sp;
		}
		
		/*生成大份感谢*/
		private function set_max():Sprite
		{
			var sp:Sprite=new Sprite();
			var max_tf:TextField = new TextField();						//大份回礼
			var max_TF:TextFormat = new TextFormat();
			max_tf.text = GameCommonData.wordDic[ "mod_wis_med_wis_17" ] + "\n" + GameCommonData.wordDic[ "mod_wis_med_wis_18" ];						//"大份"  "回礼"
			max_TF.font = "宋体";
			max_TF.size = 12;
			max_TF.color = 0x0066FF;
			max_tf.setTextFormat(max_TF);
			max_tf.selectable = false;
			max_tf.x=10;
			max_tf.y=10;
			sp.addChild(max_tf);
			return sp;
		}
		
		/*生成超级回礼*/
		private function set_sup():Sprite
		{
			var sp:Sprite=new Sprite();
			var sup_tf:TextField = new TextField();						//超级回礼
			var sup_TF:TextFormat = new TextFormat();
			sup_tf.text = GameCommonData.wordDic[ "mod_wis_med_wis_19" ] + "\n" + GameCommonData.wordDic[ "mod_wis_med_wis_18" ]; 				//"超级"
			sup_TF.font = "宋体";
			sup_TF.size = 12;
			sup_TF.color = 0xFF00FF;
			sup_tf.setTextFormat(sup_TF);
			sup_tf.selectable = false;
			sup_tf.x=10;
			sup_tf.y=10;
			sp.addChild(sup_tf);
			return sp;
		}
		
		/*生成御剑江湖至尊回礼*/
		private function set_ext():Sprite
		{
			var sp:Sprite=new Sprite();
			var max_tf:TextField = new TextField();						//大份回礼
			var max_TF:TextFormat = new TextFormat();
			max_tf.text = GameCommonData.wordDic[ "mod_wis_med_wis_17" ] + "\n" + GameCommonData.wordDic[ "mod_wis_med_wis_18" ];
			max_TF.font = "宋体";
			max_TF.size = 12;
			max_TF.color = 0x0066FF;
			max_tf.setTextFormat(max_TF);
			max_tf.selectable = false;
			max_tf.x=10;
			max_tf.y=10;
			sp.addChild(max_tf);
			return sp;
		}
		/*将数组加入textArray*/
		private function add_Array(array:Array):void
		{
			if(array!==null)
			{
				textArray=textArray.concat(array);
			}
		}
	}
}