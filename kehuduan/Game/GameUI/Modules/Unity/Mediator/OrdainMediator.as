package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.*;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class OrdainMediator extends Mediator
	{
		public static const NAME:String = "OrdainMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var ordainView:MovieClip;
		private var listView:ListComponent;
		private var oneId:int;
		private var playerJop:int;
		private var oneName:String;
		private var isLetJop:Boolean;							//帮主是否让位;
		private var selectObj:Object;							//选中者的Obj
		private var farmersJopIndex:int = 0;;					//果农数量
		public function OrdainMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
			 		EventList.INITVIEW,
			       	UnityEvent.SHOWORDAINVIEW,
			       	UnityEvent.CLOSEORDAINVIEW,
			       	UnityEvent.UPDATEORDAINDATA
			      ]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					ordainView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("OrdainView");
					panelBase = new PanelBase(ordainView, ordainView.width+8, ordainView.height+12);
					panelBase.name = "OrdainView";
					panelBase.x = UIConstData.DefaultPos2.x - 500;
					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_ord_han_1" ] ); // 任命
					if(ordainView != null)
					{
						ordainView.mouseEnabled = false;
						ordainView.txtName.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWORDAINVIEW:
					var arr:Array = notification.getBody() as Array;	
					showOrdainView();
					radioInit(); 											//单选按钮初始化，全部清空
			    	addLis();
					jopRadioInit();											//不同权限打开不同的任命按钮
					oneId   = arr[0];										//储存传过来的ID
					oneName = arr[1];
					this.selectObj = arr[2];
					ordainView.txtName.text = oneName;
					ordainView.btnComfrim.visible = false;
		    	break;
		    	
		    	case UnityEvent.CLOSEORDAINVIEW:
		    		gcAll();
		    	break;
		    	
		    	case UnityEvent.UPDATEORDAINDATA:
		    	break;
			}
		}
		
		private function showOrdainView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			UnityConstData.ordainIsOpen = true;
			ordainView.txtBox_0.text = (UnityUtils.getBuiltedSubList().length == 0 ? GameCommonData.wordDic[ "mod_uni_med_ord_sho_1" ]:UnityUtils.getBuiltedSubList()[0]);			// 			暂无分堂				//文本初始化
			ordainView.txtBox_1.text = GameCommonData.wordDic[ "mod_uni_dat_uni_ord_1" ]; // 堂主
		}
		
		private function addLis():void
		{
			 panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			 ordainView.btnCancel.addEventListener(MouseEvent.CLICK , cancelHandler);
			 ordainView.btnComfrim.addEventListener(MouseEvent.CLICK , comfrimHandler);
			 ordainView.btnComboBox_0.addEventListener(MouseEvent.CLICK, comboBoxHandler);
			 ordainView.btnComboBox_1.addEventListener(MouseEvent.CLICK, comboBoxHandler);
			 ordainView.stage.addEventListener(MouseEvent.CLICK, clearComboBoxHandler);
			 for(var i:int;i < 4;i++)
			 {
			 	ordainView["mcRadio_"+i].addEventListener(MouseEvent.CLICK , sclectHandler);
			 } 
		}
		
		private function gcAll():void
		{
			ordainView.stage.removeEventListener(MouseEvent.CLICK, clearComboBoxHandler);
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			UnityConstData.ordainIsOpen = false;
			removeList();
			listView = null;
		    oneId = 0;
		    oneName = "";
		    playerJop = 0;
		    panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			ordainView.btnCancel.removeEventListener(MouseEvent.CLICK , cancelHandler);
			ordainView.btnComfrim.removeEventListener(MouseEvent.CLICK , comfrimHandler);
			ordainView.btnComboBox_0.removeEventListener(MouseEvent.CLICK, comboBoxHandler);
			ordainView.btnComboBox_1.removeEventListener(MouseEvent.CLICK, comboBoxHandler);
			isLetJop = false;
			for(var i:int;i < 4;i++)
			{
			  	ordainView["mcRadio_"+i].removeEventListener(MouseEvent.CLICK , sclectHandler);
			} 

		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 点击确定按钮 */
		private function comfrimHandler(e:MouseEvent):void
		{
			if(isLetJop == true)												//让位请求
			{//确定要让出帮主职位吗？ 提示  确定  取消
				facade.sendNotification(EventList.SHOWALERT, {comfrim:letJopTrade, cancel:letJopClose, isShowClose:false, info: GameCommonData.wordDic[ "mod_uni_med_ord_com_1" ], title:GameCommonData.wordDic[ "often_used_tip" ], comfirmTxt:GameCommonData.wordDic[ "often_used_confim" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ] });
			}
			else																//任命请求
			{
				sendOrdian(215 , playerJop , oneId);
				facade.sendNotification(UnityEvent.CLOSEORDAINVIEW);
			}
		}
		/** 点击取消按钮 */
		private function cancelHandler(e:MouseEvent):void
		{
			facade.sendNotification(UnityEvent.CLOSEORDAINVIEW);
		}
		/** 点击单选按钮 */
		private function sclectHandler(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			changeJop(i);   															//单选后定下权限
			selectRadio(i);
		}
		/** 点击下拉列表按钮 */
		private function comboBoxHandler(e:MouseEvent):void
		{
			if(UnityUtils.getBuiltedSubList().length == 0)								//已建造的分堂数组为空
			{// 本帮暂时没有分堂
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_ord_comb_1" ], color:0xffff00});
				return;
			}
			if(listView != null && ordainView.contains(listView))
			{
				ordainView.removeChild(listView);
				return;
			}
			var n :int = e.target.name.split("_")[1];
			removeList();
			listView = new ListComponent(false);
			ordainView.addChild(listView);                      
			switch(n)
			{
				case 0:
					showcomboBox(0 , 4 , 34 , UnityUtils.getBuiltedSubList());			//有多少个分堂就显示多少分堂的任命
				break;
				
				case 1:
					showcomboBox(4 , 8 ,104);
				break;
			}
			e.stopPropagation();
		}
		/** 点击舞台取消下拉列表 */
		private function clearComboBoxHandler(e:MouseEvent):void
		{
			if(listView != null && ordainView.contains(listView))
			{
				ordainView.removeChild(listView);
			}
		}
		/** 当权限为堂主时,点击box1下拉列表框事件 */
		private function  jop80Handler(e:MouseEvent):void
		{
			if(listView != null && ordainView.contains(listView))
			{
				ordainView.removeChild(listView);
				return;
			}
			listView = new ListComponent(false);
			ordainView.addChild(listView);
			showcomboBox(5 , UnityConstData.ordainArr.length ,104);
			e.stopPropagation();
		}
		/** 当权限为副堂主时,点击box1下拉列表框事件 */
		private function  jop70Handler(e:MouseEvent):void
		{
			if(listView != null && ordainView.contains(listView))
			{
				ordainView.removeChild(listView);
				return;
			}
			listView = new ListComponent(false);
			ordainView.addChild(listView);
			showcomboBox(6 , UnityConstData.ordainArr.length ,104);
			e.stopPropagation();
		}
		/** 点击某个下拉框 */
		private function selectItem(e:MouseEvent):void
		{
			var item:MovieClip = e.currentTarget as MovieClip;
			removeList();
			int(item.name) < 4 ? ordainView.txtBox_0.text = item.txtList.text : ordainView.txtBox_1.text = item.txtList.text;
			changeJop(2);																						//定下权限，选择的是第二个单选按钮
			removeList();
		}
		/**将下拉列表框从mc中移除掉*/
		private function removeList():void
		{
			if(this.listView)
			{
				if(ordainView.contains(listView))
				{
					ordainView.removeChild(listView);
					listView = null;
				}
			}
		}
		
		/** 出现下拉列表*/
		private function showcomboBox(i:int , length:int , x:int , list:Array = null):void
		{
			if(list) length = list.length;							//如果特定了下拉列表的内容，就是用特定的内容长度
			for(i; i<length; i++)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("OrdainItem");
				item.name = i.toString();
				item.width = 110;	
				if(list) item.txtList.text = list[i];				//如果特定了下拉列表的内容，就是用特定的内容
				else item.txtList.text = UnityConstData.ordainArr[i];
				item.addEventListener(MouseEvent.CLICK, selectItem);
				listView.SetChild(item);
			}
			listView.width = 59;
			listView.x = x;
			listView.y = 62; 
			listView.upDataPos();
		}
		/** 选择某一个单选按钮*/
		private function selectRadio(i:int):void
		{
			radioInit();										//按钮初始化
			removeList();										//将下拉列表框从mc中移除掉
			ordainView.btnComfrim.visible = true;				//确定按钮可见
			ordainView["mcRadio_"+i].gotoAndStop(2);
			if(i == 2)
			{
				ordainView.btnComboBox_0.visible = true;
				ordainView.btnComboBox_1.visible = true;
			}
		}
		/** 单选按钮和下拉框的初始化*/
		private function radioInit():void
		{
			//单选按钮变空
			for(var i:int;i < 4;i++)
			{
				ordainView["mcRadio_"+i].gotoAndStop(1);
			}
			//下拉框变灰
			for(var n:int;n < 2;n++)
			{
				ordainView["btnComboBox_"+n].visible = false;
			}	
		}
		/** 初始化权限单选按钮*/
		private function jopRadioInit():void
		{
			listView = new ListComponent(false);																	//一开始就创建Listview的实例
			for(var i:int;i < 4;i++)
	 		{
	 			ordainView["mcRadio_"+i].visible = false;															//全看不见
	 		} 
			if(GameCommonData.Player.Role.unityJob  == 100)															//帮主权限
			{
				ordainBtn(0);
				ordainView.txtBoss_0.visible = true;
				ordainView.txtBoss_1.visible = true;
		 	}
		 	else if(GameCommonData.Player.Role.unityJob  ==90)														//副帮主权限
			{
				ordainBtn(2);
				ordainView.txtBoss_0.visible = false;
				ordainView.txtBoss_1.visible = false;
				
		 	}
			else if(GameCommonData.Player.Role.unityJob   > 80 && GameCommonData.Player.Role.unityJob   < 85)		//堂主权限
			{
				var l:int = GameCommonData.Player.Role.unityJob % 80 - 1;
				ordainBtn(2);
				ordainView.txtBoss_0.visible = false;
				ordainView.txtBoss_1.visible = false;
				ordainView["btnComboBox_"+0].mouseEnabled = false;
				ordainView.btnComboBox_1.removeEventListener(MouseEvent.CLICK, comboBoxHandler);
				ordainView.btnComboBox_1.addEventListener(MouseEvent.CLICK, jop80Handler);
				ordainView.txtBox_1.text = UnityConstData.ordainArr[5];
				ordainView.txtBox_0.text = UnityConstData.ordainArr[l];
			}
			else if(GameCommonData.Player.Role.unityJob > 70 && GameCommonData.Player.Role.unityJob < 75)			//副堂主权限
			{
				var h:int = GameCommonData.Player.Role.unityJob % 70 - 1;
				ordainBtn(2);
				ordainView.txtBoss_0.visible = false;
				ordainView.txtBoss_1.visible = false;
				ordainView["btnComboBox_"+0].mouseEnabled = false;
				ordainView.btnComboBox_1.removeEventListener(MouseEvent.CLICK, comboBoxHandler);
				ordainView.btnComboBox_1.addEventListener(MouseEvent.CLICK, jop70Handler);
				ordainView.txtBox_1.text = UnityConstData.ordainArr[6];
				ordainView.txtBox_0.text = UnityConstData.ordainArr[h];
			}
			else
			{
				for(var k:int;k < 4;k++)
	 			{
		 			ordainView["mcRadio_"+k].visible = false; 
		 		} 
			}
		}
		/** 单选按钮的实现 */
		private function ordainBtn(i:int):void
		{
			for(var n:int = i ;n < 4;n++)
				{
					ordainView["mcRadio_" + n].visible = true;
				}
		}
		/** 发送任命请求 */
		private function sendOrdian(type:int , jop:int , id:int):void
		{
			if(jop > 60 && jop < 65 )
			{
				getFarmersJopIndex();
				if(farmersJopIndex >= 2 * UnityConstData.otherUnityArray[getOrdainIndex()].level) 
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_ord_sen_1" ], color:0xffff00}); // 本堂精英数已达到上线
					return;
				}
			}
			
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , type , jop , oneId];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送任命请求
		}
		/** 判断任命职位的数值 */
		private function changeJop(i:int):void														//i为单选按钮的序列号
		{
			switch(i)
			{
				case 0:
					isLetJop = true;																	//帮主让位
				break;
				case 1:
					isLetJop = false;
					playerJop = 90;
				break;
				case 2:
					isLetJop = false;
					switch(ordainView.txtBox_0.text)
					{
						case GameCommonData.wordDic[ "mod_uni_dat_uni_green_1" ]:   // 青龙堂
							littlehome(1);
						break;
						case GameCommonData.wordDic[ "mod_uni_dat_uni_white_1" ]:  // 白虎堂
							littlehome(2);
						break;
						case GameCommonData.wordDic[ "mod_uni_dat_uni_red_1" ]:  // 朱雀堂
							littlehome(4);
						break;
						case GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_1" ]:  // 玄武堂
							littlehome(3);
						break;
						
					}
				break;
				case 3:		//普通帮众 ，果农去掉了															
					isLetJop = false;
					playerJop = 10;
				break;
//				case 4:
//					isLetJop = false;
//					playerJop = 10;
//				break;
			}
		}
		/** 每个分堂的判断*/
		private function littlehome(i:int):void
		{
			switch(ordainView.txtBox_1.text)
			{
				case GameCommonData.wordDic[ "mod_uni_dat_uni_ord_1" ]:  // 堂主
					isLetJop = false;
					playerJop = int(80) + i;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_uni_ord_2" ]:  // 副堂主
					isLetJop = false;
					playerJop = int(70) + i;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_uni_ord_3" ]:  // 精英
					isLetJop = false;
					playerJop = int(60) + i;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_uni_ord_4" ]:		//帮众  
					isLetJop = false;
					playerJop = int(50) + i;
				break;
			}
		}
		/** 确定让位 */
		private function letJopTrade():void
		{
			sendOrdian(217 , 0 , oneId);
//			UnityConstData.allMenberList = [];
			facade.sendNotification(UnityEvent.CLOSEORDAINVIEW);
		}
		/** 取消让位的对话框*/
		private function letJopClose():void
		{
			
		}
		/** 判断分堂精英上线 */
		private function getFarmersJopIndex():void
		{
			for(var i:int = 0; i < UnityConstData.allMenberList.length; i++)
			{
			
				if(UnityConstData.allMenberList[i] is Array)							//二维数组
				{
					for(var n:int = 0; n < (UnityConstData.allMenberList[i] as Array).length; n++)
					{
						if((UnityConstData.allMenberList[i][n] as Object)["4"] > 60 && (UnityConstData.allMenberList[i][n] as Object)["4"] < 65)					//如果是精英
						{
							farmersJopIndex += 1; 
						}
					}
				}
			}
		}
		/** 得到正要任命的分堂序号 */
		private function getOrdainIndex():int
		{
			var index:int;
			for(var i:int = 1;i < 5;i++)
			{
				if(UnityConstData.otherUnityArray[i].name == ordainView.txtBox_0.text)
				{
					index = i;
				}
			}
			return index;
		}
	}
}