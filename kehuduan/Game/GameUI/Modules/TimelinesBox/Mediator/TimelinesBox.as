package GameUI.Modules.TimelinesBox.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.TimelinesBox.Data.TimelinesBoxData;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.ColorMatrixFilter;
	
	import org.puremvc.as3.multicore.interfaces.IMediator;
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	
	public class TimelinesBox extends Mediator implements IMediator
	{	
		public static var NAME:String = "TimelinesBox";
		private var view:MovieClip;
		/**
		 * mod_tim_med_stone_1  一元石
		 * mod_tim_med_stone_2  两仪石
		 * mod_tim_med_stone_3  三才石
		 * mod_tim_med_stone_4  四象石
		 */
		 
		 /**
		 * mod_tim_med_art_1 玉风清音
		 * mod_tim_med_art_2 震雷霹雳
		 * mod_tim_med_art_3 泽水归元
		 * mod_tim_med_art_4 离火燎天
		 */
		private var stoneArray:Array =[ GameCommonData.wordDic[ "mod_tim_med_stone_1" ], GameCommonData.wordDic[ "mod_tim_med_stone_2" ], GameCommonData.wordDic[ "mod_tim_med_stone_3" ], GameCommonData.wordDic[ "mod_tim_med_stone_4" ] ];
		private var artifactArray:Array = [ GameCommonData.wordDic[ "mod_tim_med_art_1" ], GameCommonData.wordDic[ "mod_tim_med_art_2" ], GameCommonData.wordDic[ "mod_tim_med_art_3" ], GameCommonData.wordDic[ "mod_tim_med_art_4" ] ];
		private var stoneArray3:Array = [ GameCommonData.wordDic[ "mod_tim_med_stone_1" ]+"*3", GameCommonData.wordDic[ "mod_tim_med_stone_2" ]+"*3", GameCommonData.wordDic[ "mod_tim_med_stone_3" ]+"*3", GameCommonData.wordDic[ "mod_tim_med_stone_4" ]+"*3" ];
		private var panelbase:PanelBase;
		private var grayFilter:ColorMatrixFilter;							//灰色滤镜
		private var mat:Array = new Array();
		private var levelArray1:Array = [ 0, 50, 79, 89 ];					//石头界限
		private var levelArray2:Array = [ 0, 50, 70, 85 ];					//神器材料界限
		private var IsStone:int;
		private var panelbaseX:int = UIConstData.DefaultPos2.x - 200;				//存储panlebase坐标
		private var panelbaseY:int = UIConstData.DefaultPos2.y ;				//存储panlebase坐标
		private var _obj:Object = new Object();
		
		public function TimelinesBox()
		{
			super( NAME );
		}
		
		override public function listNotificationInterests():Array
		{
			return [ 
				TimelinesBoxData.SHOW_TIMELINESBOX_ARTIFACT,
				TimelinesBoxData.SHOW_TIMELINESBOX_STONE,
				TimelinesBoxData.SHOW_TIMELINESBOX_ARTIFACT3
			 ];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			_obj = notification.getBody() as Object;
			switch( notification.getName() )
			{
				case TimelinesBoxData.SHOW_TIMELINESBOX_ARTIFACT:
					IsStone = 2;
					showView( artifactArray, levelArray2 );
					break;
				
				case TimelinesBoxData.SHOW_TIMELINESBOX_STONE:
					IsStone = 0;
					showView( stoneArray, levelArray1 );
					break;
				case TimelinesBoxData.SHOW_TIMELINESBOX_ARTIFACT3:
					IsStone = 1;
					showView( stoneArray3, levelArray1 );
				default:
					break;
			}
		}
		
		private function showView( array:Array, levelArray:Array ):void
		{
			if( panelbase && GameCommonData.GameInstance.GameUI.contains( panelbase ))
			{
				panelbaseX = panelbase.x;
				panelbaseY = panelbase.y;
				GameCommonData.GameInstance.GameUI.removeChild( panelbase );
			}
			
			if( panelbase )
			{
				panelbase = null;
			}
			
			if( view )
			{
				view = null;
			}
			
			view = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip( "Timelinesbox" );
			view.mouseEnabled = false ;
			panelbase = new PanelBase( view, view.width, view.height );
			GameCommonData.GameInstance.GameUI.addChild( panelbase );
			panelbase.addEventListener( Event.CLOSE, panelCloseHandler);
			panelbase.x = panelbaseX;
			panelbase.y = panelbaseY;
			mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // red
            mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // green
            mat = mat.concat([0.3086, 0.6094, 0.082, 0, 0]);                                                     // blue
            mat = mat.concat([0, 0, 0, 1, 0]);                                                                  // alpha
            grayFilter=new ColorMatrixFilter(mat);
            
			for( var i:int =0; i < array.length; i++ )
			{
				var btn:MovieClip = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "TimelinesboxButton" );
					btn.txt.text = array[i];
					btn.id = i + 1;
					btn.txt.mouseEnabled = false;
					btn.txt.selectable = false;
					if( GameCommonData.Player.Role.Level > levelArray[i] )
					{
						btn.buttonMode = true;
					}else
					{
						btn.buttonMode = false;
						btn.mouseEnabled = false;
						btn.filters = [ grayFilter ];
					}
					btn.x = 37;
					btn.y = 49+ 34 * i;
					view.addChild( btn );
			}
			
			view.addEventListener( MouseEvent.CLICK, viewclick );
		}
		
		private function viewclick( e:MouseEvent ):void
		{
			e.stopPropagation();
			var n:String;
			if ( e.target.id )
			{
				switch( IsStone )
				{
					case 0:
						n = "10" + String( e.target.id );
						break;
					case 1:
						n = "20" + String( e.target.id );
						break;
					case 2:
						n = "30" + String( e.target.id );
						break;
					default:
						break;		
				}
				requestSerInfo( int( n ) );
				panelCloseHandler();
			}
		}
		
		private function requestSerInfo(num:Number):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push( 0 );
			parm.push(GameCommonData.Player.Role.Id);
			parm.push( 0 );
			parm.push( 0 );
			parm.push( 0 );
			if( _obj.nNPC !== null)
			{
				parm.push( _obj.nNPC );
			}else
			{
				parm.push( 0 );
			}
			parm.push( 313 );							//进入地图
			parm.push( num );
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction( obj );
		}
		
		private function panelCloseHandler( e:Event=null ):void
		{
			panelbaseX = panelbase.x;
			panelbaseY = panelbase.y;
			if( panelbase.hasEventListener( Event.CLOSE ) )
			{
				panelbase.removeEventListener( Event.CLOSE, panelCloseHandler);
			}
			
			if( view.hasEventListener( MouseEvent.CLICK ) )
			{
				view.removeEventListener( MouseEvent.CLICK, viewclick );
			}
			
			if( GameCommonData.GameInstance.GameUI.contains( panelbase ) )
			{
				GameCommonData.GameInstance.GameUI.removeChild( panelbase );
			}
			panelbase = null;
			view = null;
		}

	}
}