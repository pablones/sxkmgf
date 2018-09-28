package GameUI.Modules.Answer.MasterAnswer.view
{
	import GameUI.Modules.Answer.Data.AnswerProp;
	
	import Net.ActionSend.TutorSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class MasterAnswerView extends Sprite
	{
		public var main_mc:MovieClip;
//		private var answerData:AnswerProp;
		private var choices:Array;
		
		public function MasterAnswerView()
		{
			initUI();
		}
		
		private function initUI():void
		{
			main_mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "MasterAnswerUI" );
			for ( var i:uint=0; i<main_mc.numChildren-1; i++ )
			{
				var obj:* = main_mc.getChildAt( i );
				if ( obj is TextField )
				{
					obj.mouseEnabled = false;
				}
			}
			addChild( main_mc );
		}
		
		public function set answerItem( answerProp:AnswerProp ):void
		{
			main_mc.serial_txt.htmlText = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + answerProp.serial + GameCommonData.wordDic[ "mod_ans_mas—_view_mas_ans_1" ]; 
//			main_mc.content_txt.text = answerProp.question;
			main_mc.content_txt.htmlText = answerProp.realQuestion;
			choices = answerProp.randomAnswer;
			for ( var i:uint=0; i<3; i++ )
			{
				main_mc[ "answerTxt_"+i ].htmlText = choices[ i ].content;
			}
			isSeeBtn( true );
//			this.answerData = answerProp;
		}
		
		public function listening():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "answerBtn_"+i ] as SimpleButton ).addEventListener( MouseEvent.CLICK,clickBtnHandler );
			}
		}
		
		public function isSeeBtn( isSee:Boolean ):void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "answerBtn_"+i ] as SimpleButton ).visible = isSee;
				( main_mc[ "answerTxt_"+i ] as TextField ).visible = isSee;
			}
			if ( !isSee )
			{
				main_mc[ "answerTxt_1" ].htmlText = GameCommonData.wordDic[ "mod_ans_mas—_view_mas_iss_1" ];
				main_mc[ "answerTxt_1" ].visible = true;
			}
		}
		
		private function clickBtnHandler( evt:MouseEvent ):void
		{
			var index:int = evt.target.name.split( "_" )[ 1 ];
			isSeeBtn( false );
//			trace ( choices[ index ].id );
			
			var obj:Object = new Object();
			obj.action = 36;
			obj.amount = 0;
			obj.pageIndex = choices[ index ].id + 1;
			obj.mentorId = 0;
			TutorSend.sendTutorAction( obj );
		}
		
		public function gc():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "answerBtn_"+i ] as SimpleButton ).removeEventListener( MouseEvent.CLICK,clickBtnHandler );
			}
		}

	}
}