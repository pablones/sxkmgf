<?php


if(!defined('IN_ZBPHP')) define('IN_ZBPHP', TRUE);

define('D_BUG', '1');

D_BUG ? error_reporting(7):error_reporting(0);
//error_reporting(E_ALL);
set_magic_quotes_runtime(0);
date_default_timezone_set('PRC');

$_SG = $_Cfg = $_CACHE = $_TPL = $_SCOOKIE = $_LANG = array();
$db = NULL;


//程序目录
define('S_ROOT', dirname(__FILE__).DIRECTORY_SEPARATOR);


include_once(S_ROOT.'./source/function_common.php');

/*
    //不输出内容，而是直接跳转到官网首页
    jump("http://web.4399.com/yjjh/?msg=YJJH_Now_Is_Overflow_Please_Try_Login_Again_COMMON","《御剑江湖》服务器爆满，请到官网登录后重试！");
    exit();
    */

include_once(S_ROOT.'./source/function_cache.php');
include_once(S_ROOT.'./serverData.php');

//时间
$mtime = explode(' ', microtime());
$_SG['timestamp'] = $mtime[1];
$_SG['starttime'] = $_SG['timestamp'] + $mtime[0];

//GPC过滤
$magic_quote = get_magic_quotes_gpc();
if(empty($magic_quote))
{
	$_GET = saddslashes($_GET);
	$_POST = saddslashes($_POST);
}

if(!@include_once(S_ROOT.'./config.php')) die('Can\'t find the config file !');


/*
//链接数据库
dbconnect();
*/

if(!@include_once(S_ROOT.'./data/cache_config.php'))
{
  include_once S_ROOT.'./config.php';
  config_cache();
  include_once S_ROOT.'./data/cache_config.php';
  }

//COOKIE
$prelength = strlen($_SC['cookiepre']);
foreach($_COOKIE as $key => $val)
{
	if(substr($key, 0, $prelength) == $_SC['cookiepre'])
	{
		$_SCOOKIE[(substr($key, $prelength))] = empty($magic_quote) ? saddslashes($val) : $val;
	}
}


//启用GIP
if ($_Cfg['gzipcompress'] && function_exists('ob_gzhandler')) {
	ob_start('ob_gzhandler');
} else {
	ob_start();
}

//初始化
$_SG['uid'] = 0;
$_SG['ip'] = getip();
$_SG['username'] = '';
$_SG['inajax'] = empty($_GET['inajax']) ? 0:intval($_GET['inajax']);
//$_SG['authkey'] = md5($_SG['ip'].$_SERVER['HTTP_USER_AGENT']);
$_SG['authkey'] = md5($_SERVER['HTTP_HOST'].AUTH_KEY); //AUTH_KEY密匙

if(empty($_SERVER['HTTP_REFERER'])){
  $_SG['refer'] = $_Cfg['siteurl'];
  }else{
    $urlArr = parse_url($_SERVER['HTTP_REFERER']);
    $_SG['refer'] = $urlArr['host'] == $_SERVER['HTTP_HOST'] ? $_SERVER['HTTP_REFERER']:$_Cfg['siteurl'];
  }


//整站风格
if(empty($_SG['template']))
{
	$_SG['template'] = 'default';
}

//处理REQUEST_URI
if(!isset($_SERVER['REQUEST_URI']))
{
	$_SERVER['REQUEST_URI'] = $_SERVER['PHP_SELF'];
	if(isset($_SERVER['QUERY_STRING'])) $_SERVER['REQUEST_URI'] .= '?'.$_SERVER['QUERY_STRING'];
}


if($_SERVER['REQUEST_URI'])
{
	$temp = urldecode($_SERVER['REQUEST_URI']);
	if(strexists($temp, '<') || strexists($temp, '"'))
	{
		$_GET = shtmlspecialchars($_GET);//XSS
	}
}

$_TPL['css'][] = 'global';

// -----------------------------------------
//连接游戏数据库
function dbconn_game(){
  $GLOBALS['db']->connect(DBGM_DBHOST, DBGM_DBUSER, DBGM_DBPW ,DBGM_DBNAME);
  return;

  if($GLOBALS['db']->result_first("select database();") == DBGM_DBNAME){
    return ;
   }elseif($GLOBALS['db']->result_first("show databases like '".DBGM_DBNAME."' ") == DBGM_DBNAME){
    $GLOBALS['db']->query("use ".DBGM_DBNAME);
   }else{
    $GLOBALS['db']->connect(DBGM_DBHOST, DBGM_DBUSER, DBGM_DBPW ,DBGM_DBNAME);
   }
  }



//连接账号数据库
function dbconn_acc(){
  if($GLOBALS['db']->result_first("select database();") == DBACC_DBNAME){
    return ;
   }elseif($GLOBALS['db']->result_first("show databases like '".DBACC_DBNAME."' ") == DBACC_DBNAME){
    $GLOBALS['db']->query("use ".DBACC_DBNAME);
   }else{
    $GLOBALS['db']->connect(DBACC_DBHOST, DBACC_DBUSER, DBACC_DBPW ,DBACC_DBNAME);
   }
  }

//连接平台数据库
function dbconn_pt(){
  if($GLOBALS['db']->result_first("select database();") == DBPT_DBNAME){
    return ;
   }elseif($GLOBALS['db']->result_first("show databases like '".DBPT_DBNAME."' ") == DBPT_DBNAME){
    $GLOBALS['db']->query("use ".DBPT_DBNAME);
   }else{
    $GLOBALS['db']->connect(DBPT_DBHOST, DBPT_DBUSER, DBPT_DBPW ,DBPT_DBNAME);
   }

   //平台数据库有表前缀
   if(defined('DBPT_TBPRE')) $GLOBALS['_Cfg']['tablepre'] = $GLOBALS['_SC']['tablepre'] = DBPT_TBPRE;
  }
?>