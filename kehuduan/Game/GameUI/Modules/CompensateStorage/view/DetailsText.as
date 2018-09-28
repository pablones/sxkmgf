package GameUI.Modules.CompensateStorage.view
{
	import GameUI.Modules.Task.View.TaskText;
	
	import flash.text.TextField;

	public class DetailsText extends TaskText
	{
		public function DetailsText(w:Number=215)
		{
			super(w);
		}
		
		public function get textField():TextField
		{
			return this.tf;
		}
	}
}