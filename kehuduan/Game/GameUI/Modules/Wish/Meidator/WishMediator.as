package GameUI.Modules.Wish.Meidator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Modules.Wish.Data.WishData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.ResourcesFactory;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.ColorMatrixFilter;
	import flash.filters.GlowFilter;
	import flash.net.URLRequest;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.interfaces.IMediator;
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	public class WishMediator extends Mediator implements IMediator
	{		
		public static const NAME:String="WishMediator";
		private const WishGridCoun:int=25;                                	//格子数量总数
		private var faceNum:int=0;
		private var wishView:MovieClip;                                      //wish界面
		private var receiveView:MovieClip;									//显示接收礼物的界面
		private var wishIsOpen:Boolean=false;								//判断wish界面是否打开
	    private var grayFilter:ColorMatrixFilter;                           //灰色滤镜，用于将按钮变灰色
	    private var yellowFilter:GlowFilter;                                //黄色框
	    private var redFilter:GlowFilter;                                   //红色框
	    private var mat:Array=new Array();                                  //滤镜颜色控制数组
	    private var srcArray:Array=new Array();                             //加载Face图片数组
	    private var faceArray:Array=new Array();                            //储存已加载Face图片数组
	    private var btn:MovieClip;                       					//"接收回礼"按钮
	    private var txt_accept:TextField									//"接收回礼"按钮
	    private var clickCoun:int;											//剩余可点击次数
	    private var panelBase:PanelBase=null;								//wishView的背景界面
	    private var panelBase2:PanelBase=null;								//receiveView的背景界面

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
		private var con:uint;												//朵数
		public function WishMediator()
		{
			super(NAME);
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				WishData.WISH_OPEN,
				WishData.RETURN_CLICK_GRID,
				WishData.OPEN_RETURN_GRID,
				WishData.SEND_310,
				EventList.CLOSE_NPC_ALL_PANEL
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case WishData.WISH_OPEN:
					if(wishIsOpen)break;
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
//				    srcArray=ranArray(srcArray);									//随机数组排列
					wishView=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Wishing");  //获取Wishing界面
					on_Complete();				
					break;
				case WishData.RETURN_CLICK_GRID:                                                                            //要加载的图片最好是Bitmap,不然可能会出错
					if(!wishIsOpen)break;
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
				break;
				case WishData.OPEN_RETURN_GRID:
					if(!wishIsOpen)break;
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
				
//						textData.sortOn("num",Array.NUMERIC);
						if( GameCommonData.GameInstance.GameUI.contains( panelBase ) )
						{ 
							btn.removeEventListener(MouseEvent.CLICK,btnclick);
							GameCommonData.GameInstance.GameUI.removeChild( panelBase );
							wishIsOpen=false;
						}
        				
						receiveView=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NPCChat");
						panelBase2 = new PanelBase(receiveView, receiveView.width+10, receiveView.height+12);
						panelBase2.addEventListener(Event.CLOSE, panelCloseHandler2);
						panelBase2.x = UIConstData.DefaultPos1.x;
						panelBase2.y = UIConstData.DefaultPos1.y;
						panelBase2.SetTitleTxt( GameCommonData.wordDic[ "mod_wis_med_wis_12" ] );				//"接收回礼"
						receive_Text(textData);
						GameCommonData.GameInstance.GameUI.addChild( panelBase2 );  
				break;
				case WishData.SEND_310:
						sendNotification( PrepaidUIData.CLOSE_PREPAID_VIEW );
						requestSerInfo(310);                           //发送接收礼物信息
				break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					if ( wishIsOpen )
					{
				  		panelCloseHandler();
					}
				break;
				default:
				break;
			}
		}
		
		private function onProgress(e:Event):void
		{
			LoadingView.getInstance().showLoading();
		}
		
		private function on_Complete(e:Event=null):void
		{
			panelBase = new PanelBase(wishView, wishView.width, wishView.height+15);
			wishView.x=22;
			for(var x:int = 0 ;x < WishGridCoun ;x++ )
			{
				var bitmapdata:BitmapData=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("qMark");
				var bitmap:Bitmap=new Bitmap(bitmapdata);
				var bmc:MovieClip=new MovieClip();
				bmc.addChild(bitmap);
				var _x:int= x % 5;
				var _y:int= Math.floor( x/5 );
				bmc.x= 13 + 58 * _x;
				bmc.y= 63 + 58 * _y;
				bmc.name="bitmap_"+x;
				wishView.addChild(bmc);
			}
			panelBase.name = "WishWindows";
			panelBase.addEventListener( Event.CLOSE, panelCloseHandler);
			wishIsOpen = true;
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_wis_med_wis_13" ] );				//"许愿祝福"
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			for(var dx: int = 0; dx < WishGridCoun ; dx++)
			{
				var loader:Loader=new Loader();
				var r:URLRequest=new URLRequest();
				r.url = GameCommonData.GameInstance.Content.RootDirectory+"Resources/Face/"+srcArray[dx];
//				faceNum++;
				loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onComplete);
				loader.load(r);
			}
		}
		
		private function onComplete(e:Event):void
		{
			if( GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				faceNum++;
				var j:int=faceNum-1;
				faceArray[j]=e.target.content as Bitmap;
				var _name:String= "bitmap_" + j ;
				var bm:MovieClip=wishView.getChildByName(_name) as MovieClip ;
				var container:MovieClip=new MovieClip();
				container.addChild(faceArray[j]);
				container.x = bm.x - 9;
				container.y = bm.y - 9;
				wishView.addChild( container );
				container.name="face_"+String(j);
				wishView.removeChild(bm);
			}
			
//			if(faceNum<WishGridCoun)
//			{
//				var loader:Loader=new Loader();
//				var r:URLRequest=new URLRequest();
//				loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onComplete);
//				r.url=GameCommonData.GameInstance.Content.RootDirectory+"Resources/Face/"+srcArray[faceNum];
//				loader.load(r);	
//			
//			}
			if(faceNum==WishGridCoun)
			{
//				for(var _dx:int=0;_dx< WishGridCoun ; _dx++ )
//				{
//					var _name:String="bitmap_"+_dx;
//					var bm:MovieClip=wishView.getChildByName(_name) as MovieClip ;
//					wishView.removeChild(bm);
//				}
//				e.target.loader.unload();
//				for(var b:int=0;b<faceArray.length;b++)
//				{
//					var dx:int=b%5;
//					var dy:int=Math.floor(b/5);
//					var container:MovieClip=new MovieClip();
//					container.addChild(faceArray[b]);
//					wishView.addChild(container);
//					container.x=4+dx*58;
//					container.y=54+dy*58;
//					container.name="face_"+String(b);
//				}

				
				mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // red
            	mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // green
            	mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // blue
            	mat = mat.concat([0, 0, 0, 1, 0]);                                                                  // alpha
            	grayFilter=new ColorMatrixFilter(mat);
            	clickCoun=3;                                                                                       //可点击次数

            	faceAddLis(wishView);
            	wishIsOpen=true;
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
			btn=wishView.getChildByName("btn_accept") as MovieClip;
			txt_accept=btn.getChildByName("txt_accept") as TextField;
			txt_accept.mouseEnabled= false;
			btn.filters=[grayFilter];                                                                               //运用滤镜，将“接受回礼”颜色变成灰色
		}
	/* * MOUSE_DOWN和MOUSE_UP。
	 * 当MOUSE_OVER时，给格子加载红色框；MOUSE_OUT时,检查格子是否加载了红色框或黄色框，如果有，则移除对应的红色框或黄色框；
	 * MOUSE_DOWN的时候，移除红色框并加载黄色框；MOUSE_UP的时候，检查格子是否加载了黄色框。如果没有，则不发生任何事件。如果有，
	 * 则移除对应格子。并sendNotification。这样设置，那么玩家按下鼠标，然后移动鼠标到其他格子，那么不会打开玩家MOUSE_UP时候，
	 * 鼠标所在的格子。只有DOWN和UP是同一个格子才打开格子 */
	 
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
						sendNotification( PrepaidUIData.CLOSE_PREPAID_VIEW );
						requestSerInfo(311);                                               
						MoviePosition={name:(e.currentTarget as MovieClip).name};
					}
				}                                                                                    
				if(clickCoun==0)
				{
					if(btn.filters!==null)btn.filters=null;                                             //将btn按钮的滤镜去除，恢复btn原有的色彩
					if(!btn.hasEventListener(MouseEvent.CLICK))
					{
						wishView.setChildIndex(btn,wishView.numChildren-1);
			    		btn.addEventListener(MouseEvent.CLICK,btnclick);
					}
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
			sendNotification( PrepaidUIData.CLOSE_PREPAID_VIEW );
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
			for(var i:int=0;i<wishView.numChildren;i++)
			{
				var face:* = wishView.getChildAt( i );
				if( ( face is MovieClip ) && face.extra != undefined )
				{
					if(face.hasEventListener(MouseEvent.MOUSE_DOWN))
					{
						face.removeEventListener(MouseEvent.MOUSE_DOWN,facedown);
						face.removeEventListener(MouseEvent.MOUSE_UP,faceup);
						face.removeEventListener(MouseEvent.MOUSE_OVER,faceover);
						face.removeEventListener(MouseEvent.MOUSE_OUT,faceout);
					}
				}
				wishView.removeChild(face);
			}
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
		
		private function panelCloseHandler(e:Event = null):void
		{
//			btn.buttonMode=false;
			if(btn && btn.hasEventListener( MouseEvent.CLICK ))
			{	
			btn.removeEventListener(MouseEvent.CLICK,btnclick);
			}
			removeAll(wishView);
			if( GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
//				panelBase.removeEventListener( EventList.CLOSE_NPC_ALL_PANEL ,panelCloseHandler);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				
			}
			wishIsOpen=false;
			
		}
		
		private function panelCloseHandler2(e:Event):void
		{
			if( GameCommonData.GameInstance.GameUI.contains(panelBase2) )
			{
				panelBase2.removeEventListener(Event.CLOSE, panelCloseHandler);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase2);
			}
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
		
		/*接受礼物后，显示的对应文字*/
		private function receive_Text(arr:Array):void
		{
			var _arr:Array=new Array();
			var txt:TextField=new TextField();
			var txt2:TextField = new TextField();
			var TF:TextFormat=new TextFormat();
			var TF2:TextFormat =new TextFormat();
			var head:TextField= new TextField();
			head.text= GameCommonData.wordDic[ "mod_wis_med_wis_1" ]     						//"小小心愿，甜甜祝福。"
			txt.text= GameCommonData.wordDic[ "mod_wis_med_wis_2" ] +"\r";						//"恭喜你得到了以下礼品："
				_arr=arr.slice();
				_arr.sortOn("num",Array.NUMERIC);
				for(var j:int=0;j<_arr.length;j++)
				{
					switch(_arr[j].num)
					{
						case 1:

						txt.text = txt.text + "    " + GameCommonData.wordDic[ "mod_wis_med_wis_3" ] + _arr[j].name + "\r";					//"小份感谢："			
						
						break;
						case 2:

						txt.text = txt.text + "    " + GameCommonData.wordDic[ "mod_wis_med_wis_4" ] + _arr[j].name + "\r";					//"中份感谢："
						break;
						case 3:

						txt.text = txt.text + "    " + GameCommonData.wordDic[ "mod_wis_med_wis_5" ] + _arr[j].name + "\r";						//"大份回礼："
						break;
						case 4:

						txt.text = txt.text + "    " + GameCommonData.wordDic[ "mod_wis_med_wis_6" ] + _arr[j].name + "\r";						//"超级回礼："
						break;
//						case 5:
//
//						txt.text=txt.text+"    "+"大份回礼："+_arr[j].name+"\r";
//						break;
						default:
						break;
					}					
				}
				
				txt.text = txt.text + "    " + GameCommonData.wordDic[ "mod_wis_med_wis_7" ] + GameCommonData.wordDic[ "mod_wis_med_wis_8" ] + con + GameCommonData.wordDic[ "mod_wis_med_wis_9" ] + "\r";				//"活动回赠："  "许愿花"    "朵"
				txt2.text = "   " + GameCommonData.wordDic[ "mod_wis_med_wis_10" ] + day + GameCommonData.wordDic[ "mod_wis_med_wis_11" ] + "\r" ;     //"你已连续许愿了"    "天"
				txt.multiline= true;
				
				head.x=25;
				head.y=10;
				head.width=800;
				head.selectable= false ;
				
				TF.size=12;
				TF.font="宋体";
				TF.color=0xFFFFFF;
				TF.leading=5;
				
				TF2.size = 12;
				TF2.font = "宋体";
				TF2.color = 0x00FF00;
				TF2.leading = 5;
				txt.setTextFormat(TF);
				head.setTextFormat(TF);
				txt.x=20;
				txt.y=35;
				txt.width=800;
				txt.height=800;
				txt.selectable=false;
				
				txt2.selectable = false ;
				txt2.x = 25;
				txt2.width = 800 ;
				txt2.y = 140;
				txt2.setTextFormat( TF2 );
				
				receiveView.addChild(txt);
				receiveView.addChild(head);
				receiveView.addChild(txt2);
				receiveView.setChildIndex(txt,receiveView.numChildren-1);
				receiveView.setChildIndex(head,receiveView.numChildren-1);
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
		
	
	}
}