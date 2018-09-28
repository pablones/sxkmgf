<?php
/**
 * 前台公用文件
 * @author King.L
 * 
 */

if (!defined('IN_ECS'))
{
    die('Hacking attempt');
}

error_reporting(E_ALL);
set_time_limit(40);
if (__FILE__ == '')
{
    die('Fatal error code: 0');
}

/* 取得当前ecshop所在的根目录 */
define('ROOT_PATH', str_replace('includes/init.php', '', str_replace('\\', '/', __FILE__)));



/* 初始化设置 */
@ini_set('memory_limit',          '256M');
@ini_set('session.cache_expire',  3600);
@ini_set('session.use_trans_sid', 0);
@ini_set('session.use_cookies',   1);
@ini_set('session.auto_start',    0);
@ini_set('display_errors',        1);

if (DIRECTORY_SEPARATOR == '\\')
{
    @ini_set('include_path', '.;' . ROOT_PATH);
}
else
{
    @ini_set('include_path', '.:' . ROOT_PATH);
}


require(ROOT_PATH . 'data/db_config.php');


if (defined('DEBUG_MODE') == false)
{
    define('DEBUG_MODE', 0);
}

if (PHP_VERSION >= '5.1' && !empty($timezone))
{
    date_default_timezone_set($timezone);
}

$php_self = isset($_SERVER['PHP_SELF']) ? $_SERVER['PHP_SELF'] : $_SERVER['SCRIPT_NAME'];
if ('/' == substr($php_self, -1))
{
    $php_self .= 'index.php';
}
define('PHP_SELF', $php_self);

require(ROOT_PATH . 'includes/inc_constant.php');
require(ROOT_PATH . 'includes/cls_ecshop.php');
require(ROOT_PATH . 'includes/cls_error.php');
require(ROOT_PATH . 'includes/lib_time.php');
require(ROOT_PATH . 'includes/lib_base.php');
require(ROOT_PATH . 'includes/lib_common.php');
require(ROOT_PATH . 'includes/lib_main.php');
require(ROOT_PATH . 'includes/lib_insert.php');
require(ROOT_PATH . 'includes/lib_goods.php');
require(ROOT_PATH . 'includes/lib_article.php');
require(ROOT_PATH . 'data/upload/serverData.php');

/* 对用户传入的变量进行转义操作。*/
if (!get_magic_quotes_gpc())
{
    if (!empty($_GET))
    {
        $_GET  = addslashes_deep($_GET);
    }
    if (!empty($_POST))
    {
        $_POST = addslashes_deep($_POST);
    }

    $_COOKIE   = addslashes_deep($_COOKIE);
    $_REQUEST  = addslashes_deep($_REQUEST);
}

/* 创建 acc_db 对象 */
$acc = new ECS($acc_db_name);
/* 创建wangpu_db 对象*/
$wangpu = new ECS($wangpu_db_name);

// TODO 以下路径稍后处理
define('DATA_DIR', $acc->data_dir());
define('IMAGE_DIR', $acc->image_dir());
// TODO 创建游戏对象
// TODO 创建关系对象

/* 初始化数据库类 */
require_once ROOT_PATH .'includes/cls_mysql.php';

/* 初始化acc_db */
$acc_db = new cls_mysql($acc_db_host, $acc_db_user, $acc_db_pass, $acc_db_name);
//$acc_db -> set_disable_cache_tables(array($acc->table('sessions'), $acc->table('sessions_data'), $acc->table('cart')));
$acc_db_host = $acc_db_user = $acc_db_pass = $acc_db_name = NULL;
/* 初始化wangpu_db */
$wangpu_db = new cls_mysql($wangpu_db_host, $wangpu_db_user, $wangpu_db_pass, $wangpu_db_name);
$wangpu_db_host = $wangpu_db_user = $wangpu_db_pass = $wangpu_db_name = NULL;
/* 初始化yjjhpt_db */
$yjjhpt_db = new cls_mysql($yjjhpt_db_host, $yjjhpt_db_user, $yjjhpt_db_pass,$yjjhpt_db_name);
$yjjhpt_db_host = $yjjhpt_db_user = $yjjhpt_db_pass = $yjjhpt_db_name = NULL;

/* 初始化memcached */
if(extension_loaded('memcache')){
	$memcached = new Memcache;
	$memcached -> connect($memcahce_host,11211);
	
}

/* 创建错误处理对象 */
//$err = new ecs_error('message.dwt');

/* 载入系统参数 */
$_CFG = load_config();


/* 载入语言文件 */
require(ROOT_PATH . 'languages/' . $_CFG['lang'] . '/common.php');



if (!defined('INIT_NO_USERS'))
{
//   /* 初始化session */
   include(ROOT_PATH . 'includes/cls_session.php');
////
////    // TODO 此处表名后期处理
	$sess = new cls_session($acc_db, $acc->table('sessions'), $acc->table('sessions_data'));
//////    $sess = new cls_session($db, $ecs->table('sessions'), $ecs->table('sessions_data'));
////
	define('SESS_ID', $sess->get_session_id());
}

if (!defined('INIT_NO_SMARTY'))
{
    header('Cache-control: private');
    header('Content-type: text/html; charset='.EC_CHARSET);

    /* 创建 Smarty 对象。*/
    require(ROOT_PATH . 'includes/cls_template.php');
    $smarty = new cls_template;
    //TODO 暂时写死静态模板
    $_CFG['template'] = 'game/';
    //print_r($_CFG);
    $_CFG['cache_time'] = 100;
    $smarty->cache_lifetime = $_CFG['cache_time'];
    $smarty->template_dir   = ROOT_PATH . 'themes/' . $_CFG['template'];
    $smarty->cache_dir      = ROOT_PATH . 'temp/caches';
    $smarty->compile_dir    = ROOT_PATH . 'temp/compiled';

    if ((DEBUG_MODE & 2) == 2)
    {
        $smarty->direct_output = true;
        $smarty->force_compile = true;
    }
    else
    {
        $smarty->direct_output = false;
        $smarty->force_compile = false;
    }

    $smarty->assign('lang', $_LANG);
    $smarty->assign('ecs_charset', EC_CHARSET);
    if (!empty($_CFG['stylename']))
    {
        $smarty->assign('ecs_css_path', 'themes/' . $_CFG['template'] . '/style_' . $_CFG['stylename'] . '.css');
    }
    else
    {
        $smarty->assign('ecs_css_path', 'themes/' . $_CFG['template'] . '/style.css');
    }

}



if ((DEBUG_MODE & 1) == 1)
{
    error_reporting(E_ALL);
}
else
{
    error_reporting(E_ALL ^ E_NOTICE);
}
if ((DEBUG_MODE & 4) == 4)
{
    include(ROOT_PATH . 'includes/lib.debug.php');
}

/* 判断是否支持 Gzip 模式 */
if (!defined('INIT_NO_SMARTY') && gzip_enabled())
{
    ob_start('ob_gzhandler');
}
else
{
    ob_start();
}

?>