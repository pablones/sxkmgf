<?php

//配置参数
$_SC = array();
$_SC['dbhost']  		= '127.0.0.1';        //服务器地址 192.168.1.50 115.238.101.102
//$_SC['dbhost']  		= '115.238.101.12'; 
$_SC['dbuser']  		= 'root';   //用户 root server-phpadm
$_SC['dbpw'] 	 		= 'root'; //密码 test
$_SC['dbcharset'] 		= 'utf8';           //字符集
$_SC['pconnect'] 		= 0;                  //是否持续连接
$_SC['dbname']  		= 'acc_db';           //数据库 acc_db
$_SC['tablepre'] 		= '';                 //表名前缀
$_SC['charset'] 		= 'utf-8';            //页面字符集

$_SC['gzipcompress'] 	= 1;                //启用gzip

$_SC['cookiepre'] 		= 'Kv5jT_';         //COOKIE前缀
$_SC['cookiedomain'] 	= '.fy5.com';    //COOKIE作用域
//$_SC['cookiedomain'] 	= '';
$_SC['cookiepath'] 		= '/';              //COOKIE作用路径

$_SC['attachdir']		= './upload/';        //附件本地保存位置(服务器路径, 属性 777, 必须为 web 可访问到的目录, 相对目录务必以 "./" 开头, 末尾加 "/")
$_SC['attachurl']		= 'upload/';          //附件本地URL地址(可为当前 URL 下的相对地址或 http:// 开头的绝对地址, 末尾加 "/")

$_SC['siteurl']			= '';                 //站点的访问URL地址(http:// 开头的绝对地址, 末尾加 "/")，为空的话，系统会自动识别。


$allowIpArrIn = array(
	'122.226.73.169', //账号服务器IP地址
);

define('DBGM_DBHOST','127.0.0.1');     //GAME数据库的HOST
define('DBGM_DBUSER',$_SC['dbuser']);    //GAME数据库的用户
define('DBGM_DBPW',$_SC['dbpw']);        //GAME数据库的密码
define('DBGM_DBNAME','wangpudb');        //GAME数据库的数据库名称

define('DBACC_DBHOST',$_SC['dbhost']);     //账号数据库的HOST
define('DBACC_DBUSER',$_SC['dbuser']);     //账号数据库的用户
define('DBACC_DBPW',$_SC['dbpw']);         //账号数据库的密码
define('DBACC_DBNAME','acc_db');           //账号数据库的数据库名称


define('DBPT_DBHOST',$_SC['dbhost']);     //平台数据库的HOST
define('DBPT_DBUSER',$_SC['dbuser']);     //平台数据库的用户
define('DBPT_DBPW',$_SC['dbpw']);         //平台数据库的密码
define('DBPT_DBNAME','yjjhpt');           //平台数据库的数据库名称
define('DBPT_TBPRE','yjjhpt_');           //平台数据库表前缀


define('AUTH_KEY','X4cf3ZDrRwqEdffEs78Dr45da'); //AUTHKEY -- 用于加密解密
define('QUN_MSG','');

//日期 2010-10-16 游客ID变更为 [1]-[100000],原范围为 [10万]-[20万],<2010-10-17生效>
define('API_YK_MINID',1);       //最小的游客ID
define('API_YK_MAXID',100000);  //最大的游客ID
?>