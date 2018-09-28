package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.UIConfigData;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class ScrollNoticeMediator extends Mediator implements IUpdateable
	{
		public static const NAME:String = "ScrollNoticeMediator";
		private var xSpeed:Number;
		private var txt:TextField;
		private var format:TextFormat = null;
		private var rSpeed:Number;
//		private var timer:Timer = new Timer();
		private var isReceive:Boolean = false;
		private var aMsg:Array = [];																//消息队列
		private var numMsg:uint = 0;
		public static const REG:RegExp = /(<.*?>)/g;
		
		public function ScrollNoticeMediator()
		{
			super(NAME);
		}
		
		public function get scrollView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				ChatData.OPENSCROLLNOTICE,
				ChatData.USE_BIG_LEO
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SCROLLNOTICEUI});
					break;
					
				case ChatData.OPENSCROLLNOTICE:
					var str:String = notification.getBody().info;
					var obj:Object = new Object();
					var hasBigLeo:Boolean = false;
					obj.nAtt = notification.getBody().nAtt;
					obj.info = str;
					isReceive = true;
					if(!ChatData.isGun)
					{
						aMsg.push(obj);
						initUI();
					}
					else
					{
						for ( var i:uint=0; i<aMsg.length; i++ )
						{
							if ( aMsg[i].nAtt == 2039 )
							{
								aMsg.splice(i,0,obj);
								hasBigLeo = true;
								break;
							}
						}
						if ( !hasBigLeo )
						{
							aMsg.push(obj);
						}
					}
					break;

				case ChatData.USE_BIG_LEO:
					var _obj:Object = new Object();
					_obj.nAtt = notification.getBody().nAtt;
					_obj.info = notification.getBody().info;
//					trace( "notification.getBody().info" +notification.getBody().info +"  _obj.info: " +_obj.info );
					useBigLeo( _obj );										//使用大喇叭
					isReceive = true;
					break;
			}
		}
		
		private function cutBigLeoStr(s:String):String 
		{
			var name:String;
			var arr:Array = s.split(REG);
			for(var i:int = 0; i < arr.length; i++) 
			{
				if(arr[i].indexOf("<") >= 0) 
				{
					if ( arr[i].split("_")[0] == "<0" )
					{
						name = arr[i].split("_")[1];
					}
				}
			}
			var newStr:String = name + "："+arr[arr.length-1];
			return newStr;
		}
		
		private function cutStr(s:String):String 
		{
			var arr:Array = s.split(REG);
			for(var i:int = 0; i < arr.length; i++) {
				if(arr[i].indexOf("<") >= 0) {
					arr[i] = arr[i].split("_")[1];
				}
			}
			return arr.join("");
		}
		
		public function Update(gameTime:GameTime):void
		{
//			if ( this.timer.IsNextTime(gameTime) )
//			{
				if(scrollView.alpha <= 0)
				{
					if(scrollView && GameCommonData.GameInstance.GameUI.contains(scrollView))
					{
						GameCommonData.GameInstance.GameUI.removeChild(scrollView);
						GameCommonData.GameInstance.GameUI.Elements.Remove(this);
					}
					aMsg = [];
					numMsg = 0;
					ChatData.isGun = false;
					isReceive = false;
					return;
				}
				else
				{
					moveScroll();
				}
//			}
		}
		
		private function moveScroll():void
		{
			scrollView.alpha -= rSpeed;
			if(txt == null)
			{
				if(numMsg >= aMsg.length)
				{
					rSpeed = .07;
				}
				return;
			}
			txt.x -= 1;
	
			if(txt.x < scrollView.x - txt.width + 10)
			{
				if(txt && GameCommonData.GameInstance.GameUI.contains(txt))
				{
					GameCommonData.GameInstance.GameUI.removeChild(txt);
					txt = null;
				}					
				checkNum();
			}
		}
		
		private function initUI():void
		{
//			timer.DistanceTime = 50;				 			
			rSpeed 			   = 0;
			scrollView.mouseChildren = false;
			scrollView.mouseEnabled  = false;
			
			format 		 = new TextFormat();
			format.size  = 15;
			format.font = GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_getI" ];//"宋体"
			
			if(aMsg.length > 0)
			{
				if(GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth)
			    {
			    	scrollView.x = 210;
			    }
			    if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
			    {
			    	scrollView.x = 210 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth-3)/2;
		        }
				scrollView.y = 90;
				scrollView.alpha = 1;
				GameCommonData.GameInstance.GameUI.addChild(scrollView);
				getInfo();
				GameCommonData.GameInstance.GameUI.Elements.Add(this);
			}
		}
		
		private function getInfo():void
		{
			if(!txt)
			{
				txt = new TextField();
				txt.mouseEnabled = false;
			}
			txt.x = scrollView.x +scrollView.width - 7;
			txt.y = scrollView.y +5;
			txt.defaultTextFormat = format;
			txt.autoSize = TextFieldAutoSize.LEFT;
			txt.cacheAsBitmap = true;
			txt.mask = scrollView.mask_mc;
			
			var infoObj:Object = aMsg.shift();
			if ( infoObj.nAtt == 2039 )
			{
				txt.textColor = 0x66ffff;
				txt.text = cutBigLeoStr( infoObj.info );
//				txt.text = cutStr( infoObj.info );
			}
			else
			{
				txt.textColor = 0xfdf291;
				txt.text = cutStr( infoObj.info );
			}
			
			GameCommonData.GameInstance.GameUI.addChild(txt);
			ChatData.isGun = true;
		}
		
		private function checkNum():void
		{
			if( aMsg.length>0 )
			{
				getInfo();
			}else
			{
				rSpeed = .07;
			}
		}
		
		private function useBigLeo(obj:Object):void
		{
			if ( aMsg.length == 0 && !ChatData.isGun )
			{
				aMsg.push(obj);
				initUI();
			}
			else
			{
				aMsg.push(obj);
			}
		}
		
		private function clearView():void
		{
			if(scrollView.numChildren > 0)
			{
				for(var i:int=scrollView.numChildren;i>=0;i--)
				{
					scrollView.removeChildAt(0);
				}
			}
		}
		
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
        public function get Enabled():Boolean
		{
			return true;
		}
	}
}