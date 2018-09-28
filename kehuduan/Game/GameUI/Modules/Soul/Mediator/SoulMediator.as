package GameUI.Modules.Soul.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.Soul.Proxy.ShowSoulComponent;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.Soul.View.SoulComponents;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**
	 * @author lh
	 * 魂魄主界面管理类
	 */	
	public class SoulMediator extends Mediator
	{
		public static const NAME:String = "SoulMediator";
		private var dataProxy:DataProxy;
		private var parentView:MovieClip = null;
		public static const TYPE:int = 2;
		private var useItem:UseItem;	//魂魄图片
		public static var soulVO:SoulVO = new SoulVO();	//当前装备的魂魄VO
		public static var isFirstUseOrChangeSoul:Boolean = false;	//第一次装备魂魄（或开启面板安装）
		private var showSoulComponent:ShowSoulComponent;//魂魄Update
		public static var isEquiptSoul:Boolean = false;	//是否装备了魂魄
		private var soulStyleMc:MovieClip; //属相
		private var redFrameShap:Shape;
		private var mianSoulView:MovieClip;//魂魄面板
		private var noSoulView:MovieClip;//无魂魄时候的显示面板
		private var tempShowStyle:int = 3;
		public function SoulMediator(parentMc:MovieClip)
		{
			parentView = parentMc;
			super(NAME);
		}
		
		public function get soulView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		public function get soulComposeView():MovieClip
		{
			return this.noSoulView.showSoulBackGround as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				SoulProxy.INITSOULMEDIATOR,
				SoulProxy.SHOW_AFTER_GET_INFO,
				RoleEvents.SHOWPROPELEMENT,
				SoulProxy.SHOW_SOULVIEW,
				NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP
				
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case SoulProxy.INITSOULMEDIATOR:
					if(!this.soulView)
					{
						mianSoulView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("soulMainView");
						noSoulView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("noSoulView");
						mianSoulView.name = "soulView";
						this.setViewComponent(mianSoulView);
						facade.registerProxy(new SoulProxy());
					}
					initView();
				break;
				case RoleEvents.SHOWPROPELEMENT:
					if(notification.getBody() as int != TYPE) 
					{
						return;	
					}
					showView();
				break;
				case SoulProxy.SHOW_AFTER_GET_INFO:
					addViewToStage();
				break;
				case NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP:
					this.gc();
				break;
			}
		}
		
		private function initView():void
		{
			if(!dataProxy)
			{
				dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			}
			
			this.soulView.mouseEnabled = false;
			for(var j:int = 0; j < 10; j ++)
			{
				(this.soulView["txt_extendPro" +j] as TextField).mouseEnabled = false;
				(this.soulView["txt_extendHas1_" +j] as TextField).mouseEnabled = false;
				(this.soulView["txt_extendHas2_" +j] as TextField).mouseEnabled = false;
			}
			
			//魂魄技能格子初始化
			for(var y:int = 0; y < 3; y++)
			{
				(this.soulView["soul_skill_"+y] as MovieClip).buttonMode = true;
				(this.soulView["soul_skill_"+y] as MovieClip).addEventListener(MouseEvent.MOUSE_OVER,onGridMouseOver);
			}
			 
			 if(GameCommonData.wordVersion == 2)
			 {
			 	//繁体版
				(this.noSoulView.txt_soulLink as TextField).htmlText =  "<font color = '#E2CCA5'>方法一：<a href='\event:\'><font color = '#00FF00'>點擊這裡</font></a>進入商城，購買<font color = '#00FFFF'>魂魄禮包</font>打開獲得1級 </font>";
			 }
			 else
			 {
				(this.noSoulView.txt_soulLink as TextField).htmlText = '<a href="event:'+GameCommonData.wordDic[ "mod_soul_med_soulM_initV" ]+'">'+GameCommonData.wordDic[ "mod_soul_med_soulM_initV" ]+'</a>';//点击这里	点击这里
			 }
			(this.noSoulView.txt_soulLink as TextField).selectable = false;
			(this.noSoulView.txt_soulLink as TextField).addEventListener(TextEvent.LINK,onSoulLink);
			
			facade.registerProxy(new ShowSoulComponent());
			showSoulComponent = facade.retrieveProxy(ShowSoulComponent.NAME) as ShowSoulComponent;
			initAllMediator();
		}
		
		private function showView():void
		{
			
			if(RolePropDatas.ItemList[15])
			{
				isEquiptSoul = true;
				SoulProxy.getSoulDetailInfo();	
			}
			else
			{
				addViewToStage();
			}
		}
		
		/**
		 *进入场景 
		 * 
		 */		
		private function addViewToStage():void
		{
			if(isEquiptSoul)
			{
				if(parentView.contains(this.noSoulView))
				{
					parentView.removeChild(noSoulView);
				}
				showSoulModel(); //舞台显示
				parentView.addChildAt(soulView, 4);
				redFrameShap = new Shape();
				this.initExtendPropertyView();
				setViewData();
			}
			else
			{
				if(parentView.contains(this.mianSoulView))
				{
					parentView.removeChild(mianSoulView);
				}
				showComposeSoulModel(tempShowStyle);
				parentView.addChildAt(noSoulView, 4);
			}
			addEventListeners();
		}		
		 
		/**
		 * 给靓仔 
		 * 
		 */		
		private function showSoulModel():void
		{
			showSoulComponent.deleteView();
			if(RolePropDatas.ItemList[15])
			{
				var obj:Object = {};
				obj.composeLevel = soulVO.composeLevel;
				obj.style = soulVO.style;
				showSoulComponent.showView(obj);
			}
		}
		
		private function showComposeSoulModel(style:int):void
		{
			showSoulComponent.deleteComposeView();
			var obj:Object = {};
			if(style == 1)
			{
				obj.composeLevel = 5;
				obj.style = 1;
			}
			else if(style == 2)
			{
				obj.composeLevel = 5;
				obj.style = 2;
			}
			else if(style == 3)
			{
				obj.composeLevel = 5;
				obj.style = 3;
			}
			else if(style == 4)
			{
				obj.composeLevel = 5;
				obj.style = 4;
			}
			showSoulComponent.showComposeView(obj);
		}
		
		/**
		 *注册魂魄 相关的mediator 
		 * @return 
		 * 
		 */		
		private function initAllMediator():void
		{
			initSoulMediator(ComposeRunStoneMediator);
			initSoulMediator(ComposeSoulMediator);
			initSoulMediator(ComposeTongStroneMediator);
			initSoulMediator(GrowUpPercentMediator);
			initSoulMediator(ImproveExtendProMediator);
			initSoulMediator(ImproveSkillMediator);
			initSoulMediator(LearnExtendProMediator);
			initSoulMediator(LearnSoulSkillMediator);
			initSoulMediator(RepeatExtendProMediator);
			initSoulMediator(RepeatSkillMediator);
			initSoulMediator(RepeatStyleMediator);
			initSoulMediator(UseExtendGrooveMediator);
		}
		
		private function initSoulMediator(mediatorClass:Class):void
		{
			if(!facade.hasMediator(mediatorClass.NAME))
			{
				facade.registerMediator(new mediatorClass());
				facade.sendNotification(mediatorClass.INITMEDIATOR);
			}

		}
		
		private function addEventListeners():void
		{
			this.soulView.btn_soulHelp.addEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_grow.addEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_compose.addEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_repeaProperty.addEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_repeatSkill.addEventListener(MouseEvent.CLICK,onMoseClick);
			
			(this.noSoulView.btn_noSoulCompose as DisplayObject).addEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showGround as DisplayObject).addEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showWater as DisplayObject).addEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showFire as DisplayObject).addEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showWind as DisplayObject).addEventListener(MouseEvent.CLICK,onMoseClick);
		}
		private function removeEventListeners():void
		{
			this.soulView.btn_soulHelp.removeEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_grow.removeEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_compose.removeEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_repeaProperty.removeEventListener(MouseEvent.CLICK,onMoseClick);
			this.soulView.btn_repeatSkill.removeEventListener(MouseEvent.CLICK,onMoseClick);
			
			(this.noSoulView.btn_noSoulCompose as DisplayObject).removeEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showGround as DisplayObject).removeEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showWater as DisplayObject).removeEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showFire as DisplayObject).removeEventListener(MouseEvent.CLICK,onMoseClick);
			(this.noSoulView.btn_showWind as DisplayObject).removeEventListener(MouseEvent.CLICK,onMoseClick);
		}
		
		/**
		 *初始化扩展属性按钮 文本框 
		 * 
		 */		
		private function initExtendPropertyView():void
		{
			if(RolePropDatas.ItemList[15])
			{
				isEquiptSoul = true;
			}
			else 
			{
				isEquiptSoul = false;
				soulVO = new SoulVO();
			}
			gcSkillContainer();
			var soulcomp:SoulComponents = SoulComponents.getInstance();	//获取按钮组件
			if(isEquiptSoul)
			{
				//魂魄扩展属性条初始化
				for(var i:int = 0; i < 10; i ++)
				{
					var extComp:DisplayObjectContainer = this.soulView["soul_ext_extendPro"+i];	//对应的面板中的扩展属性按钮组件
					
					if(soulVO.extProperties[i] == false)
					{
						var notUseComp:SimpleButton = soulcomp.getNotLearn() 
						notUseComp.name = "soul_notUse_" + i;
						(this.soulView["txt_extendPro" +i] as TextField).text = GameCommonData.wordDic[ "mod_soul_med_soulM_initExt_1" ]+(i+1)+GameCommonData.wordDic[ "mod_soul_med_soulM_initExt_2" ];//"合成等级"	"时可开槽"
						(this.soulView["txt_extendPro" +i] as TextField).textColor = 0xE2CCA5;
						notUseComp.mouseEnabled
						extComp.addChild(notUseComp);
					} 
					else
					{
						var extVo:SoulExtPropertyVO = soulVO.extProperties[i];	//获取对应的数据Vo
						if(extVo.state == 0)	//已使用
						{
							var hasLearnComp:SimpleButton = soulcomp.getHasLearn();
							hasLearnComp.name = "soul_hasLearn_" + i;
							this.setHasLearnTxt(extVo);
							(this.soulView["txt_extendPro" +i] as TextField).textColor = 0X00FFFF;
							hasLearnComp.addEventListener(MouseEvent.CLICK,onLearnClick);
							extComp.addChild(hasLearnComp);
							
							var btnUpProperty:SimpleButton = soulcomp.getUpProperty();
							btnUpProperty.name = "soul_upProperty_" + i;
							btnUpProperty.addEventListener(MouseEvent.CLICK,onLearnClick);
							extComp.addChild(btnUpProperty);
							btnUpProperty.x =  162;
						}
						else if(extVo.state == 1)	//可学习
						{
							var canLearnComp:SimpleButton = soulcomp.getHasLearn(); 
							canLearnComp.name = "soul_canLearn_" + i;
							(this.soulView["txt_extendPro" +i] as TextField).text = GameCommonData.wordDic[ "mod_soul_med_soulM_initExt_3" ];//"点击学习";
							(this.soulView["txt_extendPro" +i] as TextField).textColor = 0xE2CCA5;
							canLearnComp.addEventListener(MouseEvent.CLICK,onLearnClick);
							extComp.addChild(canLearnComp);
						}
						else if(extVo.state == 2)	//可开槽 
						{
							var notLearnComp:SimpleButton = soulcomp.getCanLearn();
							notLearnComp.name = "soul_canUseToLearn_" + i;
							(this.soulView["txt_extendPro" +i] as TextField).text = GameCommonData.wordDic[ "mod_soul_med_soulM_initExt_4" ];//"点击开槽";
							(this.soulView["txt_extendPro" +i] as TextField).textColor = 0xE2CCA5;
							notLearnComp.addEventListener(MouseEvent.CLICK,onLearnClick);
							extComp.addChild(notLearnComp);
						}
					} 
				}
				
				//魂魄技能格子初始化
				for(var y:int = 0; y < 3; y++)
				{
					var skiData:Array =soulVO.soulSkills; 	
					if(soulVO.soulSkills[y] == false)
					{
						(this.soulView["soul_skill_"+y] as MovieClip).addEventListener(MouseEvent.CLICK,onLearnClick);
						continue;
					}
					else if(soulVO.soulSkills[y] is SoulSkillVO)
					{
						var skillVo:SoulSkillVO = soulVO.soulSkills[y];
						if(skillVo.state == 1)
						{
							(this.soulView["soul_skill_"+y] as MovieClip).addEventListener(MouseEvent.CLICK,onLearnClick);
							
						}
						else if(skillVo.state == 0)
						{
							var icon:int = (soulVO.soulSkills[y] as SoulSkillVO).sId;
							var useItem:UseItem = new UseItem(0,icon.toString(),this.soulView["soul_skill_"+y] as DisplayObjectContainer);
							useItem.name = "soul_itemSkill_"+icon;
							useItem.x = 2;
							useItem.y = 2;
							(this.soulView["soul_skill_"+y] as MovieClip).addChild(useItem);
							(this.soulView["soul_skill_"+y] as MovieClip).addEventListener(MouseEvent.CLICK,onLearnClick);
						}
						
					} 
				}
			}
			else
			{
				for(var j:int = 0; j < 10; j ++)
				{
					(this.soulView["txt_extendPro"+j] as TextField).mouseEnabled = false;
					(this.soulView["txt_extendPro"+j] as TextField).text = GameCommonData.wordDic[ "mod_soul_data_sou_soulTool_10" ];//"不可开槽";
					(this.soulView["txt_extendPro"+j] as TextField).textColor = 0xE2CCA5;
					var extComp2:DisplayObjectContainer = this.soulView["soul_ext_extendPro"+j];
					var notUseComp2:SimpleButton = soulcomp.getNotLearn()
					notUseComp2.name = "soul_notUse_" + j;
					extComp2.addChild(notUseComp2);
				}
			}
		}
		
		/**
		 *	扩展属性,技能格子垃圾回收 
		 * @param extCom
		 * 
		 */		
		private function gcSkillContainer():void
		{
			for(var i:int = 0; i < 10; i ++)
			{
				var extComp:DisplayObjectContainer = this.soulView["soul_ext_extendPro"+i];	//对应的面板中的扩展属性按钮组件
				var num:int = extComp.numChildren - 1;
				while(num >= 0)
				{
					var dis:DisplayObject = extComp.getChildAt(num)
					if(dis)
					{
						if(dis.name.split("_")[0] == "soul")
						{
							if(dis.hasEventListener(MouseEvent.CLICK))
							{
								dis.removeEventListener(MouseEvent.CLICK,onLearnClick);
							}
							dis.parent.removeChild(dis);
							dis = null;
						}
					}
					num --;
				}
				(this.soulView["txt_extendPro" +i] as TextField).text = "";
				(this.soulView["txt_extendHas1_" +i] as TextField).text = "";
				(this.soulView["txt_extendHas2_" +i] as TextField).text = "";
			}
			
			for(var j:int = 0; j < 3; j ++)
			{
				
				var grid:DisplayObjectContainer = this.soulView["soul_skill_"+j];	//技能格子
				var num2:int = grid.numChildren - 1;
				while(num2 >= 0)
				{
					var dis2:DisplayObject = grid.getChildAt(num2)
					if(dis2)
					{
						if(dis2.name.split("_")[0] == "soul")
						{
							if(dis2.hasEventListener(MouseEvent.CLICK))
							{
								dis2.removeEventListener(MouseEvent.CLICK,onLearnClick);
							}
							dis2.parent.removeChild(dis2);
							dis2 = null;
						}
					}
					num2 --;
				}
			}
		}
		
		private function setHasLearnTxt(extVo:SoulExtPropertyVO):void
		{
			var color:String;
			
			if(extVo.level <= 1)
			{
				color = IntroConst.itemColors[0];
			} 
			else if(extVo.level <= 2)
			{
				color = IntroConst.itemColors[2];
			}
			else if(extVo.level <= 3)
			{
				color = IntroConst.itemColors[3];
			}
			else if(extVo.level <= 4)
			{
				color = IntroConst.itemColors[4];
			}
			else if(extVo.level <= 10)
			{
				color = IntroConst.itemColors[5];
			}
			(this.soulView["txt_extendHas1_" + extVo.number] as TextField).htmlText = '<font color="'+color+'">'+extVo.name+'('+extVo.level+GameCommonData.wordDic[ "often_used_level" ]+')'+'</font>';//级
			(this.soulView["txt_extendHas2_" + extVo.number] as TextField).htmlText = '<font color="'+color+'">+'+extVo.addProperty+'</font>';
			
		}
		/**
		 * 合成等级修饰
		 * @param percent
		 * @return 
		 * 
		 */		
		private function initLevelTxt(level:int):String
		{
			var descStr:String = "";
			
			if(level <= 1)
			{
				descStr = '<font color="'+IntroConst.itemColors[0]+'">'+level+'</font>';
			}
			else if(level <= 2)
			{
				descStr = '<font color="'+IntroConst.itemColors[2]+'">'+level+'</font>';
			}
			else if(level <= 3)
			{
				descStr = '<font color="'+IntroConst.itemColors[3]+'">'+level+'</font>';
			}
			else if(level <= 4)
			{
				descStr = '<font color="'+IntroConst.itemColors[4]+'">'+level+'</font>';
			}
			else if(level >= 5)
			{
				descStr = '<font color="'+IntroConst.itemColors[5]+'">'+level+'</font>';
			}
			return descStr;
		}
		/**
		 * 成长率文字修饰 
		 * @param percent
		 * @return 
		 * 
		 */		
		private function initPercentTxt(percent:int):String
		{
			var descStr:String;
			if(500 <= percent)
			{
				if(percent <= 649)
				{
					descStr = '<font color="'+IntroConst.itemColors[0]+'">'+percent+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_16" ]+')</font>';//普通
				}
				else if(percent <= 749)
				{
					descStr = '<font color="'+IntroConst.itemColors[2]+'">'+percent+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_15" ]+')</font>';//优秀
				}
				else if(percent <= 849)
				{
					descStr = '<font color="'+IntroConst.itemColors[3]+'">'+percent+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_14" ]+')</font>';//杰出
				}
				else if(percent <= 949)
				{
					descStr = '<font color="'+IntroConst.itemColors[4]+'">'+percent+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ]+')</font>';//卓越
				}
				else if(percent <= 1000)
				{
					descStr = '<font color="'+IntroConst.itemColors[5]+'">'+percent+'('+GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ]+')</font>';//完美
				}
			}
			return descStr;
		}
		
		/**
		 * 初始化面板数据 
		 * 
		 */		
		private function setViewData():void
		{
			
			if(isEquiptSoul)
			{
				var belongStr:String = "";
				if(soulVO.belong == 2)
				{
					belongStr = GameCommonData.wordDic[ "mod_soul_med_soulM_setV_1" ];//"灵力型";
				}
				else if(soulVO.belong == 1)
				{
					belongStr = GameCommonData.wordDic[ "mod_soul_med_soulM_setV_2" ];//"力量型";
				}
				
				if(soulStyleMc)
				{
					if(soulStyleMc.hasEventListener(MouseEvent.CLICK))
					{
						soulStyleMc.removeEventListener(MouseEvent.CLICK,onMoseClick);
					}
					soulStyleMc.parent.removeChild(soulStyleMc);
					soulStyleMc = null;
				}
				var styleStr:String = "";
				if(soulVO.style == 0)
				{
					styleStr = GameCommonData.wordDic[ "often_used_none" ];//"无";
				}
				else
				{
					if(soulVO.style != 0)
					{
						soulStyleMc = SoulComponents.getInstance().getSoulStyle();
						soulStyleMc.name = "soul_style";
						soulStyleMc.buttonMode = true;
						soulStyleMc.addEventListener(MouseEvent.CLICK,onMoseClick);
						this.soulView.addChild(soulStyleMc);
						soulStyleMc.x = 60;
						soulStyleMc.y = 116;
					}
					if(soulVO.style == 1)
					{
						styleStr = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_1" ];//"地";
						soulStyleMc.gotoAndStop(2);
					}
					else if(soulVO.style == 2)
					{
						styleStr = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_2" ];//"水";
						soulStyleMc.gotoAndStop(3);
					}
					else if(soulVO.style == 3)
					{
						styleStr = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_3" ];//"火";
						soulStyleMc.gotoAndStop(1);
					}
					else if(soulVO.style == 4)
					{
						styleStr = GameCommonData.wordDic[ "mod_soul_med_repStyle_initS_4" ];//"风";
						soulStyleMc.gotoAndStop(4);
					}
				}
				
				
				(this.soulView.soul_txt_belong as TextField).text= belongStr;//类型(力量型，灵力型)
				(this.soulView.soul_txt_style as TextField).text= styleStr;//属相
				(this.soulView.soul_txt_life as TextField).text= soulVO.life.toString()+"/300";//寿命
				(this.soulView.soul_txt_level as TextField).text= soulVO.level.toString();//等级
				(this.soulView.soul_txt_exp as TextField).text= soulVO.exp.toString()+"/"+UIConstData.ExpDic[soulVO.level+8000];//经验
				(this.soulView.soul_txt_pro1 as TextField).text= soulVO.phyAttack.toString();//外功
				(this.soulView.soul_txt_pro2 as TextField).text= soulVO.magAttack.toString();//内功
				(this.soulView.soul_txt_pro3 as TextField).text= soulVO.force.toString();//力量
				(this.soulView.soul_txt_pro4 as TextField).text= soulVO.spirit.toString();//灵气
				(this.soulView.soul_txt_pro5 as TextField).text= soulVO.physical.toString();//体力
				(this.soulView.soul_txt_pro6 as TextField).text= soulVO.constant.toString();//定力
				(this.soulView.soul_txt_pro7 as TextField).text= soulVO.magic.toString();//身法
				(this.soulView.soul_txt_grow as TextField).htmlText= this.initPercentTxt(soulVO.growPercent);//成长率
				(this.soulView.soul_txt_compose as TextField).htmlText= this.initLevelTxt(soulVO.composeLevel);//合成等级
					
			}
			else
			{
				if(soulStyleMc)
				{
					if(soulStyleMc.hasEventListener(MouseEvent.CLICK))
					{
						soulStyleMc.removeEventListener(MouseEvent.CLICK,onMoseClick);
					}
					soulStyleMc.parent.removeChild(soulStyleMc);
					soulStyleMc = null;
				}
				(this.soulView.soul_txt_belong as TextField).text= "" ;//类型(力量型，灵力型)
				(this.soulView.soul_txt_style as TextField).text= GameCommonData.wordDic[ "often_used_none" ];//"无"
				(this.soulView.soul_txt_life as TextField).text= "0";//寿命
				(this.soulView.soul_txt_level as TextField).text= "0";//等级
				(this.soulView.soul_txt_exp as TextField).text= "0";//经验
				(this.soulView.soul_txt_pro1 as TextField).text= "0";//外功
				(this.soulView.soul_txt_pro2 as TextField).text= "0";//内功
				(this.soulView.soul_txt_pro3 as TextField).text= "0";//力量
				(this.soulView.soul_txt_pro4 as TextField).text= "0";//灵气
				(this.soulView.soul_txt_pro5 as TextField).text= "0";//体力
				(this.soulView.soul_txt_pro6 as TextField).text= "0";//定力
				(this.soulView.soul_txt_pro7 as TextField).text= "0";//身法
				(this.soulView.soul_txt_grow as TextField).text= "0";//成长率
				(this.soulView.soul_txt_compose as TextField).text= "0";//合成等级 
			}
			
		}
		
		private function onGridMouseOver(me:MouseEvent):void
		{
			var dis:MovieClip = me.target as MovieClip;
			redFrameShap.graphics.clear();
			redFrameShap.graphics.lineStyle(1,0xFF0000);
			redFrameShap.graphics.drawRect(dis.x,dis.y,dis.width,dis.height);
			redFrameShap.graphics.endFill(); 
			this.soulView.addChild(redFrameShap);
			dis.addEventListener(MouseEvent.MOUSE_OUT,onGridMouseOut);
		}
		private function onGridMouseOut(me:MouseEvent):void
		{
			(me.target as MovieClip).removeEventListener(MouseEvent.MOUSE_OUT,onGridMouseOut); 
			redFrameShap.graphics.clear();
			if(redFrameShap.parent)
			{
				redFrameShap.parent.removeChild(redFrameShap);
			}
		}
		private function onLearnMouseOut(me:MouseEvent):void
		{
			(me.target as DisplayObject).filters = null;
			(me.target as DisplayObject).removeEventListener(MouseEvent.MOUSE_OUT,onLearnMouseOut);
		}
		private function onLearnClick(me:MouseEvent):void
		{
			
			var data:Array = me.target.name.split("_");
			var extArr:Array = soulVO.extProperties;
			switch(data[1])
			{
				case "hasLearn":
					if((extArr[int(data[2])] as SoulExtPropertyVO).state == 0)
					{
						sendNotification(ImproveExtendProMediator.SHOWVIEW,int(data[2]));
					}
				break;
				case "upProperty":
					if((extArr[int(data[2])] as SoulExtPropertyVO).state == 0)
					{
						sendNotification(ImproveExtendProMediator.SHOWVIEW,int(data[2]));
					}
				break;
				case "canLearn":
					if((extArr[int(data[2])] as SoulExtPropertyVO).state == 1)
					{
						sendNotification(LearnExtendProMediator.SHOWVIEW,int(data[2]));
					}
				break;
				case "canUseToLearn":
					if((extArr[int(data[2])] as SoulExtPropertyVO).state == 2)
					{
						sendNotification(UseExtendGrooveMediator.SHOWVIEW,int(data[2]));
					}
				break;
				case "skill":
					if(soulVO.soulSkills[int(data[2])] == false)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_soulM_onL" ], color:0xffff00});//"您当前魂魄的等级不够"
					}
					else if((soulVO.soulSkills[int(data[2])] as SoulSkillVO).state == 0) //技能可升级
					{
						sendNotification(ImproveSkillMediator.SHOWVIEW,int(data[2]));
					}
					else if((soulVO.soulSkills[int(data[2])] as SoulSkillVO).state == 1) //技能可学习
					{
						sendNotification(LearnSoulSkillMediator.SHOWVIEW,int(data[2]));
					}
				break;
			}
		}
		
		
		private function onMoseClick(me:MouseEvent):void
		{
			var tName:String = (me.target as DisplayObject).name;
			switch(tName)
			{
				case "hero_16":
					if(this.useItem!=null && this.soulView.hero_16.contains(this.useItem)){
						
						
						this.soulView.hero_16.removeChild(this.useItem);
						this.useItem=null;
						sendNotification(EventList.BAGITEMUNLOCK, this.useItem.Id);
					}
				break;
				case "btn_soulHelp":
					if(GameCommonData.GameInstance.GameUI.contains(this.soulView)) 
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_soulM_onM_1" ], color:0xffff00});//"功能暂未开放"

					}
					
				break;
				case "btn_repeaProperty":
					sendNotification(RepeatExtendProMediator.SHOWVIEW);
				break;
				case "btn_repeatSkill":
					var boo:Boolean;
					for each(var data:* in soulVO.soulSkills)
					{
						if(data is SoulSkillVO)
						{
							if((data as SoulSkillVO).state == 0)
							{
								boo = true;
								break;
							}
						}
					}
					if(boo)
					{
						sendNotification(RepeatSkillMediator.SHOWVIEW);
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_soul_med_soulM_onM_2" ], color:0xffff00});//"您当前没有魂魄技能"
					}
				break;
				case "btn_repeaProperty":
					sendNotification(RepeatSkillMediator.SHOWVIEW);
				break;
				case "soul_style":
					sendNotification(RepeatStyleMediator.SHOWVIEW);
				break;
				case "btn_grow":
					sendNotification(GrowUpPercentMediator.SHOWVIEW);
				break;
				case "btn_noSoulCompose":
					sendNotification(ComposeSoulMediator.SHOWVIEW);
					if(!dataProxy.BagIsOpen)
					{
						sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "bag");
						facade.sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
//						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
				case "btn_compose":
					sendNotification(ComposeSoulMediator.SHOWVIEW);
					if(!dataProxy.BagIsOpen)
					{
						sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "bag");
						facade.sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
//						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
				break; 
				case "btn_showGround":
					if(tempShowStyle != 1)
					{
						tempShowStyle = 1; 
						showComposeSoulModel(1);
					}
				break;
				case "btn_showWater":
					if(tempShowStyle != 2)
					{
						tempShowStyle = 2;
						showComposeSoulModel(2);
					}
				break;
				case "btn_showFire":
					if(tempShowStyle != 3)
					{
						tempShowStyle = 3;
						showComposeSoulModel(3);
					}
				break;
				case "btn_showWind":
					if(tempShowStyle != 4)
					{
						tempShowStyle = 4;
						showComposeSoulModel(4);
					}
				break;
				default:
				break;
			}
		}
		
		private function onSoulLink(me:TextEvent):void
		{
			sendNotification(EventList.SHOWMARKETVIEW);
		}
		
		public function gc():void
		{
			if(redFrameShap)
			{
				redFrameShap.graphics.clear();
				if(redFrameShap.parent)
				{
					redFrameShap.parent.removeChild(redFrameShap);
				}
			}
			removeEventListeners();
			showSoulComponent.deleteComposeView();
			showSoulComponent.deleteView();
			redFrameShap = null;
		}
	}
}