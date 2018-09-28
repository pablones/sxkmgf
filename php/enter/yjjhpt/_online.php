<?php

/**
 * PHP [心跳包] 每隔30秒请求一次
 *
 *
 * 所有游戏页面每隔一定时间,约30秒向服务器发送一次在线请求
 * 不进行数据库检查该用户是否存在!
 *
 *--------------------------------------------------
 * COOKIE各个值的含义
 * [usertm] => 最后请求的时间
 *
 */

//限定POST请求
if($_SERVER['REQUEST_METHOD'] != 'POST' || !isset($_POST['u']) ) exit('403');


require_once './ptcommon.php';
error_reporting(E_ALL);

$ptuser = new pt_user;
$usertm = isset($_SCOOKIE[$ptuser->cookieid]['usertm']) ? intval($_SCOOKIE[$ptuser->cookieid]['usertm']):0;

//无需请求<已经掉线/离线><响应时间小于允许的请求间隔>
if($_SCOOKIE[$ptuser->cookieid]['stat'] == 'lss' || $_SG['timestamp'] - $usertm < PT_USER_TM) sexit('400');

dbconn_pt();

$ptuser->updateLastactive();

//假如无响应,那么检查SESSION
if(!$db->affected_rows()) $ptuser->ckSession();

$db->close();

//设定最后请求时间
ssetcookie( $ptuser->cookieid.'[usertm]',$_SG['timestamp']);

sexit('200');
?>