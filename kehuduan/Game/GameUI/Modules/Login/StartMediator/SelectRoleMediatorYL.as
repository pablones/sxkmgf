package GameUI.Modules.Login.StartMediator
{ 
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Login.Data.CreateRoleConstData;
	import GameUI.Modules.Login.Jobchange.Jobchange;
	
	import Net.ActionSend.CreateRoleSend;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SelectRoleMediatorYL extends Mediator
	{
		public static const NAME:String = "SelectRoleMediatorYL";
		public static const DELETEROLE:String  = "DELETEROLE";
		private var selectRole:MovieClip;
		private var index:int;                     //选择第index个人物
		
		public function SelectRoleMediatorYL()
		{
			super(NAME);
		}
		
//		private function get selectRole():MovieClip
//		{
//			return viewComponent as MovieClip
//		}
		
		public override function listNotificationInterests():Array 
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWSELECTROLE,
				EventList.REMOVEELECTROLE,
				DELETEROLE
			];
		}
		
		public override function handleNotification(notification:INotification):void 
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
//					trace("SelectRoleMediatorYL INITVIEW");
					selectRole = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassByMovieClip("SelectRoleTest") as MovieClip;
//					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SELECTROLETEST});
				break;
				
				case EventList.SHOWSELECTROLE:
//				    var c:
				    selectOnesRole( notification.getBody() as int );													//得到默认人物
					show();
				break;
				
				case EventList.REMOVEELECTROLE:
					gc();
					//facade.removeMediator(SelectRoleMediatorYL.NAME);
				break;
				
				///////角色删除成功
				case DELETEROLE:
				    GameCommonData.RoleList.splice(index,1);                                                 //在面板上删除角色
			        initView();                                                                              //初始化
			        showRoles();                                                                             //显示可选角色
			        selectOnesRole(0);                                                                      //默认选中第一个人物
//				    facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade_dok, cancel:cancelClose, isShowClose:false, info: "删除成功", title:"提示", comfirmTxt:"确定", cancelTxt:"取消"});
				break;
			}
		}
		
		private function show():void 
		{
			GameCommonData.GameInstance.GameUI.addChild(selectRole);
			initView();
			showRoles();                                                                              //显示可选角色
			selectRole.btnCreateRole.addEventListener(MouseEvent.CLICK, createHandler);
		}
		
		/** 初始化  */
		private function initView():void
		{
			for(var i:int = 0; i<3; i++)                                                              //最多三个角色
			{
				this.selectRole["role_" + i].visible = false;
			}
		}
		
		/** 显示可选的角色  */
		private function showRoles():void
		{
			//trace(GameCommonData.RoleList.length);
			for(var i:int = 0; i < GameCommonData.RoleList.length; i++)
			{
//				trace("*****GameCommonData.RoleList****** = ", GameCommonData.RoleList[i].UserId);
				this.selectRole["role_" + i].visible = true;
				this.selectRole["role_" + i].mouseEnabled = true;
				this.selectRole["role_" + i].addEventListener(MouseEvent.CLICK, click);                                                         //控制单击双击第一个人物
				showman(Jobchange.homechangeback(GameCommonData.RoleList[i].FirJob ) , int(GameCommonData.RoleList[i].sexindex)+1 , i)       //显示角色人物，参数分别是门派，性别，第Index角色
			} 
			if(GameCommonData.RoleList.length<3 &&GameCommonData.RoleList.length>0)
			{
				selectRole.btnCreateRole.visible = true;
				selectRole.btnEnterGame.visible  = true;                                            //进入游戏按钮可用
				selectRole.btndelete.visible     = true;                                            //删除角色按钮可用
			}
			else if(GameCommonData.RoleList.length == 0)
			{
				selectRole.btnEnterGame.visible = false;                                           //进入游戏按钮为灰
				selectRole.btndelete.visible = false;                                              //删除角色按钮为灰
			}
			else
			{
//				selectRole.btnCreateRole.removeEventListener(MouseEvent.CLICK , EnterGameHandler);                 //如果角色已达到3个得话，移除单击事件
				selectRole.btnCreateRole.visible = false;
			}
		}
		
		private var countClick:int = 0;
		//双击一个人物
	    private	function click(e:MouseEvent):void 
	    {
	    	var mc:MovieClip = e.target as MovieClip;	    	
			var id:int = 0;
			countClick++;
			if (countClick == 1) {
//				trace("click");
				id = setTimeout(selectedRole , 200 , mc);
				e.stopPropagation();
			} 
			else if (countClick == 2) {
//				trace("00000000000");
				countClick = 0;
				clearTimeout(id);
				e.stopPropagation();
				selectedRole(mc);											//双击也有选择人物功能
				double_EnterGameHandler(mc);
			}
		}

		/** 处理gc  */
		private function gc():void
		{
			GameCommonData.GameInstance.GameUI.removeChild(selectRole);
			for(var i:int = 0; i<GameCommonData.RoleList.length; i++)
			{
				this.selectRole["role_" + i].removeEventListener(MouseEvent.CLICK, selectedRole);
			}
		}
		
		/** 选择角色  */
		private function selectedRole(e:MovieClip):void
		{
			this.countClick = 0;
		    index = int(e.name.split("_")[1]);                                                                //获取点击的角色下标
			selectOnesRole(index);                                                                                   //选中某个角色得方法
			/** 待用 */
			/////////SetFrame.RemoveFrame(selectRole,"SelectFrame")                                                           //SelectRolemc中的选择mc全部删除
		    /////////SetFrame.UseFrame(e.target as MovieClip , "SelectFrame" , -50 ,100 , 275 , 155)                         //再添加选择mc到Role上
		}
		
		//第index个角色的信息
		private function selectOnesRole(index:int):void
		{
			this.selectRole.SzName.text            = GameCommonData.RoleList[index].SzName;                          //角色名称
			this.selectRole.level.text             = GameCommonData.RoleList[index].Level;                           //角色等级
			var firjobindex:int = GameCommonData.RoleList[index].FirJob;                                             //主职业名称index
			var secjobindex:int = GameCommonData.RoleList[index].SecJob;                                             //副职业名称index                                        
			var homeindex:int = Jobchange.homechangeback(GameCommonData.RoleList[index].FirJob );                    //门派数值转换
			var sexindex:int = int(GameCommonData.RoleList[index].sexindex) + 1;                                     //获取性别数据   
//			trace(GameCommonData.RoleList[index].sexindex);
//			trace("homeindex=" + homeindex);
//			trace("sexindex=" + int(GameCommonData.RoleList[index].sexindex));
//			trace("k=",k);
            var k:int = GameCommonData.RoleList[index].sexindex;
			this.selectRole.mcfistjop.txtjob.text  = GameCommonData.RolesListDic[firjobindex];                     //获得主职业名称
			this.selectRole.mcscendjop.txtjob.text = GameCommonData.RolesListDic[secjobindex];                     //获得副职业名称
			this.selectRole.mcfistjop.Level.text   = GameCommonData.RoleList[index].FirLev;                        //主职业等级
			this.selectRole.mcscendjop.Level.text  = GameCommonData.RoleList[index].SecLev;                        //副职业等级
			//this.selectRole.mcHead.text            = GameCommonData.RoleList[index].Photo;                       //角色图像
		    showman(homeindex , sexindex , index)                                                                  //显示角色人物，参数分别是门派，性别，第Index角色
			showlook(GameCommonData.RoleList[index].Photo);                                                        //显示角色头像                                     
			
			//this.selectRole.mcHead.gotoAndStop(GameCommonData.RoleList[index].Photo);
			selectRole.btnEnterGame.addEventListener (MouseEvent.CLICK , EnterGameHandler);
			selectRole.btndelete.addEventListener (MouseEvent.CLICK , deleteHandler);                              //删除角色
		}
		
		/** 进入游戏  */
		private function EnterGameHandler(e:MouseEvent):void
		{
			facade.sendNotification(CommandList.SELECTROLECOMMAND, index);                          //发送通知，将获取的角色下标传出去
		}
		
		/** 双击人物后进入游戏 */
		private function double_EnterGameHandler(e:MovieClip):void
		{
			facade.sendNotification(CommandList.SELECTROLECOMMAND, index);                          //发送通知，将获取的角色下标传出去
		}
		
		/** 点击创建角色按钮 */
		private function createHandler(e:MouseEvent):void
		{
		    facade.sendNotification(EventList.REMOVEELECTROLE);                                //移除选择角色面板
			facade.sendNotification(EventList.SHOWCREATEROLE);                                 //显示创建角色面板 
		}
		/**显示人物头像*/
		private function showlook(index:int):void
		{
				selectRole["mclook"].gotoAndStop(index);
		}
		/**显示角色人物*/
		private function showman(homeindex:int,sexindex:int,roleindex:int):void
		{
			var frame:String = "frmRole_" + homeindex * 10 + sexindex;
//			trace("frame="+frame);
			selectRole["role_"+roleindex].gotoAndStop(frame)//"frmRole+homeindex * 10 + sexindex");
		} 
		/**删除角色*/
		private function deleteHandler(e:MouseEvent):void
		{
			facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade, cancel:cancelClose, isShowClose:false, info: GameCommonData.wordDic[ "mod_log_sta_sel_del" ], title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});
			//"你将会删除一个角色！确定要这样做吗？删除的角色将是无法恢复的！"		"警 告"		"确定"	"取消"
		}
		/**确定删除后的操作*/
		private function applyTrade():void
		{
			trace("index=",index);
			var sexindex:int = GameCommonData.RoleList[index].sexindex;                                 //第二职业改为性别 				
			//GameCommonData.RoleList[index].UserId       = playerobj.id;                               //游戏ID
			//GameCommonData.RoleList[index].Level        = 0;                                          //新手等级为0
			//GameCommonData.RoleList[index].Photo        = 0;                                          //头像
			//GameCommonData.RoleList[index].FirJob       = 0;    			                            //主职业
			//GameCommonData.RoleList[index].FirLev       = 0;                                    	    //主职业等级
			//GameCommonData.RoleList[index].SecJob       = 0;                                          //副职业
			//GameCommonData.RoleList[index].SecLev       = 0;                                          //副职业等级
			
			//GameCommonData.RoleList[index].Coattype 	= 0;			                                //外装
			//GameCommonData.RoleList[index].Wapon 		= 0; 		     	                            //武器
			//GameCommonData.RoleList[index].Mount		= 0;                                            //坐骑
			//GameCommonData.RoleList[index].SzName		= "";                                           //角色名称
//			trace("GameCommonData.RoleList[index].UserId = "+CreateRoleMediatorYl.playerobj.id);
			CreateRoleSend.createMsgReg([GameCommonData.RoleList[index].SzName , "123", CreateRoleConstData.playerobj.id, 0  ,sexindex-1]);
		}
		
		/**确定删除成功后的操作*/
		private function applyTrade_dok():void
		{
		}
		/**取消删除后的操作*/
		private function cancelClose():void
		{
			
		}
	}
}