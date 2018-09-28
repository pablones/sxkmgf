package GameUI.Modules.VipShow.VipShowMediator
{
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.VipShow.Data.VipShowData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.ActionSend.VipListSend;
	import Net.Protocol;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.ProgressEvent;
	import flash.filters.ColorMatrixFilter;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.IMediator;
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class VipShowMediator extends Mediator implements IMediator
	{
		public static var NAME:String = "VipShowMediator";
		
		private var view:MovieClip;														//vip界面背景
		private var VIPexplain:MovieClip;												//vip说明文字
		private var page1:MovieClip;													//vip信息界面
		private var page2:MovieClip;													//vip查询界面
		private var p:int;																//0 VIP信息界面，1 VIP查询界面 
		private var vipArr:Object;														//存储当前所有在线VIP玩家的信息
		private var panelBase:PanelBase;												
		private var scrollPanel:UIScrollPane;
		private var buyPanel:PanelBase;
		private var buy_vip:MovieClip;
		private var vipType:uint;
		private var pageNum:uint;														//总页数
		private var pagePos:uint = 1;													//当前所在页数
		private var arr:Array;															//存储当前页面所有显示的VIP信息
		private var loaded:Boolean = false;												//控制只加载一次
		private var loader:Loader;
		
		private var panelBaseX:int;
		private var panelBaseY:int;
		private var time:String;														//vip剩余时间
		private var timetxt:String;														//vip剩余多少天（或时）
		private var price:uint;															//购买VIP类型的价格
		
		public function VipShowMediator()
		{
			super( NAME );
		}
		
		override public function listNotificationInterests():Array
		{
			return[ VipShowData.LOAD_VIPSHOW_VIEW,
					RoleEvents.UPDATE_OTHER_INFO,
					VipShowData.CLOSE_VIPSHOW_VIEW,
					VipShowData.UPDATA_VIP_SHOW,
					VipShowData.UPDATA_USERINFO
//					EventList.CLOSE_NPC_ALL_PANEL 
					];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			
			switch( notification.getName() )
			{
				case VipShowData.LOAD_VIPSHOW_VIEW:
					if( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase )) return;
					if( vipArr )
					{
						vipArr = null;
					}
					vipArr = notification.getBody() as Object;
					add_set( vipArr );
					if( !loaded )
					{
						load_vip();
					}
					else
					{
						initView();
					}
					break;
					
				case RoleEvents.UPDATE_OTHER_INFO:
					var obj:Object = notification.getBody() as Object;
					if( obj.target && obj.target == "vipTime_txt" )
					{
						time = obj.value;
					}else
					{
						break;
					}
					if( page1 )
					{
						timetxt  = getVipTime( time );
					}
					break;
					
				case VipShowData.CLOSE_VIPSHOW_VIEW:
					closeHandler();
					buyPanelcloseHandler();
					break;
					
				case EventList.CLOSE_NPC_ALL_PANEL:
					closeHandler();
					buyPanelcloseHandler();
					break;
				case VipShowData.UPDATA_VIP_SHOW:
					if( vipArr )
					{
						vipArr = null;
					}
					vipArr = notification.getBody() as Object;
					add_set( vipArr );
					break;
				case VipShowData.UPDATA_USERINFO:
					if( page1 ) ( page1.play_name as TextField ).text = GameCommonData.Player.Role.Name;
				default:
					break;
			}
		}
		/* 加载资源 */
		private function load_vip():void
		{
			loader = new Loader();
			var r:URLRequest = new URLRequest();
			var adr:String = GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/" + "VipShow.swf";
			r.url = adr;
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE, onComplete );
			loader.contentLoaderInfo.addEventListener( ProgressEvent.PROGRESS, onProgress );
			loader.load( r );
			loaded = true;
		}
		
		private function onProgress(e:Event):void
		{
			LoadingView.getInstance().showLoading();
		}
		
		private function onComplete( e:Event ):void
		{
			LoadingView.getInstance().removeLoading();
			var domain:ApplicationDomain = e.target.applicationDomain as ApplicationDomain;
			
			if( domain.hasDefinition( "VIP_view" ) )
			{
				var c1:Class = domain.getDefinition( "VIP_view" ) as Class;
				view = new c1();
			}
			
			if( domain.hasDefinition( "page1" ) )
			{
				var c2:Class = domain.getDefinition( "page1" ) as Class;
				page1 = new c2();
				page1.x = 8;
				page1.y = 25;
			}
			
			if( domain.hasDefinition( "page2" ) )
			{
				var c3:Class = domain.getDefinition( "page2" ) as Class;
				page2 = new c3();
				page2.x = 8;
				page2.y = 25;
			}
			
			if( domain.hasDefinition( "buy_vip" ) )
			{
				var c5:Class = domain.getDefinition( "buy_vip" ) as Class;
				buy_vip = new c5();
				buy_vip.x = 5;
				buyPanel = new PanelBase( buy_vip , buy_vip.width - 15 , buy_vip.height + 16 );
				var b1:MovieClip = drawMc( 501032 );
					b1.x = 17.5;
					b1.y = 59;
					buy_vip.addChild( b1 );
				var b2:MovieClip = drawMc( 501011 );
					b2.x = 89.5;
					b2.y = 59;
					buy_vip.addChild( b2 );
				var b3:MovieClip = drawMc( 501013 );
					b3.x =161.5;
					b3.y = 59;
					buy_vip.addChild( b3 );
				buyPanel.addEventListener( Event.CLOSE, buyPanelcloseHandler );
			}
			
			if( domain.hasDefinition( "VIPexplain" ) )
			{
				var c4:Class = domain.getDefinition( "VIPexplain" ) as Class;
				VIPexplain = new c4();
				VIPexplain.mouseChildren = false;
				VIPexplain.mouseEnabled = false;
				if( VIPexplain.hasOwnProperty( "mc_bottom_1" )) VIPexplain.mc_bottom_1.width = 266;
				if( VIPexplain.hasOwnProperty( "mc_bottom_2" )) VIPexplain.mc_bottom_2.width = 266;
				if( VIPexplain.hasOwnProperty( "mc_bottom_3" )) VIPexplain.mc_bottom_3.width = 266;
//				var v1:MovieClip = drawMc( 501032 );
//				v1.x = 26;
//				v1.y = 464;
//				VIPexplain.addChild( v1 );
//				var v2:MovieClip = drawMc( 501011 );
//				v2.x = 26;
//				v2.y = 289;
//				VIPexplain.addChild( v2 );
//				var v3:MovieClip = drawMc( 501013 );
//				v3.x = 26;
//				v3.y = 28;
//				VIPexplain.addChild( v3 );

				/////////////////////////
//				var testSprite:Sprite = new Sprite;
//				testSprite.graphics.beginFill( 0xff0000, .5 );
//				testSprite.graphics.drawRect( 0, 0, 300, 606 );
//				testSprite.graphics.endFill();
//				
//				testSprite.addChild( VIPexplain );
//				scrollPanel = new UIScrollPane( testSprite );
				/////////////////////////

				scrollPanel = new UIScrollPane( VIPexplain );
				scrollPanel.x = 1;
//				scrollPanel.x = 1;
				scrollPanel.y = 88;
//				scrollPanel.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
				scrollPanel.width = 299;
//				scrollPanel.width = 290;
				scrollPanel.height = 295;
				scrollPanel.refresh();
				var txtContent:String = GameCommonData.wordDic[ "mod_vip_vip_abc_1" ];		//"您还不是VIP玩家"
				switch( GameCommonData.Player.Role.VIP )
				{
					case 0:
					txtContent = GameCommonData.wordDic[ "mod_vip_vip_abc_1" ];			//"您还不是VIP玩家"
						break;
					case 1:
					txtContent = GameCommonData.wordDic[ "mod_vip_vip_abc_2" ];			//"您是VIP月卡玩家";
						break;
					case 2:
					txtContent = GameCommonData.wordDic[ "mod_vip_vip_abc_3" ];			//"您是VIP季卡玩家";
						break;
					case 3:
					txtContent = GameCommonData.wordDic[ "mod_vip_vip_abc_4" ];			//"您是VIP半年卡玩家";
					case 4:
					txtContent = GameCommonData.wordDic[ "mod_vip_vip_abc_5" ];			//"您是VIP周卡玩家";
						break;
					default:
						break;
				}
			}

			if( view && page1 && page2 && VIPexplain )
			{
				initView();
			}
			
			loader.contentLoaderInfo.removeEventListener( Event.COMPLETE, onComplete );
			loader.contentLoaderInfo.removeEventListener( ProgressEvent.PROGRESS, onProgress );
			loader.unload();
			loader = null;
		}
		
		/* 初始化  */ 
		public function initView():void
		{
			if( time )
			{
				timetxt  = getVipTime( time );
			}
			if( !panelBase )
			{
				panelBase = new PanelBase( view, view.width + 5, view.height + 16 );
				panelBase.addEventListener( Event.CLOSE, closeHandler );
				panelBase.SetTitleTxt( "VIP" );
				view.addChild( page1 );				
			}
			p = 0;
			( view.p1 as MovieClip ).buttonMode = true;
			( view.p1 as MovieClip ).addEventListener( MouseEvent.CLICK, p1click );
			( view.p2 as MovieClip ).buttonMode = true;
			( view.p2 as MovieClip ).addEventListener( MouseEvent.CLICK, p2click );
			( view.p1 as MovieClip ).gotoAndStop( 1 );
			( view.p2 as MovieClip ).gotoAndStop( 2 );
			page1.visible = true;
			
			//加入测试
//			scrollPanel.mouseChildren = false;
//			scrollPanel.mouseEnabled = false;
			
			if ( !page1.contains( scrollPanel ) )
			{
				page1.addChild( scrollPanel );
			}
			page1.setChildIndex( scrollPanel, ( page1.getChildIndex( ( page1.left_side as MovieClip ) ) - 1) );
//			if( this.panelBaseX && this.panelBaseY )
//			{
//				panelBase.x = panelBaseX;
//				panelBase.y = panelBaseY
//			}else{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2 + 100;
//			}
			view.addChild( page2 );
			page2.visible = false;
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			VipShowData.IsVipShowOpen = true ;

			set_page1();
			
			var str:String = String( GameCommonData.Player.Role.Face );
			var faceItem:FaceItem = new FaceItem( str, null, "face" );
				faceItem.x = 2;
				faceItem.y = 2;
			( page1.face_grid as MovieClip ).addChild( faceItem );
			
			( buy_vip.btn_buy1 as MovieClip ).buttonMode = true;
			( buy_vip.btn_buy1 as MovieClip ).id = "btn_buy1";
			( buy_vip.btn_buy2 as MovieClip ).buttonMode = true;
			( buy_vip.btn_buy2 as MovieClip ).id = "btn_buy2";
			( buy_vip.btn_buy3 as MovieClip ).buttonMode = true;
			( buy_vip.btn_buy3 as MovieClip ).id = "btn_buy3";
			buy_vip.addEventListener( MouseEvent.CLICK , buy_vip_click );
			( page1.btn_vip as SimpleButton ).addEventListener( MouseEvent.CLICK, btn_vipclick );
			
				pagePos = 1;
				( page2.prev_14 as SimpleButton ).addEventListener( MouseEvent.CLICK, prevclick );
				( page2.next_15 as SimpleButton ).addEventListener( MouseEvent.CLICK, nextclick );
				setpage2();
				page2.addEventListener( MouseEvent.CLICK ,page2click );		
				
				if( GameCommonData.Player.Role.VIP == 0 )
				{
					var mat:Array = new Array();
					mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // red
	            	mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // green
	            	mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // blue
	            	mat = mat.concat([0, 0, 0, 1, 0]);                                                                  // alpha
	            	var grayFilter:ColorMatrixFilter = new ColorMatrixFilter(mat);
	            	( page1.jewelry_return as MovieClip ).buttonMode = false;
					( page1.jewelry_return as MovieClip ).filters = [ grayFilter ];
					( page1.jewelry_return as MovieClip ).mouseEnabled = false;
				}	
		}
		
		private function set_page1():void
		{
			( page1.play_name as TextField ).text = GameCommonData.Player.Role.Name;
			( page1.play_name as TextField ).selectable = false;
			( page1.play_name as TextField ).textColor = judgmentColor( GameCommonData.Player.Role.VIP );
			( page1.vip_text as TextField ).selectable = false;
			( page1.vip_text as TextField ).width = 150;
			( page1.jewelry_return as MovieClip ).buttonMode = true;
			if ( !( page1.jewelry_return as MovieClip ).hasEventListener( MouseEvent.CLICK ) )
			{
				( page1.jewelry_return as MovieClip ).addEventListener( MouseEvent.CLICK , jewelryclick ); 
			}
			( page1.jewelry_return as MovieClip ).filters = null;
			( page1.jewelry_return as MovieClip ).mouseEnabled = true;
			set_viptext(GameCommonData.Player.Role.VIP);
		}
		
		private function jewelryclick( e:MouseEvent ):void
		{
			requestSerInfo( 314 );
		}
		
		/* 设置第二页信息 */
		private function setpage2():void
		{
			for(var i:int = 0; i < 13; i++ )
			{
				( page2[ "textname_"+ i ] as TextField ).text = "";
				( page2[ "textname_"+ i ] as TextField ).mouseEnabled = false;
				( page2[ "level_"+ i ] as TextField ).text = "";
				( page2[ "level_"+ i ] as TextField ).mouseEnabled = false;
				( page2[ "address_"+ i ] as TextField ).text = "";
				( page2[ "address_"+ i ] as TextField ).mouseEnabled = false;
				( page2[ "line_"+ i ] as TextField ).text = "";
				( page2[ "line_"+ i ] as TextField ).mouseEnabled = false;
				( page2[ "btnselect_" + i ] as SimpleButton).visible = false;
			}
			( page2.text_page as TextField ).text = "第"+ pagePos + "/" + pageNum + "页";
//			var n:int = 13 * ( pagePos - 1 )
//			var m:int = n + 13;
//			if( arr )
//			{
//				arr = null;
//			}
//			 arr = vipArr.slice( n, m);
			var len:int = arr.length;
			for( var x:int = 0; x < len; x++ )
			{
				var textColor:uint = judgmentColor( arr[x].vip );
				( page2[ "textname_"+ x ] as TextField ).text = arr[x].name;
				( page2[ "textname_"+ x ] as TextField ).textColor = textColor;
				( page2[ "level_"+ x ] as TextField ).text = String( arr[x].level );
				( page2[ "level_"+ x ] as TextField ).textColor = textColor;
				( page2[ "address_"+ x ] as TextField ).text = arr[x].address;
				( page2[ "address_"+ x ] as TextField ).textColor = textColor;
				( page2[ "line_"+ x ] as TextField ).text = arr[x].line;
				( page2[ "line_"+ x ] as TextField ).textColor = textColor;
				( page2[ "btnselect_" + x ] as SimpleButton ).visible = true;
			}	
		}
		
		private function btn_vipclick( e:MouseEvent ):void
		{
			if( !GameCommonData.GameInstance.GameUI.contains( buyPanel ) )
			{
				GameCommonData.GameInstance.GameUI.addChild( buyPanel );
				buyPanel.x = panelBase.x +panelBase.width +10;
				buyPanel.y = panelBase.y;
				
				if( buyPanel.x > ( 1000 - buyPanel.width ) )
				{
					buyPanel.x = panelBase.x - buyPanel.width - 10;
				}
				
				if( buyPanel.y > ( 580 - buyPanel.height ) )
				{
					buyPanel.y = panelBase.y - buyPanel.height;
				}
				
			}
		}
		
		private function buy_vip_click( e:MouseEvent ):void
		{
			
			var name:String;
			var info:String;
			if( e.target is MovieClip && e.target.id )
			{
				var str:String = e.target.id
				var vip_id:uint;
				switch( str )
				{
					case "btn_buy1":
					vip_id = 501032;
						break;
					case "btn_buy2":
					vip_id = 501011;
						break;
					case "btn_buy3":
					vip_id = 501013;
						break;
					default:
						break;
				}
				if( vip_id )
				{
					price = getPrice( vip_id );
//					name = String( UIConstData.getItem( vip_id ).Name );
//					info = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+ price +'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+ "1" +'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+ name +'</font>';
//					UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:"提 示",comfirmTxt:"确 定",cancelTxt:"取 消"});
					vipType = vip_id;
					if( this.price <= GameCommonData.Player.Role.UnBindRMB )
					{
						UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( MarketEvent.BUY_ITEM_MARKET,{type: this.vipType,count: 1} );
					}else
					{
						info = GameCommonData.wordDic[ "mod_vip_vip_abc_6" ];	//"所需元宝不足，请充值后购买";
						UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "often_used_tip" ],
						 comfirmTxt:GameCommonData.wordDic[ "mod_vip_vip_abc_13" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel2" ]});	//提示	快速充值	取 消
					}
				}
			}
		}
		
		private function commitHandler():void
		{
			sendNotification(TerraceController.NAME , "pay");
		}
		
		private function cancelClose():void
		{
			
		}
		
		private function getPrice( type:* ):int
		{
			var price:int;
			for( var i:uint=0; i<UIConstData.MarketGoodList.length; i++ )
			{
				var obj:Array = UIConstData.MarketGoodList[i];
				for( var j:uint =0;j < obj.length ; j++ )
				{
				if ( int( obj[j].type ) == int( type ) )
				{
					price = obj[j].PriceIn;
					return price;
				}
				}
			}
			return price;
		}
		
		/* 根据vip类型设置颜色 */
		private function judgmentColor(num:int):uint
		{
			var ap:uint;
			switch( num )
			{
				case 0:
				ap = 0xE2CCA5;
					break;
				case 1:
				ap = 0x0097FF; 
					break;
				case 2:
				ap = 0x7A3FE9;
					break;
				case 3:
				ap = 0xFF642F;
					break;
				case 4:
				ap = 0x00FF00;
					break;
				default:
				ap = 0xFFFFFF;
					break;
			}
			return ap;	
		}
		
		private function p1click( e:MouseEvent ):void
		{
			if( p==0 )
			{
				return;
			}else
			{
				( view.p1 as MovieClip ).gotoAndStop( 1 );
				( view.p2 as MovieClip ).gotoAndStop( 2 );
				page1.visible = true;
				page2.visible = false;
				p = 0;
				set_page1();
			}
		}
		
		private function p2click( e:MouseEvent ):void
		{
			if( p==1 )
			{
				return;
			}else
			{
				( view.p1 as MovieClip ).gotoAndStop( 2 );
				( view.p2 as MovieClip ).gotoAndStop( 1 );
				page1.visible = false;
				page2.visible = true;
				p = 1;
				send( pagePos );
			}
		}
		
		private function closeHandler( e:Event = null ):void
		{
			if( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBaseX = panelBase.x;
				panelBaseY = panelBase.y;
				remove_p1_Adl();
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				VipShowData.IsVipShowOpen = false;
			}
		}
		
		private function buyPanelcloseHandler( e:Event = null ):void
		{
			if( GameCommonData.GameInstance.GameUI.contains( buyPanel ) )
			{
				remove_p2_Adl();
				GameCommonData.GameInstance.GameUI.removeChild( buyPanel );
			}
		}
		private function remove_p1_Adl():void
		{
			if( view && ( view.p1 as MovieClip ).hasEventListener( MouseEvent.CLICK ) )
				{
					( view.p1 as MovieClip ).removeEventListener( MouseEvent.CLICK , p1click );
				}
			if( view && ( view.p2 as MovieClip ).hasEventListener( MouseEvent.CLICK ) )
				{
					( view.p2 as MovieClip ).removeEventListener( MouseEvent.CLICK, p2click );
				}
			if( page1 && ( page1.btn_vip as SimpleButton ).hasEventListener( MouseEvent.CLICK ) )
				{
					( page1.btn_vip as SimpleButton ).removeEventListener( MouseEvent.CLICK, btn_vipclick );
				}
		}
		
		private function remove_p2_Adl():void
		{
			if( buyPanel && buy_vip.hasEventListener( MouseEvent.CLICK ) )
			{
				buy_vip.removeEventListener( MouseEvent.CLICK , buy_vip_click );
			}
		}
		
		private function prevclick( e:MouseEvent ):void
		{
			if( pagePos == 1 || pagePos == 0 )
			{
				return;
			}else
			{
				pagePos--;
				send( pagePos );
//				setpage2();
			}
		}
		
		private function nextclick( e:MouseEvent ):void
		{
			if( pagePos >= pageNum  )
			{
				return;
			}else
			{
				pagePos++;
				send( pagePos );
//				setpage2();
			}
		}
		
		private function page2click( e:MouseEvent ):void
		{
			if( e.target is SimpleButton)
			{
				var btnArr:Array =  ( e.target.name as String ).split( "_" );
				var _num:uint = btnArr[ 1 ];
				if( _num == 14 || _num == 15 )
				{
					return;
				}else
				{
					var sendname:String = arr[ _num ].name;
					if( !facade.hasMediator( QuickSelectMediator.NAME ) )
					{
						facade.registerMediator( new QuickSelectMediator() );	
					}
				
					sendNotification( ChatEvents.SHOWQUICKOPERATOR, sendname );
				}
			}
		}
		
		private function drawMc(na:int):MovieClip
		{
			var mc:MovieClip = new MovieClip;
			var sp:Shape =new Shape();
			sp.graphics.beginFill( 0x000000, 0 );
			sp.graphics.drawRect( 0, 0, 38, 38);
			sp.graphics.endFill();
			mc.addChild( sp );
			mc.name = "TaskEqu_" + String( na );
			return mc;
		}
		
		private function getVipTime(time:String):String
		{
			var ttxt:String
			var t:int = time.indexOf( GameCommonData.wordDic[ "mod_vip_vip_abc_7" ] );	//天
			if( t !== -1 )
			{
				var tArr:Array = time.split( GameCommonData.wordDic[ "mod_vip_vip_abc_7" ] );	//天
				ttxt = String( int(tArr[ 0 ]) + int( 1 ) ) + GameCommonData.wordDic[ "mod_vip_vip_abc_7" ]; //天
			}else
			{
				var ti:int = time.indexOf( GameCommonData.wordDic[ "mod_vip_vip_abc_8" ] );	//时
				if( ti !== -1 )
				{
					var tiArr:Array = time.split( GameCommonData.wordDic[ "mod_vip_vip_abc_8" ] );			//时
					ttxt = String( int(tiArr[ 0 ]) + int( 1 ) ) + GameCommonData.wordDic[ "mod_vip_vip_abc_8" ];   //时
				}else
				{
					ttxt = time ;
				}	
			}
			return ttxt;
		}
		
		private function add_set( obj:Object ):void
		{
			arr = obj.array;
			pageNum = obj.nPage;
			if( obj.nPage == 0 )
			{
				pagePos = 0;
			}
			if( VipShowData.IsVipShowOpen )
			{
				setpage2();
			}
		}
		
		private function set_viptext( vipNum:int ):void
		{
			var txt:String;
			switch( vipNum )
			{
				case 0:
					txt = GameCommonData.wordDic[ "mod_vip_vip_abc_1" ];		//"您不是VIP玩家";
				break;
				case 1:
					txt = GameCommonData.wordDic[ "mod_vip_vip_abc_9" ] + timetxt; 		//"月卡VIP剩余："
				break;
				case 2:
					txt = GameCommonData.wordDic[ "mod_vip_vip_abc_10" ] + timetxt;		//"季卡VIP剩余："
				break;
				case 3:
					txt = GameCommonData.wordDic[ "mod_vip_vip_abc_11" ] + timetxt;		//"半年卡VIP剩余："
				break;
				case 4:
					txt = GameCommonData.wordDic[ "mod_vip_vip_abc_12" ] + timetxt;		//"周卡VIP剩余："
				break;
				default:
					txt = GameCommonData.wordDic[ "mod_vip_vip_abc_1" ];				// "您不是VIP玩家";
				break;
			}
			( page1.vip_text as TextField ).text = txt;
		}
		
		private function send( n:int ):void
		{
			var obj:Object = new Object();
				obj.action = int( 1 );
				obj.pageIndex = pagePos;
				obj.amount = int( 13 );
				obj.memID = int( 0 );
				VipListSend.sendVipListAction( obj );
		}
		
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

	}
}