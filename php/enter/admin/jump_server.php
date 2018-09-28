<?php
/**
 * 页面跳转
 * @var unknown_type
 */
define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
require(dirname(__FILE__) . '/includes/all_server_num.php');
if($_REQUEST['act'] == 'list'){
	if(admin_priv("view_all",'',false)){
		$all_server_names = $all_server_name;
		$view_all_server = 1;
	}else{		
		$all_server_names = $all_server_name[$_CFG['s_name']];
		$view_all_server = 0;
	}
		
	$url ="http://".$_SERVER['HTTP_HOST'];
	$smarty -> assign('view_all_server' ,$view_all_server);
	$smarty -> assign('url' , $url);
	$smarty -> assign('all_server' , $all_server_names);
	$smarty -> display('jump_server.htm');
}



if($_REQUEST['act'] == 'jump_query'){	
	$servername = $_REQUEST['servername'];
	$fuwuqi = $_REQUEST['fuwuqi'];	
	if($fuwuqi<=$hefu[$servername]&&$fuwuqi%2==0){
		$fuwuqi--;
	}
	$url = $_CFG['apiurl']."getpassword.php?username=".$_SESSION['admin_name']; 
	$row = gethttpcnt($url);
	$row = unserialize($row);

	if(!$row){
		$sql = "select * from ".TBL_ADMIN_USER." where `user_name` = '".$_SESSION['admin_name']."'";
		$row = $acc_db -> getRow($sql);
	}

	$cnt = $_SESSION['admin_name']."跳转后台";
	$sql = "insert into ".TBL_ADMIN_LOG."(`log_time`,`user_id`,`log_info`,`ip_address`)values('".time()."','".$_SESSION[admin_id]."','".$cnt."','".$_SERVER['REMOTE_ADDR']."')";		
	$acc_db -> query($sql);
		
	$url = $pages[$servername]['prev'].$fuwuqi."-".$pages[$servername]['last']."/privilege.php?act=signin&username=".$row['user_name']."&password=".$row['password']."&actions=jump";
	
	$url = "http://" . $url;
	echo "<script language='javascript' type='text/javascript'>"; 
	echo "window.location.href='$url'"; 
	echo "</script>"; 
	exit;
}
?>