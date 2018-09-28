<?php
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');

$date=$_GET['date'];
$date=strtotime($date);



$sql="select * from  account where phone>=$date";

$sql1    = "SELECT COUNT(*) from  account  where phone>$date";
$totalnum = $acc_db ->getOne($sql1);
if($totalnum==0){
	echo "No Data!";
	exit;
}

$query = $acc_db ->getAll($sql1);

$path="xml/userlog.xml";     
$dom = new DOMDocument("1.0"); 
$root = $dom->createElement("userlog"); 


$i=1;
foreach ($query as $key => $row){
    $REC[]=$row;
    $id=$i++;
	$tmp=$row['phone'];
	$tmp=date('Y-m-d H:i:s',$tmp);
    $zone=$row['name'];
	$user_id = $row['password'];
	$user=$row['id']; //authcode($row['id'],'DECODE');
	$times=$row['age'];
	$ip=$row['netbar_ip'];
    
	$sql="select * from wb_user ";
	$sql.="where account_id= '$user'";
    $rs=$db->$wangpu_db -> getRow($sql);
    $role=$rs['name'];
    $level=$rs['level'];
		    
    $dom->appendChild($root); 
    $log = $dom->createElement("loginlog");
    $root->appendChild($log);
    // create attribute node 
    $logid = $dom->createAttribute("idx"); 
    $log->appendChild($logid); 
    // create attribute value node 
    $idValue = $dom->createTextNode($id); 
    $logid->appendChild($idValue); 
              
    //时间
    $time = $dom->createElement("logtime"); 
    $log->appendChild($time); 
    $text = $dom->createTextNode($tmp); 
    $time->appendChild($text); 
    // zoneid 
    $line = $dom->createElement("name"); 
    $log->appendChild($line); 
    $text = $dom->createTextNode($zone); 
    $line->appendChild($text); 	
    // userid
    $userid = $dom->createElement("userid"); 
    $log->appendChild($userid); 
    $text = $dom->createTextNode($user_id); 
    $userid->appendChild($text); 
    // times
    $logintimes = $dom->createElement("logintimes"); 
    $log->appendChild($logintimes); 
    $text = $dom->createTextNode($times); 
    $logintimes->appendChild($text);
    // roles
    $roles = $dom->createElement("roles"); 
    $log->appendChild($roles); 
    $text = $dom->createTextNode($role); 
    $roles->appendChild($text);
    // level
    $lv = $dom->createElement("lv"); 
    $log->appendChild($lv); 
    $text = $dom->createTextNode($level); 
    $lv->appendChild($text);
    // loginip
    $loginip = $dom->createElement("loginip"); 
    $log->appendChild($loginip); 
    $text = $dom->createTextNode($ip); 
    $loginip->appendChild($text);
}
    
       $dom->saveXML(); 
       
       $dom->save($path);
     
if($dom->save($path)){
     	echo "update success!";
}
   //print_r($REC); 
?>
