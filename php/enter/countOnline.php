<?php
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');

$sql1    = "SELECT COUNT(*) from acc_linelog order by time desc limit 1";

$totalnum = $acc_db ->getOne($sql1);
if($totalnum==0){
	echo "No Data!";
	exit;
}

$sql="select logid,time,count_online,line from acc_linelog order by time desc limit 0,6";
$rs=$acc_db ->getAll($sql);
	
$path="xml\countonline.xml";     
$dom = new DOMDocument("1.0"); 
$root = $dom->createElement("countOline"); 
$i=1;
if(!empty($rs)){
    foreach ($rs as $key => $row){
	    $id=$i++;
		$tmp=$row['time'];
		$tmp=date('Y-m-d H:i:s',$tmp);
		$line1=$row['line'];
		$count1+=$row['count_online'];
    }
}
$dom->appendChild($root); 
$online = $dom->createElement("online");
$root->appendChild($online);
// create attribute node 
$logid = $dom->createAttribute("idx"); 
$online->appendChild($logid); 
// create attribute value node 
$idValue = $dom->createTextNode('1'); 
$logid->appendChild($idValue); 
              
//时间
$time = $dom->createElement("logtime"); 
$online->appendChild($time); 
$text = $dom->createTextNode($tmp); 
$time->appendChild($text); 
	
// 在线 人数
$count = $dom->createElement("onlinecount"); 
$online->appendChild($count); 
$text = $dom->createTextNode($count1); 
$count->appendChild($text); 	
	
$dom->saveXML(); 
	
	if($dom->save($path)){
     	echo "update success!";
     }

?>
