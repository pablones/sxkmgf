package GameUI.Modules.NPCChat.View
{
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.View.Components.UISprite;
	
	import OopsEngine.Graphics.Font;
	
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.utils.Dictionary;

	public class NPCDialogPanel extends UISprite
	{
		
		public static const UPDATE_DES_TEXT:String="updateaDesText";
		public static const UPDATE_CELL_TEXT:String="updateCellText";
		public static const UPDATE_ALL:String="updateAll";
		
		protected var describeTf:TextField;
		protected var updateHash:Dictionary;
		protected var cells:Array=[];
		private var colorDic:Dictionary;
		/** 描述文字*/
		private var _desStr:String;
		/** [{iconUrl:XXX,showText:XXX,linkText:XXX},{},....]*/
		private var _dataProvider:Array;
		
		public var onLinkClick:Function;
		
		public function NPCDialogPanel()
		{
			super();
			this.colorDic=new Dictionary();
			this.colorDic[0]='#ffffff';
			this.colorDic[1]='#ff0000';
			this.colorDic[2]='#00ff00';
			this.colorDic[3]='#00ffff';
			this.colorDic[4]='#ffff00';
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.describeTf=new TextField();
			this.describeTf.width=210;
			this.describeTf.multiline=true;
			this.describeTf.type=TextFieldType.DYNAMIC;
			describeTf.wordWrap = true;
			this.describeTf.autoSize=TextFieldAutoSize.LEFT;
			this.describeTf.filters=Font.Stroke();
			this.describeTf.defaultTextFormat=this.getTextFormat();
			this.describeTf.selectable=false;
			this.describeTf.mouseEnabled=false;
			this.addChild(this.describeTf);
			if(this.desStr!=null && this.desStr!=""){
				this.describeTf.htmlText=this.desStr;
			}
			if(this._dataProvider!=null && this._dataProvider.length>0){
				this.createCells();
			}
			this.updateHash=new Dictionary();
			this.doLayout();
		}
		
		protected function getTextFormat():TextFormat{
			var tf:TextFormat=new TextFormat("宋体",12);
			tf.leading=5;
			return tf;
		}
		
		
		
		
		protected function toRepaint():void{
			var tempUpdateHash:Dictionary=this.updateHash;
			this.updateHash=new Dictionary();
			if(tempUpdateHash[UPDATE_ALL]){
				tempUpdateHash[UPDATE_DES_TEXT]=true;
				tempUpdateHash[UPDATE_CELL_TEXT]=true;
			}
			if(tempUpdateHash[UPDATE_DES_TEXT]){
				this.describeTf.htmlText=this.desStr;
			}
			
			if(tempUpdateHash[UPDATE_CELL_TEXT]){
				this.removeAll();
				this.createCells();
			}
			
			this.doLayout();
			
		}
		
		protected function removeAll():void{
			for each(var cell:LinkCell in this.cells){
				if(this.contains(cell)){
					this.removeChild(cell);
				}
			}
			cells=[];
		}
		
		protected function doLayout():void{
			this.describeTf.x=0;
			this.describeTf.y=5;
			
			var currentY:Number=this.describeTf.height+20;
			for each(var cell:LinkCell in this.cells){
				cell.y=currentY;
				currentY+=cell.height+5;
			}
			this.width=220;
			this.height=currentY+20;
		}
		
		protected function createCells():void{
			DialogConstData.finishTask = new Array();
			DialogConstData.unAcceptTask = new Array();
			for each(var obj:Object in this._dataProvider){
				var cell:LinkCell=new LinkCell(obj.iconUrl,obj.linkText,obj.showText,obj.linkColor);
				cell.onLinkClick=this.onLinkClick;
				cells.push(cell);
				this.addChild(cell);
				if( obj.iconUrl == "symbol_finish" )
				{
					DialogConstData.finishTask.push( cell );
				}
				if( obj.iconUrl == "symbol_unAccpet" )
				{
					DialogConstData.unAcceptTask.push( cell );
				}
			}		
		}
		
		public function set desStr(value:String):void{
			this._desStr=this.analyse(value);
			updateHash[UPDATE_DES_TEXT]=true;
			this.toRepaint();	
		}
		
		
		
		
		public function get desStr():String{
			return this._desStr;
		}
		
		public function set dataProvider(value:Array):void{
			this._dataProvider=value;
			updateHash[UPDATE_CELL_TEXT]=true;
			this.toRepaint();
		}
		
		
		public function get dataProvider():Array{
			return this._dataProvider;
		}
		
		/**
		 *  对服务端发送过来的文字进行解析
		 * @param des
		 * @return 
		 * 
		 */		
		private function analyse(des:String):String{
			while(des.indexOf("&")!=-1){
				des=des.replace("&"," ");
			}
			var reg:RegExp=/(<\d_.*?>)/g;
			var arr:Array=des.split(reg);
			var len:uint=arr.length;
			var realDes:String="";
			for(var i:uint=0;i<len;i++){
				if(reg.test(arr[i])){
					var tempArr:Array=strToArr(arr[i]);
					var str:String='<font color="'+this.colorDic[tempArr[0]]+'">'+tempArr[1]+'</font>';
					realDes+=str;
				}else{
					realDes+=arr[i];
				}
			}     
			
		
			return realDes;
		}
		
		/**
		 *  将字符串信息切分成数组(首先去掉两边的尖括号) 
		 * @param str
		 * @return 
		 * 
		 */		
		protected function strToArr(str:String):Array{
			var arr:Array=[];
			if(str.length>2){
				str=str.substring(1,str.length-1);
				arr=str.split("_");
				
			}
			return arr;
		}
		
	}
}