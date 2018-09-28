package GameUI.View.Components.FastPurchase
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.UICore.UIFacade;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	
	public class FastPurchase extends Sprite
	{
		private var price:int;
		private var type:int;
		private var mc:MovieClip;
		private var txt:String;
		private var box:MovieClip;
		private var count:uint;
		public function FastPurchase( type:String )								//物品type
		{
			this.price = getPrice( type );
			this.type = int( type );
			mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "FastPurchase" );
			box = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "box" );
			if( mc )
			{
				if ( box )
				{
					mc.addChild( box );
					box.x = 18;
					box.y = 3;
				}
				mc.type = this.type;
				mc.btnAdd.buttonMode = true;
				mc.btnAdd.stop();
				mc.btnAdd.id = "btnAdd";
				mc.btnAdd.addEventListener( MouseEvent.CLICK, onClick );
				mc.btnSub.buttonMode = true;
				mc.btnSub.stop();
				mc.btnSub.id = "btnSub";
				mc.btnSub.addEventListener( MouseEvent.CLICK, onClick );
				mc.btnBuy.buttonMode = true;
				mc.btnBuy.stop();
				mc.btnBuy.id = "btnBuy";
				mc.btnBuy.addEventListener( MouseEvent.CLICK, onClick );
				
				mc.txtNum.restrict = "0-9";
				mc.txtNum.maxChars = 4;
				
				mc.txtPrice.text = this.price;
				
				var icon:Bitmap;
				var name:String = String( UIConstData.getItem( this.type ).Name );
				mc.Name.selectable = false;
				mc.Name.text = name;
				this.name = name;
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Icon/" + this.type + ".png",onLoadCom );
				mc.txtNum.addEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
				mc.txtNum.addEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
			}
		}
		
		private function onLoadCom():void
		{
			var icon:Bitmap;
			icon = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Icon/" + this.type + ".png");
			if( icon )
				{
					icon.x = 3;
					icon.y = 3;
					box.addChild( icon );
					box.name = "TaskEqu_"+this.type;
				}
				addChild( mc );
		}
		
		private function onClick( e:MouseEvent ):void
		{
			if( e.target.id == null) return;
			var num:int = int( mc.txtNum.text );
			var _mc:MovieClip = e.target.parent as MovieClip;
			var _count:int = int( _mc.txtNum.text );
			var _type:int = int( _mc.type );
			switch( e.target.id )
			{
				case "btnAdd":
				if( num < 999)
				{
					num++;
					mc.txtNum.text = String( num );
					mc.txtNum.maxChars = 3;
				}
				break;
				
				case "btnSub":
				if( num > 0)
				{
					num--;
					mc.txtNum.text = String( num );
					mc.txtNum.maxChars = 3;	
				}
				break;
				
				case "btnBuy":
//				fun();
				if( _count == 0 )
				{
					UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_enc_onG_2" ], color:0xffff00});//"请输入有效的购买数"	
				}else
				{
					if ( this.price*_count > GameCommonData.Player.Role.UnBindRMB )
					{
						UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( GameCommonData.wordDic[ "mod_mak_med_mar_sur_1" ]);//"元宝不足" 
						return;
					}
					this.type = _type; 
					this.count = _count;
//					var info:String = "<font color='#E2CCA5'>是否要与<font color = '#ffff00'>["+"studentName"+"]</font>解除师徒关系？</font>";
					var info:String = GameCommonData.wordDic[ "often_used_cost" ]+'<font color="#00ff00">'+ ( this.price * this.count ) +'</font>\\ab,'+GameCommonData.wordDic[ "often_used_buy" ]+'<font color="#00ff00">'+ this.count +'</font>'+GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]+'<font color="#00ffff">'+ this.name +'</font>';
					UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "often_used_tip" ],comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});	//提 示      确 定      取 消  
				}
				break;
				
				default:
				break;
			}
		}
		
		private function commitHandler():void
		{
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( MarketEvent.BUY_ITEM_MARKET,{type: this.type,count: this.count} );
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
		
		public function gc():void
		{
			mc.btnAdd.removeEventListener( MouseEvent.CLICK, onClick );
			mc.btnSub.removeEventListener( MouseEvent.CLICK, onClick );
			mc.btnBuy.removeEventListener( MouseEvent.CLICK, onClick );
			mc.txtNum.removeEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			mc.txtNum.removeEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
			var des:*;
			while ( this.numChildren > 0 )
			{
				des = this.removeChildAt( 0 );
				des = null;
			}
		}
		
		private function onFoucsIn(event:FocusEvent):void
		{ 
			GameCommonData.isFocusIn = true; 
		}
		
		private function onFoucsOut(event:FocusEvent):void
		{
			GameCommonData.isFocusIn = false;
		}
	}
}