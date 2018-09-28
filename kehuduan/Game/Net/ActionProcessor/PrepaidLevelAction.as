package Net.ActionProcessor
{
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class PrepaidLevelAction extends GameAction
	{
		public function PrepaidLevelAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 8;
			var obj:Object = new Object();
			
			obj.nAction  = bytes.readUnsignedShort();			//action
			switch( obj.nAction )
			{
				case 4:
					obj.arr = new Array();
					bytes.readUnsignedInt();				            //用户ID
					obj.allPrepaid  = bytes.readUnsignedInt();		    //累计元宝总数
					obj.upLevelPrepaid = bytes.readUnsignedInt();		//显示条元宝总数
					obj.needPrepaid = bytes.readUnsignedInt();			//还需充值元宝数
					obj.prepaidLevel = bytes.readUnsignedInt();		    //元宝VIP等级
					var nAmount:uint = bytes.readUnsignedInt();         //掩码
					
					for(var i:uint = 0 ; i < 10 ; i ++)
					{
						if( nAmount & Math.pow(2, i) )
						{
							obj.arr.push( true );
						}
						else
						{
							obj.arr.push( false );
						}
					}
					   
					bytes.readUnsignedInt();      
					bytes.readUnsignedInt();        
					bytes.readUnsignedInt();      
					bytes.readInt();		       
					obj.canGetZhuBao = bytes.readUnsignedInt();        //是否可领取珠宝返还
					sendNotification( PrepaidUIData.UPDATE_PREPAID_VIEW, obj );
					break;
				case 11:
					bytes.readUnsignedInt();				       
					bytes.readUnsignedInt();		  
					bytes.readUnsignedInt();		
					bytes.readUnsignedInt();		
					bytes.readUnsignedInt();		
					bytes.readUnsignedInt();    
					obj.usedYouLiCount = bytes.readUnsignedInt();       //游历次数
					obj.canYouLiCount = bytes.readUnsignedInt();        //每日最高游历次数
					obj.fugueNeedMoney = bytes.readUnsignedInt();       //神游千里所需元宝
					obj.questionIndex = bytes.readInt();		        //问题集id
					bytes.readUnsignedInt();                         
					
					sendNotification( PrepaidUIData.UPDATE_TRAVEL_VIEW, obj );
					break;
			}
		}
		
	}
}