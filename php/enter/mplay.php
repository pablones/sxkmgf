<?php
/**
 * Created by King。L
 * Date: 2011-6-6
 * Time: 14:52:06
 * To change this template use File | Settings | File Templates.
 */

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

/* 载入语言文件 */
require_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/play.php');

$key = USERINFO.$_COOKIE['username'];
$logingdata = $memcached -> get($key);

//$logingdata['4399uid_val'] = authcode($logingdata['4399uid'],'DECODE');

//TODO 判断是否登录处理
if(empty($logingdata['name']) || $logingdata['id'] < 1 ){
    if(!empty($_SERVER['HTTP_COOKIE'])){
        $logfile = ROOT_PATH . 'data/logfile/jump_user_'.date("Ymd").'.txt';
        $logcnt = date("Y-m-d H:i:s")."[".$_SERVER['REMOTE_ADDR']."]\r\n";
        $logcnt .= "    username: ".$logingdata['username']."\r\n";
        $logcnt .= "    userid  : ".$logingdata['userid']."\r\n";
        $logcnt .= "    4399uid : ".$logingdata['4399uid']."\r\n";
        $logcnt .= "    HTTP_COOKIE     :  ".$_SERVER['HTTP_COOKIE']."\r\n";
        $logcnt .= "    HTTP_HOST       :  ".$_SERVER['HTTP_HOST']."\r\n";
        $logcnt .= "    HTTP_USER_AGENT :  ".$_SERVER['HTTP_USER_AGENT']."\r\n";
        $logcnt .= "--------------------------------------------\r\n\r\n";
        swritefile($logfile,$logcnt,'a');
    }
    $jumpurl = $_CFG['serverwebpage'] . "?msg=please_login_first";
    jump($jumpurl,$_LANG['play_msg4'].$_CFG['qq'].$_LANG['play_msg5']);
    exit();
}

$verfile = ROOT_PATH.'data/upload/ver.txt';
if(!file_exists($verfile)) swritefile($verfile,date("Ymd").rand(1000,9999));
$ver = shtmlspecialchars( trim( sreadfile($verfile) ) );

$serverid = $_CFG['fuwuqi_id'];

//防沉迷处理

if(!$logingdata){
	$sql = "select `fcm` from ".TBL_ACCOUNT." where `name` = '".$logingdata['username']."'";
	$fcmConfig  = $acc_db -> getOne($sql);
}else{
	$fcmConfig = $logingdata['fcm'];
}



$json = array(
    '4399uid'   => $logingdata['password'],
    'userid'    => $logingdata['id'],
    'username'  => $logingdata['name'],
    'currurl'   => '',
    'language'   => $_CFG['language'],
    'loadNoticeWay'   => $_CFG['loadNoticeWay'],
	'IsTransferScene' => $_CFG['IsTransferScene'],
	'openTreasureStragety' => $_CFG['openTreasureStragety'],
	'isUpdataVersion' => $_CFG['isUpdataVersion'],
	'preventWallowTime'=> $_CFG['preventWallowTime'], //防沉迷定时弹出时间
	'cztype'	=> 0,//$_CFG['cztype'],//$con['cztype'], TODO暂时写死
    'timestamp' => $logingdata['timestamp'],
    'serverid'  => $logingdata['serverid'],
	'SourceURL'	=> $serverArr[$serverid]['SourceURL'],
	'AccSocketIP'=> $serverArr[$serverid]['AccSocketIP'],
	'AccSocketPort'=> $serverArr[$serverid]['AccSocketPort'],
    'swfver'    => $ver,                  //SWF版本
    'rooturi'   =>$_CFG['rooturi'],
	'showAccount' => $_CFG['showAccount'],
	'fcmPower' => $_CFG['fcmPower'], 
	'fcmConfig' => $fcmConfig, 
	'isNew' => 100,
);

$json = json_encode($json);
$swfurl = $serverArr[$serverid]['swfurl']."?v=".$ver;

//* 变量赋值 */
$smarty -> assign('serverwebpage' , $_CFG['serverwebpage']);
$smarty -> assign('serverpaypage' , $_CFG['serverpaypage']);
$smarty -> assign('servernewcard' , $_CFG['servernewcard']);
$smarty -> assign('serverbbs' , $_CFG['serverbbs']);
$smarty -> assign('serveract' ,  $_CFG['serveract']);
$smarty -> assign('cztype',$cztype);
$smarty -> assign('servername' ,$serverArr[$serverid]['servername']);
$smarty -> assign('serverid' , $serverArr[$serverid]['id']);
$smarty -> assign('json',$json);
$smarty -> assign('swfurl',$swfurl);
$smarty->assign('lang',       $_LANG);

$smarty->display('play.dwt');

?>
