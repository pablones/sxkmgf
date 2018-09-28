package GameUI.Modules.AutoPlay.mediator
{
	import Controller.CooldownController;
	import Controller.PlayerController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.command.AutoEatDragCommand;
	import GameUI.Modules.AutoPlay.command.AutoPlayCommand;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayItemsCommand;
	import GameUI.Modules.AutoPlay.command.AutoPlayPetCommand;
	import GameUI.Modules.AutoPlay.command.AutoPlayTeamCommand;
	import GameUI.Modules.Bag.Command.SetCdData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import Net.ActionSend.AutoPlaySend;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class AutoPlayMediator extends Mediator
	{
		public static const NAME:String="AutoPlayMediator";
		
		private var dataProxy:DataProxy;
		private var basePanel:PanelBase;
		
		/** dataDic[type]=useItem*/
		public var dataDic:Dictionary;
		protected var petLevel:uint=1000;
		protected var time:Timer;
		private var lastTime:Number = 0;
		
		private var lastSaveTick:Array = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1];											//上一次保存的打勾
		private var lastSaveNum:Array = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1];												//上一次保存的数字
		private var lastSaveType:Array = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1];
		
		private var aDefaultNum:Array = [60,40,70,25,40,60,65,1,0,0,0];
		
		private var tickNum:uint = 17;
		private var txtNum:uint = 11;
		private var minArea:uint = 10;
		private var maxArea:uint = 17;
		
		//按钮起始状态
		private var upState:DisplayObject;
		private var overState:DisplayObject;
		
			
		public function AutoPlayMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
			this.dataDic=new Dictionary();
		}
		
		public function get viewUI():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return 	[EventList.INITVIEW,
			 AutoPlayEventList.SHOW_AUTOPLAY_UI,
			 AutoPlayEventList.HIDE_AUTOPLAY_UI,
			 AutoPlayEventList.ADD_ITEM_AUTOPLAYUI,
			 AutoPlayEventList.ONSYN_BAG_NUM,
			 AutoPlayEventList.ATT_CHANGE_EVENT,
			 AutoPlayEventList.CANCEL_AUTOPLAY_EVENT,
			 AutoPlayEventList.STOP_DRAG_AUTOPLAY,
			 AutoPlayEventList.AUTO_PLAY_RECEIVE_SERVER,
			 AutoPlayEventList.START_AUTO_PLAY
			 ];			
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"AutoPlayUI"});
					this.viewUI.mouseEnabled=false;
					this.basePanel=new PanelBase(this.viewUI,this.viewUI.width-4,this.viewUI.height+12);
					basePanel.addEventListener(Event.CLOSE, panelCloseHandler);
					basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_med_aut_han_1" ]);//"挂 机"
					( viewUI.txt_commitAndCancel as TextField ).text= GameCommonData.wordDic[ "mod_med_aut_han_2" ];//"开始挂机"
					( viewUI.txt_commitAndCancel as TextField ).mouseEnabled=false;	
					facade.registerCommand( AutoPlayCommand.NAME,AutoPlayCommand );
					facade.registerCommand( AutoEatDragCommand.NAME,AutoEatDragCommand );
					facade.registerCommand( AutoPlayPetCommand.NAME,AutoPlayPetCommand );
					facade.registerCommand( AutoPlayTeamCommand.NAME,AutoPlayTeamCommand );
					facade.registerCommand( AutoPlayItemsCommand.NAME,AutoPlayItemsCommand );
					//防止按钮变态
					this.upState = ( viewUI.btn_commit as SimpleButton).upState;
					this.overState = ( viewUI.btn_commit as SimpleButton).overState;
					
					initItemText();					//初始化格子里的文本
					break;
					
				case AutoPlayEventList.SHOW_AUTOPLAY_UI:
					openPanel();
					if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.OPEN_AUTO_PLAY_NEWER_HELP);//通知新手引导
					break;
				case AutoPlayEventList.HIDE_AUTOPLAY_UI:
					dataProxy.autoPlayIsOpen=false;
					this.panelCloseHandler(null);
					break;	
				case AutoPlayEventList.ADD_ITEM_AUTOPLAYUI:
					var obj:Object=notification.getBody();
					this.addItem(obj.type,obj.index,obj.source,obj.target);
					break;	
				case AutoPlayEventList.ONSYN_BAG_NUM:
					var type:uint=uint(notification.getBody());
					if(type==0){                         
						this.onSyn();
						
					}else if(this.dataDic[type]!=null){
						var num:uint=this.onSynNumInBag(type);
						if(num==0){
							var deleteItem:UseItem = dataDic[type] as UseItem;
							deleteUnexistItem( deleteItem );
						}else{
							(this.dataDic[type] as UseItem).Num=this.onSynNumInBag(type);
						}
					}
					break;	
				case AutoPlayEventList.ATT_CHANGE_EVENT:
					sendNotification( AutoEatDragCommand.NAME,{type:notification.getBody()} );
					break;	
				case AutoPlayEventList.CANCEL_AUTOPLAY_EVENT:
					(this.viewUI.txt_commitAndCancel as TextField).text=GameCommonData.wordDic[ "mod_med_aut_han_2" ];//"开始挂机"
					stopAutoPlay();
					break;
					
				case AutoPlayEventList.AUTO_PLAY_RECEIVE_SERVER:
					initPanelSet();
				break;
				case AutoPlayEventList.STOP_DRAG_AUTOPLAY:
					if(basePanel) {
						if( GameCommonData.fullScreen == 2 )
						{
							basePanel.x = UIConstData.DefaultPos1.x+70 + (GameCommonData.GameInstance.MainStage.stageWidth - GameConfigData.GameWidth)/2;;
							basePanel.y = UIConstData.DefaultPos1.y-20 + (GameCommonData.GameInstance.MainStage.stageHeight - GameConfigData.GameHeight)/2;;
						}else{
							basePanel.x = UIConstData.DefaultPos1.x+70;
							basePanel.y = UIConstData.DefaultPos1.y-20;
						}
						basePanel.IsDrag = false;
					}
					break;
				case AutoPlayEventList.START_AUTO_PLAY:
					if ( GameCommonData.Player.IsAutomatism )
					{
						stopAutoPlay();
					}
					else
					{
						startAutoPlay();
					}
				break;
			}
		}
		
		private function initPanelSet():void
		{
			this.dataDic = new Dictionary();
			setArrowState( AutoPlayData.aSaveTick );				//设置打勾
			setTxtNum( AutoPlayData.aSaveNum );					//设置文本
			setItem();																	//设置物品
		}
		
		private function openPanel():void
		{
			dataProxy.autoPlayIsOpen=true;
			if( GameCommonData.fullScreen == 2 )
			{
				basePanel.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - basePanel.width)/2;;
				basePanel.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - basePanel.height)/2;
			}else{
				basePanel.x = UIConstData.DefaultPos1.x+70;
				basePanel.y = UIConstData.DefaultPos1.y-20;
			}
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
			sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
			//侦听
			initBtn();						
			initChgSet();							
		}
		
		//初始化按钮
		private function initBtn():void
		{
			( viewUI.autoSet_btn as SimpleButton).addEventListener(MouseEvent.CLICK,autoSetHandler);
			( viewUI.saveSet_btn as SimpleButton).addEventListener(MouseEvent.CLICK,saveSetHandler);
			( viewUI.btn_commit as SimpleButton).addEventListener(MouseEvent.CLICK,onCommitClickHandler);
			( viewUI.btn_commit as SimpleButton).addEventListener( MouseEvent.MOUSE_OVER,overCommitHandler );
			
		}
		
		private function overCommitHandler(evt:MouseEvent):void
		{
			( viewUI.btn_commit as SimpleButton).overState = this.overState;
			( viewUI.btn_commit as SimpleButton).removeEventListener( MouseEvent.MOUSE_OVER,overCommitHandler );
		}
		
		private function initItemText():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				viewUI[ "autoPlayHp_"+i ].addChild( creatTwoTxt(GameCommonData.wordDic[ "mod_med_aut_ini_1" ]) );//"红药"
			}
			for ( var j:uint=0; j<3; j++ )
			{
				viewUI[ "autoPlayMp_"+j ].addChild( creatTwoTxt(GameCommonData.wordDic[ "mod_med_aut_ini_2" ]) );//"蓝药"
			}
			for ( var k:uint=0; k<3; k++ )
			{
				viewUI[ "autoPlayPetHp_"+k ].addChild( creatFourTxt(GameCommonData.wordDic[ "mod_med_aut_ini_3" ]) );//"宠物口粮"
			}
			for ( var l:uint=0; l<2; l++ )
			{
				viewUI[ "autoPlayPetToy_"+l ].addChild( creatFourTxt(GameCommonData.wordDic[ "mod_med_aut_ini_4" ]) );//"宠物玩具"
			}
		}
		
		private function initChgSet():void
		{
			for ( var i:uint=0; i<tickNum; i++ )
			{
				viewUI["arrow_"+i].buttonMode = true;
				viewUI["arrow_"+i].addEventListener( MouseEvent.CLICK,chgArrowFrame );
			}
			
			for ( var j:uint=0; j<txtNum; j++ )
			{
				viewUI["txt_"+j].restrict="0-9";
				viewUI["txt_"+j].maxChars = 2;
				UIUtils.addFocusLis( viewUI["txt_"+j] );
				viewUI["txt_"+j].addEventListener( Event.CHANGE,chgNumHandler );
				viewUI["txt_"+j].addEventListener( FocusEvent.FOCUS_OUT,onTextFieldFocusOut );
			}
			
			//挂机范围设置
			for ( var k:uint=1; k<4; k++ )
			{
				viewUI["zone_"+k].buttonMode = true;
				viewUI["zone_"+k].addEventListener( MouseEvent.CLICK,chgZoneFrame );
			}
			viewUI["txt_7"].mouseEnabled = false;
		}
		
		private function chgZoneFrame(evt:MouseEvent):void
		{
			resetZoneTick();
			evt.target.gotoAndStop( 2 );
			var index:uint = uint( evt.target.name.split("_")[1] );
			AutoPlayData.aSaveNum[7] = index;
		}
		
		private function chgArrowFrame(evt:MouseEvent):void
		{
			var index:uint = uint( evt.target.name.split("_")[1] );
			if ( GameCommonData.Player.Role.Level <= 20 && ( index==14 || index==15 ) )
			{
				if ( evt.target.currentFrame == 1 )
				{
					showHint( GameCommonData.wordDic[ "mod_med_aut_chgA" ] );//"人物等级在20级或以下可免费自动复活"
					return;
				}
			}
			var curFrame:uint = evt.target.currentFrame;
			var newFrame:uint;
			curFrame == 1 ? newFrame=2 : newFrame=1;
			evt.target.gotoAndStop( newFrame );
			AutoPlayData.aSaveTick[ index ] = newFrame-1;
		}
		
		private function chgNumHandler(evt:Event):void
		{
			var num:int = int( evt.target.text );
			var min:uint;
			var max:uint;
			var index:uint = uint( evt.target.name.split("_")[1] );
			switch ( index )
			{	
				case 8:									//天灵丹
					min = 0;
					max = BagData.hasItemNum( 501000 );
					if ( num>max && max==0 ) showHint( GameCommonData.wordDic[ "mod_med_aut_chgN_1" ] );//"你没有天灵丹"
				break;
				
				case 9:								//地灵丹
					min = 0;
					max = BagData.hasItemNum( 501003 );
					if ( num>max && max==0 ) showHint( GameCommonData.wordDic[ "mod_med_aut_chgN_2" ] );//"你没有地灵丹"
				break;
				
				case 10:						//春鸽
					min = 0;
					max = BagData.hasItemNum( 630000 );
					if ( num>max && max==0 ) showHint( GameCommonData.wordDic[ "mod_med_aut_chgN_3" ] );//"你没有春鸽"
				break;
				
				default:
					min = 1;
					max = 99;
				break;
				
			}
			if ( num<min && evt.target.text!="" ) evt.target.text = min;
			if ( num>max && evt.target.text!="" ) evt.target.text = max;	
		}
		
		//设置打勾状态
		private function setArrowState(arr:Array):void
		{
			for ( var i:uint=0; i<tickNum; i++ )
			{
				var frame:uint = arr[i]+1;
				viewUI["arrow_"+i].gotoAndStop( frame );
			}
		}
		
		//设置文本
		private function setTxtNum(arr:Array):void
		{
			for ( var i:uint=0; i<txtNum; i++ )
			{
				if ( i != 7 )
				{
					if ( i == 8 )
					{
						var m1:uint = BagData.hasItemNum( 501000 );
						if ( arr[i] > m1 )
						{
							arr[i] = m1;
						}
					}
					else if ( i == 9 )
					{
						var m2:uint = BagData.hasItemNum( 501003 );
						if ( arr[i] > m2 )
						{
							arr[i] = m2;
						}
					}
					else if ( i == 10 )
					{
						var m3:uint = BagData.hasItemNum( 630000 );
						if ( arr[i] > m3  )
						{
							arr[i] = m3;
						}
					}
					
					viewUI["txt_"+i].text = arr[i].toString();
				}
				else
				{
					resetZoneTick();
					if ( arr[i]>3 ) continue;
					viewUI[ "zone_"+arr[i] ].gotoAndStop(2);
				}
			}
		}
		
		//设置物品
		private function setItem():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				addInitAutoPlay( "autoPlayHp",i,AutoPlayData.aSaveType[i] );
			}
			for ( i=0; i<3; i++ )
			{
				addInitAutoPlay( "autoPlayMp",i,AutoPlayData.aSaveType[i+3] );
			}
			for ( i=0; i<3; i++ )
			{
				addInitAutoPlay( "autoPlayPetHp",i,AutoPlayData.aSaveType[i+6] );
			}
			for ( i=0; i<2; i++ )
			{
				addInitAutoPlay( "autoPlayPetToy",i,AutoPlayData.aSaveType[i+9] );
			}
		}
		
		//自动设置
		private function autoSetHandler(evt:MouseEvent):void
		{
			if ( GameCommonData.Player.Role.Level <= 20 )
			{
				AutoPlayData.aSaveTick = [1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1];
			}
			else
			{
				AutoPlayData.aSaveTick = [1,1,1,1,0,1,1,1,1,0,1,0,0,0,1,1,1];
			}
			
			AutoPlayData.aSaveNum = [60,40,70,25,40,60,65,3,0,0,0];
			setArrowState( AutoPlayData.aSaveTick );
			setTxtNum( AutoPlayData.aSaveNum );
			var a:Array = [];
			var b:Array = [];
			var c:Array = [];
			var d:Array = [];
			AutoPlayData.aSaveType = [0,0,0,0,0,0,0,0,0,0,0];	
			
			var itemType:uint;
			var useLevel:uint;
			for each(var obj:*  in (BagData.AllItems[0] as Array) )
			{
				if ( !obj ) continue;
				itemType = obj.type;
				useLevel = UIConstData.getItem( obj.type ).Level ;
				if ( itemType>=300001 && itemType<=300020 && ( a.indexOf( itemType )==-1 ) && ( GameCommonData.Player.Role.Level >= useLevel )  )
				{
					a.push( itemType );
				}
				else if ( itemType>=310001 && itemType<=310020 && ( b.indexOf( itemType )==-1 ) && ( GameCommonData.Player.Role.Level >= useLevel ) )
				{
					b.push( itemType );
				}
				else if ( itemType>=320001 && itemType<=320020 && ( c.indexOf( itemType )==-1 ) && ( GameCommonData.Player.Role.Level >= useLevel ) )
				{
					c.push( itemType );
				}
				else if ( itemType>=330000 && itemType<340000 && ( d.indexOf( itemType )==-1 ) )
				{
					d.push( itemType );
				}
			}
			a.sort( Array.NUMERIC );
			b.sort( Array.NUMERIC );
			c.sort( Array.NUMERIC );
			d.sort( Array.NUMERIC );
			
			for ( var i:uint=0; i<a.length; i++ )
			{
				if ( i<3 ) AutoPlayData.aSaveType[i] = a[i];
			}
			for ( i=0; i<b.length; i++ )
			{
				if ( i<3 ) AutoPlayData.aSaveType[i+3] = b[i];
			}
			for ( i=0; i<c.length; i++ )
			{
				if ( i<3 ) AutoPlayData.aSaveType[i+6] = c[i];
			}
			for ( i=0; i<d.length; i++ )
			{
				if ( i<2 ) AutoPlayData.aSaveType[i+9] = d[i];
			}
			dataDic = new Dictionary();
			clearParent( "autoPlayHp",3 );
			clearParent( "autoPlayMp",3 );
			clearParent( "autoPlayPetHp",3 );
			clearParent( "autoPlayPetToy",2 );
			setItem();	
			sendDataToServer();
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_AUTO_SET_NEWER_HELP);
		}
		
		//保存设置数据
		private function saveSetHandler(evt:MouseEvent):void
		{
			for ( var i:uint=0; i<tickNum; i++ )
			{
				var frame:uint = viewUI["arrow_"+i].currentFrame-1;
				AutoPlayData.aSaveTick[i] =  frame;
			}
			
			for ( var j:uint=0; j<txtNum; j++ )
			{
				if ( j != 7 )
				{
					var num:int = int( viewUI["txt_"+j].text );
					AutoPlayData.aSaveNum[j] = num;		
				}
				else
				{
					for ( var k:uint=1; k<4; k++ )
					{
						if ( viewUI["zone_"+k].currentFrame == 2 )
						{
							AutoPlayData.aSaveNum[7] = k;
							break;
						}
					}
				}
			}
//			trace ( " aTick  "+AutoPlayData.aSaveTick,"  aNum "+AutoPlayData.aSaveNum,  " aType "+AutoPlayData.aSaveType );
			sendDataToServer();
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_SAVE_SET_NEWER_HELP);//通知新手引导
		}
		
		//发数据给服务器
		private function sendDataToServer():void
		{
			if ( lastTime == 0 )
			{
				lastTime = getTimer();
				realySend();		
			}
			else
			{
				var newTime:Number = getTimer();
				if ( (newTime-lastTime)>5000 )
				{
					lastTime = newTime;
					realySend();	
				}
			}	
		}
		
		private function realySend():void
		{
			if ( !isSameAsLastTime() )
			{
//			trace ( "send" );
				lastSaveNum = AutoPlayData.aSaveNum.slice( 0,AutoPlayData.aSaveNum.length );
				lastSaveTick = AutoPlayData.aSaveTick.slice( 0,AutoPlayData.aSaveTick.length );
				lastSaveType = AutoPlayData.aSaveType.slice( 0,AutoPlayData.aSaveType.length );
				AutoPlaySend.send();
			}
		}
		
		//判断是否和上次发送内容相同
		private function isSameAsLastTime():Boolean
		{
			for ( var i:uint=0; i<AutoPlayData.aSaveNum.length; i++ )
			{
				if ( AutoPlayData.aSaveNum[i] != lastSaveNum[i] ) return false;
			}
			for ( i=0; i<AutoPlayData.aSaveTick.length; i++ )
			{
				if ( AutoPlayData.aSaveTick[i] != lastSaveTick[i] ) return false;
			}
			for ( i=0; i<AutoPlayData.aSaveType.length; i++ )
			{
				if ( AutoPlayData.aSaveType[i] != lastSaveType[i] ) return false;
			}
			return true;
		}
		
		private function onTextFieldFocusOut(e:FocusEvent):void
		{
			var index:uint = e.target.name.split("_")[1];
			var num:uint = uint( e.target.text );
			if ( e.target.text == "" )
			{
				e.target.text = aDefaultNum[index];
			}
			
			if ( index == 6 )
			{
				if ( num<60 ) e.target.text = "60";
			}
			AutoPlayData.aSaveNum[ index ] = uint( e.target.text );
		}
		
		//开始挂机
		private function startAutoPlay():void
		{
			PlayerController.BeginAutomatism();
			if ( viewUI )
			{
				( viewUI.txt_commitAndCancel as TextField ).text=GameCommonData.wordDic[ "mod_med_aut_sta_1" ];//"停止挂机"
			}
			showHint(GameCommonData.wordDic[ "mod_med_aut_sta_2" ] );//"开始挂机，快捷键B" 
			//关闭打造
			if ( dataProxy.EquipManufatoryIsOpen )
			{
				sendNotification( ManufactoryData.CLOSE_MANUFACTORY_UI );
			}
			sendNotification( AutoPlayCommand.NAME );
		}
		
		//停止挂机
		private function stopAutoPlay():void
		{
			PlayerController.EndAutomatism();
			if ( viewUI )
			{
				( viewUI.txt_commitAndCancel as TextField ).text=GameCommonData.wordDic[ "mod_med_aut_sto" ];	//"开始挂机"
			}
			sendNotification( AutoPlayCommand.NAME );
		}
		
		/**
		 * 同步背包数量 
		 * 
		 */		
		private function onSyn():void{
			for (var type:* in this.dataDic){
				if(this.dataDic[type]==null){
					delete this.dataDic[type];
					continue;
				}
				var num:uint=this.onSynNumInBag(type);
				if(num==0){
					var item:UseItem = dataDic[type] as UseItem;
					deleteUnexistItem( item );
				}else{
					(this.dataDic[type] as UseItem).Num=this.onSynNumInBag(type);
				}
			}
			
		}	
		
		private function onCommitClickHandler(e:MouseEvent):void{
			if(!GameCommonData.Player.IsAutomatism ) 
			{
				startAutoPlay();
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_START_AUTO_PLAY_NEWER_HELP);
			}
			else
			{
				stopAutoPlay();
			}
			this.panelCloseHandler(null);
		}
			
		/**
		 * 关闲 
		 * @param e
		 * 
		 */		
		protected function panelCloseHandler(e:Event):void{
			( viewUI.btn_commit as SimpleButton).removeEventListener(MouseEvent.CLICK,onCommitClickHandler);
			( viewUI.autoSet_btn as SimpleButton).removeEventListener(MouseEvent.CLICK,autoSetHandler);
			( viewUI.saveSet_btn as SimpleButton).removeEventListener(MouseEvent.CLICK,saveSetHandler);
			( viewUI.btn_commit as SimpleButton).overState = this.upState;
			basePanel.IsDrag = true;	
			if(this.basePanel.parent!=null)
			{
				this.basePanel.parent.removeChild(this.basePanel);
				this.dataProxy.autoPlayIsOpen=false;
			}
			for ( var i:uint=0; i<tickNum; i++ )
			{
				viewUI["arrow_"+i].removeEventListener( MouseEvent.CLICK,chgArrowFrame );
			}
			
			for ( var j:uint=0; j<txtNum; j++ )
			{
				viewUI["txt_"+j].removeEventListener( Event.CHANGE,chgNumHandler );
				viewUI["txt_"+j].removeEventListener( FocusEvent.FOCUS_OUT,onTextFieldFocusOut );
			}
			for ( var k:uint=1; k<4; k++ )
			{
				viewUI["zone_"+k].removeEventListener( MouseEvent.CLICK,chgZoneFrame );
			}
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLOSE_AUTO_PLAY_NEWER_HELP);//通知新手引导
		}
		
		/**
		 * 添加物品
		 * 
		 */		
		protected function addItem(type:String,index:uint,source:UseItem,target:MovieClip):void{
			var useItem:UseItem;
			if(this.isHasTheItem(source.Type))
			{
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_aut_add" ], color:0xffff00});//"已经有该物品了"
				return; 
			}
//			trace ( "type : "+type,"  index  "+index,"  source   "+source," target: "+target );
			switch(type){
				case "autoPlayHp":
					if(this.isRightItem(source.Type,1)!=1){
						this.showTip(this.isRightItem(source.Type,1));
						return;
					}else{
						AutoPlayData.aSaveType[ index ] = source.Type;
					}
					break;
				case "autoPlayMp":
					if(this.isRightItem(source.Type,2)!=1){
						this.showTip(this.isRightItem(source.Type,2));
						return;
					}else{
						AutoPlayData.aSaveType[ index+3 ] = source.Type;
					}
					break;
				case "autoPlayPetHp":
					if(this.isRightItem(source.Type,3)!=1){
						this.showTip(this.isRightItem(source.Type,3));
						return;
					}else{
						AutoPlayData.aSaveType[ index+6 ] = source.Type;
					}
					break;	
				case "autoPlayPetToy":
					if(this.isRightItem(source.Type,4)!=1){
						this.showTip(this.isRightItem(source.Type,3));
						return;
					}else{
						AutoPlayData.aSaveType[ index+9 ] = source.Type;
					}
					break;
			}
			useItem=new UseItem(source.Pos,String(source.Type),null);
			useItem.name="AutoPlay_"+source.Type;
			useItem.Id=source.Id;
			useItem.addEventListener(MouseEvent.MOUSE_DOWN,onUseItemMouseDown);
			useItem.mouseEnabled=true;
			useItem.addEventListener(DropEvent.DRAG_THREW,onThrowItemHandler);
			useItem.addEventListener( DropEvent.DRAG_DROPPED,onDrop );
			useItem.addEventListener( MouseEvent.CLICK,doubleClickHandler );
			while(target.numChildren>2){
				target.removeChildAt(2);
			}
			target.addChild(useItem);
			useItem.x=useItem.y=2;
			this.dataDic[source.Type]=useItem;
			var cdObj:Object=SetCdData.searchCdObjByType(source.Type);
			if(cdObj!=null){
//				useItem.startCd(cdObj.cdtimer,cdObj.count);
				CooldownController.getInstance().triggerCD(useItem.Type, cdObj.cdtimer);
			}
			useItem.Num=this.onSynNumInBag(useItem.Type);	
		}
		
		//双击操作
		private function doubleClickHandler(evt:MouseEvent):void
		{
			var item:ItemBase=evt.target as ItemBase;
		 	deleteUnexistItem( item );
		}
		
	/**
		 * 玩家登录时初始化挂机操作 
		 * @param type
		 * @param index
		 * @param itemType
		 * 
		 */		
		public function addInitAutoPlay(type:String,index:uint,itemType:uint):void{
			if(itemType==0 ||this.onSynNumInBag(itemType)==0)return;
			if(this.isHasTheItem(itemType)){
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_aut_addIt" ], color:0xffff00});//"已经有该物品了"
				return; 
			}
			var useItem:UseItem;
			var target:MovieClip;
			switch (type){
				case "autoPlayHp":
					target=this.viewUI["autoPlayHp_"+index];
					break;
				case "autoPlayMp":
					target=this.viewUI["autoPlayMp_"+index];
					break;
				case "autoPlayPetHp":	
					target=this.viewUI["autoPlayPetHp_"+index];
					break;
				case "autoPlayPetToy":
					target=this.viewUI["autoPlayPetToy_"+index];
					break;	
			}
			
			useItem=new UseItem(0,String(itemType),null);
			useItem.name="AutoPlay_"+itemType;
			useItem.addEventListener(MouseEvent.MOUSE_DOWN,onUseItemMouseDown);
			useItem.mouseEnabled=true;
			useItem.addEventListener(DropEvent.DRAG_THREW,onThrowItemHandler);
			useItem.addEventListener( DropEvent.DRAG_DROPPED,onDrop );
			useItem.addEventListener( MouseEvent.CLICK,doubleClickHandler );
			while(target.numChildren>2){
				target.removeChildAt(2);
			}
			target.addChild(useItem);
			useItem.x=useItem.y=2;
			this.dataDic[itemType]=useItem;
			useItem.Num=this.onSynNumInBag(itemType);
		}
		
		private function onDrop( evt:DropEvent ):void
		{
			var parent:DisplayObjectContainer = evt.target.parent;
			var target:MovieClip;
			if ( evt.Data.target is MovieClip )
			{
				target = evt.Data.target;
			}
			else if ( evt.Data.target.parent is MovieClip )
			{
				target = evt.Data.target.parent;
			}
			else
			{
				return;
			}
			
			var oldItem:ItemBase;
			var newItem:ItemBase = evt.target as ItemBase;
			var newName:String= target.name.split("_")[0];
			var newIndex:uint = uint( target.name.split("_")[1] );
			var oldName:String = parent.name.split("_")[0];
			var oldIndex:uint = uint( parent.name.split("_")[1] );
			if ( oldName == newName )
			{
				for ( var i:uint=0; i<target.numChildren; i++ )
				{
					if ( target.getChildAt(i) is ItemBase )
					{
						oldItem = target.getChildAt(i) as ItemBase;
						break;
					}
				}
				if ( oldItem ) 
				{
					parent.addChild( oldItem );
					arrangArr( oldName,oldIndex,oldItem.Type );
				}
				else
				{
					arrangArr( oldName,oldIndex,0 );
				}
				target.addChild( newItem );
				arrangArr( newName,newIndex,newItem.Type );
			}
		}
		
		/**判断是否已经有该物品了 */
		private function isHasTheItem(itemType:uint):Boolean
		{
			if(this.dataDic[itemType]!=null)return true;
			return false;
		}
		
		
		/**
		 * 和背包中同步一下物品的数量 
		 * @param itemType
		 * @return 
		 * 
		 */		
		private function onSynNumInBag(itemType:uint):uint{
			var count:uint=0;
			for each(var obj:*  in (BagData.AllItems[0] as Array)){
				if(obj==null)continue ;
				if(obj.type==itemType){
					count+=obj.amount;
				}		
			}
			return count;
		}
		
		/**
		 * 鼠标按下，进行拖动 
		 * @param e
		 * 
		 */		 
		private function onUseItemMouseDown(e:MouseEvent):void
		{
			var item:UseItem=e.currentTarget as UseItem;
			item.onMouseDown();
		}
		
		/**
		 * 丢弃挂机中的物品 
		 * @param e
		 * 
		 */		
		private function onThrowItemHandler(e:DropEvent):void
		{
		 	var item:ItemBase=e.Data as ItemBase;
		 	deleteUnexistItem( item );
		 }
		
		/**
		 * 判断是否是正确的物品 
		 * @param itemType
		 * @param type
		 * @return 0:错误的type类型  1：正确的数品    -1：错误的物品类型   -2:使用的物品等级过高   -3：错误的数据  -4：没有出战的宠物
		 * 
		 */		
		private function isRightItem(itemType:uint,type:uint):int{
			var obj:Object=UIConstData.getItem(itemType); 
			var playerLevel:uint=GameCommonData.Player.Role.Level;
			var pLevel:uint;
			switch(type){
				case 1:
					if(itemType>=300001 && itemType<=300020){
						if(obj!=null){
							if(obj.Level>playerLevel){
								return -2;
							}else{
								return 1;
							}
						}else{
							return -3;
						}		
					}else{
						return -1;
					}
					break;
				case 2:
					if(itemType>=310001 && itemType<=310020){
						if(obj!=null){
							if(obj.Level>playerLevel){
								return -2;
							}else{
								return 1;
							}
						}else{
							return -3;
						}		
					}else{
						return -1;
					}
					break;
				case 3:
					if(itemType>=320001 && itemType<=320020){
						if(obj!=null){
							if(playerLevel<obj.Level){
								return -2;
							}else{
								return 1;
							}	
						}else{
							return -3;
						}
					}else{
						return -1;
					}
					break;
				case 4:
					if(itemType>=330000 && itemType<340000){
						if(obj!=null){
							if(playerLevel<obj.Level){
								return -2;
							}else{
								return 1;
							}	
						}else{
							return -3;
						}
					}else{
						return -1;
					}
					break;	
			} 
			return 0;
		}
		
		protected function showTip(type:int):void
		{
			switch(type){
				case -1:
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_aut_show_1" ], color:0xffff00});//"放入的物品类型不对"
					break;
				case -2:
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_aut_show_2" ], color:0xffff00});//"放入的物品等级过高"
					break;
				case -3:
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_aut_show_3" ], color:0xffff00});//"错误的数据"
					break;
				case -4:
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_med_aut_show_4" ], color:0xffff00});//"当前没有出战的宠物"
					break;
					
			}
		}
		
		//删除不存在的type物品
		private function deleteUnexistItem( item:ItemBase ):void
		{
			if( item && item.parent!=null )
		 	{
		 		item.removeEventListener( DropEvent.DRAG_DROPPED,onDrop );
		 		item.removeEventListener(DropEvent.DRAG_THREW,onThrowItemHandler);		
		 		item.removeEventListener(MouseEvent.MOUSE_DOWN,onUseItemMouseDown);
//		 		item.removeEventListener( MouseEvent.DOUBLE_CLICK,doubleClickHandler );
				item.removeEventListener( MouseEvent.CLICK,doubleClickHandler );
		 		
		 		var index:uint = uint( item.parent.name.split("_")[1] );
		 		var kind:String = item.parent.name.split("_")[0];
		 		arrangArr( kind,index,0 );
		 		
		 		item.parent.removeChild(item);
		 		delete this.dataDic[item.Type];
				item = null;
		 	}	
		}
		
		//重新排列数组
		private function arrangArr( kind:String,index:uint,type:uint ):void
		{
			switch ( kind )
			{
				case "autoPlayHp":
		 				AutoPlayData.aSaveType[ index ] = type; 
		 			break;
		 			case "autoPlayMp":
		 				AutoPlayData.aSaveType[ index+3 ] = type; 
		 			break;
		 			case "autoPlayPetHp":
		 				AutoPlayData.aSaveType[ index+6 ] = type; 
		 			break;
		 			case "autoPlayPetToy":
		 				AutoPlayData.aSaveType[ index+9 ] = type; 
		 			break;
			}
		}
		
		//创建2字文本
		private function creatTwoTxt(info:String):TextField
		{
			var txt:TextField = new TextField();
			txt.width = 28;
			txt.height = 16;
			txt.text = info;
			txt.x = 5;
			txt.y = 10;
			txt.mouseEnabled = false;
			txt.setTextFormat( UIUtils.getTextFormat() );
			return txt;
		}
		
		//创建4字文本
		private function creatFourTxt(info:String):TextField
		{
			var txt:TextField = new TextField();
			txt.width = 28;
			txt.height = 30;
			txt.text = info;
			txt.x = 5;
			txt.y = 4;
			txt.mouseEnabled = false;
			txt.multiline = true;
			txt.wordWrap = true;
			txt.setTextFormat( UIUtils.getTextFormat() );
			return txt;
		}
		
		private function showHint(info:String):void
		{
			sendNotification(HintEvents.RECEIVEINFO, {info:info, color:0xffff00});
		}
		
		private function resetZoneTick():void
		{
			for ( var k:uint=1; k<4; k++ )
			{
				viewUI["zone_"+k].gotoAndStop(1);
			}
		}
		
		//移除所有
		private function clearParent( title:String,num:uint ):void
		{
			var aItem:Array = [];
			for ( var i:uint=0; i<num; i++ )
			{
				var itemParent:DisplayObjectContainer = this.viewUI[ title + "_"+i.toString() ];
				while ( itemParent.numChildren>2 )
				{
					itemParent.removeChildAt( 2 );
				}
			}
			
//			for ( var j:uint=0; j<aItem.length; j++ )
//			{
//				if ( aItem[i].parent )
//				{
//					aItem[i].parent.removeChild( aItem[i] );			
//				}
//			}
		}
	
	}
}