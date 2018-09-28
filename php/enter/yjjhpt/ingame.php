<?php

define('IN_ECS', true);
require('../includes/init.php');

$in_time = time();
setcookie('in_time',$in_time);   //将进入页面的时间存入cookie

$http=trim($_SERVER['HTTP_HOST']);
$firstappear = stripos($http,"-");
if(($firstappear-1) >= 0){
	$sid = $http[$firstappear-1];
}else{
	$sid = 1;
}
setcookie('sid' , $sid); //将服务器编号存入cookie
$file_content = "/////////////////"."\r\n";
$file_content .= "username:".$_POST['username']."\r\n";
$file_content .= "userid:".$_POST['userid']."\r\n";
$file_content .="count4"."\r\n";
$file_content .="time:".date("Y-m-d H:i:s",time())."\r\n";
$file_path .= ROOT_PATH."data/crlog/crlog_4.txt";
swritefile($file_path, $file_content,'a+');
if($_CFG['sPower']){
	$sql = "insert into ".TBL_YJ_USERNUMLOG." (`sid`,`server`,`count1`,`time`) values(".$sid.",".$_CFG['s_name'].",1,'".time()."')";
	$yjjhpt_db -> query($sql);
}


?>