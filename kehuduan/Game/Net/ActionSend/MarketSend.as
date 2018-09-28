package Net.ActionSend
{
	import GameUI.Modules.Maket.Data.MarketConstData;
	
	import Net.ActionProcessor.MarketAction;
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class MarketSend
	{
		public static function createMsg(obj:Object):void
		{
			var goods:Array = MarketConstData.shopCarData;
			//循环发送购买商品命令
			for(var i:int = 0; i < goods.length; i++)
			{
				var good:Object = goods[i];
				var itemByteArray:ByteArray = new ByteArray();
				itemByteArray.endian = Endian.LITTLE_ENDIAN;
				
				var m_byteArr:ByteArray = new ByteArray();
				m_byteArr.endian = Endian.LITTLE_ENDIAN;
				
				itemByteArray.writeUnsignedInt(good.type);					//物品ID
				itemByteArray.writeUnsignedInt(good.buyNum);				//购买数量
				itemByteArray.writeUnsignedInt(good.payWay + 1);			//支付方式,1,元宝；2,珠宝;3,点券
				var nSize:uint = itemByteArray.length;
				
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);					//附加一下消息大小和类型，方便与账号服务器交互
				
				m_byteArr.writeUnsignedInt(0);								//元宝(未用)
				m_byteArr.writeUnsignedInt(0);								//AccountID(未用)
				m_byteArr.writeUnsignedInt(GameCommonData.Player.Role.Id);	//PlayerID
				m_byteArr.writeShort(MarketAction.MARKET_BUY);				//action
				m_byteArr.writeShort(1);									//itemAmount购物车的物品数量
				
				if(nSize>0)
				{
					m_byteArr.writeBytes(itemByteArray,0,nSize);
				}
				
				GameCommonData.GameNets.Send(m_byteArr);
			}
		}
		
		/** 新版本购买商品，添加打折商品action */
		public static function newCreateMsg( obj:Object ):void
		{
			var goods:Array = MarketConstData.shopCarData;
			//循环发送购买商品命令
			for(var i:int = 0; i < goods.length; i++)
			{
				var good:Object = goods[i];
				var itemByteArray:ByteArray = new ByteArray();
				itemByteArray.endian = Endian.LITTLE_ENDIAN;
				
				var m_byteArr:ByteArray = new ByteArray();
				m_byteArr.endian = Endian.LITTLE_ENDIAN;
				
				itemByteArray.writeUnsignedInt(good.type);					//物品ID
				itemByteArray.writeUnsignedInt(good.buyNum);				//购买数量
				itemByteArray.writeUnsignedInt(good.payWay + 1);			//支付方式,1,元宝；2,珠宝;3,点券
				var nSize:uint = itemByteArray.length;
				
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);					//附加一下消息大小和类型，方便与账号服务器交互
				
				m_byteArr.writeUnsignedInt(0);								//元宝(未用)
				m_byteArr.writeUnsignedInt(0);								//AccountID(未用)
				m_byteArr.writeUnsignedInt(GameCommonData.Player.Role.Id);	//PlayerID
				if ( good.leftNum != undefined )
				{
					m_byteArr.writeShort( 1011 );				//action
				}
				else
				{
					m_byteArr.writeShort(MarketAction.MARKET_BUY);				//action
				}
//				m_byteArr.writeShort(MarketAction.MARKET_BUY);				//action
				m_byteArr.writeShort(1);									//itemAmount购物车的物品数量
				
				if(nSize>0)
				{
					m_byteArr.writeBytes(itemByteArray,0,nSize);
				}
				
				GameCommonData.GameNets.Send(m_byteArr);
			}
		}
		
		/** 新版本购买商品，添加打折商品action */
		public static function buyDiscoutProduct( obj:Object ):void
		{
			//循环发送购买商品命令
				var itemByteArray:ByteArray = new ByteArray();
				itemByteArray.endian = Endian.LITTLE_ENDIAN;
				
				var m_byteArr:ByteArray = new ByteArray();
				m_byteArr.endian = Endian.LITTLE_ENDIAN;
				
				itemByteArray.writeUnsignedInt(obj.type);					//物品ID
				itemByteArray.writeUnsignedInt(obj.buyNum);				//购买数量
				itemByteArray.writeUnsignedInt(obj.payWay + 1);			//支付方式,1,元宝；2,珠宝;3,点券
				var nSize:uint = itemByteArray.length;
				
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);					//附加一下消息大小和类型，方便与账号服务器交互
				
				m_byteArr.writeUnsignedInt(0);								//元宝(未用)
				m_byteArr.writeUnsignedInt(0);								//AccountID(未用)
				m_byteArr.writeUnsignedInt(GameCommonData.Player.Role.Id);	//PlayerID
				if ( !isSpecialGood(obj) && obj.leftNum != undefined )
				{
					m_byteArr.writeShort( 1011 );				//action
				}
				else
				{
					m_byteArr.writeShort(MarketAction.MARKET_BUY);				//action
				}
//				m_byteArr.writeShort(MarketAction.MARKET_BUY);				//action
				m_byteArr.writeShort(1);									//itemAmount购物车的物品数量
				
				if(nSize>0)
				{
					m_byteArr.writeBytes(itemByteArray,0,nSize);
				}
				
				GameCommonData.GameNets.Send(m_byteArr);
		}
		
		public static function isSpecialGood(obj:Object):Boolean
		{
			var i:int;
			for( i = 0; i < MarketConstData.specialGoods.length ; i++)
			{
				if(MarketConstData.specialGoods[i].type == obj.type) return true;
			}
			return false;
		}
		
		/** 查询打折商品 */
		public static function inquiresDiscount():void
		{
			var goods:Array = MarketConstData.shopCarData;
			//循环发送购买商品命令
//			for(var i:int = 0; i < goods.length; i++)
//			{
//				var good:Object = goods[i];
				var itemByteArray:ByteArray = new ByteArray();
				itemByteArray.endian = Endian.LITTLE_ENDIAN;
				
				var m_byteArr:ByteArray = new ByteArray();
				m_byteArr.endian = Endian.LITTLE_ENDIAN;
				
				itemByteArray.writeUnsignedInt(0);					//物品ID
				itemByteArray.writeUnsignedInt(0);				//购买数量
				itemByteArray.writeUnsignedInt(0);			//支付方式,1,元宝；2,珠宝;3,点券
				var nSize:uint = itemByteArray.length;
				
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);
				m_byteArr.writeShort(24+nSize);
				m_byteArr.writeShort(Protocol.MSG_MARKET);					//附加一下消息大小和类型，方便与账号服务器交互
				
				m_byteArr.writeUnsignedInt(0);								//元宝(未用)
				m_byteArr.writeUnsignedInt(0);								//AccountID(未用)
				m_byteArr.writeUnsignedInt(GameCommonData.Player.Role.Id);	//PlayerID
				m_byteArr.writeShort( 314 );				//action 314为查询打折商品
				m_byteArr.writeShort(0);									//itemAmount购物车的物品数量
				
				if(nSize>0)
				{
					m_byteArr.writeBytes(itemByteArray,0,nSize);
				}
				
				GameCommonData.GameNets.Send(m_byteArr);
//			}
		}
		
		
		/** 购买单个商品 */
		public static function buyItem(obj:Object):void
		{
			var itemByteArray:ByteArray = new ByteArray();
			itemByteArray.endian = Endian.LITTLE_ENDIAN;
			
			var m_byteArr:ByteArray = new ByteArray();
			m_byteArr.endian = Endian.LITTLE_ENDIAN;
			
			itemByteArray.writeUnsignedInt(obj.type);					//物品ID
			itemByteArray.writeUnsignedInt(obj.count);					//购买数量
			itemByteArray.writeUnsignedInt(obj.payType + 1);			//支付方式,1,元宝；2,珠宝;3,点券
			var nSize:uint = itemByteArray.length;
			
			m_byteArr.writeShort(24+nSize);
			m_byteArr.writeShort(Protocol.MSG_MARKET);
			m_byteArr.writeShort(24+nSize);
			m_byteArr.writeShort(Protocol.MSG_MARKET);					//附加一下消息大小和类型，方便与账号服务器交互
			
			m_byteArr.writeUnsignedInt(0);								//元宝(未用)
			m_byteArr.writeUnsignedInt(0);								//AccountID(未用)
			m_byteArr.writeUnsignedInt(GameCommonData.Player.Role.Id);	//PlayerID
			m_byteArr.writeShort(obj.action);							//action
			m_byteArr.writeShort(1);									//itemAmount购物车的物品数量
			
			if(nSize>0)
			{
				m_byteArr.writeBytes(itemByteArray,0,nSize);
			}
			
			GameCommonData.GameNets.Send(m_byteArr);
		}
		
	}
}