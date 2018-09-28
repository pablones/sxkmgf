package GameUI.Modules.Answer
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Answer.Const.AnswerConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.text.TextFieldAutoSize;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class AnswerMediator extends Mediator
	{
		public static const NAME:String = "AnswerMediator";
		private var dataProxy:DataProxy = null;
		
		private var question:String = null; 
		private var answer1:String = null;
		private var answer2:String = null;
		private var answer3:String = null;
		private var rightOne:uint;
		private var isRight:Boolean = false;
		private var reg:RegExp = /([1-9][0-9])|[1-9]/g;
		private var id:uint;
		
		public function AnswerMediator()
		{
			super(NAME);
		}
		
		public function get answerView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				AnswerConst.DISPLAYANSWER,
				EventList.CLOSE_ANSWER_VIEW
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.ANSWERPANEL});
					break;
					
				case AnswerConst.DISPLAYANSWER:
//					trace(" notification.getBody().id "+notification.getBody().id);
//					initData(notification.getBody().id);
//					if(true) 											//测试用
					
					if(notification.getBody().id == 999)
					{
						initSelData(1);
						initSelView();
						dataProxy.answerPanelIsOpen = true;
						return;
					}
					if(notification.getBody().id == 2){
						return ;
					}else if ( notification.getBody().id == 1)
					{
						sendMessage(1);
					}else
					{
						this.id = notification.getBody().id;
						initData(this.id);
						initView();
						dataProxy.answerPanelIsOpen = true;
					}
					break;
				
				case EventList.CLOSE_ANSWER_VIEW:
					panelCloseHandler();
					break;		
			}
		}
		
		private function initSelData(id:uint):void
		{
			var dic:Dictionary=GameCommonData.SelectQueDic;
			var obj:Object=dic[uint(id+100)] as Object;
			question = obj.Question;
			answer1 = obj.SA;
			answer2 = obj.SB;
		}
		
		
		
		private function initSelView():void
		{
			showPanel();
			for(var i:uint = 0;i < 2;i++)
			{
				answerView["answer_" +(i+1)].txt.autoSize = TextFieldAutoSize.LEFT;
				answerView["answer_" +(i+1)].mouseChildren = false;
				answerView["answer_" +(i+1)].txt.mouseEnabled = false;
				answerView["answer_" +(i+1)].txt.text = this["answer" +(i+1)].split("→")[0]; 
				answerView["answer_" +(i+1)].num_txt.textColor = 0xffffff;
				answerView["answer_" +(i+1)].txt.textColor = 0xffffff;
				
				answerView["answer_" +(i+1)].addEventListener(MouseEvent.MOUSE_OVER,overSelTxtHandler);
				answerView["answer_" +(i+1)].addEventListener(MouseEvent.MOUSE_OUT,outSelTxtHandler);
				answerView["answer_" +(i+1)].addEventListener(MouseEvent.CLICK,goNextQue);
			}
			answerView.answer_1.num_txt.text = "a、";
			answerView.answer_2.num_txt.text = "b、";
			answerView.answer_3.num_txt.text = "";
			answerView.answer_3.txt.text = "";
			answerView.answer_3.txt.mouseEnabled = false;
			answerView.content_txt.htmlText = question;
		}
		
		//新加的2个函数
		private function overSelTxtHandler(evt:MouseEvent):void
		{
			evt.target.txt.textColor = 0xFFCC00;
			evt.target.num_txt.textColor = 0xFFCC00;
		}
		
		private function outSelTxtHandler(evt:MouseEvent):void
		{
			evt.target.txt.textColor = 0xffffff;
			evt.target.num_txt.textColor = 0xffffff;
		}
		
		private function goNextQue(evt:MouseEvent):void
		{
			var _id:uint;
			switch(evt.target.name)
			{
				case "answer_1":
					_id = answer1.match(reg)[0];
					break;
					
				case "answer_2":
					_id = answer2.match(reg)[0];
					break;
					
				default:
					break;
			}
			if(_id != 0)
			{
				initSelData(_id);
				initSelView();
			}else
			{
				var goodJob:String = this["answer"+evt.target.name.split("_")[1]].split("→")[1];
				trace(this["answer"+evt.target.name.split("_")[1]].split("→")[1]);
				panelCloseHandler();
				dealAnswer(goodJob);
			}
		}
		
		private function dealAnswer(goodJob:String):void
		{
			switch(goodJob)
			{
				case GameCommonData.wordDic[ "often_used_qz" ]://"全真"
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureFun, info:"<font color = '#ffffff'>"+AnswerConst.aGoodJob[0]+"</font>", title:"心理测试", comfirmTxt:"我知道了" });
					break;
				case GameCommonData.wordDic[ "often_used_gb" ]://"丐帮"
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureFun, info:"<font color = '#ffffff'>"+AnswerConst.aGoodJob[1]+"</font>", title:"心理测试", comfirmTxt:"我知道了" });
					break;
				case GameCommonData.wordDic[ "often_used_sl" ]://"少林"
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureFun, info:"<font color = '#ffffff'>"+AnswerConst.aGoodJob[2]+"</font>",title:"心理测试",  comfirmTxt:"我知道了" });
					break;
				case GameCommonData.wordDic[ "often_used_em" ]://"峨眉"
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureFun, info:"<font color = '#ffffff'>"+AnswerConst.aGoodJob[3]+"</font>", title:"心理测试", comfirmTxt:"我知道了" });
					break;
				case GameCommonData.wordDic[ "often_used_dc" ]://"点苍"
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureFun, info:"<font color = '#ffffff'>"+AnswerConst.aGoodJob[4]+"</font>", title:"心理测试", comfirmTxt:"我知道了" });
					break;
				case GameCommonData.wordDic[ "often_used_tm" ]://"唐门"
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureFun, info:"<font color = '#ffffff'>"+AnswerConst.aGoodJob[5]+"</font>", title:"心理测试", comfirmTxt:"我知道了" });
					break;
			}
		}
		
		private function initData(id:uint):void
		{
			var dic:Dictionary=GameCommonData.QuestionDic;
			var obj:Object=dic[id] as Object;
			if(obj==null) return;
			question = obj.Question;
			answer1 = obj.Answer1;
			answer2 = obj.Answer2;
			answer3 = obj.Answer3;
			rightOne = obj.Right;
			isRight = false;
//			trace("question " +question," answer1 "+answer1,"  right one "+rightOne);
		}
		
		private function showPanel():void
		{
			if(!(GameCommonData.GameInstance.GameUI.contains(answerView)))
			{
				GameCommonData.GameInstance.GameUI.addChild(answerView);
				answerView.x = 350;
				answerView.y = 150;
				answerView.bottom_mc.hint_txt.text = "";
			}
		}
		
		private function initView():void
		{
			showPanel();
			answerView.content_txt.htmlText = question;
			answerView.content_txt.autoSize = TextFieldAutoSize.LEFT;
			for(var i:uint = 0;i < 3;i++)
			{
				answerView["answer_" +(i+1)].num_txt.text = (i+1)+"、";
				answerView["answer_" +(i+1)].txt.autoSize = TextFieldAutoSize.LEFT;
				answerView["answer_" +(i+1)].mouseChildren = false;
				answerView["answer_" +(i+1)].txt.mouseEnabled = false;
				answerView["answer_" +(i+1)].txt.htmlText = this["answer" +(i+1)];
				answerView["answer_" +(i+1)].num_txt.textColor = 0xffffff;
//				answerView["answer_" +(i+1)].txt.textColor = 0xffffff;
				
				answerView["answer_" +(i+1)].addEventListener(MouseEvent.MOUSE_OVER,overTxtHandler);
				answerView["answer_" +(i+1)].addEventListener(MouseEvent.MOUSE_OUT,outTxtHandler);
				answerView["answer_" +(i+1)].addEventListener(MouseEvent.CLICK,clickHandler);
			}
			answerView.answer_2.y = answerView.answer_1.y +answerView.answer_1.height;
			answerView.answer_3.y = answerView.answer_2.y +answerView.answer_2.height;
			
			//设置缩放MC和底部MC
			answerView.scale_mc.height =  12+answerView.answer_3.y + answerView.answer_3.height - 170;
//			answerView.bottom_mc.y = answerView.scale_mc.y + answerView.scale_mc.height - 4;
			answerView.bottom_mc.y = answerView.answer_3.y + answerView.answer_3.height +4 ;
		}
		
		private function overTxtHandler(evt:MouseEvent):void
		{
			evt.target.num_txt.textColor = 0xFFCC00;
			var ind:int = int( evt.currentTarget.name.split("_")[1] );
			evt.currentTarget.txt.htmlText = "<font color='#FFCC00'>"+this["answer"+ind]+"</font>";
		}
		
		private function outTxtHandler(evt:MouseEvent):void
		{
			var ind:int = int( evt.currentTarget.name.split("_")[1] );
			evt.currentTarget.txt.htmlText = "<font color='#ffffff'>"+this["answer"+ind]+"</font>";
			evt.target.num_txt.textColor = 0xffffff;
		}
		
		private function clickHandler(evt:MouseEvent):void
		{
			var index:uint = evt.target.name.split("_")[1];
//			trace("index " +evt.target.name.split("_")[1]);
			if(index == rightOne)
			{
				sendMessage(id);
				panelCloseHandler();
				return;
			}else
			{
				evt.target.removeEventListener(MouseEvent.MOUSE_OVER,overTxtHandler);
				evt.target.removeEventListener(MouseEvent.MOUSE_OUT,outTxtHandler);
				evt.target.txt.text = "";
				evt.target.num_txt.htmlText = "";
				answerView.bottom_mc.hint_txt.text = GameCommonData.wordDic[ "mod_ans_ans_clickHandler_1" ];//"您答错了，请重新选择"
			}
		}
		
		private function sendMessage(_id:uint):void
		{
			var data:Array=[0,GameCommonData.Player.Role.Id,0,0,0,_id,263,0,0];
			var obj:Object={type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
		}
		
		private function panelCloseHandler():void
		{
			try
			{
				for(var i:uint = 0;i < 3;i++)
				{
					answerView["answer_" +(i+1)].removeEventListener(MouseEvent.MOUSE_OVER,overTxtHandler);
					answerView["answer_" +(i+1)].removeEventListener(MouseEvent.MOUSE_OUT,outTxtHandler);
					answerView["answer_" +(i+1)].removeEventListener(MouseEvent.CLICK,clickHandler);
					answerView["answer_" +(i+1)].removeEventListener(MouseEvent.MOUSE_OVER,overSelTxtHandler);
					answerView["answer_" +(i+1)].removeEventListener(MouseEvent.MOUSE_OUT,outSelTxtHandler);
				}
			}
			catch ( e:Error )
			{
				
			}
			
			if(answerView && GameCommonData.GameInstance.GameUI.contains(answerView))
			{
				GameCommonData.GameInstance.GameUI.removeChild(answerView);
			}
			dataProxy.answerPanelIsOpen = false;
		}
		
		private function sureFun():void
		{
			
		}

	}
}