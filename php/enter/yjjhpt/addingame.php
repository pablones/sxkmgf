<?php

define('IN_ECS', true);
require('../includes/init.php');

$sid = $_COOKIE['sid'];
$in_time = $_COOKIE['in_time'];

$file_content = "/////////////////"."\r\n";
$file_content .= "username:".$_POST['username']."\r\n";
$file_content .= "userid:".$_POST['userid']."\r\n";
$file_content .="count1"."\r\n";
$file_content .="time:".date("Y-m-d H:i:s",time())."\r\n";
$file_path .= ROOT_PATH."data/crlog/crlog_1.txt";
swritefile($file_path, $file_content,'a+');
//压力控制开关，开服发现数据库压力过大时关闭
if($_CFG['sPower']){
	$sql = "insert into ".TBL_YJ_USERNUMLOG." (`sid`,`server`,`count4`,`time`) values(".$sid.",".$_CFG['s_name'].",1,'".time()."')";
	$yjjhpt_db -> query($sql);	
}
?>