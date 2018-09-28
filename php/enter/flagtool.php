<?php
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

if($_REQUEST['act'] == ''){
	$smarty -> display('flagtool.dwt');
}
if($_REQUEST['act'] == 'query'){
	$userid = $_POST['userid'];
	$username = $_POST['username'];
	$time = $_POST['time'];
	$serverid = $_POST['serverid'];
	$flag = md5($userid.$username.$time.$_CFG['modes_key']);
	
	$smarty -> assign('flag' , $flag);
	$smarty -> assign('view' , 1);
	$smarty -> display('flagtool.dwt');
}

?>