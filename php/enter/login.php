<?php
/**
 * User: King.L
 * Date: 2011-6-6
 * Time: 14:51:31
 * To change this template use File | Settings | File Templates.
 */


define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

/* 载入加密类 */
require(dirname(__FILE__) . '/includes/lib_code.php');
/* 载入登录类 */
if(extension_loaded('memcache')){
	require(dirname(__FILE__) . '/class/login_class.php');	
}else{
	require(dirname(__FILE__) . '/class/oldlogin_class.php');
}

/* 载入语言文件 */
require_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/login.php');

    $mysql_server_name="127.0.0.1"; //数据库服务器名称
    $mysql_username="root"; // 连接数据库用户名
    $mysql_password="123456"; // 连接数据库密码
    $mysql_dbname="acc_db"; // 数据库的名字
	$mysql_dbtable_name="account";	//存放用户的表的名字

	$username=$_GET["username"];
	$password=$_GET["password"];

    //连接到数据库
    $conn=mysql_connect($mysql_server_name, $mysql_username, $mysql_password);
	if(!$conn)
	{
		echo("100,database connection error");
		exit();
	}
    mysql_select_db("acc_db",$conn);
	mysql_query("set names gb2312");

    //从表中提取信息的sql语句
    $strsql="select password,name from account where name='$username' ORDER BY `id` DESC";
	// 执行sql查询
    $result=mysql_query($strsql, $conn);
	if(!$result)
	{
		echo("100,database selection error2..");
		exit();
	}

	else
	{
		$rows=mysql_fetch_row($result);
		if(!$rows)
		{
                echo "<SCRIPT LANGUAGE='JavaScript'>";
                echo 'alert("不存在此用户,请注册后再登陆！- -！！！");javascript:history.back(-1)';
                echo "</SCRIPT>";
		exit();
		}
		else if($rows[0]!=$password)
		{
                 echo "<SCRIPT LANGUAGE='JavaScript'>";
                 echo 'alert("帐号密码错误,请重新输入。- -！");javascript:history.back(-1)';
                 echo "</SCRIPT>";
		exit();
			
		}
		else
		{
$data = $_GET;
$data['fcmPower'] = $_CFG['fcmPower'];
$data['modes_key'] = $_CFG['modes_key'];
$data['setserverpage'] = $_CFG['serverwebpage'];
$data['extension'] =  $extension;
$login = new userLogin($data);    
$login -> dealLogin();
$name=$username;
$time=time();	
		}
	}


?>