package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	
	import flash.display.DisplayObject;
	import flash.geom.Point;
	
	/**
	 *  
	 * @author felix
	 * 
	 */	
	public class UIEnemyList extends UIFriendsList
	{
		public function UIEnemyList(value:Array=null)
		{
			super([]);
			this.width=140;
		}
		
		protected override function createMenu():void{
			super.createMenu();
			var dataList:Array=[];
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]}});//"发送消息"		"发送消息"
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ]}});//"设为私聊"		"设为私聊"		
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ]}});//"复制名字"		"复制名字"
			if(GameCommonData.wordVersion != 2)
			{
				dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ]}});//"邀请入帮"  "邀请入帮"	
			} 
			dataList.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}});//"查看资料"		"查看资料"
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_5" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_5" ]}});//"删除仇人"  "删除仇人"			
			this.menu.dataPro=dataList;
		}
		
		protected override function onMouseClickHandler():void{
			this.countClick=0;
			var localPoint:Point=new Point(arguments[1],arguments[2]);
			var globalPoint:Point=this.localToGlobal(localPoint);
			this.currendPos=globalPoint;
			this.menu.roleInfo=arguments[0] as PlayerInfoStruct;
			var m1:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if(m1!=null){
				GameCommonData.GameInstance.GameUI.removeChild(m1);
			}
			GameCommonData.GameInstance.GameUI.addChild(this.menu);
			this.menu.x=this.currendPos.x;
			this.menu.y=this.currendPos.y;
		}
		
	}
}