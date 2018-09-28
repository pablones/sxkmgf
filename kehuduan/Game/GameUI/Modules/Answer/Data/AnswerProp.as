package GameUI.Modules.Answer.Data
{
	//答题基本数据
	public class AnswerProp
	{
		/**题号*/
		public var serial:int;
		/**关系 0为徒弟，1为师傅*/
		public var relation:int;
		/**题目内容*/
		public var question:String;
		/**选项数组*/
		public var answers:Array = [];
		/**正确答案，从0开始*/
		public var rightAnswer:int;
		/**答案数组随机打乱*/
		public function get randomAnswer():Array
		{
			var arr:Array = answers.concat();
			var b:Array = [];
			while ( arr.length>0)
			{
				var ran:int = Math.floor( Math.random()*( arr.length ) );
				var obj:Object = new Object();
				obj.content = arr.splice( ran,1 )[ 0 ];
				obj.id = answers.indexOf( obj.content );
				b.push( obj );
			}
			return b;
		}
		
		public function get realQuestion():String
		{
			var str:String;
			if ( relation == 0 )
			{
				str = question.replace( "#","你的师傅" );
			}
			else
			{
				str = question.replace( "#","你" );
			}
			return str;
		}
		
	}
}