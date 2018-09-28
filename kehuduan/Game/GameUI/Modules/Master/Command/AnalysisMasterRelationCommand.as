package GameUI.Modules.Master.Command
{
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.OldMaster;
	import GameUI.Modules.Master.Proxy.ServerMasterRelation;
	import GameUI.Modules.Master.Proxy.YoungStudent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//师徒关系分析加工处理
	public class AnalysisMasterRelationCommand extends SimpleCommand
	{
		public static const NAME:String = "AnalysisMasterRelationCommand";
		
		public function AnalysisMasterRelationCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var allRelation:Array = notification.getBody() as Array;
			var mySelfRelation:ServerMasterRelation;
			var master:OldMaster;
			var aStudents:Array = [];
			var student:YoungStudent;
			var serverRelation:ServerMasterRelation;
			
			for ( var i:uint=0; i<allRelation.length; i++ )
			{
				serverRelation = allRelation[i];
				switch ( serverRelation.relation )
				{
					case 0:				//是自己
						mySelfRelation = new ServerMasterRelation();
						mySelfRelation.chuanShouDu = serverRelation.chuanShouDu;			//传授度
						mySelfRelation.viceJob = serverRelation.viceJob;				                //师德
						mySelfRelation.mainJob = serverRelation.mainJob;							//出徒数量
						mySelfRelation.roleLevel = serverRelation.roleLevel;							//是否登记过，0和1  
					break;
					case 1:				//是师傅
						master = new OldMaster();
						master.name = serverRelation.name;
						master.line = serverRelation.line;
						master.roleLevel = serverRelation.roleLevel;
						master.mainJob = serverRelation.mainJob;
						master.mainJobLevel = serverRelation.mainJobLevel;
						master.id = serverRelation.id;
						master.viceJob = serverRelation.viceJob;
						master.viceJobLevel = serverRelation.viceJobLevel;
						master.impart = serverRelation.chuanShouDu;
						master.face = serverRelation.face;
					break;
					case 2:				// 是徒弟
						student = new YoungStudent();
						student.id = serverRelation.id;
						student.name = serverRelation.name;
						student.line = serverRelation.line;
						student.face = serverRelation.face;
						student.roleLevel = serverRelation.roleLevel;
						student.hasTeam = serverRelation.hasTeam;
						student.mainJob = serverRelation.mainJob;
						student.mainJobLevel = serverRelation.mainJobLevel;
						student.viceJob = serverRelation.viceJob;
						student.viceJobLevel = serverRelation.viceJobLevel;
						student.impart = serverRelation.chuanShouDu;
						aStudents.push( student );
					break;
				}
			} 
			aStudents.sortOn( "line",Array.NUMERIC );
			
			var obj:Object = new Object();
			obj.mySelfRelation = mySelfRelation;
			obj.master = master;
			obj.aStudents = aStudents;
			sendNotification( MasterData.REC_MAS_STU_INFO,obj );
			
			gc();
		}
		
		private function gc():void
		{
			facade.removeCommand( NAME );
		}
		
	}
}