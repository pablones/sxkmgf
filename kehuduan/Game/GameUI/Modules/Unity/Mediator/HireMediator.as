package GameUI.Modules.Unity.Mediator
{
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Command.SendActionCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class HireMediator extends Mediator
	{
		public static const NAME:String = "HireMediator";
		
		private var type:int;
		private var unityHireView:MovieClip;
		private var panelBase:PanelBase;
		private var bulitManNum:int;
		private var businessmanNum:int;
		private var masterNum:int;
		private var manCurrentArr:Array = [];
		private var manInitArr:Array = [];			//初始化雇佣数组
		private var totalMoney:int;					//总钱数
		private var initMoney:int;					//初始钱数
		public function HireMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
						UnityEvent.SHOWHIREVIEW,
						UnityEvent.CLOSEHIREVIEW,
						UnityEvent.CHANGEHIRENUM	
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case UnityEvent.SHOWHIREVIEW:
					if(UnityConstData.hireViewIsOpen == true) 
					{
						gcAll();
						return;
					}
					UnityConstData.hireViewIsOpen  = true;
					type = notification.getBody() as int;
					show();
					init();
					addLis();
					updateData();
				break;
				case UnityEvent.CLOSEHIREVIEW:
					gcAll();
				break;
				case UnityEvent.CHANGEHIRENUM:		//改变雇佣人数成功	, 带聊天信息
					dataHandler(notification.getBody() as String);
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到面板  OK
//					gcAll();
				break;
			}
		}
		
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		
		private function init():void
		{
			panelBase.SetTitleTxt( UnityConstData.otherUnityArray[type].name + GameCommonData.wordDic[ "mod_uni_med_hir_ini_1" ] );  // 雇佣
			bulitManNum 	= UnityConstData.otherUnityArray[type].craftsmanNum;
			businessmanNum 	= UnityConstData.otherUnityArray[type].businessmanNum;
			masterNum		= UnityConstData.otherUnityArray[type].masterNum;	
			for(var i:int = 0;i < 3;i++)
			{
				unityHireView["txtAddMan_" + i].stage.focus = unityHireView["txtAddMan_" + i];
				(unityHireView["txtAddMan_" + i] as TextField).maxChars = 4;
				unityHireView["txtAddMan_" + i].restrict = "0-9";
			}
			manCurrentArr = [bulitManNum , businessmanNum , masterNum];
			var _bulitManNum:int = bulitManNum;
			var _businessmanNum:int = businessmanNum;
			var _masterNum:int = masterNum;
			manInitArr = [_bulitManNum , _businessmanNum , _masterNum];
			costMoney();							//初始化金钱
		}
		private function show():void
		{
			unityHireView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("UnityHireView") as MovieClip;
			panelBase = new PanelBase(unityHireView, unityHireView.width,unityHireView.height+14);
			panelBase.name = "unityHireView";
			panelBase.x = 16;//UIConstData.DefaultPos2.x - 600;
			panelBase.y = 100;//UIConstData.DefaultPos2.y;
			
			if(unityHireView != null)
			{
				unityHireView.mouseEnabled = false;
			}
			
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
		}
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			for(var i:int = 0;i < 3;i++)
			{
				unityHireView["btnDown_" + i].addEventListener(MouseEvent.CLICK , downHandler);
				unityHireView["btnAdd_" + i].addEventListener(MouseEvent.CLICK , addHandler);
			 	unityHireView["txtAddMan_" + i].addEventListener(Event.CHANGE , inputManHandler);
			 	unityHireView["txtAddMan_" + i].addEventListener(MouseEvent.CLICK, focusInHandler);
			}
			unityHireView.btnSure.addEventListener(MouseEvent.CLICK , sureHandler);
		}
		private function gcAll():void
		{
			UnityConstData.hireViewIsOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			for(var i:int = 0;i < 3;i++)
			{
				unityHireView["btnDown_" + i].removeEventListener(MouseEvent.CLICK , downHandler);
				unityHireView["btnAdd_" + i].removeEventListener(MouseEvent.CLICK , addHandler);
				unityHireView["txtAddMan_" + i].removeEventListener(Event.CHANGE , inputManHandler);
			 	unityHireView["txtAddMan_" + i].removeEventListener(MouseEvent.CLICK, focusInHandler);
			}
			unityHireView.btnSure.removeEventListener(MouseEvent.CLICK , sureHandler);
			this.bulitManNum = 0;
			this.businessmanNum = 0;
			this.masterNum = 0;
		}
		private function updateData():void
		{
			for(var i:int = 0;i < 3;i++)
			{
				unityHireView["txtAddMan_" + i].text = manCurrentArr[i];
				unityHireView["txtManTop_" + i].text = UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[type].level , "workMan");	
			}
//			showMoney(0);
		}
		/** 文本框获得焦点事件 */
		private function focusInHandler(e:MouseEvent):void
		{
			var txt:TextField = e.currentTarget as TextField;
			txt.setSelection(0 , txt.length);//(e.currentTarget as TextField).text.length -1);
		}
		/** 点击增加*/
		private function addHandler(e:MouseEvent):void
		{
			var i:int = e.currentTarget.name.split("_")[1];
			if(manCurrentArr[i] < UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[type].level , "workMan"))
			{
				manCurrentArr[i] += 1;
				costMoney();
				updateData();
			}
		}
		/** 点击减少*/
		private function downHandler(e:MouseEvent):void
		{
			var i:int = e.currentTarget.name.split("_")[1];
			if(manCurrentArr[i] > 0)
			{
				manCurrentArr[i] -= 1;
				costMoney();
				updateData();
			}
		}
		/** 点击确定 */
		private function sureHandler(e:MouseEvent):void
		{
			
			var isChange:Boolean = false;
			for(var i:int = 0; i < manCurrentArr.length ; i++)
			{
				if(manCurrentArr[i] != manInitArr[i])
				{
					isChange = true;
				} 
			}
			if(isChange == false) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_hir_sur_1" ], color:0xffff00});  //本次操作没有雇佣新员工
				return;
			}
			if((totalMoney - initMoney) > int(UnityConstData.mainUnityDataObj.unityMoney))
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_hir_sur_2" ], color:0xffff00}); // 帮派金钱不足
				return;
			}
			facade.sendNotification(SendActionCommand.SENDACTION , {type:222 , data:this.type , list:[unityHireView["txtAddMan_" + 0].text , unityHireView["txtAddMan_" + 1].text , unityHireView["txtAddMan_" + 2].text]});
			//确定后将数据存入缓存数组对象里面
		}
		/** 输入雇用人数事件 */
		private function inputManHandler(e:Event):void
		{
			if(e.currentTarget.length > 1 ) 
			{
				if(int(String(e.currentTarget.text).charAt(0)) == 0) 
				{
					e.currentTarget.text = String(e.currentTarget.text).slice(1);
				}
			}
			//如果输入的人数大于最大值
			if(e.currentTarget.text > int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[type].level , "workMan")))
			{
				e.currentTarget.text = int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[type].level , "workMan"));
			}
			var i:int = e.currentTarget.name.split("_")[1];
			var typeArr:Array = [];
			manCurrentArr[i] = int(e.currentTarget.text);
			costMoney();
		}
		
		/** 金钱转换 */
		private function costMoney():void
		{
			totalMoney = (manCurrentArr[0] + manCurrentArr[1] + manCurrentArr[2]) * 10000;//每金一人
			showMoney(0 , true);
			showMoney(totalMoney);
		}
		/** 显示金钱 */
		private function showMoney(num:int , isclear:Boolean = false):void
		{
			initMoney = (manInitArr[0] + manInitArr[1] + manInitArr[2]) * 10000;
			if(num > initMoney)
			{
				unityHireView.mcBindCos.txtMoney.text = UIUtils.getMoneyStandInfo(int(num - initMoney) , ["\\se","\\ss","\\sc"]);	//显示本次消耗资金
			}
			else unityHireView.mcBindCos.txtMoney.text = UIUtils.getMoneyStandInfo(0 , ["\\se","\\ss","\\sc"]);						//本次消耗为0
			unityHireView.mcBind.txtMoney.text   = UIUtils.getMoneyStandInfo(num, ["\\se","\\ss","\\sc"]);						//初始化为0
			ShowMoney.ShowIcon(unityHireView.mcBind, unityHireView.mcBind.txtMoney , isclear);									//显示一个小时消耗的金钱
			ShowMoney.ShowIcon(unityHireView.mcBindCos, unityHireView.mcBindCos.txtMoney , isclear);						//显示本次操作需消耗的金钱
		}
		/** 雇佣成功后的数据处理 */
		private function dataHandler(talkInfo:String):void
		{
			var arr:Array = [ GameCommonData.wordDic[ "mod_uni_dat_uni_green_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_white_1" ] ,GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_1" ] ,GameCommonData.wordDic[ "mod_uni_dat_uni_red_1" ] ]; // 青龙堂 白虎堂  玄武堂  朱雀堂
			var type:int;
			for(var i:int = 0; i < 4;i++)
			{
				if(talkInfo.indexOf(arr[i]) >= 0)
				{
					type = i;
				}
			}
			talkInfo = talkInfo.split("：")[1];
			var str_0:String = talkInfo.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_1" ] )[1];  // 建筑工匠
			var str_1:String = str_0.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_2" ] )[1];  // 武师
			var str_2:String = str_1.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_3" ] )[1];  // 商人
			var str_5:String = str_2.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_4" ] )[1];  // 此次修改耗费资金
			
			var str_3:String = str_0.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_2" ] )[0];  // 武师
			var str_4:String = str_1.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_3" ] )[0];  // 商人
			var str_6:String = str_2.split( GameCommonData.wordDic[ "mod_uni_med_hir_dat_4" ] )[0];  // 此次修改耗费资金
			UnityConstData.otherUnityArray[int(type + 1)].craftsmanNum = str_3.slice(0 , int(str_3.length - 2));
			UnityConstData.otherUnityArray[int(type + 1)].masterNum = str_4.slice(0 , int(str_4.length - 2));
			UnityConstData.otherUnityArray[int(type + 1)].businessmanNum = str_6.slice(0 , int(str_6.length - 2));	
			var data:Object = UnityConstData.mainUnityDataObj;
			if(str_5) UnityConstData.mainUnityDataObj.unityMoney -= int(str_5.slice(0 , str_5.length -2)) * 10000;
			var obj:Object = UnityConstData.otherUnityArray[int(type + 1)];
			facade.sendNotification(UnityEvent.UPDATEOTHERDATA , int(type + 1));				//更新分堂的显示数据
			
		}
	}
}