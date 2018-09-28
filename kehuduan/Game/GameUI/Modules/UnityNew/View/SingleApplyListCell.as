package GameUI.Modules.UnityNew.View
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityMemberVo;
	import GameUI.UIUtils;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class SingleApplyListCell extends Sprite implements IUnityCell
	{
		public var clickMe:Function;
		public var unityVo:UnityMemberVo; 
		
		private var main_mc:MovieClip;
		
		public function SingleApplyListCell( _vo:UnityMemberVo )
		{
			unityVo = _vo;
		}

		public function init():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.SingleApplyListCellClass ) as MovieClip;
			
			( main_mc[ "txt_0" ] as TextField ).htmlText = unityVo.name;
			( main_mc[ "txt_1" ] as TextField ).htmlText = unityVo.roleLevel.toString();
			( main_mc[ "txt_2" ] as TextField ).htmlText = unityVo.sexStr;
			( main_mc[ "txt_3" ] as TextField ).htmlText = unityVo.mainJobStr;
			( main_mc[ "txt_4" ] as TextField ).htmlText = unityVo.fighting.toString();
			( main_mc[ "btn_0" ] as SimpleButton ).addEventListener( MouseEvent.MOUSE_DOWN,acceptJoin );
			( main_mc[ "btn_1" ] as SimpleButton ).addEventListener( MouseEvent.MOUSE_DOWN,refuseJoin );
			
			for ( var i:uint=0; i<5; i++ )
			{
				( main_mc[ "txt_"+i ] as TextField ).textColor = UIUtils.getVipColor( unityVo.vip );
				( main_mc[ "txt_"+i ] as TextField ).mouseEnabled = false;
			}
			addChild( main_mc );
			
			main_mc.btn.addEventListener( MouseEvent.CLICK,onClick );
		}
		
		public function gc():void
		{
			main_mc.btn.removeEventListener( MouseEvent.CLICK,onClick );
			( main_mc[ "btn_0" ] as SimpleButton ).removeEventListener( MouseEvent.MOUSE_DOWN,acceptJoin );
			( main_mc[ "btn_1" ] as SimpleButton ).removeEventListener( MouseEvent.MOUSE_DOWN,refuseJoin );
			
			clickMe = null;
			var des:*;
			while ( this.numChildren> 0 )
			{
				des = removeChildAt( 0 );
				des = null;
			}
		}
		
		private function onClick( evt:MouseEvent ):void
		{
			if ( clickMe != null )
			{
				clickMe( this );
			}
		}
		
		private function acceptJoin( evt:MouseEvent ):void
		{
//			evt.stopPropagation();
			if ( GameCommonData.Player.Role.unityJob < 85 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );
				return;
			}
//			RequestUnity.send( 211, 0 );
			
			var obj:Object = new Object();
			obj.type = 1107;
			obj.data = [ 0, 0, 211, 0, this.unityVo.id ];
			UnityActionSend.SendSynAction( obj );	
		}
		
		private function refuseJoin( evt:MouseEvent ):void
		{
//			evt.stopPropagation();
			if ( GameCommonData.Player.Role.unityJob < 85 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );
				return;
			}
			
			var obj:Object = new Object();
			obj.type = 1107;
			obj.data = [ 0, 0, 212, 0, this.unityVo.id ];
			UnityActionSend.SendSynAction( obj );	
//			RequestUnity.send( 212, 0 );
		}
		
	}
}