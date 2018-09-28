package GameUI.Modules.Task.View
{
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Task.Commamd.MoveToCommon;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	import GameUI.View.Components.UISprite;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.BlendMode;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	import flash.utils.setTimeout;

	public class TaskText extends UISprite
	{
		protected var tf:TextField;
		private var timer:Timer;
		private var mapId:int;
		private var tileX:int;
		private var tileY:int;
		private var taskId:int;
		private var npcId:int;
		
		public const iconDic:Dictionary=new Dictionary();
		
		public function TaskText(w:Number=215)
		{
			super();
			this.width=w
			iconDic["fx"]=true;
			this.tf=new TextField();
			this.tf.wordWrap=true;
			this.tf.condenseWhite=false;
			this.tf.filters =Font.Stroke(0);
			this.tf.multiline=true;
			this.tf.type=TextFieldType.DYNAMIC;
			this.tf.selectable=false;
			this.tf.autoSize=TextFieldAutoSize.LEFT;
			this.tf.width=w;
			this.tf.addEventListener(MouseEvent.ROLL_OVER,onTextRollOverHandler);
			this.tf.addEventListener(MouseEvent.ROLL_OUT,onTextRollOutHandler);
			this.tf.mouseWheelEnabled=false;
			this.tf.defaultTextFormat = textFormat();
			this.addChildAt(tf,1);
		}
		
		protected function onTextRollOverHandler(e:MouseEvent):void{
			SysCursor.GetInstance().showSystemCursor();
		}
		
		protected function onTextRollOutHandler(e:MouseEvent):void{
			SysCursor.GetInstance().revert();
		}
		
		
		public function set tfText(value:String):void{
			if(this.tf.htmlText==value)return;
			this.removeAll();
//			if ( GameCommonData.wordVersion == 2 )
//			{
//				var regExp:RegExp = /\\fx/g;
//				value = value.replace( regExp , "\\fx  " );
//			}
			this.tf.htmlText=value;
			this.tf.addEventListener(TextEvent.LINK,onClickLink);
			this.showIcon();
			this.height=this.tf.height;
			this.width=this.tf.width;
//			this.tf.width = this.tf.textWidth + 5;
//			this.tf.mouseEnabled = false;
		}
		
		
		/**
		 *  
		 * @param e
		 * 
		 */		
		protected function onClickLink(e:TextEvent):void{
			//[1001,75,75,101,116]
			//[sceneId,titleX,titleY,taksId,npcId]
			var arr:Array=e.text.split(",");
			if(arr.length!=5)return;
			mapId = arr[0];
			tileX = arr[1];
			tileY = arr[2];
			taskId = arr[3];
			npcId = arr[4];
			
			if( 99999 == mapId )
			{
				UIUtils.firstTreasure();
				return;
			}
			
//			if( NewerHelpData.CAN_FLY && arr[0] == 1001 && arr[1] == 77 && arr[2] == 88 && arr[3] == 303 && arr[4] == 116 )
//			{
//				var obj:Object = NewerHelpData.changeId( mapId, npcId );
//				MoveToCommon.sendFlyToCommand({mapId:obj._mapId,tileX:78,tileY:92,taskId:302,npcId:obj._npcId});    // 使用小飞鞋去找洛神
//				timer = new Timer(500, 1);
//				timer.addEventListener(TimerEvent.TIMER, timer_Fun);
//				timer.start();
//				NewerHelpData.CAN_FLY = false;
//				return;
//			}
//			if( NewerHelpData.CAN_FLY && arr[0] == 1002 && arr[1] == 43 && arr[2] == 102 && arr[3] == 303 && arr[4] == -1)
//			{
//				var object:Object = NewerHelpData.changeId( mapId, npcId );
//				MoveToCommon.sendFlyToCommand({mapId:object._mapId,tileX:arr[1],tileY:arr[2],taskId:arr[3],npcId:arr[4]});    // 使用小飞鞋去找兔子
//				NewerHelpData.CAN_FLY = false;
//				return;
//			} 
			
			//不同线路不可寻路过去
			if ( ( int(npcId) >= 9046 && int(npcId) <= 9050 ) || ( int(npcId) >= -9030 && int(npcId) <= -9001 )   ) 
			{
				if ( GameConfigData.GameSocketName != GameConfigData.specialLineName )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_TaskText_onClickLink_1" ]);//"该任务只有在帮派里才可寻路" ); 
					return;
				}
			}
			else
			{
//				trace ( GameCommonData.GameInstance.GameScene.GetGameScene.MapName );
				if ( GameConfigData.GameSocketName == GameConfigData.specialLineName )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_TaskText_onClickLink_1" ]);//"帮派内无法寻路出去" );
					return;
				}
			}
			
			MoveToCommon.MoveTo(arr[0],arr[1],arr[2],arr[3],arr[4]);	
		}
		
		private function timer_Fun(event:TimerEvent):void
		{
			timer.removeEventListener(TimerEvent.TIMER, timer_Fun);
			timer = null;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification(TaskCommandList.RECEIVE_TASK,{id:303});		
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( NewerHelpEvent.SHOW_NEWER_HELP, 2 );
		} 
		
		protected function removeAll():void{
			while(this.numChildren>2){
				this.removeChildAt(2);
			}
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.leading=5;
			tf.size = 12;
			tf.font = "宋体";            //宋体
			return tf;
		}
		
		protected function showIcon():void{
			var regExp:RegExp = /(\\[a-z]{2})/g;
			var chatArr:Array=tf.text.split(regExp);
			var chatHtmlArr:Array=tf.htmlText.split(regExp);
			
			if (!chatArr||chatArr.length==0) 
			{
				return;
			}
			var index:int = 0;
			var pos:int = 0;
			var posHtml:int=0;
			while (index<chatArr.length) 
			{
				if (regExp.test(chatArr[index])) 
				{	
					
					if(!isIcon(chatArr[index].slice(1,3))) return;
					var mc:MovieClip=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(chatArr[index].slice(1,3));	
					mc.buttonMode=true;
					if(tf.getCharBoundaries(pos)==null){
						setTimeout(showIcon,50);
						return;
					}
					
					var tempStr:String=tf.htmlText.substring(posHtml-50,posHtml);
					var eventPosStart:int=tempStr.indexOf("event");
					var eventPosEnd:int=tempStr.indexOf("TARGET");
					if(eventPosStart!=-1 && eventPosEnd!=-1 ){
						tempStr=tempStr.substring(eventPosStart+6,eventPosEnd-2);
						mc["taskEvent"]=tempStr;
						mc.addEventListener(MouseEvent.CLICK,onShooseClickHandler);
						
					}
				
					mc.x = int(tf.getCharBoundaries(pos).x + tf.x+1);
					mc.y = int(tf.getCharBoundaries(pos).y + tf.y - 3);
					mc.name = chatArr[index].slice(1,3);
					this.blendMode = BlendMode.LAYER;
					setMcMask(mc);
					this.addChild(mc);
					posHtml+=chatHtmlArr[index].length;
					pos = pos + chatArr[index].length;
					
				} else {
					posHtml+=chatHtmlArr[index].length;
					pos = pos + chatArr[index].length;
				}
				index++;
			}
		}		
		
		private function onShooseClickHandler(e:MouseEvent):void{
			var str:String=e.currentTarget["taskEvent"];
			if(str==null || str.length<1){
				return;
			}
			
			var arr:Array=str.split(",");
			if(arr.length!=5)return;
			MoveToCommon.FlyTo(arr[0],arr[1],arr[2],arr[3],arr[4]);
		}
		
		/**
		 * 检测这种特殊的字符串是否为图标 
		 * @param str
		 * @return 
		 * 
		 */		
		protected function isIcon(str:String):Boolean{
			if(iconDic[str]){
				return true;
			}
			return false;
		}
		
		protected function setMcMask(mc:MovieClip):void{
			var s:Shape = new Shape();
			s.graphics.beginFill(0xffffff, 1);
			s.graphics.drawRect(mc.x-1, mc.y, mc.width+2, mc.height+1);
			s.graphics.endFill();
			s.blendMode = BlendMode.ERASE;
			this.addChild(s);
		}
		
	}
}