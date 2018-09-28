package GameUI.Modules.DragonEgg.View
{
	import Controller.PlayerSkinsController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.DragonEgg.Data.DragonEggData;
	import GameUI.Modules.DragonEgg.Data.DragonEggVo;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.ResourcesFactory;
	
	import Net.ActionSend.PetEggSend;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class SingleDragonEggCell extends Sprite
	{
		public var vo:DragonEggVo;
		private var main_mc:MovieClip;
		private var eggId:int;
		private var petName:String;
		
		private var petPhoto:Bitmap;
		private var faceName:String;
		
		private var noResourceBmp:Bitmap;
		
		public function SingleDragonEggCell( _vo:DragonEggVo,_eggId:int,_petName:String )
		{
			eggId = _eggId;
			vo = _vo;
			petName = _petName;
		}
		
		public function init():void
		{
			main_mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "SingleDragonEggRes" );
			noResourceBmp = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("qMark");
			noResourceBmp.x = 11;
			noResourceBmp.y = 14;
			main_mc.addChild( noResourceBmp );
			
			main_mc.txt_0.mouseEnabled = false;
			main_mc.txt_1.mouseEnabled = false;
			main_mc.txt_2.mouseEnabled = false;
			
			main_mc.txt_0.htmlText = "二代" + petName;
			main_mc.txt_1.htmlText = vo.dangci;
			main_mc.txt_2.htmlText = vo.growStr;
			
			main_mc.look_btn.addEventListener( MouseEvent.CLICK,onLook );
			main_mc.adopt_btn.addEventListener( MouseEvent.CLICK,onAdopt );
			addChild( main_mc );
			
			addPhoto();
		}
		
		//添加头像
		private function addPhoto():void
		{
			var faceType:int = vo.type;
			faceName = faceType.toString();
			var petV:XML = PlayerSkinsController.GetPetV( faceType.toString() , vo.level-1 );
			if( petV != null )
			{
				faceName = petV.@EnemyIcon.toString();
			}
			
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/EnemyIcon/" + faceName + ".png",onLoabdComplete);
		}
		
		//加载完成
		private function onLoabdComplete():void
		{
			petPhoto = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/EnemyIcon/" + this.faceName + ".png");
 			petPhoto.x = 4;
 			petPhoto.y = 3;
 			petPhoto.scaleX = .5;
 			petPhoto.scaleY = .5;
 			main_mc.addChild( petPhoto );
 			
// 			petPhoto.filters = [ new GlowFilter( ]
 			
 			if ( main_mc.contains( noResourceBmp ) )
 			{
 				main_mc.removeChild( noResourceBmp );
 			}
		}
		
		public function onLook( evt:MouseEvent ):void
		{
			GameCommonData.UIFacadeIntance.sendNotification( DragonEggData.SHOW_LOOK_DRAGONEGG_INFO,this.vo );
		}
		
		public function onAdopt( evt:MouseEvent ):void
		{
			var petNum:int = 0;   //宠物背包容量
			///判断宠物背包是否已满
			for(var key:Object in GameCommonData.Player.Role.PetSnapList) 
  			{
				if(GameCommonData.Player.Role.PetSnapList[key].IsLock == false) 
				{
   					petNum++;
				}
			}
			if(petNum+1 >PetPropConstData.petBagNum)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_pro_bag_bag_1" ], color:0xffff00});  // 宠物背包已满
				return;
			}
			
			var info:String = "<font color='#e2cca5'>每个顽龙蛋只能领养一只顽龙，领养完成后龙蛋会消失。是否确认？</font>";    //是否要与            解除师徒关系？
			GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});  //提 示      确 定      取 消  
		}
		
		private function commitHandler():void
		{
			var obj:Object = new Object();
			obj.itemid = eggId;
			obj.action = 3;
			obj.petid = vo.id;
			obj.grow = vo.grow;
			obj.level = vo.level;
			PetEggSend.SendPetEggAction( obj );
			
			GameCommonData.UIFacadeIntance.sendNotification( DragonEggData.CLOSE_DRAGONEGG_PANEL,this.vo );
		}
		
		private function cancelClose():void{}
		
		public function gc():void
		{
			main_mc.look_btn.removeEventListener( MouseEvent.CLICK,onLook );
			main_mc.adopt_btn.removeEventListener( MouseEvent.CLICK,onAdopt );
			var des:*;
			while ( this.numChildren>0 )
			{
				des = this.removeChildAt( 0 );
				des = null;
			}
//			main_mc = null;
		}
		
	}
}