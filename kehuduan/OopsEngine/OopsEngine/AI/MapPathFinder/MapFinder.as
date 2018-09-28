package OopsEngine.AI.MapPathFinder
{
	import flash.utils.Dictionary;
	
	/** 地图节点搜索  */
	public class MapFinder
	{
		private var mapTree:Dictionary;
		private var start:MapNode;
		private var end:MapNode;
		private var openList:Dictionary;
		private var isFind:Boolean = false;
		private var targetMapId:String;
		
		public function MapFinder(mapTree:Dictionary)
		{
			this.mapTree = mapTree;
		}

		public function Find(currentMapId:String, targetMapId:String) : Array
		{
			this.targetMapId = targetMapId;
			this.openList    = new Dictionary();
			
			this.NoteFinder([currentMapId]);
			
			var path:Array      = [];
			if(this.end!=null)
			{	
				var node:MapNode = this.end;
				while(true)
				{
					if(node!=null)
					{
						path.unshift(node.Id)
						node = this.openList[node.Parent];
						if(node==null || node.Id == currentMapId)
						{
							break;
						}
					}
				}
			}
			return path; 
		}
		
		private function NoteFinder(current:Array):void
		{
			var next:Array = [];
			for each(var node1:String in current)
			{
				var mapNode:String = this.mapTree[node1] as String;
				var nodes:Array;
				if(mapNode!=null)
				{
					nodes = mapNode.split(",");
					
					for each(var node2:String in nodes)
					{
						var n:MapNode = new MapNode();
						n.Id 	      = node2;
						n.Parent      = node1;
						
						if(this.openList[n.Id]==null)
						{
							this.openList[n.Id] = n;
						}
						
						if(n.Id == this.targetMapId)
						{
							this.end = n;
							return;
						}
						else
						{
							next.push(n.Id);
						}
					}
				}
			}
			
			if(next.length > 0)
			{
				this.NoteFinder(next);
			}
		}
	}
}