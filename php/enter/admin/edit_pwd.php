<?php
/** 
 * 全服修改密码
 * @var unknown_type
 */

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
set_time_limit(0);
if($_REQUEST['act'] == 'list'){
	$smarty -> assign('user_name' , $_SESSION['admin_name']);
	$smarty -> display("edit_pwd.htm");
}
if($_REQUEST['act'] == 'edit'){
	$user_name = $_POST['user_name'];
	$old_pwd = md5($_POST['old_pwd']);
	$new_pwd = md5($_POST['new_pwd']);
	$link[] = array('text' => $_LANG['back_edit'], 'href'=>'edit_pwd.php?act=list');

	$url = $_CFG['apiurl']."acceptadmin.php?username=".$user_name."&password=".$new_pwd;
	$result = gethttpcnt($url);
	
	if($result == "ok"){
		$sql = "update ".TBL_ADMIN_USER." set `password` = '".$new_pwd."' where `user_name` = '".$user_name."'";
		$acc_db -> query($sql);
		sys_msg($_LANG['success'],0,$link);
	}else{
		sys_msg($_LANG['err_account'],1,$link);
	}
	
	
}
?>