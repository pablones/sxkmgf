package Net.ActionProcessor
{
	import GameUI.Modules.GotoCopy.Mediator.GotoCopyMediator;
	import GameUI.Modules.GotoCopy.data.GotoCopyCommand;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class GotoCopyAction extends GameAction
	{
		public function GotoCopyAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		/**
		 * 副本判断
		 * @param bytes
		 * 
		 */		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			var amount:uint = bytes.readUnsignedInt();					//amount
			var copyInfo:Array = [];
			
			for ( var i:uint=0; i<amount; i++ )
			{
				var szName:String = bytes.readMultiByte(16,GameCommonData.CODE);
				var nCondition:uint = bytes.readUnsignedInt();
				copyInfo.push({name:szName,condition:nCondition});
			}
			
			if(!facade.hasMediator(GotoCopyMediator.NAME))
			{
				facade.registerMediator(new GotoCopyMediator());
			}
			facade.sendNotification(GotoCopyCommand.OPEN_GOTOCOPY_VIEW,copyInfo);
		}
	}
}