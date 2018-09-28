<?php
/**
 * 
 * 后台登陆游戏
 * @var unknown_type
 */
define('IN_ECS', true);

/* 代码 */
require(dirname(__FILE__) . '/includes/init.php');
require_once(ROOT_PATH . 'data/upload/serverData.php');


$_COOKIE['4399uid_val'] = $_COOKIE['4399uid'];

$serverid = $_COOKIE['serverid'];
if(!array_key_exists($serverid,$serverArr)) jump($_CFG['serverwebpage']."?msg=serverid_error",$_LANG['server'].$serverid.$_LANG['no_exit']);

$verfile = ROOT_PATH .'data/upload/ver.txt';
if(!file_exists($verfile)) swritefile($verfile,date("Ymd").rand(1000,9999));
$ver = shtmlspecialchars( trim( sreadfile($verfile) ) );

//防沉迷处理
$sql = "select `fcm` from ".TBL_ACCOUNT." where `name` = '".$_COOKIE['username']."'";
$fcmConfig  = $acc_db -> getOne($sql);

$json = array(
    '4399uid'   => $_COOKIE['4399uid_val'],//'139602614',
    'userid'    => $_COOKIE['userid'], //'18502',
    'username'  => $_COOKIE['playername'], //'joe702',
    'currurl'   => '',
	'preventWallowTime'=> 10*1000,
	'cztype'	=> 0,
    'timestamp' => $_COOKIE['timestamp'],
    'serverid'  => $_COOKIE['serverid'],
	'servernum'  => $_COOKIE['servernum'],
	'rolecount' => $_COOKIE['rolecount'],
	'SourceURL'	=> $serverArr[$serverid]['SourceURL'],
	'AccSocketIP'=> $serverArr[$serverid]['AccSocketIP'],
	'AccSocketPort'=> $serverArr[$serverid]['AccSocketPort'],
    'swfver'    => $ver,                  //SWF版本
    'rooturi'   => $_CFG['rooturi'],
	'fcmPower' => $_CFG['fcmPower'], 
	'fcmConfig' => $fcmConfig, 
);

$json = json_encode($json);
$server_id = $serverArr[$serverid]['id'];
$server_name = $serverArr[$serverid]['servername'];
$swfurl = $serverArr[$serverid]['swfurl']."?v=".$ver;

$smarty -> assign('swfurl',$swfurl);
$smarty -> assign('servername',$server_name);
$smarty -> assign('serverid',$server_id);
$smarty -> assign('json',$json);
$smarty -> display('play.htm');
?>
