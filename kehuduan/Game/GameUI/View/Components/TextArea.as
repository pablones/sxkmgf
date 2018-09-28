package GameUI.View.Components
{
	import flash.text.*;
	
	public class TextArea extends UIScrollPane
	{
		private var tf:TextField;
		
		public function TextArea()
		{
			tf = new TextField();
            tf.multiline = true;
            tf.wordWrap = true;
            super(tf);
            this.scrollStep = 1;
            tf.mouseWheelEnabled = true;
            setPaddings({left:tf.x, right:tf.y, top:tf.width, bottom:tf.height});
		}
		
		public function set htmlText(str:String) : void
        {
            tf.htmlText = str;
            refresh();
        }

        public function set text(str:String) : void
        {
            tf.text = str;
            refresh();
        }

        public function get textField():TextField
        {
            return tf;
        }
	}
}